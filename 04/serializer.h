#pragma once
#include <iostream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
public:
	explicit Serializer(std::ostream& out) : out_(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&& ... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
private:
	static constexpr char Separator = ' ';
	std::ostream & out_;
	
	Error process(bool val)
	{
		out_ << (val ? "true" : "false") << Separator;
		return Error::NoError;
	}
	
	Error process(uint64_t val)
	{
		out_ << val << Separator;
		return Error::NoError;
	}
	
	template<class T>
	Error process(T&& value)
	{
		return Error::CorruptedArchive;
	}
	
	template<class T, class... ArgsT>
	Error process(T&& value, ArgsT&& ... args)
	{
		if (process(std::forward<T>(value)) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT>(args)...);
		}
	}
};

class Deserializer
{
public:
	explicit Deserializer(std::istream& in) : in_(in)
	{
	}
	
	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}
	
	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
private:
	static constexpr char Separator = ' ';
	std::istream & in_;
	
	Error process(bool & val)
	{
		std::string input;
		in_ >> input;
		
		if(!input.compare("true"))
			val = true;
		else if(!input.compare("false"))
			val = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}
	
	Error process(uint64_t & val)
	{
		std::string input;
		in_ >> input;
		val = 0;
		
		for(auto buf: input)
		{
			if(buf >= '0' && buf <= '9'){
				val *= 10;
				val += buf - '0';
			}
			else
				return Error::CorruptedArchive;
		}
		return Error::NoError;
	}
	
	template<class T, class... ArgsT>
	Error process(T&& value, ArgsT&&... args)
	{
		if (process(std::forward<T>(value)) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT>(args)...);
		}
	}
};
