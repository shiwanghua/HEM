//
// Created by swh on 2022/1/12.
//

#ifndef MAC_BGTREE1_H
#define MAC_BGTREE1_H

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
	int l, h, mid, nodeid, levelid;// numNodeSub = 0;
	bitset<subs> bst;
	vector<int> subids;
	vector<int> midv;
	lgreennode *leftChild, *rightChild;
	lgreennode(int low, int high, int nid, int lid, lgreennode *lc,
			   lgreennode *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};

struct hgreennode {
	int l, h, mid, nodeid, levelid; //numNodeSub = 0;
	bitset<subs> bst;
	vector<int> subids;
	vector<int> midv;
	hgreennode *leftChild, *rightChild;
	hgreennode(int low, int high, int nid, int lid, hgreennode *lc,
			   hgreennode *rc)
		: l(low), h(high), nodeid(nid), levelid(lid), leftChild(lc), rightChild(rc) {
		mid = (l + h) >> 1;
	}
};
struct bluenode {
	int l, h, mid, nodeid, levelid, numNodeSub = 0;
	bitset<subs> bst;
	vector<int> subids; // 对于内部节点, 只能用于反向标记
	vector<int> lMidv;
	vector<int> rMidv;
	lgreennode *lowGreenChild;
	hgreennode *highGreenChild;
	bluenode *leftBlueChild, *rightBlueChild;
	bluenode(int low, int high, int nid, int lid, lgreennode *lc,
			 hgreennode *rc, bluenode *lbc, bluenode *rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), lowGreenChild(lc),
		  highGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc) {
		mid = (l + h) >> 1;
	}
};

// Version2: expand node, forward&backward, vector&bitset
class BGTree1 {
private:
	int numSub;         // Number of inserted subscriptions
	int numNode;        // Number of all nodes
	int height;         // The height of BGTree with only one node is 1
	int maxNodeSize;
	int subPredicate[subs];
	int counter[subs];
	vector<bluenode *> roots;
	vector<bitset<subs>> nB; // null bitset for C-BOMP
	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void initBlueNode(bluenode *&r);
	void initGreenNode(lgreennode *&r);
	void initGreenNode(hgreennode *&r);

	void releaseBlueNode(bluenode *&r);
	void releaseGreenNode(lgreennode *&r);
	void releaseGreenNode(hgreennode *&r);

	void insertIntoBlueNode(bluenode *&r, const int &subID, const int &l, const int &h,const int& attrId, const vector<IntervalSub> &subList);
	void insertIntoGreenNode(lgreennode *&r, const int &subID, const int &l,const int& attrId, const vector<IntervalSub> &subList);
	void insertIntoGreenNode(hgreennode *&r, const int &subID, const int &h,const int& attrId, const vector<IntervalSub> &subList);

	bool deleteFromBlueNode(bluenode *&r, const int &subID, const int &l, const int &h);
	bool deleteFromGreenNode(lgreennode *&r, const int &subID, const int &l);
	bool deleteFromGreenNode(hgreennode *&r, const int &subID, const int &h);

	void vectorToBitset(vector<int> &v, bitset<subs> *&);
	void bitsetToVector(bitset<subs> *&b, vector<int> &);

	void forward_match_blueNode(bluenode *&r, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_lgreenNode(lgreennode *&l, const int &att, const int &value, const vector<IntervalSub> &subList);
	void forward_match_rgreenNode(hgreennode *&r, const int &att, const int &value, const vector<IntervalSub> &subList);

	void
	forward_match_blueNode_C_BOMP(bluenode *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								  bitset<subs> &mB);
	void forward_match_lgreenNode_C_BOMP(lgreennode *&l, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);
	void forward_match_rgreenNode_C_BOMP(hgreennode *&r, const int &att, const int &value,
										 const vector<IntervalSub> &subList, bitset<subs> &mB);

	void
	backward_match_blueNode_C_BOMP(bluenode *&r, const int &att, const int &value, const vector<IntervalSub> &subList,
								   bitset<subs> &mB);
	void backward_match_lgreenNode_C_BOMP(lgreennode *&l, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);
	void backward_match_rgreenNode_C_BOMP(hgreennode *&r, const int &att, const int &value,
										  const vector<IntervalSub> &subList, bitset<subs> &mB);

	void backward_match_blueNode_native(bluenode *&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_lgreenNode_native(lgreennode *&l, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);
	void
	backward_match_rgreenNode_native(hgreennode *&r, const int &att, const int &value, const vector<IntervalSub> &subList,bitset<subs> &mB);

	double calBlueNodeMemory(bluenode *&r);
	double calLGreenNodeMemory(lgreennode *&r);
	double calRGreenNodeMemory(hgreennode *&r);

public:
	int hit = 0; // mid 命中次数
	int numProcessExactNode = 0, numProcessOneCmpNode = 0, numProcessTwoCmpNode = 0;
	int numProcessExactPredicate = 0, numProcessOneCmpPredicate = 0, numProcessTwoCmpPredicate = 0;
	int numEffectivePredicate=0;

	BGTree1();

	~BGTree1();

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	void forward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama6
	void forward_match_C_BOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
	// Similar to bTama8
	void backward_match_C_BOMP(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);

	void backward_match_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);

	int calMemory();      // 计算占用内存大小

	void printBGTree();   // 找每一层的存储订阅数最大的节点

	int getHeight() { return height; }

	int getNumNode() { return numNode; }

};


#endif //MAC_BGTREE1_H
