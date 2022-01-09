#include "Rein.h"

Rein::Rein(int type) : numSub(0), numDimension(atts) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	bucketSub.resize(numBucket);
	if (type == OriginalRein) { // Original Rein
		cout << "ExpID = " << expID << ". Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
		data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
		data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
	} else {
		memset(subPredicate, 0, sizeof(subPredicate));
		memset(counter, 0, sizeof(counter));
		fData[0].resize(numDimension, vector<vector<IntervalCombo>>(numBucket));
		fData[1].resize(numDimension, vector<vector<IntervalCombo>>(numBucket));
		if (type == ForwardRein) {
			cout << "ExpID = " << expID << ". Forward Rein (fRein): bucketStep = " << buckStep << ", numBucket = "
				 << numBucket << endl;
			//nnB.resize(atts); // fRein_to_bRein
		} else if (type == ForwardRein_CBOMP) {
			cout << "ExpID = " << expID << ". Forward Rein (fRein) with C-BOMP: bucketStep = " << buckStep
				 << ", numBucket = " << numBucket << endl;
			nB.resize(atts);
		} else {
			data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
			data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
			if (type == HybridRein) {
				cout << "ExpID = " << expID << ". HybridRein (AWRein): bucketStep = " << buckStep << ", numBucket = "
					 << numBucket << endl;
			} else if (type == HybridRein_CBOMP) {
				cout << "ExpID = " << expID << ". HybridRein (AWRein) with C-BOMP: bucketStep = " << buckStep
					 << ", numBucket = "
					 << numBucket << endl;
				nnB.resize(atts);
			}
		}
	}

}

//void Rein::insert(Sub sub)
//{
//    for (int i = 0; i < sub.size; i++)
//    {
//        Cnt cnt = sub.constraints[i];
//        Combo c;
//        c.val = cnt.value;
//        c.subID = sub.id;
//        if (cnt.op == 0)        // ==
//        {
//            data[0][cnt.att][c.val / buckStep].push_back(c);
//            data[1][cnt.att][c.val / buckStep].push_back(c);
//        }
//        else if (cnt.op == 1)   // >=
//            data[0][cnt.att][c.val / buckStep].push_back(c);
//        else                    // >=
//            data[1][cnt.att][c.val / buckStep].push_back(c);
//    }
//}

void Rein::insert_backward_original(IntervalSub sub) {
	for (int i = 0; i < sub.size; i++) {
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		// int bucketID = cnt.lowValue / buckStep; // Bug: 这里被坑了
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][cnt.lowValue / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][cnt.highValue / buckStep].push_back(c);
	}
	numSub++;
}

bool Rein::deleteSubscription_backward_original(IntervalSub sub) {
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}
// 01在第二维，比较-标记-比较-标记
//void Rein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//
//	for (int i = 0; i < pub.size; i++)
//	{
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > value)
//				bits[data[0][att][buck][k].subID] = true;
//
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//
//
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < value)
//				bits[data[1][att][buck][k].subID] = true;
//
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
//	}
//
//	for (int i = 0; i < numSub; i++)
//		if (!bits[i])
//			++matchSubs;
//}

