#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <string>
#include <exception>

std::atomic<bool> finished = false;
std::atomic<int> finishedStep = 0;
std::atomic<int> finishedSort = 0;
std::condition_variable cond;
std::mutex sortFinishM;
std::mutex outQueM;
std::mutex streamReadM;
std::mutex iterFinishM;

class Thread_sort
{
	size_t memLimit;
	int threadCount = 2;
	size_t threadMemLimit;
	size_t threadUintLimit;
	//std::unique_ptr<uint64_t>  buf;
	uint64_t * const buf;
	std::ifstream inputStream;
	std::queue<std::string> outputFiles;


	void read_files(uint64_t * const buffer, const int id)
	{
		int file = 0;
		while (inputStream.good()) {
			{
				std::unique_lock<std::mutex> lock(streamReadM);
				inputStream.read(reinterpret_cast<char *>(buffer), threadMemLimit);
			}

			std::streamsize readAmount = inputStream.gcount() / sizeof(uint64_t);

			if (readAmount)
			{
				std::sort(buffer, buffer + readAmount);
				std::string outputFileName = std::to_string(0) + '_' + std::to_string(id) + '_' + std::to_string(file) + ".tmp";
				std::ofstream outputStream(outputFileName, std::ios::binary);
				outputStream.write(reinterpret_cast<char*>(buffer), readAmount * sizeof(uint64_t));
				outputFiles.push(outputFileName);
				++file;
			}
		}
		
	}

	void merge_files(const std::string & fileName1, const std::string & fileName2, uint64_t * const buffer, const int id, const int iter, const int file)
	{
		std::ifstream file1(fileName1, std::ios::binary), file2(fileName2, std::ios::binary);
		const std::string name = std::to_string(iter) + '_' + std::to_string(id) + '_' + std::to_string(file) + ".tmp";
		std::ofstream out(name, std::ios::binary);
		
		const size_t limit = (threadUintLimit / 4), limitM = 2 * limit;

		uint64_t * const bufL = buffer, * const bufR = (buffer + limit), * const bufM = (buffer + limitM);
		
		file1.read(reinterpret_cast<char*>(bufL), limit * sizeof(uint64_t));
		size_t readL = file1.gcount() / sizeof(uint64_t);
		file2.read(reinterpret_cast<char*>(bufR), limit * sizeof(uint64_t));
		size_t readR = file2.gcount() / sizeof(uint64_t);
		
		size_t left = 0, middle = 0, right = 0;
		
		while (!file1.eof() || !file2.eof() || left < readL || right < readR) {
			if (left == readL && !file1.eof()) { //закончился левый индекс, но остался левый файл
				file1.read(reinterpret_cast<char *>(bufL), limit * sizeof(uint64_t));
				readL = file1.gcount() / sizeof(uint64_t);
				left = 0;
			}
			if (right == readR && !file2.eof()) { //закончился правый индекс, но остался правый файл
				file2.read(reinterpret_cast<char *>(bufR), limit * sizeof(uint64_t));
				readR = file2.gcount() / sizeof(uint64_t);
				right = 0;
			}
			if (middle == limitM) { //заполнился буфер
				out.write(reinterpret_cast<char *>(bufM), middle * sizeof(uint64_t));
				middle = 0;
			}
			
			if (left < readL && right < readR) { //если есть индексы сравниваем и закидываем в буфер
				if (bufR[right] < bufL[left]) {
					bufM[middle] = bufR[right]; //закидываем правое число
					++right;
					++middle;
				}
				else {
					bufM[middle] = bufL[left]; //иначе левое
					++left;
					++middle;
				}
			}
			else
			{
				if (left == readL && right < readR) { //остались только правые
					bufM[middle] = bufR[right];
					++right;
					++middle;
				} else {
					if (right == readR && left < readL) { //остались только левые
						bufM[middle] = bufL[left];
						++left;
						++middle;
					}
				}
			}
			
			
		}
		
		out.write(reinterpret_cast<char *>(bufM), middle * sizeof(uint64_t)); //дописываем остатки
		std::unique_lock<std::mutex> queueLock(outQueM);
		outputFiles.push(name);
	}

	void thread_sort(const int id) {
		uint64_t * const buffer = buf + id * threadUintLimit;
		int iteration = 0, file = 0;
		
		read_files(buffer, id);
		
		++iteration;
		
		std::unique_lock<std::mutex> lock(iterFinishM);
		++finishedStep;
		cond.notify_all();
		while (finishedStep < threadCount)
			cond.wait(lock);
		lock.unlock();
		
		while (outputFiles.size() >= 2) {
			std::unique_lock<std::mutex> queueLock(outQueM);
			if (outputFiles.size() >= 2) {
				std::string tmp1 = outputFiles.front();
				outputFiles.pop();
				std::string tmp2 = outputFiles.front();
				outputFiles.pop();
				queueLock.unlock();
				merge_files(tmp1, tmp2, buffer, id, iteration, file);
				std::remove(tmp1.c_str());
				std::remove(tmp2.c_str());
				++file;
			}
		}
		std::unique_lock<std::mutex> fLock(sortFinishM);
		++finishedSort;
		if (finishedSort == threadCount) {
			if (outputFiles.empty()) throw std::logic_error("no output files");
			else std::cout << "finished, result in file: " << outputFiles.front() << std::endl;
		}
	}

public:
	Thread_sort(size_t _memLimit, std::string _data) :
		memLimit(_memLimit), 
		threadMemLimit (memLimit / threadCount), 
		threadUintLimit(threadMemLimit / sizeof(uint64_t)), 
		buf(new uint64_t[memLimit / sizeof(uint64_t)]),
		inputStream(_data, std::ios::binary)
	{
	}

	void sort(){
		if(!inputStream.good())
			throw std::invalid_argument("can't open file");
		std::vector<std::thread> threads;
		for (int i = 0; i < threadCount; ++i) {
			threads.emplace_back(&Thread_sort::thread_sort, this, i);
		}
		for (int i = 0; i < threadCount; ++i) {
			threads[i].join();
		}
	}

	~Thread_sort(){
		delete [] this->buf;
	}
	
};
