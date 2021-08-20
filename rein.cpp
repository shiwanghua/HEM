#include "rein.h"

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
//            data[cnt.att][0][c.val / buckStep].push_back(c);
//            data[cnt.att][1][c.val / buckStep].push_back(c);
//        }
//        else if (cnt.op == 1)   // >=
//            data[cnt.att][0][c.val / buckStep].push_back(c);
//        else                    // >=
//            data[cnt.att][1][c.val / buckStep].push_back(c);
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
}
//void Rein::insert(IntervalSub sub)
//{
//	for (int i = 0; i < sub.size; i++)
//	{
//		IntervalCnt cnt = sub.constraints[i];
//		Combo c;
//		c.val = cnt.lowValue;
//		c.subID = sub.id;
//		data[cnt.att][0][cnt.lowValue / buckStep].push_back(c);
//		c.val = cnt.highValue;
//		data[cnt.att][1][cnt.highValue / buckStep].push_back(c);
//	}
//}

//void Rein::match(const Pub &pub, int &matchSubs, const vector<Sub> &subList)
//{
//    vector<bool> bits (subList.size(), false);
//
//    for (int i = 0; i < pub.size; i++)
//    {
//        int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//        for (int k = 0; k < data[att][0][buck].size(); k++)
//            if (data[att][0][buck][k].val > value)
//                bits[data[att][0][buck][k].subID] = true;
//        for (int j = buck + 1; j < bucks; j++)
//            for (int k = 0; k < data[att][0][j].size(); k++)
//                bits[data[att][0][j][k].subID] = true;
//
//        for (int k = 0; k < data[att][1][buck].size(); k++)
//            if (data[att][1][buck][k].val < value)
//                bits[data[att][1][buck][k].subID] = true;
//        for (int j = buck - 1; j >= 0; j--)
//            for (int k = 0; k < data[att][1][j].size(); k++)
//                bits[data[att][1][j][k].subID] = true;
//    }
//
//    for (int i = 0; i < subList.size(); i++)
//        if (!bits[i])
//            ++ matchSubs;
//}

// 01在第一维
void Rein::match(const Pub& pub, int& matchSubs)
{
	vector<bool> bits(numSub, false);

	for (int i = 0; i < pub.size; i++)
	{
		Timer compareStart;
		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;
		compareTime += (double)compareStart.elapsed_nano();

		Timer markStart;
		for (int j = buck + 1; j < numBucket; j++)
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
		markTime += (double)markStart.elapsed_nano();
	}

	Timer bitStart;
	for (int i = 0; i < numSub; i++)
		if (!bits[i])
		{
			++matchSubs;
			//cout << "rein: " << i << endl;
		}
	bitTime += (double)bitStart.elapsed_nano();
}

// 01在第二维
//void Rein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//
//	for (int i = 0; i < pub.size; i++)
//	{
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//
//		for (int k = 0; k < data[att][0][buck].size(); k++)
//			if (data[att][0][buck][k].val > value)
//				bits[data[att][0][buck][k].subID] = true;
//
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[att][0][j].size(); k++)
//				bits[data[att][0][j][k].subID] = true;
//
//
//		for (int k = 0; k < data[att][1][buck].size(); k++)
//			if (data[att][1][buck][k].val < value)
//				bits[data[att][1][buck][k].subID] = true;
//
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[att][1][j].size(); k++)
//				bits[data[att][1][j][k].subID] = true;
//	}
//
//	for (int i = 0; i < numSub; i++)
//		if (!bits[i])
//			++matchSubs;
//}

void Rein::calBucketSize() {
	bucketSub.clear();
	bucketSub.resize(numBucket);
	_for(i,0,numDimension)
		_for(j, 0, numBucket)
		{
		_for(k, 0, data[0][i][j].size())
			bucketSub[j].insert(data[0][i][j][k].subID);
		_for(k, 0, data[1][i][j].size())
			bucketSub[j].insert(data[1][i][j][k].subID);
		}
}

int Rein::calMemory() {
	int size = 0; // Byte
	_for(i, 0, numDimension)
		_for(j, 0, numBucket)
		size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	return size / 1024 / 1024; // MB
}