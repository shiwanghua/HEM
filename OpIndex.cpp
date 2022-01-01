#include "OpIndex.h"

void OpIndex::insert(Sub x) {
	int att = getMinFre(x);
	isPivot[att] = true;
	for (int i = 0; i < x.size; i++) {
		ConElement e;
		e.subID = x.id;
		e.att = x.constraints[i].att;
		e.val = x.constraints[i].value;

		if (x.constraints[i].op == 0) {
			data[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash1(e.att, e.val)].push_back(e);
			sig[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash1(e.att, e.val)] = true;
		} else {
			data[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
			sig[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash2(e.att)] = true;
		}
	}
	numSub++;
}

void OpIndex::insert(IntervalSub x) {
	int att = getMinFre(x);
	isPivot[att] = true;
	for (int i = 0; i < x.size; i++) {
		ConElement e;
		e.subID = x.id;
		e.att = x.constraints[i].att;
		e.val = x.constraints[i].lowValue;
		data[att][1][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
		sig[att][1][e.att % SEGMENTS][signatureHash2(e.att)] = true;
		e.val = x.constraints[i].highValue;
		data[att][2][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
		sig[att][2][e.att % SEGMENTS][signatureHash2(e.att)] = true;
	}
	numSub++;
}

bool OpIndex::deleteSubscription(IntervalSub sub) {
	int find = 0;
	int id = sub.id;
	int pivotAtt = getMinFre(sub);
	//isPivot[pivotAtt] = false; // 未实现

	for (int i = 0; i < sub.size; i++) {
		int att = sub.constraints[i].att;
		int seg = att % SEGMENTS;
		int hash = signatureHash2(att);
		vector<ConElement>::iterator it;
		for (it = data[pivotAtt][1][seg][hash].begin(); it != data[pivotAtt][1][seg][hash].end(); it++)
			if (it->subID == id) {
				data[pivotAtt][1][seg][hash].erase(it);
				find++;
				break;
			}
		for (it = data[pivotAtt][2][seg][hash].begin(); it != data[pivotAtt][2][seg][hash].end(); it++)
			if (it->subID == id) {
				data[pivotAtt][2][seg][hash].erase(it);
				find++;
				break;
			}
		//sig[pivotAtt][1][seg][hash] = false; // 未实现
		//sig[pivotAtt][2][seg][hash] = false; // 未实现
	}

	if (find == 2 * sub.size)
		numSub--;
	return find == 2 * sub.size;
}

void OpIndex::match(Pub pub, int &matchSubs, const vector<Sub> &subList) {
	initCounter(subList);
	for (int i = 0; i < pub.size; i++) {
		int piv_att = pub.pairs[i].att;

		if (!isPivot[piv_att])
			continue;

		for (int j = 0; j < pub.size; j++) {
			int att = pub.pairs[j].att % SEGMENTS, value = pub.pairs[j].value;
			int hashValue = signatureHash1(pub.pairs[j].att, value);
			if (sig[piv_att][0][att][hashValue])
				for (int k = 0; k < data[piv_att][0][att][hashValue].size(); k++) {
					ConElement ce = data[piv_att][0][att][hashValue][k];
					if (ce.val == value && ce.att == pub.pairs[j].att) {
						--counter[ce.subID];
						if (counter[ce.subID] == 0)
							++matchSubs;
					}
				}
			hashValue = signatureHash2(pub.pairs[j].att);
			if (sig[piv_att][1][att][hashValue])
				for (int k = 0; k < data[piv_att][1][att][hashValue].size(); k++) {
					ConElement ce = data[piv_att][1][att][hashValue][k];
					if (ce.att == pub.pairs[j].att && ce.val <= value) {
						--counter[ce.subID];
						if (counter[ce.subID] == 0)
							++matchSubs;
					}
				}
			if (sig[piv_att][2][att][hashValue])
				for (int k = 0; k < data[piv_att][2][att][hashValue].size(); k++) {
					ConElement ce = data[piv_att][2][att][hashValue][k];
					if (ce.att == pub.pairs[j].att && ce.val >= value) {
						--counter[ce.subID];
						if (counter[ce.subID] == 0)
							++matchSubs;
					}
				}
		}
	}
}

void OpIndex::match(Pub pub, int &matchSubs, const vector<IntervalSub> &subList) {
	initCounter(subList);
	for (int i = 0; i < pub.size; i++) {
		int piv_att = pub.pairs[i].att;
		if (!isPivot[piv_att])
			continue;

		for (int j = 0; j < pub.size; j++) {
			int att = pub.pairs[j].att % SEGMENTS, value = pub.pairs[j].value;

			int hashValue = signatureHash2(pub.pairs[j].att);
			if (sig[piv_att][1][att][hashValue])
				for (int k = 0; k < data[piv_att][1][att][hashValue].size(); k++) {
					ConElement ce = data[piv_att][1][att][hashValue][k];
					if (ce.att == pub.pairs[j].att && ce.val <= value) {
						--counter[ce.subID];
						if (counter[ce.subID] == 0)
							++matchSubs;
					}
				}
			if (sig[piv_att][2][att][hashValue])
				for (int k = 0; k < data[piv_att][2][att][hashValue].size(); k++) {
					ConElement ce = data[piv_att][2][att][hashValue][k];
					if (ce.att == pub.pairs[j].att && ce.val >= value) {
						--counter[ce.subID];
						if (counter[ce.subID] == 0)
							++matchSubs;
					}
				}
		}
	}
}

void OpIndex::initCounter(const vector<Sub> &subList) {
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size;
}

void OpIndex::initCounter(const vector<IntervalSub> &subList) {
	for (int i = 0; i < subList.size(); i++)
		counter[i] = subList[i].size << 1;
}

int OpIndex::getMinFre(Sub x) {
	int pAtt = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[pAtt])
			pAtt = x.constraints[i].att;
	return pAtt;
}

int OpIndex::getMinFre(IntervalSub x) {
	int tmp = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[tmp])
			tmp = x.constraints[i].att;
	return tmp;
}

void OpIndex::calcFrequency(const vector<Sub> &subList) {
	memset(fre, 0, sizeof(fre));
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];
}