// 01在第一维 计算时间组成
//void Rein::match(const Pub& pub, int& matchSubs)
//{
//	vector<bool> bits(numSub, false);
//	vector<bool> attExist(numDimension, false);
//	for (int i = 0; i < pub.size; i++)
//	{
//		Timer compareStart;
//		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
//		attExist[att] = true;
//		// 把下面两个for循环注释了就是模糊匹配, 类似Tama
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > value)
//				bits[data[0][att][buck][k].subID] = true;
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < value)
//				bits[data[1][att][buck][k].subID] = true;
//		compareTime += (double)compareStart.elapsed_nano();
//
//		Timer markStart;
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
//		markTime += (double)markStart.elapsed_nano();
//	}
//
//	Timer markStart;
//	for (int i = 0; i < numDimension; i++)
//		if (!attExist[i])
//			for (int j = 0; j < numBucket; j++)
//				for (int k = 0; k < data[0][i][j].size(); k++)
//					bits[data[0][i][j][k].subID] = true;
//	markTime += (double)markStart.elapsed_nano();
//
//	Timer bitStart;
//	for (int i = 0; i < subs; i++)
//		if (!bits[i])
//		{
//			++matchSubs;
//			//cout << "rein matches sub: " << i << endl;
//		}
//	bitTime += (double)bitStart.elapsed_nano();
//}
// 01在第一维 不计算时间组成
void Rein::match_backward_original(const Pub &pub, int &matchSubs) {
	vector<bool> bits(numSub, false);
	vector<bool> attExist(numDimension, false);
	for (int i = 0; i < pub.size; i++) {
		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		attExist[att] = true;
		// 把下面两个for循环注释了就是模糊匹配, 类似Tama
		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;

		for (int j = buck + 1; j < numBucket; j++)
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	for (int i = 0; i < numDimension; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subs; i++)
		if (!bits[i]) {
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}

int Rein::calMemory_backward_original() {
	long long size = sizeof(data[0]) * 2; // Byte
	_for(i, 0, numDimension) {
		//cout <<i<<": "<< sizeof(data[0][i]) << ": ";
		size += sizeof(data[0][i]) * 2;
		_for(j, 0, numBucket) {
			size += sizeof(data[0][i][j]) * 2 + sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
			//cout << sizeof(data[0][i][j]) << " ";
		}
		//cout << "\n";
	}
	size = size / 1024 / 1024; // MB
	return (int) size;
}



// ---------------------------------------------------------------------------------------------
// forward Rein (fRein)

void Rein::insert_forward_native(IntervalSub sub) {
	int bucketID;
	IntervalCombo c;
	c.subID = sub.id;
	subPredicate[sub.id] = sub.size;
	for (auto &&cnt: sub.constraints) {
		c.lowValue = cnt.lowValue;
		c.highValue = cnt.highValue;
		bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
		fData[0][cnt.att][bucketID].insert(
			lower_bound(fData[0][cnt.att][bucketID].begin(), fData[0][cnt.att][bucketID].end(), c,
						[&](const IntervalCombo &c1, const IntervalCombo &c2) {
							return c1.highValue == c2.highValue ? c1.lowValue < c2.lowValue : c1.highValue <
																							  c2.highValue;
						}), c); // insert 到迭代器前面!
		bucketID = cnt.highValue / buckStep;
		fData[1][cnt.att][bucketID].insert(
			lower_bound(fData[1][cnt.att][bucketID].begin(), fData[1][cnt.att][bucketID].end(), c,
						[&](const IntervalCombo &c1, const IntervalCombo &c2) {
							return c1.lowValue == c2.lowValue ? c1.highValue < c2.highValue : c1.lowValue < c2.lowValue;
						}), c);
	}
	numSub++;
}

void Rein::match_forward_native(const Pub &pub, int &matchSubs) {
	memcpy(counter, subPredicate, sizeof(subPredicate));
	int att, buck, midBuck = buks >> 1;
	IntervalCombo pubPredicateTmp;
	for (auto &&pair: pub.pairs) {
		pubPredicateTmp.lowValue = pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		if (buck < midBuck) { // Use low bucket list.
			const auto &&lowerBoundIterator = lower_bound(fData[0][att][buck].begin(), fData[0][att][buck].end(),
														  pubPredicateTmp,
														  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.highValue < c2.highValue;
														  });
			for_each(lowerBoundIterator, fData[0][att][buck].end(), [&](const IntervalCombo &c) {
				if (c.lowValue <= pair.value) counter[c.subID]--;
			});

			for_each(fData[0][att].begin(), fData[0][att].begin() + buck, [&](const vector<IntervalCombo> &bucketList) {
				auto &&lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
														[&](const IntervalCombo &c1, const IntervalCombo &c2) {
															return c1.highValue < c2.highValue;
														});
				for_each(lowerBoundIterator, bucketList.end(), [&](const IntervalCombo &c) { counter[c.subID]--; });
			});
		} else { // Use high bucket list.
			const auto &&upperBoundIterator = upper_bound(fData[1][att][buck].begin(), fData[1][att][buck].end(),
														  pubPredicateTmp,
														  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.lowValue < c2.lowValue;
														  });
			for_each(fData[1][att][buck].begin(), upperBoundIterator, [&](const IntervalCombo &c) {
				if (c.highValue >= pair.value) counter[c.subID]--;
			});

			for_each(fData[1][att].begin() + buck + 1, fData[1][att].end(),
					 [&](const vector<IntervalCombo> &bucketList) {
						 auto &&upperBoundIterator = upper_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
																 [&](const IntervalCombo &c1, const IntervalCombo &c2) {
																	 return c1.lowValue < c2.lowValue;
																 });
						 for_each(bucketList.begin(), upperBoundIterator,
								  [&](const IntervalCombo &c) { counter[c.subID]--; });
					 });
		}
	}

	for (int i = 0; i < subs; i++)
		if (counter[i] == 0) {
			++matchSubs;
			//cout << "rein matches sub: " << i << endl;
		}
}

