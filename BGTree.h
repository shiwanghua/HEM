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

struct greennode
{
	uint32_t l, h, mid, nodeid, levelid;
	bitset<subs>* bst;
	vector<uint32_t> subids; // Either realBstPtr is nullptr or subids.size()==0
	greennode* leftChild, * rightChild; // 2个节点指针要么都是空的, 要么都是非空
	greennode(uint32_t low, uint32_t high, uint32_t nid, uint32_t lid, vector<uint32_t> subIDs, bitset<subs>* bits, greennode* lc,
		greennode* rc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), leftChild(lc), rightChild(rc)
	{
		mid = (l + h) >> 1;
	}
};
struct lgreennode : public greennode
{
	lgreennode(uint32_t low, uint32_t high, uint32_t nid, uint32_t lid, vector<uint32_t> subIDs, bitset<subs>* bits, lgreennode* lc,
		lgreennode* rc)
		: greennode(low, high, nid, lid, subIDs, bits, lc, rc)
	{
	}
};
struct hgreennode : public greennode
{
	hgreennode(uint32_t low, uint32_t high, uint32_t nid, uint32_t lid, vector<uint32_t> subIDs, bitset<subs>* bits, hgreennode* lc,
		hgreennode* rc)
		: greennode(low, high, nid, lid, subIDs, bits, lc, rc)
	{
	}
};

struct bluenode
{
	uint32_t l, h, mid, nodeid, levelid; // l,h,nodeid,levelid 其实不需要存储
	bitset<subs>* bst;
	vector<uint32_t> subids;         // 对于内部节点, 只能用于反向标记
	vector<uint32_t> midEqual;       // Record subID with predicate including mid
	lgreennode* lowGreenChild; // 4个节点指针要么都是空的, 要么都是非空
	hgreennode* highGreenChild;
	bluenode* leftBlueChild, * rightBlueChild;

	bluenode(uint32_t low, uint32_t high, uint32_t nid, uint32_t lid, vector<uint32_t> subIDs, bitset<subs>* bits, lgreennode* lc,
		hgreennode* rc, bluenode* lbc, bluenode* rbc)
		: l(low), h(high), nodeid(nid), levelid(lid), subids(subIDs), bst(bits), lowGreenChild(lc),
		  highGreenChild(rc), leftBlueChild(lbc), rightBlueChild(rbc)
	{
		mid = (l + h) >> 1;
	}
};

// Version1: can't expand node, height is fixed, only native forward and CBOMP(to backward), no bitset in nodes.
class BGTree
{
 private:
	uint32_t numSub;         // Number of inserted subscriptions
	uint32_t boundaryNumSub; // When the value of numSub in a node > boundaryNumSub, the memory of bitset is smaller than storing $numSub subscriptions directly
	uint32_t numNode;        // Number of all nodes
	uint32_t height;         // The height of BGTree with only one node is 1
	uint32_t initHeight;     // Initial height constructing the BGTree, >=1
	uint32_t subPredicate[subs];
	uint32_t counter[subs];
	vector<bluenode*> roots;
	vector<bitset<subs>> nB; // null bitset for C-BOMP
	vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

	void initBlueNode(bluenode*& r);
	void initGreenNode(greennode* r);

	void releaseBlueNode(bluenode*& r);
	void releaseGreenNode(greennode* r);

	void insertIntoBlueNode(bluenode* const& r, const int& subID, const int& l, const int& h);
	void insertIntoGreenNode(greennode* const& r, const int& subID, const int& v);

	bool deleteFromBlueNode(bluenode*& r, const int& subID, const int& l, const int& h);
	bool deleteFromGreenNode(greennode* r, const int& subID, const int& v);

	void vectorToBitset(vector<int>& v, bitset<subs>*&);
	void bitsetToVector(bitset<subs>*& b, vector<int>&);

	void
	match_forward_blueNode(const bluenode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList);
	void
	match_forward_lgreenNode(const greennode* const& l, const int& att, const int& value, const vector<IntervalSub>& subList);
	void
	match_forward_hgreenNode(const greennode* const& h, const int& att, const int& value, const vector<IntervalSub>& subList);

	void
	match_forward_CBOMP_blueNode(const bluenode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList,
		bitset<subs>& mB);
	void match_forward_CBOMP_lgreenNode(const greennode* const& l, const int& att, const int& value,
		const vector<IntervalSub>& subList, bitset<subs>& mB);
	void match_forward_CBOMP_hgreenNode(const greennode* const& r, const int& att, const int& value,
		const vector<IntervalSub>& subList, bitset<subs>& mB);

	void
	match_backward_DMFT_fBGTree_CBOMP_blueNode(const bluenode* const& r, const int& att, const int& value, const vector<
		IntervalSub>& subList,
		bitset<subs>& mB);
	void match_backward_DMFT_fBGTree_CBOMP_lgreenNode(const greennode* const& l, const int& att, const int& value,
		const vector<IntervalSub>& subList, bitset<subs>& mB);
	void match_backward_DMFT_fBGTree_CBOMP_hgreenNode(const greennode* const& r, const int& att, const int& value,
		const vector<IntervalSub>& subList, bitset<subs>& mB);

	void
	match_backward_blueNode(const bluenode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
		subs>& gB);
	void
	match_backward_lgreenNode(const greennode* const& l, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
		subs>& gB);
	void
	match_backward_hgreenNode(const greennode* const& r, const int& att, const int& value, const vector<IntervalSub>& subList, bitset<
		subs>& gB);

	void match_forward_DMFT_bBGTree_blueNode(const bluenode* const& r, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);
	void
	match_forward_DMFT_bBGTree_lgreenNode(const greennode* const& l, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);
	void
	match_forward_DMFT_bBGTree_hgreenNode(const greennode* const& h, const int& att, const int& value, const vector<
		IntervalSub>& subList, bitset<subs>& gB);

	double calBlueNodeMemory(const bluenode* const& r) const;
	double calGreenNodeMemory(const greennode* const& r) const;

 public:
	uint32_t hit = 0; // mid 命中次数
	uint32_t numProcessExactNode = 0, numProcessOneCmpNode = 0, numProcessTwoCmpNode = 0;
	uint32_t numProcessExactPredicate = 0, numProcessOneCmpPredicate = 0, numProcessTwoCmpPredicate = 0;
	uint32_t numEffectivePredicate = 0, numBitsetOperation = 0;

	BGTree();
	~BGTree();

	void insert(IntervalSub sub);
	bool deleteSubscription(IntervalSub sub);

	void match_forward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // fBG-Tree
	void
	match_forward_CBOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // fBG-Tree-C, Similar to bTama6
	void
	match_backward_DMFT_fBGTree_CBOMP(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // fBG-Tree-CB, Similar to bTama8

	void match_backward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // bBG-Tree
	void match_forward_DMFT_bBGTree(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList); // bBG-Tree-F

	uint32_t calMemory() const;      // 计算占用内存大小

	void printBGTree() const;   // 找每一层的存储订阅数最大的节点

	uint32_t getHeight() const
	{
		return height;
	}

	uint32_t getBoundaryNumSub() const
	{
		return boundaryNumSub;
	}

	uint32_t getNumNode() const
	{
		return numNode;
	}

};

