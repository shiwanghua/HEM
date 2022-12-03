#include "BGTree.h"

BGTree::BGTree()
{
	numSub = 0;
	boundaryNumSub = BoundaryNumSub;// 32;
	numNode = 0;
	initHeight = initH;
	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);

	_for(i, 0, atts)
	{
		roots[i] = new bluenode(0, valDom - 1, ++numNode, 1, {}, nullptr, nullptr, nullptr, nullptr, nullptr);
		initBlueNode(roots[i]);
	}
	height = initHeight;

	nB.resize(atts);
	nnB.resize(atts);

	cout << "ExpID = " << expID << ". BGTree: height = " << height << "\n";
}

BGTree::~BGTree()
{
	_for(i, 0, atts)
	{
		releaseBlueNode(roots[i]);
	}
}

void BGTree::initBlueNode(bluenode*& r)
{
	if (r->levelid == initHeight)
	{
		return;
	}
	int nextLevelID = r->levelid + 1;
	r->lowGreenChild = new lgreennode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->lowGreenChild); // l<=mid<h
	r->highGreenChild = new hgreennode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->highGreenChild);
	r->leftBlueChild = new bluenode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr,
		nullptr); // l<=h<=mid
	initBlueNode(r->leftBlueChild);
	r->rightBlueChild = new bluenode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr,
		nullptr); // h>=l>mid
	initBlueNode(r->rightBlueChild);
}
void BGTree::initGreenNode(greennode* r)
{
	if (r->levelid == initHeight)
	{
		return;
	}
	int nextLevelID = r->levelid + 1;
	r->leftChild = new greennode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new greennode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightChild);
}

void BGTree::releaseBlueNode(bluenode*& r)
{
	if (r->lowGreenChild) releaseGreenNode(r->lowGreenChild);
	if (r->highGreenChild) releaseGreenNode(r->highGreenChild);
	if (r->leftBlueChild) releaseBlueNode(r->leftBlueChild);
	if (r->rightBlueChild) releaseBlueNode(r->rightBlueChild);
	if (r->bst != nullptr) delete r->bst;
//	delete r->lowGreenChild, r->highGreenChild, r->leftBlueChild, r->rightBlueChild;
	delete r;
}
void BGTree::releaseGreenNode(greennode* r)
{
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->bst != nullptr) delete r->bst;
	delete r;
}

