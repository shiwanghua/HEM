#include "AWBTree.h"
#include <math.h>
//mutex m;
//atomic_flag flag = ATOMIC_FLAG_INIT;

bool AWBTree::readSubs(string file)
{
	ifstream infile(file, ios::in);
	if (!infile)
	{
		cout << "error opening source file." << endl;
		return false;
	}
	double tmp;
	for (int i = 0; i < 9; ++i)infile >> tmp;
	subList.resize(subs);
	for (auto& sub : subList)
	{
		infile >> sub.id;
		infile >> sub.size;
		sub.constraints.resize(sub.size);
		for (auto& pred : sub.constraints)
		{
			infile >> pred.att;
			infile >> pred.lowValue;
			infile >> pred.highValue;
		}
	}
	infile.close();
	return true;
}

bool AWBTree::readPus(string file)
{
	ifstream infile(file, ios::in);
	if (!infile)
	{
		cout << "error opening source file." << endl;
		return false;
	}
	double tmp;
	for (int i = 0; i < 8; ++i)infile >> tmp;
	pubList.resize(pubs);
	for (auto& pub : pubList)
	{
		infile >> pub.size;
		pub.pairs.resize(pub.size);
		for (auto& pred : pub.pairs)
		{
			infile >> pred.att;
			infile >> pred.value;
		}
	}
	infile.close();
	return true;
}

void AWBTree::insert(IntervalSub sub)
{
	int id = sub.id;
	counter[id] = sub.size;
	//atomic_counter[id] = sub.size;
	for (auto pred : sub.constraints)
	{
		int pred_att = pred.att;
		int lowVal = pred.lowValue;
		int highVal = pred.highValue;
		//Element ele = { lowVal, highVal, id };
		lowTreeEle ele = { highVal, id };
		uint16_t w_id = (highVal - lowVal) / width;
		if (!(w_id > dPoint))++pdr[id];
		sig[pred_att][w_id] = true;
		ltree[pred.att][w_id]->insert(lowVal, ele);
		htree[pred.att][w_id]->insert(highVal, id);
	}
}

void AWBTree::setbits()
{
	for (int i = 0; i < pdr.size(); i++)
		if (pdr[i] == 0)bit_forward.set(i);
}

void AWBTree::forward(const Pub& pub, int& matchSubs)
{
	vector<uint16_t> c(counter);
	bitset<subs> bits;
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		double val = pred.value;
		for (int i = 0; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				double w_min = i * width;
				ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
				ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
				ltree_iter end = ltree[pred_att][i]->upper_bound(val);
				for (; it != sPoint; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(val > ele.highVal))
						if (!(--c[ele.subID]))bits[ele.subID] = true;
				}
				for (; it != end; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(--c[ele.subID]))bits[ele.subID] = true;
				}
			}
		}
	}
	matchSubs = bits.count();
}

void AWBTree::forward_o(const Pub& pub, int& matchSubs)
{
	vector<uint16_t> c(counter);
	bitset<subs> bits;
	Timer start;
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		int val = pred.value;
		int i = 0;
		for (; i < width_size >> 1; ++i)
		{
			if (sig[pred_att][i])
			{
				double w_min = i * width;
				ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
				ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
				ltree_iter end =
					val < (valDom - w_min) ? ltree[pred_att][i]->upper_bound(val) : ltree[pred_att][i]->end();
				for (; it != sPoint; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(val > ele.highVal))
						if (!(--c[ele.subID]))bits[ele.subID] = true;
				}
				for (; it != end; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(--c[ele.subID]))bits[ele.subID] = true;
				}
			}
		}
		for (; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				double w_min = i * width;
				double w_max = w_min + width;
				double v_max = valDom - w_min;
				if (val > w_min)
				{
					ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
					ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
					ltree_iter end = ltree[pred_att][i]->end();
					for (; it != sPoint; ++it)
					{
						lowTreeEle ele = it.get_val();
						if (!(val > ele.highVal))
							if (!(--c[ele.subID]))bits[ele.subID] = true;
					}
					for (; it != end; ++it)
					{
						lowTreeEle ele = it.get_val();
						if (!(--c[ele.subID]))bits[ele.subID] = true;
					}
				}
				else if (val > v_max)
				{
					ltree_iter it = ltree[pred_att][i]->begin();
					ltree_iter end = ltree[pred_att][i]->end();
					for (; it != end; ++it)
					{
						lowTreeEle ele = it.get_val();
						if (!(--c[ele.subID]))bits[ele.subID] = true;
					}
				}
				else
				{
					ltree_iter it = ltree[pred_att][i]->begin();
					ltree_iter end = ltree[pred_att][i]->upper_bound(val);
					for (; it != end; ++it)
					{
						lowTreeEle ele = it.get_val();
						if (!(--c[ele.subID]))bits[ele.subID] = true;
					}
				}
			}
		}
	}
	int64_t end = start.elapsed_nano();
	cout << (double)end / 1000000 << " ms\n";
	matchSubs = bits.count();
}

