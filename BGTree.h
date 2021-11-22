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

struct lgreennode {
	bool isleaf = false;
	// low/high boundary
	int l, h, nodeid, levelid, numNodeSub = 0;
	vector<int> subids; // Either bst is nullptr or subids.size()==0
	bitset<subs> *bst;
	lgreennode *leftChild, *rightChild;

	lgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, lgreennode *lc,
			   lgreennode *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc) {}
};

struct rgreennode {
	bool isleaf = false;
	int l, h, nodeid, levelid, numNodeSub = 0;
	vector<int> subids;
	bitset<subs> *bst;
	rgreennode *leftChild, *rightChild;

	rgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, rgreennode *lc,
			   rgreennode *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc) {}
};

struct bluenode {
	bool isleaf = false;
	int l, h, nodeid, levelid, numNodeSub = 0;
	vector<int> subids;
	bitset<subs> *bst;
	lgreennode *leftGreenChild;
	rgreennode *rightGreenChild;
	bluenode *leftBlueChild, *rightBlueChild;

	bluenode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, lgreennode *lc,
			 rgreennode *rc, bluenode *lbc, bluenode *rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftGreenChild(lc),
		  rightGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {}
};

// Version1: can't expand node, height is fixed.
class BGTree {
private:
	int numSub;         // Number of inserted subscriptions
	int boundaryNumSub; // When the value of numSub in a node > boundaryNumSub, the memory of bitset is smaller than storing $numSub subscriptions directly
	int numNode;        // Number of all nodes
	int height;         // The height of BGTree with only one node is 1
	int initHeight;     // Initial height constructing the BGTree, >=1
	vector<bluenode *> roots;

	void initBlueNode(bluenode *&r);

	void initGreenNode(lgreennode *&r);

	void initGreenNode(rgreennode *&r);

	void releaseBlueNode(bluenode *&r);

	void releaseGreenNode(lgreennode *&r);

	void releaseGreenNode(rgreennode *&r);

	void insertIntoBlueNode(bluenode *&r, const int &subID, const int &l, const int &h);

	void insertIntoGreenNode(lgreennode *&r, const int &subID, const int &l);

	void insertIntoGreenNode(rgreennode *&r, const int &subID, const int &h);

	bitset<subs> *&vectorToBitset(vector<int> &v);

	vector<int> &bitsetToVector(bitset<subs> *&b);

	void vectorToBitset(vector<int> &v, bitset<subs> *&);

	void bitsetToVector(bitset<subs> *&b, vector<int> &);

public:
	BGTree();

	~BGTree();

	void insert(IntervalSub sub);

	void forward_match(const Pub &pub, int &matchSubs);

	void backward_match(const Pub &pub, int &matchSubs);

	int calMemory();      // 计算占用内存大小
	void printBGTree();
};