void BGTree::insert(IntervalSub sub)
{
	vector<bool> attrExist(atts, false);
	subPredicate[sub.id] = sub.size;
	for (auto&& c : sub.constraints)
	{
		insertIntoBlueNode(roots[c.att], sub.id, c.lowValue,
			c.highValue);
		attrExist[c.att] = true;
		nnB[c.att][sub.id] = 1;
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}
void BGTree::insertIntoBlueNode(bluenode*& r, const int& subID, const int& l, const int& h)
{
	r->subids.push_back(subID);
	if (r->bst == nullptr)
	{
		if (r->subids.size() > boundaryNumSub)
		{
			r->bst = new bitset<subs>;
			for (auto&& id : r->subids)
				(*r->bst)[id] = 1;
		}
	}
	else
	{
		(*r->bst)[subID] = 1;
	}

	if (r->mid == l || r->mid == h)
	{
		r->midEqual.push_back(subID);
	}

	if (r->leftBlueChild != nullptr)
	{ // 有子节点
		if (h <= r->mid)
			insertIntoBlueNode(r->leftBlueChild, subID, l, h);
		else if (l > r->mid)
			insertIntoBlueNode(r->rightBlueChild, subID, l, h);
		else
		{  // l<=mid<h
			r->midEqual.push_back(subID);
			insertIntoGreenNode(r->lowGreenChild, subID, l);
			insertIntoGreenNode(r->highGreenChild, subID, h);
		}
	}
}
void BGTree::insertIntoGreenNode(greennode* r, const int& subID, const int& v)
{
	r->subids.push_back(subID);
	if (r->bst == nullptr)
	{
		if (r->subids.size() > boundaryNumSub)
		{
			r->bst = new bitset<subs>;
			for (auto&& id : r->subids)
				(*r->bst)[id] = 1;
		}
	}
	else
	{
		(*r->bst)[subID] = 1;
	}

	if (r->leftChild != nullptr)
	{  // 有左子必有右子节点
		if (v <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, v);
		else insertIntoGreenNode(r->rightChild, subID, v);
	}
}

bool BGTree::deleteSubscription(IntervalSub sub)
{
	subPredicate[sub.id] = -1;
	bool find = true;
	for (auto&& c : sub.constraints)
		find &= deleteFromBlueNode(roots[c.att], sub.id, c.lowValue, c.highValue);
	return find;
}
bool BGTree::deleteFromBlueNode(bluenode*& r, const int& subID, const int& l, const int& h)
{
	bool find = false;
	for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++)
	{
		if (*it == subID)
		{
			r->subids.erase(it);
			find = true;
			break;
		}
	}
	if (!find) return false;

	if (r->bst)
	{
		if ((*(r->bst))[subID])
			(*(r->bst))[subID] = 0;
		else return false;
	}

	if (r->mid == l || r->mid == h)
	{
		find = false;
		for (auto it = r->midEqual.cbegin(); it != r->midEqual.cend(); it++)
		{
			if (*it == subID)
			{
				r->midEqual.erase(it);
				find = true;
				break;
			}
		}
	}
	if (!find) return false;

	if (r->leftBlueChild != nullptr)
	{
		if (h <= r->mid)
			find = deleteFromBlueNode(r->leftBlueChild, subID, l, h);
		else if (l > r->mid)
			find = deleteFromBlueNode(r->rightBlueChild, subID, l, h);
		else
		{  // l<mid<h
			find = deleteFromGreenNode(r->lowGreenChild, subID, l);
			find &= deleteFromGreenNode(r->highGreenChild, subID, h);
		}
	}

	return find;
}
bool BGTree::deleteFromGreenNode(greennode* r, const int& subID, const int& v)
{
	bool find = false;
	for (vector<int>::const_iterator it = r->subids.begin(); it != r->subids.cend(); it++)
	{
		if (*it == subID)
		{
			r->subids.erase(it);
			find = true;
			break;
		}
	}
	if (!find) return false;

	if (r->bst)
	{
		if ((*(r->bst))[subID])
			(*(r->bst))[subID] = 0;
		return false;
	}

	if (r->leftChild != nullptr)
	{
		if (v <= r->mid)
			find = deleteFromGreenNode(r->leftChild, subID, v);
		else find = deleteFromGreenNode(r->rightChild, subID, v);
	}
	return find;
}

void BGTree::vectorToBitset(vector<int>& v, bitset<subs>*& b)
{
	b = new bitset<subs>;
	for (int i = 0; i < v.size(); i++)
		(*b)[v[i]] = 1;
	v.resize(0);
}
void BGTree::bitsetToVector(bitset<subs>*& b, vector<int>& v)
{
	_for(i, 0, subs) if ((*b)[i] == 1)
			v.push_back(i);
	delete b;
}

