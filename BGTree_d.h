//
// Created by swh on 2022/1/12.
//

#ifndef MAC_BGTREE_d_H
#define MAC_BGTREE_d_H

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

struct lgreennode_d {
	int32_t l, h, mid, nodeid, levelid;// numNodeSub = 0;
	vector<int32_t> subids;
	vector<int32_t> midv;
	bitset<subs>* bst;
	lgreennode_d *leftChild, *rightChild;
	lgreennode_d(int32_t low, int32_t high, int32_t nid, int32_t lid, bitset<subs>* bstp, lgreennode_d *lc,
			   lgreennode_d *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), bst(bstp), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct hgreennode_d {
	int32_t l, h, mid, nodeid, levelid; //numNodeSub = 0;
	vector<int32_t> subids;
	vector<int32_t> midv;
	bitset<subs>* bst;
	hgreennode_d *leftChild, *rightChild;
	hgreennode_d(int32_t low, int32_t high, int32_t nid, int32_t lid, bitset<subs>* bstp, hgreennode_d *lc,
			   hgreennode_d *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), bst(bstp), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct bluenode_d {
	int32_t l, h, mid, nodeid, levelid;
	vector<int32_t> subids; // 对于内部节点, 只能用于反向标记
	vector<int32_t> lMidv;
	vector<int32_t> rMidv;
	bitset<subs>* bst;
	lgreennode_d *lowGreenChild;
	hgreennode_d *highGreenChild;
	bluenode_d *leftBlueChild, *rightBlueChild;
	bluenode_d(int32_t low, int32_t high, int32_t nid, int32_t lid, bitset<subs>* bstp, lgreennode_d *lc,
			 hgreennode_d *rc, bluenode_d*lbc, bluenode_d*rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), bst(bstp), lowGreenChild(lc),
		  highGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {
		mid = (l + h) >> 1;
	}
};

// Version2: expand node (BoundaryNumSub), forward&backward, vector&bitset
class BGTree_d {
private:
	int32_t numSub;         // Number of inserted subscriptions
	int32_t numNode;        // Number of all nodes
	int32_t height;         // The height of BGTree with only one node is 1
	int32_t maxNodeSize;
	int32_t subPredicate[subs];
	int32_t counter[subs];
	vector<bluenode_d *> roots;
	vector<bitset<subs>> nB; // null bitset for C-BOMP
	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void releaseBlueNode(bluenode_d *&r);
	void releaseGreenNode(lgreennode_d *&r);
	void releaseGreenNode(hgreennode_d *&r);

	void insertIntoBlueNode(bluenode_d *&r, const int &subID, const int &l, const int &h,const int& attrId, const vector<IntervalSub> &subList);
	void insertIntoGreenNode(lgreennode_d *&r, const int &subID, const int &l,const int& attrId, const vector<IntervalSub> &subList);
	void insertIntoGreenNode(hgreennode_d *&r, const int &subID, const int &h,const int& attrId, const vector<IntervalSub> &subList);

	bool deleteFromBlueNode(bluenode_d *&r, const int &subID, const int &l, const int &h);
	bool deleteFromGreenNode(lgreennode_d *&r, const int &subID, const int &l);
	bool deleteFromGreenNode(hgreennode_d *&r, const int &subID, const int &h);

	void vectorToBitset(vector<int> &v, bitset<subs> *&);
	void bitsetToVector(bitset<subs> *&b, vector<int> &);

	void forward_match_blueNode(bluenode_d *&r, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_lgreenNode(lgreennode_d *&l, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_hgreenNode(hgreennode_d *&h, const int &att, const int &value, const vector<IntervalSub> &subList);

	void
	forward_match_blueNode_C_BOMP(bluenode_d *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								  bitset<subs> &mB);
	void forward_match_lgreenNode_C_BOMP(lgreennode_d *&l, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);
	void forward_match_hgreenNode_C_BOMP(hgreennode_d *&r, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);

	void
	backward_match_DMFT_fBGTree_d_CBOMP_blueNode(bluenode_d *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								   bitset<subs> &mB);
	void backward_match_DMFT_fBGTree_d_CBOMP_lgreenNode(lgreennode_d *&l, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);
	void backward_match_DMFT_fBGTree_d_CBOMP_hgreenNode(hgreennode_d *&r, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);

	void backward_match_blueNode_native(bluenode_d *&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_lgreenNode_native(lgreennode_d *&l, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_hgreenNode_native(hgreennode_d *&h, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);

	void forward_match_DMFT_bBGTree_d_blueNode(const bluenode_d* const& r, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);
	void
	forward_match_DMFT_bBGTree_d_lgreenNode(const lgreennode_d* const& l, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);
	void
	forward_match_DMFT_bBGTree_d_hgreenNode(const hgreennode_d* const& h, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);

	double calBlueNodeMemory(bluenode_d *&r);
	double calLGreenNodeMemory(lgreennode_d *&r);
	double calRGreenNodeMemory(hgreennode_d *&r);

public:
	int32_t hit = 0; // mid 命中次数
	int32_t numProcessExactNode = 0, numProcessOneCmpNode = 0, numProcessTwoCmpNode = 0;
	int32_t numProcessExactPredicate = 0, numProcessOneCmpPredicate = 0, numProcessTwoCmpPredicate = 0;
	int32_t numEffectivePredicate=0,numBitsetOperation;

	BGTree_d();

	~BGTree_d();

	void insert(IntervalSub sub, const vector<IntervalSub>& subList);

	bool deleteSubscription(IntervalSub sub);

	void forward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama6
	void forward_match_C_BOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama8
	void backward_match_DMFT_fBGTree_d_CBOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);

	void backward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	void forward_match_DMFT_bBGTree_d(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // bBG-Tree_d-F

	int32_t calMemory();      // 计算占用内存大小

	void printBGTree();   // 找每一层的存储订阅数最大的节点

	int32_t getHeight();

	int32_t getNumNode() { return numNode; }

};


#endif //MAC_BGTREE_d_H
