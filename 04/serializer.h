#pragma once

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
		: out_(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}
	
private:
	
};

class Deserializer
{
	static constexpr char Separator = ' ';
public:
	explicit Deserializer(std::istream& in)
		: in_(in)
	{
	}

	template <class T>
	Error load(T& object)
	{
		return object.deserialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}
	
private:
	// process использует variadic templates
};

Error stouint(std::string & input, uint64t & res)
{
	char buf;
	for(size_t i = 0; i < input.length(); i++)
	{
		buf = input[i];
		if(buf >= '0' && buf <= '9'){
			res *= 10;
			res += buf - '0';
		}
		else
		{
			res = 0;
			return Error::CorruptedArchive;
		}
	}
	return Error::NoError;
}

void uinttos(std::string & output, uint64t & val)
{
	output = std::to_string(val);
}

Error stobool(std::string & input, bool & res)
{
	if(!input.compare("true"))
		res = true;
	else if(!input.compare("false"))
		res = false;
	else
		return Error::CorruptedArchive;
	return Error::NoError;
}

void booltos(std::string & output, bool & val)
{
	if(val)
		output = "true";
	else
		output = "false";
}

Error split_str(std::string & line, std::vector<std::string> & vect)
{
	// int i = 0;
	// stringstream ssin(line);
	// std::string buf;
	// while (ssin.good() && !ssin.eof()){
	//	 ssin >> buf;
	//	 vect(buf);
	//	 buf.clear();
	//	 ++i;
	// }
	std::string buf;
	while(line[i] != '\0')
	{
		while(line[i] != Separator)
		{
			//add(line[i])
			buf += line[i];
			i++;
		}
		vect.push_back(buf);
		buf.clear();
		i++;
		j++;
	}
	if (i != 3)
		return Error::CorruptedArchive;
	else
		return Error::NoError;
}

Error load_obj(T& object)
{
	std::vector<std::string> vect;
	if(split_str(vect) == Error::NoError)
	{
		try
		{
			for(int i = 0; i < vect.size(); i++)
				if(vect[i][0] >= '0' && vect[i][0] <= '9')
					stouint(vect[i]);
				else
					stobool(vect[i]);
		}
		catch(Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
	}
	return Error::NoError;
}

Error save()
