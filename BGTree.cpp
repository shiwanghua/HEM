#include "BGTree.h"

BGTree::BGTree() {
	numSub = 0;
	boundaryNumSub = subs / 32;
	numNode = 1;
	initHeight = initH;
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
		r->isleaf = true;
		return;
	}
	int mid = (r->l + r->h) >> 1, nextLevelID = r->levelid + 1;
	r->leftGreenChild = new lgreennode(r->l, mid - 1, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftGreenChild);
	r->rightGreenChild = new rgreennode(mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightGreenChild);
	r->leftBlueChild = new bluenode(r->l, mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr, nullptr);
	initBlueNode(r->leftBlueChild);
	r->rightBlueChild = new bluenode(mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr, nullptr,
		nullptr);
	initBlueNode(r->rightBlueChild);
}

void BGTree::initGreenNode(lgreennode*& r) {
	if (r->levelid == initHeight) {
		r->isleaf = true;
		return;
	}
	int mid = (r->l + r->h) >> 1, nextLevelID = r->levelid + 1;
	r->leftChild = new lgreennode(r->l, mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new lgreennode(mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->rightChild);
}

void BGTree::initGreenNode(rgreennode*& r) {
	if (r->levelid == initHeight) {
		r->isleaf = true;
		return;
	}
	int mid = (r->l + r->h) >> 1, nextLevelID = r->levelid + 1;
	r->leftChild = new rgreennode(r->l, mid, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
	initGreenNode(r->leftChild);
	r->rightChild = new rgreennode(mid + 1, r->h, ++numNode, nextLevelID, {}, nullptr, nullptr, nullptr);
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

	int mid = (r->l + r->h) >> 1;
	if (r->leftBlueChild != nullptr && h <= mid)
		insertIntoBlueNode(r->leftBlueChild, subID, l, h);
	else if (r->rightBlueChild != nullptr && l >= mid)
		insertIntoBlueNode(r->rightBlueChild, subID, l, h);
	else if (r->leftGreenChild) {// l<mid<h
		insertIntoGreenNode(r->leftGreenChild, subID, l);
		insertIntoGreenNode(r->rightGreenChild, subID, h);
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

	int mid = (r->l + r->h) >> 1;
	if (r->leftChild != nullptr) {
		if (l <= mid)
			insertIntoGreenNode(r->leftChild, subID, l);
		else insertIntoGreenNode(r->rightChild, subID, l);
	}
}
void BGTree::insertIntoGreenNode(rgreennode*& r, const int& subID, const int& l) {
	r->numNodeSub++;
	if (r->bst == nullptr) {
		r->subids.push_back(subID);
		if (r->numNodeSub >= boundaryNumSub) //r->subids.size()==r->numNodeSub
			vectorToBitset(r->subids, r->bst);
	}
	else {
		(*(r->bst))[subID] = 1;
	}

	int mid = (r->l + r->h) >> 1;
	if (r->leftChild != nullptr) {
		if (l <= mid)
			insertIntoGreenNode(r->leftChild, subID, l);
		else insertIntoGreenNode(r->rightChild, subID, l);
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
	else if((*(r->bst))[subID]) {
		(*(r->bst))[subID] = 0;
	}
	else return false;

	int mid = (r->l + r->h) >> 1;
	if (r->leftBlueChild != nullptr && h <= mid)
		find&=deleteFromBlueNode(r->leftBlueChild, subID, l, h);
	else if (r->rightBlueChild != nullptr && l >= mid)
		find&=deleteFromBlueNode(r->rightBlueChild, subID, l, h);
	else if (r->leftGreenChild) {// l<mid<h
		find&=deleteFromGreenNode(r->leftGreenChild, subID, l);
		find&=deleteFromGreenNode(r->rightGreenChild, subID, h);
	}
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
	
}
bool BGTree::deleteFromGreenNode(rgreennode*& r, const int& subID, const int& h) {

}

void BGTree::vectorToBitset(vector<int>& v, bitset<subs>*& b) {
	for (int i = 0; i < v.size(); i++)
		(*b)[v[i]] = 1;
	v.resize(0);
}

void BGTree::bitsetToVector(bitset<subs>*& b, vector<int>& v) {
	_for(i, 0, subs)
		if ((*b)[i])
			v.push_back(i);
	delete b;
}

void BGTree::forward_match(const Pub& pub, int& matchSubs) {

}

void BGTree::backward_match(const Pub& pub, int& matchSubs) {

}

int BGTree::calMemory() {

	return 1;
}

void BGTree::printBGTree() {

}