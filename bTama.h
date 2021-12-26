#ifndef bTAMA_H
#define bTAMA_H

#include "util.h"
#include "constant.h"
#include <bitset>

using namespace std;

//const int MAX_ATTS = 101;

class bTama {
	int nodeCounter;
	int numSub; // Sub inserted
	int* lchild, * rchild, * mid;
	//    vector<int>* data[MAX_ATTS];
	vector<vector<vector<int>>> data;
	//int counter[subs];
	vector<bitset<subs>> nB; // null bitset
	vector<bitset<subs>> nnB; // non-null bitset, same as HEM

	void initiate(int p, int l, int r, int lvl);

	int median(int l, int r);

	void insert(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	bool deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	void forward_match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList,bitset<subs>& mB);

	void backward2_match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<subs>& mB);

	//void match_vague(int p, int att, int l, int r, const int value, int lvl);

public:
	bTama() {
		//		printf("In tama ()\n");
		//		fflush(stdout);
		int nodeNumber = 1 << level;
		numSub = 0;
		//        for (int i = 0; i < atts; i++)
		//            data[i] = new vector<int>[nodeNumber];
		data.resize(atts, vector<vector<int>>(nodeNumber));
		lchild = new int[nodeNumber];
		rchild = new int[nodeNumber];
		mid = new int[nodeNumber];
		nodeCounter = 0;
		initiate(0, 0, valDom - 1, 1);
		nB.resize(atts);
		nnB.resize(atts);
		cout << "ExpID = " << expID << ". bTama: level = " << level << "\n";
	}

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	// bTAMA6 C-BOMP
	void forward_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// bTAMA7
	void backward1_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// bTAMA8
	void backward2_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	//void match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	int calMemory();
};

#endif //TAMA_H
