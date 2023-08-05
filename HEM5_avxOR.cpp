#include "HEM5_avxOR.h"

HEM5_avxOR::HEM5_avxOR()
{
	numSub = 0;
	numDimension = atts;
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "ExpID = " << expID << ". HEM5DD_avxOR" + to_string(BlockSize) + ": bit exponent = " << be
		 << ", bucketStep = " << buckStep
		 << ", numBucket = " << numBucket << endl;

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (be == -1)
		numBits = be2;
	else
		numBits = pow(2, be); // ÿ��ά����lowValue��Ӧ��bits�������?
	//if (numBits > 1)
	fullBits.resize(numDimension); // ά��������Զ���䣬����ֻ��Ҫresizeһ��
	//else bitStep = numBucket >> 1;

	doubleReverse[0] = new bool* [numDimension];
	doubleReverse[1] = new bool* [numDimension];
	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension)
	{
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

HEM5_avxOR::~HEM5_avxOR()
{
	_for(i, 0,
		numDimension) delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
}

void HEM5_avxOR::insert(IntervalSub sub)
{
	for (int i = 0; i < sub.size; i++)
	{
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		// int bucketID = cnt.lowValue / buckStep; // Bug: ���ﱻ����
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][cnt.lowValue / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][cnt.highValue / buckStep].push_back(c);
	}
	numSub++;
}

void HEM5_avxOR::insert_online(IntervalSub sub)
{
	IntervalCnt cnt;
	Combo c;
	int b, bucketID;
	c.subID = sub.id;
	//if (numBits > 1) { // ����������forѭ����ÿ�ζ��ж�һ��
	_for(i, 0, sub.size)
	{
		fullBits[sub.constraints[i].att][sub.id] = 1;
	}
	//}
	_for(i, 0, sub.size)
	{
		cnt = sub.constraints[i];
		bucketID = cnt.lowValue / buckStep;
		c.val = cnt.lowValue;
		data[0][cnt.att][bucketID].push_back(c);
		if (doubleReverse[0][cnt.att][bucketID])
			b = bitsID[0][cnt.att][bucketID];
		else
			b = bitsID[0][cnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][cnt.att][q][sub.id] = 1;

		bucketID = cnt.highValue / buckStep;
		c.val = cnt.highValue;
		data[1][cnt.att][bucketID].push_back(c);
		if (doubleReverse[1][cnt.att][bucketID])
			b = bitsID[1][cnt.att][bucketID];
		else
			b = bitsID[1][cnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][cnt.att][q][sub.id] = 1;
	}
	numSub++;
}

