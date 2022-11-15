#include "Tama.h"

Tama::Tama(int type)
{
	int nodeNumber = 1 << level;
	data.resize(atts, vector<vector<int>>(nodeNumber));
	lchild = new int[nodeNumber];
	rchild = new int[nodeNumber];
	mid = new int[nodeNumber];
	nodeCounter = 0;
	initiate(0, 0, valDom - 1, 1);
	string TYPE = "Tama";
	if (type == TAMA_PARALLEL_LOCK)
	{
		mutexs.resize(subs);
		_for(i, 0, subs) mutexs[i] = new mutex;
		threadPool.initThreadPool(parallelDegree);
		TYPE += "-Parallel" + to_string(parallelDegree) + "-Lock";
	}
	else if (type == TAMA_PARALLEL_REDUCE)
	{
		threadPool.initThreadPool(parallelDegree);
		TYPE += "-Parallel" + to_string(parallelDegree) + "-Reduce";
	}
	cout << "ExpID = " << expID << ", " << TYPE << ": level = " << level << "\n";
}

Tama::~Tama()
{
	_for(i, 0, mutexs.size()) delete mutexs[i];
}

void Tama::initiate(int p, int l, int r, int lvl)
{
	if (lvl == level)  // level 从1开始
		return;
	mid[p] = median(l, r);
//	printf("p %d, l %d, r %d, lvl %d\n",p,l,r,lvl);
//	fflush(stdout);
	if (l != r)
	{
		lchild[p] = ++nodeCounter;// 根节点是0, 第一个左子是1
		initiate(nodeCounter, l, mid[p], lvl + 1);
		rchild[p] = ++nodeCounter;
		initiate(nodeCounter, mid[p] + 1, r, lvl + 1);
	}
//	cout<<"TAMA initialization finishes.\n";
}

int Tama::median(int l, int r)
{
	return (l + r) >> 1;
}

void Tama::insert(IntervalSub sub)
{
	for (int i = 0; i < sub.size; i++)
		insert(0, sub.constraints[i].att, sub.id, 0,
			valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1);
}

void Tama::insert(int p, int att, int subID, int l, int r, int low, int high, int lvl)
{
	if ((low <= l && high >= r) || lvl == level)
	{
		data[att][p].push_back(subID);
		return;
	}
	if (high <= mid[p])
		insert(lchild[p], att, subID, l, mid[p], low, high, lvl + 1);
	else if (low > mid[p])
		insert(rchild[p], att, subID, mid[p] + 1, r, low, high, lvl + 1);
	else
	{
		insert(lchild[p], att, subID, l, mid[p], low, high, lvl + 1);
		insert(rchild[p], att, subID, mid[p] + 1, r, low, high, lvl + 1);
	}
}

bool Tama::deleteSubscription(IntervalSub sub)
{
	bool find = true;
	for (int i = 0; i < sub.size; i++)
		if (!deleteSubscription(0, sub.constraints[i].att, sub.id, 0,
			valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1))
			find = false;
	return find;
}

bool Tama::deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl)
{
	if ((low <= l && high >= r) || lvl == level)
	{
		vector<int>::iterator it;
		for (it = data[att][p].begin(); it != data[att][p].end(); it++)
			if (*it == subID)
			{
				data[att][p].erase(it); // it = 
				return 1;
			}
		return false;
	}
	if (high <= mid[p])
		return deleteSubscription(lchild[p], att, subID, l, mid[p], low, high, lvl + 1);
	else if (low > mid[p])
		return deleteSubscription(rchild[p], att, subID, mid[p] + 1, r, low, high, lvl + 1);
	else
	{
		bool find = deleteSubscription(lchild[p], att, subID, l, mid[p], low, high, lvl + 1);
		if (!deleteSubscription(rchild[p], att, subID, mid[p] + 1, r, low, high, lvl + 1))
			find = false;  // 左右都必须找到
		return find;
	}
}

void Tama::match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size;
	for (int i = 0; i < pub.size; i++)
		match_accurate(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList);
	for (int i = 0; i < subList.size(); i++)
		if (counter[i] == 0)
		{
			++matchSubs;
			//cout << "tama matches sub: " << i << endl;
		}
}

