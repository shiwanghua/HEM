#include "HEM5_AS.h"

HEM5_AS::HEM5_AS(int type) {
	numSub = 0;
	numDimension = atts;
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;

	//bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

	if (be == -1)
		numBits = be2;
	else
		numBits = pow(2, be); // ÿ��ά����lowValue��Ӧ��bits�������
	//if (numBits > 1)
	fullBits.resize(numDimension); // ά��������Զ���䣬����ֻ��Ҫresizeһ��
	numAttrGroup = attrGroup;
	attrGroupSize = (numDimension + numAttrGroup - 1) / numAttrGroup;
	attrGroupBits.resize(numAttrGroup);

	doubleReverse[0] = new bool *[numDimension];
	doubleReverse[1] = new bool *[numDimension];
	endBucket[0] = new int *[numDimension];
	endBucket[1] = new int *[numDimension];
	bitsID[0] = new int *[numDimension];
	bitsID[1] = new int *[numDimension];
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
	string TYPE;
	if (type == HEM5_DD_VAS) TYPE = "HEM5_DD_VAS";
	else if(type==HEM5_DD_RAS) TYPE = "HEM5_DD_RAS";
	else if(type==HEM5_DD_RAS_AVXOR_PARALLEL) {
		TYPE = "HEM5_DD_RAS_AVXOR"+to_string(BlockSize)+"_PARALLEL";
		threadPool.initThreadPool(parallelDegree);
	}
	cout << "ExpID = " << expID << ". " + TYPE + ": bitset number = " << numBits << ", bucketStep = " << buckStep
		 << ", numBucket = " << numBucket << ", attrGroupNum = " << numAttrGroup << ", attGroupSize = " << attrGroupSize
		 << endl;
}

HEM5_AS::~HEM5_AS() {
	_for(i, 0,
		 numDimension) delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
}

void HEM5_AS::insert_VAS(IntervalSub sub) {
	Combo c;
	c.subID = sub.id;
	for (auto &&iCnt: sub.constraints) {
		attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;
		c.val = iCnt.lowValue;
		data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
		c.val = iCnt.highValue;
		data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
	}
	numSub++;
}

void HEM5_AS::insert_RAS(IntervalSub sub) {
	for (int attGroupNo = sub.constraints[0].att / attrGroupSize, i = 0; i < numAttrGroup; i++) {
		if (i != attGroupNo)
			attrGroupBits[i][sub.id] = 1;
	}
	Combo c;
	c.subID = sub.id;
	for (auto &&iCnt: sub.constraints) {
		c.val = iCnt.lowValue;
		data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
		c.val = iCnt.highValue;
		data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
	}
	numSub++;
}

void HEM5_AS::insert_online_VAS(IntervalSub sub) {
	int b, bucketID;
	Combo c;
	c.subID = sub.id;

	for (auto &&iCnt: sub.constraints) {
		fullBits[iCnt.att][sub.id] = 1;
		attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;

		bucketID = iCnt.lowValue / buckStep;
		c.val = iCnt.lowValue;
		data[0][iCnt.att][bucketID].emplace_back(c);
		if (doubleReverse[0][iCnt.att][bucketID])
			b = bitsID[0][iCnt.att][bucketID];
		else
			b = bitsID[0][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][iCnt.att][q][sub.id] = 1;

		bucketID = iCnt.highValue / buckStep;
		c.val = iCnt.highValue;
		data[1][iCnt.att][bucketID].emplace_back(c);
		if (doubleReverse[1][iCnt.att][bucketID])
			b = bitsID[1][iCnt.att][bucketID];
		else
			b = bitsID[1][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][iCnt.att][q][sub.id] = 1;
	}
	numSub++;
}