bool HEM5_avxOR::deleteSubscription(IntervalSub sub)
{
	int find = 0;
	IntervalCnt cnt;
	int b, bucketID, id = sub.id;

	//if (numBits > 1) { // ����������forѭ����ÿ�ζ��ж�һ��
	_for(i, 0, sub.size)
	{
		fullBits[sub.constraints[i].att][id] = 0;
	}
	//}

	_for(i, 0, sub.size)
	{
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::const_iterator it;
		for (it = data[0][cnt.att][bucketID].cbegin(); it != data[0][cnt.att][bucketID].cend(); it++)
			if (it->subID == id)
			{
				data[0][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[0][cnt.att][bucketID])
			b = bitsID[0][cnt.att][bucketID];
		else
			b = bitsID[0][cnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][cnt.att][q][sub.id] = 0;

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].cbegin(); it != data[1][cnt.att][bucketID].cend(); it++)
			if (it->subID == id)
			{
				data[1][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[1][cnt.att][bucketID])
			b = bitsID[1][cnt.att][bucketID];
		else
			b = bitsID[1][cnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][cnt.att][q][sub.id] = 0;
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

// fullBits�����洢�İ汾
void HEM5_avxOR::initBits()
{

	// ����ж�γ�ʼ��
	_for(i, 0,
		numDimension) delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
	doubleReverse[0] = new bool* [numDimension];
	doubleReverse[1] = new bool* [numDimension];
	endBucket[0] = new int* [numDimension];
	endBucket[1] = new int* [numDimension];
	bitsID[0] = new int* [numDimension];
	bitsID[1] = new int* [numDimension];
	_for(i, 0, numDimension)
	{
		doubleReverse[0][i] = new bool[numBucket];
		doubleReverse[1][i] = new bool[numBucket];
		endBucket[0][i] = new int[numBucket];
		endBucket[1][i] = new int[numBucket];
		bitsID[0][i] = new int[numBucket];
		bitsID[1][i] = new int[numBucket];
	}
	bits[0].clear(), bits[1].clear();
	bits[0].resize(numDimension, vector<bitset<subs>>(numBits - 1));
	bits[1].resize(numDimension, vector<bitset<subs>>(numBits - 1));

	//// ǰ׺�͡���׺������, ����������
	//_for(i, 0, numDimension) {
	//	_for(j, 1, numBucket) {
	//		fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
	//		fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
	//	}
	//	// ��������ĺʹ������һ��Ԫ����
	//	fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
	//	fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: ����-1!!!
	//}

	// ǰ׺������(����������)����׺������(��������)
	_for(i, 0, numDimension)
	{
		fix[0][i][numBucket] = 0;
		fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
		fix[1][i][0] = 0;
		_for(j, 1, numBucket)
		{
			fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j - 1].size();
			fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
		}
		// ��������ĺʹ������һ��Ԫ����
		//fix[0][i][numBucket] = fix[0][i][0];  // Bug: ���º���տ�ʼ��ӳ���ϵʱfix[0][i][lowBktId]������
		fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: ����-1!!!
	}

	//if (numBits == 1) { // ֻ��һ��bitsʱ���У�����fullBits
	//	_for(i, 0, numDimension) {
	//		int halfWorkLoad = fix[0][i][0] >> 1; // subWorkLoadStep  fix[1][i][numBucket]
	//		int quarterWorkLoad = halfWorkLoad >> 1;
	//		// ��һ����/ǰ׺�Ͱ���һ�붩�ĵ�ͰID��bit������Զ���ø��ǵ�lowHalfPoint��highHalfPoint-1
	//		int lowHalfPoint = -1, lowQuarterPoint = -1, highHalfPoint = -1, highQuarterPoint = -1;
	//		_for(j, 0, numBucket) {
	//			if (lowQuarterPoint == -1 && fix[0][i][numBucket - 1 - j] >= quarterWorkLoad)
	//				lowQuarterPoint = numBucket - 1 - j;
	//			else if (lowHalfPoint == -1 && fix[0][i][numBucket - 1 - j] >= halfWorkLoad)
	//				lowHalfPoint = numBucket - 1 - j;

	//			if (highQuarterPoint == -1 && fix[1][i][j] >= quarterWorkLoad)
	//				highQuarterPoint = j;
	//			else if (highHalfPoint == -1 && fix[1][i][j] >= halfWorkLoad)
	//				highHalfPoint = j;
	//		}

	//		_for(j, 0, numBucket) {
	//			if (j < lowHalfPoint) { // ��������bitset
	//				bitsID[0][i][j] = 0;
	//				endBucket[0][i][j] = lowHalfPoint; // ������С�� lowCriticalPoint
	//				doubleReverse[0][i][j] = false;
	//			}
	//			else if (j < lowQuarterPoint) {
	//				bitsID[0][i][j] = 0;
	//				endBucket[0][i][j] = lowHalfPoint; // �� j ���ط������������? lowCriticalPoint(������)
	//				doubleReverse[0][i][j] = true;
	//				_for(k, 0, data[0][i][j].size()) // Ͱ��ÿ������
	//					bits[0][i][0][data[0][i][j][k].subID] = 1;
	//			}
	//			else {
	//				bitsID[0][i][j] = -1;
	//				endBucket[0][i][j] = numBucket;
	//				doubleReverse[0][i][j] = false;
	//				_for(k, 0, data[0][i][j].size()) // Ͱ��ÿ������
	//					bits[0][i][0][data[0][i][j][k].subID] = 1;
	//			}
	//			if (j < highQuarterPoint) { // ��������bitset
	//				bitsID[1][i][j] = -1;
	//				endBucket[1][i][j] = 0; // ����������0
	//				doubleReverse[1][i][j] = false;
	//				_for(k, 0, data[1][i][j].size()) // Ͱ��ÿ������
	//					bits[1][i][0][data[1][i][j][k].subID] = 1;
	//			}
	//			else if (j < highHalfPoint) {
	//				bitsID[1][i][j] = 0;
	//				endBucket[1][i][j] = highHalfPoint; // �� j ���ط�����������ڵ���? highCriticalPoint
	//				doubleReverse[1][i][j] = true;
	//				_for(k, 0, data[1][i][j].size()) // Ͱ��ÿ������
	//					bits[1][i][0][data[1][i][j][k].subID] = 1;
	//			}
	//			else {
	//				bitsID[1][i][j] = 0;
	//				endBucket[1][i][j] = highHalfPoint; // �� j-1 ���������ڵ��� highHalfPoint, ����ǰ����һ��
	//				doubleReverse[1][i][j] = false;
	//			}
	//		}
	//	}
	//	//cout << "HEM5_256ORDD Stop.\n";
	//	return;
	//}

	// ��ǰӦ��ӳ�䵽��bitId, Ͱid, ��һ���ٽ縺�ص�
	int lowBid, highBid, lowBktId, highBktId, lowSubWorkLoad, highSubWorkLoad;
	int subWorkLoadStep; // ÿ��ά���ϵ�subWorkLoadStep����ͬ, ��ͬһ��ά���ϵ�low/high subWorkLoadStep��һ����
	_for(i, 0, numDimension)
	{

		// �����������?
		if (fix[0][i][0] == 0)
		{
			_for(j, 0, numBucket)
			{
				bitsID[0][i][j] = -1;
				endBucket[0][i][j] = j;
				doubleReverse[0][i][j] = false;
				bitsID[1][i][j] = -1;
				endBucket[1][i][j] = j; // ���������ڵ���endBucket[1][i][j]
				doubleReverse[1][i][j] = false;
			}
			continue;
		}

		subWorkLoadStep = (fix[0][i][0] + numBits - 1) / numBits; // fix[1][i][numBucket]

		// ������low/high���Ƕ�̬��, ���������ܹ���ͬһ��partition/cell,
		// ������low���Ǵ���߿�ʼ��һ��subWorkLoadStep����, ����һ��
		// ���ߴ��ұ��� ʣ�ฺ���� ��ʼ�ۼ�subWorkLoadStep, �������endBucket!
		// 0��lowͰһ�������õ��� (numBits - 2) Ϊ�±��bitset
		// ���һ��Ͱһ���ò���bitset
		// ����: numBits=15, fix[0][i][numBucket]=1000000, subWorkLoadStep=66667 (low�ϵĺ�14����1, high�ϵ�ǰ14����1)
		// fix[0][i][numBucket] / subWorkLoadStep=14, lowSubWorkLoad=66662
		lowBid = -1;
		lowBktId = numBucket;
		lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
		highBid = -1;
		highBktId = 0;
		highSubWorkLoad = subWorkLoadStep;

		// lowContain[i]=����(��һ������)lowSubWorkLoad+(i-1)*subWorkLoadStep������������Ͱ��(i>0ʱ)
		vector<int> lowContain(numBits + 1, numBucket);
		// highContain[i]=���� i*subWorkLoadStep ������������Ͱ��
		vector<int> highContain(numBits + 1, 0);
		int li = 1, hi = 1; // lowContain��highContain���±�
		_for(j, 0, numBucket)
		{
			if (fix[1][i][j] >= highSubWorkLoad)
			{
				highContain[hi++] = j;
				highSubWorkLoad += subWorkLoadStep;
			}
			// ����: fix[0][i][0]=1M, subWorkLoadStep=100000, numBits=10
			// li,lowSubWorkLoad = 1,100000; 2,200000; ... ; 9,900000; 10,1000000; 11,1100000
			if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad)
			{
				lowContain[li++] = numBucket - j - 1;
				lowSubWorkLoad += subWorkLoadStep;
			}
		}
		//lowContain[li] = 0; // Ϊɶ����Խ��??? li==numBits+1��
		if (hi == numBits) // Bug: ��󼸸�Ͱ�?��ʱhi����forѭ�������ӵ�numBits+1
			highContain[hi] = numBucket;

		li = hi = 1; // ˫�ط������ʱ����Ӧ����һ�˵�Ͱ����contain�����е��±�, ��ʵ li=lowBid+2, hi=highBid+2
		lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
		highSubWorkLoad = subWorkLoadStep;
		_for(j, 0, numBucket)
		{
			// ��ʱ���ڵ���highSubWorkLoad��, ������bits, ��Ϊbits���ǵ�j-1Ͱ
			if (fix[1][i][j] >= highSubWorkLoad)
			{ // ��һ�����ڵ����ٽ���Ͱ(j-1��, ǰ׺�Ͳ���������)��Ϊbitset���ǵ��յ�Ͱ
				highSubWorkLoad += subWorkLoadStep;
				hi++;
				highBid++;
				highBktId = j;
			}

			// Bug: ��ǰ����, ��󼸸�Ͱ�?��, ��ʱhighBid=numBits-1, Խ����, ֱ����fullBL
			if (fix[1][i][j] == fix[1][i][numBucket])
			{
				bitsID[1][i][j] = numBits - 1;
				endBucket[1][i][j] = j + 1; // ����ǵ�һ�ν���?, j��Ͱ�ǿ�, ��Ҫ���ط�����, �����ǿ�Ͱ, ���Լ����������?
				doubleReverse[1][i][j] = true;
			}
			else if (fix[1][i][j] - fix[1][i][highBktId] <
					 fix[1][i][highContain[hi]] - fix[1][i][j + 1])
			{ // Bug: û�м�highBktId
				bitsID[1][i][j] = highBid;
				endBucket[1][i][j] = highBktId; // ���������ڵ���endBucket[1][i][j]
				doubleReverse[1][i][j] = false;
			}
			else
			{
				bitsID[1][i][j] = hi - 1;              // highBid+1
				endBucket[1][i][j] = highContain[hi]; // ��j���ұ�����С��endBucket[1][i][j]
				doubleReverse[1][i][j] = true;
			}

			// ��׺�������ʱ��������?(�������������?, ��������j��lowBktId��lowContain[li]���ټ�һ����lowContain[li]�п���Ϊ0); -1+1ʡȥ��
			// fix[0][i][j][numBucket]��Ҫ��0, ʹfix[0][i][j][lowBktId]�տ�ʼΪ0
			// Bug: ��ǰ����, ���С�ļ���Ͱ�?��, ��������, ֱ���ö��ط���
			if (fix[0][i][numBucket - j - 1] == fix[0][i][0])
			{
				bitsID[0][i][numBucket - j - 1] = numBits - 1;
				endBucket[0][i][numBucket - j - 1] = numBucket - j - 1;
				doubleReverse[0][i][numBucket - j - 1] = true;
			}
			else if (fix[0][i][numBucket - j] - fix[0][i][lowBktId] <
					 fix[0][i][lowContain[li]] - fix[0][i][numBucket - j - 1])
			{
				bitsID[0][i][numBucket - j - 1] = lowBid;
				endBucket[0][i][numBucket - j - 1] = lowBktId;
				doubleReverse[0][i][numBucket - j - 1] = false;
			}
			else
			{
				bitsID[0][i][numBucket - j - 1] = li - 1; // lowBktId+1
				endBucket[0][i][numBucket - j - 1] = lowContain[li];
				doubleReverse[0][i][numBucket - j - 1] = true;
			}

			// ��ʱ��Ȼ���ڵ���lowSubWorkLoad��, ���Բ�������bits, ��ΪbitsҪ���ǵ�j��Ͱ
			if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad)
			{
				lowSubWorkLoad += subWorkLoadStep;
				li++;
				lowBid++;
				lowBktId = numBucket - j - 1;
			}
		}
	}

	int subID, b; // ��ʼ���������±�
	_for(i, 0, numDimension)
	{ // ÿ��ά��
		_for(j, 0, numBucket)
		{ // ÿ��Ͱ
			if (doubleReverse[0][i][j])
				b = bitsID[0][i][j]; // ��С����Ҫ�����bits�����ID
			else
				b = bitsID[0][i][j] + 1;
			_for(k, 0, data[0][i][j].size())
			{
				subID = data[0][i][j][k].subID;
				fullBits[i][subID] = 1; // 0��bitsÿ�α�����
				_for(q, b, numBits - 1) // Bug: bits�����ǴӸ�λ(���ǹ�)����λ������
					bits[0][i][q][subID] = 1;
			}

			if (doubleReverse[1][i][j])
				b = bitsID[1][i][j];
			else
				b = bitsID[1][i][j] + 1; // ��С����Ҫ�����bits�����ID
			_for(k, 0, data[1][i][j].size())
			{ // Ͱ��ÿ������
				subID = data[1][i][j][k].subID;
				_for(q, b, numBits - 1) bits[1][i][q][subID] = 1;
			}
		}
	}
	//cout << "HEM5_256ORDD Stop.\n";
}