bool Rein::deleteSubscription_forward_native(IntervalSub sub) { // 未实现
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it = 
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

int Rein::calMemory_forward_native() {
	long long size = sizeof(fData[0]) * 2; // Byte
	_for(i, 0, numDimension) {
		//cout <<i<<": "<< sizeof(data[0][i]) << ": ";
		size += sizeof(fData[0][i]) * 2;
		_for(j, 0, numBucket) {
			size += sizeof(fData[0][i][j]) + sizeof(fData[1][i][j]) +
					sizeof(IntervalCombo) * (fData[0][i][j].size() + fData[0][i][j].size());
			//cout << sizeof(data[0][i][j]) << " ";
		}
		//cout << "\n";
	}
	size += sizeof(subPredicate) + sizeof(counter);
	size = size / 1024 / 1024; // MB
	return (int) size;
}



// ---------------------------------------------------------------------------------------------
// forward Rein with C-BOMP (fRein_c)

void Rein::insert_forward_CBOMP(IntervalSub sub) {
	vector<bool> attrExist(atts, false);
	int bucketID;
	IntervalCombo c;
	c.subID = sub.id;
	subPredicate[sub.id] = sub.size;
	for (auto &&cnt: sub.constraints) {
		attrExist[cnt.att] = true;
		//nnB[cnt.att][sub.id] = 1;
		c.lowValue = cnt.lowValue;
		c.highValue = cnt.highValue;
		bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
		fData[0][cnt.att][bucketID].insert(
			lower_bound(fData[0][cnt.att][bucketID].begin(), fData[0][cnt.att][bucketID].end(), c,
						[&](const IntervalCombo &c1, const IntervalCombo &c2) {
							return c1.highValue == c2.highValue ? c1.lowValue < c2.lowValue : c1.highValue <
																							  c2.highValue;
						}), c); // insert 到迭代器前面!
		bucketID = cnt.highValue / buckStep;
		fData[1][cnt.att][bucketID].insert(
			lower_bound(fData[1][cnt.att][bucketID].begin(), fData[1][cnt.att][bucketID].end(), c,
						[&](const IntervalCombo &c1, const IntervalCombo &c2) {
							return c1.lowValue == c2.lowValue ? c1.highValue < c2.highValue : c1.lowValue < c2.lowValue;
						}), c);
	}
	_for(i, 0, atts) if (!attrExist[i])
			nB[i][sub.id] = 1;
	numSub++;
}

void Rein::match_forward_CBOMP(const Pub &pub, int &matchSubs) {
	bitset<subs> gB, mB; // global bitset
	gB.set(); // 全设为1, 假设都是匹配的
	vector<bool> attExist(atts, false);
	int att, buck, midBuck = buks >> 1;
	IntervalCombo pubPredicateTmp;
	for (auto &&pair: pub.pairs) {
		mB = nB[pair.att];
		attExist[pair.att] = true;
		pubPredicateTmp.lowValue = pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		if (buck < midBuck) { // Use low bucket list.
			const auto &&lowerBoundIterator = lower_bound(fData[0][att][buck].begin(), fData[0][att][buck].end(),
														  pubPredicateTmp,
														  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.highValue < c2.highValue;
														  });
			for_each(lowerBoundIterator, fData[0][att][buck].end(), [&](const IntervalCombo &c) {
				if (c.lowValue <= pair.value) mB[c.subID] = 1;
			});

			for_each(fData[0][att].begin(), fData[0][att].begin() + buck, [&](const vector<IntervalCombo> &bucketList) {
				const auto &&lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
															  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
																  return c1.highValue < c2.highValue;
															  });
				for_each(lowerBoundIterator, bucketList.end(), [&](const IntervalCombo &c) { mB[c.subID] = 1; });
			});
		} else { // Use high bucket list.
			const auto &&upperBoundIterator = upper_bound(fData[1][att][buck].begin(), fData[1][att][buck].end(),
														  pubPredicateTmp,
														  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.lowValue < c2.lowValue;
														  });
			for_each(fData[1][att][buck].begin(), upperBoundIterator, [&](const IntervalCombo &c) {
				if (c.highValue >= pair.value) mB[c.subID] = 1;
			});

			for_each(fData[1][att].begin() + buck + 1, fData[1][att].end(),
					 [&](const vector<IntervalCombo> &bucketList) {
						 const auto &&upperBoundIterator = upper_bound(bucketList.begin(), bucketList.end(),
																	   pubPredicateTmp,
																	   [&](const IntervalCombo &c1,
																		   const IntervalCombo &c2) {
																		   return c1.lowValue < c2.lowValue;
																	   });
						 for_each(bucketList.begin(), upperBoundIterator,
								  [&](const IntervalCombo &c) { mB[c.subID] = 1; });
					 });
		}
		gB = gB & mB;
	}
	_for(i, 0, atts) if (!attExist[i])
			gB = gB & nB[i];
	matchSubs = gB.count();
}