void BGTree::match_forward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	memcpy(counter, subPredicate, sizeof(subPredicate));
	for (auto&& pi : pub.pairs)
		match_forward_blueNode(roots[pi.att], pi.att, pi.value, subList);
	for (int i = 0; i < subs; i++)
		if (counter[i] == 0)
		{
#ifdef DEBUG
			numEffectivePredicate += subPredicate[i];
#endif
			++matchSubs;
			//cout << "BG-Tree matches sub: " << i << endl;
		}
}
void
BGTree::match_forward_blueNode(bluenode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList)
{
	if (r->leftBlueChild == nullptr)
	{ // 1.叶节点, 暴力匹配
#ifdef DEBUG
		numProcessTwoCmpNode++;
		numProcessTwoCmpPredicate += r->subids.size();
#endif
		for (auto&& i : r->subids)
		{
			for (auto&& pi : subList[i].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value && value <= pi.highValue)
						counter[i]--;
					break;
				}
		}
	}
	else if (r->mid == value)
	{ // 2.等于中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += r->midEqual.size() + r->lowGreenChild->subids.size();
#endif
		for (auto&& i : r->midEqual)
			counter[i]--;
		for (auto&& id : r->lowGreenChild->subids) // 这部分最初是多存了一份到midEqual里于是只需要 1 个 for 循环
			counter[id]--;
	}
	else if (value < r->mid)
	{ // 3.检索左蓝绿子节点
		match_forward_blueNode(r->leftBlueChild, att, value, subList);
		match_forward_lgreenNode(r->lowGreenChild, att, value, subList);
	}
	else
	{ // 4. value > r->mid 4.检索右蓝绿子节点
		match_forward_blueNode(r->rightBlueChild, att, value, subList);
		match_forward_hgreenNode(r->highGreenChild, att, value, subList);
	}
}
void
BGTree::match_forward_lgreenNode(greennode* const& l, const int& att, const int& value, const vector<IntervalSub>& subList)
{
	if (l->leftChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += l->subids.size();
#endif
		for (auto&& id : l->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value)
						counter[id]--;
					break;
				}
		} // return;
	}
	else if (value == l->mid)
	{ // 2. 等于中点且有左低绿子节点, 左低绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			counter[id]--;
	}
	else if (value < l->mid)
	{ // 3. 检索左低绿子节点, 右低绿子节点全部不匹配
		match_forward_lgreenNode(l->leftChild, att, value, subList);
	}
	else
	{ // value > l->mid  4. 检索右低绿子节点, 左低绿子节点全部匹配
		match_forward_lgreenNode(l->rightChild, att, value, subList);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			counter[id]--;
	}
}
void
BGTree::match_forward_hgreenNode(greennode* const& h, const int& att, const int& value, const vector<IntervalSub>& subList)
{
	if (h->rightChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.highValue >= value)
						counter[id]--;
					break;
				}
		} // return;
	}
	else if (value == h->mid)
	{ // 2.等于中点且有右高绿子节点, 右高绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			counter[id]--;
	}
	else if (value > h->mid)
	{ // 3. 检索右高绿子节点, 左高绿子节点全部不匹配
		match_forward_hgreenNode(h->rightChild, att, value, subList);
	}
	else
	{ // value < h->mid  4. 检索左绿左子节点, 右高绿子节点全部匹配
		match_forward_hgreenNode(h->leftChild, att, value, subList);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			counter[id]--;
	}
}

void BGTree::match_forward_CBOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		mB = nB[pi.att];
		attExist[pi.att] = true;
		match_forward_CBOMP_blueNode(roots[pi.att], pi.att, pi.value, subList, mB);
		gB = gB & mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB & nB[i];
	matchSubs = gB.count();
}
void BGTree::match_forward_CBOMP_blueNode(bluenode* const& r, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (r->leftBlueChild == nullptr)
	{ // 1.叶节点, 暴力匹配
#ifdef DEBUG
		numProcessTwoCmpNode++;
		numProcessTwoCmpPredicate += r->subids.size();
#endif
		for (auto&& id : r->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value && value <= pi.highValue)
						mB[id] = 1;
					break;
				}
		}
	}
	else if (r->mid == value)
	{ // 2.等于中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += r->midEqual.size() + r->lowGreenChild->subids.size();
#endif
		for (auto&& id : r->midEqual)
			mB[id] = 1;
		for (auto&& id : r->lowGreenChild->subids) // 这部分最初是多存了一份到midEqual里于是只需要 1 个 for 循环
			mB[id] = 1;
	}
	else if (value < r->mid)
	{ // 3.检索左蓝绿子节点
		match_forward_CBOMP_blueNode(r->leftBlueChild, att, value, subList, mB);
		match_forward_CBOMP_lgreenNode(r->lowGreenChild, att, value, subList, mB);
	}
	else
	{ // 4. value > r->mid 4.检索右蓝绿子节点
		match_forward_CBOMP_blueNode(r->rightBlueChild, att, value, subList, mB);
		match_forward_CBOMP_hgreenNode(r->highGreenChild, att, value, subList, mB);
	}
}
void BGTree::match_forward_CBOMP_lgreenNode(greennode* const& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (l->leftChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += l->subids.size();
#endif
		for (auto&& id : l->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value)
						mB[id] = 1;
					break;
				}
		} // return;
	}
	else if (value == l->mid)
	{ // 2. 等于中点且有左低绿子节点, 左低绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			mB[id] = 1;
	}
	else if (value < l->mid)
	{ // 3. 检索左低绿子节点, 右低绿子节点全部不匹配
		match_forward_CBOMP_lgreenNode(l->leftChild, att, value, subList, mB);
	}
	else
	{ // value > l->mid  4. 检索右低绿子节点, 左低绿子节点全部匹配
		match_forward_CBOMP_lgreenNode(l->rightChild, att, value, subList, mB);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			mB[id] = 1;
	}
}
void BGTree::match_forward_CBOMP_hgreenNode(greennode* const& h, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (h->rightChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.highValue >= value)
						mB[id] = 1;
					break;
				}
		} // return;
	}
	else if (value == h->mid)
	{ // 2.等于中点且有右高绿子节点, 右高绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			mB[id] = 1;
	}
	else if (value > h->mid)
	{ // 3. 检索右高绿子节点, 左高绿子节点全部不匹配
		match_forward_CBOMP_hgreenNode(h->rightChild, att, value, subList, mB);
	}
	else
	{ // value < h->mid  4. 检索左绿左子节点, 右高绿子节点全部匹配
		match_forward_CBOMP_hgreenNode(h->leftChild, att, value, subList, mB);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			mB[id] = 1;
	}
}

