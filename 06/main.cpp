#include "thread-sort.h"

int main(int argc, char ** argv){	
	try{
		if(argc == 3){
			Thread_sort thr(6 * 1024 * 1024, argv[1]);
			thr.sort();
			Thread_sort thr2(6 * 1024 * 1024, argv[2]);
			thr2.sort();

		}
		else
			throw std::invalid_argument("incorrect number of arguments");
	}
	catch(const std::logic_error & err){
		std::cerr << err.what() << std::endl;
	}
	catch(...){
		std::cerr << "unknown error" << std::endl;
	}
	return 0;
}
