#include "BGTree.h"

BGTree::BGTree() {
	numSub = 0;
	boundaryNumSub = subs / 32;
	numNode = 1;
	initHeight = initH;
	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);

	_for(i, 0, atts) {
		roots[i] = new bluenode(0, valDom - 1, 1, 1, {}, nullptr, nullptr, nullptr, nullptr, nullptr);
		initBlueNode(roots[i]);
	}
	height = initHeight;
}

BGTree::~BGTree() {
	_for(i, 0, atts) {
		releaseBlueNode(roots[i]);
		delete roots[i];
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
	r->leftBlueChild = new bluenode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr, nullptr);
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
	r->leftChild = new rgreennode(r->l, r->mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new rgreennode(r->mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
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
	subPredicate[sub.id] = sub.size;
	_for(i, 0, sub.size)
		insertIntoBlueNode(roots[sub.constraints[i].att], sub.id, sub.constraints[i].lowValue,
			sub.constraints[i].highValue);
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

	if (l == r->l)
		r->lequal.push_back(subID);

	if (r->leftBlueChild != nullptr) { // 有子节点
		if (h <= r->mid)
			insertIntoBlueNode(r->leftBlueChild, subID, l, h);
		else if (l >= r->mid)
			insertIntoBlueNode(r->rightBlueChild, subID, l, h);
		else {  // l<mid<h
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
		if (h <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, h);
		else insertIntoGreenNode(r->rightChild, subID, h);
	}
}

bool BGTree::deleteFromBlueNode(bluenode*& r, const int& subID, const int& l, const int& h) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::iterator it = r->subids.begin(); it != r->subids.end(); it++) {
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
	}
	else return false;

	if (r->leftBlueChild != nullptr) {
		if (h <= r->mid)
			find &= deleteFromBlueNode(r->leftBlueChild, subID, l, h);
		else if (l >= r->mid)
			find &= deleteFromBlueNode(r->rightBlueChild, subID, l, h);
		else {// l<mid<h
			find &= deleteFromGreenNode(r->leftGreenChild, subID, l);
			find &= deleteFromGreenNode(r->rightGreenChild, subID, h);
		}
	}


	return find;
}
bool BGTree::deleteFromGreenNode(lgreennode*& r, const int& subID, const int& l) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::iterator it = r->subids.begin(); it != r->subids.end(); it++)
		{
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
	}
	else return false;

	if (r->leftChild != nullptr) {
		if (l <= r->mid)
			find &= deleteFromGreenNode(r->leftChild, subID, l);
		else find &= deleteFromGreenNode(r->rightChild, subID, l);
	}
	return find;
}
bool BGTree::deleteFromGreenNode(rgreennode*& r, const int& subID, const int& h) {
	bool find = false;
	r->numNodeSub--;
	if (r->bst == nullptr) {
		for (vector<int>::iterator it = r->subids.begin(); it != r->subids.end(); it++)
		{
			if (*it == subID) {
				r->subids.erase(it);
				find = true;
				break;
			}
		}
	}
	else if ((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
	}
	else return false;

	if (r->leftChild != nullptr) {
		if (h <= r->mid)
			find &= deleteFromGreenNode(r->leftChild, subID, h);
		else find &= deleteFromGreenNode(r->rightChild, subID, h);
	}
	return find;
}

void BGTree::vectorToBitset(vector<int>& v, bitset<subs>*& b) {
	for (int i = 0; i < v.size(); i++)
		(*b)[v[i]] = 1;
	v.resize(0);
}

void BGTree::bitsetToVector(bitset<subs>*& b, vector<int>& v) {
	_for(i, 0, subs)
		if ((*b)[i]==1)
			v.push_back(i);
	delete b;
}

void BGTree::forward_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	memcpy(counter, subPredicate, sizeof(subPredicate));
	for (int i = 0; i < pub.size; i++)
		forward_match_blueNode(roots[pub.pairs[i].att], pub.pairs[i].att, pub.pairs[i].value, subList);
	for (int i = 0; i < subs; i++)
		if (counter[i] == 0) {
			++matchSubs;
			//cout << "BG-Tree matches sub: " << i << endl;
		}
}

void BGTree::forward_match_blueNode(bluenode*& r, int att, int value, const vector<IntervalSub>& subList) {
	if (r->l == value || r->h == value) {

	}

	if (r->leftBlueChild == nullptr) { // 叶节点, 暴力匹配
		if (r->bst == nullptr) {
			_for(i, 0, r->subids.size()) {
				if (subList[r->subids[i]].constraints[att].lowValue <= value 
			     <= subList[r->subids[i]].constraints[att].highValue)
					counter[r->subids[i]]--;
			}
		}
		else {
			_for(i, 0, subs)
				if ((*(r->bst))[i] == 1 && subList[i].constraints[att].lowValue <= value
					<= subList[i].constraints[att].highValue)
					counter[i]--;
		}
	}
	else if(value< r->mid) {
		forward_match_blueNode(r->leftBlueChild, att, value, subList);
		forward_match_lgreenNode(r->leftGreenChild, att, value, subList);
	}
	else if (value == r->mid) {
		forward_match_blueNode(r->leftBlueChild, att, value, subList);
		forward_match_lgreenNode(r->leftGreenChild, att, value, subList);
		forward_match_blueNode(r->rightBlueChild, att, value, subList);
	}
	else {
		forward_match_blueNode(r->rightBlueChild, att, value, subList);
		forward_match_rgreenNode(r->rightGreenChild, att, value, subList);
	}
}

void BGTree::forward_match_lgreenNode(lgreennode*& r, int att, int value, const vector<IntervalSub>& subList) {

}

void BGTree::forward_match_rgreenNode(rgreennode*& r, int att, int value, const vector<IntervalSub>& subList) {

}

void BGTree::backward_match(const Pub& pub, int& matchSubs) {

}

int BGTree::calMemory() {

	return 0;
}

void BGTree::printBGTree() {

}