bool Rein::deleteSubscription_forward_CBOMP(IntervalSub sub) { // 未实现
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

int Rein::calMemory_forward_CBOMP() {
	long long size = sizeof(fData[0]) * 2; // Byte
	_for(i, 0, numDimension) {
		//cout <<i<<": "<< sizeof(data[0][i]) << ": ";
		size += sizeof(fData[0][i]) * 2;
		_for(j, 0, numBucket) {
			size += sizeof(fData[0][i][j]) + sizeof(fData[1][i][j]) +
					sizeof(IntervalCombo) * (fData[0][i][j].size() + fData[0][i][j].size());
			//cout << sizeof(data[0][i][j]) << " ";
		}
		//cout << "\n";
	}
	size += sizeof(subPredicate) + sizeof(counter);
	size += sizeof(bitset<subs>) * atts;
	size = size / 1024 / 1024; // MB
	return (int) size;
}


// ---------------------------------------------------------------------------------------------
// HybridRein (AWRein) 2022-01-08
void Rein::insert_hybrid_native(IntervalSub sub) {
	int bucketID;
	IntervalCombo ic;
	Combo cb;
	ic.subID = sub.id;
	cb.subID = sub.id;
	for (auto &&cnt: sub.constraints) {
		if (awRein_Ppoint <= cnt.highValue - cnt.lowValue) { // backward
			cb.val = cnt.lowValue;
			bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
			data[0][cnt.att][bucketID].insert(
				lower_bound(data[0][cnt.att][bucketID].begin(), data[0][cnt.att][bucketID].end(), cb,
							[&](const Combo &c1, const Combo &c2) {
								return c1.val < c2.val;
							}), cb);
			cb.val = cnt.highValue;
			bucketID = cnt.highValue / buckStep;
			data[1][cnt.att][bucketID].insert(
				lower_bound(data[1][cnt.att][bucketID].begin(), data[1][cnt.att][bucketID].end(), cb,
							[&](const Combo &c1, const Combo &c2) {
								return c1.val < c2.val;
							}), cb);
		} else { // forward
			subPredicate[sub.id]++;
			ic.lowValue = cnt.lowValue;
			ic.highValue = cnt.highValue;
			bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
			fData[0][cnt.att][bucketID].insert(
				lower_bound(fData[0][cnt.att][bucketID].begin(), fData[0][cnt.att][bucketID].end(), ic,
							[&](const IntervalCombo &c1, const IntervalCombo &c2) {
								return c1.highValue == c2.highValue ? c1.lowValue < c2.lowValue : c1.highValue <
																								  c2.highValue;
							}), ic);
			bucketID = cnt.highValue / buckStep;
			fData[1][cnt.att][bucketID].insert(
				lower_bound(fData[1][cnt.att][bucketID].begin(), fData[1][cnt.att][bucketID].end(), ic,
							[&](const IntervalCombo &c1, const IntervalCombo &c2) {
								return c1.lowValue == c2.lowValue ? c1.highValue < c2.highValue : c1.lowValue <
																								  c2.lowValue;
							}), ic);
		}
	}
	numSub++;
}

void Rein::match_hybrid_native(const Pub &pub, int &matchSubs) {
	memcpy(counter, subPredicate, sizeof(subPredicate));
	int att, buck, midBuck = buks >> 1;
	IntervalCombo pubPredicateTmp;
//	for (auto &&pair: pub.pairs) {
	for_each(pub.pairs.begin(), pub.pairs.end(), [&](const Pair &pair) {
		pubPredicateTmp.lowValue = pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
		// cout<<"pubid= "<<pub.id<<" att= "<<att<<" value= "<<value<<endl;
		if (buck < midBuck) { // Use low bucket list.
			const auto &&lowerBoundIterator = lower_bound(fData[0][att][buck].begin(), fData[0][att][buck].end(),
														  pubPredicateTmp,
														  [](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.highValue < c2.highValue;
														  });
			for_each(lowerBoundIterator, fData[0][att][buck].end(), [&](const IntervalCombo &c) {
				if (c.lowValue <= pair.value) counter[c.subID]--;
			});

			for_each(fData[0][att].begin(), fData[0][att].begin() + buck, [&](const vector<IntervalCombo> &bucketList) {
				const auto &&lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
															  [](const IntervalCombo &c1, const IntervalCombo &c2) {
																  return c1.highValue < c2.highValue;
															  });
				for_each(lowerBoundIterator, bucketList.end(), [&](const IntervalCombo &c) { counter[c.subID]--; });
			});
		} else { // Use high bucket list.
			const auto &&upperBoundIterator = upper_bound(fData[1][att][buck].begin(), fData[1][att][buck].end(),
														  pubPredicateTmp,
														  [&](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.lowValue < c2.lowValue;
														  });
			for_each(fData[1][att][buck].begin(), upperBoundIterator, [&](const IntervalCombo &c) {
				if (c.highValue >= pair.value) counter[c.subID]--;
			});

			for_each(fData[1][att].begin() + buck + 1, fData[1][att].end(),
					 [&](const vector<IntervalCombo> &bucketList) {
						 const auto &&upperBoundIterator = upper_bound(bucketList.begin(), bucketList.end(),
																	   pubPredicateTmp,
																	   [](const IntervalCombo &c1,
																		  const IntervalCombo &c2) {
																		   return c1.lowValue < c2.lowValue;
																	   });
						 for_each(bucketList.begin(), upperBoundIterator,
								  [&](const IntervalCombo &c) { counter[c.subID]--; });
					 });
		}
	});
//	}

	vector<bool> bits(numSub, false);
	vector<bool> attExist(numDimension, false);
	Combo cb;
//	for (auto &&pair: pub.pairs) {
	for_each(pub.pairs.begin(), pub.pairs.end(), [&](const Pair &pair) {
		att = pair.att, buck = pair.value / buckStep;
		attExist[att] = true;
		cb.val = pair.value;
		auto &&boundIterator = upper_bound(data[0][att][buck].begin(), data[0][att][buck].end(),
										   cb,
										   [](const Combo &c1, const Combo &c2) {
											   return c1.val < c2.val;
										   });
		for_each(boundIterator, data[0][att][buck].end(), [&bits](const Combo &c) {
			bits[c.subID] = true;
		});
		for_each(data[0][att].begin() + buck + 1, data[0][att].end(), [&bits](const vector<Combo> &bucketList) {
			for_each(bucketList.begin(), bucketList.end(), [&bits](const Combo &c) { bits[c.subID] = true; });
		});

		boundIterator = lower_bound(data[1][att][buck].begin(), data[1][att][buck].end(),
									cb,
									[](const Combo &c1, const Combo &c2) {
										return c1.val < c2.val;
									});
		for_each(data[1][att][buck].begin(), boundIterator, [&bits](const Combo &c) {
			bits[c.subID] = true;
		});
		for_each(data[1][att].begin(), data[1][att].begin() + buck, [&bits](const vector<Combo> &bucketList) {
			for_each(bucketList.begin(), bucketList.end(), [&bits](const Combo &c) { bits[c.subID] = true; });
		});

//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > pair.value)
//				bits[data[0][att][buck][k].subID] = true;
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < pair.value)
//				bits[data[1][att][buck][k].subID] = true;
//
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				bits[data[0][att][j][k].subID] = true;
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				bits[data[1][att][j][k].subID] = true;
	});
