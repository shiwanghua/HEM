#pragma once
#ifndef _PSTREE_H
#define _PSTREE_H
#include "util.h"
#include "constant.h"

using namespace std;

class PSTree {
private:
    vector<InnerNodeLink> pstrees;
    int attNum;
    int valDomain;
    int psTree_level;
    int movebits[10];
    int LENGTHS[10];
    vector<int> subAccPred;
public:
    PSTree() :attNum(atts), valDomain(valDom), psTree_level(PSTree_level)
    {
        pstrees.resize(attNum);
        for (auto& root : pstrees) {
            root = new InnerNode(2);
            LeafNodeLink tmp = new LeafNode;
            tmp->next = NULL;
            //tmp->counter = 0;
            //tmp->spaceId[0] = 0;
            //tmp->spaceId[1] = valDom;
            root->l = root->g = tmp;
        }
        for (int i = 0; i < psTree_level - 1; i++)
            movebits[i] = i * 4;
        movebits[psTree_level - 1] = movebits[psTree_level - 2] + 3;
        LENGTHS[0] = 2;
        LENGTHS[1] = 8;
        for (int i = 2; i <= psTree_level; i++)
            LENGTHS[i] = PSTREE_MAX_LENGTH;
        //memSum = 0;
        subAccPred.resize(subs);
    }

    void InsertPredicate(const Cnt2& pred, InnerNodeLink pstree, LeafNodeLink& startNode, LeafNodeLink& endNode);
    LeafNodeLink Partition(const int& val, const int& op, InnerNodeLink pstreeRoot);
    LeafNodeLink MatchPair(const Pair& p, InnerNodeLink pstreeRoot);
    //InnerNodeLink CreateInnerNode();
    InnerNodeLink GetRNode(vector<pair<InnerNodeLink, int>>& path);
    InnerNodeLink GetLNode(const InnerNodeLink iRNode, const InnerNodeLink pstreeRoot);
    void PartitionValue(const int& val, vector<int>& elems);
    void PartitionLeafNode(InnerNodeLink currNode, InnerNodeLink iRNode, const int& op, const int& val);
    void PartitionLeafNode(InnerNodeLink currNode, const int& op, const int& val);
    void insert(Sub2& sub);
    void MatchEvent(const Pub& pub, int& matchSubs, const vector<Sub2>& subList);
    bool Match(const Pub& pub, const Sub2& sub);
    int SelectAccPred(Sub2& sub);
    int calMemory();
    size_t mem_size(InnerNodeLink node);
    size_t mem_size(LeafNodeLink node);
};

#endif


