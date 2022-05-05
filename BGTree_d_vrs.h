//
// Created by swh on 2022/2/20.
//

#ifndef MAC_BGTREE_D_VRS_H
#define MAC_BGTREE_D_VRS_H

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

struct lgreennode_d_vrs {
	int l, h, mid, nodeid, levelid;// numNodeSub = 0;
	vector<int> vSubids; // virtual sub id
	vector<int> rSubids; // real sub id
	vector<int> midv;  // for low value = mid + 1
	bitset<subs>* realBstPtr; // only for real attribute
	lgreennode_d_vrs *leftChild, *rightChild;
	lgreennode_d_vrs(int low, int high, int nid, int lid, bitset<subs>* bstp, lgreennode_d_vrs *lc,
				 lgreennode_d_vrs *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), realBstPtr(bstp), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct hgreennode_d_vrs {
	int l, h, mid, nodeid, levelid; //numNodeSub = 0;
	vector<int> vSubids; // virtual sub id
	vector<int> rSubids; // real sub id
	vector<int> midv;  // for high value = mid - 1
	bitset<subs>* realBstPtr; // only for real attribute
	hgreennode_d_vrs *leftChild, *rightChild;
	hgreennode_d_vrs(int low, int high, int nid, int lid, bitset<subs>* bstp, hgreennode_d_vrs *lc,
				 hgreennode_d_vrs *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), realBstPtr(bstp), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct bluenode_d_vrs {
	int l, h, mid, nodeid, levelid;
	vector<int> vSubids; // virtual sub id
	vector<int> rSubids; // real sub id
	vector<int> lMidv;  // for high value = mid
	vector<int> rMidv;  // for low value = mid + 1
	lgreennode_d_vrs *lowGreenChild;
	hgreennode_d_vrs *highGreenChild;
	bluenode_d_vrs *leftBlueChild, *rightBlueChild;
	bluenode_d_vrs(int low, int high, int nid, int lid, lgreennode_d_vrs *lc,
			   hgreennode_d_vrs *rc, bluenode_d_vrs*lbc, bluenode_d_vrs*rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), lowGreenChild(lc),
		  highGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {
		mid = (l + h) >> 1;
	}
};

// Version3: virtual&&real shared
// expand node (boundary), forward&backward, vector&bitset
class BGTree_d_vrs {
private:
	int numSub;         // Number of inserted subscriptions
	int numNode;        // Number of all nodes
	int height;         // The height of BGTree with only one node is 1
	int maxNodeSize;
//	int subPredicate[subs];
//	int counter[subs];
	vector<int> predicateNumOnAttr; // The number of predicate on each attribute
	vector<bluenode_d_vrs *> roots;
//	vector<bitset<subs>> nB; // null bitset for C-BOMP
//	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void releaseBlueNode(bluenode_d_vrs *&r);
	void releaseGreenNode(lgreennode_d_vrs *&r);
	void releaseGreenNode(hgreennode_d_vrs *&r);

	void calPredicateNum(const vector<IntervalSub> &subList); // calculate predicateNumOnAttr, preprocess
	int getVirtualAttrByMinFre(const IntervalSub &iSub); // return the virtual attribute with minimum frequency
	int getVirtualAttrByMinWidth(const IntervalSub &iSub); // return the virtual attribute with minimum probability

	void insertVAttrIntoBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h, const int& attrId, const vector<IntervalSub> &subList);
	void insertVAttrIntoGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l, const int& attrId, const vector<IntervalSub> &subList);
	void insertVAttrIntoGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h, const int& attrId, const vector<IntervalSub> &subList);
	void insertRAttrIntoBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h, const int& attrId, const vector<IntervalSub> &subList);
	void insertRAttrIntoGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l, const int& attrId, const vector<IntervalSub> &subList);
	void insertRAttrIntoGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h, const int& attrId, const vector<IntervalSub> &subList);

	bool deleteFromBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h);
	bool deleteFromGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l);
	bool deleteFromGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h);

	void vectorToBitset(vector<int> &v, bitset<subs> *&);
	void bitsetToVector(bitset<subs> *&b, vector<int> &);

	void forward_match_blueNode(bluenode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_lgreenNode(lgreennode_d_vrs *&l, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_hgreenNode(hgreennode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList);

	void
	forward_match_blueNode_C_BOMP(bluenode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								  bitset<subs> &mB);
	void forward_match_lgreenNode_C_BOMP(lgreennode_d_vrs *&l, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);
	void forward_match_hgreenNode_C_BOMP(hgreennode_d_vrs *&r, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);

	void
	backward_match_blueNode_C_BOMP(bluenode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								   bitset<subs> &mB);
	void backward_match_lgreenNode_C_BOMP(lgreennode_d_vrs *&l, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);
	void backward_match_hgreenNode_C_BOMP(hgreennode_d_vrs *&r, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);

	void backward_match_blueNode_native(bluenode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_lgreenNode_native(lgreennode_d_vrs *&l, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_hgreenNode_native(hgreennode_d_vrs *&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);

	double calBlueNodeMemory(bluenode_d_vrs *&r);
	double calLGreenNodeMemory(lgreennode_d_vrs *&r);
	double calRGreenNodeMemory(hgreennode_d_vrs *&r);

public:
	int hit = 0; // mid 命中次数
	int numProcessExactNode = 0, numProcessOneCmpNode = 0, numProcessTwoCmpNode = 0;
	int numProcessExactPredicate = 0, numProcessOneCmpPredicate = 0, numProcessTwoCmpPredicate = 0;
	int numEffectivePredicate=0,numOROperation;

	BGTree_d_vrs();

	~BGTree_d_vrs();

	void insert(IntervalSub sub, const vector<IntervalSub>& subList);

	bool deleteSubscription(IntervalSub sub);

	void forward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama6
	void forward_match_C_BOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama8
	void backward_match_C_BOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);

	void backward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);

	int calMemory();      // 计算占用内存大小

	void printBGTree();   // 找每一层的存储订阅数最大的节点

	int getHeight();

	int getNumNode() { return numNode; }

};

#endif //MAC_BGTREE_D_VRS_H

