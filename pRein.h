//
// Created by swh on 2021/11/24.
//

#ifndef MAC_PREIN_H
#define MAC_PREIN_H

#include <cstring>
#include "util.h"
#include "constant.h"
#include "omp.h"
#include <algorithm>
#include <unordered_set>
#include <thread>
//#include <boost/thread/thread_pool.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/bind.hpp>
//#include <boost/asio.hpp>
#include "ThreadPool.h"
#include "thpool.h"
#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

struct parallelData{
	bool* bits;
	bool* attExist;
	vector<vector<vector<Combo>>>* data;
	const Pub* pub;
	int begin;
	int end;
	int buckStep;
};
void pReinThreadFunction1(void *pd1);

// Parallel Rein in openmp
class pRein {
	int numSub, numDimension, buckStep,pD;    // parallelDegree
//	vector<bool> bits;
	vector<vector<vector<Combo>>> data[2];    // 0:left parenthesis, 1:right parenthesis
//	boost::thread_group vecThreads;
//	boost::asio::thread_pool threadPool(parallelData);
//	ThreadPool_asio threadPoolAsio;
	ThreadPool threadPool;
//	threadpool thpool;
public:
	int numBucket;
	double compareTime = 0.0; // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;    // 标记时间
	double bitTime = 0.0;     // 遍历bits数组得到结果所需的时间
	vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	pRein();
	//void insert(Sub sub);
	void insert(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(const Pub& pub, int& matchSubs);
	void parallelMatch(const Pub& pub, int& matchSubs);
	bool deleteSubscription(IntervalSub sub);
	void calBucketSize(); // 计算bucketSize
	int calMemory();     // 计算占用内存大小, 返回MB


	//	bool bits[],bool attExist[],vector<vector<vector<Combo>>> data[], ,int buckStep
	void pReinThreadFunction2(const Pub& pub,int begin,int end);

};

#endif //MAC_PREIN_H
