//
// Created by swh on 2022/1/12.
//

#include "BGTree_d.h"

BGTree_d::BGTree_d()
{
	numSub = 0;
	numNode = 0;
	height = 0;
	maxNodeSize = MAXNodeSIZE;
	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);
	_for(i, 0, atts)
	{
		roots[i] = new bluenode_d(0, valDom - 1, ++numNode, 1, nullptr, nullptr, nullptr, nullptr, nullptr);
	}
	nB.resize(atts);
	nnB.resize(atts);

	cout << "ExpID = " << expID << ". BGTree_d: maxNodeSize = " << maxNodeSize << ", BoundaryNumSub = "
		 << BoundaryNumSub << "\n";
}

BGTree_d::~BGTree_d()
{
	_for(i, 0, atts)
	{
		releaseBlueNode(roots[i]);
	}
}

void BGTree_d::releaseBlueNode(bluenode_d*& r)
{
	if (r->lowGreenChild) releaseGreenNode(r->lowGreenChild);
	if (r->highGreenChild) releaseGreenNode(r->highGreenChild);
	if (r->leftBlueChild) releaseBlueNode(r->leftBlueChild);
	if (r->rightBlueChild) releaseBlueNode(r->rightBlueChild);
	if (r->bst) delete r->bst;
	delete r;
}

void BGTree_d::releaseGreenNode(lgreennode_d*& r)
{
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->bst) delete r->bst;
	delete r;
}

void BGTree_d::releaseGreenNode(hgreennode_d*& r)
{
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->bst) delete r->bst;
	delete r;
}