void OpIndex::calcFrequency(const vector<IntervalSub> &subList) {
	memset(fre, 0, sizeof(fre));
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];

	for (int i = 0; i < atts; i++) {
		cout << "Att " << i << ": " << fre[i] << "\t";
		if (i > 0 && i % 5 == 0)
			cout << endl;
	}
	cout << endl;
}

int OpIndex::signatureHash1(int att, int val) {
	return att * val % MAX_SIGNATURE;
}

int OpIndex::signatureHash2(int att) {
	return att * att % MAX_SIGNATURE;
}

int OpIndex::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, atts) {
		_for(j, 0, 3) {
			_for(k, 0, SEGMENTS) {
				_for(q, 0, MAX_SIGNATURE) {
					size += data[i][j][k][q].size() * sizeof(ConElement) + sizeof(bool); // sig 数组
				}
			}
		}
	}
	size += MAX_SUBS * sizeof(int) + MAX_ATTS * sizeof(bool) + MAX_ATTS * sizeof(int); // counter, isPivot, fre
	size = size / 1024 / 1024;                                                         // MB
	return (int) size;
}

OpIndex2::OpIndex2() : numSub(0) {
	data.resize(atts, vector<vector<IntervalCombo>>(atts));
	fre.resize(atts, 0);
	pivotCount.resize(atts, 0);
	isPivot.resize(atts, false);
	cout << "ExpID = " << expID << ". OpIndex2 begins. " << endl;
}

OpIndex2::~OpIndex2() {
}

