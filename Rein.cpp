#include "Rein.h"

Rein::Rein(int type) :numSub(0), numDimension(atts) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	bucketSub.resize(numBucket);
	if (type == 1) { // Original Rein
		cout << "ExpID = " << expID << ". Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
		data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
		data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
	}
	else if (type == 2) { // forward or forward with CBOMP
		cout << "ExpID = " << expID << ". Forward Rein (fRein): bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
		fData.resize(numDimension, vector<vector<IntervalCombo>>(numBucket));
		nB.resize(atts);
		//nnB.resize(atts);
	}
}

//void Rein::insert(Sub sub)
//{
//    for (int i = 0; i < sub.size; i++)
//    {
//        Cnt cnt = sub.constraints[i];
//        Combo c;
//        c.val = cnt.value;
//        c.subID = sub.id;
//        if (cnt.op == 0)        // ==
//        {
//            data[0][cnt.att][c.val / buckStep].push_back(c);
//            data[1][cnt.att][c.val / buckStep].push_back(c);
//        }
//        else if (cnt.op == 1)   // >=
//            data[0][cnt.att][c.val / buckStep].push_back(c);
//        else                    // >=
//            data[1][cnt.att][c.val / buckStep].push_back(c);
//    }
//}

void Rein::insert(IntervalSub sub)
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

bool Rein::deleteSubscription(IntervalSub sub)
{
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size)
	{
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

// 01在第二维，比较-标记-比较-标记
//void Rein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//
//	for (int i = 0; i < pub.size; i++)
//	{
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > value)
//				bits[data[0][att][buck][k].subID] = true;
//
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//
//
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < value)
//				bits[data[1][att][buck][k].subID] = true;
//
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
//	}
//
//	for (int i = 0; i < numSub; i++)
//		if (!bits[i])
//			++matchSubs;
//}

// 01在第一维 计算时间组成
//void Rein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//	vector<bool> attExist(numDimension, false);
//	for (int i = 0; i < pub.size; i++)
//	{
//		Timer compareStart;
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		attExist[att] = true;
//		// 把下面两个for循环注释了就是模糊匹配, 类似Tama
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > value)
//				bits[data[0][att][buck][k].subID] = true;
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < value)
//				bits[data[1][att][buck][k].subID] = true;
//		compareTime += (double)compareStart.elapsed_nano();
//
//		Timer markStart;
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
//		markTime += (double)markStart.elapsed_nano();
//	}
//
//	Timer markStart;
//	for (int i = 0; i < numDimension; i++)
//		if (!attExist[i])
//			for (int j = 0; j < numBucket; j++)
//				for (int k = 0; k < data[0][i][j].size(); k++)
//					bits[data[0][i][j][k].subID] = true;
//	markTime += (double)markStart.elapsed_nano();
//
//	Timer bitStart;
//	for (int i = 0; i < subs; i++)
//		if (!bits[i])
//		{
//			++matchSubs;
//			//cout << "rein matches sub: " << i << endl;
//		}
//	bitTime += (double)bitStart.elapsed_nano();
//}

