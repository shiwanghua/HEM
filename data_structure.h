#ifndef _DATA_STRUCTURE_H
#define _DATA_STRUCTURE_H
//#include<string>
#include<vector>
#include<unordered_map>
#include "BloomFilter.h"

using namespace std;

struct Cnt {
    int att;
    int value;
    int op;    //  op==0 -> "="  op==1 -> ">="  op==2 -> "<="
};

// For PS-Tree
struct Cnt2{
    int att;
    int value[2]; // if op==3 -->{ v[0] -> "low value"  v[1] -> "high value" }
    int op;     //  op==0 -> "="  op==1 -> ">="  op==2 -> "<=" op==3 -> "in"
};

struct IntervalCnt{
    int att;
    int lowValue, highValue;
};

struct Sub {
    int id;
    int size; 								//number of predicates
    vector<Cnt> constraints;				//list of constraints
};

// For PS-Tree
struct Sub2 {
    int id;
    int size; 								//number of predicates
    vector<Cnt2> constraints;				//list of constraints
};

struct IntervalSub{
    int id;
    int size;
    vector<IntervalCnt> constraints; // constraints的size不就是上面的size吗
};

struct ConElement {
    int att;
    int val;
    int subID;
};

struct Combo{
    int val;
    int subID;
};

struct IntervalCombo{
    int lowValue, highValue;
    int subID;
};

struct Pair{
    int att;
    int value;
};

struct Pub{
    int size,id;
    vector<Pair> pairs;// pairs的size不就是上面的size吗
};

struct dPub { // dimensional publish
    int pubId;
    unordered_map<int,int> AVmap;
};

struct attAndCount { // AdaRein
    int att, count;
    bool operator < (const attAndCount& b) const {
        return  count < b.count;
    }
};

// PS-Tree
#define PSTREE_MAX_LENGTH 16
typedef struct LeafNode
{
    //int counter;
    //int spaceId[2];
    LeafNode* next;
    BloomFilter signature;
    vector<int> subLinkedList; //record sub
}LeafNode, * LeafNodeLink;

typedef struct InnerNode
{
    InnerNode(int len) {
        l = e = g = NULL;
        length = len;
        for (int i = 0; i < length; i++)p[i] = NULL;
    }
    int length;
    LeafNodeLink l;
    LeafNodeLink e;
    LeafNodeLink g;
    InnerNode* p[PSTREE_MAX_LENGTH];
}InnerNode, * InnerNodeLink;


// AWB+Tree
struct lowTreeEle
{
	int highVal;
	int subID;
	bool operator!=(const lowTreeEle b) const
	{
		return this->subID != b.subID;
	}
};

#endif //_DATA_STRUCTURE_H
