#pragma once

#include<vector>
#include <cstring>
#include <algorithm>
#include "generator.h"
#include "chrono_time.h"
#include "Util.h"
#include "data_structure.h"
#include "constant.h"
#include <bitset>
#include <queue>

//#define DEBUG

struct greennode{
	int l, h, mid, nodeid, levelid;
	bitset<subs> *bst;
	vector<int> subids; // Either realBstPtr is nullptr or subids.size()==0
	greennode *leftChild, *rightChild; // 2个节点指针要么都是空的, 要么都是非空
	greennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, greennode *lc,
		greennode *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct lgreennode:public greennode {
	lgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, lgreennode *lc,
			   lgreennode *rc)
		: greennode(low,high,nid,lid,subIDs,bits,lc,rc) {
	}
};
struct hgreennode:public greennode  {
	hgreennode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, hgreennode *lc,
			   hgreennode *rc)
		:greennode(low,high,nid,lid,subIDs,bits,lc,rc) {
	}
};

struct bluenode {
	int l, h, mid, nodeid, levelid;
	bitset<subs> *bst;
	vector<int> subids;         // 对于内部节点, 只能用于反向标记
	vector<int> midEqual;       // Record subID with predicate including mid
	lgreennode *lowGreenChild; // 4个节点指针要么都是空的, 要么都是非空
	hgreennode *highGreenChild;
	bluenode *leftBlueChild, *rightBlueChild;

	bluenode(int low, int high, int nid, int lid, vector<int> subIDs, bitset<subs> *bits, lgreennode *lc,
			 hgreennode *rc, bluenode *lbc, bluenode *rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), lowGreenChild(lc),
		  highGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {
		mid = (l + h) >> 1;
	}
};

// Version1: can't expand node, height is fixed, only native forward and CBOMP(to backward), no bitset in nodes.
class BGTree {
private:
	int numSub;         // Number of inserted subscriptions
	int boundaryNumSub; // When the value of numSub in a node > boundaryNumSub, the memory of bitset is smaller than storing $numSub subscriptions directly
	int numNode;        // Number of all nodes
	int height;         // The height of BGTree with only one node is 1
	int initHeight;     // Initial height constructing the BGTree, >=1
	int subPredicate[subs];
	int counter[subs];
	vector<bluenode *> roots;
	vector<bitset<subs>> nB; // null bitset for C-BOMP
	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void initBlueNode(bluenode *&r);
	void initGreenNode(greennode *r);

	void releaseBlueNode(bluenode *&r);
	void releaseGreenNode(greennode *r);

	void insertIntoBlueNode(bluenode *&r, const int &subID, const int &l, const int &h);
	void insertIntoGreenNode(greennode *r, const int &subID, const int &v);

	bool deleteFromBlueNode(bluenode *&r, const int &subID, const int &l, const int &h);
	bool deleteFromGreenNode(greennode *r, const int &subID, const int &v);

	void vectorToBitset(vector<int> &v, bitset<subs> *&);
	void bitsetToVector(bitset<subs> *&b, vector<int> &);

	void match_forward_blueNode(bluenode *const& r, const int &att, const int &value, const vector<IntervalSub> &subList);
	void match_forward_lgreenNode(greennode *const& l, const int &att, const int &value, const vector<IntervalSub> &subList);
	void match_forward_hgreenNode(greennode *const& h, const int &att, const int &value, const vector<IntervalSub> &subList);

	void
	match_forward_CBOMP_blueNode(bluenode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								  bitset<subs> &mB);
	void match_forward_CBOMP_lgreenNode(greennode *const&l, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);
	void match_forward_CBOMP_hgreenNode(greennode *const&r, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);

	void
	match_backward_DMFT_fBGTree_CBOMP_blueNode(bluenode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,
		bitset<subs> &mB);
	void match_backward_DMFT_fBGTree_CBOMP_lgreenNode(greennode *const&l, const int &att, const int &value,
		const vector<IntervalSub> &subList, bitset<subs> &mB);
	void match_backward_DMFT_fBGTree_CBOMP_hgreenNode(greennode *const&r, const int &att, const int &value,
		const vector<IntervalSub> &subList, bitset<subs> &mB);

	void match_backward_blueNode(bluenode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	match_backward_lgreenNode(greennode *const&l, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	match_backward_hgreenNode(greennode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);

	void match_backward_DMFT_bBGTree_blueNode(bluenode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	match_backward_DMFT_bBGTree_lgreenNode(greennode *const&l, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	match_backward_DMFT_bBGTree_hgreenNode(greennode *const&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);

	double calBlueNodeMemory(const bluenode *const&r);
	double calGreenNodeMemory(greennode *const&r);

public:
	int hit = 0; // mid 命中次数
	int numProcessExactNode = 0, numProcessOneCmpNode = 0, numProcessTwoCmpNode = 0;
	int numProcessExactPredicate = 0, numProcessOneCmpPredicate = 0, numProcessTwoCmpPredicate = 0;
	int numEffectivePredicate=0;

	BGTree();
	~BGTree();

	void insert(IntervalSub sub);
	bool deleteSubscription(IntervalSub sub);

	void match_forward_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList); // fBG-Tree
	void match_forward_CBOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList); // fBG-Tree-C, Similar to bTama6
	void match_backward_DMFT_fBGTree_CBOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList); // fBG-Tree-CB, Similar to bTama8

	void match_backward_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList); // bBG-Tree
	void match_forward_DMFT_bBGTree(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList); // bBG-Tree-F

	int calMemory();      // 计算占用内存大小

	void printBGTree();   // 找每一层的存储订阅数最大的节点 

	int getHeight() { return height; }

	int getBoundaryNumSub() { return boundaryNumSub; }

	int getNumNode() { return numNode; }

};