void AWBTree::forward_p(const Pub& pub, int& matchSubs)
{
	vector<uint16_t> c(counter);
	//for (int i = 0; i < subs; i++)c[i] = counter[i];
	//c.assign(counter.begin(), counter.end());
	bitset<subs> bits;

	vector<std::future<vector<uint16_t>>> results;
	//size_t step = (pub.size / pdegree);

	for (int j = 0; j < parallelDegree; ++j)
	{
		results.emplace_back(
			pool.enqueue(
				[&](int _step, int tid, int _dim, uint16_t _width_size, double _width)
				{
				  //bitset<subs> _b;
				  vector<uint16_t> _c(subs, 0);
				  for (int x = tid; x < _dim; x += _step)
				  {
					  int pred_att = pub.pairs[x].att;
					  double val = pub.pairs[x].value;
					  for (int i = 0; i < _width_size; ++i)
					  {
						  if (sig[pred_att][i])
						  {
							  double w_min = i * _width;
							  ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - _width);
							  ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
							  ltree_iter end = ltree[pred_att][i]->upper_bound(val);
							  for (; it != sPoint; ++it)
							  {
								  lowTreeEle ele = it.get_val();
								  if (!(val > ele.highVal))
									  ++_c[ele.subID];
								  //if (!(--c[ele.subID]))_b[ele.subID] = true;
							  }
							  for (; it != end; ++it)
							  {
								  lowTreeEle ele = it.get_val();
								  ++_c[ele.subID];
								  //if (!(--c[ele.subID]))_b[ele.subID] = true;
							  }
						  }
					  }
				  }
				  return _c;
				}, parallelDegree, j, dim, width_size, width
			)
		);
	}

	//vector<lbptree_node*> nodes;
	//vector<bool> node_type;
	//vector<uint16_t> node_begin;
	//vector<uint16_t> node_end;
	//for (auto& pred : pub.pairs) {
	//    int pred_att = pred.att; double val = pred.value;
	//    for (int i = 0; i < width_size; ++i) {
	//        if (sig[pred_att][i]) {
	//            double w_min = i * width;
	//            ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
	//            ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
	//            ltree_iter end = ltree[pred_att][i]->upper_bound(val);
	//            if (it != sPoint) {
	//                lbptree_node* n = it.node;
	//                nodes.emplace_back(n); node_type.emplace_back(0); node_begin.emplace_back(it.idx);
	//                if (n == sPoint.node)node_end.emplace_back(sPoint.idx);
	//                else {
	//                    node_end.emplace_back(0xFFFF); n = n->next_leaf;
	//                    while (n != sPoint.node) {
	//                        nodes.emplace_back(n);
	//                        node_type.emplace_back(0);
	//                        node_begin.emplace_back(0);
	//                        node_end.emplace_back(0xFFFF);
	//                        n = n->next_leaf;
	//                    }
	//                    if (sPoint.idx != 0) {
	//                        nodes.emplace_back(n); node_type.emplace_back(0);
	//                        node_begin.emplace_back(0); node_end.emplace_back(sPoint.idx);
	//                    }
	//                }
	//            }
	//            if (sPoint != end) {
	//                lbptree_node* n = sPoint.node;
	//                nodes.emplace_back(n); node_type.emplace_back(1); node_begin.emplace_back(sPoint.idx);
	//                if (n == end.node)node_end.emplace_back(end.idx);
	//                else {
	//                    node_end.emplace_back(0xFFFF); n = n->next_leaf;
	//                    while (n != end.node) {
	//                        nodes.emplace_back(n); node_type.emplace_back(1);
	//                        node_begin.emplace_back(0); node_end.emplace_back(0xFFFF);
	//                        n = n->next_leaf;
	//                    }
	//                    if (end.idx != 0) {
	//                        nodes.emplace_back(n); node_type.emplace_back(1);
	//                        node_begin.emplace_back(0); node_end.emplace_back(end.idx);
	//                    }
	//                }
	//            }
	//        }
	//    }
	//}
	//vector<std::future<bitset<subs>>> results;
	//size_t step = (nodes.size() / pdegree) + 1;
	//for (int i = 0; i < pdegree; i++) {
	//    results.emplace_back(
	//        pool.enqueue([](size_t start, size_t end, vector<lbptree_node*>& _nodes, vector<uint16_t>& _node_begin, vector<uint16_t>& _node_end, vector<uint16_t>& _node_type, vector<atomic_uint16_t> _c) {
	//            bitset<subs> _b;
	//            for (size_t x = start; x < end; ++x) {
	//                lbptree_node* _n = _nodes[x];
	//                if (_node_type[x]) {
	//                    for (int nid = _node_begin[x]; nid < _node_end[x]; ++nid)
	//                        if (!(--_c[_n->vals[nid].subID]))_b[_n->vals[nid].subID] = true;
	//                }
	//                else {
	//                    for (int nid = _node_begin[x]; nid < _node_end[x]; ++nid)
	//                        if (!(--_c[_n->vals[nid].subID]))_b[_n->vals[nid].subID] = true;
	//                }
	//            }
	//            return _b;
	//            }, (size_t)i * step, (((size_t)i + 1) * step) >= nodes.size() ? nodes.size() : (((size_t)i + 1) * step), nodes, offset, node_type));
	//}
	//for (auto&& res : results)bits |= res.get();
	for (int j = 0; j < results.size(); ++j)
	{
		vector<uint16_t>&& _cc = results[j].get();
		for (int i = 0; i < subs; ++i)
			c[i] -= _cc[i];
		//if (!(c[i] -= _cc[i]))
		//++matchSubs;
		//bits[i] = true;
	}
	for (int i = 0; i < subs; ++i)if (!c[i])++matchSubs;
	//matchSubs = bits.count();
}