void BGTree::match_backward_DMFT_fBGTree_CBOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		mB = nnB[pi.att]; // Assume all having definitions is mismatching and marked
		attExist[pi.att] = true;
		match_backward_DMFT_fBGTree_CBOMP_blueNode(roots[pi.att], pi.att, pi.value, subList, mB);
		gB = gB | mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
}
void BGTree::match_backward_DMFT_fBGTree_CBOMP_blueNode(bluenode* const& r, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (r->leftBlueChild == nullptr)
	{ // 1.叶节点, 暴力匹配
#ifdef DEBUG
		numProcessTwoCmpNode++;
		numProcessTwoCmpPredicate += r->subids.size();
#endif
		for (auto&& id : r->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value && value <= pi.highValue)
						mB[id] = 0;
					break;
				}
		}
	}
	else if (r->mid == value)
	{ // 2.等于中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += r->midEqual.size() + r->lowGreenChild->subids.size();
#endif
		for (auto&& id : r->midEqual)
			mB[id] = 0;
		for (auto&& id : r->lowGreenChild->subids) // 这部分最初是多存了一份到midEqual里于是只需要 1 个 for 循环
			mB[id] = 0;
	}
	else if (value < r->mid)
	{ // 3.检索左蓝绿子节点
		match_backward_DMFT_fBGTree_CBOMP_blueNode(r->leftBlueChild, att, value, subList, mB);
		match_backward_DMFT_fBGTree_CBOMP_lgreenNode(r->lowGreenChild, att, value, subList, mB);
	}
	else
	{ // 4. value > r->mid 4.检索右蓝绿子节点
		match_backward_DMFT_fBGTree_CBOMP_blueNode(r->rightBlueChild, att, value, subList, mB);
		match_backward_DMFT_fBGTree_CBOMP_hgreenNode(r->highGreenChild, att, value, subList, mB);
	}
}
void BGTree::match_backward_DMFT_fBGTree_CBOMP_lgreenNode(greennode* const& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (l->leftChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += l->subids.size();
#endif
		for (auto&& id : l->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue <= value)
						mB[id] = 0;
					break;
				}
		} // return;
	}
	else if (value == l->mid)
	{ // 2. 等于中点且有左低绿子节点, 左低绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			mB[id] = 0;
	}
	else if (value < l->mid)
	{ // 3. 检索左低绿子节点, 右低绿子节点全部不匹配
		match_backward_DMFT_fBGTree_CBOMP_lgreenNode(l->leftChild, att, value, subList, mB);
	}
	else
	{ // value > l->mid  4. 检索右低绿子节点, 左低绿子节点全部匹配
		match_backward_DMFT_fBGTree_CBOMP_lgreenNode(l->rightChild, att, value, subList, mB);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
#endif
		for (auto&& id : l->leftChild->subids)
			mB[id] = 0;
	}
}
void BGTree::match_backward_DMFT_fBGTree_CBOMP_hgreenNode(greennode* const& h, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (h->rightChild == nullptr)
	{ // 1.叶子节点, 暴力匹配
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.highValue >= value)
						mB[id] = 0;
					break;
				}
		} // return;
	}
	else if (value == h->mid)
	{ // 2.等于中点且有右高绿子节点, 右高绿子节点即为匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			mB[id] = 0;
	}
	else if (value > h->mid)
	{ // 3. 检索右高绿子节点, 左高绿子节点全部不匹配
		match_backward_DMFT_fBGTree_CBOMP_hgreenNode(h->rightChild, att, value, subList, mB);
	}
	else
	{ // value < h->mid  4. 检索左绿左子节点, 右高绿子节点全部匹配
		match_backward_DMFT_fBGTree_CBOMP_hgreenNode(h->leftChild, att, value, subList, mB);
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
#endif
		for (auto&& id : h->rightChild->subids)
			mB[id] = 0;
	}
}

