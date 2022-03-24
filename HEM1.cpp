#include "HEM1.h"

HEM1::HEM1(){
	numSub = 0;
	numDimension = atts;
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
    cout <<"ExpID = "<<expID<< ". HEM1SS: bit exponent = "<< be<<", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
	// 如果桶数会变化，以下代码也要放入init函数里
	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (be == -1)
		numBits = be2;
	else
		numBits = pow(2, be);  // 每个维度上lowValue对应的bits数组个数
	//if (numBits > 1) {
		fullBits.resize(numDimension);  // 维度总数永远不变，所以只需要resize一次
		bitStep = (numBucket + numBits - 1) / numBits;  // 每过这么远新设一个bits
		// 等价写法：bitStep = numBucket % numBits == 0 ? numBucket / numBits : numBucket / numBits + 1;
	//}
	//else bitStep = numBucket >> 1;

	doubleReverse[0] = new bool[numBucket];
	doubleReverse[1] = new bool[numBucket];
	endBucket[0] = new int[numBucket];
	endBucket[1] = new int[numBucket];
	bitsID[0] = new int[numBucket];
	bitsID[1] = new int[numBucket];

	//fix[0].resize(numDimension, vector<int>(numBucket + 1, 0));
	//fix[1].resize(numDimension, vector<int>(numBucket + 1, 0));
}

HEM1::~HEM1() {
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
}