//	}

	for (int i = 0; i < numDimension; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (auto &&cb: data[0][i][j])
					bits[cb.subID] = true;

	for (int i = 0; i < subs; i++)
		if (!bits[i] && counter[i] == 0) {
			++matchSubs;
			//cout << "AWRein matches sub: " << i << endl;
		}
}

bool Rein::deleteSubscription_hybrid_native(IntervalSub sub) { // 未实现
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

int Rein::calMemory_hybrid_native() {
	long long size = sizeof(fData[0]) * 2 + sizeof(data[0]) * 2; // Byte
	_for(i, 0, numDimension) {
		size += (sizeof(data[0][i]) + sizeof(fData[0][i])) * 2;
		_for(j, 0, numBucket) {
			size += (sizeof(data[0][i][j]) + sizeof(fData[0][i][j])) * 2 + \
            sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size()) + \
            sizeof(IntervalCombo) * (fData[0][i][j].size() + fData[0][i][j].size());
		}
	}
	size += sizeof(subPredicate) + sizeof(counter);
	size = size / 1024 / 1024; // MB
	return (int) size;
}



// ---------------------------------------------------------------------------------------------
// HybridRein with C-BOMP (AWRein_CBOMP) 2022-01-08

void Rein::insert_hybrid_CBOMP(IntervalSub sub) {
	int bucketID;
//	vector<bool> attrExist(atts, false);
	IntervalCombo ic;
	Combo cb;
	ic.subID = sub.id;
	cb.subID = sub.id;
	for (auto &&cnt: sub.constraints) {
		if (awRein_Ppoint <= cnt.highValue - cnt.lowValue) { // backward
			cb.val = cnt.lowValue;
			bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
			data[0][cnt.att][bucketID].insert(
				lower_bound(data[0][cnt.att][bucketID].begin(), data[0][cnt.att][bucketID].end(), cb,
							[&](const Combo &c1, const Combo &c2) {
								return c1.val < c2.val;
							}), cb);
			cb.val = cnt.highValue;
			bucketID = cnt.highValue / buckStep;
			data[1][cnt.att][bucketID].insert(
				lower_bound(data[1][cnt.att][bucketID].begin(), data[1][cnt.att][bucketID].end(), cb,
							[&](const Combo &c1, const Combo &c2) {
								return c1.val < c2.val;
							}), cb);
		} else { // forward
			nnB[cnt.att][sub.id] = true; // Only insert into forward data structure the forward matching think this attribute of the event is not null ! ('null' means match here)
//			attrExist[cnt.att] = true;
			ic.lowValue = cnt.lowValue;
			ic.highValue = cnt.highValue;
			bucketID = cnt.lowValue / buckStep; // upper_bound probably is also OK!
			fData[0][cnt.att][bucketID].insert(
				lower_bound(fData[0][cnt.att][bucketID].begin(), fData[0][cnt.att][bucketID].end(), ic,
							[&](const IntervalCombo &c1, const IntervalCombo &c2) {
								return c1.highValue == c2.highValue ? c1.lowValue < c2.lowValue : c1.highValue <
																								  c2.highValue;
							}), ic);
			bucketID = cnt.highValue / buckStep;
			fData[1][cnt.att][bucketID].insert(
				lower_bound(fData[1][cnt.att][bucketID].begin(), fData[1][cnt.att][bucketID].end(), ic,
							[&](const IntervalCombo &c1, const IntervalCombo &c2) {
								return c1.lowValue == c2.lowValue ? c1.highValue < c2.highValue : c1.lowValue <
																								  c2.lowValue;
							}), ic);
		}
	}
//	_for(i, 0, atts) if (!attrExist[i])
//			nB[i][sub.id] = 1;
	numSub++;
}