void Tama::match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList)
{
	if (level == lvl)
	{
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue <= value && value <= predicate.highValue)
						--counter[id];
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
		--counter[id];
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_accurate(lchild[p], att, l, mid[p], value, lvl + 1, subList);
	else
		match_accurate(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList);
}

void Tama::match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size;
	for (int i = 0; i < pub.size; i++)
		match_vague(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1);
	for (int i = 0; i < subList.size(); i++)
		if (counter[i] == 0)
		{
			++matchSubs;
			//cout << "tama matches sub: " << i << endl;
		}
}

void Tama::match_vague(int p, int att, int l, int r, const int value, int lvl)
{
	for (auto& id : data[att][p])
		--counter[id];
	if (level == lvl || l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_vague(lchild[p], att, l, mid[p], value, lvl + 1);
	else
		match_vague(rchild[p], att, mid[p] + 1, r, value, lvl + 1);
}

void Tama::match_parallel_lock(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size;
	vector<future<bool>> threadResult;
	int seg = pub.size / parallelDegree;
	int remainder = pub.size % parallelDegree;
	int tId = 0, end;
	for (int begin = 0; begin < pub.size; begin = end, tId++)
	{
		if (tId < remainder)
			end = begin + seg + 1;
		else end = begin + seg;
		threadResult.emplace_back(threadPool.enqueue([this, &pub, begin, end, subList]
		{
		  for (int i = begin; i < end; i++)
		  {
			  match_parallel_lock(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList);
		  }
		  return true;
		}));
	}
	for (int i = 0; i < threadResult.size(); i++)
		threadResult[i].get();
	for (int i = 0; i < subList.size(); i++)
		if (counter[i] == 0)
			++matchSubs;
}

void
Tama::match_parallel_lock(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList)
{
	if (level == lvl)
	{
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue <= value && value <= predicate.highValue)
					{
						mutexs[id]->lock();
						--counter[id];
						mutexs[id]->unlock();
					}
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
	{
		mutexs[id]->lock();
		--counter[id];
		mutexs[id]->unlock();
	}
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_parallel_lock(lchild[p], att, l, mid[p], value, lvl + 1, subList);
	else
		match_parallel_lock(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList);
}

void Tama::match_parallel_reduce(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size;
	vector<future<vector<int32_t>>> threadResult;
//	vector<future< array<int32_t, subs>>> threadResult;
	int seg = pub.size / parallelDegree;
	int remainder = pub.size % parallelDegree;
	int tId = 0, end;
	for (int begin = 0; begin < pub.size; begin = end, tId++)
	{
		if (tId < remainder)
			end = begin + seg + 1;
		else end = begin + seg;
		threadResult.emplace_back(threadPool.enqueue([this, &pub, begin, end, &subList]
		{
//		  array<int32_t, subs> pCounter;
//		  pCounter.fill(0);
			vector<int32_t> pCounter(subs);
		  for (int i = begin; i < end; i++)
		  {
			  match_parallel_reduce(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList, pCounter);
		  }
		  return pCounter;
		}));
	}
	for (int i = 0; i < threadResult.size(); i++)
	{
//		array<int32_t, subs> pCounter = threadResult[i].get();
		vector<int32_t> pCounter = threadResult[i].get();
		_for(i, 0, subs) counter[i] -= pCounter[i];
	}

	for (int i = 0; i < subList.size(); i++)
		if (counter[i] == 0)
			++matchSubs;
}

void
Tama::match_parallel_reduce(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, vector<int32_t>& pCounter)
{
	if (level == lvl)
	{
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue <= value && value <= predicate.highValue)
					{
//						printf("sub%d match att%d\n", id, att);
						pCounter[id]++;
					}
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
	{
		pCounter[id]++;
	}
	if (l == r)
		return;
	else if (value <= mid[p])
		match_parallel_reduce(lchild[p], att, l, mid[p], value, lvl + 1, subList, pCounter);
	else
		match_parallel_reduce(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList, pCounter);
}

int Tama::calMemory()
{
	long long size = 0; // Byte
	size += 3 * sizeof(int) * (1 << level);
	_for(i, 0, atts) _for(j, 0, 1 << level) size += sizeof(int) * data[i][j].size();
	size = size / 1024 / 1024; // MB
	return (int)size;
}