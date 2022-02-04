#include "run_models.h"

// 原始反向Rein
void run_rein(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Rein rein(OriginalRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rein.insert_backward_original(gen.subList[i]); // Insert sub[i] into original rein data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "Rein Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto &&kv: deleteNo) {
			Timer deleteStart;
			if (!rein.deleteSubscription_backward_original(gen.subList[kv.first]))
				cout << "Rein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Rein Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		rein.match_backward_original(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Rein Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "Rein.txt";
	string content = expID
					 + " memory= " + Util::Int2String(rein.calMemory_backward_original())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rein.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rein.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rein.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "Rein= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "ReinBucketSize.txt";
	//rein.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rein.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rein.numBucket)
	//	content += " " + to_string(rein.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/Rein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 正向计数fRein
void run_rein_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Rein fRein(ForwardRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		fRein.insert_forward_native(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "fRein Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!fRein.deleteSubscription_forward_native(gen.subList[kv.first]))
				cout << "fRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein.match_forward_native(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fRein: Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "fRein.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fRein.calMemory_forward_native())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(fRein.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(fRein.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(fRein.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(fRein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "Rein= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "ReinBucketSize.txt";
	//rein.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rein.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, fRein.numBucket)
	//	content += " " + to_string(rein.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/fRein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 正向位集fRein
void run_rein_forward_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Rein fRein_c(ForwardRein_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		fRein_c.insert_forward_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "fRein_c CBOMP Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!fRein_c.deleteSubscription_forward_CBOMP(gen.subList[kv.first]))
				cout << "fRein_c CBOMP: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein_c CBOMP Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein_c.match_forward_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "forward Rein CBOMP Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "fRein_c.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fRein_c.calMemory_forward_CBOMP())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(fRein_c.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(fRein_c.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(fRein_c.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(fRein_c.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "Rein= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/fRein_c.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Rein hRein(HybridRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hRein.insert_hybrid_native(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HybridRein (hRein) Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!hRein.deleteSubscription_hybrid_native(gen.subList[kv.first]))
				cout << "HybridRein (hRein): sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein (hRein) Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein.match_hybrid_native(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein (hRein): Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "hRein.txt";
	string content = expID
					 + " memory= " + Util::Int2String(hRein.calMemory_hybrid_native())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hRein.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hRein.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hRein.bitTime / pubs / 1000000)
					 + " ms Ppoint= " + to_string(awRein_Ppoint)
					 + " numBuk= " + Util::Int2String(hRein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "Rein= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/hRein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Rein hRein_c(HybridRein_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hRein_c.insert_hybrid_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HybridRein (HRein) with CBOMP Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!hRein_c.deleteSubscription_hybrid_CBOMP(gen.subList[kv.first]))
				cout << "HybridRein (HRein) with CBOMP: sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein (HRein) with CBOMP Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein_c.match_hybrid_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein (HRein) with CBOMP: Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "hRein_c.txt";
	string content = expID
					 + " memory= " + Util::Int2String(hRein_c.calMemory_hybrid_CBOMP())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hRein_c.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hRein_c.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hRein_c.bitTime / pubs / 1000000)
					 + " ms Ppoint= " + to_string(awRein_Ppoint)
					 + " numBuk= " + Util::Int2String(hRein_c.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "Rein= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/hRein_c.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 并行Rein
void run_pRein(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	pRein prein;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		prein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "pRein Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!prein.deleteSubscription(gen.subList[kv.first]))
				cout << "pRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "pRein Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			prein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
//#pragma omp parallel for schedule(static, 5) num_threads(4) default(none) shared(prein,gen.pubList,matchTimeList,matchSubList) private(stdout) //dynamic
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

//		prein.match(gen.pubList[i], matchSubs);
		prein.parallelMatch(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "pRein Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "pRein.txt";
	string content = expID
					 + " memory= " + Util::Int2String(prein.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(prein.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(prein.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(prein.bitTime / pubs / 1000000)
					 + " ms pD= " + to_string(parallelDegree)
					 + " numBuk = " + Util::Int2String(prein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "pRein= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/pRein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 自适应Rein
void run_adarein(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	AdaRein adarein;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		adarein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "AdaRein Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein.select_skipped_atts(falsePositiveRate, gen.subList);
	initTime = (double) initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein Skipping Task Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!adarein.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			adarein.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		//adarein.exact_match(gen.pubList[i], matchSubs,gen.subList);
		adarein.approx_match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "AdaRein.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms fPR= " + Util::Double2String(falsePositiveRate)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "AdaRein= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/bTama8.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 纯静模式
void run_HEM(const intervalGenerator &gen) {
	HEM rb;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM Event " << i << " is matched.\n";
	}
	cout << endl;

	if (display)
		rb.printRelation();
	// output
	string outputFileName = "HEM.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be==-1?be2:be)
					 + " memory= " + Util::Int2String(rb.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rb.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rb.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(rb.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rb.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rb.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "HEM0PS= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEMBucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 静静模式
void run_HEM1(const intervalGenerator &gen) {
	HEM1 rb1;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb1.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM1 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb1.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb1.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM1 Event " << i << " is matched.\n";
	}

	if (display)
		rb1.printRelation();
	cout << endl;

	// output
	string outputFileName = "HEM1.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(rb1.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rb1.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rb1.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(rb1.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rb1.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rb1.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM1SS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEM1BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM1.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 静动模式
void run_HEM2(const intervalGenerator &gen) {
	HEM2 rb2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM2 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb2.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb2.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM2 Event " << i << " is matched.\n";
	}

	if (display)
		rb2.printRelation(0);
	cout << endl;

	// output
	string outputFileName = "HEM2.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(rb2.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rb2.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rb2.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(rb2.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rb2.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rb2.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM2SD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEM2BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 纯动模式
void run_HEM3(const intervalGenerator &gen) {
	HEM3 hem3;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hem3.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM3 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem3.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		hem3.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM3 Event " << i << " is matched.\n";
	}

	if (display)
		hem3.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "HEM3.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem3.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem3.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem3.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem3.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem3.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem3.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM3PD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEM3BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem3.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(hem3.bucketSub.begin(), hem3.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem3.bucketSub.begin(), hem3.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem3.bucketSub.begin(), hem3.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem3.numBucket)
	//	content += " " + to_string(hem3.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM3.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 动静模式
void run_HEM4(const intervalGenerator &gen) {
	HEM4 hem4;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hem4.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM4DS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem4.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		hem4.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM4DS Event " << i << " is matched.\n";
	}

	if (display)
		hem4.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "HEM4.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem4.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem4.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem4.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem4.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem4.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem4.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM4DS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEM4BucketSize.txt";
	//hem4.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem4.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(hem4.bucketSub.begin(), hem4.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem4.bucketSub.begin(), hem4.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem4.bucketSub.begin(), hem4.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem4.numBucket)
	//	content += " " + to_string(rb3.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM4.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 动动模式
void run_HEM5(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	HEM5 hem5;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hem5.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM5DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!hem5.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD Deletion Finishes.\n";
		//for (auto kv: deleteNo) {
		//	hem5.insert_online(gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		//}
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		hem5.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD Event " << i << " is matched.\n";
	}

	if (display)
		hem5.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "HEM5.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem5.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem5.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem5.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem5.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	//outputFileName = "HEM5BucketSize.txt";
	//hem5.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem5.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(hem5.bucketSub.begin(), hem5.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem5.bucketSub.begin(), hem5.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem5.bucketSub.begin(), hem5.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem5.numBucket)
	//	content += " " + to_string(hem5.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM5.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 动动模式 + avx指令
void run_HEM5_avxOR(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	HEM5_avxOR hem5_avxor;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hem5_avxor.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM5DD_avxOR Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_avxor.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!hem5_avxor.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD_avxOR: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD_avxOR Deletion Finishes.\n";
		//for (auto kv : deleteNo) {
		//	hem5_avxor.insert_online(gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		//}
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		hem5_avxor.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD_avxOR Event " << i << " is matched.\n";
	}

	if (display)
		hem5_avxor.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "HEM5_avxOR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_avxor.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem5_avxor.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem5_avxor.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem5_avxor.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_avxor.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem5_avxor.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD_256OR= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	//outputFileName = "HEM5BucketSize.txt";
	//hem5_avxor.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem5_avxor.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(hem5_avxor.bucketSub.begin(), hem5_avxor.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem5_avxor.bucketSub.begin(), hem5_avxor.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem5_avxor.bucketSub.begin(), hem5_avxor.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem5_avxor.numBucket)
	//	content += " " + to_string(hem5_avxor.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM5_avxOR.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// HEM 动动模式 + 根据宽度分层
void run_HEMSC(const intervalGenerator &gen) {
	HEMSC HEMsc;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		HEMsc.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM-SC-DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	HEMsc.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		//cout << "Begin to match event " << i << endl;
		HEMsc.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM-SC-DD Event " << i << " is matched.\n";
	}

	if (display)
		HEMsc.printRelation(1, 2);
	cout << endl;

	// output
	string outputFileName = "HEMSC.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(HEMsc.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(HEMsc.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(HEMsc.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(HEMsc.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(HEMsc.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(HEMsc.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " lvls=" + Util::Int2String(lvls);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEMSCDD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//outputFileName = "HEMSCBucketSize.txt";
	//HEMsc.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(HEMsc.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(HEMsc.bucketSub.begin(), HEMsc.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(HEMsc.bucketSub.begin(), HEMsc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(HEMsc.bucketSub.begin(), HEMsc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, HEMsc.numBucket)
	//	content += " " + to_string(HEMsc.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEMSC.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 状态压缩
void run_HEMSR(const intervalGenerator &gen) {
	HEMSR hemSR;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		hemSR.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "HEM-SR-PS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hemSR.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		//cout << "Begin to match event " << i << endl;
		hemSR.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM-SR-PS Event " << i << " is matched.\n";
	}

	if (display)
		hemSR.printRelation();
	cout << endl;

	// output
	string outputFileName = "HEMSR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hemSR.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hemSR.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hemSR.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hemSR.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hemSR.bitTime / pubs / 1000000)
					 + " ms GroupSize=" + Util::Int2String(gs)
					 + " numBuk= " + Util::Int2String(hemSR.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEMSRPS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEMSR.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 暴力判断
void run_Simple(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Simple simple;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		simple.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "Simple Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!simple.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Simple Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			simple.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		dPub dpub;
		dpub.pubId = i;
		Util::Pub2dPub(gen.pubList[i], dpub);

		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		simple.match(dpub, matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Simple Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "Simple.txt";
	string content = expID
					 + " memory= " + Util::Int2String(simple.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "Simple= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/Simple.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// 按谓词宽度增序排列+暴力判断
void run_Simple2(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	Simple2 simple2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		simple2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "Simple2 Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!simple2.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple2: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Simple2 Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			simple2.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		dPub dpub;
		dpub.pubId = i;
		Util::Pub2dPub(gen.pubList[i], dpub);

		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		simple2.match(dpub, matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Simple2 Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "Simple2.txt";
	string content = expID
					 + " memory= " + Util::Int2String(simple2.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData2Begin(outputFileName.c_str(), content);

//	outputFileName = "ComprehensiveExpTime.txt";
//	content = "Simple2= [";
//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
//	content[content.length() - 2] = ']';
//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/Simple2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
//	printf("123\n");
//	fflush(stdout);
	Tama tama;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;
//		printf("Sub %d\n",i);
//		fflush(stdout);
		tama.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "Tama Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!tama.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			tama.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		tama.match_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "Tama.txt";
	string content = expID
					 + " memory= " + Util::Int2String(tama.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/Tama.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_btama_forward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	//	printf("123\n");
	//	fflush(stdout);
	bTama btama; // bTAMA6

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;
		//		printf("Sub %d\n",i);
		//		fflush(stdout);
		btama.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "bTama_forward Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!btama.deleteSubscription(gen.subList[kv.first]))
				cout << "bTama_forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama_forward Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			btama.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama.forward_match_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bTama_forward Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "bTama6.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/bTama6.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_btama_backward1_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	bTama btama; // bTAMA7

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;
		//		printf("Sub %d\n",i);
		//		fflush(stdout);
		btama.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "bTama_backward1 Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!btama.deleteSubscription(gen.subList[kv.first]))
				cout << "bTama_backward1: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama_backward1 Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			btama.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama.backward1_match_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bTama_backward1 Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "bTama7.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/bTama7.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_btama_backward2_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	bTama btama; // bTAMA8

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;
		//		printf("Sub %d\n",i);
		//		fflush(stdout);
		btama.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "bTama_backward2 Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!btama.deleteSubscription(gen.subList[kv.first]))
				cout << "bTama_backward2: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama_backward2 Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			btama.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama.backward2_match_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bTama_backward2 Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "bTama8.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/bTama8.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_OpIndex(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	OpIndex2 opindex2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// Note: for OpIndex, it needs to calculate frequency first!
	double initTime;
	Timer initStart;
	opindex2.calcFrequency(gen.subList);
	initTime = (double) initStart.elapsed_nano() / 1000000.0;
	cout << "OpIndex2 CalcFrequency Task Finishes.\n";

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		opindex2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "OpIndex2 Insertion Finishes.\n";

	if (display) {// show pivot attribute
		int counter = 0;
		for (int i = 0; i < atts; i++) {
			cout << "Att " << i << ": " << opindex2.isPivot[i] << ", ";
			if (opindex2.isPivot[i])counter++;
			if (i > 0 && i % 5 == 0) cout << endl;
		}
		cout << "\nTotal pivot attribute: " << counter << endl;
	}

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!opindex2.deleteSubscription(gen.subList[kv.first]))
				cout << "OpIndex2: sub " << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "OpIndex2 Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			opindex2.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		opindex2.match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "OpIndex2 Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "OpIndex2.txt";
	string content = expID
					 + " memory= " + Util::Int2String(opindex2.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "OpIndex= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/OpIndex2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_bOpIndex2(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	bOpIndex2 bOpindex2; // Opindex2 with CBOMP

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// Note: for OpIndex, it needs to calculate frequency first!
	double initTime;
	Timer initStart;
	bOpindex2.calcFrequency(gen.subList);
	initTime = (double) initStart.elapsed_nano() / 1000000.0;
	cout << "bOpIndex2 (C-BOMP) CalcFrequency Task Finishes.\n";

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		bOpindex2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "bOpIndex2 (C-BOMP) Insertion Finishes.\n";

	if (display) {// show pivot attribute
		int counter = 0;
		for (int i = 0; i < atts; i++) {
			cout << "Att " << i << ": " << bOpindex2.isPivot[i] << ", ";
			if (bOpindex2.isPivot[i])counter++;
			if (i > 0 && i % 5 == 0) cout << endl;
		}
		cout << "\nTotal pivot attribute: " << counter << endl;
	}

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!bOpindex2.deleteSubscription(gen.subList[kv.first]))
				cout << "bOpIndex2 (C-BOMP): sub " << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bOpIndex2 (C-BOMP) Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			bOpindex2.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		bOpindex2.match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bOpIndex2 (C-BOMP) Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "bOpIndex2.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bOpindex2.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	/*outputFileName = "ComprehensiveExpTime.txt";
	content = "OpIndex= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);*/

	outputFileName = "tmpData/bOpIndex2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_forward(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	BGTree bgTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		bgTree.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BG-Tree Forward Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!bgTree.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree Forward Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			bgTree.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree.forward_match_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree Event " << i << " is matched forwardly.\n";
	}
	cout << endl;

	if (!display)
		bgTree.printBGTree();
	// output
	string outputFileName = "BGTree.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bgTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bgTree.getHeight())
					 + " numNode = " + Util::Int2String(bgTree.getNumNode())
					 + " BNS = " + Util::Int2String(bgTree.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bgTree.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BG-Tree= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/BGTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_forward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	BGTree bgTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		bgTree.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BG-Tree(C-BOMP) Forward Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!bgTree.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree(C-BOMP) forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree(C-BOMP) Forward Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			bgTree.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree.forward_match_C_BOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree(C-BOMP) Event " << i << " is matched forwardly.\n";
	}
	cout << endl;

	// output
	string outputFileName = "BGTree1.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bgTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bgTree.getHeight())
					 + " numNode = " + Util::Int2String(bgTree.getNumNode())
					 + " BNS = " + Util::Int2String(bgTree.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bgTree.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BG-Tree= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/BGTree1.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_backward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	BGTree bgTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto &&sub: gen.subList) {
		Timer insertStart;

		bgTree.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BG-Tree(C-BOMP) Backward Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!bgTree.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree(C-BOMP) Backward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree(C-BOMP) Backward Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			bgTree.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree.backward_match_C_BOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree(C-BOMP)  Event " << i << " is matched backwardly.\n";
	}
	cout << endl;

	// output
	string outputFileName = "BGTree2.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bgTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bgTree.getHeight())
					 + " numNode = " + Util::Int2String(bgTree.getNumNode())
					 + " BNS = " + Util::Int2String(bgTree.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bgTree.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BG-Tree= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/BGTree2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_forward(const intervalGenerator& gen, unordered_map<int, bool> deleteNo) {
	BGTree_d bgTree_d;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		bgTree_d.insert(gen.subList[i],gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "BG-Tree_d Forward Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv : deleteNo) {
			Timer deleteStart;
			if (!bgTree_d.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree_d Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree_d Forward Deletion Finishes.\n";
		/*for (auto kv: deleteNo) {
			bgTree_d.insert(gen.subList[kv.first]);
		}*/
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree_d.forward_match_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree_d Event " << i << " is matched forwardly.\n";
	}
	cout << endl;

	if (!display)
		bgTree_d.printBGTree();
	// output
	string outputFileName = "BGTree_d.txt";
	string content = expID
		+ " memory= " + Util::Int2String(bgTree_d.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
		+ " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
		+ " ms height = " + Util::Int2String(bgTree_d.getHeight())
		+ " numNode = " + Util::Int2String(bgTree_d.getNumNode())
		+ "  maxNodeSize = "+Util::Int2String(MAXNodeSIZE)
		+ " AvgHit = " + Util::Int2String(bgTree_d.hit / pubs)
		+ " numSub= " + Util::Int2String(subs)
		+ " subSize= " + Util::Int2String(cons)
		+ " numPub= " + Util::Int2String(pubs)
		+ " pubSize= " + Util::Int2String(m)
		+ " attTypes= " + Util::Int2String(atts)
		+ " attDis= " + Util::Int2String(attDis)
		+ " valDis= " + Util::Int2String(valDis)
		+ " width= " + Util::Double2String(width)
		+ " alpha= " + Util::Double2String(alpha)
		+ " subp= " + Util::Double2String(subp)
		+ " mean= " + Util::Double2String(mean)
		+ " stddev= " + Util::Double2String(stddev)
		+ " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BG-Tree_d= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/BGTree_d.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_PSTREE(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	PSTree psTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	Sub2 sub; // 需要转换一下
	Cnt2 cnt2;
	cnt2.op = 3;
	vector<Sub2> subList;
	for (auto &iSub: gen.subList) {
		sub.id = iSub.id;
		sub.size = iSub.size;
		sub.constraints.resize(0);
		for (auto &iCnt: iSub.constraints) {
			cnt2.att = iCnt.att;
			cnt2.value[0] = iCnt.lowValue;
			cnt2.value[1] = iCnt.highValue;
			sub.constraints.push_back(cnt2);
		}
		subList.push_back(sub);
	}

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		psTree.insert(subList[i]);

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "PS-Tree Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		//for (auto kv : deleteNo) {
		//	Timer deleteStart;
		//	if (!psTree.deleteSubscription_backward_original(gen.subList[kv.first]))
		//		cout << "PS-Tree: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
		//	deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		//}
		//cout << "PS-Tree Deletion Finishes.\n";
		////for (auto kv : deleteNo) {
		////	psTree.insert(gen.subList[kv.first]);
		////}
	}

	// match

	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		psTree.MatchEvent(gen.pubList[i], matchSubs, subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "PS-Tree Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "PSTree.txt";
	string content = expID
					 + " memory= " + Util::Int2String(psTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level = " + Util::Int2String(PSTree_level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "PS-Tree= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/PSTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AWBTREE(const intervalGenerator &gen, unordered_map<int, bool> deleteNo) {
	AWBTree awbTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto &&sub: gen.subList) {
		Timer insertStart;

		awbTree.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "AWB+Tree Hybrid_opt Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for (auto kv: deleteNo) {
			Timer deleteStart;
			if (!awbTree.deleteSubscription(gen.subList[kv.first]))
				cout << "AWB+Tree Hybrid_opt: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AWB+Tree Hybrid_opt Deletion Finishes.\n";
		//for (auto kv : deleteNo) {
		//	awbTree.insert(gen.subList[kv.first]);
		//}
	}

	// match

	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		awbTree.hybrid_opt(gen.pubList[i], matchSubs, gen.subList, awbTree_Ppoint);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AWB+Tree Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "AWBTree.txt";
	string content = expID
					 + " memory= " + Util::Int2String(awbTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms awbTree_Ppoint = " + Util::Double2String(awbTree_Ppoint)
					 + " WCsize = " + Util::Int2String(WCsize)
					 + " branch = " + Util::Int2String(awbTree_branch)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "PS-Tree= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/AWBTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void measure_numMark(const intervalGenerator &gen) {
	Rein rein(1);
	HEM5 hem5;

	for (int i = 0; i < subs; i++) {
		rein.insert_backward_original(gen.subList[i]);
		hem5.insert(gen.subList[i]);
	}

	hem5.initBits();

	vector<int> reinMarkNum = rein.calMarkNumForBuckets();
	vector<int> hem5MarkNum = hem5.calMarkNumForBuckets();

	cout << "rein=[";
	for (auto &&i: reinMarkNum)
		cout << i << ", ";
	cout << "]\n\nhem5=[";
	for (auto &&i: hem5MarkNum)
		cout << i << ", ";
	cout << "]\n";

}