void Rein::match_hybrid_CBOMP(const Pub &pub, int &matchSubs) {
	int att, buck, midBuck = buks >> 1;
	vector<bool> attExist(atts, false);
	bitset<subs> backwardBits, mB;
	Combo cb;
//	for (auto &&pair: pub.pairs) {
	for_each(pub.pairs.begin(), pub.pairs.end(), [&](const Pair &pair) {
		att = pair.att, buck = pair.value / buckStep;
		attExist[att] = true;
		cb.val = pair.value;
		auto &&boundIterator = upper_bound(data[0][att][buck].begin(), data[0][att][buck].end(),
										   cb,
										   [](const Combo &c1, const Combo &c2) {
											   return c1.val < c2.val;
										   });
		for_each(boundIterator, data[0][att][buck].end(), [&backwardBits](const Combo &c) {
			backwardBits[c.subID] = 1;
		});
		for_each(data[0][att].begin() + buck + 1, data[0][att].end(), [&backwardBits](const vector<Combo> &bucketList) {
			for_each(bucketList.begin(), bucketList.end(),
					 [&backwardBits](const Combo &c) { backwardBits[c.subID] = 1; });
		});

		boundIterator = lower_bound(data[1][att][buck].begin(), data[1][att][buck].end(),
									cb,
									[](const Combo &c1, const Combo &c2) {
										return c1.val < c2.val;
									});
		for_each(data[1][att][buck].begin(), boundIterator, [&backwardBits](const Combo &c) {
			backwardBits[c.subID] = 1;
		});
		for_each(data[1][att].begin(), data[1][att].begin() + buck, [&backwardBits](const vector<Combo> &bucketList) {
			for_each(bucketList.begin(), bucketList.end(),
					 [&backwardBits](const Combo &c) { backwardBits[c.subID] = 1; });
		});
//		for (int k = 0; k < data[0][att][buck].size(); k++)
//			if (data[0][att][buck][k].val > pair.value)
//				backwardBits[data[0][att][buck][k].subID] = 1;
//		for (int k = 0; k < data[1][att][buck].size(); k++)
//			if (data[1][att][buck][k].val < pair.value)
//				backwardBits[data[1][att][buck][k].subID] = 1;
//
//		for (int j = buck + 1; j < numBucket; j++)
//			for (int k = 0; k < data[0][att][j].size(); k++)
//				backwardBits[data[0][att][j][k].subID] = 1;
//		for (int j = buck - 1; j >= 0; j--)
//			for (int k = 0; k < data[1][att][j].size(); k++)
//				backwardBits[data[1][att][j][k].subID] = 1;
	});

	IntervalCombo pubPredicateTmp;
//	for (auto &&pair: pub.pairs) {
	for_each(pub.pairs.begin(), pub.pairs.end(), [&](const Pair &pair) {
		pubPredicateTmp.lowValue = pubPredicateTmp.highValue = pair.value, att = pair.att, buck = pair.value / buckStep;
		mB = nnB[att];
		if (buck < midBuck) { // Use low bucket list.
			const auto &&lowerBoundIterator = lower_bound(fData[0][att][buck].begin(), fData[0][att][buck].end(),
														  pubPredicateTmp,
														  [](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.highValue < c2.highValue;
														  });
			for_each(lowerBoundIterator, fData[0][att][buck].end(), [&mB, &pair](const IntervalCombo &c) {
				if (c.lowValue <= pair.value) mB[c.subID] = 0;
			});

			for_each(fData[0][att].begin(), fData[0][att].begin() + buck, [&](const vector<IntervalCombo> &bucketList) {
				auto &&lowerBoundIterator = lower_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
														[](const IntervalCombo &c1, const IntervalCombo &c2) {
															return c1.highValue < c2.highValue;
														});
				for_each(lowerBoundIterator, bucketList.end(), [&mB](const IntervalCombo &c) { mB[c.subID] = 0; });
			});
		} else { // Use high bucket list.
			const auto &&upperBoundIterator = upper_bound(fData[1][att][buck].begin(), fData[1][att][buck].end(),
														  pubPredicateTmp,
														  [](const IntervalCombo &c1, const IntervalCombo &c2) {
															  return c1.lowValue < c2.lowValue;
														  });
			for_each(fData[1][att][buck].begin(), upperBoundIterator, [&mB, &pair](const IntervalCombo &c) {
				if (c.highValue >= pair.value) mB[c.subID] = 0;
			});

			for_each(fData[1][att].begin() + buck + 1, fData[1][att].end(), [&](const vector<IntervalCombo> &bucketList) {
				auto &&upperBoundIterator = upper_bound(bucketList.begin(), bucketList.end(), pubPredicateTmp,
														[](const IntervalCombo &c1, const IntervalCombo &c2) {
															return c1.lowValue < c2.lowValue;
														});
				for_each(bucketList.begin(), upperBoundIterator, [&mB](const IntervalCombo &c) { mB[c.subID] = 0; });
			});
		}
		backwardBits = backwardBits | mB;
	});

	_for(i, 0, atts) if (!attExist[i]) {
			for (int j = 0; j < numBucket; j++)
				for (auto &&cb: data[0][i][j])
					backwardBits[cb.subID] = 1;
			backwardBits = backwardBits | nnB[i];
		}
	matchSubs = subs - backwardBits.count();
}

