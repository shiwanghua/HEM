#include "BIOP4.h"

BIOP4::BIOP4() {
	numSub = 0;
	numDimension = atts;
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "ExpID = " << expID << ". BIOP4DS: bit exponent = " << be << ", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (be == -1)
		numBits = be2;
	else
		numBits = pow(2, be);  // 每个维度上lowValue对应的bits数组个数
	if (numBits > 1)
		fullBits.resize(numDimension);  // 维度总数永远不变，所以只需要resize一次

	//else bitStep = numBucket >> 1;

	doubleReverse[0] = new bool* [numDimension];
	doubleReverse[1] = new bool* [numDimension];
	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension) {
		doubleReverse[0][i] = new bool[numBucket];
		doubleReverse[1][i] = new bool[numBucket];
		endBucket[0][i] = new int[numBucket];
		endBucket[1][i] = new int[numBucket];
		bitsID[0][i] = new int[numBucket];
		bitsID[1][i] = new int[numBucket];
	}

	fix[0].resize(numDimension, vector<int>(numBucket + 1));
	fix[1].resize(numDimension, vector<int>(numBucket + 1));
}

BIOP4::~BIOP4() {
	_for(i, 0, numDimension)
		delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
}

void BIOP4::insert(IntervalSub sub)
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
void BIOP4::initBits() {

	// 如果有多次初始化
	_for(i, 0, numDimension)
		delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
	doubleReverse[0] = new bool* [numDimension];
	doubleReverse[1] = new bool* [numDimension];
	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension) {
		doubleReverse[0][i] = new bool[numBucket];
		doubleReverse[1][i] = new bool[numBucket];
		endBucket[0][i] = new int[numBucket];
		endBucket[1][i] = new int[numBucket];
		bitsID[0][i] = new int[numBucket];
		bitsID[1][i] = new int[numBucket];
	}
	bits[0].clear(), bits[1].clear();
	bits[0].resize(numDimension, vector<bitset<subs>>(numBits > 1 ? numBits - 1 : 1));
	bits[1].resize(numDimension, vector<bitset<subs>>(max(numBits - 1, 1)));

	//// 前缀和、后缀和数组, 不包括本身
	//_for(i, 0, numDimension) {
	//	_for(j, 1, numBucket) {
	//		fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
	//		fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
	//	}
	//	// 整个数组的和存在最后一个元素上
	//	fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
	//	fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
	//}

	// 前缀和数组(不包括本身)、后缀和数组(包括本身)
	_for(i, 0, numDimension) {
		fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
		_for(j, 1, numBucket) {
			fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j - 1].size();
			fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
		}
		// 整个数组的和存在最后一个元素上
		fix[0][i][numBucket] = fix[0][i][0];
		fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
	}

	if (numBits == 1) { // 只有一个bits时特判，不用fullBits
		_for(i, 0, numDimension) {
			int halfWorkLoad = fix[0][i][numBucket] >> 1; // subWordLoadStep  fix[1][i][numBucket]
			// 第一个后/前缀和包含一半订阅的桶ID，bit数组最远正好覆盖到lowCriticalPoint和highCriticalPoint-1
			int lowCriticalPoint = -1, highCriticalPoint = -1;
			_for(j, 0, numBucket) {
				if (fix[0][i][numBucket - 1 - j] >= halfWorkLoad && lowCriticalPoint == -1)
					lowCriticalPoint = numBucket - 1 - j;
				if (fix[1][i][j] >= halfWorkLoad && highCriticalPoint == -1)
					highCriticalPoint = j;
			}
			int lowMid = (lowCriticalPoint + numBucket) / 2, highMid = highCriticalPoint / 2;
			_for(j, 0, numBucket) {
				if (j < lowCriticalPoint) {                 // 可以用上bitset
					bitsID[0][i][j] = 0;
					endBucket[0][i][j] = lowCriticalPoint;  // 遍历到小于 lowCriticalPoint
					doubleReverse[0][i][j] = false;
				}
				else if (j < lowMid) {
					bitsID[0][i][j] = 0;
					endBucket[0][i][j] = lowCriticalPoint;  // 从 j 二重反向遍历到等于 lowCriticalPoint(都包含)
					doubleReverse[0][i][j] = true;
					_for(k, 0, data[0][i][j].size())        // 桶里每个订阅
						bits[0][i][0][data[0][i][j][k].subID] = 1;
				}
				else {
					bitsID[0][i][j] = -1;
					endBucket[0][i][j] = numBucket;
					doubleReverse[0][i][j] = false;
					_for(k, 0, data[0][i][j].size())        // 桶里每个订阅
						bits[0][i][0][data[0][i][j][k].subID] = 1;
				}
				if (j < highMid) {                          // 不可以用bitset
					bitsID[1][i][j] = -1;
					endBucket[1][i][j] = 0;                 // 遍历到等于0
					doubleReverse[1][i][j] = false;
					_for(k, 0, data[1][i][j].size())        // 桶里每个订阅
						bits[1][i][0][data[1][i][j][k].subID] = 1;
				}
				else if (j < highCriticalPoint) {
					bitsID[1][i][j] = 0;
					endBucket[1][i][j] = highCriticalPoint;  // 从 j 二重反向遍历到大于等于 highCriticalPoint
					doubleReverse[1][i][j] = true;
					_for(k, 0, data[1][i][j].size())        // 桶里每个订阅
						bits[1][i][0][data[1][i][j][k].subID] = 1;
				}
				else {
					bitsID[1][i][j] = 0;
					endBucket[1][i][j] = highCriticalPoint; // 从 j-1 反向遍历到大于等于 highCriticalPoint, 和以前保持一致
					doubleReverse[1][i][j] = false;
				}
			}
		}
		//cout << "BIOP4DS Stop.\n";
		return;
	}

	// 当前应该映射到的bitId, 桶id, 下一个临界负载点
	int lowBid, highBid, lowBktId, highBktId, lowSubWorkLoad, highSubWorkLoad;
	int subWorkLoadStep; // 每个维度上的subWordLoadStep都不同, 但同一个维度上的low/high subWordLoadStep是一样的
	_for(i, 0, numDimension) {

		// 基本不会出现
		if (fix[0][i][0] == 0) {
			_for(j, 0, numBucket) {
				bitsID[0][i][j] = -1;
				endBucket[0][i][j] = j;
				doubleReverse[0][i][j] = false;
				bitsID[1][i][j] = -1;
				endBucket[1][i][j] = j;       // 遍历到大于等于endBucket[1][i][j]
				doubleReverse[1][i][j] = false;
			}
			continue;
		}

		subWorkLoadStep = (fix[0][i][numBucket] + numBits - 1) / numBits; // fix[1][i][numBucket]

		// 由于是low/high都是动态的, 基本不可能共用同一套partition/cell,
		// 但这里low还是从左边开始数一个subWordLoadStep的量, 保持一致      
		// 或者从右边数 剩余负载量 开始累加subWordLoadStep, 否则不清楚endBucket!
		// 0号low桶一定可以用到以 (numBits - 2) 为下标的bitset
		// 最后一个桶一定用不到bitset
		// 举例: numBits=15, fix[0][i][numBucket]=1000000, subWorkLoadStep=66667 (low上的后14个多1, high上的前14个多1)
		// fix[0][i][numBucket] / subWordLoadStep=14, lowSubWorkLoad=66662
		lowBid = -1;
		lowBktId = numBucket;
		lowSubWorkLoad = fix[0][i][numBucket] - (fix[0][i][numBucket] - 1) / subWorkLoadStep * subWorkLoadStep;
		highBid = -1;
		highBktId = 0;
		highSubWorkLoad = subWorkLoadStep;
		
		// lowContain[i]=右数(第一个覆盖)lowSubWorkLoad+(i-1)*subWorkLoadStep个订阅所到的桶号(i>0时)
		vector<int> lowContain(numBits+1,numBucket);
		// highContain[i]=左数i*subWorkLoadStep个订阅所到的桶号
		vector<int> highContain(numBits + 1, 0);
		int li = 1, hi = 1; // lowContain和highContain的下标
		_for(j, 0, numBucket) {
			if (fix[1][i][j] >= highSubWorkLoad) {
				highContain[hi++] = j;
				highSubWorkLoad += subWorkLoadStep;
			}
			if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad) {
				lowContain[li++] = numBucket - j - 1;
				lowSubWorkLoad += subWorkLoadStep;
			}
		}
		if (li == numBits) {
			lowContain[li] = 0;
			cout << "BIOP4 li=" << li << endl;
		}
		if (hi == numBits) {
			highContain[hi] = numBucket;
			//cout << "BIOP4 hi=" << hi << endl;
		}
		
		li = hi = 1; // 双重反向遍历时所对应的另一端的桶号在contain数组中的下标, 其实 li=lowBid+2, hi=highBid+2
		lowSubWorkLoad = fix[0][i][numBucket] - (fix[0][i][numBucket] - 1) / subWorkLoadStep * subWorkLoadStep;
		highSubWorkLoad = subWorkLoadStep;
		_for(j, 0, numBucket) {
			// 此时大于等于highSubWorkLoad了, 可以用bits, 因为bits覆盖到j-1桶
			if (fix[1][i][j] >= highSubWorkLoad) {         // 第一个大于等于临界点的桶(j-1号, 前缀和不包含本身)作为bitset覆盖的终点桶
				highSubWorkLoad += subWorkLoadStep;	      
				hi++;								      
				highBid++;							      
				highBktId = j;						      
			}					

			// Bug: 提前满了, 最后几个桶为空, 此时highBid=numBits-1, 越界了, 直接用fullBL
			if (fix[1][i][j] == fix[1][i][numBucket]) {
				bitsID[1][i][j] = numBits - 1;
				endBucket[1][i][j] = j + 1; // 如果是第一次进来, j号桶非空, 需要二重反向标记, 否则是空桶, 可以兼容这种情况
				doubleReverse[1][i][j] = true;
			}
			else if (j- highBktId <= highContain[hi] - j) {     // Bug: 没有减highBktId
				bitsID[1][i][j] = highBid;			      
				endBucket[1][i][j] = highBktId;            // 遍历到大于等于endBucket[1][i][j]
				doubleReverse[1][i][j] = false;		      
			}										      
			else {									      
				bitsID[1][i][j] = hi - 1;                  // highBid+1
				endBucket[1][i][j] = highContain[hi];      // 从j往右遍历到小于endBucket[1][i][j]
				doubleReverse[1][i][j] = true;		      
			}

			// Bug: 提前满了, 序号小的几个桶为空, 单独考虑, 直接用二重反向
			if (fix[0][i][numBucket - j - 1] == fix[0][i][0]) {
				bitsID[0][i][numBucket - j - 1] = numBits - 1;
				endBucket[0][i][numBucket - j - 1] = numBucket - j - 1;
				doubleReverse[0][i][numBucket - j - 1] = true;
			}
			else if (lowBktId - (numBucket - j - 1) - 1 <= numBucket - j - 1 - lowContain[li]+1) {
				bitsID[0][i][numBucket - j - 1] = lowBid;
				endBucket[0][i][numBucket - j - 1] = lowBktId;
				doubleReverse[0][i][numBucket - j - 1] = false;
			}
			else {
				bitsID[0][i][numBucket - j - 1] = li - 1;  // lowBktId+1
				endBucket[0][i][numBucket - j - 1] = lowContain[li];
				doubleReverse[0][i][numBucket - j - 1] = true;
			}
			
			// 此时虽然大于等于lowSubWorkLoad了, 但仍不可以用bits, 因为bits要覆盖到j号桶
			if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad) {
				lowSubWorkLoad += subWorkLoadStep;
				li++;
				lowBid++;
				lowBktId = numBucket - j - 1;
			}
		}
	}

	int subID, b;   // 起始标记数组的下标
	_for(i, 0, numDimension) {          // 每个维度
		_for(j, 0, numBucket) {         // 每个桶
			if (doubleReverse[0][i][j])
				b = bitsID[0][i][j];       // 最小的需要插入的bits数组的ID
			else b = bitsID[0][i][j] + 1;
			_for(k, 0, data[0][i][j].size()) {
				subID = data[0][i][j][k].subID;
				fullBits[i][subID] = 1; // 0号bits每次必须标记   
				_for(q, b, numBits - 1) // Bug: bits都是是从高位(覆盖广)往低位遍历！
					bits[0][i][q][subID] = 1;
			}

			if (doubleReverse[1][i][j])
				b = bitsID[1][i][j];
			else b = bitsID[1][i][j] + 1;  // 最小的需要插入的bits数组的ID
			_for(k, 0, data[1][i][j].size()) {     // 桶里每个订阅
				subID = data[1][i][j][k].subID;
				_for(q, b, numBits - 1)
					bits[1][i][q][subID] = 1;
			}
		}
	}
	//cout << "BIOP4DS Stop.\n";
}

