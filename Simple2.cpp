#include "Simple2.h"

Simple2::Simple2() {
	cout << "ExpID = " << expID << ". Simple2 Algorithm." << endl;
}

Simple2::~Simple2() {

}

void Simple2::insert(IntervalSub sub)
{
	sort(sub.constraints.begin(), sub.constraints.end(), [](IntervalCnt& a, IntervalCnt& b) {return a.highValue - a.lowValue < b.highValue - b.lowValue; });
	data.push_back(sub);
}

void Simple2::match(dPub& dpub, int& matchSubs)
{
	bool flag;
	int eValue;
	_for(i, 0, data.size())
	{
		flag = true;
		_for(j, 0, data[i].size) {
			eValue = dpub.AVmap[data[i].constraints[j].att];
			if (eValue< data[i].constraints[j].lowValue || eValue>data[i].constraints[j].highValue) {
				flag = false;
				break;
			}
		}
		if (flag)
			matchSubs++;
	}
	//cout << "Simple2 matches sub: : " << i << endl;

}

int Simple2::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, data.size()) {
		size += sizeof(int) * 3 * data[i].constraints.size();
	}
	size = size / 1024 / 1024; // MB
	return (int)size;
}