void AWBTree::backward(const Pub& pub, int& matchSubs)
{
	bitset<subs> bits;
	//Timer start;
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		int val = pred.value;
		for (int i = 0; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
				htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
				for (; itlow != ltree[pred_att][i]->end(); ++itlow)
				{
					//bits.set(itlow.get_val().subID);
					bits[itlow.get_val().subID] = true;
				}
				for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
				{
					//bits.set(it.get_val());
					bits[it.get_val()] = true;
				}
			}
		}
	}
	//int64_t end = start.elapsed_nano();
	//cout << (double)end / 1000000 << " ms\n";
	//Timer start;
	matchSubs = subs - bits.count();
	//int64_t end = start.elapsed_nano();
	//cout << (double)end / 1000 << "um\n";
}

void AWBTree::backward_o(const Pub& pub, int& matchSubs)
{
	bitset<subs> bits;
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		int val = pred.value;
		int i = 0;
		for (; i < width_size >> 1; ++i)
		{
			if (sig[pred_att][i])
			{
				ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
				htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
				for (; itlow != ltree[pred_att][i]->end(); ++itlow)
					bits[itlow.get_val().subID] = true;
				for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
					bits[it.get_val()] = true;
			}
		}
		for (; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				double v_min = i * width;
				double v_max = valDom - v_min;
				if (!(val > v_max))
				{
					ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
					for (; itlow != ltree[pred_att][i]->end(); ++itlow)
						bits[itlow.get_val().subID] = true;
				}
				else if (!(val < v_min))
				{
					htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
					for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
						bits[it.get_val()] = true;
				}
			}
		}
	}
	matchSubs = subs - bits.count();
}

