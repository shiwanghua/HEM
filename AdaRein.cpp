#include "AdaRein.h"

AdaRein::AdaRein(int type) : numSub(0) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	data[0].resize(atts, vector<vector<Combo >>(numBucket));
	data[1].resize(atts, vector<vector<Combo >>(numBucket));
	skipped.resize(atts, false);
	string TYPE;
	switch (type) {
	case AdaRein_ORI:
		TYPE = "AdaRein_ORI";
		attsCounts.resize(atts);
		break;
	case AdaRein_SSS:
		TYPE = "AdaRein_SSS";
		attsCounts.resize(atts);
		endBucket[0].resize(atts, buks - 1);
		endBucket[1].resize(atts, 0);
		break;
	case AdaRein_SDS:
		TYPE = "AdaRein_SDS";
		break;
	case AdaRein_DSS:
		TYPE = "AdaRein_DSS";
		break;
	case AdaRein_DDS:
		TYPE = "AdaRein_DDS";
		break;
	case AdaRein_IBU:
		TYPE = "AdaRein_IBU";
		break;
	case AdaRein_PPH:
		TYPE = "AdaRein_PPH";
		break;
	case AdaRein_SSS_B:
		TYPE = "AdaRein_SSS_B";
		attsCounts.resize(atts);
		beginBucket[0].resize(atts, 0);
		beginBucket[1].resize(atts, buks - 1);
		break;
	default:
		break;
	}
	cout << "ExpID = " << expID << ". " + TYPE + ": falsePositiveRate_local = " << falsePositiveRate
		<< ", bucketStep = "
		<< buckStep << ", numBucket = " << numBucket << endl;
}

void AdaRein::insert(IntervalSub sub) {
	for (int i = 0; i < sub.size; i++) {
		IntervalCnt cnt = sub.constraints[i];
		Combo c;
		c.val = cnt.lowValue;
		c.subID = sub.id;
		data[0][cnt.att][c.val / buckStep].push_back(c);
		c.val = cnt.highValue;
		data[1][cnt.att][c.val / buckStep].push_back(c);
	}
	numSub++;
}