//// 计算时间组成
//void BIOP4::match(const Pub& pub, int& matchSubs)
//{
//	bitset<subs> b, bLocal;
//	vector<bool> attExist(numDimension, false);
//	int value, att, buck;
//
//	_for(i, 0, pub.size)
//	{
//		Timer compareStart;
//		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		attExist[att] = true;
//		_for(k, 0, data[0][att][buck].size())
//			if (data[0][att][buck][k].val > value)
//				b[data[0][att][buck][k].subID] = 1;
//		_for(k, 0, data[1][att][buck].size())
//			if (data[1][att][buck][k].val < value)
//				b[data[1][att][buck][k].subID] = 1;
//		compareTime += (double)compareStart.elapsed_nano();
//
//		if (doubleReverse[0][att][buck]) {
//			Timer markStart;
//			if (bitsID[0][att][buck] == numBits - 1 && numBits > 1)
//				bLocal = fullBits[att];
//			else
//				bLocal = bits[0][att][bitsID[0][att][buck]];
//			_for(j, endBucket[0][att][buck], buck + 1)
//				_for(k, 0, data[0][att][j].size())
//				bLocal[data[0][att][j][k].subID] = 0;
//			markTime += (double)markStart.elapsed_nano();
//
//			Timer orStart;
//			b = b | bLocal;
//			orTime += (double)orStart.elapsed_nano();
//		}
//		else {
//			Timer markStart;
//			_for(j, buck + 1, endBucket[0][att][buck])
//				_for(k, 0, data[0][att][j].size())
//				b[data[0][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			if (bitsID[0][att][buck] != -1)
//				b = b | bits[0][att][bitsID[0][att][buck]];
//			orTime += (double)orStart.elapsed_nano();
//		}
//
//		if (doubleReverse[1][att][buck]) {
//			Timer markStart;
//			if (bitsID[1][att][buck] == numBits - 1 && numBits > 1)
//				bLocal = fullBits[att];
//			else
//				bLocal = bits[1][att][bitsID[1][att][buck]];
//			_for(j, buck, endBucket[1][att][buck])
//				_for(k, 0, data[1][att][j].size())
//				bLocal[data[1][att][j][k].subID] = 0;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			b = b | bLocal;
//			orTime += (double)orStart.elapsed_nano();
//		}
//		else {
//			Timer markStart;
//			_for(j, endBucket[1][att][buck], buck)
//				_for(k, 0, data[1][att][j].size())
//				b[data[1][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			if (bitsID[1][att][buck] != -1)
//				b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: 是att不是i
//			orTime += (double)orStart.elapsed_nano();
//		}
//	}
//
//	if (numBits > 1) {
//		Timer orStart;
//		_for(i, 0, numDimension)
//			if (!attExist[i])
//				b = b | fullBits[i];
//		orTime += (double)orStart.elapsed_nano();
//	}
//	else {
//		Timer markStart;
//		_for(i, 0, numDimension)
//			if (!attExist[i])
//				_for(j, 0, endBucket[0][i][0])
//				_for(k, 0, data[0][i][j].size())
//				b[data[0][i][j][k].subID] = 1;
//		markTime += (double)markStart.elapsed_nano();
//
//		Timer orStart;
//		_for(i, 0, numDimension)
//			if (!attExist[i])
//				b = b | bits[0][i][0];
//		orTime += (double)orStart.elapsed_nano();
//	}
//
//	Timer bitStart;
//	_for(i, 0, subs)
//		if (!b[i])
//		{
//			++matchSubs;
//			//cout << "BIOP4 matches sub: " << i << endl;
//		}
//	bitTime += (double)bitStart.elapsed_nano();
//}