// ������ʱ�����?
void HEM5_avxOR::match(const Pub& pub, int& matchSubs)
{
	bitset<subs> b; // register
	bitset<subs> bLocal;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;

	_for(i, 0, pub.size)
	{
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;

		if (doubleReverse[0][att][buck])
		{
			if (bitsID[0][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ����: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[0][att][bitsID[0][att][buck]];
			_for(j, endBucket[0][att][buck],
				buck) _for(k, 0, data[0][att][j].size()) bLocal[data[0][att][j][k].subID] = 0;
			_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val <= value)
					bLocal[data[0][att][buck][k].subID] = 0;
			Util::bitsetOr(b, bLocal); //b = b | bLocal;
		}
		else
		{
			_for(j,
				buck + 1, endBucket[0][att][buck]) _for(k, 0, data[0][att][j].size()) b[data[0][att][j][k].subID] = 1;
			_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val > value)
					b[data[0][att][buck][k].subID] = 1;
			if (bitsID[0][att][buck] != -1)
				Util::bitsetOr(b, bits[0][att][bitsID[0][att][buck]]);
		}

		if (doubleReverse[1][att][buck])
		{
			if (bitsID[1][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ����: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[1][att][bitsID[1][att][buck]];

			_for(j, buck
				+1, endBucket[1][att][buck]) _for(k, 0, data[1][att][j].size()) bLocal[data[1][att][j][k].subID] = 0;
			_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val >= value)
					bLocal[data[1][att][buck][k].subID] = 0;
			Util::bitsetOr(b, bLocal);
		}
		else
		{
			_for(j, endBucket[1][att][buck], buck) _for(k, 0, data[1][att][j].size()) b[data[1][att][j][k].subID] = 1;
			_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val < value)
					b[data[1][att][buck][k].subID] = 1;
			if (bitsID[1][att][buck] != -1)
				Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);
		}
	}

	/*if (numBits > 1)
	{*/
	_for(i, 0, numDimension) if (!attExist[i])
			Util::bitsetOr(b, fullBits[i]);
	//}
	//else // ֻ��һ������bitset����
	//{
	//	_for(i, 0, numDimension) if (!attExist[i])
	//		_for(j, 0, endBucket[0][i][0])
	//		for (auto&& kId : data[0][i][j])
	//			b[kId.subID] = 1;
	//	_for(i, 0, numDimension) if (!attExist[i])
	//		Util::bitsetOr(b, bits[0][i][0]);
	//}

	//_for(i, 0, subs) if (!b[i])
	//{
	//	++matchSubs;
	//	//cout << "HEM5_avxOR matches sub: " << i << endl;
	//}
	matchSubs = numSub - b.count();
}

