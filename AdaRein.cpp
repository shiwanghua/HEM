#include "AdaRein.h"

AdaRein::AdaRein(int type) :numSub(0) {
	buckStep = (valDom - 1) / buks + 1;
	numBucket = (valDom - 1) / buckStep + 1;
	data[0].resize(atts, vector<vector<Combo>>(numBucket));
	data[1].resize(atts, vector<vector<Combo>>(numBucket));
	skipped.resize(atts, false);
	string TYPE;
	switch (type)
	{
	case AdaRein_ORI:
		TYPE = "AdaRein_ORI";
		attsCounts.resize(atts);
	case AdaRein_SSS:
		TYPE = "AdaRein_SSS";
		attrFre.resize(atts);
		endBucket[0].resize(atts, vector<int>(buks));
		endBucket[1].resize(atts, vector<int>(buks));
	case AdaRein_SDS:
		TYPE = "AdaRein_SDS";
	case AdaRein_DSS:
		TYPE = "AdaRein_DSS";
	case AdaRein_DDS:
		TYPE = "AdaRein_DDS";
	case AdaRein_IBU:
		TYPE = "AdaRein_IBU";
	case AdaRein_PPH:
		TYPE = "AdaRein_PPH";
	default:
		break;
	}
	cout << "ExpID = " << expID << ". " + TYPE + ": falsePositiveRate = " << falsePositiveRate << ", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
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

void AdaRein::original_selection(double falsePositive, const vector<IntervalSub>& subList) {
	for (int i = 0; i < atts; i++) {
		attsCounts[i].att = i;
		attsCounts[i].count = 0;
	}

	int countSum = 0, currentSum = 0;
	double avgSubSize = 0, avgWidth = 0;

	for (int i = 0; i < subList.size(); i++) {
		countSum += subList[i].size;
		for (int j = 0; j < subList[i].size; j++) {
			++attsCounts[subList[i].constraints[j].att].count;
			avgWidth += subList[i].constraints[j].highValue - subList[i].constraints[j].lowValue;
		}
	}
	avgSubSize = countSum / subList.size();
	avgWidth /= countSum;

	// faster version, need to be verified.
	/*_for(skipIndex, 0, atts) {
		_for(j, 0, bucks)
			attsCounts[skipIndex].count += data[0][skipIndex][j].size();
		countSum += attsCounts[skipIndex].count;
	}*/

	sort(attsCounts.begin(), attsCounts.end());

	for (int i = 0; i < atts; i++) {
		currentSum += attsCounts[i].count;
		// µÈÐ§°æ±¾:
		if ((double)(countSum - currentSum) / (double)subs >
			avgSubSize + log(falsePositive + 1) / log(avgWidth / valDom)) {
			//		if ((double)(countSum - currentSum) / (double)subList.size() > subList[0].constraints.size() + log(falsePositive + 1) / log((subList[0].constraints[0].highValue - subList[0].constraints[0].lowValue) / (double)valDom)) {
			skipped[attsCounts[i].att] = true;
			//cout << " " << attsCounts[skipIndex].att; // could output in finding order.
		}
		else {
			currentSum -= attsCounts[i].count; // back
			break;
		}
	}

	if (!display) {
		int counter = 0;
		cout << "avgSubSize= " << avgSubSize << ", " << "avgWidth= " << avgWidth << ", countSum= " << countSum << ", "
			<< "currentSum= " << currentSum << ".\n";
		cout << "Skip attribute:";
		_for(i, 0, atts) {
			if (skipped[i]) {
				counter++;
				cout << " " << i;
			}
		}
		cout << endl << "Total skipped attribute: " << counter << "\n";
	}
}

void AdaRein::static_succession_selection(double falsePositive, const vector<IntervalSub>& subList) {
	for (int i = 0; i < atts; i++) {
		attsCounts[i].att = i;
		attsCounts[i].count = 0;
	}

	int countSum = 0, currentSum = 0;
	double avgSubSize = 0, avgWidth = 0;

	//for (int skipIndex = 0; skipIndex < subList.size(); skipIndex++) {
	//	countSum += subList[skipIndex].size;
	//	for (int j = 0; j < subList[skipIndex].size; j++) {
	//		++attsCounts[subList[skipIndex].constraints[j].att].count;
	//		avgWidth += subList[skipIndex].constraints[j].highValue - subList[skipIndex].constraints[j].lowValue;
	//	}
	//}
	_for(i, 0, atts) {
		_for(j, 0, buks)
			attsCounts[i].count += data[0][i][j].size();
		countSum += attsCounts[i].count;
	}
	avgSubSize = countSum / subList.size();
	avgWidth /= countSum;

	sort(attsCounts.begin(), attsCounts.end());

	double minK = log(pow(avgWidth / valDom, avgSubSize) + falsePositive) / log(avgWidth / valDom);
	int skipIndex = 0;
	for (skipIndex = 0; skipIndex < atts; skipIndex++) {
		currentSum += attsCounts[skipIndex].count;
		if ((double)(countSum - currentSum) / (double)subs > minK) {
			skipped[attsCounts[skipIndex].att] = true;
			//cout << " " << attsCounts[skipIndex].att; // could output in finding order.
		}
		else {
			currentSum -= attsCounts[skipIndex].count; // back
			break;
		}
	}

	// <AttributeId, low0/high1, bucketId, sizeOfBucket>
	priority_queue<tuple<int, int, int, int>> minHeap;
	while (skipIndex < atts) {
		//minHeap.push(make_tuple(attsCounts[skipIndex].att,0,));

	}

	if (!display) {
		int counter = 0;
		cout << "avgSubSize= " << avgSubSize << ", " << "avgWidth= " << avgWidth << ", countSum= " << countSum << ", "
			<< "currentSum= " << currentSum << ".\n";
		cout << "Skip attribute:";
		_for(i, 0, atts) {
			if (skipped[i]) {
				counter++;
				cout << " " << i;
			}
		}
		cout << endl << "Total skipped attribute: " << counter << "\n";
	}
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

int AdaRein::calMemory() {
	long long size = 0; // Byte
	_for(i, 0, atts) _for(j, 0, numBucket) size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
	size += sizeof(bool) * atts + sizeof(attAndCount) * atts;
	//cout << "attAndCount size = " << sizeof(attAndCount) << endl; // 8
	size = size / 1024 / 1024; // MB
	return (int)size;
}