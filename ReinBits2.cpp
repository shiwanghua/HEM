#include "ReinBits2.h"

ReinBits2::ReinBits2(int valDom, int numSubscription, int numDim, int numBuck, int b) :maxValue(valDom), numSub(numSubscription), numDimension(numDim) {
	buckStep = (valDom - 1) / numBuck + 1;
	numBucket = (valDom - 1) / buckStep + 1;
        cout <<"ExpID = "<<expID<< ". ReinBits2: bit exponent = "<< be<<", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
	// È  ç¹ûÍ°Êı»á±ä»¯£¬ÒÔÏÂ´úÂëÒ²Òª·ÅÈëinitº¯ÊıÀï
	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (b == -1)
		numBits = numBucket;
	else
		numBits = pow(2, b);  // Ã¿¸öÎ¬¶ÈÉÏlowValue¶ÔÓ¦µÄbitsÊı×é¸öÊı
	if (numBits > 1) {
		fullBits.resize(numDimension);  // Î¬¶È×ÜÊıÓÀÔ¶²»±ä£¬ËùÒÔÖ»ĞèÒªresizeÒ»´Î
		bitStep = (numBucket + numBits - 1) / numBits;  // Ã¿¹ıÕâÃ´Ô¶ĞÂÉèÒ»¸öbits
		// µÈ¼ÛĞ´·¨£ºbitStep = numBucket % numBits == 0 ? numBucket / numBits : numBucket / numBits + 1;
	}
	else bitStep = numBucket >> 1;

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

	fix[0].resize(numDimension, vector<int>(numBucket + 1, 0));
	fix[1].resize(numDimension, vector<int>(numBucket + 1, 0));
}

ReinBits2::~ReinBits2() {
	_for(i, 0, numDimension)
		delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
}

void ReinBits2::insert(IntervalSub sub)
{
	for (int i = 0; i < sub.size; i++)
	{
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		// int bucketID = cnt.lowValue / buckStep; // Bug: ÕâÀï±»¿ÓÁË
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][cnt.lowValue / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][cnt.highValue / buckStep].push_back(c);
	}
}