void AWBTree::backward_p(const Pub& pub, int& matchSubs)
{
	vector<void*> nodes;
	vector<uint16_t> offset;
	vector<uint16_t> node_type;
	bitset<subs> bits;
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		int val = pred.value;
		int i = 0;
		for (; i < width_size >> 1; ++i)
		{
			if (sig[pred_att][i])
			{
				int pre_id = nodes.size();
				ltree_iter itlow = ltree[pred_att][i]->parallel_upper_bound(val, nodes);
				if (pre_id < nodes.size())
				{
					offset.emplace_back(itlow.idx);
					node_type.emplace_back(1);
					for (++pre_id; pre_id < nodes.size(); ++pre_id)
					{
						offset.emplace_back(0xFFFF);
						node_type.emplace_back(1);
					}
				}
				htree_iter ithigh = htree[pred_att][i]->parallel_lower_bound(val, nodes);
				if (pre_id < nodes.size())
				{
					if (ithigh == htree[pred_att][i]->end())offset.emplace_back(0xFFFF);
					else offset.emplace_back(ithigh.idx);
					node_type.emplace_back(0);
					for (++pre_id; pre_id < nodes.size(); ++pre_id)
					{
						offset.emplace_back(0xFFFF);
						node_type.emplace_back(0);
					}
				}
			}
		}
		for (; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				double v_min = i * width;
				double v_max = valDom - v_min;
				if (!(val > v_max))
				{
					int pre_id = nodes.size();
					ltree_iter itlow = ltree[pred_att][i]->parallel_upper_bound(val, nodes);
					if (pre_id < nodes.size())
					{
						offset.emplace_back(itlow.idx);
						node_type.emplace_back(1);
						for (++pre_id; pre_id < nodes.size(); ++pre_id)
						{
							offset.emplace_back(0xFFFF);
							node_type.emplace_back(1);
						}
					}
				}
				else if (!(val < v_min))
				{
					int pre_id = nodes.size();
					htree_iter ithigh = htree[pred_att][i]->parallel_lower_bound(val, nodes);
					if (pre_id < nodes.size())
					{
						if (ithigh == htree[pred_att][i]->end())offset.emplace_back(0xFFFF);
						else offset.emplace_back(ithigh.idx);
						node_type.emplace_back(0);
						for (++pre_id; pre_id < nodes.size(); ++pre_id)
						{
							offset.emplace_back(0xFFFF);
							node_type.emplace_back(0);
						}
					}
				}
			}
		}
	}

	vector<std::future<bitset<subs>>> results;
	size_t step = (nodes.size() / parallelDegree) + 1;

	for (int i = 0; i < parallelDegree; i++)
	{
		results.emplace_back(
			pool.enqueue([](size_t start, size_t end, vector<void*> _nodes, vector<uint16_t> _offset, vector<uint16_t> _node_type)
			{
			  //cout << start << endl;
			  bitset<subs> _b;
			  for (size_t x = start; x < end; ++x)
			  {
				  if (_node_type[x])
				  {
					  lbptree_node* _node = (lbptree_node*)(_nodes[x]);
					  if (_offset[x] != 0xFFFF)
					  {
						  for (int nid = _offset[x]; nid < _node->vals.size(); ++nid)
							  _b[_node->vals[nid].subID] = true;
					  }
					  else
					  {
						  for (auto&& val : _node->vals)
							  _b[val.subID] = true;
					  }
				  }
				  else
				  {
					  hbptree_node* _node = (hbptree_node*)(_nodes[x]);
					  if (_offset[x] != 0xFFFF)
					  {
						  for (int nid = 0; nid < _offset[x]; ++nid)
							  _b[_node->vals[nid]] = true;
					  }
					  else
					  {
						  for (auto&& val : _node->vals)
							  _b[val] = true;
					  }
				  }
			  }
			  //while (flag.test_and_set());
			  //_bits |= _b;
			  //flag.clear();
			  return _b;
			}, (size_t)i * step, (((size_t)i + 1) * step) >= nodes.size() ? nodes.size() : (((size_t)i + 1)
																							* step), nodes, offset, node_type));
	}
	for (auto&& res : results)bits |= res.get();
	matchSubs = subs - bits.count();
}