void OpIndex2::insert(IntervalSub sub) {
	if (sub.size == 0) {
		numSub++;
		return;
	}
	int pivotAtt = getMinFre(sub);
	isPivot[pivotAtt] = true;
	pivotCount[pivotAtt]++;
	for (int i = 0; i < sub.size; i++) {
		IntervalCombo e;
		e.subID = sub.id;
		e.lowValue = sub.constraints[i].lowValue;
		e.highValue = sub.constraints[i].highValue;
		data[pivotAtt][sub.constraints[i].att].push_back(e);
	}
	numSub++;
}

// 没有更新fre数组, 直接更新会出错
bool OpIndex2::deleteSubscription(IntervalSub sub) {
	if (sub.size == 0) {
		numSub--;
		return true;
	}
	int find = 0;
	int id = sub.id;
	int pivotAtt = getMinFre(sub);
	pivotCount[pivotAtt]--;
	if (pivotCount[pivotAtt] == 0)
		isPivot[pivotAtt] = false;

	for (int i = 0; i < sub.size; i++) {
		int att = sub.constraints[i].att;
		vector<IntervalCombo>::iterator it;
		for (it = data[pivotAtt][att].begin(); it != data[pivotAtt][att].end(); it++)
			if (it->subID == id) {
				data[pivotAtt][att].erase(it);
				find++;
				break;
			}
	}

	if (find == sub.size)
		numSub--;
	return find == sub.size;
}

void OpIndex2::match(Pub pub, int &matchSubs, const vector<IntervalSub> &subList) {
	vector<int> counter(subs);
	for (int i = 0; i < subList.size(); i++) {
		counter[i] = subList[i].size;
		if (counter[i] == 0)
			++matchSubs;
	}

	for (int i = 0; i < pub.size; i++) {
		int piv_att = pub.pairs[i].att;
		if (!isPivot[piv_att])
			continue;

		for (int j = 0; j < pub.size; j++) {
			int att = pub.pairs[j].att, value = pub.pairs[j].value;
			for (int k = 0; k < data[piv_att][att].size(); k++) {
				IntervalCombo ic = data[piv_att][att][k];
				if (ic.lowValue <= value && value <= ic.highValue) {
					--counter[ic.subID];
					if (counter[ic.subID] == 0)
						++matchSubs;
				}
			}
		}
	}
}

int OpIndex2::getMinFre(Sub x) {
	int pAtt = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[pAtt])
			pAtt = x.constraints[i].att;
	return pAtt;
}

int OpIndex2::getMinFre(IntervalSub x) {
	int pAtt = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[pAtt])
			pAtt = x.constraints[i].att;
	return pAtt;
}

void OpIndex2::calcFrequency(const vector<Sub> &subList) {
	fre.resize(atts, 0);
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];
}

void OpIndex2::calcFrequency(const vector<IntervalSub> &subList) {
	fre.resize(atts, 0);
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];

	if (!display) {
		for (int i = 0; i < atts; i++) {
			cout << "Att " << i << ": " << fre[i] << "\t";
			if (i > 0 && i % 5 == 0)
				cout << endl;
		}
		cout << endl;
	}
}

int OpIndex2::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, atts) {
		_for(j, 0, atts) {
			size += data[i][j].size() * sizeof(IntervalCombo); // sig 数组
		}
	}
	size += atts * sizeof(bool) + atts * sizeof(int); // isPivot, fre
	size = size / 1024 / 1024;                                                         // MB
	return (int) size;
}


bOpIndex2::bOpIndex2() : numSub(0) {
	data.resize(atts, vector<vector<IntervalCombo>>(atts));
	fre.resize(atts, 0);
	pivotCount.resize(atts, 0);
	isPivot.resize(atts, false);
	nB.resize(atts);
	cout << "ExpID = " << expID << ". bOpIndex2 (C-BOMP) begins. " << endl;
}

bOpIndex2::~bOpIndex2() {

}