void HEM5_AS::insert_online_RAS(IntervalSub sub) {
	int b, bucketID;
	Combo c;
	c.subID = sub.id;

	for (int attGroupNo = sub.constraints[0].att / attrGroupSize, i = 0; i < numAttrGroup; i++) {
		if (i != attGroupNo)
			attrGroupBits[i][sub.id] = 1;
	}

	for (auto &&iCnt: sub.constraints) {
		fullBits[iCnt.att][sub.id] = 1;

		bucketID = iCnt.lowValue / buckStep;
		c.val = iCnt.lowValue;
		data[0][iCnt.att][bucketID].emplace_back(c);
		if (doubleReverse[0][iCnt.att][bucketID])
			b = bitsID[0][iCnt.att][bucketID];
		else
			b = bitsID[0][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][iCnt.att][q][sub.id] = 1;

		bucketID = iCnt.highValue / buckStep;
		c.val = iCnt.highValue;
		data[1][iCnt.att][bucketID].emplace_back(c);
		if (doubleReverse[1][iCnt.att][bucketID])
			b = bitsID[1][iCnt.att][bucketID];
		else
			b = bitsID[1][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][iCnt.att][q][sub.id] = 1;
	}
	numSub++;
}

bool HEM5_AS::deleteSubscription_VAS(IntervalSub sub) {
	int find = 0, b, bucketID, id = sub.id;

	for (auto &&iCnt: sub.constraints) {
		fullBits[iCnt.att][id] = 0;
		attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 0;

		bucketID = iCnt.lowValue / buckStep;
		vector<Combo>::const_iterator it;
		for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
			if (it->subID == id) {
				data[0][iCnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[0][iCnt.att][bucketID])
			b = bitsID[0][iCnt.att][bucketID];
		else
			b = bitsID[0][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][iCnt.att][q][sub.id] = 0;

		bucketID = iCnt.highValue / buckStep;
		for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
			if (it->subID == id) {
				data[1][iCnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[1][iCnt.att][bucketID])
			b = bitsID[1][iCnt.att][bucketID];
		else
			b = bitsID[1][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][iCnt.att][q][sub.id] = 0;
	}
	if (find == sub.size << 1)
		numSub--;
	return find == sub.size << 1;
}

bool HEM5_AS::deleteSubscription_RAS(IntervalSub sub) {
	int find = 0, b, bucketID, id = sub.id;

	// int attGroupNo = sub.constraints[0].att / attrGroupSize;
	_for(i, 0, numAttrGroup) {
		//if (i != attGroupNo)
		attrGroupBits[i][sub.id] = 0;
	}

	for (auto &&iCnt: sub.constraints) {
		fullBits[iCnt.att][id] = 0;

		bucketID = iCnt.lowValue / buckStep;
		vector<Combo>::const_iterator it;
		for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
			if (it->subID == id) {
				data[0][iCnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[0][iCnt.att][bucketID])
			b = bitsID[0][iCnt.att][bucketID];
		else
			b = bitsID[0][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[0][iCnt.att][q][sub.id] = 0;

		bucketID = iCnt.highValue / buckStep;
		for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
			if (it->subID == id) {
				data[1][iCnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		if (doubleReverse[1][iCnt.att][bucketID])
			b = bitsID[1][iCnt.att][bucketID];
		else
			b = bitsID[1][iCnt.att][bucketID] + 1;
		_for(q, b, numBits - 1) bits[1][iCnt.att][q][sub.id] = 0;
	}
	if (find == sub.size << 1)
		numSub--;
	return find == sub.size << 1;
}

void HEM5_AS::initBits() {
	// ����ж�γ�ʼ��
	_for(i, 0,
		 numDimension) delete[] doubleReverse[0][i], doubleReverse[1][i], endBucket[0][i], endBucket[1][i], bitsID[0][i], bitsID[1][i];
	delete[] endBucket[0], endBucket[1], bitsID[0], bitsID[1], doubleReverse[0], doubleReverse[1];
	doubleReverse[0] = new bool *[numDimension];
	doubleReverse[1] = new bool *[numDimension];
	endBucket[0] = new int *[numDimension];
	endBucket[1] = new int *[numDimension];
	bitsID[0] = new int *[numDimension];
	bitsID[1] = new int *[numDimension];
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
	_for(i, 0, numDimension) {
		fix[0][i][numBucket]=0;
		fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
		fix[1][i][0]=0;
		_for(j, 1, numBucket) {
			fix[0][i][numBucket - j - 1] = fix[0][i][numBucket - j] + data[0][i][numBucket - j - 1].size();
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
	//				endBucket[0][i][j] = lowHalfPoint; // �� j ���ط������������ lowCriticalPoint(������)
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
	//				endBucket[1][i][j] = highHalfPoint; // �� j ���ط�����������ڵ��� highCriticalPoint
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
	//	//cout << "HEM5_AGDD Stop.\n";
	//	return;
	//}

	// ��ǰӦ��ӳ�䵽��bitId, Ͱid, ��һ���ٽ縺�ص�
	int lowBid, highBid, lowBktId, highBktId, lowSubWorkLoad, highSubWorkLoad;
	int subWorkLoadStep; // ÿ��ά���ϵ�subWorkLoadStep����ͬ, ��ͬһ��ά���ϵ�low/high subWorkLoadStep��һ����
	_for(i, 0, numDimension) {

		// �����������
		if (fix[0][i][0] == 0) {
			_for(j, 0, numBucket) {
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
		// ����(����Ҫ�ĳ�)���ұ��� ʣ�ฺ���� ��ʼ�ۼ�subWorkLoadStep, �������endBucket!
		// 0��lowͰһ�������õ��� (numBits - 2) Ϊ�±��bitset
		// ���һ��Ͱһ���ò���bitset
		// ����: numBits=15(����16), fix[0][i][0]=1000000, subWorkLoadStep=66667 (low�ϵĺ�14����1, high�ϵ�ǰ14����1)
		// fix[0][i][numBucket] / subWorkLoadStep=14, lowSubWorkLoad=66662
		lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
		highSubWorkLoad = subWorkLoadStep;

		// lowContain[i]=������(��һ������)lowSubWorkLoad+(i-1)*subWorkLoadStep������������Ͱ��(i>0ʱ)
		vector<int> lowContain(numBits + 1, numBucket);
		// highContain[i]=���� i*subWorkLoadStep ������������Ͱ��
		vector<int> highContain(numBits + 1, 0);
		int li = 1, hi = 1; // lowContain��highContain���±�
		_for(j, 0, numBucket) {
			if (fix[1][i][j] >= highSubWorkLoad) { // fix[1][i][numBucket]�Ű���ȫ��, ���һ���ܽ���if
				highContain[hi++] = j; // numBits=1ʱhighContain[1]<=numBucket-1(������һ���ǿ�Ͱλ��+1)
				highSubWorkLoad += subWorkLoadStep;
			}
			// ����: fix[0][i][0]=1M, subWorkLoadStep=100000, numBits=10
			// li,lowSubWorkLoad = 1,100000; 2,200000; ... ; 9,900000; 10,1000000; 11,1100000
			if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad) { // fix[0][i][0]�Ͱ���ȫ��, ����һ������if
				lowContain[li++] = numBucket - j - 1; // numBits=1ʱlowContain[1]>=0 (������һ���ǿ�Ͱλ��)
				lowSubWorkLoad += subWorkLoadStep;
			}
		}
		//lowContain[li] = 0; // Ϊɶ����Խ��??? li==numBits+1��
		if (hi == numBits) // Bug: ��󼸸�ͰΪ��ʱhi����forѭ�������ӵ�numBits+1; ���һ��Ͱ�ǿ�ʱhighContain[numBits]��û��ֵ
			highContain[hi] = numBucket; // numBits=1ʱhighContain[1]=numBucket

		li = hi = 1; // ˫�ط������ʱ����Ӧ����һ�˵�Ͱ����contain�����е��±�, ��ʵ li=lowBid+2, hi=highBid+2
		lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
		highSubWorkLoad = subWorkLoadStep;
		lowBid = -1;
		highBid = -1;
		lowBktId = numBucket;
		highBktId = 0;
		for (int lj = 0, hj = numBucket - 1; lj < numBucket; lj++, hj--) {
			// ��ʱ���ڵ���highSubWorkLoad��, ������bits, ��Ϊbits���ǵ�j-1Ͱ
			if (fix[1][i][lj] >= highSubWorkLoad) { // ��һ�����ڵ����ٽ���Ͱ(j-1��, ǰ׺�Ͳ���������)��Ϊbitset���ǵ��յ�Ͱ
				highSubWorkLoad += subWorkLoadStep;
				hi++;
				highBid++;
				highBktId = lj;
			}

			// Bug: ��ǰ����, ��󼸸�ͰΪ��, ��ʱhighBid=numBits-1, hi=numBits+1, Խ����, ֱ����fullBL
			if (fix[1][i][lj] == fix[1][i][numBucket]) {
				bitsID[1][i][lj] = numBits - 1;
				endBucket[1][i][lj] = lj; // bkt lj does not need to be marked
				doubleReverse[1][i][lj] = true;
			} else if (fix[1][i][lj] - fix[1][i][highBktId] <
					   fix[1][i][highContain[hi]] - fix[1][i][lj+1]) { // Bug: û�м�highBktId
				bitsID[1][i][lj] = highBid; // hi - 2
				endBucket[1][i][lj] = highBktId; // ���������ڵ���endBucket[1][i][j]
				doubleReverse[1][i][lj] = false;
			} else {
				bitsID[1][i][lj] = hi - 1;             // highBid+1
				endBucket[1][i][lj] = highContain[hi]; // ��j���ұ�����С��endBucket[1][i][j]
				doubleReverse[1][i][lj] = true;
			}

			// ��׺�������ʱ��������(�������������, ��������lj��lowBktId��lowContain[li]���ټ�һ����lowContain[li]�п���Ϊ0); -1+1ʡȥ��
			// fix[0][i][j][numBucket]��Ҫ��0, ʹfix[0][i][j][lowBktId]�տ�ʼΪ0
			// Bug: ��ǰ����, ���С�ļ���ͰΪ��, ��������, ֱ���ö��ط���
			if (fix[0][i][hj] == fix[0][i][0]) {
				bitsID[0][i][hj] = numBits - 1;
				endBucket[0][i][hj] = hj+1;
				doubleReverse[0][i][hj] = true;
			} else if (fix[0][i][hj + 1] - fix[0][i][lowBktId] <
					   fix[0][i][lowContain[li]] - fix[0][i][hj]) {
				bitsID[0][i][hj] = lowBid;
				endBucket[0][i][hj] = lowBktId;
				doubleReverse[0][i][hj] = false;
			} else {
				bitsID[0][i][hj] = li - 1; // lowBid+1
				endBucket[0][i][hj] = lowContain[li];
				doubleReverse[0][i][hj] = true;
			}

			// ��ʱ��Ȼ���ڵ���lowSubWorkLoad��, ���Բ�������bits, ��ΪbitsҪ���ǵ�hj��Ͱ
			if (fix[0][i][hj] >= lowSubWorkLoad) {
				lowSubWorkLoad += subWorkLoadStep;
				li++;
				lowBid++;
				lowBktId = hj;
			}
		}
	}

	int subID, b; // ��ʼ���������±�
	_for(i, 0, numDimension) { // ÿ��ά��
		_for(j, 0, numBucket) { // ÿ��Ͱ
			if (doubleReverse[0][i][j])
				b = bitsID[0][i][j]; // ��С����Ҫ�����bits�����ID
			else
				b = bitsID[0][i][j] + 1;
			_for(k, 0, data[0][i][j].size()) {
				subID = data[0][i][j][k].subID;
				fullBits[i][subID] = 1; // 0��bitsÿ�α�����
				_for(q, b, numBits - 1) // Bug: bits�����ǴӸ�λ(���ǹ�)����λ������
					bits[0][i][q][subID] = 1;
			}

			if (doubleReverse[1][i][j])
				b = bitsID[1][i][j];
			else
				b = bitsID[1][i][j] + 1; // ��С����Ҫ�����bits�����ID
			_for(k, 0, data[1][i][j].size()) { // Ͱ��ÿ������
				subID = data[1][i][j][k].subID;
				_for(q, b, numBits - 1) bits[1][i][q][subID] = 1;
			}
		}
	}
	//cout << "HEM5_AGDD Stop.\n";
}

void HEM5_AS::match_VAS(const Pub &pub, int &matchSubs) {
	bitset<subs> b; // register
	bitset<subs> bLocal;
	vector<bool> attExist(numDimension, false);
	vector<bool> attGroupExist(numAttrGroup, false);
	int value, att, buck;

	_for(i, 0, pub.size) {

		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;
		attGroupExist[att / attrGroupSize] = true;

		if (doubleReverse[0][att][buck]) {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			if (bitsID[0][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[0][att][bitsID[0][att][buck]];
			_for(j, endBucket[0][att][buck], buck) for (auto &&iCob: data[0][att][j])
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[0][att][buck])
				if (iCob.val <= value)
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
#if BlockSize == 64
            b |= bLocal;
#else
            Util::bitsetOr(b, bLocal);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		} else {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			_for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob: data[0][att][j])
					b[iCob.subID] = 1;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[0][att][buck])
				if (iCob.val > value)
					b[iCob.subID] = 1;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
			if (bitsID[0][att][buck] != -1)
#if BlockSize == 64
                b |= bits[0][att][bitsID[0][att][buck]];
#else
                Util::bitsetOr(b, bits[0][att][bitsID[0][att][buck]]);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		}

		if (doubleReverse[1][att][buck]) {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			if (bitsID[1][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[1][att][bitsID[1][att][buck]];
			_for(j, buck+1, endBucket[1][att][buck]) for (auto &&iCob: data[1][att][j])
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[1][att][buck])
				if (iCob.val >= value)
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
#if BlockSize == 64
            b |= bLocal;
#else
            Util::bitsetOr(b, bLocal);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		} else {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			_for(j, endBucket[1][att][buck], buck) for (auto &&iCob: data[1][att][j])
					b[iCob.subID] = 1;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[1][att][buck])
				if (iCob.val < value)
					b[iCob.subID] = 1;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
			if (bitsID[1][att][buck] != -1)
#if BlockSize == 64
            	b |= bits[1][att][bitsID[1][att][buck]];
#else
            	Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		}
	}

	// ������ά�����
	/*if (numBits > 1) // ���ֻ��һ��bitsetʱbitset������Ϊ����һ���Ͱ������ȫ��Ͱ��Ҫ��if����
	{*/
	/*_for(i, 0, numDimension) if (!attExist[i])
		b = b | fullBits[i];*/
	/*}
	else
	{
		_for(i, 0, numDimension) if (!attExist[i])
			_for(j, 0, endBucket[0][i][0])
			_for(k, 0, data[0][i][j].size())
			b[data[0][i][j][k].subID] = 1;

		_for(i, 0, numDimension) if (!attExist[i])
			b = b | bits[0][i][0];
	}*/
#ifdef DEBUG
	Timer orStart;
#endif // DEBUG
	for (int AGi = 0, base; AGi < numAttrGroup; AGi++) {
		if (attGroupExist[AGi]) {
			base = AGi * attrGroupSize;
			_for(aj, base, base + attrGroupSize) {
				if (!attExist[aj])
#if BlockSize == 64
                b |= fullBits[aj];
#else
                Util::bitsetOr(b, fullBits[aj]);
#endif
			}
		} else {
#if BlockSize == 64
            b |= attrGroupBits[AGi];
#else
            Util::bitsetOr(b, attrGroupBits[AGi]);
#endif
		}
	}
#ifdef DEBUG
	orTime += (double)orStart.elapsed_nano();
	Timer bitStart;
#endif // DEBUG
	//	_for(i, 0, subs) if (!b[i]) {
	//			++matchSubs;
	//			//cout << "HEM5_VAG matches sub: " << i << endl;
	//		}
	matchSubs = subs - b.count();
#ifdef DEBUG
	bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

void HEM5_AS::match_RAS(const Pub &pub, int &matchSubs) {
	bitset<subs> b; // register
	bitset<subs> bLocal;
	vector<bool> attExist(numDimension, false);
	int value, att, buck;

	_for(i, 0, pub.size) {

		value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;

		if (doubleReverse[0][att][buck]) {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			if (bitsID[0][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[0][att][bitsID[0][att][buck]];
			_for(j, endBucket[0][att][buck], buck) for (auto &&iCob: data[0][att][j])
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[0][att][buck])
				if (iCob.val <= value)
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
#if BlockSize == 64
            b |= bLocal;
#else
            Util::bitsetOr(b, bLocal);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		} else {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			_for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob: data[0][att][j])
					b[iCob.subID] = 1;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[0][att][buck])
				if (iCob.val > value)
					b[iCob.subID] = 1;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
			if (bitsID[0][att][buck] != -1)
#if BlockSize == 64
            	b |= bits[0][att][bitsID[0][att][buck]];
#else
            	Util::bitsetOr(b, bits[0][att][bitsID[0][att][buck]]);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		}

		if (doubleReverse[1][att][buck]) {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			if (bitsID[1][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
				bLocal = fullBits[att];
			else
				bLocal = bits[1][att][bitsID[1][att][buck]];
			_for(j, buck+1, endBucket[1][att][buck]) for (auto &&iCob: data[1][att][j])
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[1][att][buck])
				if (iCob.val >= value)
					bLocal[iCob.subID] = 0;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
#if BlockSize == 64
            b |= bLocal;
#else
            Util::bitsetOr(b, bLocal);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		} else {
#ifdef DEBUG
			Timer markStart;
#endif // DEBUG
			_for(j, endBucket[1][att][buck], buck) for (auto &&iCob: data[1][att][j])
					b[iCob.subID] = 1;
#ifdef DEBUG
			markTime += (double)markStart.elapsed_nano();
			Timer compareStart;
#endif // DEBUG
			for (auto &&iCob: data[1][att][buck])
				if (iCob.val < value)
					b[iCob.subID] = 1;
#ifdef DEBUG
			compareTime += (double)compareStart.elapsed_nano();
			Timer orStart;
#endif // DEBUG
			if (bitsID[1][att][buck] != -1)
#if BlockSize == 64
            	b |= bits[1][att][bitsID[1][att][buck]];
#else
            	Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);
#endif
#ifdef DEBUG
			orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
		}
	}

	// ������ά�����
	/*if (numBits > 1) // ���ֻ��һ��bitsetʱbitset������Ϊ����һ���Ͱ������ȫ��Ͱ��Ҫ��if����
	{*/
	/*_for(i, 0, numDimension) if (!attExist[i])
		b = b | fullBits[i];*/
	/*}
	else
	{
		_for(i, 0, numDimension) if (!attExist[i])
			_for(j, 0, endBucket[0][i][0])
			_for(k, 0, data[0][i][j].size())
			b[data[0][i][j][k].subID] = 1;

		_for(i, 0, numDimension) if (!attExist[i])
			b = b | bits[0][i][0];
	}*/
#ifdef DEBUG
	Timer orStart;
#endif // DEBUG
	int attGroupNo = att / attrGroupSize;
#if BlockSize == 64
    b |= attrGroupBits[attGroupNo];
#else
    Util::bitsetOr(b, attrGroupBits[attGroupNo]);
#endif
	int base = attGroupNo * attrGroupSize;
	_for(ai, base, base + attrGroupSize) {
		if (!attExist[ai])
#if BlockSize == 64
            b |= fullBits[ai];
#else
            Util::bitsetOr(b, fullBits[ai]);
#endif
	}
#ifdef DEBUG
	orTime += (double)orStart.elapsed_nano();
	Timer bitStart;
#endif // DEBUG
	//	_for(i, 0, subs) if (!b[i]) {
	//			++matchSubs;
	//			//cout << "HEM5_VAG matches sub: " << i << endl;
	//		}
	matchSubs = subs - b.count();
#ifdef DEBUG
	bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

void HEM5_AS::match_RAS_avxOR_parallel(const Pub &pub, int &matchSubs) {
	vector<future<bitset<subs>>> threadResult;
	int seg = pub.size / parallelDegree;
	int remainder=pub.size%parallelDegree;
	int tId=0,end;
	for (int begin = 0; begin < pub.size; begin = end,tId++) {
		if(tId<remainder)
			end=begin+seg+1;
		else end=begin+seg;
		threadResult.emplace_back(threadPool.enqueue([this, &pub, begin, end] {

			bitset<subs> b; // register
			bitset<subs> bLocal;
			
			int value, att, buck;
			for (int i = begin; i < end; i++) {
				value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;

				if (doubleReverse[0][att][buck]) {
					if (bitsID[0][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
						bLocal = fullBits[att];
					else
						bLocal = bits[0][att][bitsID[0][att][buck]];
					_for(j, endBucket[0][att][buck], buck)
					for (auto &&iCob: data[0][att][j])
							bLocal[iCob.subID] = 0;
					_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val <= value)
							bLocal[data[0][att][buck][k].subID] = 0;
					
#if BlockSize == 64
                    b |= bLocal;
#else
                    Util::bitsetOr(b, bLocal);
#endif
                } else {
					_for(j, buck + 1, endBucket[0][att][buck]) 
					for (auto &&iCob: data[0][att][j])
							b[iCob.subID] = 1;
					_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val > value)
							b[data[0][att][buck][k].subID] = 1;
					if (bitsID[0][att][buck] != -1)
#if BlockSize == 64
                    b |= bits[0][att][bitsID[0][att][buck]];
#else
                    Util::bitsetOr(b, bits[0][att][bitsID[0][att][buck]]);
#endif
                }

				if (doubleReverse[1][att][buck]) {
					if (bitsID[1][att][buck] == numBits - 1) // ֻ��1��bitsetʱ����fullBits�ϣ�ȥ��: && numBits > 1
						bLocal = fullBits[att];
					else
						bLocal = bits[1][att][bitsID[1][att][buck]];
					_for(j, buck+1, endBucket[1][att][buck])
					for (auto &&iCob: data[1][att][j])
							bLocal[iCob.subID] = 0;
					_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val >= value)
							bLocal[data[1][att][buck][k].subID] = 0;
#if BlockSize == 64
                    b |= bLocal;
#else
                    Util::bitsetOr(b, bLocal);
#endif
                } else {
					_for(j, endBucket[1][att][buck], buck) 
					for (auto &&iCob: data[1][att][j])
							b[iCob.subID] = 1;
					_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val < value)
							b[data[1][att][buck][k].subID] = 1;
					if (bitsID[1][att][buck] != -1)
#if BlockSize == 64
                    b |= bits[1][att][bitsID[1][att][buck]];
#else
                    Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);
#endif
                }
			}
			return b;
		}));
    }

	int attGroupNo = pub.pairs[0].att / attrGroupSize;
	bitset<subs> gb=attrGroupBits[attGroupNo];
	if(pub.size<attrGroupSize){
		vector<bool> attExist(numDimension, false);
		for (auto& item: pub.pairs)
			attExist[item.att] = true;
		int base = attGroupNo * attrGroupSize;
		_for(ai, base, base + attrGroupSize) {
			if (!attExist[ai])
#if BlockSize == 64
                    gb |= fullBits[ai];
#else
                    Util::bitsetOr(gb, fullBits[ai]);
#endif
		}
	}
#ifdef DEBUG
	Timer mergeStart;
#endif // DEBUG
	for (int i = 0; i < threadResult.size(); i++)
#if BlockSize == 64
    gb |= threadResult[i].get();
#else
    Util::bitsetOr(gb, threadResult[i].get());
#endif
#ifdef DEBUG
	mergeTime += (double)mergeStart.elapsed_nano();
	Timer bitStart;
#endif // DEBUG
	//	_for(i, 0, subs) if (!b[i]) {
	//			++matchSubs;
	//			//cout << "HEM5_VAG matches sub: " << i << endl;
	//		}
	matchSubs = subs - gb.count();
#ifdef DEBUG
	bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

//void HEM5_AS::calBucketSize() {
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

int HEM5_AS::calMemory() {
	long long size = 0; // Byte
	size += sizeof(bits) + sizeof(bits[0]) * 2 + sizeof(data) + sizeof(data[0]) + sizeof(data[1]);
	//cout << sizeof(bits[0]) << " " << sizeof(bits[1]) <<" " << sizeof(data) << " " << sizeof(data[0]) << " " << sizeof(data[1]) << "\n";
	_for(i, 0, numDimension) {
		// ��ÿ��ά����bits�������һ������ 2*sizeof(bitset<subs>)*numDimension*numBits
		size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
		size += (sizeof(bits[0][i]) + sizeof(data[0][i])) * 2;
		//cout << i << ": " << sizeof(bits[0][i]) << " " << sizeof(data[0][i]) << " ";
		_for(j, 0, numBucket) {
			//cout << sizeof(data[0][i][j]) << " " << sizeof(data[1][i][j]) << " ";
			size += sizeof(data[0][i][j]) + sizeof(data[1][i][j]);
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
		}
		//cout << "\n";
	}

	// fullBits
	size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()��numDimension
	size += sizeof(fullBits); // 24
	//cout << "fullBits: " << sizeof(fullBits) << " " << sizeof(fullBits[0]) << "\n";

	// attrGroupBits
	size += sizeof(bitset<subs>) * attrGroupBits.size();

	// ����fix
	//cout << "fix: " << sizeof(fix) << " " << sizeof(fix[0]) << " " << sizeof(fix[0][10]) << sizeof(fix[1][7][20]) << "\n";
	size += sizeof(fix) + sizeof(fix[0]) * 2 + sizeof(fix[0][0]) * numDimension +
			sizeof(fix[0][0][0]) * numDimension * (numBucket + 1) * 2;

	// ����endBucket������bitsID������doubleReverse
	size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
	size += sizeof(endBucket[0]) * 4 + sizeof(endBucket[0][0]) * numDimension * 4;
	//cout << sizeof(endBucket) << " " << sizeof(endBucket[0]) << " " << sizeof(endBucket[0][0]) << " " << sizeof(endBucket[0][0][0]) << "\n";
	size = size / 1024 / 1024; // MB
	return (int) size;
}

void HEM5_AS::printRelation(int dimension_i) {
	cout << "\n\nHEM5_AGDDMap\n";
	if (dimension_i == -1)
		_for(i, 0, numDimension) {
			cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][i][0] << "   ----------------\n";
			_for(j, 0, numBucket) {
				cout << "lBkt" << j << ": bID=" << bitsID[0][i][j] << ", eBkt=" << endBucket[0][i][j] << ", dRvs="
					 << doubleReverse[0][i][j] << "; ";
				if (j % 5 == 0 && j > 0)
					cout << "\n";
			}
			cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][i][numBucket]
				 << "   ----------------\n";
			_for(j, 0, numBucket) {
				cout << "hBkt" << j << ": bID=" << bitsID[1][i][j] << ", eBkt=" << endBucket[1][i][j] << ", dRvs="
					 << doubleReverse[1][i][j] << "; ";
				if (j % 5 == 0 && j > 0)
					cout << "\n";
			}
		}
	else {
		cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][0]
			 << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "lBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i]
				 << ", dRvs=" << doubleReverse[0][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)
				cout << "\n";
		}
		cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket]
			 << "   ----------------\n";
		_for(i, 0, numBucket) {
			cout << "hBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i]
				 << ", dRvs=" << doubleReverse[1][dimension_i][i] << "; ";
			if (i % 5 == 0 && i > 0)
				cout << "\n";
		}
	}
	cout << "\n\n";
}

vector<int> HEM5_AS::calMarkNumForBuckets() {
	vector<int> numMarking(numBucket, 0);
	_for(i, 0, numBucket) {
		_for(j, 0, numDimension) {
			numMarking[i] += data[0][j][i].size() + data[1][j][i].size(); // �Ƚ�

			if (doubleReverse[0][j][i]) {
				_for(k, endBucket[0][j][i], i) numMarking[i] += data[0][j][k].size();
			} else {
				_for(k, i + 1, endBucket[0][j][i]) numMarking[i] += data[0][j][k].size();
			}

			if (doubleReverse[1][j][i]) {
				_for(k, i+1, endBucket[1][j][i]) numMarking[i] += data[1][j][k].size();
			} else {
				_for(k, endBucket[1][j][i], i) numMarking[i] += data[1][j][k].size();
			}
		}
	}
	return numMarking;
}