// ����ʱ�����?
void HEM5_avxOR::match_debug(const Pub& pub, int& matchSubs)
{
	bitset<subs> b, bLocal;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;

	_for(i, 0, pub.size)
	{
		Timer compareStart;
		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;

		if (doubleReverse[0][att][buck])
		{
			Timer markStart;
			if (bitsID[0][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ����: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[0][att][bitsID[0][att][buck]];
			_for(j, endBucket[0][att][buck],
				buck) _for(k, 0, data[0][att][j].size()) bLocal[data[0][att][j][k].subID] = 0;
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
			_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val <= value)
					bLocal[data[0][att][buck][k].subID] = 0;
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
			Util::bitsetOr(b, bLocal);
			orTime += (double)orStart.elapsed_nano();
		}
		else
		{
			Timer markStart;
			_for(j, buck + 1, endBucket[0][att][buck]) _for(k, 0,
					data[0][att][j].size()) b[data[0][att][j][k].subID] = 1;
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
			_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val > value)
					b[data[0][att][buck][k].subID] = 1;
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
			if (bitsID[0][att][buck] != -1)
				Util::bitsetOr(b, bits[0][att][bitsID[0][att][buck]]);
			orTime += (double)orStart.elapsed_nano();
		}

		if (doubleReverse[1][att][buck])
		{
			Timer markStart;
			if (bitsID[1][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ����: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[1][att][bitsID[1][att][buck]];
			_for(j, buck + 1, endBucket[1][att][buck]) _for(k, 0,
					data[1][att][j].size()) bLocal[data[1][att][j][k].subID] = 0;
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
			_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val >= value)
					bLocal[data[1][att][buck][k].subID] = 0;
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
			Util::bitsetOr(b, bLocal);
			orTime += (double)orStart.elapsed_nano();
		}
		else
		{
			Timer markStart;
			_for(j, endBucket[1][att][buck], buck) _for(k, 0, data[1][att][j].size()) b[data[1][att][j][k].subID] = 1;
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
			_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val < value)
					b[data[1][att][buck][k].subID] = 1;
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
			if (bitsID[1][att][buck] != -1)
				Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);
			orTime += (double)orStart.elapsed_nano();
		}
	}

	//if (numBits > 1) {
	Timer orStart;
	_for(i, 0, numDimension) if (!attExist[i])
			Util::bitsetOr(b, fullBits[i]);
	orTime += (double)orStart.elapsed_nano();
	/*}
	else {
		Timer markStart;
		_for(i, 0, numDimension) if (!attExist[i])
			_for(j, 0, endBucket[0][i][0]) _for(k, 0, data[0][i][j].size()) b[data[0][i][j][k].subID] = 1;
		markTime += (double)markStart.elapsed_nano();

		Timer orStart;
		_for(i, 0, numDimension) if (!attExist[i])
			Util::bitsetOr(b, bits[0][i][0]);
		orTime += (double)orStart.elapsed_nano();
	}*/

	Timer bitStart;
	//	_for(i, 0, subs) if (!b[i]) {
	//			++matchSubs;
	//			//cout << "HEM5_avxOR matches sub: " << i << endl;
	//		}
	matchSubs = subs - b.count();
	bitTime += (double)bitStart.elapsed_nano();
}