void BGTree_d::insert(IntervalSub sub, const vector<IntervalSub>& subList)
{
	vector<bool> attrExist(atts, false);
	subPredicate[sub.id] = sub.size;
	for (auto&& c : sub.constraints)
	{
		insertIntoBlueNode(roots[c.att], sub.id, c.lowValue,
			c.highValue, c.att, subList);
		attrExist[c.att] = true;
		nnB[c.att][sub.id] = 1;
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}

void BGTree_d::insertIntoBlueNode(bluenode_d*& r, const int& subID, const int& l, const int& h, const int& attrId,
	const vector<IntervalSub>& subList)
{
	r->subids.push_back(subID);
	if (r->subids.size() > BoundaryNumSub)
	{
		if (r->bst == nullptr)
		{
			r->bst = new bitset<subs>;
			for (auto&& id : r->subids)
				(*r->bst)[id] = 1;
		}
		(*r->bst)[subID] = 1;
	}
	if (r->mid == h) r->lMidv.push_back(subID);
	else if (r->mid + 1 == l) r->rMidv.push_back(subID);

	if (r->leftBlueChild != nullptr)
	{ // has child
		if (h <= r->mid)
		{
			insertIntoBlueNode(r->leftBlueChild, subID, l, h, attrId, subList);
		}
		else if (l > r->mid)
		{
			insertIntoBlueNode(r->rightBlueChild, subID, l, h, attrId, subList);
		}
		else
		{
			insertIntoGreenNode(r->lowGreenChild, subID, l, attrId, subList);
			insertIntoGreenNode(r->highGreenChild, subID, h, attrId, subList);
		}
	}
	else if (r->subids.size() > maxNodeSize)
	{ // no child and too big node
		r->leftBlueChild = new bluenode_d(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr, nullptr,
			nullptr);
		r->rightBlueChild = new bluenode_d(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr,
			nullptr, nullptr);
		r->lowGreenChild = new lgreennode_d(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->highGreenChild = new hgreennode_d(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto&& id : r->subids)
		{
			for (auto&& c : subList[id].constraints)
			{
				if (c.att == attrId)
				{
					if (c.highValue <= r->mid)
					{
						r->leftBlueChild->subids.push_back(id);
						// r->leftBlueChild->realBstPtr[id] = 1; // BoundaryNumSub>>maxNodeSize, so 节点需分裂时子节点不可能需要bitset存储
						if (r->leftBlueChild->mid == c.highValue) r->leftBlueChild->lMidv.push_back(id);
						else if (r->leftBlueChild->mid + 1 == c.lowValue) r->leftBlueChild->rMidv.push_back(id);
					}
					else if (c.lowValue > r->mid)
					{
						r->rightBlueChild->subids.push_back(id);
						// r->rightBlueChild->realBstPtr[id] = 1;
						if (r->rightBlueChild->mid == c.highValue) r->rightBlueChild->lMidv.push_back(id);
						else if (r->rightBlueChild->mid + 1 == c.lowValue) r->rightBlueChild->rMidv.push_back(id);
					}
					else
					{ // l<=mid<mid+1<=h
						r->lowGreenChild->subids.push_back(id);
						// r->lowGreenChild->realBstPtr[id] = 1;
						if (r->lowGreenChild->mid + 1 == c.lowValue)
							r->lowGreenChild->midv.push_back(id);
						r->highGreenChild->subids.push_back(id);
						// r->highGreenChild->realBstPtr[id] = 1;
						if (r->highGreenChild->mid - 1 == c.highValue)
							r->highGreenChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}

void BGTree_d::insertIntoGreenNode(lgreennode_d*& r, const int& subID, const int& l, const int& attrId,
	const vector<IntervalSub>& subList)
{
	r->subids.push_back(subID);
	if (r->subids.size() > BoundaryNumSub)
	{
		if (r->bst == nullptr)
		{
			r->bst = new bitset<subs>;
			for (auto&& id : r->subids)
				(*r->bst)[id] = 1;
		}
		(*r->bst)[subID] = 1;
	}
	if (r->mid + 1 == l)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr)
	{  // 有左子必有右子节点
		if (l <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, l, attrId, subList);
		else insertIntoGreenNode(r->rightChild, subID, l, attrId, subList);
	}
	else if (r->subids.size() > maxNodeSize)
	{
		r->leftChild = new lgreennode_d(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new lgreennode_d(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto&& id : r->subids)
		{
			for (auto&& c : subList[id].constraints)
			{
				if (c.att == attrId)
				{
					if (c.lowValue <= r->mid)
					{
						r->leftChild->subids.push_back(id);
						// r->leftChild->realBstPtr[id] = 1;  // BoundaryNumSub>>maxNodeSize, so 节点需分裂时子节点不可能需要bitset存储
						if (r->leftChild->mid + 1 == c.lowValue)
							r->leftChild->midv.push_back(id);
					}
					else
					{
						r->rightChild->subids.push_back(id);
						// r->rightChild->realBstPtr[id] = 1;
						if (r->rightChild->mid + 1 == c.lowValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}

void BGTree_d::insertIntoGreenNode(hgreennode_d*& r, const int& subID, const int& h, const int& attrId,
	const vector<IntervalSub>& subList)
{
	r->subids.push_back(subID);
	if (r->subids.size() > BoundaryNumSub)
	{
		if (r->bst == nullptr)
		{
			r->bst = new bitset<subs>;
			for (auto&& id : r->subids)
				(*r->bst)[id] = 1;
		}
		(*r->bst)[subID] = 1;
	}
	if (r->mid - 1 == h)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr)
	{  // 有左子必有右子节点
		if (h < r->mid)
			insertIntoGreenNode(r->leftChild, subID, h, attrId, subList);
		else insertIntoGreenNode(r->rightChild, subID, h, attrId, subList);
	}
	else if (r->subids.size() > maxNodeSize)
	{
		r->leftChild = new hgreennode_d(r->l, r->mid - 1, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new hgreennode_d(r->mid, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto&& id : r->subids)
		{
			for (auto&& c : subList[id].constraints)
			{
				if (c.att == attrId)
				{
					if (c.highValue < r->mid)
					{
						r->leftChild->subids.push_back(id);
						// r->leftChild->realBstPtr[id] = 1;
						if (r->leftChild->mid - 1 == c.highValue)
							r->leftChild->midv.push_back(id);
					}
					else
					{
						r->rightChild->subids.push_back(id);
						// r->rightChild->realBstPtr[id] = 1;
						if (r->rightChild->mid - 1 == c.highValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}

bool BGTree_d::deleteSubscription(IntervalSub sub)
{
	bool find = true;
	for (auto&& c : sub.constraints)
	{
		find &= deleteFromBlueNode(roots[c.att], sub.id, c.lowValue,
			c.highValue);
	}
	subPredicate[sub.id] = 0;
	numSub--;
	return find;
}

bool BGTree_d::deleteFromBlueNode(bluenode_d*& r, const int& subID, const int& l, const int& h)
{
	bool find = false;
	if (r->bst != nullptr)
	{
		(*(r->bst))[subID] = 0;
		if (r->subids.size() < BoundaryNumSub)
			delete r->bst;
	}
	for (vector<int32_t>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++)
	{
		if (*it == subID)
		{
			r->subids.erase(it);
			find = true;
			break;
		}
	}
	if (r->mid == h)
	{
		find = false;
		for (vector<int32_t>::const_iterator it = r->lMidv.cbegin(); it != r->lMidv.cend(); it++)
		{
			if (*it == subID)
			{
				r->lMidv.erase(it);
				find = true;
				break;
			}
		}
	}
	else if (r->mid + 1 == l)
	{
		find = false;
		for (vector<int32_t>::const_iterator it = r->rMidv.cbegin(); it != r->rMidv.cend(); it++)
		{
			if (*it == subID)
			{
				r->rMidv.erase(it);
				find = true;
				break;
			}
		}
	}

	if (r->leftBlueChild != nullptr)
	{ // has child
		if (h <= r->mid)
		{
			find &= deleteFromBlueNode(r->leftBlueChild, subID, l, h);
		}
		else if (l > r->mid)
		{
			find &= deleteFromBlueNode(r->rightBlueChild, subID, l, h);
		}
		else
		{
			find &= deleteFromGreenNode(r->lowGreenChild, subID, l);
			find &= deleteFromGreenNode(r->highGreenChild, subID, h);
		}

		if (r->subids.size() < maxNodeSize)
		{ // has child and too small node
			delete r->leftBlueChild, r->rightBlueChild, r->lowGreenChild, r->highGreenChild;
		}
	}
	return find;
}

bool BGTree_d::deleteFromGreenNode(lgreennode_d*& r, const int& subID, const int& l)
{
	bool find = false;
	if (r->bst != nullptr)
	{
		(*(r->bst))[subID] = 0;
		if (r->subids.size() < BoundaryNumSub)
			delete r->bst;
	}
	for (vector<int32_t>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++)
	{
		if (*it == subID)
		{
			r->subids.erase(it);
			find = true;
			break;
		}
	}
	if (r->mid + 1 == l)
	{
		find = false;
		for (vector<int32_t>::const_iterator it = r->midv.cbegin(); it != r->midv.cend(); it++)
		{
			if (*it == subID)
			{
				r->midv.erase(it);
				find = true;
				break;
			}
		}
	}
	if (r->leftChild != nullptr)
	{ // has child
		if (l <= r->mid)
		{
			find &= deleteFromGreenNode(r->leftChild, subID, l);
		}
		else
		{
			find &= deleteFromGreenNode(r->rightChild, subID, l);
		}
		if (r->subids.size() < maxNodeSize)
		{ // has child and too small node
			delete r->leftChild, r->rightChild, r->bst;
		}
	}
	return find;
}

bool BGTree_d::deleteFromGreenNode(hgreennode_d*& r, const int& subID, const int& h)
{
	bool find = false;
	if (r->bst != nullptr)
	{
		(*(r->bst))[subID] = 0;
		if (r->subids.size() < BoundaryNumSub)
			delete r->bst;
	}
	for (vector<int32_t>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++)
	{
		if (*it == subID)
		{
			r->subids.erase(it);
			find = true;
			break;
		}
	}
	if (r->mid - 1 == h)
	{
		find = false;
		for (vector<int32_t>::const_iterator it = r->midv.cbegin(); it != r->midv.cend(); it++)
		{
			if (*it == subID)
			{
				r->midv.erase(it);
				find = true;
				break;
			}
		}
	}
	if (r->leftChild != nullptr)
	{ // has child
		if (h < r->mid)
		{
			find &= deleteFromGreenNode(r->leftChild, subID, h);
		}
		else
		{
			find &= deleteFromGreenNode(r->rightChild, subID, h);
		}

		if (r->subids.size() < maxNodeSize)
		{ // has child and too small node
			delete r->leftChild, r->rightChild;
		}
	}
	return find;
}

void BGTree_d::forward_match_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	memcpy(counter, subPredicate, sizeof(subPredicate));
	for (auto&& pi : pub.pairs)
		forward_match_blueNode(roots[pi.att], pi.att, pi.value, subList);
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
BGTree_d::forward_match_blueNode(bluenode_d*& r, const int& att, const int& value, const vector<IntervalSub>& subList)
{
	if (r->leftBlueChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
						counter[id]--;
					break;
				}
		}
	}
	else if (value < r->mid)
	{ // 2. 小于左中点, 检索两个子节点
		forward_match_blueNode(r->leftBlueChild, att, value, subList);
		forward_match_lgreenNode(r->lowGreenChild, att, value, subList);
	}
	else if (value > r->mid + 1)
	{ // 5. value > r->mid + 1 大于右中节点, 检索两个子节点
		forward_match_blueNode(r->rightBlueChild, att, value, subList);
		forward_match_hgreenNode(r->highGreenChild, att, value, subList);
	}
	else
	{ // 3、4. 等于左中点或右中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		if (value == r->mid)
			numProcessExactPredicate += r->lMidv.size() + r->lowGreenChild->subids.size();
		else numProcessExactPredicate += r->rMidv.size() + r->highGreenChild->subids.size();
#endif
		if (value == r->mid)
			for (auto&& id : r->lMidv)
				counter[id]--;
		else
			for (auto&& id : r->rMidv)
				counter[id]--;
		for (auto&& id : r->lowGreenChild->subids)
			counter[id]--;
	}
}

void BGTree_d::forward_match_lgreenNode(lgreennode_d*& l, const int& att, const int& value,
	const vector<IntervalSub>& subList)
{
	if (l->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
		}
	}
	else if (value < l->mid)
	{ // 2. 小于左中点, 检索左子节点
		forward_match_lgreenNode(l->leftChild, att, value, subList);
	}
	else
	{ // 3、4、5. 左子节点完全匹配
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
		if (value == l->mid) hit++;
		if (value == l->mid + 1)
		{
			hit++;
			numProcessExactPredicate += l->midv.size();
		}
#endif
		for (auto&& id : l->leftChild->subids)
			counter[id]--;
		if (value == l->mid) return;
		else if (value == l->mid + 1)
			for (auto&& id : l->midv)
				counter[id]--;
		else //  检索右子节点
			forward_match_lgreenNode(l->rightChild, att, value, subList);
	}
}

void BGTree_d::forward_match_hgreenNode(hgreennode_d*& h, const int& att, const int& value,
	const vector<IntervalSub>& subList)
{
	if (h->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (value <= pi.highValue)
						counter[id]--;
					break;
				}
		}
	}
	else if (value > h->mid)
	{ // 2. 大于右中点, 检索右子节点
		forward_match_hgreenNode(h->rightChild, att, value, subList);
	}
	else
	{ // 3、4、5. 小于左中点, 右子节点完全匹配, 检索左子节点
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
		if (value == h->mid) hit++;
		else if (value == h->mid - 1)
		{
			hit++;
			numProcessExactPredicate += h->midv.size();
		}
#endif
		for (auto&& id : h->rightChild->subids)
			counter[id]--;
		if (value == h->mid) return;
		else if (value == h->mid - 1)
			for (auto&& id : h->midv)
				counter[id]--;
		else
			forward_match_hgreenNode(h->leftChild, att, value, subList);
	}
}

void BGTree_d::forward_match_C_BOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		mB = nB[pi.att];
		attExist[pi.att] = true;
		forward_match_blueNode_C_BOMP(roots[pi.att], pi.att, pi.value, subList, mB);
		gB &= mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB &= nB[i];
	matchSubs = gB.count();
}

void
BGTree_d::forward_match_blueNode_C_BOMP(bluenode_d*& r, const int& att, const int& value, const vector<IntervalSub>& subList,
	bitset<subs>& mB)
{
	if (r->leftBlueChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
	else if (value < r->mid)
	{ // 2. 小于左中点, 检索两个子节点
		forward_match_blueNode_C_BOMP(r->leftBlueChild, att, value, subList, mB);
		forward_match_lgreenNode_C_BOMP(r->lowGreenChild, att, value, subList, mB);
	}
	else if (value > r->mid + 1)
	{ // 5. value > r->mid + 1 大于右中节点, 检索两个子节点
		forward_match_blueNode_C_BOMP(r->rightBlueChild, att, value, subList, mB);
		forward_match_hgreenNode_C_BOMP(r->highGreenChild, att, value, subList, mB);
	}
	else
	{ // 3、4. 等于左中点或右中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		if (value == r->mid)
			numProcessExactPredicate += r->lMidv.size() + r->lowGreenChild->subids.size();
		else numProcessExactPredicate += r->rMidv.size() + r->highGreenChild->subids.size();
		if (r->lowGreenChild->bst) numBitsetOperation++;
#endif
		if (value == r->mid)
			for (auto&& id : r->lMidv)
				mB[id] = 1;
		else
			for (auto&& id : r->rMidv)
				mB[id] = 1;
		if (r->lowGreenChild->bst)
		{
			mB |= *r->lowGreenChild->bst;
		}
		else
		{
			for (auto&& id : r->lowGreenChild->subids)
				mB[id] = 1;
		}
	}
}

void BGTree_d::forward_match_lgreenNode_C_BOMP(lgreennode_d*& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (l->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
		}
	}
	else if (value < l->mid)
	{ // 2. 小于左中点, 检索左子节点
		forward_match_lgreenNode_C_BOMP(l->leftChild, att, value, subList, mB);
	}
	else
	{ // 3、4、5. 左子节点完全匹配
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
		if (value == l->mid) hit++;
		if (value == l->mid + 1)
		{
			hit++;
			numProcessExactPredicate += l->midv.size();
		}
		if (l->leftChild->bst) numBitsetOperation++;
#endif
		if (l->leftChild->bst)
		{
			mB |= *l->leftChild->bst;
		}
		else
		{
			for (auto&& id : l->leftChild->subids)
				mB[id] = 1;
		}
		if (value == l->mid) return;
		else if (value == l->mid + 1)
			for (auto&& id : l->midv)
				mB[id] = 1;
		else //  value > l->mid + 1 检索右子节点
			forward_match_lgreenNode_C_BOMP(l->rightChild, att, value, subList, mB);
	}
}

void BGTree_d::forward_match_hgreenNode_C_BOMP(hgreennode_d*& h, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (h->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (value <= pi.highValue)
						mB[id] = 1;
					break;
				}
		}
	}
	else if (value > h->mid)
	{ // 2. 大于右中点, 检索右子节点
		forward_match_hgreenNode_C_BOMP(h->rightChild, att, value, subList, mB);
	}
	else
	{ // 3、4、5. 小于左中点, 右子节点完全匹配, 检索左子节点
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
		if (value == h->mid) hit++;
		else if (value == h->mid - 1)
		{
			hit++;
			numProcessExactPredicate += h->midv.size();
		}
		if (h->rightChild->bst) numBitsetOperation++;
#endif
		if (h->rightChild->bst)
		{
			mB |= *h->rightChild->bst;
		}
		else
		{
			for (auto&& id : h->rightChild->subids)
				mB[id] = 1;
		}
		if (value == h->mid) return;
		else if (value == h->mid - 1)
			for (auto&& id : h->midv)
				mB[id] = 1;
		else
			forward_match_hgreenNode_C_BOMP(h->leftChild, att, value, subList, mB);
	}
}

void BGTree_d::backward_match_DMFT_fBGTree_d_CBOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB, mB; // global bitset
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		mB = nnB[pi.att]; // Assume all having definitions is mismatching and marked
		attExist[pi.att] = true;
		backward_match_DMFT_fBGTree_d_CBOMP_blueNode(roots[pi.att], pi.att, pi.value, subList, mB);
		gB |= mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB |= nnB[i];
	matchSubs = numSub - gB.count();
}

void
BGTree_d::backward_match_DMFT_fBGTree_d_CBOMP_blueNode(bluenode_d*& r, const int& att, const int& value, const vector<
	IntervalSub>& subList, bitset<subs>& mB)
{
	if (r->leftBlueChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
	else if (value < r->mid)
	{ // 2. 小于左中点, 检索两个子节点
		backward_match_DMFT_fBGTree_d_CBOMP_blueNode(r->leftBlueChild, att, value, subList, mB);
		backward_match_DMFT_fBGTree_d_CBOMP_lgreenNode(r->lowGreenChild, att, value, subList, mB);
	}
	else if (value > r->mid + 1)
	{ // 5. value > r->mid + 1 大于右中节点, 检索两个子节点
		backward_match_DMFT_fBGTree_d_CBOMP_blueNode(r->rightBlueChild, att, value, subList, mB);
		backward_match_DMFT_fBGTree_d_CBOMP_hgreenNode(r->highGreenChild, att, value, subList, mB);
	}
	else
	{ // 3、4. 等于左中点或右中点, 直接得到匹配结果
#ifdef DEBUG
		hit++;
		numProcessExactNode++;
		if (value == r->mid)
			numProcessExactPredicate += r->lMidv.size() + r->lowGreenChild->subids.size();
		else numProcessExactPredicate += r->rMidv.size() + r->highGreenChild->subids.size();
		if (r->lowGreenChild->bst) numBitsetOperation++;
#endif
		if (value == r->mid)
			for (auto&& id : r->lMidv)
				mB[id] = 0;
		else
			for (auto&& id : r->rMidv)
				mB[id] = 0;
		if (r->lowGreenChild->bst)
		{
			mB &= ~(*r->lowGreenChild->bst);
		}
		else
		{
			for (auto&& id : r->lowGreenChild->subids)
				mB[id] = 0;
		}
	}
}

void BGTree_d::backward_match_DMFT_fBGTree_d_CBOMP_lgreenNode(lgreennode_d*& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (l->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
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
		}
	}
	else if (value < l->mid)
	{ // 2. 小于左中点, 检索左子节点
		backward_match_DMFT_fBGTree_d_CBOMP_lgreenNode(l->leftChild, att, value, subList, mB);
	}
	else
	{ // 3、4、5. 左子节点完全匹配
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->leftChild->subids.size();
		if (value == l->mid) hit++;
		else if (value == l->mid + 1)
		{
			hit++;
			numProcessExactPredicate += l->midv.size();
		}
		if (l->leftChild->bst) numBitsetOperation++;
#endif
		if (l->leftChild->bst)
		{
			mB &= ~(*l->leftChild->bst);
		}
		else
		{
			for (auto&& id : l->leftChild->subids)
				mB[id] = 0;
		}
		if (value == l->mid) return;
		else if (value == l->mid + 1)
			for (auto&& id : l->midv)
				mB[id] = 0;
		else //  value > l->mid + 1 检索右子节点
			backward_match_DMFT_fBGTree_d_CBOMP_lgreenNode(l->rightChild, att, value, subList, mB);
	}
}

void BGTree_d::backward_match_DMFT_fBGTree_d_CBOMP_hgreenNode(hgreennode_d*& h, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB)
{
	if (h->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (value <= pi.highValue)
						mB[id] = 0;
					break;
				}
		}
	}
	else if (value > h->mid)
	{ // 2. 大于右中点, 检索右子节点
		backward_match_DMFT_fBGTree_d_CBOMP_hgreenNode(h->rightChild, att, value, subList, mB);
	}
	else
	{ // 3、4、5. 小于左中点, 右子节点完全匹配, 检索左子节点
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->rightChild->subids.size();
		if (value == h->mid) hit++;
		else if (value == h->mid - 1)
		{
			hit++;
			numProcessExactPredicate += h->midv.size();
		}
		if (h->rightChild->bst) numBitsetOperation++;
#endif
		if (h->rightChild->bst)
		{
			mB &= ~(*h->rightChild->bst);
		}
		else
		{
			for (auto&& id : h->rightChild->subids)
				mB[id] = 0;
		}
		if (value == h->mid) return;
		else if (value == h->mid - 1)
			for (auto&& id : h->midv)
				mB[id] = 0;
		else
			backward_match_DMFT_fBGTree_d_CBOMP_hgreenNode(h->leftChild, att, value, subList, mB);
	}
}

void BGTree_d::backward_match_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
	bitset<subs> gB; // global bitset
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		attExist[pi.att] = true;
		backward_match_blueNode_native(roots[pi.att], pi.att, pi.value, subList, gB);
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
#ifdef DEBUG
	for (int i = 0; i < subs; i++)
		if (gB[i] == 0)
			numEffectivePredicate += subPredicate[i];
#endif
}

void BGTree_d::backward_match_blueNode_native(bluenode_d*& r, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& gB)
{
	if (r->leftBlueChild == nullptr)
	{
#ifdef DEBUG
		numProcessTwoCmpNode++;
		numProcessTwoCmpPredicate += r->subids.size();
#endif
		for (auto&& i : r->subids)
		{
			for (auto&& pi : subList[i].constraints)
				if (att == pi.att)
				{
					if (value < pi.lowValue || pi.highValue < value)
						gB[i] = 1;
					break;
				}
		}
	}
	else if (value <= r->mid)
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += r->rightBlueChild->subids.size();
		if (r->rightBlueChild->bst) numBitsetOperation++;
#endif
		if (r->rightBlueChild->bst == nullptr)
		{
			for (auto&& id : r->rightBlueChild->subids)
				gB[id] = 1;
		}
		else
		{
			gB = gB | *r->rightBlueChild->bst;
		}
		if (value != r->mid || r->leftBlueChild->bst == nullptr)
		{
			backward_match_blueNode_native(r->leftBlueChild, att, value, subList, gB);
			backward_match_lgreenNode_native(r->lowGreenChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			hit++;
			numProcessExactNode++;
			numProcessExactPredicate += r->leftBlueChild->subids.size() - r->lMidv.size();
			numBitsetOperation++;
#endif
			bitset<subs> bst_c = *r->leftBlueChild->bst;
			for (auto&& id : r->lMidv)
				bst_c[id] = 0;
			gB = gB | bst_c;
		}
	}
	else // value>mid
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += r->leftBlueChild->subids.size();
		if (r->leftBlueChild->bst) numBitsetOperation++;
#endif
		if (r->leftBlueChild->bst == nullptr)
		{
			for (auto&& id : r->leftBlueChild->subids)
				gB[id] = 1;
		}
		else
		{
			gB = gB | *r->leftBlueChild->bst;
		}
		if (value != r->mid + 1 || r->rightBlueChild->bst == nullptr)
		{
			backward_match_blueNode_native(r->rightBlueChild, att, value, subList, gB);
			backward_match_hgreenNode_native(r->highGreenChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			hit++;
			numProcessExactNode++;
			numProcessExactPredicate += r->rightBlueChild->subids.size() - r->rMidv.size();
			numBitsetOperation++;
#endif
			bitset<subs> bst_c = *r->rightBlueChild->bst;
			for (auto&& id : r->rMidv)
				bst_c[id] = 0;
			gB = gB | bst_c;
		}
	}
}

void
BGTree_d::backward_match_lgreenNode_native(lgreennode_d*& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& gB)
{
	if (l->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += l->subids.size();
#endif
		for (auto&& id : l->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue > value)
						gB[id] = 1;
					break;
				}
		}
	}
	else if (value <= l->mid)
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->rightChild->subids.size();
		if (value == l->mid) hit++;
		if (l->rightChild->bst) numBitsetOperation++;
#endif
		if (l->rightChild->bst == nullptr)
		{
			for (auto&& id : l->rightChild->subids)
				gB[id] = 1;
		}
		else
		{
			gB = gB | *l->rightChild->bst;
		}
		if (value < l->mid)
			backward_match_lgreenNode_native(l->leftChild, att, value, subList, gB);
	}
	else
	{ // l->mid < value
		if (l->mid + 1 != value || l->rightChild->bst == nullptr)
		{
			backward_match_lgreenNode_native(l->rightChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			numProcessExactNode++;
			numProcessExactPredicate += l->rightChild->subids.size() - l->midv.size();
			numBitsetOperation++;
			hit++;
#endif
			bitset<subs> bst_c = *l->rightChild->bst;
			for (auto&& id : l->midv)
				bst_c[id] = 0;
			gB = gB | bst_c;
		}
	}
}

void
BGTree_d::backward_match_hgreenNode_native(hgreennode_d*& h, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& gB)
{
	if (h->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.highValue < value)
						gB[id] = 1;
					break;
				}
		}
	}
	else if (value >= h->mid)
	{ // 2. fall into right subInterval
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->leftChild->subids.size();
		if (value == h->mid) hit++;
		if (h->leftChild->bst) numBitsetOperation++;
#endif
		if (h->leftChild->bst == nullptr)
		{ // leftChild must be all-unmatch
			for (auto&& id : h->leftChild->subids)
				gB[id] = 1;
		}
		else
		{
			gB = gB | *h->leftChild->bst;
		}
		if (value > h->mid)
			backward_match_hgreenNode_native(h->rightChild, att, value, subList, gB);
	}
	else
	{ // value < h->mid
		if (h->mid != value + 1 || h->leftChild->bst == nullptr)
		{
			backward_match_hgreenNode_native(h->leftChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			numProcessExactNode++;
			numProcessExactPredicate += h->leftChild->subids.size() - h->midv.size();
			numBitsetOperation++;
			hit++;
#endif
			bitset<subs> bst_c = *h->leftChild->bst;
			for (auto&& id : h->midv)
				bst_c[id] = 0;
			gB = gB | bst_c;
		}
	}
}

void
BGTree_d::forward_match_DMFT_bBGTree_d(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) // bBG-Tree_d-F
{  // bBG-Tree_d-F
	bitset<subs> gB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs)
	{
		attExist[pi.att] = true;
		forward_match_DMFT_bBGTree_d_blueNode(roots[pi.att], pi.att, pi.value, subList, gB);
	}
	_for(i, 0, atts) if (!attExist[i])
			gB &= nB[i];
	matchSubs = gB.count();
}

void
BGTree_d::forward_match_DMFT_bBGTree_d_blueNode(const bluenode_d* const& r, const int& att, const int& value, const vector<
	IntervalSub>& subList, bitset<subs>& gB)
{
	if (r->leftBlueChild == nullptr)
	{
#ifdef DEBUG
		numProcessTwoCmpNode++;
		numProcessTwoCmpPredicate += r->subids.size();
#endif
		for (auto&& i : r->subids)
		{
			for (auto&& pi : subList[i].constraints)
				if (att == pi.att)
				{
					if (value < pi.lowValue || pi.highValue < value)
						gB[i] = 0;
					break;
				}
		}
	}
	else if (value <= r->mid)
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += r->rightBlueChild->subids.size();
		if (r->rightBlueChild->bst) numBitsetOperation++;
#endif
		if (r->rightBlueChild->bst == nullptr)
		{
			for (auto&& id : r->rightBlueChild->subids)
				gB[id] = 0;
		}
		else
		{
			gB &= ~*r->rightBlueChild->bst;
		}
		if (value != r->mid || r->leftBlueChild->bst == nullptr)
		{
			forward_match_DMFT_bBGTree_d_blueNode(r->leftBlueChild, att, value, subList, gB);
			forward_match_DMFT_bBGTree_d_lgreenNode(r->lowGreenChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			hit++;
			numProcessExactNode++;
			numProcessExactPredicate += r->leftBlueChild->subids.size() - r->lMidv.size();
			numBitsetOperation++;
#endif
			bitset<subs> bst_c = *r->leftBlueChild->bst;
			for (auto&& id : r->lMidv)
				bst_c[id] = 0;
			gB &= ~bst_c;
		}
	}
	else // value>mid
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += r->leftBlueChild->subids.size();
		if (r->leftBlueChild->bst) numBitsetOperation++;
#endif
		if (r->leftBlueChild->bst == nullptr)
		{
			for (auto&& id : r->leftBlueChild->subids)
				gB[id] = 0;
		}
		else
		{
			gB &= ~*r->leftBlueChild->bst;
		}
		if (value != r->mid + 1 || r->rightBlueChild->bst == nullptr)
		{
			forward_match_DMFT_bBGTree_d_blueNode(r->rightBlueChild, att, value, subList, gB);
			forward_match_DMFT_bBGTree_d_hgreenNode(r->highGreenChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			hit++;
			numProcessExactNode++;
			numProcessExactPredicate += r->rightBlueChild->subids.size() - r->rMidv.size();
			numBitsetOperation++;
#endif
			bitset<subs> bst_c = *r->rightBlueChild->bst;
			for (auto&& id : r->rMidv)
				bst_c[id] = 0;
			gB &= ~bst_c;
		}
	}
}

void
BGTree_d::forward_match_DMFT_bBGTree_d_lgreenNode(const lgreennode_d* const& l, const int& att, const int& value, const vector<
	IntervalSub>& subList, bitset<subs>& gB)
{
	if (l->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += l->subids.size();
#endif
		for (auto&& id : l->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.lowValue > value)
						gB[id] = 0;
					break;
				}
		}
	}
	else if (value <= l->mid)
	{
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += l->rightChild->subids.size();
		if (value == l->mid) hit++;
		if (l->rightChild->bst) numBitsetOperation++;
#endif
		if (l->rightChild->bst)
		{
			gB &= ~*l->rightChild->bst;
		}
		else
		{
			for (auto&& id : l->rightChild->subids)
				gB[id] = 0;
		}
		if (value < l->mid)
			forward_match_DMFT_bBGTree_d_lgreenNode(l->leftChild, att, value, subList, gB);
	}
	else
	{ // l->mid < value
		if (l->mid + 1 != value || l->rightChild->bst == nullptr)
		{
			forward_match_DMFT_bBGTree_d_lgreenNode(l->rightChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			hit++;
			numProcessExactNode++;
			numProcessExactPredicate += l->rightChild->subids.size() - l->midv.size();
			numBitsetOperation++;
#endif
			bitset<subs> bst_c = *l->rightChild->bst;
			for (auto&& id : l->midv)
				bst_c[id] = 0;
			gB &= ~bst_c;
		}
	}
}

void
BGTree_d::forward_match_DMFT_bBGTree_d_hgreenNode(const hgreennode_d* const& h, const int& att, const int& value, const vector<
	IntervalSub>& subList, bitset<subs>& gB)
{
	if (h->leftChild == nullptr)
	{ // 1. 叶子节点暴力处理
#ifdef DEBUG
		numProcessOneCmpNode++;
		numProcessOneCmpPredicate += h->subids.size();
#endif
		for (auto&& id : h->subids)
		{
			for (auto&& pi : subList[id].constraints)
				if (att == pi.att)
				{
					if (pi.highValue < value)
						gB[id] = 0;
					break;
				}
		}
	}
	else if (value >= h->mid)
	{ // 2. fall into right subInterval
#ifdef DEBUG
		numProcessExactNode++;
		numProcessExactPredicate += h->leftChild->subids.size();
		if (value == h->mid) hit++;
		if (h->leftChild->bst) numBitsetOperation++;
#endif
		if (h->leftChild->bst) // leftChild must be all-unmatch
		{
			gB &= ~*h->leftChild->bst;
		}
		else
		{
			for (auto&& id : h->leftChild->subids)
				gB[id] = 0;
		}
		if (value > h->mid)
			forward_match_DMFT_bBGTree_d_hgreenNode(h->rightChild, att, value, subList, gB);
	}
	else
	{ // value < h->mid
		if (h->mid != value + 1 || h->leftChild->bst == nullptr)
		{
			forward_match_DMFT_bBGTree_d_hgreenNode(h->leftChild, att, value, subList, gB);
		}
		else
		{
#ifdef DEBUG
			numProcessExactNode++;
			numProcessExactPredicate += h->leftChild->subids.size() - h->midv.size();
			numBitsetOperation++;
			hit++;
#endif
			bitset<subs> bst_c = *h->leftChild->bst;
			for (auto&& id : h->midv)
				bst_c[id] = 0;
			gB &= ~bst_c;
		}
	}
}

int BGTree_d::calMemory()
{
	double size = 0.0; // Byte
	size += sizeof(roots) + sizeof(bluenode_d*) * atts;
	bluenode_d* b;
	lgreennode_d* l;
	hgreennode_d* h;
	for (int i = 0; i < atts; i++)
	{
		queue<bluenode_d*> bq;
		queue<lgreennode_d*> lq;
		queue<hgreennode_d*> hq;
		bq.push(roots[i]);
		while (!bq.empty() || !lq.empty() || !hq.empty())
		{
			if (!bq.empty())
			{
				b = bq.front();
				bq.pop();
				size += sizeof(b) + (5 + b->subids.size() + b->lMidv.size() + b->rMidv.size()) * sizeof(int) +
						sizeof(vector<int>) * 3;
				size +=
					sizeof(lgreennode_d*) + sizeof(hgreennode_d*) + sizeof(bluenode_d*) * 2 + sizeof(bitset<subs>*);
				if (b->leftBlueChild)
				{
					bq.push(b->leftBlueChild);
					bq.push(b->rightBlueChild);
					lq.push(b->lowGreenChild);
					hq.push(b->highGreenChild);
				}
				if (b->bst) size += sizeof(bitset<subs>);
			}
			if (!lq.empty())
			{
				l = lq.front();
				lq.pop();
				size += sizeof(l) + (5 + l->subids.size() + l->midv.size()) * sizeof(int) + sizeof(vector<int>) * 2 +
						sizeof(bitset<subs>*);
				if (l->leftChild != nullptr)
				{
					size += sizeof(lgreennode_d*) * 2;
					lq.emplace(l->leftChild);
					lq.emplace(l->rightChild);
				}
				if (l->bst)size += sizeof(bitset<subs>);
			}
			if (!hq.empty())
			{
				h = hq.front();
				hq.pop();
				size += sizeof(h) + (5 + h->subids.size() + h->midv.size()) * sizeof(int) + sizeof(vector<int>) * 2 +
						sizeof(bitset<subs>*);
				if (h->leftChild != nullptr)
				{
					size += sizeof(hgreennode_d*) * 2;
					hq.emplace(h->leftChild);
					hq.emplace(h->rightChild);
				}
				if (h->bst)size += sizeof(bitset<subs>);
			}
		}
	}
	size += sizeof(int) * subs * 2; // subPredicate counter
	size += sizeof(nB) + sizeof(nB[0]) * atts; // nB or nnB
	size = size / 1024 / 1024; // MB
	return (int32_t)size;
}

void BGTree_d::printBGTree()
{
	vector<vector<vector<tuple<int, int, int, int>>>> nodeInfo(atts, vector<vector<tuple<int, int, int, int>>>());
	int height = 0;
	_for(i, 0, atts)
	{
		height = 0;
		queue<bluenode_d*> bNextLevel;
		queue<lgreennode_d*> lgNextLevel;
		queue<hgreennode_d*> hgNextLevel;
		bNextLevel.push(roots[i]);
		while (!bNextLevel.empty() || !lgNextLevel.empty() || !hgNextLevel.empty())
		{ // for each level
			nodeInfo[i].push_back(vector<tuple<int, int, int, int>>());
			int b = bNextLevel.size(), l = lgNextLevel.size(), r = hgNextLevel.size();
			_for(j, 0, b)
			{
				bluenode_d* t = bNextLevel.front();
				bNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(1, t->l, t->h, t->subids.size()));
				if (t->leftBlueChild) bNextLevel.push(t->leftBlueChild);
				if (t->rightBlueChild) bNextLevel.push(t->rightBlueChild);
				if (t->lowGreenChild) lgNextLevel.push(t->lowGreenChild);
				if (t->highGreenChild) hgNextLevel.push(t->highGreenChild);
			}
			_for(j, 0, l)
			{
				lgreennode_d* t = lgNextLevel.front();
				lgNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(2, t->l, t->h, t->subids.size()));
				if (t->leftChild) lgNextLevel.push(t->leftChild);
				if (t->rightChild) lgNextLevel.push(t->rightChild);
			}
			_for(j, 0, r)
			{
				hgreennode_d* t = hgNextLevel.front();
				hgNextLevel.pop();
				nodeInfo[i][height].push_back(make_tuple(3, t->l, t->h, t->subids.size()));
				if (t->leftChild) hgNextLevel.push(t->leftChild);
				if (t->rightChild) hgNextLevel.push(t->rightChild);
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
 << "\nEffectivePredicate: " << numEffectivePredicate << ", effectiveRate: "
		 << (double)numEffectivePredicate /
			(double)(numProcessExactPredicate + numProcessOneCmpPredicate + numProcessTwoCmpPredicate + 1)
		 << "\nOR: " << numBitsetOperation << ", AvgHit: " << hit << ".\n\n";

}

int BGTree_d::getHeight()
{
	int height = 0;
	queue<bluenode_d*> bq;
	queue<lgreennode_d*> lq;
	queue<hgreennode_d*> hq;
	bluenode_d* b;
	lgreennode_d* l;
	hgreennode_d* h;
	_for(i, 0, atts) bq.emplace(roots[i]);
	while (!bq.empty() || !lq.empty() || !hq.empty())
	{
		if (!bq.empty())
		{
			b = bq.front();
			bq.pop();
			height = max(height, b->levelid);
			if (b->leftBlueChild != nullptr)
			{
				bq.emplace(b->leftBlueChild);
				bq.emplace(b->rightBlueChild);
				lq.emplace(b->lowGreenChild);
				hq.emplace(b->highGreenChild);
			}
		}
		if (!lq.empty())
		{
			l = lq.front();
			lq.pop();
			height = max(height, l->levelid);
			if (l->leftChild != nullptr)
			{
				lq.emplace(l->leftChild);
				lq.emplace(l->rightChild);
			}
		}
		if (!hq.empty())
		{
			h = hq.front();
			hq.pop();
			height = max(height, h->levelid);
			if (h->leftChild != nullptr)
			{
				hq.emplace(h->leftChild);
				hq.emplace(h->rightChild);
			}
		}
	}
	return height;
}