// fullBitsµ¥¶À´æ´¢µÄ°æ±¾
void ReinBits2::initBits() {

	// Èç¹ûÓĞ¶à´Î³õÊ¼»¯
	_for(i, 0, numDimension)
		delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] doubleReverse[0], doubleReverse[1], endBucket[0], endBucket[1], bitsID[0], bitsID[1];
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

	// Ç°×ººÍ¡¢ºó×ººÍÊı×é, ²»°üÀ¨±¾Éí
	_for(i, 0, numDimension) {
		_for(j, 1, numBucket) {
			fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
			fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
		}
		// Õû¸öÊı×éµÄºÍ´æÔÚ×îºóÒ»¸öÔªËØÉÏ
		fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
		fix[1][i][numBucket] = fix[1][i][numBucket] + data[1][i][numBucket - 1].size();
	}

	if (numBits == 1) {                           // Ö»ÓĞÒ»¸öbitsÊ±ÌØÅĞ£¬²»ÓÃfullBits

		_for(i, 0, numDimension) {
			_for(j, 0, numBucket >> 1) {
				// ´ËÊ±lowÕâÒ»¶ËÒ»¶¨ÓÃµ½Ò²Ö»ÄÜÓÃµ½0ºÅbitsÊı×é
				bitsID[0][i][j] = 0;                     // ´ËÊ±µÄ0ºÅ´ú±í0.5~1
				endBucket[0][i][j] = numBucket >> 1;     // ±ê¼ÇÊ±±éÀúµ½Ğ¡ÓÚÕâ¸öÖµ
				doubleReverse[0][i][j] = false;          // Bug£ºÕâ¸öÒ²Òª¸³Öµ£¨Ã»³õÊ¼»¯£©£¬ÕÒÁËÒ»¸ö¶àĞ¡Ê±
				int bid1 = -1, bid2 = 0;
				int bktid1 = 0, bktid2 = bitStep;
				int workload1 = fix[1][i][j], workload2 = fix[1][i][bktid2] - fix[1][i][j]; // µÚj¸öÍ°Ò²ĞèÒª°Ñ1±ä³É0£¬ºó½øĞĞ±È½Ï
				if (workload1 <= workload2) {
					bitsID[1][i][j] = bid1;              // Îª-1Ê±±íÊ¾È·ÊµÓÃ²»µ½bitsÊı×é
					endBucket[1][i][j] = bktid1;         // Íù×ó±ê¼Ç1Ê±´Ó j-1 ±éÀúµ½ bktid1 ºÅÍ°
					doubleReverse[1][i][j] = false;
				}
				else {
					bitsID[1][i][j] = bid2;
					endBucket[1][i][j] = bktid2;         // ¶şÖØ·´Ïò±ê¼Ç0Ê±´Ó j ±éÀúµ½ bktid2 - 1 ºÅÍ°
					doubleReverse[1][i][j] = true;
				}
			}
			_for(j, numBucket >> 1, numBucket) {
				// ´ËÊ±highÕâÒ»¶ËÒ»¶¨ÓÃµ½Ò²Ö»ÄÜÓÃµ½0ºÅbitsÊı×é
				bitsID[1][i][j] = 0;
				endBucket[1][i][j] = bitStep;            // ±ê¼ÇÊ±±éÀúµ½µÈÓÚÕâ¸öÖµ
				doubleReverse[1][i][j] = false;          

				int bid1 = -1, bid2 = 0;
				int bktid1 = numBucket, bktid2 = numBucket - bitStep;// 1000-500
				int workload1 = fix[0][i][j], workload2 = fix[0][i][bktid2 - 1] - fix[0][i][j];
				if (workload1 <= workload2) {
					bitsID[0][i][j] = bid1;              // Îª-1Ê±±íÊ¾È·ÊµÓÃ²»µ½bitsÊı×é
					endBucket[0][i][j] = bktid1;         // ÍùÓÒ±ê¼Ç1Ê±´Ó j+1 ±éÀúµ½ bktid1-1 ºÅÍ°
					doubleReverse[0][i][j] = false;
				}
				else {
					bitsID[0][i][j] = bid2;
					endBucket[0][i][j] = bktid2;         // ¶şÖØ·´Ïò±ê¼Ç0Ê±´Ó bktid2 ±éÀúµ½ j ºÅÍ°
					doubleReverse[0][i][j] = true;
				}
			}
		}

		// Õâ¶Î±ê¼ÇÓëÉÏÃæµÄÓ³Éä·ÖÀë³öÀ´ÁË£¬¿É²»·ÖÏÈºóÖ´ĞĞ
		_for(i, 0, numDimension) {                // Ã¿¸öÎ¬¶È
			_for(j, 0, numBucket >> 1)            // Ã¿¸ö×ó°ë²¿·ÖµÄÍ°
				_for(k, 0, data[1][i][j].size())  // Í°ÀïÃ¿¸ö¶©ÔÄ
				bits[1][i][0][data[1][i][j][k].subID] = 1;  // Bug: high²»ÊÇlow, iÎ¬, 0ºÅbits, subID
			_for(j, numBucket >> 1, numBucket)    // Ã¿¸öÓÒ°ë²¿·ÖµÄÍ°
				_for(k, 0, data[0][i][j].size())  // Í°ÀïÃ¿¸ö¶©ÔÄ
				bits[0][i][0][data[0][i][j][k].subID] = 1;  // high, iÎ¬, 0ºÅbits, subID
		}
		//cout << "Stop.\n";
		return;
	}

	_for(i, 0, numDimension)
		_for(j, 0, numBucket) {
		int bid1 = j / bitStep - 1, bid2 = bid1 + 1;   // 62/63-1=-1, 63/63-1=0, -1+1=0     945/63-1=14, 15´ú±íÓÃfullBits
		int bktid1 = (bid1 + 1) * bitStep, bktid2 = min(bktid1 + bitStep, numBucket); // (-1+1)*63=0 <= 62, (0+1)*63=63>62
		int workload1 = fix[1][i][j] - fix[1][i][bktid1], workload2 = fix[1][i][bktid2] - fix[1][i][j]; // µÚj¸öÍ°Ò²ĞèÒª°Ñ1±ä³É0£¬ºó½øĞĞ±È½Ï
		if (workload1 <= workload2) {
			bitsID[1][i][j] = bid1;      // Îª-1Ê±±íÊ¾È·ÊµÓÃ²»µ½bitsÊı×é
			endBucket[1][i][j] = bktid1; // Íù×ó±ê¼Ç1Ê±´Ó j-1 ±éÀúµ½ bktid1 ºÅÍ°
			doubleReverse[1][i][j] = false;
		}
		else {
			// numBits>1, bid2=numBits-1Ê±±íÊ¾ÓÃfullBits
			bitsID[1][i][j] = bid2;        // bid2±ÈÔ­À´µÄid¶àÁË1£¬ºóÃæ½øĞĞ²åÈë±ê¼ÇµÄÊ±ºòĞèÒª¸ù¾İ¼õÒ»À´ÅĞ¶Ï
			endBucket[1][i][j] = bktid2;   // ¶şÖØ·´Ïò±ê¼Ç0Ê±´Ó j ±éÀúµ½ bktid2 - 1 ºÅÍ°
			doubleReverse[1][i][j] = true;
		}

		//bid1 = (numBucket - j - 1) / bitStep - 1, bid2 = bid1 + 1;// 2,3; 1,2; 0,1; -1,0; // Í°Êı²»ÊÇbitsÊı×éÊı±¶ÊıÊ±²»ÄÜ¹²ÓÃÍ¬Ò»Ì×cell
		//bktid1 = numBucket - (bid1 + 1) * bitStep, bktid2 = max(0, bktid1 - bitStep); // <bktid1, >=bktid2 250,0; 500,250; 750,500; 1000,750;
		bid1 = numBits - 3 - bid1, bid2 = bid1 + 1;
		bktid1 = bktid1 ^ bktid2;
		bktid2 = bktid1 ^ bktid2;
		bktid1 = bktid1 ^ bktid2;
		workload1 = fix[0][i][j] - fix[0][i][bktid1 - 1];
		workload2 = fix[0][i][bktid2 > 0 ? bktid2 - 1 : numBucket] - fix[0][i][j];
		if (workload1 <= workload2) {
			bitsID[0][i][j] = bid1;
			endBucket[0][i][j] = bktid1;
			doubleReverse[0][i][j] = false; // Bug: ´ËÊı×éÃ»³õÊ¼»¯£¬¿ÉÄÜÎªtrue£¡
		}
		else {
			bitsID[0][i][j] = bid2;
			endBucket[0][i][j] = bktid2;
			doubleReverse[0][i][j] = true;
		}
	}

	int subID, b;   // ÆğÊ¼±ê¼ÇÊı×éµÄÏÂ±ê
	_for(i, 0, numDimension) {          // Ã¿¸öÎ¬¶È
		_for(j, 0, numBucket) {         // Ã¿¸öÍ°
			if (doubleReverse[0][i][j])
				b = bitsID[0][i][j];       // ³ıÁË0ºÅÍâ×îĞ¡µÄĞèÒª²åÈëµÄbitsÊı×éµÄID
			else b = bitsID[0][i][j] + 1;
			_for(k, 0, data[0][i][j].size()) {
				subID = data[0][i][j][k].subID;
				fullBits[i][subID] = 1; // 0ºÅbitsÃ¿´Î±ØĞë±ê¼Ç   
				_for(q, b, numBits - 1) // Bug: bits¶¼ÊÇÊÇ´Ó¸ßÎ»(¸²¸Ç¹ã)ÍùµÍÎ»±éÀú£¡
					bits[0][i][q][subID] = 1;
			}

			if (doubleReverse[1][i][j])
				b = bitsID[1][i][j];
			else b = bitsID[1][i][j] + 1;  // ³ıÁË0ºÅÍâ×îĞ¡µÄĞèÒª²åÈëµÄbitsÊı×éµÄID
			_for(k, 0, data[1][i][j].size()) {     // Í°ÀïÃ¿¸ö¶©ÔÄ
				subID = data[1][i][j][k].subID;
				_for(q, b, numBits - 1)
					bits[1][i][q][subID] = 1;
			}
		}
	}
	//cout << "Stop.\n";
}

