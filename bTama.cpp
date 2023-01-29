#include "bTama.h"

bTama::bTama(int32_t type)
{
	int nodeNumber = 1 << level;
	numSub = 0;
	//        for (int i = 0; i < atts; i++)
	//            data[i] = new vector<int>[nodeNumber];
	data.resize(atts, vector<vector<int>>(nodeNumber));
	lchild = new int[nodeNumber];
	rchild = new int[nodeNumber];
	mid = new int[nodeNumber];
	nodeCounter = 0;
	initiate(0, 0, valDom - 1, 1);
	nB.resize(atts);
	nnB.resize(atts);
	string TYPE = "Tama";
	if (type == TAMA_FORWARD_CBOMP)
	{
		TYPE = "f" + TYPE + "-CF";
	}
	else if (type == TAMA_BACKWARD1_DMFT)
	{
		TYPE = "f" + TYPE + "-CB1";
	}
	else if (type == TAMA_BACKWARD2_DMFT)
	{
		TYPE = "f" + TYPE + "-CB2";
	}
	else if (type == TAMA_FORWARD_CBOMP_PARALLEL)
	{
		threadPool.initThreadPool(parallelDegree);
		TYPE = "f" + TYPE + "-CF-Parallel" + to_string(parallelDegree);
	}
	else if (type == TAMA_BACKWARD2_DMFT_PARALLEL)
	{
		threadPool.initThreadPool(parallelDegree);
		TYPE = "f" + TYPE + "-CB2-Parallel" + to_string(parallelDegree);
	}
	else if (type == bTAMA_BACKWARD)
	{
		TYPE = "b" + TYPE + "-B";
	}
	else if (type == bTAMA_FORWARD_DMFT)
	{
		TYPE = "b" + TYPE + "-F";
	}
	cout << "ExpID = " << expID << ". " << TYPE << ": level = " << level << "\n";
}

void bTama::initiate(int p, int l, int r, int lvl)
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
	//	cout<<"bTama initialization finishes.\n";
}

int bTama::median(int l, int r)
{
	return (l + r) >> 1;
}

void bTama::insert(IntervalSub sub)
{
	vector<bool> attrExist(atts, false);
	for (int i = 0; i < sub.size; i++)
	{
		insert(0, sub.constraints[i].att, sub.id, 0,
			valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1);
		attrExist[sub.constraints[i].att] = true;
		nnB[sub.constraints[i].att][sub.id] = 1;
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}

void bTama::insert(int p, int att, int subID, int l, int r, int low, int high, int lvl)
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

bool bTama::deleteSubscription(IntervalSub sub)
{
	bool find = true;
	vector<bool> attrExist(atts, false);
	for (int i = 0; i < sub.size; i++)
	{
		if (!deleteSubscription(0, sub.constraints[i].att, sub.id, 0,
			valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1))
			find = false;
		attrExist[sub.constraints[i].att] = true;
		nnB[sub.constraints[i].att][sub.id] = 0;
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 0;
	numSub--;
	return find;
}

bool bTama::deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl)
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

void bTama::match_forward_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	//_for(i, 0, subs)
	//	gB[i] = 1;
	gB.set();
	vector<bool> attExist(atts, false);
	for (int i = 0; i < pub.size; i++)
	{
		mB = nB[pub.pairs[i].att];
		attExist[pub.pairs[i].att] = true;
		match_forward_CBOMP_accurate(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList, mB);
		gB = gB & mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB & nB[i];
	//_for(i, 0, subs) 
	//	if (gB[i])
	//{
	//	++matchSubs;
	//	//cout << "bTama matches sub: " << i << endl;
	//}
	matchSubs = gB.count();
}

void bTama::match_forward_CBOMP_parallel(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // global bitset
	gB.set();
	vector<bool> attExist(atts, false);
	vector<future<bitset<subs>>> threadResult;
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
		  bitset<subs> localB, mB;
		  localB.set();
		  for (int i = begin; i < end; i++)
		  {
			  mB = nB[pub.pairs[i].att];
			  match_forward_CBOMP_accurate(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList, mB);
			  localB = localB & mB;
		  }
		  return localB;
		}));
	}
	for (auto& pa : pub.pairs)
		attExist[pa.att] = true;
	_for(i, 0, atts) if (!attExist[i])
			gB = gB & nB[i];

	for (int i = 0; i < threadResult.size(); i++)
		gB = gB & threadResult[i].get();

	matchSubs = gB.count();
}

// TAMA7-C-BOMP TAMA_FORWARD-CB1
void bTama::match_backward1_DMFT_TAMA_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	vector<bool> attExist(atts, false);
	for (int i = 0; i < pub.size; i++)
	{
		mB = nB[pub.pairs[i].att];
		attExist[pub.pairs[i].att] = true;
		match_forward_CBOMP_accurate(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList, mB);
		gB = gB | (mB.flip());
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	//_for(i, 0, subs) 
	//	if (gB[i])
	//{
	//	++matchSubs;
	//	//cout << "bTama matches sub: " << i << endl;
	//}
	matchSubs = numSub - gB.count();
}