bool AdaRein::deleteSubscription(IntervalSub sub) {
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

void AdaRein::exact_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	vector<bool> bits(subs, false);
	vector<bool> attExist(atts, false);

	for (int i = 0; i < pub.size; i++) {
		int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
		attExist[att] = true;
		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int j = buck + 1; j < numBucket; j++)
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;

		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	for (int i = 0; i < atts; i++)
		if (!attExist[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subList.size(); i++)
		if (!bits[i])
			++matchSubs;
}

void AdaRein::original_selection(double falsePositive, const vector<IntervalSub>& subList) {
	for (int i = 0; i < atts; i++) {
		attsCounts[i].att = i;
		attsCounts[i].count = 0;
	}

	int numPredicate = 0, numSkipPredicate = 0;
	double avgSubSize = 0, avgWidth = 0;

	for (int i = 0; i < subList.size(); i++) {
		numPredicate += subList[i].size;
		for (int j = 0; j < subList[i].size; j++) {
			++attsCounts[subList[i].constraints[j].att].count;
			avgWidth += subList[i].constraints[j].highValue - subList[i].constraints[j].lowValue;
		}
	}
	avgSubSize = numPredicate / subList.size();
	avgWidth /= numPredicate;

	// faster version, need to be verified.--没有算avgWidth！
	/*_for(skipIndex, 0, atts) {
		_for(j, 0, bucks)
			attsCounts[skipIndex].count += data[0][skipIndex][j].size();
		numPredicate += attsCounts[skipIndex].count;
	}*/

	sort(attsCounts.begin(), attsCounts.end());

	int maxSkipPredicate =
		numPredicate - (avgSubSize + log(falsePositive + 1) / log(avgWidth / valDom)) *
		subs; // 至多可以过滤的谓词数, currentSum的最大值
#ifdef DEBUG
	int numSkipAttr = 0;
#endif // DEBUG

	for (int i = 0; i < atts; i++) {
		numSkipPredicate += attsCounts[i].count;
		// 等效版本:
		if ((double)(numPredicate - numSkipPredicate) / (double)subs >
			avgSubSize + log(falsePositive + 1) / log(avgWidth / valDom)) {
			//		if ((double)(numPredicate - numSkipPredicate) / (double)subList.size() > subList[0].constraints.size() + log(falsePositive + 1) / log((subList[0].constraints[0].highValue - subList[0].constraints[0].lowValue) / (double)valDom)) {
			skipped[attsCounts[i].att] = true;
#ifdef DEBUG
			numSkipAttr++;
			//cout << "Skip Attribute " << attsCounts[skipIndex].att<<"\n"; // could output in finding order.
#endif
		}
		else {
			numSkipPredicate -= attsCounts[i].count; // back
			break;
		}
	}

#ifdef DEBUG
	cout << "avgSubSize= " << avgSubSize << ", " << "avgWidth= " << avgWidth << ", numPredicate= " << numPredicate
		<< ", maxSkipPredicate= " << maxSkipPredicate << ", numSkipPredicate= " << numSkipPredicate << ".\n";
	cout << "Total skipped attribute: " << numSkipAttr << " among " << atts << " attributes.\n";
	cout << "Skip attribute:";
	_for(i, 0, atts) if (skipped[i])
		cout << " " << i;
	cout << "\n";
#endif
}

void AdaRein::approx_match_ori(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	vector<bool> bits(subs, false);
	vector<bool> attExist(atts, false);
	for (int i = 0; i < pub.size; i++) {
		int att = pub.pairs[i].att;
		attExist[att] = true;
		if (skipped[att])
			continue;
		int value = pub.pairs[i].value, buck = value / buckStep;
		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int j = buck + 1; j < numBucket; j++)
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;

		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;
		for (int j = buck - 1; j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	for (int i = 0; i < atts; i++)
		if (!attExist[i] && !skipped[i])
			for (int j = 0; j < numBucket; j++)
				for (int k = 0; k < data[0][i][j].size(); k++)
					bits[data[0][i][j][k].subID] = true;

	for (int i = 0; i < subList.size(); i++)
		if (!bits[i])
			++matchSubs;
}

void AdaRein::static_succession_selection(double falsePositive, const vector<IntervalSub>& subList) {
	for (int i = 0; i < atts; i++) {
		attsCounts[i].att = i;
		attsCounts[i].count = 0;
	}

	int numPredicate = 0, numSkipPredicate = 0; // 谓词总数, 已过滤的谓词总数
	double avgSubSize = 0, avgWidth = 0; // 平均每个订阅有多少个谓词, 谓词的平均宽度
	for (auto&& iSub : subList) {
		//numPredicate += iSub.constraints.size(); // 冗余
		for (auto&& iCnt : iSub.constraints) {
			//++attsCounts[iCnt.att].count; // 冗余
			avgWidth += iCnt.highValue - iCnt.lowValue;
		}
	}
	// 需要先插入订阅才能这么算
	_for(i, 0, atts) {
		_for(j, 0, buks) attsCounts[i].count += data[0][i][j].size();
		numPredicate += attsCounts[i].count;
	}
	avgSubSize = (double)numPredicate / subList.size();
	avgWidth /= numPredicate;

	sort(attsCounts.begin(), attsCounts.end());

	// minPredicate 解释性代码
	//double minK = log(pow(avgWidth / valDom, avgSubSize) + falsePositive) / log(avgWidth / valDom);
	//inline auto valid = [&](double) {return (double)(numPredicate - numSkipPredicate) > minK * subs; };

	double falsePositiveRate_global = pow(width, avgSubSize) * subs / (1 - falsePositive) * falsePositive / subs;

	int maxSkipPredicate = numPredicate - (avgSubSize - log(1 - falsePositive) / log(avgWidth / valDom)) * subs; // k2
	cout << "k2= " << maxSkipPredicate << "\n";
	maxSkipPredicate =
		numPredicate - log(pow(avgWidth / valDom, avgSubSize) + falsePositiveRate_global) / log(avgWidth / valDom) *
		subs; // 至多可以过滤的谓词数, currentSum的最大值
	cout << "k3_global= " << maxSkipPredicate << "\n";
	maxSkipPredicate =
		numPredicate - log(pow(avgWidth / valDom, avgSubSize) / (1 - falsePositive)) / log(avgWidth / valDom) * subs;
	cout << "k3_local= " << maxSkipPredicate << "\n";
	int skipIndex = 0;
#ifdef DEBUG
	int numSkipAttr = 0;
	int numSkipBkt = 0;
#endif // DEBUG

	for (skipIndex = 0; skipIndex < atts; skipIndex++) {
		if (numSkipPredicate + attsCounts[skipIndex].count < maxSkipPredicate) {
			numSkipPredicate = numSkipPredicate + attsCounts[skipIndex].count;
			skipped[attsCounts[skipIndex].att] = true;
#ifdef DEBUG
			numSkipAttr++;
			//cout << "Skip Attribute " << attsCounts[skipIndex].att<<"\n"; // could output in finding order.
#endif // DEBUG
		}
		else {
			break;
		}
	}

	// <low0/high1, AttributeId, bucketId, sizeOfBucket>
	auto cmp = [&](const auto& a, const auto& b) {
		return get<3>(a) > get<3>(b); // a桶小, 返回false, 就把a作为根, 即实现小根堆
	};
	priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, decltype(cmp)> minHeap(cmp);
	while (skipIndex < atts) {
		minHeap.push(
			make_tuple(0, attsCounts[skipIndex].att, buks - 1, data[0][attsCounts[skipIndex].att][buks - 1].size()));
		minHeap.push(make_tuple(1, attsCounts[skipIndex].att, 0, data[1][attsCounts[skipIndex].att][0].size()));
		skipIndex++;
	}
	while (true) {
		auto item = minHeap.top();
		minHeap.pop();
		if (numSkipPredicate + get<3>(item) > maxSkipPredicate) {
			break;
		}
		numSkipPredicate = numSkipPredicate + get<3>(item);
#ifdef DEBUG
		numSkipBkt++;
#endif
		if (get<0>(item) == 0) { // low
			get<2>(item) -= 1; // 过滤掉这个桶
			endBucket[0][get<1>(item)] = get<2>(item); // 过滤掉这个桶
			if (get<2>(item) >= 1) { // 还有剩余的桶可以被过滤, 为0时停止, 因为0号桶在LVE上顶多是比较桶不可能是标记桶
				get<3>(item) = data[0][get<1>(item)][get<2>(item)].size(); // 更新桶大小
				minHeap.push(item);
			}
		}
		else { // high
			get<2>(item) += 1; // 过滤掉这个桶
			endBucket[1][get<1>(item)] = get<2>(item); // 过滤掉这个桶
			if (get<2>(item) < buks - 1) { // 还有剩余的桶可以被过滤
				get<3>(item) = data[1][get<1>(item)][get<2>(item)].size();
				minHeap.push(item);
			}
		}
	}
#ifdef DEBUG
	cout << "rightMatchNum= " << pow(width, avgSubSize) * subs << ", wrongMatchNum= "
		<< pow(width, avgSubSize) * subs / (1 - falsePositive) * falsePositive << ", falsePositiveRate_local= "
		<< falsePositive
		<< ", falsePositiveRate_global= " << falsePositiveRate_global << ".\n";
	cout << "avgSubSize= " << avgSubSize << ", " << "avgWidth= " << avgWidth << ", numPredicate= " << numPredicate
		<< ", maxSkipPredicate= " << maxSkipPredicate << ", numSkipPredicate= " << numSkipPredicate << ".\n";
	cout << "Total skipped attribute: " << numSkipAttr << " among " << atts << " attributes.\n";
	cout << "Total skipped bucket: " << numSkipAttr << "*2*" << buks << " + " << numSkipBkt << " = "
		<< numSkipAttr * 2 * buks + numSkipBkt << " among " << atts * 2 * buks << " buckets.\n";
	cout << "Skip attribute:";
	_for(i, 0, atts) if (skipped[i])
		cout << " " << i;
	cout << "\n";
#endif
}

void AdaRein::approx_match_sss(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	bitset<subs> bits;
	vector<bool> attExist(atts, false);
	for (auto&& iPair : pub.pairs) {
		int att = iPair.att;
		attExist[att] = true;
		if (skipped[att])
			continue;
		int value = iPair.value, buck = value / buckStep;

		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int j = buck + 1; j <= endBucket[0][att]; j++) // 和HEM系列的设计不同, 这里取闭括号
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;

		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;
		for (int j = buck - 1; j >= endBucket[1][att]; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	// 可以替换为1次位集或
	for (int i = 0; i < atts; i++)
		if (!attExist[i] && !skipped[i])
			for (int j = buks - 1; j >= endBucket[1][i]; j--)
				for (int k = 0; k < data[1][i][j].size(); k++)
					bits[data[1][i][j][k].subID] = true;

	matchSubs = subs - bits.count();
}

void AdaRein::static_succession_selection_backward(double falsePositive, const vector<IntervalSub>& subList) {
	for (int i = 0; i < atts; i++) {
		attsCounts[i].att = i;
		attsCounts[i].count = 0;
	}

	int numPredicate = 0, numSkipPredicate = 0; // 谓词总数, 已过滤的谓词总数
	double avgSubSize = 0, avgWidth = 0; // 平均每个订阅有多少个谓词, 谓词的平均宽度
	for (auto&& iSub : subList) {
		//numPredicate += iSub.constraints.size(); // 冗余
		for (auto&& iCnt : iSub.constraints) {
			//++attsCounts[iCnt.att].count; // 冗余
			avgWidth += iCnt.highValue - iCnt.lowValue;
		}
	}
	// 需要先插入订阅才能这么算
	_for(i, 0, atts) {
		_for(j, 0, buks) attsCounts[i].count += data[0][i][j].size();
		numPredicate += attsCounts[i].count;
	}
	avgSubSize = (double)numPredicate / subList.size();
	avgWidth /= numPredicate;

	sort(attsCounts.begin(), attsCounts.end());

	// minPredicate 解释性代码
	//double minK = log(pow(avgWidth / valDom, avgSubSize) + falsePositive) / log(avgWidth / valDom);
	//inline auto valid = [&](double) {return (double)(numPredicate - numSkipPredicate) > minK * subs; };

	double falsePositiveRate_global = pow(width, avgSubSize) * subs / (1 - falsePositive) * falsePositive / subs;

	int maxSkipPredicate = numPredicate - (avgSubSize - log(1 - falsePositive) / log(avgWidth / valDom)) * subs; // k2
	
	cout << "k2= " << maxSkipPredicate << "\n";
	maxSkipPredicate =
		numPredicate - log(pow(avgWidth / valDom, avgSubSize) + falsePositiveRate_global) / log(avgWidth / valDom) *
		subs; // 至多可以过滤的谓词数, currentSum的最大值
	cout << "k3_global= " << maxSkipPredicate << "\n";
	maxSkipPredicate =
		numPredicate - log(pow(avgWidth / valDom, avgSubSize) / (1 - falsePositive)) / log(avgWidth / valDom) * subs;
	cout << "k3_local= " << maxSkipPredicate << "\n";
	int skipIndex = 0;
#ifdef DEBUG
	int numSkipAttr = 0;
	int numSkipBkt = 0;
#endif // DEBUG
	//maxSkipPredicate *= 6;
	for (skipIndex = 0; skipIndex < atts; skipIndex++) {
		if (numSkipPredicate + attsCounts[skipIndex].count < maxSkipPredicate) {
			numSkipPredicate = numSkipPredicate + attsCounts[skipIndex].count;
			skipped[attsCounts[skipIndex].att] = true;
#ifdef DEBUG
			numSkipAttr++;
			//cout << "Skip Attribute " << attsCounts[skipIndex].att<<"\n"; // could output in finding order.
#endif // DEBUG
		}
		else {
			break;
		}
	}

	// <low0/high1, AttributeId, bucketId, sizeOfBucket>
	auto cmp = [&](const auto& a, const auto& b) {
		return get<3>(a) > get<3>(b); // a桶小, 返回false, 就把a作为根, 即实现小根堆
	};
	priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, decltype(cmp)> minHeap(cmp);
	while (skipIndex < atts) {
		minHeap.push(
			make_tuple(0, attsCounts[skipIndex].att, 0, data[0][attsCounts[skipIndex].att][0].size()));
		minHeap.push(make_tuple(1, attsCounts[skipIndex].att, buks - 1, data[1][attsCounts[skipIndex].att][buks - 1].size()));
		skipIndex++;
	}
	
	while (true) {
		auto item = minHeap.top();
		minHeap.pop();
		if (numSkipPredicate + get<3>(item)/2 > maxSkipPredicate) {
			break;
		}
		numSkipPredicate = numSkipPredicate + get<3>(item)/2;
#ifdef DEBUG
		numSkipBkt++;
#endif
		if (get<0>(item) == 0) { // low
			get<2>(item) += 1; // 过滤掉这个桶
			beginBucket[0][get<1>(item)] = get<2>(item); // 过滤掉这个桶
			if (get<2>(item) < buks - 1) { // 还有剩余的桶可以被过滤
				get<3>(item) = data[0][get<1>(item)][get<2>(item)].size();
				minHeap.push(item);
			}
		}
		else { // high
			get<2>(item) -= 1; // 过滤掉这个桶
			beginBucket[1][get<1>(item)] = get<2>(item); // 过滤掉这个桶
			if (get<2>(item) > 0) { // 还有剩余的桶可以被过滤, 为0时停止, 因为0号桶在LVE上顶多是比较桶不可能是标记桶
				get<3>(item) = data[1][get<1>(item)][get<2>(item)].size(); // 更新桶大小
				minHeap.push(item);
			}
		}
	}
#ifdef DEBUG
	cout << "rightMatchNum= " << pow(width, avgSubSize) * subs << ", wrongMatchNum= "
		<< pow(width, avgSubSize) * subs / (1 - falsePositive) * falsePositive << ", falsePositiveRate_local= "
		<< falsePositive
		<< ", falsePositiveRate_global= " << falsePositiveRate_global << ".\n";
	cout << "avgSubSize= " << avgSubSize << ", " << "avgWidth= " << avgWidth << ", numPredicate= " << numPredicate
		<< ", maxSkipPredicate= " << maxSkipPredicate << ", numSkipPredicate= " << numSkipPredicate << ".\n";
	cout << "Total skipped attribute: " << numSkipAttr << " among " << atts << " attributes.\n";
	cout << "Total skipped bucket: " << numSkipAttr << "*2*" << buks << " + " << numSkipBkt << " = "
		<< numSkipAttr * 2 * buks + numSkipBkt << " among " << atts * 2 * buks << " buckets.\n";
	cout << "Skip attribute:";
	_for(i, 0, atts) if (skipped[i])
		cout << " " << i;
	cout << "\n";
#endif
}

void AdaRein::approx_match_sss_b(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList) {
	bitset<subs> bits;
	vector<bool> attExist(atts, false);
	for (auto&& iPair : pub.pairs) {
		int att = iPair.att;
		attExist[att] = true;
		if (skipped[att])
			continue;
		int value = iPair.value, buck = value / buckStep;

		for (int k = 0; k < data[0][att][buck].size(); k++)
			if (data[0][att][buck][k].val > value)
				bits[data[0][att][buck][k].subID] = true;
		for (int j = max(buck + 1, beginBucket[0][att]); j < buks; j++) // 和HEM系列的设计不同, 这里取闭括号
			for (int k = 0; k < data[0][att][j].size(); k++)
				bits[data[0][att][j][k].subID] = true;

		for (int k = 0; k < data[1][att][buck].size(); k++)
			if (data[1][att][buck][k].val < value)
				bits[data[1][att][buck][k].subID] = true;
		for (int j = min(buck - 1, beginBucket[1][att]); j >= 0; j--)
			for (int k = 0; k < data[1][att][j].size(); k++)
				bits[data[1][att][j][k].subID] = true;
	}

	// 可以替换为1次位集或
	for (int i = 0; i < atts; i++)
		if (!attExist[i] && !skipped[i])
			for (int j = beginBucket[1][i]; j >= 0; j--)
				for (auto&& k : data[1][i][j])
					bits[k.subID] = true;

	matchSubs = subs - bits.count();
}

int AdaRein::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, atts) _for(j, 0, numBucket) size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	size += sizeof(bool) * atts + sizeof(attAndCount) * atts;
	//cout << "attAndCount size = " << sizeof(attAndCount) << endl; // 8
	size = size / 1024 / 1024; // MB
	return (int)size;
}