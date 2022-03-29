#include "HEMSC.h"

HEMSC::HEMSC() {
	numSub = 0;
	numDimension = atts;
	levelStep = (valDom - 1) / lvls + 1;
	numLevel = (valDom - 1) / levelStep + 1;
	buckStep = (valDom - 1) / buks + 1;
	//buckStep = buckStep * numLevel; // 没用
	numBucket = (valDom - 1) / buckStep + 1;

	cout << "ExpID = " << expID << ". HEMSC: numLevel = " << numLevel << ", levelStep = " << levelStep << ", numBucket = " << numBucket << ", bucketStep = " << buckStep << ", bit exponent = " << be << endl;

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<vector<Combo>>>(numLevel, vector<vector<Combo>>(numBucket)));
	data[1].resize(numDimension, vector<vector<vector<Combo>>>(numLevel, vector<vector<Combo>>(numBucket)));

	if (be == -1)
		numBits = be2;
	else
		numBits = pow(2, be);  // 每个维度上lowValue对应的bits数组个数
	//if (numBits > 1) {
	fullBits.resize(numDimension);  // 维度总数永远不变，所以只需要resize一次
	fullBL.resize(numDimension, vector<bitset<subs>>(numLevel));
	//}

	doubleReverse[0] = new bool** [numDimension];
	doubleReverse[1] = new bool** [numDimension];
	endBucket[0] = new int** [numDimension];
	endBucket[1] = new int** [numDimension];
	bitsID[0] = new int** [numDimension];
	bitsID[1] = new int** [numDimension];
	_for(lh, 0, 2) // 0 or 1 low/high
		_for(i, 0, numDimension) {
		doubleReverse[lh][i] = new bool* [numLevel];
		endBucket[lh][i] = new int* [numLevel];
		bitsID[lh][i] = new int* [numLevel];
		_for(j, 0, numLevel) {
			doubleReverse[lh][i][j] = new bool[numBucket];
			endBucket[lh][i][j] = new int[numBucket];
			bitsID[lh][i][j] = new int[numBucket];
		}
	}

	fix[0].resize(numDimension, vector<vector<int>>(numLevel, vector<int>(numBucket + 1, 0)));
	fix[1].resize(numDimension, vector<vector<int>>(numLevel, vector<int>(numBucket + 1, 0)));
}

HEMSC::~HEMSC() {
	_for(lh, 0, 2) { // 0 or 1 low/high
		_for(i, 0, numDimension) {
			_for(j, 0, numLevel)
				delete[] doubleReverse[lh][i][j], endBucket[lh][i][j], bitsID[lh][i][j];
			delete[] doubleReverse[lh][i], endBucket[lh][i], bitsID[lh][i];
		}
		delete[] endBucket[lh], bitsID[lh], doubleReverse[lh];
	}
}

void HEMSC::insert(IntervalSub sub)
{
	for (int i = 0; i < sub.size; i++)
	{
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		//int li = (cnt.highValue - cnt.lowValue) / levelStep;
		int li = cnt.highValue / levelStep;
		//int li = cnt.lowValue / levelStep;

		/*int li;
		if (cnt.highValue <= valDom / 2)
			li = 0;
		else if (cnt.lowValue >= valDom / 2)
			li = 2;
		else li = 1;*/
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][li][cnt.lowValue / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][li][cnt.highValue / buckStep].push_back(c);
	}
	numSub++;
}