bool Rein::deleteSubscription_hybrid_CBOMP(IntervalSub sub) { // 未实现
	int find = 0;
	IntervalCnt cnt;
	int bucketID, id = sub.id;

	_for(i, 0, sub.size) {
		cnt = sub.constraints[i];

		bucketID = cnt.lowValue / buckStep;
		vector<Combo>::iterator it;
		for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[0][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}

		bucketID = cnt.highValue / buckStep;
		for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
			if (it->subID == id) {
				data[1][cnt.att][bucketID].erase(it); // it =
				find++;
				break;
			}
	}
	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

int Rein::calMemory_hybrid_CBOMP() {
	long long size = sizeof(fData[0]) * 2 + sizeof(data[0]) * 2; // Byte
	_for(i, 0, numDimension) {
		size += (sizeof(data[0][i]) + sizeof(fData[0][i])) * 2;
		_for(j, 0, numBucket) {
			size += (sizeof(data[0][i][j]) + sizeof(fData[0][i][j])) * 2 + \
            sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size()) + \
            sizeof(IntervalCombo) * (fData[0][i][j].size() + fData[0][i][j].size());
		}
	}
	size += sizeof(bitset<subs>) * atts; // nnB
	size = size / 1024 / 1024; // MB
	return (int) size;
}



// Measurement/Visualization function

void Rein::calBucketSize() {
	bucketSub.clear();
	bucketSub.resize(numBucket);
	_for(i, 0, numDimension) _for(j, 0, numBucket) {
			_for(k, 0, data[0][i][j].size()) bucketSub[j].insert(data[0][i][j][k].subID);
			_for(k, 0, data[1][i][j].size()) bucketSub[j].insert(data[1][i][j][k].subID);
		}
}

vector<int> Rein::calMarkNumForBuckets() {
	vector<int> numMarking(numBucket, 0);
	_for(i, 0, numBucket) {
		_for(j, 0, numDimension) {
			_for(k, i, numBucket) {
				numMarking[i] += data[0][j][k].size();
			}
			_for(k, 0, i + 1) {
				numMarking[i] += data[1][j][k].size();
			}
		}
	}
	return numMarking;
}