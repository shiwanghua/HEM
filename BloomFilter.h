#pragma once
#include <bitset>
#include <vector>
#include <iostream>

//using namespace std;
using std::bitset;

typedef unsigned int uint;
#define MAX_BIT 256


class BloomFilter
{
public:
	bitset<MAX_BIT> bit;

	BloomFilter()
	{
		bit.reset();
	}
	//hash function 1
	/*
	uint RSHash(const int& e, int seed) {
		uint hash = 0;
		hash = seed * e;
		return (MAX_BIT - 1) & hash;
	}
	*/
	void SetKey(const int& e) {
		bit.set(((MAX_BIT - 1) & e));
	}
	int VaryExist(const int& e) {
		if (!bit.test((MAX_BIT - 1) & e))
		{
			return 0;
		}
		return 1;
	}
};