void BGTree::match_backward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // global bitset
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		attExist[pi.att] = true;
		match_backward_blueNode(roots[pi.att], pi.att, pi.value, subList, gB);
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
}
void
BGTree::match_backward_blueNode(bluenode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
	subs>& mB)
{
//	if(r->)
}
void
BGTree::match_backward_lgreenNode(greennode* const& l, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
	subs>& mB)
{

}
void
BGTree::match_backward_hgreenNode(greennode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
	subs>& mB)
{

}

void BGTree::match_forward_DMFT_bBGTree(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList)
{ // bBG-Tree-F

}

int BGTree::calMemory()
{
	double size = 0.0; // Byte
	size += sizeof(roots) + sizeof(bluenode*) * atts;
	for (int i = 0; i < atts; i++)
	{
		size += calBlueNodeMemory(roots[i]);
	}
	size += sizeof(int) * subs * 2; // subPredicate counter
	size += sizeof(nB) + sizeof(nB[0]) * atts; // nB or nnB
	size = size / 1024 / 1024; // MB
	return (int)size;
}

double BGTree::calBlueNodeMemory(const bluenode* const& r)
{
	double size = sizeof(bitset<subs>*) + (sizeof(vector<int>) + sizeof(lgreennode*) + sizeof(bluenode*)) * 2 +
				  sizeof(int) * 5;
	//cout << sizeof(bitset<subs>*) << " " << sizeof(vector<int>) << " " << sizeof(lgreennode*) << " " << sizeof(bluenode*)<<sizeof(bitset<subs>) << "\n";
	if (r->bst != nullptr)
		size += sizeof(bitset<subs>);
	else size += sizeof(int) * r->subids.size();

	size += sizeof(int) * r->midEqual.size();

	if (r->leftBlueChild)
		size += calBlueNodeMemory(r->leftBlueChild);
	if (r->rightBlueChild)
		size += calBlueNodeMemory(r->rightBlueChild);
	if (r->lowGreenChild)
		size += calGreenNodeMemory(r->lowGreenChild);
	if (r->highGreenChild)
		size += calGreenNodeMemory(r->highGreenChild);
	return size;
}

double BGTree::calGreenNodeMemory(greennode* const& r)
{
	double size = sizeof(bitset<subs>*) + sizeof(vector<int>) + sizeof(lgreennode*) * 2 + sizeof(int) * 5;
	if (r->bst != nullptr)
		size += sizeof(bitset<subs>);
	else size += sizeof(int) * r->subids.size();
	if (r->leftChild)
		size += calGreenNodeMemory(r->leftChild);
	if (r->rightChild)
		size += calGreenNodeMemory(r->rightChild);
	return size;
}

