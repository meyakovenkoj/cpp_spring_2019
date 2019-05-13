#include "thread-sort.h"

int main(int argc, char ** argv){	
	try{
		if(argc == 2){
			Thread_sort thr(6 * 1024 * 1024, argv[1]);
			thr.sort();
		}
		else
			throw std::invalid_argument("incorrect number of arguments");
	}
	catch(std::logic_error err){
		std::cerr << err.what() << std::endl;
	}
	return 0;
}
