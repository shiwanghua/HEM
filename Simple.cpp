#include "Simple.h"

Simple::Simple() {
	cout << "ExpID = " << expID << ". Simple Algorithm."<< endl;
}

Simple::~Simple() {
	
}

void Simple::insert(IntervalSub sub)
{
	data.push_back(sub);
}

void Simple::match(dPub& dpub, int& matchSubs)
{
	bool flag;
	int eValue;
	_for(i, 0, data.size())
	{
		flag = true;
		_for(j, 0, data[i].size) {
			eValue = dpub.AVmap[data[i].constraints[j].att];
			if (eValue < data[i].constraints[j].lowValue || eValue>data[i].constraints[j].highValue) {
				flag = false;
				break;
			}
		}
		if (flag)
			matchSubs++;
	}
	//cout << "Simple matches sub: : " << i << endl;
}

int Simple::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, data.size()) {
		size += sizeof(int) * 3 * data[i].constraints.size();
	}
	size = size / 1024 / 1024; // MB
	return (int)size;
}