void BGTree::printBGTree()
{
	vector<vector<vector<tuple<int, int, int, int>>>> nodeInfo(atts, vector<vector<tuple<int, int, int, int>>>());
	int height = 0;
	_for(i, 0, atts)
	{
		height = 0;
		queue<bluenode*> bNextLevel;
		queue<greennode*> lgNextLevel;
		queue<greennode*> rgNextLevel;
		bNextLevel.push(roots[i]);
		while (!bNextLevel.empty() || !lgNextLevel.empty() || !rgNextLevel.empty())
		{ // for each level
			nodeInfo[i].push_back(vector<tuple<int, int, int, int>>());
			int b = bNextLevel.size(), l = lgNextLevel.size(), r = rgNextLevel.size();
			_for(j, 0, b)
			{
				bluenode* t = bNextLevel.front();
				bNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(1, t->l, t->h, t->subids.size()));
				if (t->leftBlueChild) bNextLevel.push(t->leftBlueChild);
				if (t->rightBlueChild) bNextLevel.push(t->rightBlueChild);
				if (t->lowGreenChild) lgNextLevel.push(t->lowGreenChild);
				if (t->highGreenChild) rgNextLevel.push(t->highGreenChild);
			}
			_for(j, 0, l)
			{
				greennode* t = lgNextLevel.front();
				lgNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(2, t->l, t->h, t->subids.size()));
				if (t->leftChild) lgNextLevel.push(t->leftChild);
				if (t->rightChild) lgNextLevel.push(t->rightChild);
			}
			_for(j, 0, r)
			{
				greennode* t = rgNextLevel.front();
				rgNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(3, t->l, t->h, t->subids.size()));
				if (t->leftChild) rgNextLevel.push(t->leftChild);
				if (t->rightChild) rgNextLevel.push(t->rightChild);
			}
			height++;
		}
	}
	_for(i, 0, 1)
	{
		_for(j, 0, nodeInfo[i].size())
		{
			cout << "a" << i << ", h" << j << ", nodeNum= " << nodeInfo[i][j].size() << "\n";
			int nodeId = -1, lv = -1, hv = -1, maxNumNodeSub = -1;
			sort(nodeInfo[i][j].begin(), nodeInfo[i][j].end(),
				[](const tuple<int, int, int, int>& a, const tuple<int, int, int, int>& b)
				{
				  return get<0>(a) == get<0>(b) ? get<3>(a) > get<3>(b) : get<0>(a) < get<0>(b);
				});
			_for(k, 0, nodeInfo[i][j].size())
			{
				cout << "(" << get<0>(nodeInfo[i][j][k]) << "," << get<1>(nodeInfo[i][j][k]) << ","
					 << get<2>(nodeInfo[i][j][k]) << ", " << get<3>(nodeInfo[i][j][k]) << "), ";
				if (k < nodeInfo[i][j].size() - 1 && (get<0>(nodeInfo[i][j][k]) != get<0>(nodeInfo[i][j][k + 1])))
					cout << "\n";
				if (maxNumNodeSub < get<3>(nodeInfo[i][j][k]))
				{
					nodeId = get<0>(nodeInfo[i][j][k]);
					lv = get<1>(nodeInfo[i][j][k]);
					hv = get<2>(nodeInfo[i][j][k]);
					maxNumNodeSub = get<3>(nodeInfo[i][j][k]);
				}
			}
			cout << "\nNodeID= " << nodeId << " [" << lv << ", " << hv << "], size= " << maxNumNodeSub << "\n\n";
		}
		cout << "\n";
	}
	cout << "ExactNode: " << numProcessExactNode << ", oneCmpNode: " << numProcessOneCmpNode << ", twoCmpNode: "
		 << numProcessTwoCmpNode \
 << "\nExactPredicate: " << numProcessExactPredicate << ", oneCmpPredicate: " << numProcessOneCmpPredicate
		 << ", twoCmpPredicate: " << numProcessTwoCmpPredicate \
 << "\nEffectivePredicate: " << numEffectivePredicate << ", effectiveRate: " << (double)numEffectivePredicate /
																				(double)(numProcessExactPredicate +
																						 numProcessOneCmpPredicate +
																						 numProcessTwoCmpPredicate + 1)
		 << ".\n\n";
}