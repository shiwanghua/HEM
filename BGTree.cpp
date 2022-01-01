#include "BGTree.h"

BGTree::BGTree() {
	numSub = 0;
	boundaryNumSub = subs;// 32;
	numNode = 1;
	initHeight = initH;
	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);

	_for(i, 0, atts) {
		roots[i] = new bluenode(0, valDom - 1, 1, 1, {}, nullptr, nullptr, nullptr, nullptr, nullptr);
		initBlueNode(roots[i]);
	}
	height = initHeight;

	nB.resize(atts);
	nnB.resize(atts);

	cout << "ExpID = " << expID << ". BGTree: height = " << height << "\n";
}

BGTree::~BGTree() {
	_for(i, 0, atts) {
		releaseBlueNode(roots[i]);
	}
}

void BGTree::initBlueNode(bluenode*& r) {
	if (r->levelid == initHeight) {
		return;
	}
	int nextLevelID = r->levelid + 1;
	r->leftGreenChild = new lgreennode(r->l, r->mid - 1, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftGreenChild);
	r->rightGreenChild = new rgreennode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightGreenChild);
	r->leftBlueChild = new bluenode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr,
		nullptr);
	initBlueNode(r->leftBlueChild);
	r->rightBlueChild = new bluenode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr,
		nullptr);
	initBlueNode(r->rightBlueChild);
}

void BGTree::initGreenNode(lgreennode*& r) {
	if (r->levelid == initHeight) {
		return;
	}
	int nextLevelID = r->levelid + 1;
	r->leftChild = new lgreennode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new lgreennode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightChild);
}

void BGTree::initGreenNode(rgreennode*& r) {
	if (r->levelid == initHeight) {
		return;
	}
	int nextLevelID = r->levelid + 1;
	// 注意右绿节点的左子区间为 [l,mid-1], 对称美
	r->leftChild = new rgreennode(r->l, r->mid - 1, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new rgreennode(r->mid, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightChild);
}

void BGTree::releaseBlueNode(bluenode*& r) {
	if (r->leftGreenChild) releaseGreenNode(r->leftGreenChild);
	if (r->rightGreenChild) releaseGreenNode(r->rightGreenChild);
	if (r->leftBlueChild) releaseBlueNode(r->leftBlueChild);
	if (r->rightBlueChild) releaseBlueNode(r->rightBlueChild);
	if (r->bst != nullptr) delete r->bst;
	delete r;
}

void BGTree::releaseGreenNode(lgreennode*& r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->bst != nullptr) delete r->bst;
	delete r;
}

void BGTree::releaseGreenNode(rgreennode*& r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->bst != nullptr) delete r->bst;
	delete r;
}

void BGTree::insert(IntervalSub sub) {
	vector<bool> attrExist(atts, false);
	subPredicate[sub.id] = sub.size;
	for (auto&& c : sub.constraints) {
		insertIntoBlueNode(roots[c.att], sub.id, c.lowValue,
			c.highValue);
		attrExist[c.att] = true;
		nnB[c.att][sub.id] = 1;
	}
	_for(i, 0, atts) if (!attrExist[i])
		nB[i][sub.id] = 1;
	numSub++;
}

void BGTree::insertIntoBlueNode(bluenode*& r, const int& subID, const int& l, const int& h) {
	r->numNodeSub++;
	if (r->bst == nullptr) {
		r->subids.push_back(subID);
		if (r->numNodeSub >= boundaryNumSub) //r->subids.size()==r->numNodeSub
			vectorToBitset(r->subids, r->bst);
	}
	else {
		(*(r->bst))[subID] = 1;
	}

	if (r->mid == l || r->mid == h)
		r->midEqual.push_back(subID);

	if (r->leftBlueChild != nullptr) { // 有子节点
		if (h <= r->mid)
			insertIntoBlueNode(r->leftBlueChild, subID, l, h);
		else if (l >= r->mid)
			insertIntoBlueNode(r->rightBlueChild, subID, l, h);
		else {  // l<mid<h
			r->midEqual.push_back(subID);
			insertIntoGreenNode(r->leftGreenChild, subID, l);
			insertIntoGreenNode(r->rightGreenChild, subID, h);
		}
	}
}

