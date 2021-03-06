#ifndef _AWBTree_H
#define _AWBTree_H

#include <cstring>
#include "Util.h"

using namespace std;

using Lowtree = BPlusTree::Tree<int, lowTreeValue>;
using Hightree = BPlusTree::Tree<int, int>;

//const int MAX_WIDTH_CELL = 1001;
//const int MAX_DIM = 30001;

class AWBTree {
private:
	//Lowtree* lTree[MAX_DIM][MAX_WIDTH_CELL];
	//Hightree* hTree[MAX_DIM][MAX_WIDTH_CELL];
	vector<vector< Lowtree*>>lTree;
	vector<vector< Hightree*>>hTree;

	//bool sigs[MAX_WIDTH_CELL];
	vector<bool> sigs;
	bool bitsets[subs];
	unsigned short counter[subs];
	unsigned short PDR[subs];
	//int counter[MAX_SUB];
	//int PDR[MAX_SUB];
	//vector<int> counter;

	int check, recheck;

	int valDom;
	//total dimension
	int dim;
	//width size
	double Wsize;
	//width cell size
	unsigned short WCsize;
	// patition point
	float Ppoint;
	size_t calMemoryInAttri(int i);
public:
	AWBTree() :
		dim(atts), valDom(valDom), WCsize(WCsize), Ppoint(Ppoint), Wsize((double)valDom / (double)WCsize)
	{
		sigs.resize(WCsize);
		for (int i = 0; i < subs; i++) {
			bitsets[i] = false;
			counter[i] = PDR[i] = 0;
		}
		lTree.resize(atts, vector<Lowtree*>(WCsize));
		hTree.resize(atts, vector<Hightree*>(WCsize));
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < WCsize; j++) {
				sigs[j] = false;
				lTree[i][j] = new Lowtree(awbTree_branch);
				hTree[i][j] = new Hightree(awbTree_branch);
			}
		}
	}

	void insert(IntervalSub sub);
	bool deleteSubscription(IntervalSub sub);

	void forward(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const int widthId);
	void forward_opt(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const int& widthId);
	void forward_a(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const int widthId);
	void backward(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const int widthId);
	void backward_opt(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const int widthId);

	//void hybrid(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const float awbTree_Ppoint);
	void hybrid_opt(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const float& Ppoint);
	void hybrid_a(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList, const float& Ppoint);
	int calMemory();
};
#endif // !_AWBTree
