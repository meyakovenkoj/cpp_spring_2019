#include "numbers.dat"
#include <iostream>
#include <math.h>

bool prime(int n) {
	if(n != 1 && n != 2 && n%2 != 0) {
		int root = sqrt(n);
		for(int i = 3; i <= root; i+=2)
			if(n%i == 0)
				return false;
	}else if(n==1 || (n != 2 && n%2 == 0))
		return false;
	return true;
}

int main(int argc, char* argv[]){
	if (argc == 1 || !(argc % 2))
		return -1;
	int amount_arg = 1;
	int count,left,right;
	while (amount_arg < argc) {
		count = 0;
		left = std::atoi(argv[amount_arg]);
		right = std::atoi(argv[++amount_arg]);
		for(int i = 0; i < Size && Data[i] <= right; i++) {
			if(Data[i] >= left && prime(Data[i])) {
				count++;
				while(i < Size-1 && Data[i+1] == Data[i]){
					i++;
					count++;
				}
			}
			else while(i < Size-1 && Data[i+1] == Data[i])
				i++;
		}
		std::cout << count << std::endl;
		amount_arg++;
	}
	return 0;
}