// 不计算时间组成
 void BIOP4::match(const Pub& pub, int& matchSubs)
 {
 	bitset<subs> b, bLocal;
 	vector<bool> attExist(numDimension, false);
 	int value, att, buck;

 	_for(i, 0, pub.size)
 	{
 		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
 		attExist[att] = true;
 		_for(k, 0, data[0][att][buck].size())
 			if (data[0][att][buck][k].val > value)
 				b[data[0][att][buck][k].subID] = 1;
 		_for(k, 0, data[1][att][buck].size())
 			if (data[1][att][buck][k].val < value)
 				b[data[1][att][buck][k].subID] = 1;

 		if (doubleReverse[0][att][buck]) {
 			if (bitsID[0][att][buck] == numBits - 1 && numBits > 1)
 				bLocal = fullBits[att];
 			else
 				bLocal = bits[0][att][bitsID[0][att][buck]];
 			_for(j, endBucket[0][att][buck], buck + 1)
 				_for(k, 0, data[0][att][j].size())
 				bLocal[data[0][att][j][k].subID] = 0;

 			b = b | bLocal;
 		}
 		else {
 			_for(j, buck + 1, endBucket[0][att][buck])
 				_for(k, 0, data[0][att][j].size())
 				b[data[0][att][j][k].subID] = 1;
	
 			if (bitsID[0][att][buck] != -1)
 				b = b | bits[0][att][bitsID[0][att][buck]];
 		}

 		if (doubleReverse[1][att][buck]) {
 			if (bitsID[1][att][buck] == numBits - 1 && numBits > 1)
 				bLocal = fullBits[att];
 			else
 				bLocal = bits[1][att][bitsID[1][att][buck]];
 			_for(j, buck, endBucket[1][att][buck])
 				_for(k, 0, data[1][att][j].size())
 				bLocal[data[1][att][j][k].subID] = 0;
 			b = b | bLocal;
 		}
 		else {
 			_for(j, endBucket[1][att][buck], buck)
 				_for(k, 0, data[1][att][j].size())
 				b[data[1][att][j][k].subID] = 1;

 			if (bitsID[1][att][buck] != -1)
 				b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: 是att不是i
 		}
 	}

 	if (numBits > 1) {
 		_for(i, 0, numDimension)
 			if (!attExist[i])
 				b = b | fullBits[i];
 	}
 	else {
 		_for(i, 0, numDimension)
 			if (!attExist[i])
 				_for(j, 0, endBucket[0][i][0])
 				_for(k, 0, data[0][i][j].size())
 				b[data[0][i][j][k].subID] = 1;
	
 		_for(i, 0, numDimension)
 			if (!attExist[i])
 				b = b | bits[0][i][0];
 	}

 	_for(i, 0, subs)
 		if (!b[i])
 		{
 			++matchSubs;
 			//cout << "BIOP4 matches sub: " << i << endl;
 		}
 }