void AWBTree::hybrid(const Pub& pub, int& matchSubs)
{
	//Timer start;
	vector<uint16_t> c(pdr);
	bitset<subs> bits_f(bit_forward);
	bitset<subs> bits_b;
	//int64_t end = start.elapsed_nano();
	//cout << (double)end / 1000 << "um\n";
	for (auto& pred : pub.pairs)
	{
		int pred_att = pred.att;
		int val = pred.value;
		int i = 0;
		// forward
		for (; i <= dPoint; ++i)
		{
			if (sig[pred_att][i])
			{
				double w_min = i * width;
				ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
				ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
				ltree_iter end =
					val < (valDom - w_min) ? ltree[pred_att][i]->upper_bound(val) : ltree[pred_att][i]->end();
				for (; it != sPoint; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(val > ele.highVal))
						if (!(--c[ele.subID]))bits_f[ele.subID] = true;
				}
				for (; it != end; ++it)
				{
					lowTreeEle ele = it.get_val();
					if (!(--c[ele.subID]))bits_f[ele.subID] = true;
				}
			}
		}
		// backward
		for (; i < width_size >> 1; ++i)
		{
			if (sig[pred_att][i])
			{
				ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
				htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
				for (; itlow != ltree[pred_att][i]->end(); ++itlow)
					bits_b[itlow.get_val().subID] = true;
				for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
					bits_b[it.get_val()] = true;
			}
		}
		for (; i < width_size; ++i)
		{
			if (sig[pred_att][i])
			{
				double v_min = i * width;
				double v_max = valDom - v_min;
				if (!(val > v_max))
				{
					ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
					for (; itlow != ltree[pred_att][i]->end(); ++itlow)
						bits_b[itlow.get_val().subID] = true;
				}
				else if (!(val < v_min))
				{
					htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
					for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
						bits_b[it.get_val()] = true;
				}
			}
		}
	}
	//Timer start;
	matchSubs = (bits_f & bits_b.flip()).count();
	/*int64_t end = start.elapsed_nano();
	cout << (double)end / 1000 << "um\n";*/
}

void AWBTree::hybrid_a(const Pub& pub, int& matchSubs)
{
}