void BGTree::insertIntoGreenNode(lgreennode*& r, const int& subID, const int& l) {
	r->numNodeSub++;
	if (r->bst == nullptr) {
		r->subids.push_back(subID);
		if (r->numNodeSub >= boundaryNumSub) //r->subids.size()==r->numNodeSub
			vectorToBitset(r->subids, r->bst);
	}
	else {
		(*(r->bst))[subID] = 1;
	}

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (l <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, l);
		else insertIntoGreenNode(r->rightChild, subID, l);
	}
}

void BGTree::insertIntoGreenNode(rgreennode*& r, const int& subID, const int& h) {
	r->numNodeSub++;
	if (r->bst == nullptr) {
		r->subids.push_back(subID);
		if (r->numNodeSub >= boundaryNumSub) //r->subids.size()==r->numNodeSub
			vectorToBitset(r->subids, r->bst);
	}
	else {
		(*(r->bst))[subID] = 1;
	}

	if (r->leftChild != nullptr) {
		if (h >= r->mid)
			insertIntoGreenNode(r->rightChild, subID, h);
		else insertIntoGreenNode(r->leftChild, subID, h);
	}
}

bool BGTree::deleteSubscription(IntervalSub sub) {
	subPredicate[sub.id] = -1;
	bool find = true;
	for (auto&& c : sub.constraints)
		find &= deleteFromBlueNode(roots[c.att], sub.id, c.lowValue,
			c.highValue);
	return find;
}

bool BGTree::deleteFromBlueNode(bluenode*& r, const int& subID, const int& l, const int& h) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++) {
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
		find = true;
	}

	if (!find) return false;

	if (r->mid == l || r->mid == h) {
		find = false;
		for (auto it = r->midEqual.cbegin(); it != r->midEqual.cend(); it++) {
			if (*it == subID) {
				r->midEqual.erase(it);
				find = true;
				break;
			}
		}
	}

	if (!find) return false;

	if (r->leftBlueChild != nullptr) {
		if (h <= r->mid)
			find = deleteFromBlueNode(r->leftBlueChild, subID, l, h);
		else if (l >= r->mid)
			find = deleteFromBlueNode(r->rightBlueChild, subID, l, h);
		else {// l<mid<h
			find = deleteFromGreenNode(r->leftGreenChild, subID, l);
			find &= deleteFromGreenNode(r->rightGreenChild, subID, h);
		}
	}

	return find;
}

bool BGTree::deleteFromGreenNode(lgreennode*& r, const int& subID, const int& l) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::const_iterator it = r->subids.begin(); it != r->subids.cend(); it++) {
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
		find = true;
	}

	if (!find) return false;

	if (r->leftChild != nullptr) {
		if (l <= r->mid)
			find = deleteFromGreenNode(r->leftChild, subID, l);
		else find = deleteFromGreenNode(r->rightChild, subID, l);
	}
	return find;
}

bool BGTree::deleteFromGreenNode(rgreennode*& r, const int& subID, const int& h) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++) {
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
		find = true;
	}

	if (!find) return false;

	if (r->leftChild != nullptr) {
		if (h <= r->mid)
			find = deleteFromGreenNode(r->leftChild, subID, h);
		else find = deleteFromGreenNode(r->rightChild, subID, h);
	}
	return find;
}

void BGTree::vectorToBitset(vector<int>& v, bitset<subs>*& b) {
	b = new bitset<subs>;
	for (int i = 0; i < v.size(); i++)
		(*b)[v[i]] = 1;
	v.resize(0);
}

void BGTree::bitsetToVector(bitset<subs>*& b, vector<int>& v) {
	_for(i, 0, subs) if ((*b)[i] == 1)
		v.push_back(i);
	delete b;
}