void bOpIndex2::insert(IntervalSub sub) {
	if (sub.size == 0) {
		numSub++;
		return;
	}
	vector<bool> attrExist(atts, false);
	int pivotAtt = getMinFre(sub);
	isPivot[pivotAtt] = true;
	pivotCount[pivotAtt]++;
	for (int i = 0; i < sub.size; i++) {
		IntervalCombo e;
		e.subID = sub.id;
		e.lowValue = sub.constraints[i].lowValue;
		e.highValue = sub.constraints[i].highValue;
		data[pivotAtt][sub.constraints[i].att].push_back(e);
		attrExist[sub.constraints[i].att] = true;
	}
	_for(i, 0, atts) if (!attrExist[i]) nB[i][sub.id] = 1;
	numSub++;
}

// 没有Achieve this function.
bool bOpIndex2::deleteSubscription(IntervalSub sub) {
	if (sub.size == 0) {
		numSub--;
		return true;
	}
	int find = 0;
	int id = sub.id;
	int pivotAtt = getMinFre(sub);
	pivotCount[pivotAtt]--;
	if (pivotCount[pivotAtt] == 0)
		isPivot[pivotAtt] = false;

	for (int i = 0; i < sub.size; i++) {
		int att = sub.constraints[i].att;
		vector<IntervalCombo>::iterator it;
		for (it = data[pivotAtt][att].begin(); it != data[pivotAtt][att].end(); it++)
			if (it->subID == id) {
				data[pivotAtt][att].erase(it);
				find++;
				break;
			}
	}

	if (find == sub.size)
		numSub--;
	return find == sub.size;
}

void bOpIndex2::match(Pub pub, int &matchSubs, const vector<IntervalSub> &subList) {
	vector<bitset<subs>> mB(atts);
	_for(i,0,atts) memcpy(&mB[i],&nB[i],subs/8+1);//sizeof(nB[i])
	int piv_att, att;
	for (auto &&p: pub.pairs) {
		piv_att = p.att;
		if (!isPivot[piv_att])
			continue;
		for (auto &&pair: pub.pairs) {
			att = pair.att;
			for (auto &&ic: data[piv_att][att]) {
				if (ic.lowValue <= pair.value && pair.value <= ic.highValue) {
					mB[att][ic.subID] = 1;
				}
			}
		}
	}
	bitset<subs> gB; // global bitset
	gB.set();
	_for(i, 0, atts)
			gB = gB & mB[i];
	matchSubs = gB.count();
}

int bOpIndex2::getMinFre(Sub x) {
	int pAtt = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[pAtt])
			pAtt = x.constraints[i].att;
	return pAtt;
}

int bOpIndex2::getMinFre(IntervalSub x) {
	int pAtt = x.constraints.at(0).att;
	for (int i = 1; i < x.size; i++)
		if (fre[x.constraints[i].att] < fre[pAtt])
			pAtt = x.constraints[i].att;
	return pAtt;
}

void bOpIndex2::calcFrequency(const vector<Sub> &subList) {
	fre.resize(atts, 0);
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];
}

void bOpIndex2::calcFrequency(const vector<IntervalSub> &subList) {
	fre.resize(atts, 0);
	for (int i = 0; i < subList.size(); i++)
		for (int j = 0; j < subList[i].size; j++)
			++fre[subList[i].constraints[j].att];

	if (!display) {
		for (int i = 0; i < atts; i++) {
			cout << "Att " << i << ": " << fre[i] << "\t";
			if (i > 0 && i % 5 == 0)
				cout << endl;
		}
		cout << endl;
	}
}

int bOpIndex2::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, atts) {
		_for(j, 0, atts) {
			size += data[i][j].size() * sizeof(IntervalCombo); // sig 数组
		}
	}
	size += atts * sizeof(bool) + atts * sizeof(int); // isPivot, fre
	size = size / 1024 / 1024;                                                         // MB
	return (int) size;
}