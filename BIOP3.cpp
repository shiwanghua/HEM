#include "BIOP3.h"

BIOP3::BIOP3() {
	numSub = 0;
	numDimension = atts;
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "ExpID = " << expID << ". BIOP3PD: bit exponent = " << be << ", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (be == -1)
		numBits = numBucket;
	else
		numBits = pow(2, be);  // 每个维度上lowValue对应的bits数组个数
	if (numBits > 1) {
		fullBits.resize(numDimension);  // 维度总数永远不变，所以只需要resize一次
		//bitStep = (numBucket + numBits - 1) / numBits;  // 每过这么远新设一个bits
		// 等价写法：bitStep = numBucket % numBits == 0 ? numBucket / numBits : numBucket / numBits + 1;
	}
	//else bitStep = numBucket >> 1;

	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension) {
		endBucket[0][i] = new int[numBucket];
		endBucket[1][i] = new int[numBucket];
		bitsID[0][i] = new int[numBucket];
		bitsID[1][i] = new int[numBucket];
	}

	fix[0].resize(numDimension, vector<int>(numBucket+1));
	fix[1].resize(numDimension, vector<int>(numBucket+1));
}

BIOP3::~BIOP3() {
	_for(i, 0, numDimension)
		delete[] endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1];
}

void BIOP3::insert(IntervalSub sub)
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

// fullBits单独存储的版本
void BIOP3::initBits() {

	// 如果有多次初始化
	_for(i, 0, numDimension)
		delete[] endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1];
	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension) {
		endBucket[0][i] = new int[numBucket];
		endBucket[1][i] = new int[numBucket];
		bitsID[0][i] = new int[numBucket];
		bitsID[1][i] = new int[numBucket];
	}
	bits[0].clear(), bits[1].clear();
	bits[0].resize(numDimension, vector<bitset<subs>>(numBits > 1 ? numBits - 1 : 1));
	bits[1].resize(numDimension, vector<bitset<subs>>(max(numBits - 1, 1)));

	// 前缀和、后缀和数组, 不包括本身
	_for(i, 0, numDimension) {
		_for(j, 1, numBucket) {
			fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
			fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
		}
		// 整个数组的和存在最后一个元素上
		fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
		fix[1][i][numBucket] = fix[1][i][numBucket] + data[1][i][numBucket - 1].size();
	}

	if (numBits == 1) { // 只有一个bits时特判，不用fullBits
		int halfWorkLoad = numSub >> 1;
		_for(i, 0, numDimension) {
			int lowCriticalPoint = -1, highCriticalPoint = -1; // 第一个后/前缀和包含一半订阅的桶ID，bit数组最远正好覆盖到这个桶
			_for(j, 0, numBucket) {
				if (fix[0][i][numBucket-1-j]>=halfWorkLoad&& lowCriticalPoint==-1) 
					lowCriticalPoint = j;
				if (fix[1][i][j] >= halfWorkLoad && highCriticalPoint == -1)
					highCriticalPoint = j;
			}
			_for(j, 0, numBucket) {
				if (j < lowCriticalPoint) { // 可以用bit
					bitsID[0][i][j] = 0;
					endBucket[0][i][j] = lowCriticalPoint;
				}
				else {
					bitsID[0][i][j] = -1;
					endBucket[0][i][j] = numBucket;
					_for(k, 0, data[0][i][j].size())  // 桶里每个订阅
						bits[0][i][0][data[0][i][j][k].subID] = 1;  
				}
				if (j <= highCriticalPoint) { // 不可以用bit
					bitsID[1][i][j] = -1;
					endBucket[1][i][j] = 0;
					_for(k, 0, data[1][i][j].size())  // 桶里每个订阅
						bits[1][i][0][data[1][i][j][k].subID] = 1;
				}
				else {
					bitsID[1][i][j] = 0;
					endBucket[1][i][j] = highCriticalPoint;
				}
			}
		}
		//cout << "Stop.\n";
		return;
	}

	//_for(i, 0, numBucket) {
	//	//bitsID[0][i] = (numBucket - i - 1) / bitStep - 1; // (1000-499-1)/500=1, (1000-749-1)/250=1, (1000-936-1)/63=1, (1000-873-1)/63=2, (1000-54-1)/63=15 // 这个映射关系可以兼容numBits为1的情况，numBits为1时bitStep为500
	//	bitsID[1][i] = i / bitStep - 1; // 750/250=3, 63/63=1
	//	bitsID[0][i] = numBits - 3 - bitsID[1][i]; // 保证共用同一套cell

	//	//endBucket[0][i] = numBucket - (bitsID[0][i] + 1) * bitStep; // 1000-1*63=937, 1000-2*63=874, 1000-15*63=55
	//	endBucket[1][i] = (bitsID[1][i] + 1) * bitStep;
	//	endBucket[0][i] = min(numBucket, endBucket[1][i] + bitStep);
	//}

	//int subID, b;   // 起始标记数组的下标
	//_for(i, 0, numDimension) {      // 每个维度
	//	_for(j, 0, numBucket) {     // 每个桶
	//		b = bitsID[0][j] + 1;
	//		_for(k, 0, data[0][i][j].size()) {
	//			subID = data[0][i][j][k].subID;
	//			fullBits[i][subID] = 1;            // 0号bits每次必须标记
	//			//bits[0][i][0][subID] = 1;        
	//			_for(q, b, numBits - 1)              // Bug: bits都是是从高位(覆盖广)往低位遍历！
	//				bits[0][i][q][subID] = 1;
	//		}

	//		b = bitsID[1][j] + 1;   // 除了0号外最小的需要插入的bits数组的ID
	//		_for(k, 0, data[1][i][j].size()) {   // 桶里每个订阅
	//			subID = data[1][i][j][k].subID;
	//			//bits[1][i][0][subID] = 1;        // 0号bits每次必须标记
	//			_for(q, b, numBits - 1)
	//				bits[1][i][q][subID] = 1;
	//		}
	//	}
	//}
	//cout << "Stop.\n";
}