void
bTama::match_forward_CBOMP_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<
	subs>& mB)
{
	if (level == lvl)
	{
		int id = -1;
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue <= value && value <= predicate.highValue)
						mB[id] = 1;
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
		mB[id] = 1;
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_forward_CBOMP_accurate(lchild[p], att, l, mid[p], value, lvl + 1, subList, mB);
	else
		match_forward_CBOMP_accurate(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList, mB);
}

// TAMA8-C-BOMP TAMA_FORWARD-CB2
void bTama::match_backward2_DMFT_TAMA_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // register
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		mB = nnB[pi.att]; // based on a non null bitset/def bitset
		attExist[pi.att] = true;
		match_backward2_DMFT_TAMA_CBOMP_accurate(0, pi.att, 0, valDom - 1, pi.value, 1, subList, mB);
		gB = gB | mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
}

void
bTama::match_backward2_DMFT_TAMA_CBOMP_accurate_parallel(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // global bitset
	vector<bool> attExist(atts, false);
	vector<future<bitset<subs>>> threadResult;
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
		  bitset<subs> localB, mB;
		  for (int i = begin; i < end; i++)
		  {
			  mB = nnB[pub.pairs[i].att]; // based on a non-null bitset/def bitset
			  match_backward2_DMFT_TAMA_CBOMP_accurate(0, pub.pairs[i].att, 0,
				  valDom - 1, pub.pairs[i].value, 1, subList, mB);
			  localB = localB | mB;
		  }
		  return localB;
		}));
	}
	for (auto& pa : pub.pairs)
		attExist[pa.att] = true;
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];

	for (int i = 0; i < threadResult.size(); i++)
		gB = gB | threadResult[i].get();

	matchSubs = numSub - gB.count();
}

void
bTama::match_backward2_DMFT_TAMA_CBOMP_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<
	IntervalSub>& subList, bitset<
	subs>& mB)
{
	if (level == lvl)
	{
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue <= value && value <= predicate.highValue)
						mB[id] = 0;
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
		mB[id] = 0;
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_backward2_DMFT_TAMA_CBOMP_accurate(lchild[p], att, l, mid[p], value, lvl + 1, subList, mB);
	else
		match_backward2_DMFT_TAMA_CBOMP_accurate(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList, mB);
}


// -------------------- backward TAMA_FORWARD 2023.01.25 --------------------

void bTama::insert_backward(IntervalSub sub)
{
	vector<bool> attrExist(atts, false);
	for (const auto& iCnt : sub.constraints)
	{
		if (iCnt.lowValue > 0)
			insert(0, iCnt.att, sub.id, 0, valDom - 1, 0, iCnt.lowValue - 1, 1);
		if (iCnt.highValue + 1 < valDom)
			insert(0, iCnt.att, sub.id, 0, valDom - 1, iCnt.highValue, valDom - 1, 1);
		attrExist[iCnt.att] = true;
		nnB[iCnt.att][sub.id] = 1;
	}
	_for(ai, 0, atts) if (!attrExist[ai])
			nB[ai][sub.id] = 1;
	numSub++;
}

void bTama::match_backward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // register
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		attExist[pi.att] = true;
		match_backward_native(0, pi.att, 0, valDom - 1, pi.value, 1, subList, gB);
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
}

void
bTama::match_backward_native(const int& p, const int& att, const int& l, const int& r, const int& value, const int& lvl, const vector<
	IntervalSub>& subList, bitset<
	subs>& gB)
{
	if (level == lvl)
	{
		for (const auto& id : data[att][p])
		{
			for (const auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue > value || value > predicate.highValue)
						gB[id] = 1;
					break;
				}
		}
		return;
	}
	for (const auto& id : data[att][p])
		gB[id] = 1;
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_backward_native(lchild[p], att, l, mid[p], value, lvl + 1, subList, gB);
	else
		match_backward_native(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList, gB);
}

void bTama::match_forward_DMFT_bTAMA(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // register
	gB.set();
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		attExist[pi.att] = true;
		match_forward_DMFT_bTAMA(0, pi.att, 0, valDom - 1, pi.value, 1, subList, gB);
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB & nB[i];
	matchSubs = gB.count();
}

void
bTama::match_forward_DMFT_bTAMA(const int& p, const int& att, const int& l, const int& r, const int& value, const int& lvl, \
    const vector<IntervalSub>& subList, bitset<subs>& gB)
{
	if (level == lvl)
	{
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att)
				{
					if (predicate.lowValue > value || value > predicate.highValue)
						gB[id] = 0;
					break;
				}
		}
		return;
	}
	for (auto& id : data[att][p])
		gB[id] = 0;
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_forward_DMFT_bTAMA(lchild[p], att, l, mid[p], value, lvl + 1, subList, gB);
	else
		match_forward_DMFT_bTAMA(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList, gB);
}

int bTama::calMemory()
{
	long long size = 0; // Byte
	size += 3 * sizeof(int) * (1 << level);
	_for(i, 0, atts) _for(j, 0, 1 << level) size += sizeof(int) * data[i][j].size();
	size = size / 1024 / 1024; // MB
	return (int)size;
}