// µ÷ÕûË³Ğò°æ±¾£ºbuck±ê¼ÇÊ±¼äÔÚ±È½ÏÊ±Ò»²¢½øĞĞ 
// ±ê¼ÇÊ±¼äËäÈ»¼õÉÙÁËÒ»µã£¬µ«±È½ÏÊ±¼ä¶àÁËÒ»±¶£¡
//void ReinBits2::match(const Pub& pub, int& matchSubs)
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
//			_for(j, endBucket[0][att][buck], buck)  // buckÁôµ½±È½ÏÊ±ÔÙ±ê¼Ç
//				_for(k, 0, data[0][att][j].size())
//				bLocal[data[0][att][j][k].subID] = 0;
//			markTime += (double)markStart.elapsed_nano();
//
//			Timer compareStart;
//			_for(k, 0, data[0][att][buck].size())
//				if (data[0][att][buck][k].val > value) {
//					b[data[0][att][buck][k].subID] = 1;
//					bLocal[data[0][att][buck][k].subID] = 0;
//				}
//			compareTime += (double)compareStart.elapsed_nano();
//
//			Timer orStart;
//			b = b | bLocal;
//			orTime += (double)orStart.elapsed_nano();
//		}
//		else {
//			Timer compareStart;
//			_for(k, 0, data[0][att][buck].size())
//				if (data[0][att][buck][k].val > value)
//					b[data[0][att][buck][k].subID] = 1;
//			compareTime += (double)compareStart.elapsed_nano();
//
//			Timer markStart;
//			_for(j, buck + 1, endBucket[0][att][buck])
//				_for(k, 0, data[0][att][j].size())
//				b[data[0][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//
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
//			_for(j, buck+1, endBucket[1][att][buck]) // buckÁôµ½±È½ÏÊ±±ê¼Ç
//				_for(k, 0, data[1][att][j].size())
//				bLocal[data[1][att][j][k].subID] = 0;
//			markTime += (double)markStart.elapsed_nano();
//
//			Timer compareStart;
//			_for(k, 0, data[1][att][buck].size())
//				if (data[1][att][buck][k].val < value) {
//					b[data[1][att][buck][k].subID] = 1;
//					bLocal[data[1][att][buck][k].subID] = 0;
//				}		
//			compareTime += (double)compareStart.elapsed_nano();
//
//			Timer orStart;
//			b = b | bLocal;
//			orTime += (double)orStart.elapsed_nano();
//		}
//		else {
//			Timer compareStart;
//			_for(k, 0, data[1][att][buck].size())
//				if (data[1][att][buck][k].val < value)
//					b[data[1][att][buck][k].subID] = 1;
//			compareTime += (double)compareStart.elapsed_nano();
//
//			Timer markStart;
//			_for(j, endBucket[1][att][buck], buck)
//				_for(k, 0, data[1][att][j].size())
//				b[data[1][att][j][k].subID] = 1;
//			markTime += (double)markStart.elapsed_nano();
//
//			Timer orStart;
//			if (bitsID[1][att][buck] != -1)
//				b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: ÊÇatt²»ÊÇi
//			orTime += (double)orStart.elapsed_nano();
//		}
//	}
//
//	Timer orStart;
//	_for(i, 0, numDimension)
//		if (!attExist[i])
//			b = b | fullBits[i];
//	orTime += (double)orStart.elapsed_nano();
//
//	Timer bitStart;
//	_for(i, 0, numSub)
//		if (!b[i])
//		{
//			++matchSubs;
//			//cout << "reinbits2 matches sub: " << i << endl;
//		}
//	bitTime += (double)bitStart.elapsed_nano();
//}