//void HEM5_avxOR::calBucketSize() {
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

int HEM5_avxOR::calMemory()
{
	long long size = 0; // Byte
	_for(i, 0, numDimension)
	{
		// ��ÿ��ά����bits�������һ������? 2*sizeof(bitset<subs>)*numDimension*numBits
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		_for(j, 0, numBucket) size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	}

	// fullBits
	if (numBits > 1)
		size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()��numDimension

	// ����fix
	size += sizeof(int) * numDimension * (numBucket + 1);
	// ����endBucket������bitsID������doubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void HEM5_avxOR::printRelation(int dimension_i)
{
	cout << "\n\nHEM5_256ORDDMap\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension)
		{
			cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][i][0] << "   ----------------\n";
			_for(j, 0, numBucket)
			{
				cout << "lBkt" << j << ": bID=" << bitsID[0][i][j] << ", eBkt=" << endBucket[0][i][j] << ", dRvs="
					 << doubleReverse[0][i][j] << "; ";
				if (j % 5 == 0 && j > 0)
					cout << "\n";
			}
			cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][i][numBucket]
				 << "   ----------------\n";
			_for(j, 0, numBucket)
			{
				cout << "hBkt" << j << ": bID=" << bitsID[1][i][j] << ", eBkt=" << endBucket[1][i][j] << ", dRvs="
					 << doubleReverse[1][i][j] << "; ";
				if (j % 5 == 0 && j > 0)
					cout << "\n";
			}
		}
	else
	{
		cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][0]
			 << "   ----------------\n";
		_for(i, 0, numBucket)
		{
			cout << "lBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i]
				 << ", dRvs=" << doubleReverse[0][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)
				cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket]
			 << "   ----------------\n";
		_for(i, 0, numBucket)
		{
			cout << "hBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i]
				 << ", dRvs=" << doubleReverse[1][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)
				cout << "\n";
		}
	}
	cout << "\n\n";
}

vector<int> HEM5_avxOR::calMarkNumForBuckets()
{
    vector<int> numMarking(numBucket, 0);
    _for(i, 0, numDimension)
    {
        _for(j, 0, numBucket)
        {
            numMarking[j] += data[0][i][j].size() + data[1][i][j].size();

            if (doubleReverse[0][i][j])
            {
                _for(k, endBucket[0][i][j], j) numMarking[j] += data[0][i][k].size();
            }
            else
            {
                _for(k, j + 1, endBucket[0][i][j]) numMarking[j] += data[0][i][k].size();
            }

            if (doubleReverse[1][i][j])
            {
                _for(k, j + 1, endBucket[1][i][j]) numMarking[j] += data[1][i][k].size();
            }
            else
            {
                _for(k, endBucket[1][i][j], j) numMarking[j] += data[1][i][k].size();
            }
        }
    }
    return numMarking;
}