//void BIOP4::calBucketSize() {
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

int BIOP4::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, numDimension) {
		// 若每个维度上bits数组个数一样就是 2*sizeof(bitset<subs>)*numDimension*numBits
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		_for(j, 0, numBucket)
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}

	// fullBits
	if (numBits > 1)
		size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()即numDimension

	// 两个fix
	size += sizeof(int) * numDimension * (numBucket + 1);
	// 两个endBucket、两个bitsID、两个doubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void BIOP4::printRelation(int dimension_i) {
	cout << "\n\nBIOP4DSMap\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension) {
		cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][i][numBucket] << "   ----------------\n";
		_for(j, 0, numBucket) {
			cout << "lBkt" << j << ": bID=" << bitsID[0][i][j] << ", eBkt=" << endBucket[0][i][j] << ", dRvs=" << doubleReverse[0][i][j] << "; ";
			if (j % 5 == 0 && j > 0)cout << "\n";
		}
		cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][i][numBucket] << "   ----------------\n";
		_for(j, 0, numBucket) {
			cout << "hBkt" << j << ": bID=" << bitsID[1][i][j] << ", eBkt=" << endBucket[1][i][j] << ", dRvs=" << doubleReverse[1][i][j] << "; ";
			if (j % 5 == 0 && j > 0)cout << "\n";
		}
	}
	else {
		cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "lBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i] << ", dRvs=" << doubleReverse[0][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "hBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i] << ", dRvs=" << doubleReverse[1][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
	}
	cout << "\n\n";
}
