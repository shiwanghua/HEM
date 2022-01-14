//
// Created by swh on 2022/1/12.
//

#include "BGTree1.h"

BGTree1::BGTree1() {
	numSub = 0;
	numNode = 0;
	height = 0;
	maxNodeSize = MAXNodeSIZE;
	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);
	_for(i, 0, atts) {
		roots[i] = new bluenode(0, valDom - 1, ++numNode, 1, nullptr, nullptr, nullptr, nullptr);
	}
	nB.resize(atts);
	nnB.resize(atts);

	cout << "ExpID = " << expID << ". BGTree1: maxNodeSize = " << maxNodeSize << "\n";
}

BGTree1::~BGTree1() {
	_for(i, 0, atts) {
		releaseBlueNode(roots[i]);
	}
}

void BGTree1::releaseBlueNode(bluenode *&r) {
	if (r->lowGreenChild) releaseGreenNode(r->lowGreenChild);
	if (r->highGreenChild) releaseGreenNode(r->highGreenChild);
	if (r->leftBlueChild) releaseBlueNode(r->leftBlueChild);
	if (r->rightBlueChild) releaseBlueNode(r->rightBlueChild);
	delete r;
}

void BGTree1::releaseGreenNode(lgreennode *&r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	delete r;
}

void BGTree1::releaseGreenNode(hgreennode *&r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	delete r;
}

void BGTree1::insert(IntervalSub sub) {
	vector<bool> attrExist(atts, false);
	subPredicate[sub.id] = sub.size;
	for (auto &&c: sub.constraints) {
		insertIntoBlueNode(roots[c.att], sub.id, c.lowValue,
						   c.highValue);
		attrExist[c.att] = true;
		nnB[c.att][sub.id] = 1;
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}

void BGTree1::insertIntoBlueNode(bluenode *&r, const int &subID, const int &l, const int &h, const int &attrId,
								 const vector<IntervalSub> &subList) {
	r->subids.push_back(subID);
	r->bst[subID] = 1;
	if (r->mid == h) r->lMidv.push_back(subID);
	else if (r->mid + 1 == l) r->rMidv.push_back(subID);

	if(r->leftBlueChild!= nullptr){ // has child
		if (h <= r->mid) {
				insertIntoBlueNode(r->leftBlueChild, subID, l, h, attrId, subList);
		} else if (l > r->mid) {
				insertIntoBlueNode(r->rightBlueChild, subID, l, h, attrId, subList);
		} else {
				insertIntoGreenNode(r->lowGreenChild, subID, l, attrId, subList);
				insertIntoGreenNode(r->highGreenChild, subID, h, attrId, subList);
			}
	}else if(r->subids.size() > maxNodeSize){ // no child and too big node
		r->leftBlueChild = new bluenode(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr, nullptr);
		r->rightBlueChild = new bluenode(r->mid+1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr, nullptr);
		r->lowGreenChild = new lgreennode(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr);
		r->highGreenChild = new hgreennode(r->mid+1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr);
		for (auto &&id: r->subids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue <= r->mid) {
						r->leftBlueChild->subids.push_back(id);
						r->leftBlueChild->bst[id] = 1;
						if (r->leftBlueChild->mid == c.highValue) r->leftBlueChild->lMidv.push_back(subID);
						else if (r->leftBlueChild->mid + 1 == c.lowValue) r->leftBlueChild->rMidv.push_back(subID);
					} else if (c.lowValue > r->mid) {
						r->rightBlueChild->numNodeSub++;
						r->rightBlueChild->subids.push_back(id);
						r->rightBlueChild->bst[id] = 1;
						if (r->rightBlueChild->mid == c.highValue) r->rightBlueChild->lMidv.push_back(subID);
						else if (r->rightBlueChild->mid + 1 == c.lowValue) r->rightBlueChild->rMidv.push_back(subID);
					} else { // l<=mid<mid+1<=h
						r->lowGreenChild->subids.push_back(id);
						r->lowGreenChild->bst[id] = 1;
						if (r->lowGreenChild->mid + 1 == c.lowValue)
							r->lowGreenChild->midv.push_back(id);
						r->highGreenChild->subids.push_back(id);
						r->highGreenChild->bst[id] = 1;
						if (r->highGreenChild->mid == c.highValue)
							r->highGreenChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree1::insertIntoGreenNode(lgreennode *&r, const int &subID, const int &l, const int &attrId,
								  const vector<IntervalSub> &subList) {
	r->subids.push_back(subID);
	r->bst[subID] = 1;
	if(r->mid+1==l)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (l <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, l, attrId, subList);
		else insertIntoGreenNode(r->rightChild, subID, l, attrId, subList);
	}else if(r->subids.size()>maxNodeSize){
		r->leftChild=new lgreennode(r->l,r->mid,++numNode,r->levelid+1, nullptr, nullptr);
		r->rightChild=new lgreennode(r->mid+1,r->h,++numNode,r->levelid+1, nullptr, nullptr);
		for (auto &&id: r->subids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if(c.lowValue<=r->mid){
						r->leftChild->subids.push_back(id);
						r->leftChild->bst[id]=1;
						if(r->leftChild->mid+1==c.lowValue)
							r->leftChild->midv.push_back(id);
					}else{
						r->rightChild->subids.push_back(id);
						r->rightChild->bst[id]=1;
						if(r->rightChild->mid+1==c.lowValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree1::insertIntoGreenNode(hgreennode *&r, const int &subID, const int &h, const int &attrId,
								  const vector<IntervalSub> &subList) {
	r->subids.push_back(subID);
	r->bst[subID] = 1;
	if(r->mid==h)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (h < r->mid)
			insertIntoGreenNode(r->leftChild, subID, h, attrId, subList);
		else insertIntoGreenNode(r->rightChild, subID, h, attrId, subList);
	}else if(r->subids.size()>maxNodeSize){
		r->leftChild=new hgreennode(r->l,r->mid-1,++numNode,r->levelid+1, nullptr, nullptr);
		r->rightChild=new hgreennode(r->mid,r->h,++numNode,r->levelid+1, nullptr, nullptr);
		for (auto &&id: r->subids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if(c.highValue<r->mid){
						r->leftChild->subids.push_back(id);
						r->leftChild->bst[id]=1;
						if(r->leftChild->mid==c.highValue)
							r->leftChild->midv.push_back(id);
					}else{
						r->rightChild->subids.push_back(id);
						r->rightChild->bst[id]=1;
						if(r->rightChild->mid==c.highValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}

bool BGTree1::deleteSubscription(IntervalSub sub){
	bool find=true;
	subPredicate[sub.id] = 0;
	for (auto &&c: sub.constraints) {
		find&=deleteFromBlueNode(roots[c.att], sub.id, c.lowValue,
						   c.highValue);
	}
	numSub--;
	return find;
}

bool BGTree1::deleteFromBlueNode(bluenode *&r, const int &subID, const int &l, const int &h) {
	bool find=false;
	r->bst[subID]=0;
	
	return find;
}