void BGTree::forward_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	memcpy(counter, subPredicate, sizeof(subPredicate));
	for (auto&& pi : pub.pairs)
		forward_match_blueNode(roots[pi.att], pi.att, pi.value, subList);
	for (int i = 0; i < subs; i++)
		if (counter[i] == 0) {
			++matchSubs;
			//cout << "BG-Tree matches sub: " << i << endl;
		}
}
void BGTree::forward_match_blueNode(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList) {
	if (r->mid == value) { // 1.等于中点, 直接得到匹配结果
		hit++;
		for (auto&& i : r->midEqual) {
			counter[i]--;
		}
	}
	else if (r->leftBlueChild == nullptr) { // 2.叶节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							counter[i]--;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							counter[i]--;
						break;
					}
		}
	}
	else if (value < r->mid) { // 3.检索蓝绿左子节点
		forward_match_blueNode(r->leftBlueChild, att, value, subList);
		forward_match_lgreenNode(r->leftGreenChild, att, value, subList);
	}
	else { // value > r->mid 4.检索蓝绿右子节点
		forward_match_blueNode(r->rightBlueChild, att, value, subList);
		forward_match_rgreenNode(r->rightGreenChild, att, value, subList);
	}
}
void BGTree::forward_match_lgreenNode(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList) {
	if (l->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (l->bst == nullptr) {
			for (auto&& i : l->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							counter[i]--;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							counter[i]--;
						break;
					}
		}
	}
	else if (value == l->mid) { // 2.等于中点且有左绿左子节点, 左绿左子节点即为匹配结果
		hit++;
		if (l->leftChild->bst == nullptr)
			for (auto&& i : l->leftChild->subids) {
				counter[i]--;
			}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				counter[i]--;
		}
	}
	else if (value < l->mid) { // 3. 检索左绿左子节点, 左绿右子节点全部不匹配
		forward_match_lgreenNode(l->leftChild, att, value, subList);
	}
	else { // value > l->mid  4. 检索左绿右子节点, 左子节点全部匹配
		forward_match_lgreenNode(l->rightChild, att, value, subList);
		if (l->leftChild->bst == nullptr) {
			for (auto&& i : l->leftChild->subids) {
				counter[i]--;
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				counter[i]--;
		}
	}
}
void BGTree::forward_match_rgreenNode(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList) {
	if (r->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							counter[i]--;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							counter[i]--;
						break;
					}
		}
	}
	else if (value == r->mid) { // 2.等于中点且有右绿右子节点, 右绿右子节点即为匹配结果
		hit++;
		if (r->rightChild->bst == nullptr)
			for (auto&& i : r->rightChild->subids) {
				counter[i]--;
			}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				counter[i]--;
		}
	}
	else if (value > r->mid) { // 3. 检索右绿右子节点, 右绿左子节点全部不匹配
		forward_match_rgreenNode(r->rightChild, att, value, subList);
	}
	else { // value < r->mid  4. 检索右绿左子节点, 右绿右子节点全部匹配
		forward_match_rgreenNode(r->leftChild, att, value, subList);
		if (r->rightChild->bst == nullptr) {
			for (auto&& i : r->rightChild->subids) {
				counter[i]--;
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				counter[i]--;
		}
	}
}

void BGTree::forward_match_C_BOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	bitset<subs> gB, mB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs) {
		mB = nB[pi.att];
		attExist[pi.att] = true;
		forward_match_blueNode_C_BOMP(roots[pi.att], pi.att, pi.value, subList, mB);
		gB = gB & mB;
	}
	_for(i, 0, atts)
		if (!attExist[i])
			gB = gB & nB[i];
	matchSubs = gB.count();
}
void BGTree::forward_match_blueNode_C_BOMP(bluenode*& r, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (r->mid == value) { // 1.等于中点, 直接得到匹配结果
		hit++;
		for (auto&& i : r->midEqual) {
			mB[i] = 1;
		}
	}
	else if (r->leftBlueChild == nullptr) { // 2.叶节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							mB[i] = 1;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							mB[i] = 1;
						break;
					}
		}
	}
	else if (value < r->mid) { // 3.检索蓝绿左子节点
		forward_match_blueNode_C_BOMP(r->leftBlueChild, att, value, subList, mB);
		forward_match_lgreenNode_C_BOMP(r->leftGreenChild, att, value, subList, mB);
	}
	else { // value > r->mid 4.检索蓝绿右子节点
		forward_match_blueNode_C_BOMP(r->rightBlueChild, att, value, subList, mB);
		forward_match_rgreenNode_C_BOMP(r->rightGreenChild, att, value, subList, mB);
	}
}
void BGTree::forward_match_lgreenNode_C_BOMP(lgreennode*& l, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (l->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (l->bst == nullptr) {
			for (auto&& i : l->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							mB[i] = 1;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							mB[i] = 1;
						break;
					}
		}
	}
	else if (value == l->mid) { // 2.等于中点且有左绿左子节点, 左绿左子节点即为匹配结果
		hit++;
		if (l->leftChild->bst == nullptr)
			for (auto&& i : l->leftChild->subids) {
				mB[i] = 1;
			}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				mB[i] = 1;
		}
	}
	else if (value < l->mid) { // 3. 检索左绿左子节点, 左绿右子节点全部不匹配
		forward_match_lgreenNode_C_BOMP(l->leftChild, att, value, subList, mB);
	}
	else { // value > l->mid  4. 检索左绿右子节点, 左子节点全部匹配
		forward_match_lgreenNode_C_BOMP(l->rightChild, att, value, subList, mB);
		if (l->leftChild->bst == nullptr) {
			for (auto&& i : l->leftChild->subids) {
				mB[i] = 1;
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				mB[i] = 1;
		}
	}
}
void BGTree::forward_match_rgreenNode_C_BOMP(rgreennode*& r, const int& att, const int& value,
	const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (r->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							mB[i] = 1;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							mB[i] = 1;
						break;
					}
		}
	}
	else if (value == r->mid) { // 2.等于中点且有右绿右子节点, 右绿右子节点即为匹配结果
		hit++;
		if (r->rightChild->bst == nullptr)
			for (auto&& i : r->rightChild->subids) {
				mB[i] = 1;
			}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				mB[i] = 1;
		}
	}
	else if (value > r->mid) { // 3. 检索右绿右子节点, 右绿左子节点全部不匹配
		forward_match_rgreenNode_C_BOMP(r->rightChild, att, value, subList, mB);
	}
	else { // value < r->mid  4. 检索右绿左子节点, 右绿右子节点全部匹配
		forward_match_rgreenNode_C_BOMP(r->leftChild, att, value, subList, mB);
		if (r->rightChild->bst == nullptr) {
			for (auto&& i : r->rightChild->subids) {
				mB[i] = 1;
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				mB[i] = 1;
		}
	}
}