// fullBits单独存储的版本
void HEMSC::initBits() {

	// 如果有多次初始化
	_for(lh, 0, 2) { // 0 or 1 low/high
		_for(i, 0, numDimension) {
			_for(j, 0, numLevel)
				delete[] doubleReverse[lh][i][j], endBucket[lh][i][j], bitsID[lh][i][j];
			delete[] doubleReverse[lh][i], endBucket[lh][i], bitsID[lh][i];
		}
		delete[] endBucket[lh], bitsID[lh], doubleReverse[lh];
	}
	doubleReverse[0] = new bool** [numDimension];
	doubleReverse[1] = new bool** [numDimension];
	endBucket[0] = new int** [numDimension];
	endBucket[1] = new int** [numDimension];
	bitsID[0] = new int** [numDimension];
	bitsID[1] = new int** [numDimension];
	_for(lh, 0, 2) // 0 or 1 low/high
		_for(i, 0, numDimension) {
		doubleReverse[lh][i] = new bool* [numLevel];
		endBucket[lh][i] = new int* [numLevel];
		bitsID[lh][i] = new int* [numLevel];
		_for(j, 0, numLevel) {
			doubleReverse[lh][i][j] = new bool[numBucket];
			endBucket[lh][i][j] = new int[numBucket];
			bitsID[lh][i][j] = new int[numBucket];
		}
	}
	bits[0].clear(), bits[1].clear();
	bits[0].resize(numDimension, vector<vector<bitset<subs>>>(numLevel, vector<bitset<subs>>(numBits - 1)));
	bits[1].resize(numDimension, vector<vector<bitset<subs>>>(numLevel, vector<bitset<subs>>(numBits - 1)));

	// 前缀和、后缀和数组, 不包括本身
	//_for(i, 0, numDimension) {
	//	_for(j, 0, numLevel) {
	//		_for(k, 1, numBucket) {
	//			fix[0][i][j][numBucket - 1 - k] = fix[0][i][j][numBucket - k] + data[0][i][j][numBucket - k].size();
	//			fix[1][i][j][k] = fix[1][i][j][k - 1] + data[1][i][j][k - 1].size();
	//		}
	//		// 整个数组的和存在最后一个元素上
	//		fix[0][i][j][numBucket] = fix[0][i][j][0] + data[0][i][j][0].size();
	//		fix[1][i][j][numBucket] = fix[1][i][j][numBucket - 1] + data[1][i][j][numBucket - 1].size();
	//	}
	//}

	// 前缀和数组(不包括本身)、后缀和数组(包括本身)
	_for(i, 0, numDimension) {
		_for(j, 0, numLevel) {
			fix[0][i][j][numBucket - 1] = data[0][i][j][numBucket - 1].size();
			_for(k, 1, numBucket) {
				fix[0][i][j][numBucket - 1 - k] = fix[0][i][j][numBucket - k] + data[0][i][j][numBucket - k - 1].size();
				fix[1][i][j][k] = fix[1][i][j][k - 1] + data[1][i][j][k - 1].size();
			}

			//fix[0][i][j][numBucket] = fix[0][i][j][0];  // 后缀和数组不用存和
			fix[1][i][j][numBucket] = fix[1][i][j][numBucket - 1] + data[1][i][j][numBucket - 1].size();
		}
	}

	//if (numBits == 1) { // 只有一个bits时特判，不用fullBits
	//	_for(i, 0, numDimension) {
	//		_for(j, 0, numLevel) {
	//			int halfWorkLoad = fix[1][i][j][numBucket] >> 1; // subWorkLoadStep  fix[0][i][j][0]
	//			int quarterWorkLoad = halfWorkLoad >> 1;
	//			// 第一个后/前缀和包含一半订阅的桶ID，bit数组最远正好覆盖到lowHalfPoint和highHalfPoint-1
	//			int lowHalfPoint = -1, lowQuarterPoint = -1, highHalfPoint = -1, highQuarterPoint = -1;
	//			_for(k, 0, numBucket) {
	//				if (lowQuarterPoint == -1 && fix[0][i][j][numBucket - 1 - k] >= quarterWorkLoad)
	//					lowQuarterPoint = numBucket - 1 - k;
	//				else if (lowHalfPoint == -1 && fix[0][i][j][numBucket - 1 - k] >= halfWorkLoad)
	//					lowHalfPoint = numBucket - 1 - k;

	//				if (highQuarterPoint == -1 && fix[1][i][j][k] >= quarterWorkLoad)
	//					highQuarterPoint = k;
	//				else if (highHalfPoint == -1 && fix[1][i][j][k] >= halfWorkLoad)
	//					highHalfPoint = k;
	//			}

	//			_for(k, 0, numBucket) {
	//				if (k < lowHalfPoint) {                    // 可以用上bitset
	//					bitsID[0][i][j][k] = 0;
	//					endBucket[0][i][j][k] = lowHalfPoint;  // 遍历到小于 lowCriticalPoint
	//					doubleReverse[0][i][j][k] = false;
	//				}
	//				else if (k < lowQuarterPoint) {
	//					bitsID[0][i][j][k] = 0;
	//					endBucket[0][i][j][k] = lowHalfPoint;  // 从 k 二重反向遍历到等于 lowCriticalPoint(都包含)
	//					doubleReverse[0][i][j][k] = true;
	//					_for(q, 0, data[0][i][j][k].size())    // 桶里每个订阅
	//						bits[0][i][j][0][data[0][i][j][k][q].subID] = 1;
	//				}
	//				else {
	//					bitsID[0][i][j][k] = -1;
	//					endBucket[0][i][j][k] = numBucket;
	//					doubleReverse[0][i][j][k] = false;
	//					_for(q, 0, data[0][i][j][k].size())    // 桶里每个订阅
	//						bits[0][i][j][0][data[0][i][j][k][q].subID] = 1;
	//				}
	//				if (k < highQuarterPoint) {                // 不可以用bitset
	//					bitsID[1][i][j][k] = -1;
	//					endBucket[1][i][j][k] = 0;             // 遍历到等于0
	//					doubleReverse[1][i][j][k] = false;
	//					_for(q, 0, data[1][i][j][k].size())    // 桶里每个订阅
	//						bits[1][i][j][0][data[1][i][j][k][q].subID] = 1;
	//				}
	//				else if (k < highHalfPoint) {
	//					bitsID[1][i][j][k] = 0;
	//					endBucket[1][i][j][k] = highHalfPoint;  // 从 j 二重反向遍历到大于等于 highCriticalPoint
	//					doubleReverse[1][i][j][k] = true;
	//					_for(q, 0, data[1][i][j][k].size())     // 桶里每个订阅
	//						bits[1][i][j][0][data[1][i][j][k][q].subID] = 1;
	//				}
	//				else {
	//					bitsID[1][i][j][k] = 0;
	//					endBucket[1][i][j][k] = highHalfPoint;  // 从 j-1 遍历到大于等于 highHalfPoint, 和以前保持一致
	//					doubleReverse[1][i][j][k] = false;
	//				}
	//			}
	//		}  // level
	//	}      // dimension
	//	//cout << "HEMSCDD Stop.\n";
	//	return;
	//}

	// 当前应该映射到的bitId, 桶id, 下一个临界负载点
	int lowBid, highBid, lowBktId, highBktId, lowSubWorkLoad, highSubWorkLoad;
	int subWorkLoadStep; // 每个维度上的subWorkLoadStep都不同, 但同一个维度上的low/high subWorkLoadStep是一样的
	_for(i, 0, numDimension) {
		_for(j, 0, numLevel) {

			if (fix[1][i][j][numBucket] == 0) {
				_for(k, 0, numBucket) {
					bitsID[0][i][j][k] = -1;
					endBucket[0][i][j][k] = k;
					doubleReverse[0][i][j][k] = false;
					bitsID[1][i][j][k] = -1;
					endBucket[1][i][j][k] = k;       // 遍历到大于等于endBucket[1][i][j]
					doubleReverse[1][i][j][k] = false;
				}
				continue;
			}

			subWorkLoadStep = (fix[1][i][j][numBucket] + numBits - 1) / numBits; // fix[0][i][j][0]

		// 由于是low/high都是动态的, 基本不可能共用同一套partition/cell,
		// 但这里low还是从左边开始数一个subWorkLoadStep的量, 保持一致      
		// 或者从右边数 剩余负载量 开始累加subWorkLoadStep, 否则不清楚endBucket!
		// 0号low桶一定可以用到以 (numBits - 2) 为下标的bitset
		// 最后一个桶一定用不到bitset
		// 举例: numBits=15, fix[0][i][numBucket]=1000000, subWorkLoadStep=66667 (low上的后14个多1, high上的前14个多1)
		// fix[0][i][numBucket] / subWorkLoadStep=14, lowSubWorkLoad=66662
			lowBid = -1;
			lowBktId = numBucket;
			lowSubWorkLoad = fix[0][i][j][0] - (fix[0][i][j][0] - 1) / subWorkLoadStep * subWorkLoadStep;
			highBid = -1;
			highBktId = 0;
			highSubWorkLoad = subWorkLoadStep;

			// lowContain[i]=右数(第一个覆盖)lowSubWorkLoad+(i-1)*subWorkLoadStep个订阅所到的桶号(i>0时)
			vector<int> lowContain(numBits + 1, numBucket);
			// highContain[i]=左数i*subWorkLoadStep个订阅所到的桶号+1（因为前缀和没包括本身）
			vector<int> highContain(numBits + 1, 0);
			int li = 1, hi = 1; // lowContain和highContain的下标
			_for(k, 0, numBucket) {
				if (fix[1][i][j][k] >= highSubWorkLoad) {
					highContain[hi++] = k;
					highSubWorkLoad += subWorkLoadStep;
				}
				if (fix[0][i][j][numBucket - k - 1] >= lowSubWorkLoad) {
					lowContain[li++] = numBucket - k - 1;
					lowSubWorkLoad += subWorkLoadStep;
				}
			}
			//lowContain[li] = 0;
			if (hi == numBits)  // Bug: 最后几个桶为空时hi会在for循环里增加到numBits+1
				highContain[hi] = numBucket;

			li = hi = 1; // 双重反向遍历时所对应的另一端的桶号在contain数组中的下标, 其实 li=lowBid+2, hi=highBid+2
			lowSubWorkLoad = fix[0][i][j][0] - (fix[0][i][j][0] - 1) / subWorkLoadStep * subWorkLoadStep;
			highSubWorkLoad = subWorkLoadStep;
			_for(k, 0, numBucket) {
				// 此时大于等于highSubWorkLoad了, 可以用bits, 因为bits覆盖到j-1桶
				if (fix[1][i][j][k] >= highSubWorkLoad) {    // 第一个大于等于临界点的桶(k-1号, 前缀和不包含本身)作为bitset覆盖的终点桶
					highSubWorkLoad += subWorkLoadStep;
					hi++;
					highBid++;
					highBktId = k;
				}
				//if (i == 5 && j == 0 && k == 827) {
				//	cout << "error\n";
				//}
				// Bug: 提前满了, 最后几个桶为空, 此时highBid=numBits-1, 越界了, 直接用fullBL
				if (fix[1][i][j][k] == fix[1][i][j][numBucket]) {
					bitsID[1][i][j][k] = numBits - 1;
					endBucket[1][i][j][k] = j + 1; // 如果是第一次进来, j号桶非空, 需要二重反向标记, 否则是空桶, 可以兼容这种情况
					doubleReverse[1][i][j][k] = true;
				}
				else if (fix[1][i][j][k] - fix[1][i][j][highBktId] <= fix[1][i][j][highContain[hi]] - fix[1][i][j][k]) {     // Bug: 没有减highBktId
					bitsID[1][i][j][k] = highBid;
					endBucket[1][i][j][k] = highBktId;       // 遍历到大于等于endBucket[1][i][j]
					doubleReverse[1][i][j][k] = false;
				}
				else {
					bitsID[1][i][j][k] = hi - 1;             // highBid+1
					endBucket[1][i][j][k] = highContain[hi]; // 从j往右遍历到小于endBucket[1][i][j]
					doubleReverse[1][i][j][k] = true;
				}

				// 后缀数组求和时包括本身(如果不包括本身, 则在两个k、lowBktId和lowContain[li]后再减一，而lowContain[li]有可能为0)
				// fix[0][i][j][numBucket]需要是0, 使fix[0][i][j][lowBktId]刚开始为0
				// Bug: 提前满了, 序号小的几个桶为空, 单独考虑, 直接用二重反向
				if (fix[0][i][j][numBucket - k - 1] == fix[0][i][j][0]) {
					bitsID[0][i][j][numBucket - k - 1] = numBits - 1;
					endBucket[0][i][j][numBucket - k - 1] = numBucket - k - 1;
					doubleReverse[0][i][j][numBucket - k - 1] = true;
				}
				else if (fix[0][i][j][numBucket - k] - fix[0][i][j][lowBktId] <= fix[0][i][j][lowContain[li]] - fix[0][i][j][numBucket - k]) {
					bitsID[0][i][j][numBucket - k - 1] = lowBid;
					endBucket[0][i][j][numBucket - k - 1] = lowBktId;
					doubleReverse[0][i][j][numBucket - k - 1] = false;
				}
				else {
					bitsID[0][i][j][numBucket - k - 1] = li - 1;  // lowBktId+1
					endBucket[0][i][j][numBucket - k - 1] = lowContain[li];
					doubleReverse[0][i][j][numBucket - k - 1] = true;
				}

				// 此时虽然大于等于lowSubWorkLoad了, 但仍不可以用bits, 因为bits要覆盖到j号桶
				if (fix[0][i][j][numBucket - k - 1] >= lowSubWorkLoad) {
					lowSubWorkLoad += subWorkLoadStep;
					li++;
					lowBid++;
					lowBktId = numBucket - k - 1;
				}
			}
		}   // level
	}       // dimension

	int subID, b;                       // 起始标记数组的下标
	_for(i, 0, numDimension) {          // 每个维度
		_for(j, 0, numLevel) {          // 每层
			_for(k, 0, numBucket) {     // 每个桶
				if (doubleReverse[0][i][j][k])
					b = bitsID[0][i][j][k];       // 最小的需要插入的bits数组的ID
				else b = bitsID[0][i][j][k] + 1;
				_for(q, 0, data[0][i][j][k].size()) {
					subID = data[0][i][j][k][q].subID;
					fullBits[i][subID] = 1;    // 0号bits每次必须标记  
					fullBL[i][j][subID] = 1;
					_for(p, b, numBits - 1) // Bug: bits都是是从高位(覆盖广)往低位遍历！
						bits[0][i][j][p][subID] = 1;
				}

				if (doubleReverse[1][i][j][k])
					b = bitsID[1][i][j][k];
				else b = bitsID[1][i][j][k] + 1;  // 最小的需要插入的bits数组的ID
				_for(q, 0, data[1][i][j][k].size()) {     // 桶里每个订阅
					subID = data[1][i][j][k][q].subID;
					_for(p, b, numBits - 1)
						bits[1][i][j][p][subID] = 1;
				}
			}
		}
	}
	//cout << "HEMSCDD Stop.\n";
}

