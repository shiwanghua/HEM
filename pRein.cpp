//
// Created by swh on 2021/11/24.
//

#include "pRein.h"

pRein::pRein() : numSub(0), numDimension(atts), pD(parallelDegree) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	cout << "ExpID = " << expID << ". pRein: bucketStep = " << buckStep << ", numBucket = " << numBucket
		 << ", parallelDegree = " << parallelDegree << endl;
	bucketSub.resize(numBucket);
	data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
	data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
//	boost::asio::thread_pool pool(parallelDegree);
//	_for(i,0,parallelDegree){
//		boost::thread t;
//		vecThreads.add_thread(&t);
//	}
//	threadPoolAsio.initThreads(parallelDegree);
	threadPool.initThreadPool(parallelDegree);
//	thpool = thpool_init(parallelDegree);
}

void pRein::insert(IntervalSub sub) {
	for (int i = 0; i < sub.size; i++) {
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

// 01在第一维 计算时间组成
//void pRein::match(const Pub& pub, int& matchSubs)
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
void pRein::match(const Pub &pub, int &matchSubs) {
	vector<bool> bits(numSub, false);
	vector<bool> attExist(numDimension, false);
	for (int i = 0; i < pub.size; i++) {
		/*printf("rank %d of %d: %.4f\n", omp_get_thread_num(), omp_get_num_threads(), (double)logStart.elapsed_nano());
		fflush(stdout);*/
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
//	int seg = (pub.size + pD - 1) / pD;
//	parallelData* pdata;
//	for (int i = 0; i < pub.size; i += seg) {
//		pdata=new parallelData;
//		pdata->bits = bits;
//		pdata->attExist = attExist;
//		pdata->data = data;
//		pdata->pub = &pub;
//		pdata->buckStep = buckStep;
//		pdata->begin = i;
//		pdata->end = min(pub.size, i+seg);
//		printf("pub%d, begin=%d, end=%d\n", pub.id, pdata->begin, pdata->end);
//		fflush(stdout);

//		thpool_add_work(thpool, pReinThreadFunction1, pdata);

//		threads[i] = thread(pReinThreadFunction, &pdata);
//		threads[i] = thread(pReinThreadFunction, ref(bits),ref(attExist),ref(data),ref(pub),seg * i,min(pub.size, seg * (i + 1)),buckStep);
//		threads[i] = thread(pReinThreadFunction, bits, attExist, data, pub, seg * i, min(pub.size, seg * (i + 1)),
//							buckStep);
//		boost::asio::post(threadPool, [&](){cout<<i;});
//	}

//		threadPool.enqueue(pReinThreadFunction2,pub,i,min(i+seg, pub.size)); // does not capture 'this'
	vector<future<bitset<subs>>> threadResult;
	int seg = pub.size / parallelDegree;
	int remainder=pub.size%parallelDegree;
	int tId=0,end;
	for (int begin = 0; begin < pub.size; begin = end,tId++) {
		if(tId<remainder)
			end=begin+seg+1;
		else end=begin+seg;
		threadResult.emplace_back(threadPool.enqueue([this, &pub, begin, end] {
//			printf("pub%d, begin=%d\n", pub.id, begin);
			bitset<subs> b;//=new bitset<subs>;
			for (int i = begin; i < end; i++) {
				int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;

				for (int k = 0; k < data[0][att][buck].size(); k++)
					if (data[0][att][buck][k].val > value)
						b[data[0][att][buck][k].subID] = true;
				for (int k = 0; k < data[1][att][buck].size(); k++)
					if (data[1][att][buck][k].val < value)
						b[data[1][att][buck][k].subID] = true;

				for (int j = buck + 1; j < data[1][att].size(); j++)
					for (int k = 0; k < data[0][att][j].size(); k++)
						b[data[0][att][j][k].subID] = true;
				for (int j = buck - 1; j >= 0; j--)
					for (int k = 0; k < data[1][att][j].size(); k++)
						b[data[1][att][j][k].subID] = true;
			}
			return b;
		}));
	}

	bitset<subs> gb;

	if (pub.size < atts) {
		vector<bool> attExist(atts, false);
		for (const auto item: pub.pairs)
			attExist[item.att] = true;
		for (int i = 0; i < numDimension; i++)
			if (!attExist[i]) {
//			cout << "Null attribute: " << i << endl;
				for (int j = 0; j < numBucket; j++)
					for (int k = 0; k < data[0][i][j].size(); k++)
						gb[data[0][i][j][k].subID] = true;
			}
	}

//  one thread merges 4 results
/*	vector<bitset<subs>*> threadResult2;
	for (int i = 0; i < parallelDegree; i++)
		threadResult2.emplace_back(threadResult[i].get());
	int pDi = parallelDegree >> 2, pDn = parallelDegree;
	while (pDn > 3) {
		for (int i = 0; i < pDi; i++) {
			threadResult[i] = threadPool.enqueue([&, i](int pDi) {
				           bitset<subs>* pb=new bitset<subs>;
                                           *pb=(*pb)|(*threadResult2[i] | *threadResult2[i + pDi] | *threadResult2[i + (pDi << 1)] |*threadResult2[i + 3 * pDi]);
                                           return pb;
			}, pDi);
		}
		for (int i = 0; i < pDi; i++) {
			threadResult2[i] = threadResult[i].get();
		}
		if (pDi < 4) {
			for (int i = 1; i < pDi; i++)
				*(threadResult2[0]) |= *(threadResult2[i]);
		}
		pDn = pDi;
		pDi = pDi >> 2;
	}
*/

	// one thread merges two results
//	vector<bitset<subs>> threadResult2;
//	for (int i = 0; i < parallelDegree; i++)
//		threadResult2.emplace_back(threadResult[i].get());
//	int pDi = (parallelDegree + 1) >> 1, pDn = parallelDegree;
//	while (pDn > 1) {
//		for (int i = 0; i < pDi; i++) {
//			if (i + pDi < pDn)
//				threadResult[i] = threadPool.enqueue([&, i](int j) {
//					return threadResult2[i] | threadResult2[j];
//				}, i + pDi);
//		}
//		for (int i = 0; i < pDi; i++) {
//			if (i + pDi < pDn)
//				threadResult2[i]=threadResult[i].get();
//		}
//		pDn = pDi;
//		pDi = (pDi + 1) >> 1;
//	}

	// 死循环Bug only use threadResult to store result
//	for (int i = 0; i < parallelDegree; i++)
//		threadResult[i].wait();
//
//	int pDi = (parallelDegree + 1) >> 1, pDn = parallelDegree;
//	while (pDn > 1) {
//		for (int i = 0; i < pDi; i++) {
//			if (i + pDi < pDn)
//				threadResult[i] = threadPool.enqueue([&, i](int j) {
//					printf("pub%d, i=%d, j=%d\n", pub.id, i, j);
//					fflush(stdout);
//					return threadResult[i].get() | threadResult[j].get();
//				}, i + pDi);
//		}
//		for (int i = 0; i < pDi; i++) {
//			printf("i=%d\n",i);
//			if (i + pDi < pDn)
//				threadResult[i].wait();
//		}
//		pDn = pDi;
//		pDi = (pDi + 1) >> 1;
//		printf("pDi=%d, pDn=%d\n",pDi,pDn);
//		fflush(stdout);
//	}
//	printf("2 pDi=%d, pDn=%d\n",pDi,pDn);
//	fflush(stdout);

/*	if (pub.size < atts) {
		gb |= *(threadResult2[0]);
		matchSubs = subs - gb.count();
	} else {
		matchSubs = subs - (*(threadResult2[0])).count();
	}
*/
	// without optimization:
#ifdef DEBUG
	Timer mergeStart;
#endif
	for (int i = 0; i < threadResult.size(); i++)
		gb |= threadResult[i].get();
#ifdef DEBUG
	mergeTime+=(double)mergeStart.elapsed_nano();
	Timer bitStart;
#endif
	matchSubs = numSub - gb.count();
#ifdef DEBUG
	bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG

//	printf("\n");
//	fflush(stdout);
//		thpool_wait(thpool);
//		for (int i = 0; i < subs; i++)
//			if (!bits[i]) {
//				++matchSubs;
//				//cout << "rein matches sub: " << i << endl;
//			}
//	}
}


void pReinThreadFunction1(void *pd1) {
	parallelData *pd = (parallelData *) pd1;
//	printf("SubThread: pub%d, begin=%d, end=%d\n\n", pd->pub->id, pd->begin, pd->end); //this_thread::get_id()
//	fflush(stdout);
	int value, att, buck;
	for (int i = pd->begin; i < pd->end; i++) {
		value = pd->pub->pairs[i].value, att = pd->pub->pairs[i].att, buck = value / pd->buckStep;
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
	delete pd;
}

//void pRein::pReinThreadFunction2(const Pub &pub, int begin, int end) {
//	printf("pub%d, begin=%d, end=%d", pub.id, begin, end);
//	for (int i = begin; i < end; i++) {
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		attExist[att] = true;
//
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > value)
//				bits[data[0][att][buck][k].subID] = true;
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < value)
//				bits[data[1][att][buck][k].subID] = true;
//
//		for (int j = buck + 1; j < data[1][att].size(); j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
//	}
//}