void HEM1::insert(IntervalSub sub)
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
void HEM1::initBits() {

	// 如果有多次初始化
	delete[] doubleReverse[0], doubleReverse[1], endBucket[0], endBucket[1], bitsID[0], bitsID[1];
	doubleReverse[0] = new bool[numBucket];
	doubleReverse[1] = new bool[numBucket];
	endBucket[0] = new int[numBucket];
	endBucket[1] = new int[numBucket];
	bitsID[0] = new int[numBucket];
	bitsID[1] = new int[numBucket];
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
	//	fix[1][i][numBucket] = fix[1][i][numBucket] + data[1][i][numBucket - 1].size();
	//}

	//if (numBits == 1) {                           // 只有一个bits时特判，不用fullBits

	//	_for(j, 0, numBucket >> 1) {
	//		// 此时low这一端一定用到也只能用到0号bits数组
	//		bitsID[0][j] = 0;                     // 此时的0号代表0.5~1
	//		endBucket[0][j] = numBucket >> 1;     // 标记时遍历到小于这个值
	//		doubleReverse[0][j] = false;          // Bug：这个也要赋值（没初始化），找了一个多小时
	//		int bid1 = -1, bid2 = 0;
	//		int bktid1 = 0, bktid2 = bitStep;     // Bug: bitStep是0不是500! 折磨多天的毒瘤!???没错吧
	//		int midid = (bktid1 + bktid2) / 2;

	//		if (j <= midid) {
	//			bitsID[1][j] = bid1;              // 为-1时表示确实用不到bits数组
	//			endBucket[1][j] = bktid1;         // 往左标记1时从 j-1 遍历到 bktid1 号桶
	//			doubleReverse[1][j] = false;
	//		}
	//		else {
	//			bitsID[1][j] = bid2;
	//			endBucket[1][j] = bktid2;         // 二重反向标记0时从 j 遍历到 bktid2 - 1 号桶
	//			doubleReverse[1][j] = true;
	//		}
	//	}
	//	_for(j, numBucket >> 1, numBucket) {
	//		// 此时high这一端一定用到也只能用到0号bits数组
	//		bitsID[1][j] = 0;
	//		endBucket[1][j] = numBucket >> 1;     // 标记时遍历到等于这个值 Bug: bitStep是0不是500! ??? 是500吧
	//		doubleReverse[1][j] = false;

	//		int bid1 = -1, bid2 = 0;
	//		int bktid1 = numBucket, bktid2 = numBucket - (numBucket >> 1);// 1000-500
	//		int midid = (bktid1 + bktid2) / 2;
	//		if (j < midid) {
	//			bitsID[0][j] = bid2;              // 为-1时表示确实用不到bits数组
	//			endBucket[0][j] = bktid2;         // 往右标记1时从 j+1 遍历到 bktid1-1 号桶
	//			doubleReverse[0][j] = true;
	//		}
	//		else {
	//			bitsID[0][j] = bid1;
	//			endBucket[0][j] = bktid1;         // 二重反向标记0时从 bktid2 遍历到 j 号桶
	//			doubleReverse[0][j] = false;
	//		}
	//	}

	//	// 这段标记与上面的映射分离出来了，可不分先后执行
	//	_for(i, 0, numDimension) {                // 每个维度
	//		_for(j, 0, numBucket >> 1)            // 每个左半部分的桶
	//			_for(k, 0, data[1][i][j].size())  // 桶里每个订阅
	//			bits[1][i][0][data[1][i][j][k].subID] = 1;  // Bug: high不是low, i维, 0号bits, subID
	//		_for(j, numBucket >> 1, numBucket)    // 每个右半部分的桶
	//			_for(k, 0, data[0][i][j].size())  // 桶里每个订阅
	//			bits[0][i][0][data[0][i][j][k].subID] = 1;  // high, i维, 0号bits, subID
	//	}
	//	//cout << "Stop.\n";
	//	return;
	//}

	_for(i, 0, numDimension)
		_for(j, 0, numBucket) {
		int bid1 = j / bitStep - 1, bid2 = bid1 + 1;   // 62/63-1=-1, 63/63-1=0, -1+1=0     945/63-1=14, 15代表用fullBits
		int bktid1 = (bid1 + 1) * bitStep, bktid2 = min(bktid1 + bitStep, numBucket); // (-1+1)*63=0 <= 62, (0+1)*63=63>62
		int midid = (bktid1 + bktid2) / 2;
		if (j <= midid) {
			bitsID[1][j] = bid1;      // 为-1时表示确实用不到bits数组
			endBucket[1][j] = bktid1; // 往左标记1时从 j-1 遍历到 bktid1 号桶
			doubleReverse[1][j] = false;
		}
		else {
			// numBits>1, bid2=numBits-1时表示用fullBits
			bitsID[1][j] = bid2;        // bid2比原来的id多了1，后面进行插入标记的时候需要根据减一来判断
			endBucket[1][j] = bktid2;   // 二重反向标记0时从 j 遍历到 bktid2 - 1 号桶
			doubleReverse[1][j] = true;
		}

		//bid1 = (numBucket - j - 1) / bitStep - 1, bid2 = bid1 + 1;// 2,3; 1,2; 0,1; -1,0; // 桶数不是bits数组数倍数时不能共用同一套cell
		//bktid1 = numBucket - (bid1 + 1) * bitStep, bktid2 = max(0, bktid1 - bitStep); // <bktid1, >=bktid2 250,0; 500,250; 750,500; 1000,750;
		bid1 = numBits - 3 - bid1, bid2 = bid1 + 1;
		bktid1 = bktid1 ^ bktid2;
		bktid2 = bktid1 ^ bktid2;
		bktid1 = bktid1 ^ bktid2;
		if (j >= midid) {
			bitsID[0][j] = bid1;
			endBucket[0][j] = bktid1;
			doubleReverse[0][j] = false; // Bug: 此数组没初始化，可能为true！
		}
		else {
			bitsID[0][j] = bid2;
			endBucket[0][j] = bktid2;
			doubleReverse[0][j] = true;
		}
		}

	int subID, b;   // 起始标记数组的下标
	_for(i, 0, numDimension) {          // 每个维度
		_for(j, 0, numBucket) {         // 每个桶
			if (doubleReverse[0][j])
				b = bitsID[0][j];       // 除了0号外最小的需要插入的bits数组的ID
			else b = bitsID[0][j] + 1;
			_for(k, 0, data[0][i][j].size()) {
				subID = data[0][i][j][k].subID;
				fullBits[i][subID] = 1; // 0号bits每次必须标记   
				_for(q, b, numBits - 1) // Bug: bits都是是从高位(覆盖广)往低位遍历！
					bits[0][i][q][subID] = 1;
			}

			if (doubleReverse[1][j])
				b = bitsID[1][j];
			else b = bitsID[1][j] + 1;  // 除了0号外最小的需要插入的bits数组的ID
			_for(k, 0, data[1][i][j].size()) {     // 桶里每个订阅
				subID = data[1][i][j][k].subID;
				_for(q, b, numBits - 1)
					bits[1][i][q][subID] = 1;
			}
		}
	}
	//cout << "Stop.\n";
}