void BGTree::backward_match_C_BOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	bitset<subs> gB, mB; // global bitset
	vector<bool> attExist(atts, false);
	for (auto&& pi : pub.pairs) {
		mB = nnB[pi.att]; // Assume all having definitions is mismatching and marked
		attExist[pi.att] = true;
		backward_match_blueNode_C_BOMP(roots[pi.att], pi.att, pi.value, subList, mB);
		gB = gB | mB;
	}
	_for(i, 0, atts)
		if (!attExist[i])
			gB = gB | nnB[i];
	matchSubs = numSub - gB.count();
}
void BGTree::backward_match_blueNode_C_BOMP(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (r->mid == value) { // 1.等于中点, 直接得到匹配结果
		hit++;
		for (auto&& i : r->midEqual) {
			mB[i] = 0;
		}
	}
	else if (r->leftBlueChild == nullptr) { // 2.叶节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							mB[i] = 0;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value <= pi.highValue)
							mB[i] = 0;
						break;
					}
		}
	}
	else if (value < r->mid) { // 3.检索蓝绿左子节点
		backward_match_blueNode_C_BOMP(r->leftBlueChild, att, value, subList, mB);
		backward_match_lgreenNode_C_BOMP(r->leftGreenChild, att, value, subList, mB);
	}
	else { // value > r->mid 4.检索蓝绿右子节点
		backward_match_blueNode_C_BOMP(r->rightBlueChild, att, value, subList, mB);
		backward_match_rgreenNode_C_BOMP(r->rightGreenChild, att, value, subList, mB);
	}
}
void BGTree::backward_match_lgreenNode_C_BOMP(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (l->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (l->bst == nullptr) {
			for (auto&& i : l->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							mB[i] = 0;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.lowValue <= value)
							mB[i] = 0;
						break;
					}
		}
	}
	else if (value == l->mid) { // 2.等于中点且有左绿左子节点, 左绿左子节点即为匹配结果
		hit++;
		if (l->leftChild->bst == nullptr)
			for (auto&& i : l->leftChild->subids) {
				mB[i] = 0;
			}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				mB[i] = 0;
		}
	}
	else if (value < l->mid) { // 3. 检索左绿左子节点, 左绿右子节点全部不匹配
		backward_match_lgreenNode_C_BOMP(l->leftChild, att, value, subList, mB);
	}
	else { // value > l->mid  4. 检索左绿右子节点, 左子节点全部匹配
		backward_match_lgreenNode_C_BOMP(l->rightChild, att, value, subList, mB);
		if (l->leftChild->bst == nullptr) {
			for (auto&& i : l->leftChild->subids) {
				mB[i] = 0;
			}
		}
		else {
			_for(i, 0, subs) if ((*(l->leftChild->bst))[i] == 1)
				mB[i] = 0;
		}
	}
}
void BGTree::backward_match_rgreenNode_C_BOMP(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB) {
	if (r->leftChild == nullptr) { // 1.叶子节点, 暴力匹配
		if (r->bst == nullptr) {
			for (auto&& i : r->subids) {
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							mB[i] = 0;
						break;
					}
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->bst))[i] == 1)
				for (auto&& pi : subList[i].constraints)
					if (att == pi.att) {
						if (pi.highValue >= value)
							mB[i] = 0;
						break;
					}
		}
	}
	else if (value == r->mid) { // 2.等于中点且有右绿右子节点, 右绿右子节点即为匹配结果
		hit++;
		if (r->rightChild->bst == nullptr)
			for (auto&& i : r->rightChild->subids) {
				mB[i] = 0;
			}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				mB[i] = 0;
		}
	}
	else if (value > r->mid) { // 3. 检索右绿右子节点, 右绿左子节点全部不匹配
		backward_match_rgreenNode_C_BOMP(r->rightChild, att, value, subList, mB);
	}
	else { // value < r->mid  4. 检索右绿左子节点, 右绿右子节点全部匹配
		backward_match_rgreenNode_C_BOMP(r->leftChild, att, value, subList, mB);
		if (r->rightChild->bst == nullptr) {
			for (auto&& i : r->rightChild->subids) {
				mB[i] = 0;
			}
		}
		else {
			_for(i, 0, subs) if ((*(r->rightChild->bst))[i] == 1)
				mB[i] = 0;
		}
	}
}

