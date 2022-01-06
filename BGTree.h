#pragma once

#include<vector>
#include <cstring>
#include <algorithm>
#include "generator.h"
#include "chrono_time.h"
#include "util.h"
#include "data_structure.h"
#include "constant.h"
#include <bitset>
#include <queue>

struct lgreennode {
	// low/high boundary
	int l, h, mid, nodeid, levelid, numNodeSub = 0;
	bitset<subs>* bst;
	vector<int> subids; // Either bst is nullptr or subids.size()==0
	//vector<int> midequal; // Record subID with predicate including mid
	lgreennode* leftChild, * rightChild; // 2个节点指针要么都是空的, 要么都是非空

	lgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs>* bits, lgreennode* lc,
		lgreennode* rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};

struct rgreennode {
	int l, h, mid, nodeid, levelid, numNodeSub = 0;
	bitset<subs>* bst;
	vector<int> subids;
	//vector<int> midEqual; // Record subID with predicate including mid
	rgreennode* leftChild, * rightChild;

	rgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs>* bits, rgreennode* lc,
		rgreennode* rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};

struct bluenode {
	int l, h, mid, nodeid, levelid, numNodeSub = 0;
	bitset<subs>* bst;
	vector<int> subids;         // 对于内部节点, 只能用于反向标记
	vector<int> midEqual;       // Record subID with predicate including mid
	lgreennode* leftGreenChild; // 4个节点指针要么都是空的, 要么都是非空
	rgreennode* rightGreenChild;
	bluenode* leftBlueChild, * rightBlueChild;

	bluenode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs>* bits, lgreennode* lc,
		rgreennode* rc, bluenode* lbc, bluenode* rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftGreenChild(lc),
		rightGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {
		mid = (l + h) >> 1;
	}
};

// Version1: can't expand node, height is fixed.
class BGTree {
private:
	int numSub;         // Number of inserted subscriptions
	int boundaryNumSub; // When the value of numSub in a node > boundaryNumSub, the memory of bitset is smaller than storing $numSub subscriptions directly
	int numNode;        // Number of all nodes
	int height;         // The height of BGTree with only one node is 1
	int initHeight;     // Initial height constructing the BGTree, >=1
	int subPredicate[subs];
	int counter[subs];
	vector<bluenode*> roots;
	vector<bitset<subs>> nB; // null bitset for C-BOMP
	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void initBlueNode(bluenode*& r);
	void initGreenNode(lgreennode*& r);
	void initGreenNode(rgreennode*& r);

	void releaseBlueNode(bluenode*& r);
	void releaseGreenNode(lgreennode*& r);
	void releaseGreenNode(rgreennode*& r);

	void insertIntoBlueNode(bluenode*& r, const int& subID, const int& l, const int& h);
	void insertIntoGreenNode(lgreennode*& r, const int& subID, const int& l);
	void insertIntoGreenNode(rgreennode*& r, const int& subID, const int& h);

	bool deleteFromBlueNode(bluenode*& r, const int& subID, const int& l, const int& h);
	bool deleteFromGreenNode(lgreennode*& r, const int& subID, const int& l);
	bool deleteFromGreenNode(rgreennode*& r, const int& subID, const int& h);

	void vectorToBitset(vector<int>& v, bitset<subs>*&);
	void bitsetToVector(bitset<subs>*& b, vector<int>&);

	void forward_match_blueNode(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList);
	void forward_match_lgreenNode(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList);
	void forward_match_rgreenNode(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList);

	void backward_match_blueNode(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList);
	void backward_match_lgreenNode(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList);
	void backward_match_rgreenNode(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList);

	void forward_match_blueNode_C_BOMP(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList,bitset<subs>& mB);
	void forward_match_lgreenNode_C_BOMP(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList,bitset<subs>& mB);
	void forward_match_rgreenNode_C_BOMP(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList,bitset<subs>& mB);

	void backward_match_blueNode_C_BOMP(bluenode*& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB);
	void backward_match_lgreenNode_C_BOMP(lgreennode*& l, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB);
	void backward_match_rgreenNode_C_BOMP(rgreennode*& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<subs>& mB);

	double calBlueNodeMemory(bluenode*& r);
	double calLGreenNodeMemory(lgreennode*& r);
	double calRGreenNodeMemory(rgreennode*& r);

public:
	int hit = 0; // mid 命中次数

	BGTree();

	~BGTree();

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	void forward_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// Similar to bTama6
	void forward_match_C_BOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// Similar to bTama8
	void backward_match_C_BOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	int calMemory();      // 计算占用内存大小

	void printBGTree();   // 找每一层的存储订阅数最大的节点 

	int getHeight() { return height; }
	int getBoundaryNumSub() { return boundaryNumSub; }
	int getNumNode() { return numNode; }

};

