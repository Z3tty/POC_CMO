#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

template <typename T> int getCardinality(T n)
{
	int c = 1; std::string str = std::to_string(n);
	for(int i = 0; i < str.length(); i++) c *= 10;
	return c;
}

int cmovalidate(int dc, int dcsg, int dcsgc)
{
	if(dc == dcsg && dcsg == dcsgc) {
		printf("[CLEAN] Memory unaltered\n");
		printf("CMO [%d]\tCMO-SG[%d]\tCMO-SG-CHECK[%d]\n", dc, dcsg, dcsgc);
		return 0;
	} else {
		printf("[CORRUPT] Memory altered\n\n");
		printf("CMO [%d]\tCMO-SG[%d]\tCMO-SG-CHECK[%d]\n\n", dc, dcsg, dcsgc);
		return 1;
	} return -1;
}

template <typename T> class ComplexMemoryObject {
public:
	bool _useSimilarValues;	std::size_t _elementCount;
	std::vector<T> _memory;	std::size_t _idx;

	ComplexMemoryObject(bool useSimilarValues, std::size_t elementCount, T data) {
			
		std::srand(std::time(nullptr));	_useSimilarValues = useSimilarValues;
		_elementCount = elementCount;	_memory.resize(_elementCount);
		do {
			_idx = std::rand() % _elementCount;
		} while (_idx > _elementCount);

		std::srand(std::time(nullptr));
		for (int i = 0; i < _elementCount; i++)
			_memory[i] = (std::rand() % getCardinality(data));
		if (_useSimilarValues) {
			int j = 0, i = std::rand() % _elementCount;
			while (j < _elementCount / 2) {
				_memory[i] = data;
				j++; i = std::rand() % _elementCount;
			}
		} _memory[_idx] = data;
	}
	
	void writeMemory(T data) {
		std::srand(std::time(nullptr));
		for (int i = 0; i < _elementCount; i++) _memory[i] = (std::rand() % getCardinality(data));
		_memory[_idx] = data;
		
		if (_useSimilarValues) {
			int j = 0, i = std::rand() % _elementCount;
			while (j < _elementCount / 2) {
				_memory[i] = data;
				j++; i = std::rand() % _elementCount;
			}
		} _memory[_idx] = data;
	}
	
	T readMemory() { return _memory[_idx]; }
	void printMemory() { for (auto m : _memory) std::cout << m << std::endl; }
};

int main(void) {
	ComplexMemoryObject<long> cmo(true, 5, 1234);
	ComplexMemoryObject<long> cmo_safeguard(true, 5, 1234);
	ComplexMemoryObject<long> cmo_sg_check(true, 5, 1234);
	int data_cmo = cmo.readMemory();
	int data_cmo_sg = cmo.readMemory();
	int data_cmo_sg_check = cmo.readMemory();
	std::cout << data_cmo << "\t*" << std::endl;
	std::cout << data_cmo_sg << "\t*" << std::endl;
	std::cout << data_cmo_sg_check << "\t*" << std::endl;
	cmo.printMemory();
	cmo_safeguard.printMemory();
	cmo_sg_check.printMemory();
	printf("\n");
	
	cmo.writeMemory(987654321); 			data_cmo = cmo.readMemory();
	cmo_safeguard.writeMemory(987654321);	data_cmo_sg = cmo_safeguard.readMemory();
	cmo_sg_check.writeMemory(987654321);	data_cmo_sg_check = cmo_sg_check.readMemory();
	
	std::cout << data_cmo << "\t*" << std::endl;
	std::cout << data_cmo_sg << "\t*" << std::endl;
	std::cout << data_cmo_sg_check << "\t*" << std::endl;
	cmo.printMemory();
	cmo_safeguard.printMemory();
	cmo_sg_check.printMemory();
	printf("\n---------------------------------------\n");
	
	int err1 = cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
	if(err1 == 0) {
		printf("Memory was clean, roll-back not performed\n");
	} else {
		printf("Memory corrupted -- rolling back to safeguard\n");
		if(cmo_safeguard.readMemory() == cmo_sg_check.readMemory()) {
			cmo.writeMemory(cmo_safeguard.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		} else {
			printf("Safeguard corrupted, rolling back to check-value\n");
			cmo.writeMemory(cmo_sg_check.readMemory());
			cmo_safeguard.writeMemory(cmo.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		}
	} printf("\n---------------------------------------\n");
	
	cmo.writeMemory(1423987);
	data_cmo = cmo.readMemory();
	data_cmo_sg = cmo_safeguard.readMemory();
	data_cmo_sg_check = cmo_sg_check.readMemory();
	int err2 = cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
	if(err2 == 0) {
		printf("Memory was clean, roll-back not performed\n");
	} else {
		printf("Memory corrupted -- rolling back to safeguard\n");
		if(cmo_safeguard.readMemory() == cmo_sg_check.readMemory()) {
			cmo.writeMemory(cmo_safeguard.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		} else {
			printf("Safeguard corrupted, rolling back to check-value\n");
			cmo.writeMemory(cmo_sg_check.readMemory());
			cmo_safeguard.writeMemory(cmo.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		}
	} printf("\n---------------------------------------\n");
	
	cmo.writeMemory(1423987);
	cmo_safeguard.writeMemory(14739287487);
	data_cmo = cmo.readMemory();
	data_cmo_sg = cmo_safeguard.readMemory();
	data_cmo_sg_check = cmo_sg_check.readMemory();
	int err3 = cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
	if(err3 == 0) {
		printf("Memory was clean, roll-back not performed\n");
	} else {
		printf("Memory corrupted -- rolling back to safeguard\n");
		if(cmo_safeguard.readMemory() == cmo_sg_check.readMemory()) {
			cmo.writeMemory(cmo_safeguard.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		} else {
			printf("Safeguard corrupted, rolling back to check-value\n");
			cmo.writeMemory(cmo_sg_check.readMemory());
			cmo_safeguard.writeMemory(cmo.readMemory());
			data_cmo = cmo.readMemory();
			data_cmo_sg = cmo_safeguard.readMemory();
			data_cmo_sg_check = cmo_sg_check.readMemory();
			cmovalidate(data_cmo, data_cmo_sg, data_cmo_sg_check);
		}
	} printf("\n---------------------------------------\n");
	return 0;
}