void BIOP3::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> b;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;
	_for(i, 0, pub.size)
	{
		Timer compareStart;
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;
		_for(k, 0, data[0][att][buck].size())
			if (data[0][att][buck][k].val > value)
				b[data[0][att][buck][k].subID] = 1;
		_for(k, 0, data[1][att][buck].size())
			if (data[1][att][buck][k].val < value)
				b[data[1][att][buck][k].subID] = 1;
		compareTime += (double)compareStart.elapsed_nano();

		//Timer markStart;
		//_for(j, buck + 1, endBucket[0][buck])
		//	_for(k, 0, data[0][att][j].size())
		//	b[data[0][att][j][k].subID] = 1;
		//mmfor(j, buck - 1, endBucket[1][buck])
		//	_for(k, 0, data[1][att][j].size())
		//	b[data[1][att][j][k].subID] = 1;
		//markTime += (double)markStart.elapsed_nano();

		//Timer orStart;
		//if (bitsID[0][buck] != -1)
		//	b = b | bits[0][att][bitsID[0][buck]]; // 此时如果bitsID[0][buck]为0，不是表示覆盖所有桶的bits数组，而是表示只有1个bits数组(覆盖一半桶)的情况
		//if (bitsID[1][buck] != -1)
		//	b = b | bits[1][att][bitsID[1][buck]]; // Bug: 是att不是i
		//orTime += (double)orStart.elapsed_nano();
	}

	Timer orStart;
	_for(i, 0, numDimension)
		if (!attExist[i])
			b = b | fullBits[i];
	orTime += (double)orStart.elapsed_nano();

	Timer bitStart;
	_for(i, 0, subs)
		if (!b[i])
		{
			++matchSubs;
			//cout << "BIOP3 matches sub: : " << i << endl;
		}
	bitTime += (double)bitStart.elapsed_nano();
}

//void BIOP3::calBucketSize() {
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

int BIOP3::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, numDimension) {
		// 若每个维度上bits数组个数一样就是 2*sizeof(bitset<subs>)*numDimension*numBits
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		_for(j, 0, numBucket)
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}

	// fullBits
	if (numBits > 1) // 不加也行
		size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()即numDimension

	// 两个fix
	size += sizeof(int) * numDimension * (numBucket + 1);
	// 两个endBucket、两个bitsID
	size += 4 * sizeof(int) * numDimension * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void BIOP3::printRelation(int dimension_i) {
	cout << "\n\nBIOP3Map\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension) {
		cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][dimension_i][numBucket] << "   ----------------\n";
		_for(j, 0, numBucket) {
			cout << "lBkt" << j << ": bID=" << bitsID[0][i][j] << ", eBkt=" << endBucket[0][i][j] << "; ";
			if (j % 5 == 0 && j > 0)cout << "\n";
		}
		cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket] << "   ----------------\n";
		_for(j, 0, numBucket) {
			cout << "hBkt" << j << ": bID=" << bitsID[1][i][j] << ", eBkt=" << endBucket[1][i][j]  << "; ";
			if (j % 5 == 0 && j > 0)cout << "\n";
		}
	}
	else {
		cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "lBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "hBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
	}
	cout << "\n\n";
}
