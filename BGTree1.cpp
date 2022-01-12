//
// Created by swh on 2022/1/12.
//

#include "BGTree1.h"

BGTree1::BGTree1() {
	numSub = 0;
	numNode = 0;
	height=0;
	maxNodeSize=MAXNodeSIZE;
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
void BGTree1::insertIntoBlueNode(bluenode *&r, const int &subID, const int &l, const int &h,const int& attrId, const vector<IntervalSub> &subList) {
	r->subids.push_back(subID);
	r->bst[subID] = 1;
	if(r->mid==h) r->lMidv.push_back(subID);
	else if(r->mid+1==l) r->rMidv.push_back(subID);

	if(r->subids.size()>maxNodeSize){ ;
		r->leftBlueChild=new bluenode(l,h,numNode++,r->levelid+1, nullptr, nullptr, nullptr, nullptr);
		r->rightBlueChild=new bluenode(l,h,numNode++,r->levelid+1, nullptr, nullptr, nullptr, nullptr);
		r->lowGreenChild=new lgreennode(l,h,numNode++,r->levelid+1,nullptr,nullptr);
		r->highGreenChild=new hgreennode(l,h,numNode++,r->levelid+1,nullptr,nullptr);
		for(auto&& id:r->subids){
			for(auto&& c:subList[id].constraints){
				if(c.att==attrId){
					if(c.highValue<=r->mid){
						r->leftBlueChild->subids.push_back(id);
						r->leftBlueChild->bst[id]=1;
						if(r->leftBlueChild->mid==h) r->leftBlueChild->lMidv.push_back(subID);
						else if(r->leftBlueChild->mid+1==l) r->leftBlueChild->rMidv.push_back(subID);
					}else if(c.lowValue>r->mid){
						r->rightBlueChild->numNodeSub++;
						r->rightBlueChild->subids.push_back(id);
						r->rightBlueChild->bst[id]=1;
						if(r->rightBlueChild->mid==h) r->rightBlueChild->lMidv.push_back(subID);
						else if(r->rightBlueChild->mid+1==l) r->rightBlueChild->rMidv.push_back(subID);
					}else{ // l<=mid<mid+1<=h
						r->lowGreenChild->subids.push_back(id);
						r->lowGreenChild->bst[id]=1;
						if(r->lowGreenChild->mid+1==l)
							r->lowGreenChild->midv.push_back(id);
						r->highGreenChild->subids.push_back(id);
						r->highGreenChild->bst[id]=1;
						if(r->highGreenChild->mid==h)
							r->highGreenChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
	if(h<=r->mid){
		if(r->leftBlueChild!= nullptr)
			insertIntoBlueNode(r->leftBlueChild, subID, l, h,attrId,subList);
	}
	else if(l>r->mid){
		if(r->rightBlueChild!= nullptr)
			insertIntoBlueNode(r->rightBlueChild,subID,l,h,attrId,subList);
	}
	else{
		if(r->lowGreenChild!= nullptr){
			insertIntoGreenNode(r->lowGreenChild,subID,l,attrId,subList);
			insertIntoGreenNode(r->highGreenChild,subID,h,attrId,subList);
		}
	}
}
void BGTree1::insertIntoGreenNode(lgreennode *&r, const int &subID, const int &l,const int& attrId, const vector<IntervalSub> &subList) {
	r->subids.push_back(subID);
	r->bst[subID] = 1;
	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (l <= r->mid)
			insertIntoGreenNode(r->leftChild, subID, l);
		else insertIntoGreenNode(r->rightChild, subID, l);
	}
}
void BGTree1::insertIntoGreenNode(hgreennode *&r, const int &subID, const int &h,const int& attrId, const vector<IntervalSub> &subList) {
	if (r->bst == nullptr) {
		r->subids.push_back(subID);
		if (r->numNodeSub >= boundaryNumSub) //r->subids.size()==r->numNodeSub
			vectorToBitset(r->subids, r->bst);
	} else {
		(*(r->bst))[subID] = 1;
	}

	if (r->leftChild != nullptr) {
		if (h >= r->mid)
			insertIntoGreenNode(r->rightChild, subID, h);
		else insertIntoGreenNode(r->leftChild, subID, h);
	}
}