void AWBTree::hybrid_p(const Pub& pub, int& matchSubs)
{
	//Timer start;
	vector<uint16_t> c(pdr);
	bitset<subs> bits_f(bit_forward);
	bitset<subs> bits;
	//int64_t end = start.elapsed_nano();
	//cout << (double)end / 1000 << "um\n";

	vector<std::future<pair<vector<uint16_t>, bitset<subs>>>> results;
	size_t step = ceil((float)dim / (float)parallelDegree);

	for (int x = 0; x < parallelDegree; ++x)
	{
		results.emplace_back(pool.enqueue([&](int start, int end)
		{
		  //Timer time_rec;
		  vector<uint16_t> _c(subs, 0);
		  bitset<subs> _b;
		  for (int j = start; j < end; ++j)
		  {
			  int pred_att = pub.pairs[j].att;
			  int val = pub.pairs[j].value;
			  int i = 0;
			  // forward
			  for (; i <= dPoint; ++i)
			  {
				  if (sig[pred_att][i])
				  {
					  double w_min = i * width;
					  ltree_iter it = ltree[pred_att][i]->upper_bound(val - w_min - width);
					  ltree_iter sPoint = ltree[pred_att][i]->lower_bound(val - w_min);
					  ltree_iter end =
						  val < (valDom - w_min) ? ltree[pred_att][i]->upper_bound(val) : ltree[pred_att][i]->end();
					  for (; it != sPoint; ++it)
					  {
						  lowTreeEle ele = it.get_val();
						  if (!(val > ele.highVal))
							  //if (!(--c[ele.subID]))bits_f[ele.subID] = true;
							  ++_c[ele.subID];
					  }
					  for (; it != end; ++it)
					  {
						  lowTreeEle ele = it.get_val();
						  //if (!(--c[ele.subID]))bits_f[ele.subID] = true;
						  ++_c[ele.subID];
					  }
				  }
			  }
			  // backward
			  for (; i < width_size >> 1; ++i)
			  {
				  if (sig[pred_att][i])
				  {
					  ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
					  htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
					  for (; itlow != ltree[pred_att][i]->end(); ++itlow)
						  _b[itlow.get_val().subID] = true;
					  for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
						  _b[it.get_val()] = true;
				  }
			  }
			  for (; i < width_size; ++i)
			  {
				  if (sig[pred_att][i])
				  {
					  double v_min = i * width;
					  double v_max = valDom - v_min;
					  if (!(val > v_max))
					  {
						  ltree_iter itlow = ltree[pred_att][i]->upper_bound(val);
						  for (; itlow != ltree[pred_att][i]->end(); ++itlow)
							  _b[itlow.get_val().subID] = true;
					  }
					  else if (!(val < v_min))
					  {
						  htree_iter ithigh = htree[pred_att][i]->lower_bound(val);
						  for (htree_iter it = htree[pred_att][i]->begin(); it != ithigh; ++it)
							  _b[it.get_val()] = true;
					  }
				  }
			  }
		  }
		  //int64_t time_end = time_rec.elapsed_nano();
		  //cout << start << " " << (double)time_end / 1000000 << "ms\n";
		  return pair<vector<uint16_t>, bitset<subs>>(_c, _b);
		}, x * step, ((x + 1) * step > dim) ? dim : (x + 1) * step));
	}
	//double converge_time = 0;
	step = ceil((float)subs / (float)parallelDegree);
	//cout << step << endl;
	vector<pair<vector<uint16_t>, bitset<subs>>> pp(parallelDegree);
	for (int i = 0; i < parallelDegree; ++i)pp[i] = results[i].get();
	vector<std::future<void>> rresults;
	for (int x = 0; x < parallelDegree; ++x)
	{
		rresults.emplace_back(pool.enqueue([&](size_t _step, int _start, int _end, int tid)
		{
		  vector<uint16_t> _c(_step, 0);
		  //cout << _start << " " << _end << endl;
		  for (auto&& _p : pp)
		  {
			  //cout << _p.first[0] << endl;
			  int j = 0;
			  for (int i = _start; i < _end; ++i)
			  {
				  //cout << i << " " << j << endl;
				  _c[j++] += _p.first[i];
			  }
		  }
		  int j = 0;
		  for (int i = _start; i < _end; ++i)if (!(c[i] -= _c[j++]))bits_f[i] = true;
		  /*j = pdegree / 2;
		  vector<bitset<subs>> _bits(j);
		  while (tid < j) {
			  for (int i = 0; i < j; ++i)
		  }*/
		}, step, x * step, ((x + 1) * step > subs) ? subs : (x + 1) * step, x));
	}

	for (auto&& _p : pp)
	{
		//pair<vector<uint16_t>, bitset<subs>>&& _p = res.get();
		//Timer time_rec;
		//for (int i = 0; i < subs; ++i)c[i] -= _p.first[i];
		bits |= _p.second;
		//converge_time += (time_rec.elapsed_nano() / 1000000.0);
	}
	for (auto&& res : rresults)res.get();
	//Timer time_rec;
	//int totalnum = 0;
	//for (int i = 0; i < subs; ++i)if (!(bits[i] || c[i]))++totalnum;
	matchSubs = (bits_f & bits.flip()).count();
	//converge_time += (time_rec.elapsed_nano() / 1000000.0);
	//cout << "converge time " << converge_time << endl;
}

size_t AWBTree::calMemory()
{
	size_t mem = sizeof(lbptree*) * dim * width_size * 2;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < width_size; j++)
		{
			mem += ltree[i][j]->mem_size();
			mem += htree[i][j]->mem_size();
		}
	}
	return mem/1024/1024;
}

bool AWBTree::check(vector<uint32_t>& matchSubList)
{
	cout << "check start...\n";
	int i = 0;
	for (auto& pub : pubList)
	{
		int n = 0;
		vector<int> index(dim, 0);
		for (int j = 0; j < pub.pairs.size(); j++)index[pub.pairs[j].att] = j + 1;
		for (auto& sub : subList)
		{
			int match = 0;
			for (auto& spred : sub.constraints)
			{
				int pid = index[spred.att];
				if (index[spred.att] == 0)continue;
				if (pub.pairs[--pid].value < spred.lowValue || pub.pairs[pid].value > spred.highValue)break;
				++match;
			}
			if (match == sub.size)++n;
		}
		if (n != matchSubList[i])
		{
			cout << "pub " << i << " match error. " << "True match is " << n << ", but result is " << matchSubList[i]
				 << endl;
			return false;
		}
		++i;
	}
	cout << "result is correct.\n";
	return true;
}
