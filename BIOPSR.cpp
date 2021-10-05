#include "BIOPSR.h"

BIOPSR::BIOPSR() {
	numSub = 0;
	numDimension = atts;
	numGroup = (numDimension - 1) / gs + 1;
	numState = pow(2, gs); // 每个维度上有两种状态
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	bitStep = numBucket >> 1;

	cout << "ExpID = " << expID << ". BIOPSRPS: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;

	if (be != 0) be = 0;
	//assert(be == 0);

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	endBucket[0] = new int[numBucket];
	endBucket[1] = new int[numBucket];
	bitsID[0] = new int[numBucket];
	bitsID[1] = new int[numBucket];
}

BIOPSR::~BIOPSR() {
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1];
}

void BIOPSR::insert(IntervalSub sub)
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
	numSub++;
}

void BIOPSR::initBits() {

	// 如果有多次初始化
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1];
	endBucket[0] = new int[numBucket];
	endBucket[1] = new int[numBucket];
	bitsID[0] = new int[numBucket];
	bitsID[1] = new int[numBucket];
	bits[0].clear(), bits[1].clear();
	bits[0].resize(numDimension);
	bits[1].resize(numDimension);
	bitsSR.clear();
	bitsSR.resize(numGroup, vector<bitset<subs>>(numState)); // 最后一组可能没这么多状态

	_for(i, 0, numBucket >> 1) {
		bitsID[0][i] = 0;                     // 此时的0号代表0.5~1, 不是0~1
		bitsID[1][i] = -1;                    // 此时用不到bits数组, -1表示非法
		endBucket[0][i] = bitStep;            // 标记时遍历到小于这个值
		endBucket[1][i] = 0;                  // 标记时遍历到大于等于这个值
	}
	_for(i, numBucket >> 1, numBucket) {
		bitsID[0][i] = -1;
		bitsID[1][i] = 0;
		endBucket[0][i] = numBucket;
		endBucket[1][i] = bitStep;
	}

	_for(i, 0, numDimension) {                // 每个维度
		_for(j, 0, bitStep)                   // 每个左半部分的桶
			_for(k, 0, data[1][i][j].size())  // 桶里每个订阅
			bits[1][i][data[1][i][j][k].subID] = 1;  // high, i维, subID
		_for(j, bitStep, numBucket)           // 每个右半部分的桶
			_for(k, 0, data[0][i][j].size())  // 桶里每个订阅
			bits[0][i][data[0][i][j][k].subID] = 1;  // high, i维, subID
	}

	_for(g, 0, numGroup) {
		int begin = g * gs, end = min(numDimension, (g + 1) * gs); // 为了适应最后一个组不满gs个维度的情况
		int group_size = end - begin;
		int state_num = pow(2, group_size);                        // 一般都等于numState
		_for(s, 0, state_num) {
			int si = 1; // s的最低位表示begin维度, 最高位表示end-1维度
			_for(i, 0, group_size) {
				if (si & s) // s的第i位为1表示用high上的bitset
					bitsSR[g][s] = bitsSR[g][s] | bits[1][begin + i];
				else bitsSR[g][s] = bitsSR[g][s] | bits[0][begin + i];
				si = si << 1;
			}
		}
	}

	//cout << "Stop.\n";
	return;
}

void BIOPSR::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> globalBitset;
	vector<int> attExist(numDimension, -1);
	int value, att, buck;
	_for(i, 0, pub.size)
	{
		Timer compareStart;
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = bitsID[1][buck] + 1; // 用low上的就是0, 用high上的就是1
		_for(k, 0, data[0][att][buck].size())
			if (data[0][att][buck][k].val > value)
				globalBitset[data[0][att][buck][k].subID] = 1;
		_for(k, 0, data[1][att][buck].size())
			if (data[1][att][buck][k].val < value)
				globalBitset[data[1][att][buck][k].subID] = 1;
		compareTime += (double)compareStart.elapsed_nano();

		Timer markStart;
		_for(j, buck + 1, endBucket[0][buck])
			_for(k, 0, data[0][att][j].size())
			globalBitset[data[0][att][j][k].subID] = 1;
		mmfor(j, buck - 1, endBucket[1][buck])
			_for(k, 0, data[1][att][j].size())
			globalBitset[data[1][att][j][k].subID] = 1;
		markTime += (double)markStart.elapsed_nano();

		//Timer orStart;
		//if (bitsID[0][buck] != -1) 
		//	globalBitset = globalBitset | bits[0][att];
		//else //if (bitsID[1][buck] != -1)  // 每个维度上只有两种状态
		//	globalBitset = globalBitset | bits[1][att]; // Bug: 是att不是i
		//orTime += (double)orStart.elapsed_nano();
	}

	Timer markStart2;
	_for(i, 0, numDimension)
		if (attExist[i] == -1) {
			attExist[i] = 0;
			_for(j, 0, bitStep) // 令空维度的状态为0, 即使用low上的bitset
				_for(k, 0, data[0][i][j].size())
				globalBitset[data[0][i][j][k].subID] = 1;
		}else{
			
		}
	markTime += (double)markStart2.elapsed_nano();

	Timer orStart;
	int i = 0, j, state, end; // i 遍历每个维度, j 遍历组内部的每一位, 每个组最后一个元素的下一个位置是end, 
	_for(g, 0, numGroup) {
		state = 0, j = 1;
		end = min(numDimension, i + gs);
		while (i < end) {
			if (attExist[i])
				state |= j;
			j = j << 1;
			i++;
		}
		globalBitset = globalBitset | bitsSR[g][state];
	}
	orTime += (double)orStart.elapsed_nano();

	Timer bitStart;
	_for(i, 0, subs)
		if (!globalBitset[i])
		{
			++matchSubs;
			//cout << "BIOPSR matches sub: : " << i << endl;
		}
	bitTime += (double)bitStart.elapsed_nano();
}

//void BIOPSR::calBucketSize() {
//	bucketSub.clear();
//	bucketSub.resize(numBucket);
//	_for(i, 0, numDimension)
//		_for(j, 0, numBucket)
//		{
//			_for(k, 0, data[0][i][j].size())
//				bucketSub[j].insert(data[0][i][j][k].subID);
//			_for(k, 0, data[1][i][j].size())
//				bucketSub[j].insert(data[1][i][j][k].subID);
//		}
//}

int BIOPSR::calMemory() {
	long long size = 0; // Byte
	size += 2 * numDimension * sizeof(bitset<subs>); // bits
	size += numGroup*numState* sizeof(bitset<subs>); // bitsSR
	size -= (numState - pow(2, numDimension % numGroup)) * sizeof(bitset<subs>); // 减去bitsSR最后一组多算的内存大小
	_for(i, 0, numDimension) {
		_for(j, 0, numBucket)
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}

	// 两个endBucket和两个bitsID
	size += 4 * numBucket * sizeof(int);
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void BIOPSR::printRelation() {
	cout << "\n\nBIOP-SR-PS Map LowBucket\n";
	_for(i, 0, numBucket) {
		cout << "LBkt" << i << ": bID=" << bitsID[0][i] << ", eBkt=" << endBucket[0][i] << "; ";
		if (i % 5 == 0 && i > 0)cout << "\n";
	}
	cout << "\n\nBIOP-SR-PS Map HighBucket\n";
	_for(i, 0, numBucket) {
		cout << "HBkt" << i << ": bID=" << bitsID[1][i] << ", eBkt=" << endBucket[1][i] << "; ";
		if (i % 5 == 0 && i > 0)cout << "\n";
	}
	cout << "\n\n";
}