void ReinBits2::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> b, bLocal;
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

		if (doubleReverse[0][att][buck]) {
			Timer markStart;
			if (bitsID[0][att][buck] == numBits - 1 && numBits > 1)
				bLocal = fullBits[att];
			else
				bLocal = bits[0][att][bitsID[0][att][buck]];
			_for(j, endBucket[0][att][buck], buck + 1)
				_for(k, 0, data[0][att][j].size())
				bLocal[data[0][att][j][k].subID] = 0;
			markTime += (double)markStart.elapsed_nano();

			Timer orStart;
			b = b | bLocal;
			orTime += (double)orStart.elapsed_nano();
		}
		else {
			Timer markStart;
			_for(j, buck + 1, endBucket[0][att][buck])
				_for(k, 0, data[0][att][j].size())
				b[data[0][att][j][k].subID] = 1;
			markTime += (double)markStart.elapsed_nano();
			Timer orStart;
			if (bitsID[0][att][buck] != -1)
				b = b | bits[0][att][bitsID[0][att][buck]];
			orTime += (double)orStart.elapsed_nano();
		}

		if (doubleReverse[1][att][buck]) {
			Timer markStart;
			if (bitsID[1][att][buck] == numBits - 1 && numBits > 1)
				bLocal = fullBits[att];
			else
				bLocal = bits[1][att][bitsID[1][att][buck]];
			_for(j, buck, endBucket[1][att][buck])
				_for(k, 0, data[1][att][j].size())
				bLocal[data[1][att][j][k].subID] = 0;
			markTime += (double)markStart.elapsed_nano();
			Timer orStart;
			b = b | bLocal;
			orTime += (double)orStart.elapsed_nano();
		}
		else {
			Timer markStart;
			_for(j, endBucket[1][att][buck], buck)
				_for(k, 0, data[1][att][j].size())
				b[data[1][att][j][k].subID] = 1;
			markTime += (double)markStart.elapsed_nano();
			Timer orStart;
			if (bitsID[1][att][buck] != -1)
				b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: ÊÇatt²»ÊÇi
			orTime += (double)orStart.elapsed_nano();
		}
	}

	Timer orStart;
	_for(i, 0, numDimension)
		if (!attExist[i])
			b = b | fullBits[i];
	orTime += (double)orStart.elapsed_nano();

	Timer bitStart;
	_for(i, 0, numSub)
		if (!b[i])
		{
			++matchSubs;
			//cout << "reinbits2 matches sub: " << i << endl;
		}
	bitTime += (double)bitStart.elapsed_nano();
}