// 01在第一维 不计算时间组成
void Rein::match(const Pub& pub, int& matchSubs)
{
	vector<bool> bits(numSub, false);
	vector<bool> attExist(numDimension, false);
	for (int i = 0; i < pub.size; i++)
	{
		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		attExist[att] = true;
		// 把下面两个for循环注释了就是模糊匹配, 类似Tama
		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;

		for (int j = buck + 1; j < numBucket; j++)
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	for (int i = 0; i < numDimension; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subs; i++)
		if (!bits[i])
		{
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}

void Rein::insert_forward_native(IntervalSub sub) {
	vector<bool> attrExist(atts, false);
	int bucketID;
	IntervalCombo c;
	c.subID = sub.id;
	subPredicate[sub.id] = sub.size;
	for (auto&& cnt : sub.constraints)
	{
		attrExist[cnt.att] = true;
		//nnB[cnt.att][sub.id] = 1;
		bucketID = cnt.lowValue / buckStep;
		c.lowValue = cnt.lowValue;
		c.highValue = cnt.highValue;
		fData[cnt.att][bucketID].insert(lower_bound(fData[cnt.att][bucketID].begin(), fData[cnt.att][bucketID].end(), c, [](const IntervalCombo& c1, const IntervalCombo& c2) {return c1.highValue == c2.highValue ? c1.lowValue < c2.lowValue : c1.highValue < c2.highValue; }), c); // insert 到迭代器前面!
	}
	_for(i, 0, atts)
		if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}

void Rein::match_forward_native(const Pub& pub, int& matchSubs) {
	memcpy(counter, subPredicate, sizeof(subPredicate));
	int att, buck;
	IntervalCombo pubPredicateTmp;
	pubPredicateTmp.lowValue = 0; // No sense.
	for (auto&& pair : pub.pairs)
	{
		pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;

		const auto&& lowerBoundIterator = lower_bound(fData[att][buck].begin(), fData[att][buck].end(), pubPredicateTmp, [&](const IntervalCombo& c1, const IntervalCombo& c2) {return c1.highValue < c2.highValue; });
		for_each(lowerBoundIterator, fData[att][buck].end(), [&](const IntervalCombo& c) {
			if (c.lowValue <= pair.value) counter[c.subID]--; });

		for_each(fData[att].begin(), fData[att].begin() + buck, [&](const vector<IntervalCombo>& bucketList) {
			auto&& lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp, [](const IntervalCombo& c1, const IntervalCombo& c2) {return c1.highValue < c2.highValue; });
			for_each(lowerBoundIterator, bucketList.end(), [&](const IntervalCombo& c) {counter[c.subID]--; });
			});
	}

	for (int i = 0; i < subs; i++)
		if (counter[i] == 0)
		{
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}
bool Rein::deleteSubscription_forward_native(IntervalSub sub) { // 未实现
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size)
	{
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

int Rein::calMemory() {
	long long  size = 0; // Byte
	_for(i, 0, numDimension) {
		//cout <<i<<": "<< sizeof(data[0][i]) << ": ";
		size += sizeof(data[0][i]);
		_for(j, 0, numBucket) {
			size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
			//cout << sizeof(data[0][i][j]) << " ";
		}
		//cout << "\n";
	}
	size = size / 1024 / 1024; // MB
	return (int)size;
}

int Rein::calMemory_forward_native() {
	long long  size = sizeof(fData); // Byte
	_for(i, 0, numDimension) {
		//cout <<i<<": "<< sizeof(data[0][i]) << ": ";
		size += sizeof(fData[i]);
		_for(j, 0, numBucket) {
			size += sizeof(IntervalCombo) * fData[i][j].size();
			//cout << sizeof(data[0][i][j]) << " ";
		}
		//cout << "\n";
	}
	size = size / 1024 / 1024; // MB
	return (int)size;
}

void Rein::match_forward_CBOMP(const Pub& pub, int& matchSubs) {
	bitset<subs> gB, mB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	int att, buck;
	IntervalCombo pubPredicateTmp;
	pubPredicateTmp.lowValue = 0; // No sense.
//	cout<<pub.id<<"\n";
	for (auto&& pair : pub.pairs)
	{
		mB = nB[pair.att];
		attExist[pair.att] = true;
		pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
//		cout<<pair.att<<" ";
		const auto&& lowerBoundIterator = lower_bound(fData[att][buck].begin(), fData[att][buck].end(), pubPredicateTmp, [&](const IntervalCombo& c1, const IntervalCombo& c2) {return c1.highValue < c2.highValue; });
		for_each(lowerBoundIterator, fData[att][buck].end(), [&](const IntervalCombo& c) {
			if (c.lowValue <= pair.value) mB[c.subID]=1; });

		for_each(fData[att].begin(), fData[att].begin() + buck, [&](const vector<IntervalCombo>& bucketList) {
			auto&& lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp, [](const IntervalCombo& c1, const IntervalCombo& c2) {return c1.highValue < c2.highValue; });
			for_each(lowerBoundIterator, bucketList.end(), [&](const IntervalCombo& c) {mB[c.subID] = 1; });
			});
		gB = gB & mB;
	}
//	cout<<"1\n";
	_for(i, 0, atts)
		if (!attExist[i])
			gB = gB & nB[i];
	matchSubs = gB.count();
}

void Rein::calBucketSize() {
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

vector<int> Rein::calMarkNumForBuckets() {
	vector<int> numMarking(numBucket, 0);
	_for(i, 0, numBucket) {
		_for(j, 0, numDimension) {
			_for(k, i, numBucket) {
				numMarking[i] += data[0][j][k].size();
			}
			_for(k, 0, i + 1) {
				numMarking[i] += data[1][j][k].size();
			}
		}
	}
	return numMarking;
}