int BGTree::calMemory() {
	double size = 0.0; // Byte
	size += sizeof(roots)+sizeof(bluenode*) * atts;
	for (int i = 0; i < atts;i++) {
		size+=calBlueNodeMemory(roots[i]);
	}
	size += sizeof(int) * subs * 2; // subPredicate counter
	size += sizeof(nB) + sizeof(nB[0]) * atts; // nB or nnB
	size = size / 1024 / 1024; // MB
	return (int)size;
}
double BGTree::calBlueNodeMemory(bluenode*& r) {
	double size = sizeof(bitset<subs>*) + (sizeof(vector<int>) + sizeof(lgreennode*) + sizeof(bluenode*)) * 2+sizeof(int)*6;
	//cout << sizeof(bitset<subs>*) << " " << sizeof(vector<int>) << " " << sizeof(lgreennode*) << " " << sizeof(bluenode*)<<sizeof(bitset<subs>) << "\n";
	if (r->bst != nullptr)
		size += sizeof(bitset<subs>);
	else size += sizeof(int) * r->subids.size();

	size += sizeof(int)*r->midEqual.size();

	if (r->leftBlueChild)
		size += calBlueNodeMemory(r->leftBlueChild);
	if (r->rightBlueChild)
		size += calBlueNodeMemory(r->rightBlueChild);
	if (r->leftGreenChild)
		size += calLGreenNodeMemory(r->leftGreenChild);
	if (r->rightGreenChild)
		size += calRGreenNodeMemory(r->rightGreenChild);
	return size;
}
double BGTree::calLGreenNodeMemory(lgreennode*& r) {
	double size = sizeof(bitset<subs>*) +sizeof(vector<int>) + sizeof(lgreennode*) * 2 + sizeof(int) * 6;
	if (r->bst != nullptr)
		size += sizeof(bitset<subs>);
	else size += sizeof(int) * r->subids.size();
	if (r->leftChild)
		size += calLGreenNodeMemory(r->leftChild);
	if (r->rightChild)
		size += calLGreenNodeMemory(r->rightChild);
	return size;
}
double BGTree::calRGreenNodeMemory(rgreennode*& r) {
	double size = sizeof(bitset<subs>*) + sizeof(vector<int>) + sizeof(rgreennode*) * 2 + sizeof(int) * 6;
	if (r->bst != nullptr)
		size += sizeof(bitset<subs>);
	else size += sizeof(int) * r->subids.size();
	if (r->leftChild)
		size += calRGreenNodeMemory(r->leftChild);
	if (r->rightChild)
		size += calRGreenNodeMemory(r->rightChild);
	return size;
}

void BGTree::printBGTree() {

}