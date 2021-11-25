//
// Created by swh on 2021/11/24.
//

#include "pRein.h"

pRein::pRein() : numSub(0), numDimension(atts), pD(parallelDegree) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "ExpID = " << expID << ". pRein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
	bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
}

void pRein::insert(IntervalSub sub) {
	for (int i = 0; i < sub.size; i++) {
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

bool pRein::deleteSubscription(IntervalSub sub) {
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
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

// 01�ڵ�һά ����ʱ�����
//void pRein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//	vector<bool> attExist(numDimension, false);
//	for (int i = 0; i < pub.size; i++)
//	{
//		Timer compareStart;
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		attExist[att] = true;
//		// ����������forѭ��ע���˾���ģ��ƥ��, ����Tama
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

// 01�ڵ�һά ������ʱ�����
void pRein::match(const Pub &pub, int &matchSubs) {
	vector<bool> bits(numSub, false);
	vector<bool> attExist(numDimension, false);
	Timer logStart;
//#pragma omp parallel shared(pub, attExist, bits, data, logStart)  private(stdout)
{
	//#pragma omp parallel for schedule(static, 5) num_threads(pD) default(none) shared(pub, attExist, bits, data, logStart) private(stdout) //dynamic
#pragma omp parallel for num_threads(pD)  schedule(static, 10) default(shared) // private(stdout)
	for (int i = 0; i < pub.size; i++) {
		/*printf("rank %d of %d: %.4f\n", omp_get_thread_num(), omp_get_num_threads(), (double)logStart.elapsed_nano());
		fflush(stdout);*/
		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		attExist[att] = true;
		// ����������forѭ��ע���˾���ģ��ƥ��, ����Tama
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
}

//#pragma omp parallel for schedule(dynamic) num_threads(pD) default(none) shared(numDimension,attExist,bits,data)
	for (int i = 0; i < numDimension; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subs; i++)
		if (!bits[i]) {
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}

void pRein::calBucketSize() {
	bucketSub.clear();
	bucketSub.resize(numBucket);
	_for(i, 0, numDimension) _for(j, 0, numBucket) {
			_for(k, 0, data[0][i][j].size()) bucketSub[j].insert(data[0][i][j][k].subID);
			_for(k, 0, data[1][i][j].size()) bucketSub[j].insert(data[1][i][j][k].subID);
		}
}

int pRein::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, numDimension) _for(j, 0, numBucket) size +=
													   sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	size = size / 1024 / 1024; // MB
	return (int) size;
}

void pRein::parallelMatch(const Pub &pub, int &matchSubs) {
	bool *bits = new bool[numSub];
	memset(bits, 0, sizeof(bool) * numSub);
	bool *attExist = new bool[numDimension];
	memset(attExist, 0, sizeof(bool) * numDimension);
	parallelData pdata;
	pdata.bits = bits;
	pdata.attExist = attExist;
	pdata.data = data;
	pdata.pub = &pub;
	pdata.buckStep = buckStep;
	int seg = (pub.size+pD-1) / pD;
	thread* threads=new thread[pD];
	for (int i = 0; i < pD; i++) {
		pdata.begin = seg * i;
		pdata.end = min(pub.size, seg * (i + 1));
		threads[i] = thread(pReinThreadFunction, &pdata);
	}
	_for(i, 0, pD) {
		threads[i].join();
	}
	for (int i = 0; i < numDimension; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subs; i++)
		if (!bits[i]) {
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}

void pReinThreadFunction(void *pd1) {
	parallelData* pd = (parallelData*)pd1;
	printf("ThreadID: %d\n", this_thread::get_id());
	fflush(stdout);
	for (int i = pd->begin; i < pd->end; i++) {
		int value = pd->pub->pairs[i].value, att = pd->pub->pairs[i].att, buck = value / pd->buckStep;
		pd->attExist[att] = true;

		for (int k = 0; k < pd->data[0][att][buck].size(); k++)
			if (pd->data[0][att][buck][k].val > value)
				pd->bits[pd->data[0][att][buck][k].subID] = true;
		for (int k = 0; k < pd->data[1][att][buck].size(); k++)
			if (pd->data[1][att][buck][k].val < value)
				pd->bits[pd->data[1][att][buck][k].subID] = true;

		for (int j = buck + 1; j < pd->data[1][att].size(); j++)
			for (int k = 0; k < pd->data[0][att][j].size(); k++)
				pd->bits[pd->data[0][att][j][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < pd->data[1][att][j].size(); k++)
				pd->bits[pd->data[1][att][j][k].subID] = true;
	}
}