//// 计算时间组成
//void HEM1::match(const Pub& pub, int& matchSubs)
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
//		if (doubleReverse[0][buck]) {
//			Timer markStart;
//			if (bitsID[0][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
//				bLocal = fullBits[att];
//			else
//				bLocal = bits[0][att][bitsID[0][buck]];
//			_for(j, endBucket[0][buck], buck + 1)
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
//			_for(j, buck + 1, endBucket[0][buck])
//				_for(k, 0, data[0][att][j].size())
//				b[data[0][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			if (bitsID[0][buck] != -1)
//				b = b | bits[0][att][bitsID[0][buck]];
//			orTime += (double)orStart.elapsed_nano();
//		}
//
//		if (doubleReverse[1][buck]) {
//			Timer markStart;
//			if (bitsID[1][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
//				bLocal = fullBits[att];
//			else
//				bLocal = bits[1][att][bitsID[1][buck]];
//			_for(j, buck, endBucket[1][buck])
//				_for(k, 0, data[1][att][j].size())
//				bLocal[data[1][att][j][k].subID] = 0;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			b = b | bLocal;
//			orTime += (double)orStart.elapsed_nano();
//		}
//		else {
//			Timer markStart;
//			_for(j, endBucket[1][buck], buck)
//				_for(k, 0, data[1][att][j].size())
//				b[data[1][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//			Timer orStart;
//			if (bitsID[1][buck] != -1)
//				b = b | bits[1][att][bitsID[1][buck]]; // Bug: 是att不是i
//			orTime += (double)orStart.elapsed_nano();
//		}
//	}
//
////	if (numBits > 1) {
//		Timer orStart;
//		_for(i, 0, numDimension)
//			if (!attExist[i])
//				b = b | fullBits[i];
//		orTime += (double)orStart.elapsed_nano();
////	}
////	else {
////		Timer markStart;
////		_for(i, 0, numDimension)
////			if (!attExist[i])
////				_for(j, 0, bitStep)
////				_for(k, 0, data[0][i][j].size())
////				b[data[0][i][j][k].subID] = 1;
////		markTime += (double)markStart.elapsed_nano();
////
////		Timer orStart;
////		_for(i, 0, numDimension)
////			if (!attExist[i])
////				b = b | bits[0][i][0];
////		orTime += (double)orStart.elapsed_nano();
////	}
//
//	Timer bitStart;
//	_for(i, 0, subs)
//		if (!b[i])
//		{
//			++matchSubs;
//			//cout << "HEM1 matches sub: " << i << endl;
//		}
//	bitTime += (double)bitStart.elapsed_nano();
//}

// 不计算时间组成
 void HEM1::match(const Pub& pub, int& matchSubs)
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

 		if (doubleReverse[0][buck]) {
 			if (bitsID[0][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
 				bLocal = fullBits[att];
 			else
 				bLocal = bits[0][att][bitsID[0][buck]];

 			_for(j, endBucket[0][buck], buck + 1)
 				_for(k, 0, data[0][att][j].size())
 				bLocal[data[0][att][j][k].subID] = 0;

 			b = b | bLocal;
 		}
 		else {
 			_for(j, buck + 1, endBucket[0][buck])
 				_for(k, 0, data[0][att][j].size())
 				b[data[0][att][j][k].subID] = 1;

 			if (bitsID[0][buck] != -1)
 				b = b | bits[0][att][bitsID[0][buck]];
 		}

 		if (doubleReverse[1][buck]) {
 			if (bitsID[1][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
 				bLocal = fullBits[att];
 			else
 				bLocal = bits[1][att][bitsID[1][buck]];

 			_for(j, buck, endBucket[1][buck])
 				_for(k, 0, data[1][att][j].size())
 				bLocal[data[1][att][j][k].subID] = 0;

 			b = b | bLocal;
 		}
 		else {
 			_for(j, endBucket[1][buck], buck)
 				_for(k, 0, data[1][att][j].size())
 				b[data[1][att][j][k].subID] = 1;

 			if (bitsID[1][buck] != -1)
 				b = b | bits[1][att][bitsID[1][buck]]; // Bug: 是att不是i
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
 				_for(j, 0, bitStep)
 				_for(k, 0, data[0][i][j].size())
 				b[data[0][i][j][k].subID] = 1;

 		_for(i, 0, numDimension)
 			if (!attExist[i])
 				b = b | bits[0][i][0];
 	}*/

// 	_for(i, 0, subs)
// 		if (!b[i])
// 		{
// 			++matchSubs;
// 			//cout << "HEM1 matches sub: " << i << endl;
// 		}
	matchSubs = subs - b.count();
 }

//void HEM1::calBucketSize() {
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

int HEM1::calMemory() {
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
	//size += sizeof(int) * numDimension * (numBucket + 1);
	// 两个endBucket、两个bitsID、两个doubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void HEM1::printRelation() {
	cout << "\n\nHEM1SSMap    LowBucket   ----------------\n";
	_for(i, 0, numBucket) {
		cout << "lBkt" << i << ": bID=" << bitsID[0][i] << ", eBkt=" << endBucket[0][i] << ", dRvs=" << doubleReverse[0][i] << "; ";
		if (i % 5 == 0 && i > 0)cout << "\n";
	}
	cout << "\n\nHEM1SSMap    HighBucket   ----------------\n";
	_for(i, 0, numBucket) {
		cout << "hBkt" << i << ": bID=" << bitsID[1][i] << ", eBkt=" << endBucket[1][i] << ", dRvs=" << doubleReverse[1][i] << "; ";
		if (i % 5 == 0 && i > 0)cout << "\n";
	}
	
	cout << "\n\n";
}
