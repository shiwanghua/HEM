#include "bTama.h"

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
	for (int i = 0; i < sub.size; i++) {
		insert(0, sub.constraints[i].att, sub.id, 0, valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1);
		attrExist[sub.constraints[i].att] = true;
	}
	_for(i, 0, atts)
		if (!attrExist[i]) nB[i][sub.id] = 1;
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

bool bTama::deleteSubscription(IntervalSub sub) {
	bool find = true;
	for (int i = 0; i < sub.size; i++)
		if (!deleteSubscription(0, sub.constraints[i].att, sub.id, 0, valDom - 1, sub.constraints[i].lowValue, sub.constraints[i].highValue, 1))
			find = false;
	return find;
}

bool bTama::deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl) {
	if ((low <= l && high >= r) || lvl == level)
	{
		vector<int>::iterator it;
		for (it = data[att][p].begin(); it != data[att][p].end(); it++)
			if (*it == subID) {
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
void bTama::match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB,mB; // global bitset
	//_for(i, 0, subs)
	//	gB[i] = 1;
	gB.set();
	for (int i = 0; i < pub.size; i++) {
		mB = nB[pub.pairs[i].att];
		match_accurate(0, pub.pairs[i].att, 0, valDom - 1, pub.pairs[i].value, 1, subList,mB);
		gB = gB & mB;
	}
	
	//_for(i, 0, subs) 
	//	if (gB[i])
	//{
	//	++matchSubs;
	//	//cout << "bTama matches sub: " << i << endl;
	//}
	matchSubs = gB.count();
}

void bTama::match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (level == lvl) {
		int id = -1;
		for (auto& id : data[att][p])
		{
			for (auto& predicate : subList[id].constraints)
				if (att == predicate.att) {
					if (predicate.lowValue <= value && value <= predicate.highValue)
						mB[id] = 1;
					break;
				}
		}
		return;
	}
	for (int i = 0; i < data[att][p].size(); i++)
		mB[data[att][p][i]] = 1;
	if (l == r) // 这里l有可能等于r吗？当取值范围比较小, 层数很高时会等于; 当事件值刚好等于边界时也会等于!
		return;
	else if (value <= mid[p])
		match_accurate(lchild[p], att, l, mid[p], value, lvl + 1, subList,mB);
	else
		match_accurate(rchild[p], att, mid[p] + 1, r, value, lvl + 1, subList,mB);
}

int bTama::calMemory() {
	long long  size = 0; // Byte
	size += 3 * sizeof(int) * (1 << level);
	_for(i, 0, atts)
		_for(j, 0, 1 << level)
		size += sizeof(int) * data[i][j].size();
	size = size / 1024 / 1024; // MB
	return (int)size;
}