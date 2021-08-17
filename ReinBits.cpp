#include "ReinBits.h"

ReinBits::ReinBits(int valDom, int numSubscription, int numDim, int numBuck, int b) :maxValue(valDom), numSub(numSubscription), numDimension(numDim) {
	buckStep = (valDom - 1) / numBuck + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
	bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	numBits = pow(2, b);  // 每个维度上lowValue对应的bits数组个数
	bitStep = numBucket / numBits;
	bits[0].resize(numDimension, vector<bitset<subs>>(numBits));
	bits[1].resize(numDimension, vector<bitset<subs>>(numBits));
	endBucket[0] = new int[numBucket];
	endBucket[1] = new int[numBucket];
	bitsID[0] = new int[numBucket];
	bitsID[1] = new int[numBucket];

	fix[0].resize(numDimension, vector<int>(numBucket));
	fix[1].resize(numDimension, vector<int>(numBucket));
}

ReinBits::~ReinBits() {
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1];
	delete[] endBucket, bitsID;
}

void ReinBits::insert(IntervalSub sub)
{
	for (int i = 0; i < sub.size; i++)
	{
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		// int bucketID = cnt.lowValue / buckStep; // Bug: 这里被坑了
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][cnt.lowValue / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][cnt.highValue / buckStep].push_back(c);
	}
}

void ReinBits::initBits() {

	/*_for(i, 0, numDimension) {
		fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
		fix[1][i][0] = data[1][i][0].size();
		_for(j, 1, numBucket) {
			fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - 1 - j].size();
			fix[1][i][j] = fix[1][i][j - 1] + data[0][i][j].size();
		}
	}*/

	if (numBits == 1) {                           // 只有一个bits时特判，因为后面默认0号bits代表0~1的全局bits
		_for(i, 0, numDimension) {                // 每个维度
			_for(j, 0, numBucket >> 1)              // 每个左半部分的桶
				_for(k, 0, data[0][i][j].size())  // 桶里每个订阅
				bits[0][i][0][data[0][i][j][k].subID] = 1;  // low, i维, 0号bits, subID
			_for(j, numBucket >> 1, numBucket)    // 每个右半部分的桶
				_for(k, 0, data[1][i][j].size())  // 桶里每个订阅
				bits[1][i][0][data[1][i][j][k].subID] = 1;  // high, i维, 0号bits, subID
		}
		_for(i, 0, numBucket >> 1) {
			bitsID[0][i] = 0;                     // 此时的0号代表0.5~1, 不是0~1
			bitsID[1][i] = -1;                    // 此时用不到bits数组, -1表示非法
			endBucket[0][i] = numBucket >> 1;     // 标记时遍历到小于这个值
			endBucket[1][i] = 0;                  // 标记时遍历到大于等于这个值
		}
		_for(i, numBucket >> 1, numBucket) {
			bitsID[0][i] = -1;
			bitsID[1][i] = 0;
			endBucket[0][i] = numBucket;
			endBucket[1][i] = numBucket >> 1;
		}
		cout << "Stop.\n";
		return;
	}

	int subID;
	int b;    // 需要标记bits数组的个数
	_for(i, 0, numDimension) {  // 每个维度
		_for(j, 0, numBucket) { // 每个桶
			b = min(numBits, (numBucket - j) * numBits / numBucket + 1); // low 上需要标记的bits数组的个数
			_for(k, 0, data[0][i][j].size()) { // 桶里每个订阅
				subID = data[0][i][j][k].subID;
				_for(q, 0, b)
					bits[0][i][q][subID] = 1;
			}
			b = numBits + 1 - b; // high 上需要标记的bits数组的个数
			_for(k, 0, data[1][i][j].size()) {
				subID = data[1][i][j][k].subID;
				_for(q, 0, b)
					bits[1][i][q][subID] = 1;
			}
		}
	}
	_for(i, 0, numBucket) {
		bitsID[0][i] = (numBucket - i - 1) * numBits / numBucket;
		bitsID[1][i] = numBits - 1 - bitsID[0][i];
		endBucket[0][i] = numBucket - bitsID[0][i] * bitStep;
		endBucket[1][i] = bitsID[1][i] * bitStep;
		if (bitsID[0][i] == 0) // 真正的0号bits是留给全覆盖的bits的编号
			bitsID[0][i] = -1; // -1表示此时用不到bits数组
		if (bitsID[1][i] == 0) // 这两个if要放到最后，因为求endBucket会用到
			bitsID[1][i] = -1;
	}
	cout << "Stop.\n";
}

void ReinBits::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> b;
	int value, att, buck;
	_for(i, 0, pub.size)
	{
		Timer compareStart;
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		_for(k, 0, data[0][att][buck].size())
			if (data[0][att][buck][k].val > value)
				b[data[0][att][buck][k].subID] = 1;
		_for(k, 0, data[1][att][buck].size())
			if (data[1][att][buck][k].val < value)
				b[data[1][att][buck][k].subID] = 1;
		compareTime += (double)compareStart.elapsed_nano();

		Timer markStart;
		_for(j, buck + 1, endBucket[0][buck])
			_for(k, 0, data[0][att][j].size())
			b[data[0][att][j][k].subID] = 1;
		mmfor(j, buck - 1, endBucket[1][buck])
			_for(k, 0, data[1][att][j].size())
			b[data[1][att][j][k].subID] = 1;
		markTime += (double)markStart.elapsed_nano();

		Timer orStart;
		if (bitsID[0][buck] != -1)
			b = b | bits[0][i][bitsID[0][buck]]; // 此时如果bitsID[0][buck]为0，不是表示覆盖所有桶的bits数组，而是表示只有1个bits数组(覆盖一半桶)的情况
		if (bitsID[1][buck] != -1)
			b = b | bits[1][i][bitsID[1][buck]];
		orTime += (double)orStart.elapsed_nano();
	}

	Timer bitStart;
	_for(i, 0, numSub)
		if (!b[i])
			++matchSubs;
	bitTime += (double)bitStart.elapsed_nano();
}

void ReinBits::calBucketSize() {
	bucketSub.clear();
	bucketSub.resize(numBucket);
	_for(i, 0, numDimension)
		_for(j, 0, numBucket)
		{
			_for(k, 0, data[0][i][j].size())
				bucketSub[j].insert(data[0][i][j][k].subID);
			_for(k, 0, data[1][i][j].size())
				bucketSub[j].insert(data[1][i][j][k].subID);
		}
}

int ReinBits::calMemory() {
	int size = 0; // Byte
	_for(i, 0, numDimension) {
		// 若每个维度上bits数组个数一样就是 2*numDimension*numBits*sizeof(bitset<subs>)
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		_for(j, 0, numBucket)
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}
	return size / 1024 / 1024; // MB
}
