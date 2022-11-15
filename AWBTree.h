#ifndef _AWBTree_H
#define _AWBTree_H

#include <cstring>
#include "Util.h"
#include "ThreadPool.h"
using namespace std;

//using lbptree = BplusTree::bplus_tree<int, lowTreeEle>;
//using lbptree_node = BplusTree::Node<int, lowTreeEle>;
//using ltree_iter = BplusTree::bplus_tree<int, lowTreeEle>::iterator;
//using hbptree = BplusTree::bplus_tree<int, int>;
//using hbptree_node = BplusTree::Node<int, int>;
//using htree_iter = BplusTree::bplus_tree<int, int>::iterator;
using lbptree = BplusTree::Tree<int, lowTreeEle>;
using lbptree_node = BplusTree::Node<int, lowTreeEle>;
using ltree_iter = BplusTree::Tree<int, lowTreeEle>::iterator;
using hbptree = BplusTree::Tree<int, int>;
using hbptree_node = BplusTree::Node<int, int>;
using htree_iter = BplusTree::Tree<int, int>::iterator;

class AWBTree
{
 private:
	vector<vector<lbptree*>> ltree;
	vector<vector<hbptree*>> htree;

	uint32_t dim;  // 维度
	uint16_t width_size; // 宽度单元数目
	uint16_t branch; // btree最大分支数
	vector<uint16_t> pdr;  // 记录正向匹配需要匹配的谓词数目
	vector<vector<bool>> sig;
	vector<uint16_t> counter;
	//vector<atomic_uint16_t> atomic_counter;
	bitset<subs> bit_forward;
	ThreadPool pool;
	double width;  // 宽度单元大小
	uint16_t dPoint; // 任务切分点

 public:
	vector<IntervalSub> subList;
	vector<Pub> pubList;

 public:
	AWBTree(int type)
		: dim(atts), width_size(WCsize), branch(awbTree_branch), width(
		valDom / WCsize), sig(atts, vector<bool>(WCsize, 0)), counter(subs, 0), dPoint(
		awbTree_Ppoint * WCsize), pdr(subs, 0)
	{
		ltree.resize(dim);
		htree.resize(dim);
		for (int i = 0; i < dim; i++)
		{
			ltree[i].resize(width_size);
			htree[i].resize(width_size);
			for (int j = 0; j < width_size; j++)
			{
				//ltree[i][j] = new lbptree(branch, new lbptree_node);
				//htree[i][j] = new hbptree(branch, new hbptree_node);
				ltree[i][j] = new lbptree(branch);
				htree[i][j] = new hbptree(branch);
			}
		}
		if (type == AWBPTREE_PARALLEL)
			pool.initThreadPool(parallelDegree);
	}

	bool readSubs(string file);
	bool readPus(string file);

	void insert(IntervalSub sub);
	void setbits();
	void erase(IntervalSub sub);

	void forward(const Pub& pub, int& matchSubs);
	void forward_o(const Pub& pub, int& matchSubs);
	void forward_p(const Pub& pub, int& matchSubs);
	void backward(const Pub& pub, int& matchSubs);
	void backward_o(const Pub& pub, int& matchSubs);
	void backward_p(const Pub& pub, int& matchSubs);
	void hybrid(const Pub& pub, int& matchSubs);
	void hybrid_a(const Pub& pub, int& matchSubs);
	void hybrid_p(const Pub& pub, int& matchSubs);

	size_t calMemory();
	bool check(vector<uint32_t>& matchSubList);
};
#endif // !_AWBTree
