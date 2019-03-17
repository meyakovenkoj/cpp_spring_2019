#include "numbers.dat"
#include <iostream>
#include <math.h>

int is_prime(int a)
{
	if (a == 0 || a == 1)
		return 0;
	if (a == 2 || a == 3 || a == 5 || a == 7 || a == 11 || a == 13 || a == 17 || a == 19 || a == 23 || a == 29)
		return 1;
	if (!(a%2) || !(a%3) || !(a%5) || !(a%7) || !(a%11) || !(a%13) || !(a%17) || !(a%19) || !(a%23) || !(a%29))
		return 0;
	int i1, i2, i3, i4, i5, i6, i7, i8, root;
	root = sqrt(a);
	i1 = 31; i2 = 37; i3 = 41; i4 = 43; i5 = 47; i6 = 49; i7 = 53; i8 = 59;
	while (i8 <= root && a%i1 && a%i2 && a%i3 && a%i4 && a%i5 && a%i6 && a%i7 && a%i8){
		i1 += 30; i2 += 30; i3 += 30; i4 += 30; i5 += 30; i6 += 30; i7 += 30; i8 += 30;
	}
	if (i8 <= root || (i1 <= root && !(a % i1)) || (i2 <= root && !(a % i2)) || 
		(i3 <= root && !(a % i3)) || (i4 <= root && !(a % i4)) || 
		(i5 <= root && !(a % i5)) || (i6 <= root && !(a % i6)) || 
		(i7 <= root && !(a % i7)))
		return 0;
	return 1;
}


int search(int a, int b, int &s, int &e){
	bool t = false;
	for(int i = 0; i<Size; i++){
		if(s == -1 && Data[i] == a){
			s = i;
			t = true;
		}
		if(Data[Size - 1 - i] == b){
			e = Size - 1 - i;
			if(t)
				break;
		}
	}
	if(s == -1 || e == -1)
		return 1;
	else
		return 0;
}

void init(bool (&primes)[Size]){
	for(int i = 0; i<Size; i++){
		if(is_prime(Data[i]))
			primes[i] = true;
	}
}

int main(int argc, char* argv[]){
	if (argc == 1 || !(argc % 2))
		return -1;
	bool primes[Size] = {false};
	int c = 1;
	int count,a,b;
	init(primes);
	while (c < argc) {
		count = 0;
		a = std::atoi(argv[c]);
		b = std::atoi(argv[++c]);
		if (b >= a){
			int s = -1,e = -1;
			if(search(a, b, s, e))
				break;
			for(int i = s; i < e; i++){
				if(primes[i])
					count++;
			}
		}
		std::cout << count << std::endl;
		c++;
	}
	return 0;
}
