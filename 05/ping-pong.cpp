#include <iostream>
#include <mutex>
#include <thread>

std::mutex m1;
std::mutex m2;

void ping()
{
	for(int i = 0; i < 500000; i++)
	{
		m1.lock();
		std::cout << "ping" << std::endl;
		m2.unlock();
	}
}

void pong()
{
	for(int i = 0; i < 500000; i++)
	{
		m2.lock();
		std::cout << "pong" << std::endl;
		m1.unlock();
	}
}

int main()
{
	std::thread t1(ping);
	std::thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}