// 不计算时间组成
void HEMSC::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> b, bLocal;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;

	_for(i, 0, pub.size)
	{
		// 落入每层的桶号一样...
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;
		_for(j, 0, numLevel) {
			_for(q, 0, data[0][att][j][buck].size())
				if (data[0][att][j][buck][q].val > value)
					b[data[0][att][j][buck][q].subID] = 1;
			_for(q, 0, data[1][att][j][buck].size())
				if (data[1][att][j][buck][q].val < value)
					b[data[1][att][j][buck][q].subID] = 1;

			if (doubleReverse[0][att][j][buck]) {
				if (bitsID[0][att][j][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉了: && numBits > 1
					bLocal = fullBL[att][j];
				else
					bLocal = bits[0][att][j][bitsID[0][att][j][buck]];
				_for(k, endBucket[0][att][j][buck], buck + 1)
					_for(q, 0, data[0][att][j][k].size())
					bLocal[data[0][att][j][k][q].subID] = 0;

				b = b | bLocal;
			}
			else {
				_for(k, buck + 1, endBucket[0][att][j][buck])
					_for(q, 0, data[0][att][j][k].size())
					b[data[0][att][j][k][q].subID] = 1;

				if (bitsID[0][att][j][buck] != -1)
					b = b | bits[0][att][j][bitsID[0][att][j][buck]];
			}

			if (doubleReverse[1][att][j][buck]) {
				if (bitsID[1][att][j][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉了: && numBits > 1
					bLocal = fullBL[att][j];
				else
					bLocal = bits[1][att][j][bitsID[1][att][j][buck]];
				_for(k, buck, endBucket[1][att][j][buck])
					_for(q, 0, data[1][att][j][k].size())
					bLocal[data[1][att][j][k][q].subID] = 0;
				b = b | bLocal;
			}
			else {
				_for(k, endBucket[1][att][j][buck], buck)
					_for(q, 0, data[1][att][j][k].size())
					b[data[1][att][j][k][q].subID] = 1;
				if (bitsID[1][att][j][buck] != -1)
					b = b | bits[1][att][j][bitsID[1][att][j][buck]]; // Bug: 是att不是i
			}
		}
	}

	//if (numBits > 1) {
	_for(i, 0, numDimension)
		if (!attExist[i])
			b = b | fullBits[i];
	/*}
	else {
		_for(i, 0, numDimension)
			if (!attExist[i])
				_for(j, 0, numLevel)
				_for(k, 0, endBucket[0][i][j][0])
				_for(q, 0, data[0][i][j].size())
				b[data[0][i][j][k][q].subID] = 1;

		_for(i, 0, numDimension)
			if (!attExist[i])
				_for(j, 0, numLevel)
				b = b | bits[0][i][j][0];
	}*/

	//	_for(i, 0, subs)
	//		if (!b[i])
	//		{
	//			++matchSubs;
	//			//cout << "HEMSC matches sub: " << i << endl;
	//		}
	matchSubs = subs - b.count();
}

// 计算时间组成
void HEMSC::match_debug(const Pub& pub, int& matchSubs)
{
	bitset<subs> b, bLocal;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;

	_for(i, 0, pub.size)
	{
		// 落入每层的桶号一样...
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;
		_for(j, 0, numLevel) {
			Timer compareStart;
			_for(q, 0, data[0][att][j][buck].size())
				if (data[0][att][j][buck][q].val > value)
					b[data[0][att][j][buck][q].subID] = 1;
			_for(q, 0, data[1][att][j][buck].size())
				if (data[1][att][j][buck][q].val < value)
					b[data[1][att][j][buck][q].subID] = 1;
			compareTime += (double)compareStart.elapsed_nano();

			if (doubleReverse[0][att][j][buck]) {
				Timer markStart;
				if (bitsID[0][att][j][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉了: && numBits > 1
					bLocal = fullBL[att][j];
				else
					bLocal = bits[0][att][j][bitsID[0][att][j][buck]];
				_for(k, endBucket[0][att][j][buck], buck + 1)
					_for(q, 0, data[0][att][j][k].size())
					bLocal[data[0][att][j][k][q].subID] = 0;
				markTime += (double)markStart.elapsed_nano();

				Timer orStart;
				b = b | bLocal;
				orTime += (double)orStart.elapsed_nano();
			}
			else {
				Timer markStart;
				_for(k, buck + 1, endBucket[0][att][j][buck])
					_for(q, 0, data[0][att][j][k].size())
					b[data[0][att][j][k][q].subID] = 1;
				markTime += (double)markStart.elapsed_nano();

				Timer orStart;
				if (bitsID[0][att][j][buck] != -1)
					b = b | bits[0][att][j][bitsID[0][att][j][buck]];
				orTime += (double)orStart.elapsed_nano();
			}

			if (doubleReverse[1][att][j][buck]) {
				Timer markStart;
				if (bitsID[1][att][j][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉了: && numBits > 1
					bLocal = fullBL[att][j];
				else
					bLocal = bits[1][att][j][bitsID[1][att][j][buck]];
				_for(k, buck, endBucket[1][att][j][buck])
					_for(q, 0, data[1][att][j][k].size())
					bLocal[data[1][att][j][k][q].subID] = 0;
				markTime += (double)markStart.elapsed_nano();
				Timer orStart;
				b = b | bLocal;
				orTime += (double)orStart.elapsed_nano();
			}
			else {
				Timer markStart;
				_for(k, endBucket[1][att][j][buck], buck)
					_for(q, 0, data[1][att][j][k].size())
					b[data[1][att][j][k][q].subID] = 1;
				markTime += (double)markStart.elapsed_nano();
				Timer orStart;
				if (bitsID[1][att][j][buck] != -1)
					b = b | bits[1][att][j][bitsID[1][att][j][buck]]; // Bug: 是att不是i
				orTime += (double)orStart.elapsed_nano();
			}
		}
	}

	//if (numBits > 1) {
	Timer orStart;
	_for(i, 0, numDimension)
		if (!attExist[i])
			b = b | fullBits[i];
	orTime += (double)orStart.elapsed_nano();
	/*}
	else {
		Timer markStart;
		_for(i, 0, numDimension)
			if (!attExist[i])
				_for(j, 0, numLevel)
				_for(k, 0, endBucket[0][i][j][0])
				_for(q, 0, data[0][i][j].size())
				b[data[0][i][j][k][q].subID] = 1;
		markTime += (double)markStart.elapsed_nano();

		Timer orStart;
		_for(i, 0, numDimension)
			if (!attExist[i])
				_for(j, 0, numLevel)
					b = b | bits[0][i][j][0];
		orTime += (double)orStart.elapsed_nano();
	}*/

	Timer bitStart;
	//_for(i, 0, subs)
	//	if (!b[i])
	//	{
	//		++matchSubs;
	//		//cout << "HEMSC matches sub: " << i << endl;
	//	}
	matchSubs = subs - b.count();
	bitTime += (double)bitStart.elapsed_nano();
}

//void HEMSC::calBucketSize() {
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

int HEMSC::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, numDimension) {
		_for(j, 0, numLevel) {
			// 若每个维度上bits数组个数一样就是 2*sizeof(bitset<subs>)*numDimension*numBits*numLevel
			size += sizeof(bitset<subs>) * (bits[0][i][j].size() + bits[1][i][j].size());
			_for(k, 0, numBucket)
				size += sizeof(Combo) * (data[0][i][j][k].size() + data[1][i][j][k].size());
		}
	}

	// fullBits
	//if (numBits > 1) {
	size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()即numDimension
	size += sizeof(bitset<subs>) * numDimension * numLevel; // fullBL
//}


// 两个fix
	size += sizeof(int) * numDimension * numLevel * (numBucket + 1);
	// 两个endBucket、两个bitsID、两个doubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numLevel * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void HEMSC::printRelation(int dimension_i, int li) { // 维度号和层号
	cout << "\n\nHEMSCDDMap\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension) {
		_for(j, 0, numLevel) {
			cout << "\nDimension " << i << "  Level " << j << "    LowBucket Predicates: " << fix[0][i][j][0] << "   ----------------\n";
			_for(k, 0, numBucket) {
				cout << "lBkt" << k << ": bID=" << bitsID[0][i][j][k] << ", eBkt=" << endBucket[0][i][j][k] << ", dRvs=" << doubleReverse[0][i][j][k] << "; ";
				if (k % 5 == 0 && k > 0)cout << "\n";
			}
			cout << "\n\nDimension " << i << "  Level " << j << "    HighBucket Predicates: " << fix[1][i][j][numBucket] << "   ----------------\n";
			_for(k, 0, numBucket) {
				cout << "hBkt" << k << ": bID=" << bitsID[1][i][j][k] << ", eBkt=" << endBucket[1][i][j][k] << ", dRvs=" << doubleReverse[1][i][j][k] << "; ";
				if (k % 5 == 0 && k > 0)cout << "\n";
			}
		}
	}
	else {
		cout << "\nDimension: " << dimension_i << "  Level " << li << "    LowBucket Predicates: " << fix[0][dimension_i][li][0] << "   ----------------\n";
		_for(k, 0, numBucket) {
			cout << "lBkt" << k << ": bID=" << bitsID[0][dimension_i][li][k] << ", eBkt=" << endBucket[0][dimension_i][li][k] << ", dRvs=" << doubleReverse[0][dimension_i][li][k] << "; ";
			if (k % 5 == 0 && k > 0)cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "  Level " << li << "    HighBucket Predicates: " << fix[1][dimension_i][li][numBucket] << "   ----------------\n";
		_for(k, 0, numBucket) {
			cout << "hBkt" << k << ": bID=" << bitsID[1][dimension_i][li][k] << ", eBkt=" << endBucket[1][dimension_i][li][k] << ", dRvs=" << doubleReverse[1][dimension_i][li][k] << "; ";
			if (k % 5 == 0 && k > 0)cout << "\n";
		}
	}
	cout << "\n\n";
}