//void ReinBits2::calBucketSize() {
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

int ReinBits2::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, numDimension) {
		// ÈôÃ¿¸öÎ¬¶ÈÉÏbitsÊı×é¸öÊıÒ»Ñù¾ÍÊÇ 2*sizeof(bitset<subs>)*numDimension*numBits
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		_for(j, 0, numBucket)
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}

	// fullBits
	size += sizeof(bitset<subs>) * fullBits.size();

	// Á½¸öfix
	size += sizeof(int) * numDimension * (numBucket + 1);
	// Á½¸öendBucket¡¢Á½¸öbitsID¡¢Á½¸ödoubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void ReinBits2::printRelation(int dimension_i) {
	cout << "\n\nReinBitsMap\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension) {
		cout << "\nDimension " << i << "   ----------------\n";
		_for(j, 0, numBucket)
			cout << "Low Bucket" << j << ": bitsID=" << bitsID[0][i][j] << ", endBucket=" << endBucket[0][i][j] << ", doubleReverse=" << doubleReverse[0][i][j] << endl;
	}
	else {
		cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "LBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i] << ", dRvs=" << doubleReverse[0][dimension_i][i]<<"; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket] << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "HBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i] << ", dRvs=" << doubleReverse[1][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)cout << "\n";
		}
	}
	cout << "\n\n";
}
 
