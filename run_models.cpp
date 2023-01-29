#include "run_models.h"

// ԭʼ����Rein
void run_rein(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein rein(OriginalRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		rein.insert_backward_original(gen.subList[i]); // Insert sub[i] into original rein data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Rein Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto&& kv : deleteNo)
		{
			Timer deleteStart;
			if (!rein.deleteSubscription_backward_original(gen.subList[kv.first]))
				cout << "Rein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Rein Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			rein.insert_backward_original(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		rein.match_backward_original(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Rein Event " << i << " is matched.\n";
	}

	realMatchNum = Util::Average(matchSubList);
#ifdef DEBUG
	cout << "realMatchNum= " << realMatchNum << ", matchTime= " << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " ms pD= " + to_string(parallelDegree)
					 + " avx= " + to_string(blockSize)
					 + " numBuk= " + Util::Int2String(rein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Rein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

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

void run_rein_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein fRein(ForwardRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fRein.insert_forward_native(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fRein Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fRein.deleteSubscription_forward_native(gen.subList[kv.first]))
				cout << "fRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fRein.insert_forward_native(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein.match_forward_native(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fRein: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "fRein: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fRein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

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

void run_rein_forward_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein fRein_c(ForwardRein_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fRein_c.insert_forward_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fRein_c CBOMP Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fRein_c.deleteSubscription_forward_CBOMP(gen.subList[kv.first]))
				cout << "fRein_c CBOMP: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein_c CBOMP Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fRein_c.insert_forward_CBOMP(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein_c.match_forward_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "forward Rein CBOMP Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "fRein-CBOMP: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fRein_C.txt";
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fRein_C= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fRein_C.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein hRein(HybridRein);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hRein.insert_hybrid_native(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HybridRein (hRein) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hRein.deleteSubscription_hybrid_native(gen.subList[kv.first]))
				cout << "HybridRein (hRein): sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein (hRein) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hRein.insert_hybrid_native(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein.match_hybrid_native(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein (hRein): Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "hRein: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "hRein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/hRein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein hRein_c(HybridRein_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hRein_c.insert_hybrid_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HybridRein (HRein) with CBOMP Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hRein_c.deleteSubscription_hybrid_CBOMP(gen.subList[kv.first]))
				cout << "HybridRein (HRein) with CBOMP: sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein (HRein) with CBOMP Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hRein_c.insert_hybrid_CBOMP(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein_c.match_hybrid_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein (HRein) with CBOMP: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "hRein_c: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "hRein_c= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/hRein_c.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_forward_PGWO(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein fRein_pgwo(ForwardRein_PGWO);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fRein_pgwo.insert_forward_PGWO(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fRein_pgwo Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fRein_pgwo.deleteSubscription_forward_PGWO(gen.subList[kv.first]))
				cout << "fRein_pgwo: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein_pgwo Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fRein_pgwo.insert_forward_PGWO(gen.subList[kv.first]);
		}
	}
	fRein_pgwo.initFix();
	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein_pgwo.match_forward_PGWO(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fRein_pgwo: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "fRein_pgwo: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fRein_pgwo.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fRein_pgwo.calMemory_forward_native())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(fRein_pgwo.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(fRein_pgwo.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(fRein_pgwo.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(fRein_pgwo.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fRein_pgwo= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fRein_pgwo.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_forward_PGWO_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein fRein_pgwo_c(ForwardRein_PGWO_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fRein_pgwo_c.insert_forward_PGWO_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fRein_pgwo C-BOMP Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fRein_pgwo_c.deleteSubscription_forward_PGWO_CBOMP(gen.subList[kv.first]))
				cout << "fRein_pgwo C-BOMP: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein_pgwo  C-BOMP Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fRein_pgwo_c.insert_forward_PGWO_CBOMP(gen.subList[kv.first]);
		}
	}
	fRein_pgwo_c.initFix();
	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein_pgwo_c.match_forward_PGWO_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "forward Rein_pgwo C-BOMP Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "fRein_pgwo C-BOMP: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fRein_pgwo_c.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fRein_pgwo_c.calMemory_forward_CBOMP())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(fRein_pgwo_c.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(fRein_pgwo_c.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(fRein_pgwo_c.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(fRein_pgwo_c.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fRein_pgwo_c= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fRein_pgwo_c.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid_PGWO(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein hRein_pgwo(HybridRein_PGWO);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hRein_pgwo.insert_hybrid_PGWO(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HybridRein-PGWO (hRein_pgwo) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hRein_pgwo.deleteSubscription_hybrid_PGWO(gen.subList[kv.first]))
				cout << "HybridRein-PGWO (hRein_pgwo): sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein-PGWO (hRein_pgwo) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hRein_pgwo.insert_hybrid_PGWO(gen.subList[kv.first]);
		}
	}
	hRein_pgwo.initFix();
	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein_pgwo.match_hybrid_PGWO(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein (hRein_pgwo): Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "hRein_pgwo: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "hRein_pgwo.txt";
	string content = expID
					 + " memory= " + Util::Int2String(hRein_pgwo.calMemory_hybrid_native())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hRein_pgwo.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hRein_pgwo.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hRein_pgwo.bitTime / pubs / 1000000)
					 + " ms Ppoint= " + to_string(awRein_Ppoint)
					 + " numBuk= " + Util::Int2String(hRein_pgwo.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "hRein_pgwo= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/hRein_pgwo.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_hybrid_PGWO_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Rein hRein_pgwo_c(HybridRein_PGWO_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hRein_pgwo_c.insert_hybrid_PGWO_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HybridRein_PGWO_CBOMP (HRein-PGWO-C) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hRein_pgwo_c.deleteSubscription_hybrid_PGWO_CBOMP(gen.subList[kv.first]))
				cout << "HybridRein_PGWO_CBOMP (HRein-PGWO-C): sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HybridRein_PGWO_CBOMP (HRein-PGWO-C) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hRein_pgwo_c.insert_hybrid_PGWO_CBOMP(gen.subList[kv.first]);
		}
	}
	hRein_pgwo_c.initFix();
	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		hRein_pgwo_c.match_hybrid_PGWO_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HybridRein_PGWO_CBOMP (HRein-PGWO-C): Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "hRein_pgwo_cbomp: MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "hRein_pgwo_c.txt";
	string content = expID
					 + " memory= " + Util::Int2String(hRein_pgwo_c.calMemory_hybrid_CBOMP())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hRein_pgwo_c.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hRein_pgwo_c.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hRein_pgwo_c.bitTime / pubs / 1000000)
					 + " ms Ppoint= " + to_string(awRein_Ppoint)
					 + " numBuk= " + Util::Int2String(hRein_pgwo_c.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "hRein_pgwo_c= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/hRein_pgwo_c.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_forward_DMFT(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{                 // REIN-F
	Rein rein_F(Forward_DMFT_REIN);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		rein_F.insert_forward_DMFT_REIN(gen.subList[i]); // Insert sub[i] into original rein_F data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Rein-F (DMFT) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto&& kv : deleteNo)
		{
			Timer deleteStart;
			if (!rein_F.deleteSubscription_forward_DMFT_REIN(gen.subList[kv.first]))
				cout << "Rein-F (DMFT): sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Rein-F (DMFT) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			rein_F.insert_forward_DMFT_REIN(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		rein_F.match_forward_DMFT_REIN(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Rein-F (DMFT) Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << " Rein-F (DMFT): MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "Rein_F_DMFT.txt";
	string content = expID
					 + " memory= " + Util::Int2String(rein_F.calMemory_forward_DMFT_REIN())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rein_F.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rein_F.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rein_F.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rein_F.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Rein_F_DMFT= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/Rein_F_DMFT.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_rein_backward_DMFT(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{            // fREIN-C-B
	Rein fRein_c_b(Backward_DMFT_fREIN_CBOMP);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fRein_c_b.insert_backward_DMFT_fREIN_CBOMP(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fRein_c_b (DMFT) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fRein_c_b.deleteSubscription_backward_DMFT_fREIN_CBOMP(gen.subList[kv.first]))
				cout << "fRein_c_b (DMFT): sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fRein_c_b (DMFT) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fRein_c_b.insert_backward_DMFT_fREIN_CBOMP(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fRein_c_b.match_backward_DMFT_fREIN_CBOMP(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fRein_c_b (DMFT) Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "fRein_c_b (DMFT): MatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fRein_C_B.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fRein_c_b.calMemory_forward_CBOMP())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(fRein_c_b.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(fRein_c_b.markTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(fRein_c_b.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(fRein_c_b.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fRein_C_B= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fRein_C_B.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// parallel Rein
void run_pRein(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	pRein prein;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		prein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "pRein Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!prein.deleteSubscription(gen.subList[kv.first]))
				cout << "pRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "pRein Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			prein.insert(gen.subList[kv.first]);
		}
	}

	// match
//#pragma omp parallel for schedule(static, 5) num_threads(4) default(none) shared(prein,gen.pubList,matchTimeList,matchSubList) private(stdout) //dynamic
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		//		prein.match(gen.pubList[i], matchSubs);
		prein.parallelMatch(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
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
					 + " ms MergeTime= " + to_string(prein.mergeTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(prein.bitTime / pubs / 1000000)
					 + " ms pD= " + to_string(parallelDegree)
					 + " numBuk = " + Util::Int2String(prein.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pRein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/pRein.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ӦRein
void run_AdaRein_ORI(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein(AdaRein_ORI);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein.original_selection(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein.approx_match_ori(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "falseMatchNum= " << Util::Average(matchSubList) << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / Util::Average(matchSubList) << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "AdaRein_ORI.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms maxSkipPre= " + Util::Int2String(adarein.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_ORI.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_SSS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_sss(AdaRein_SSS);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_sss.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_SSS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein_sss.static_succession_selection(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_SSS Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_sss.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_SSS: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_SSS Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_sss.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_sss.approx_match_sss(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_SSS Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "falseMatchNum= " << Util::Average(matchSubList) << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / Util::Average(matchSubList) << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "AdaRein_SSS.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_sss.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms maxSkipPre= " + Util::Int2String(adarein_sss.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_SSS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_SSS.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_SSS_B(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_sss_b(AdaRein_SSS_B);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_sss_b.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_SSS_B Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein_sss_b.static_succession_selection_backward(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_SSS_B Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_sss_b.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_SSS_B: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_SSS_B Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_sss_b.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_sss_b.approx_match_sss_b(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_SSS_B Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "falseMatchNum= " << Util::Average(matchSubList) << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / Util::Average(matchSubList) << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "AdaRein_SSS_B.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_sss_b.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms maxSkipPre= " + Util::Int2String(adarein_sss_b.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_SSS_B= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_SSS_B.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_SSS_C(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_sss_c(AdaRein_SSS_C);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_sss_c.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_SSS_C Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein_sss_c.static_succession_selection_crossed(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_SSS_C Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_sss_c.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_SSS_C: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_SSS_C Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_sss_c.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_sss_c.approx_match_sss_c(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_SSS_C Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "falseMatchNum= " << Util::Average(matchSubList) << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / Util::Average(matchSubList) << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "AdaRein_SSS_C.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_sss_c.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms maxSkipPre= " + Util::Int2String(adarein_sss_c.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_SSS_C= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_SSS_C.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_SSS_C_W(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_sss_c_w(AdaRein_SSS_C_W);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_sss_c_w.insert_sss_c_w(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_SSS_C_W Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	adarein_sss_c_w.static_succession_selection_crossed_width(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_SSS_C_W Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_sss_c_w.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_SSS_C_W: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_SSS_C_W Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_sss_c_w.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_sss_c_w.approx_match_sss_c_w(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_SSS_C_W Event " << i << " is matched.\n";
	}

	double falseAvgMatchNum = Util::Average(matchSubList);

#ifdef DEBUG
	cout << "falseMatchNum= " << falseAvgMatchNum << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / falseAvgMatchNum << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "AdaRein_SSS_C_W.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_sss_c_w.calMemory_sss_c_w())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(adarein_level)
					 + " maxSkipPre= " + Util::Int2String(adarein_sss_c_w.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / falseAvgMatchNum)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_SSS_C_W= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_SSS_C_W.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_pAdaRein_SSS_C_W(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein padarein_sss_c_w(pAdaRein_SSS_C_W);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		padarein_sss_c_w.insert_sss_c_w(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "pAdaRein_SSS_C_W Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	padarein_sss_c_w.static_succession_selection_crossed_width(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "pAdaRein_SSS_C_W Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!padarein_sss_c_w.deleteSubscription(gen.subList[kv.first]))
				cout << "pAdaRein_SSS_C_W: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "pAdaRein_SSS_C_W Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			padarein_sss_c_w.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		padarein_sss_c_w.parallel_approx_match_sss_c_w(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "pAdaRein_SSS_C_W Event " << i << " is matched.\n";
	}

	double falseAvgMatchNum = Util::Average(matchSubList);

#ifdef DEBUG
	cout << "falseMatchNum= " << falseAvgMatchNum << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / falseAvgMatchNum << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "pAdaRein_SSS_C_W.txt";
	string content = expID
					 + " memory= " + Util::Int2String(padarein_sss_c_w.calMemory_sss_c_w())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(adarein_level)
					 + " pD= " + Util::Int2String(parallelDegree)
					 + " maxSkipPre= " + Util::Int2String(padarein_sss_c_w.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / falseAvgMatchNum)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pAdaRein_SSS_C_W= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/pAdaRein_SSS_C_W.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_p2AdaRein_SSS_C_W(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein p2adarein_sss_c_w(p2AdaRein_SSS_C_W);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		p2adarein_sss_c_w.insert_sss_c_w(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "p2AdaRein_SSS_C_W Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	p2adarein_sss_c_w.parallel2_static_succession_selection_crossed_width(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "p2AdaRein_SSS_C_W Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!p2adarein_sss_c_w.deleteSubscription(gen.subList[kv.first]))
				cout << "p2AdaRein_SSS_C_W: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "p2AdaRein_SSS_C_W Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			p2adarein_sss_c_w.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		p2adarein_sss_c_w.parallel2_approx_match_sss_c_w(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "p2AdaRein_SSS_C_W Event " << i << " is matched.\n";
	}

	double falseAvgMatchNum = Util::Average(matchSubList);

#ifdef DEBUG
	cout << "falseMatchNum= " << falseAvgMatchNum << ", realFalsePositiveRate= " \
 << 1 - realMatchNum / falseAvgMatchNum << ", matchTime= " \
 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";
	// output
	string outputFileName = "p2AdaRein_SSS_C_W.txt";
	string content = expID
					 + " memory= " + Util::Int2String(p2adarein_sss_c_w.calMemory_sss_c_w())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(adarein_level)
					 + " pD= " + Util::Int2String(parallelDegree)
					 + " maxSkipPre= " + Util::Int2String(p2adarein_sss_c_w.maxSkipPredicate)
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / falseAvgMatchNum)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "p2AdaRein_SSS_C_W= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/p2AdaRein_SSS_C_W.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_DSS_W(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_dss_w(AdaRein_DSS_W);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_dss_w.insert_dss_w(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_DSS_W Insertion Finishes.\n";

	double initTime = 0.0;
	Timer initStart;
	adarein_dss_w.dynamic_succession_selection_width(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_DSS_W Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_dss_w.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_DSS_W: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_DSS_W Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_dss_w.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_dss_w.approx_match_dss_w(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_DSS_W Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "numSkipPredicate= " << adarein_dss_w.numSkipPredicateInTotal / pubs \
 << ", numSkipAttr= " << adarein_dss_w.numSkipAttsInTotal / pubs \
 << ", totalSkipBkt= " << adarein_dss_w.numSkipAttsInTotal / pubs << "*2*" << buks << " + "
		 << adarein_dss_w.numSkipBuckInTotal / pubs \
 << " = " << adarein_dss_w.numSkipAttsInTotal / pubs * 2 * buks + adarein_dss_w.numSkipBuckInTotal / pubs \
 << " among " << atts * 2 * buks << " buckets.\n"\
 << "falseMatchNum= " << Util::Average(matchSubList) \
 << ", realFalsePositiveRate= " << 1 - realMatchNum / Util::Average(matchSubList)
		 << ", matchTime= " << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";

	// output
	string outputFileName = "AdaRein_DSS_W.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_dss_w.calMemory_dss_w())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms level= " + Util::Int2String(adarein_level)
					 + " maxSkipPre= " + Util::Int2String(adarein_dss_w.maxSkipPredicate)
					 + " numSkipPre= " + Util::Int2String((int)(adarein_dss_w.numSkipPredicateInTotal / pubs))
					 + " numSkipBkt= " + Util::Int2String((int)(adarein_dss_w.numSkipBuckInTotal / pubs))
					 + " numSkipAtt= " + Util::Int2String((int)(adarein_dss_w.numSkipAttsInTotal / pubs))
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_DSS_W= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_DSS_W.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AdaRein_DSS_B(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AdaRein adarein_dss_b(AdaRein_DSS_B);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		adarein_dss_b.insert_dss_b(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "AdaRein_DSS_B Insertion Finishes.\n";

	double initTime = 0.0;
	Timer initStart;
	adarein_dss_b.dynamic_succession_selection_backward(falsePositiveRate, gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "AdaRein_DSS_B Skipping Task Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!adarein_dss_b.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein_DSS_B: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "AdaRein_DSS_B Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			adarein_dss_b.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		adarein_dss_b.approx_match_dss_b(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AdaRein_DSS_B Event " << i << " is matched.\n";
	}

#ifdef DEBUG
	cout << "numSkipPredicate= " << adarein_dss_b.numSkipPredicateInTotal / pubs \
 << ", numSkipAttr= " << adarein_dss_b.numSkipAttsInTotal / pubs \
 << ", totalSkipBkt= " << adarein_dss_b.numSkipAttsInTotal / pubs << "*2*" << buks << " + "
		 << adarein_dss_b.numSkipBuckInTotal / pubs \
 << " = " << adarein_dss_b.numSkipAttsInTotal / pubs * 2 * buks + adarein_dss_b.numSkipBuckInTotal / pubs \
 << " among " << atts * 2 * buks << " buckets.\n"\
 << "\nfalseMatchNum= " << Util::Average(matchSubList) \
 << ", realFalsePositiveRate= " << 1 - realMatchNum / Util::Average(matchSubList)
		 << ", matchTime= " << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << "\n";

	// output
	string outputFileName = "AdaRein_DSS_B.txt";
	string content = expID
					 + " memory= " + Util::Int2String(adarein_dss_b.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms maxSkipPre= " + Util::Int2String(adarein_dss_b.maxSkipPredicate)
					 + " numSkipPre= " + Util::Int2String((int)(adarein_dss_b.numSkipPredicateInTotal / pubs))
					 + " numSkipBkt= " + Util::Int2String((int)(adarein_dss_b.numSkipBuckInTotal / pubs))
					 + " numSkipAtt= " + Util::Int2String((int)(adarein_dss_b.numSkipAttsInTotal / pubs))
					 + " fPR= " + Util::Double2String(falsePositiveRate)
					 + " realfPR= " + Util::Double2String(1 - realMatchNum / Util::Average(matchSubList))
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein_DSS_B= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AdaRein_DSS_B.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ
void run_HEM(const intervalGenerator& gen)
{
	HEM hem;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem.match_debug(gen.pubList[i], matchSubs);
#else
		hem.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM Event " << i << " is matched.\n";
	}
	cout << endl;

	if (display)
		hem.printRelation();
	// output
	string outputFileName = "HEM0.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be == -1 ? be2 : be)
					 + " memory= " + Util::Int2String(hem.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM0PS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	//outputFileName = "HEMBucketSize.txt";
	//hem.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem.numBucket)
	//	content += " " + to_string(hem.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM0.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ
void run_HEM1(const intervalGenerator& gen)
{
	HEM1 hem1;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem1.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM1 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem1.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem1.match_debug(gen.pubList[i], matchSubs);
#else
		hem1.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM1 Event " << i << " is matched.\n";
	}

	if (display)
		hem1.printRelation();
	cout << endl;

	// output
	string outputFileName = "HEM1.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem1.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem1.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem1.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem1.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem1.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem1.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM1SS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	//outputFileName = "HEM1BucketSize.txt";
	//hem.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem.numBucket)
	//	content += " " + to_string(hem.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM1.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ
void run_HEM2(const intervalGenerator& gen)
{
	HEM2 hem2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM2 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem2.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem2.match_debug(gen.pubList[i], matchSubs);
#else
		hem2.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM2 Event " << i << " is matched.\n";
	}

	if (display)
		hem2.printRelation(0);
	cout << endl;

	// output
	string outputFileName = "HEM2.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem2.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem2.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem2.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem2.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem2.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem2.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM2SD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	//outputFileName = "HEM2BucketSize.txt";
	//hem.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem.bucketSub.begin(), hem.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem.numBucket)
	//	content += " " + to_string(hem.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEM2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ
void run_HEM3(const intervalGenerator& gen)
{
	HEM3 hem3;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem3.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM3 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem3.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem3.match_debug(gen.pubList[i], matchSubs);
#else
		hem3.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
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
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM3PD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	//outputFileName = "HEM3BucketSize.txt";
	//hem.calBucketSize();
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

// ����ģʽ
void run_HEM4(const intervalGenerator& gen)
{
	HEM4 hem4;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem4.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM4DS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem4.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem4.match_debug(gen.pubList[i], matchSubs);
#else
		hem4.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
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
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM4DS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

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

// ����ģʽ
void run_HEM5(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5 hem5(HEM5_DD);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5.insert_online(gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
#ifdef DEBUG
		hem5.match_debug(gen.pubList[i], matchSubs);
#else
		hem5.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD Event " << i << " is matched.\n";
	}

	if (display)
		hem5.printRelation(1);

#ifdef DEBUG
	cout << "HEM5DD: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " AvgCmpTime= " + to_string(hem5.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem5.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem5.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5.bitTime / pubs / 1000000)
					 + " ms avx= " + to_string(blockSize)
					 + " numBuk= " + Util::Int2String(hem5.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

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

// ����ģʽ + avxָ��
void run_HEM5_avxOR(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5_avxOR hem5_avxor;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_avxor.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD_avxOR Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_avxor.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_avxor.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD_avxOR: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD_avxOR Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_avxor.insert_online(
				gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem5_avxor.match_debug(gen.pubList[i], matchSubs);
#else
		hem5_avxor.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD_avxOR Event " << i << " is matched.\n";
	}

	if (display)
		hem5_avxor.printRelation(1);

#ifdef DEBUG
	cout << "HEM5DD_avxOR: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " ms avx= " + to_string(blockSize)
					 + " numBuk= " + Util::Int2String(hem5_avxor.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD_avxOR= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

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

void run_HEM5_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5 hem5_p(HEM5_DD_PARALLEL);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_p.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD-Parallel Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_p.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_p.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD-Parallel: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD-Parallel Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_p.insert_online(gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
		hem5_p.match_parallel(gen.pubList[i], matchSubs);
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD-Parallel Event " << i << " is matched.\n";
	}

	if (display)
		hem5_p.printRelation(1);

#ifdef DEBUG
	cout << "HEM5DD-Parallel: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "pHEM5.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_p.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " ms MergeTime= " + to_string(hem5_p.mergeTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_p.bitTime / pubs / 1000000)
					 + " ms pD= " + to_string(parallelDegree)
					 + " numBuk= " + Util::Int2String(hem5_p.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD_Parallel= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/pHEM5.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_HEM5_avxOR_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5 hem5_avxOR_p(HEM5_DD_AVXOR_PARALLEL);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_avxOR_p.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD-axvOR-Parallel Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_avxOR_p.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_avxOR_p.deleteSubscription(gen.subList[kv.first]))
				cout << "HEM5DD-avxOR-Parallel: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD-avxOR-Parallel Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_avxOR_p.insert_online(
				gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
		hem5_avxOR_p.match_avxOR_parallel(gen.pubList[i], matchSubs);
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD-avxOR-Parallel Event " << i << " is matched.\n";
	}

	if (display)
		hem5_avxOR_p.printRelation(1);
	cout << endl;

#ifdef DEBUG
	cout << "HEM5DD-avxOR-Parallel: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif

	// output
	string outputFileName = "pHEM5_avxOR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_avxOR_p.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " ms MergeTime= " + to_string(hem5_avxOR_p.mergeTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_avxOR_p.bitTime / pubs / 1000000)
					 + " ms pD= " + to_string(parallelDegree)
					 + " avx= " + to_string(blockSize)
					 + " numBuk= " + Util::Int2String(hem5_avxOR_p.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pHEM5DD_avxOR= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/pHEM5_avxOR.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ + ��������(�¼��������Էֲ�������)�汾
void run_HEM5_VAS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5_AS hem5_vas(HEM5_DD_VAS);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_vas.insert_VAS(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD_VAS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_vas.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_vas.deleteSubscription_VAS(gen.subList[kv.first]))
				cout << "HEM5DD_VAS: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD_VAS Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_vas.insert_online_VAS(
				gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
		hem5_vas.match_VAS(gen.pubList[i], matchSubs);
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD_VAS Event " << i << " is matched.\n";
	}

	if (display)
		hem5_vas.printRelation(1);
	cout << endl;

#ifdef DEBUG
	cout << "HEM5DD_VAS: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif

	// output
	string outputFileName = "HEM5_VAS.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_vas.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " AvgCmpTime= " + to_string(hem5_vas.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem5_vas.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem5_vas.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_vas.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem5_vas.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD_VAS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/HEM5_VAS.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ + ʵ������(�����¼������ĵ�����������ĳ����������)�汾
void run_HEM5_RAS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5_AS hem5_ras(HEM5_DD_RAS);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_ras.insert_RAS(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD_RAS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_ras.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_ras.deleteSubscription_RAS(gen.subList[kv.first]))
				cout << "HEM5DD_RAS: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD_RAS Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_ras.insert_online_RAS(
				gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
		hem5_ras.match_RAS(gen.pubList[i], matchSubs);
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD_RAS Event " << i << " is matched.\n";
	}

	if (display)
		hem5_ras.printRelation(1);
	cout << endl;

#ifdef DEBUG
	cout << "HEM5DD_RAS: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif

	// output
	string outputFileName = "HEM5_RAS.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_ras.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " AvgCmpTime= " + to_string(hem5_ras.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem5_ras.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem5_ras.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_ras.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem5_ras.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEM5DD_RAS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/HEM5_RAS.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ����ģʽ + ʵ�����Ӽ� + avx2 + ����
void run_HEM5_RAS_avxOR_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	HEM5_AS hem5_ras_a_p(HEM5_DD_RAS_AVXOR_PARALLEL);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchInstructionList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem5_ras_a_p.insert_RAS(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM5DD_RAS_avxOR_Parallel Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem5_ras_a_p.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!hem5_ras_a_p.deleteSubscription_RAS(gen.subList[kv.first]))
				cout << "HEM5DD_RAS_avxOR_Parallel: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "HEM5DD_RAS_avxOR_Parallel Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			hem5_ras_a_p.insert_online_RAS(
				gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		int64_t begin = GetCPUCycle();
		hem5_ras_a_p.match_RAS_avxOR_parallel(gen.pubList[i], matchSubs);
		matchInstructionList.push_back(GetCPUCycle() - begin);
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM5DD_RAS_avxOR_Parallel" + to_string(blockSize) + "-Parallel: Event " << i << " is matched.\n";
	}

	if (display)
		hem5_ras_a_p.printRelation(1);
	cout << endl;

#ifdef DEBUG
	cout << "HEM5DD_RAS_avxOR_Parallel: matchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif

	// output
	string outputFileName = "pHEM5_RAS_avxOR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem5_ras_a_p.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgMatchInst= " + Util::Double2String(Util::Average(matchInstructionList))
					 + " ms MergeTime= " + to_string(hem5_ras_a_p.mergeTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem5_ras_a_p.bitTime / pubs / 1000000)
					 + " ms pD= " + to_string(parallelDegree)
					 + " avx= " + to_string(blockSize)
					 + " numBuk= " + Util::Int2String(hem5_ras_a_p.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pHEM5DD_RAS_avxOR= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/pHEM5_RAS_avxOR.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// HEM ����ģʽ + ���ݿ��ȷֲ�
void run_HEMSC(const intervalGenerator& gen)
{
	HEMSC hem_sc;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem_sc.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM-SC-DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem_sc.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem_sc.match_debug(gen.pubList[i], matchSubs);
#else
		hem_sc.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM-SC-DD Event " << i << " is matched.\n";
	}

	if (display)
		hem_sc.printRelation(1, 2);
	cout << endl;

	// output
	string outputFileName = "HEMSC.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem_sc.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem_sc.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem_sc.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem_sc.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem_sc.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(hem_sc.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " lvls=" + Util::Int2String(lvls);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEMSCDD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	//outputFileName = "HEMSCBucketSize.txt";
	//hem_sc.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(hem_sc.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(hem_sc.bucketSub.begin(), hem_sc.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(hem_sc.bucketSub.begin(), hem_sc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(hem_sc.bucketSub.begin(), hem_sc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, hem_sc.numBucket)
	//	content += " " + to_string(hem_sc.bucketSub[i].size());
	//Util::WriteData2Begin(outputFileName.c_str(), content);

	outputFileName = "tmpData/HEMSC.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ״̬ѹ��
void run_HEMSR(const intervalGenerator& gen)
{
	HEMSR hem_sr;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		hem_sr.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "HEM-SR-PS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	hem_sr.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
#ifdef DEBUG
		hem_sr.match_debug(gen.pubList[i], matchSubs);
#else
		hem_sr.match(gen.pubList[i], matchSubs);
#endif // DEBUG
		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "HEM-SR-PS Event " << i << " is matched.\n";
	}

	if (display)
		hem_sr.printRelation();
	cout << endl;

	// output
	string outputFileName = "HEMSR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(hem_sr.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(hem_sr.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(hem_sr.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(hem_sr.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(hem_sr.bitTime / pubs / 1000000)
					 + " ms GroupSize=" + Util::Int2String(gs)
					 + " numBuk= " + Util::Int2String(hem_sr.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "HEMSRPS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif // DEBUG

	outputFileName = "tmpData/HEMSR.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// �����ж�
void run_Simple(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Simple simple;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		simple.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Simple Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!simple.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Simple Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			simple.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		dPub dpub;
		dpub.pubId = i;
		Util::Pub2dPub(gen.pubList[i], dpub);

		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		simple.match(dpub, matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
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
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Simple= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/Simple.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

// ��ν�ʿ�����������+�����ж�
void run_Simple2(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Simple2 simple2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		simple2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Simple2 Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!simple2.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple2: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Simple2 Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			simple2.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		dPub dpub;
		dpub.pubId = i;
		Util::Pub2dPub(gen.pubList[i], dpub);

		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		simple2.match(dpub, matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
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
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Simple2= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/Simple2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	//	printf("123\n");
	//	fflush(stdout);
	Tama tama(TAMA_FORWARD);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		//		printf("Sub %d\n",i);
		//		fflush(stdout);
		tama.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!tama.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			tama.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		tama.match_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "TamaAvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/Tama.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_forward_native_parallel_lock(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Tama pTama_l(TAMA_PARALLEL_LOCK);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		pTama_l.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama-Parallel-" << parallelDegree << "-Lock: Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!pTama_l.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama-Parallel-" << parallelDegree << "-Lock: sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama-Parallel-" << parallelDegree << "-Lock:  Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			pTama_l.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		pTama_l.match_parallel_lock(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama-Parallel-" << parallelDegree << "-Lock: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama-Parallel" << parallelDegree << "-Lock: AvgMatchNum= " << Util::Average(matchSubList)
		 << ", matchTime= " << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "pTama_Lock.txt";
	string content = expID
					 + " memory= " + Util::Int2String(pTama_l.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms pD= " + to_string(parallelDegree)
					 + " level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pTama_Lock= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/pTama_Lock.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_forward_native_parallel_reduce(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	Tama pTama_r(TAMA_PARALLEL_REDUCE);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		pTama_r.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama-Parallel-" << parallelDegree << "-Reduce: Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!pTama_r.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama-Parallel-" << parallelDegree << "-Reduce: sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama-Parallel-" << parallelDegree << "-Reduce:  Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			pTama_r.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		pTama_r.match_parallel_reduce(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama-Parallel-" << parallelDegree << "-Reduce: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama-Parallel" << parallelDegree << "-Reduce: AvgMatchNum= " << Util::Average(matchSubList)
		 << ", matchTime= " << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "pTama_Reduce.txt";
	string content = expID
					 + " memory= " + Util::Int2String(pTama_r.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms pD= " + to_string(parallelDegree)
					 + " level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "pTama_Reduce= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/pTama_Reduce.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_forward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bTama btama_fc(TAMA_FORWARD_CBOMP); // bTAMA6

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_fc.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama_CF Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_fc.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama_CF: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama_CF Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			btama_fc.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_fc.match_forward_CBOMP_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama_CF Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama_CF: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama6.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_fc.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama6_CF= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama6.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_forward_C_BOMP_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bTama btama_fcp(TAMA_FORWARD_CBOMP_PARALLEL); // TAMA6-CFP

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_fcp.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama_CFP" << parallelDegree << " Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_fcp.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama_CFP" << parallelDegree << ": sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama_CFP" << parallelDegree << ": Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			btama_fcp.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_fcp.match_forward_CBOMP_parallel(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama_CFP" << parallelDegree << ": Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama_CFP" << parallelDegree << ": AvgMatchNum= " << Util::Average(matchSubList)
		 << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama6_p.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_fcp.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms pD= " + to_string(parallelDegree)
					 + " level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama6_CF_p= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama6_p.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_backward1_DMFT_TAMA_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bTama btama_b1c(TAMA_BACKWARD1_DMFT); // bTAMA7

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_b1c.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama-CB1 Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_b1c.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama-CB1: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama-CB1 Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			btama_b1c.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_b1c.match_backward1_DMFT_TAMA_CBOMP_accurate(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama-CB1 Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama-CB1: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama7.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_b1c.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama7_CB1= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama7.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_backward2_DMFT_TAMA_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bTama btama_b2c(TAMA_BACKWARD2_DMFT); // bTAMA8

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_b2c.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bTama-CB2 Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_b2c.deleteSubscription(gen.subList[kv.first]))
				cout << "bTama_backward2: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama-CB2 Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			btama_b2c.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_b2c.match_backward2_DMFT_TAMA_CBOMP_accurate(gen.pubList[i], matchSubs, gen.subList);
		//tama.match_vague(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama_-CB2 Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "Tama-CB2: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama8.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_b2c.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama8-CB2= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama8.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_tama_backward2_DMFT_TAMA_CBOMP_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bTama btama_b2cp(TAMA_BACKWARD2_DMFT_PARALLEL); // bTAMA8

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_b2cp.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Tama-CB2_parallel: Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_b2cp.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama-CB2_parallel: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "Tama-CB2_parallel: Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			btama_b2cp.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_b2cp.match_backward2_DMFT_TAMA_CBOMP_accurate_parallel(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "Tama-CB2_parallel: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "bTama-CB2_parallel: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama8_p.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_b2cp.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms pD= " + to_string(parallelDegree)
					 + " level= " + Util::Int2String(level)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama8_CB_p= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama8_p.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_btama_backward(const intervalGenerator& gen, unordered_map<int, bool> deleteNo) // bTAMA
{
	bTama btama_b(bTAMA_BACKWARD); // bTAMA-B

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_b.insert_backward(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bTama-B Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_b.deleteSubscription(gen.subList[kv.first])) // unimplemented
				cout << "bTama-B: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama-B Deletion Finishes.\n";
		for (auto kv : deleteNo)
			btama_b.insert_backward(gen.subList[kv.first]);
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_b.match_backward_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bTama-B Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "bTama-B: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_b.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bTama-B= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama_B.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_btama_forward_DMFT(const intervalGenerator& gen, unordered_map<int, bool> deleteNo) // bTAMA-F
{
	bTama btama_f(bTAMA_FORWARD_DMFT); // bTAMA-F

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;
		btama_f.insert_backward(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bTama-F Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!btama_f.deleteSubscription(gen.subList[kv.first])) // unimplemented
				cout << "bTama-F: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bTama-F Deletion Finishes.\n";
		for (auto kv : deleteNo)
			btama_f.insert_backward(gen.subList[kv.first]);
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		btama_f.match_forward_DMFT_bTAMA(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bTama-F Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "bTama-F: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList) << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bTama_F.txt";
	string content = expID
					 + " memory= " + Util::Int2String(btama_f.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bTama-F= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bTama_F.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_OpIndex(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	OpIndex2 opindex2;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// Note: for OpIndex, it needs to calculate frequency first!
	double initTime;
	Timer initStart;
	opindex2.calcFrequency(gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "OpIndex2 CalcFrequency Task Finishes.\n";

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		opindex2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "OpIndex2 Insertion Finishes.\n";

	if (display)
	{// show pivot attribute
		int counter = 0;
		for (int i = 0; i < atts; i++)
		{
			cout << "Att " << i << ": " << opindex2.isPivot[i] << ", ";
			if (opindex2.isPivot[i])counter++;
			if (i > 0 && i % 5 == 0) cout << endl;
		}
		cout << "\nTotal pivot attribute: " << counter << endl;
	}

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!opindex2.deleteSubscription(gen.subList[kv.first]))
				cout << "OpIndex2: sub " << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "OpIndex2 Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			opindex2.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		opindex2.match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "OpIndex2 Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "OpIndex2: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "OpIndex2= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/OpIndex2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_bOpIndex2(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	bOpIndex2 bOpindex2; // Opindex2 with CBOMP

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// Note: for OpIndex, it needs to calculate frequency first!
	double initTime;
	Timer initStart;
	bOpindex2.calcFrequency(gen.subList);
	initTime = (double)initStart.elapsed_nano() / 1000000.0;
	cout << "bOpIndex2 (C-BOMP) CalcFrequency Task Finishes.\n";

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		bOpindex2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bOpIndex2 (C-BOMP) Insertion Finishes.\n";

	if (display)
	{// show pivot attribute
		int counter = 0;
		for (int i = 0; i < atts; i++)
		{
			cout << "Att " << i << ": " << bOpindex2.isPivot[i] << ", ";
			if (bOpindex2.isPivot[i])counter++;
			if (i > 0 && i % 5 == 0) cout << endl;
		}
		cout << "\nTotal pivot attribute: " << counter << endl;
	}

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bOpindex2.deleteSubscription(gen.subList[kv.first]))
				cout << "bOpIndex2 (C-BOMP): sub " << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bOpIndex2 (C-BOMP) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bOpindex2.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		bOpindex2.match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bOpIndex2 (C-BOMP) Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "bOpIndex2 (C-BOMP): AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bOpIndex2= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bOpIndex2.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree bgTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		bgTree.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "BG-Tree Forward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bgTree.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree Forward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bgTree.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree.match_forward_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree Event " << i << " is matched forwardly.\n";
	}
#ifdef DEBUG
	cout << "BG-Tree Forward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	if (display)
		bgTree.printBGTree();
	// output
	string outputFileName = "fBGTree.txt";
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_forward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree bgTree_c;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		bgTree_c.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "BG-Tree(C-BOMP) Forward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bgTree_c.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree(C-BOMP) forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "BG-Tree(C-BOMP) Forward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bgTree_c.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bgTree_c.match_forward_CBOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree(C-BOMP) Event " << i << " is matched forwardly.\n";
	}
#ifdef DEBUG
	cout << "BG-Tree(C-BOMP) Forward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fBGTree_C.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bgTree_c.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bgTree_c.getHeight())
					 + " numNode = " + Util::Int2String(bgTree_c.getNumNode())
					 + " BNS = " + Util::Int2String(bgTree_c.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bgTree_c.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree_C= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree_C.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_backward_DMFT_fBGTree_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree fBGTree_cb;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto&& sub : gen.subList)
	{
		Timer insertStart;

		fBGTree_cb.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fBG-Tree(C-BOMP) Backward (DMFT) Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fBGTree_cb.deleteSubscription(gen.subList[kv.first]))
				cout << "fBG-Tree(C-BOMP) Backward (DMFT): sub" << gen.subList[kv.first].id
					 << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fBG-Tree(C-BOMP) Backward (DMFT) Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fBGTree_cb.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fBGTree_cb.match_backward_DMFT_fBGTree_CBOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fBG-Tree-CB  Event " << i << " is matched backwardly.\n";
	}
#ifdef DEBUG
	cout << "fBG-Tree-CB: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "fBGTree_CB.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fBGTree_cb.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(fBGTree_cb.getHeight())
					 + " numNode = " + Util::Int2String(fBGTree_cb.getNumNode())
					 + " BNS = " + Util::Int2String(fBGTree_cb.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(fBGTree_cb.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree_CB= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree_CB.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_backward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree bBGTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto&& sub : gen.subList)
	{
		Timer insertStart;

		bBGTree.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bBGTree Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bBGTree.deleteSubscription(gen.subList[kv.first]))
				cout << "bBGTree: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bBGTree Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bBGTree.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bBGTree.match_backward_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bBGTree  Event " << i << " is matched backwardly.\n";
	}
#ifdef DEBUG
	cout << "bBGTree: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bBGTree.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bBGTree.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bBGTree.getHeight())
					 + " numNode = " + Util::Int2String(bBGTree.getNumNode())
					 + " BNS = " + Util::Int2String(bBGTree.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bBGTree.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bBGTree= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bBGTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_forward_DMFT_bBGTree(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree bBGTree_F;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto&& sub : gen.subList)
	{
		Timer insertStart;

		bBGTree_F.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bBGTree_F Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bBGTree_F.deleteSubscription(gen.subList[kv.first]))
				cout << "bBGTree_F: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bBGTree_F Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bBGTree_F.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bBGTree_F.match_forward_DMFT_bBGTree(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bBGTree_F  Event " << i << " is matched backwardly.\n";
	}
#ifdef DEBUG
	cout << "bBGTree_F: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "bBGTree_F.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bBGTree_F.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bBGTree_F.getHeight())
					 + " numNode = " + Util::Int2String(bBGTree_F.getNumNode())
					 + " BNS = " + Util::Int2String(bBGTree_F.getBoundaryNumSub())
					 + " AvgHit = " + Util::Int2String(bBGTree_F.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bBGTree_F= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bBGTree_F.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree_d fBGTree_d;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fBGTree_d.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fBG-Tree_d Forward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fBGTree_d.deleteSubscription(gen.subList[kv.first]))
				cout << "BG-Tree_d Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fBG-Tree_d Forward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fBGTree_d.insert(gen.subList[kv.first], gen.subList);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fBGTree_d.forward_match_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree_d Event " << i << " is matched forwardly.\n";
	}
#ifdef DEBUG
	cout << "BG-Tree_d Forward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	if (display)
		fBGTree_d.printBGTree();
	// output
	string outputFileName = "fBGTree_d.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fBGTree_d.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(fBGTree_d.getHeight())
					 + " numNode = " + Util::Int2String(fBGTree_d.getNumNode())
					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
					 + " AvgHit = " + Util::Int2String(fBGTree_d.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree_d= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree_d.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_forward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree_d fBGTree_d_C;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fBGTree_d_C.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fBGTree_d_C Forward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fBGTree_d_C.deleteSubscription(gen.subList[kv.first]))
				cout << "fBGTree_d_C Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fBG-Tree_d_C Forward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fBGTree_d_C.insert(gen.subList[kv.first], gen.subList);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fBGTree_d_C.forward_match_C_BOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BG-Tree_d_C Event " << i << " is matched forwardly.\n";
	}
#ifdef DEBUG
	cout << "BG-Tree_d_C Forward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	if (display)
		fBGTree_d_C.printBGTree();
	// output
	string outputFileName = "fBGTree_d_C.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fBGTree_d_C.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(fBGTree_d_C.getHeight())
					 + " numNode = " + Util::Int2String(fBGTree_d_C.getNumNode())
					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
					 + " AvgHit = " + Util::Int2String(fBGTree_d_C.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree_d_C= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree_d_C.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_backward_DMFT_fBGTree_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree_d fBGTree_d_CB;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		fBGTree_d_CB.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "fBGTree_d_CB Forward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!fBGTree_d_CB.deleteSubscription(gen.subList[kv.first]))
				cout << "fBGTree_d_CB Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "fBG-Tree_d_CB Forward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			fBGTree_d_CB.insert(gen.subList[kv.first], gen.subList);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		fBGTree_d_CB.backward_match_DMFT_fBGTree_d_CBOMP(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "fBG-Tree_d_CB Event " << i << " is matched forwardly.\n";
	}
#ifdef DEBUG
	cout << "fBG-Tree_d_CB Forward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	if (display)
		fBGTree_d_CB.printBGTree();
	// output
	string outputFileName = "fBGTree_d_CB.txt";
	string content = expID
					 + " memory= " + Util::Int2String(fBGTree_d_CB.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(fBGTree_d_CB.getHeight())
					 + " numNode = " + Util::Int2String(fBGTree_d_CB.getNumNode())
					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
					 + " AvgHit = " + Util::Int2String(fBGTree_d_CB.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "fBGTree_d_CB= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/fBGTree_d_CB.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_backward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree_d bBGTree_d;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		bBGTree_d.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bBG-Tree_d Backward Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bBGTree_d.deleteSubscription(gen.subList[kv.first]))
				cout << "bBG-Tree_d Backward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bBG-Tree_d Backward Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bBGTree_d.insert(gen.subList[kv.first], gen.subList);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bBGTree_d.backward_match_native(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bBG-Tree_d Event " << i << " is matched backwardly.\n";
	}
#ifdef DEBUG
	cout << "bBG-Tree_d Backward: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	if (display)
		bBGTree_d.printBGTree();
	// output
	string outputFileName = "bBGTree_d.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bBGTree_d.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bBGTree_d.getHeight())
					 + " numNode = " + Util::Int2String(bBGTree_d.getNumNode())
					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
					 + " BNS = " + Util::Int2String(BoundaryNumSub)
					 + " AvgHit = " + Util::Int2String(bBGTree_d.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bBGTree_d= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bBGTree_d.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_forward_DMFT_bBGTree(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	BGTree_d bBGTree_d_F;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		bBGTree_d_F.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "bBG-Tree_d_F Insertion Finishes.\n";

	if (verifyID)
	{
		for (auto kv : deleteNo)
		{
			Timer deleteStart;
			if (!bBGTree_d_F.deleteSubscription(gen.subList[kv.first]))
				cout << "bBG-Tree_d_F: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
		}
		cout << "bBG-Tree_d_F Deletion Finishes.\n";
		for (auto kv : deleteNo)
		{
			bBGTree_d_F.insert(gen.subList[kv.first], gen.subList);
		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		bBGTree_d_F.forward_match_DMFT_bBGTree_d(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "bBG-Tree_d_F Event " << i << " is matched backwardly.\n";
	}
#ifdef DEBUG
	cout << "bBG-Tree_d_F: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList)) << " ms\n";
#endif
	cout << endl;

	if (display)
		bBGTree_d_F.printBGTree();
	// output
	string outputFileName = "bBGTree_d_F.txt";
	string content = expID
					 + " memory= " + Util::Int2String(bBGTree_d_F.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms height = " + Util::Int2String(bBGTree_d_F.getHeight())
					 + " numNode = " + Util::Int2String(bBGTree_d_F.getNumNode())
					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
					 + " BNS = " + Util::Int2String(BoundaryNumSub)
					 + " AvgHit = " + Util::Int2String(bBGTree_d_F.hit / pubs)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "bBGTree_d_F= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/bBGTree_d_F.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_vrs_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	//	BGTree_d_vrs bgTree_d_vrs;
	//
	//	vector<double> insertTimeList;
	//	vector<double> deleteTimeList;
	//	vector<double> matchTimeList;
	//	vector<double> matchSubList;
	//
	//	// insert
	//	for (int i = 0; i < subs; i++) {
	//		Timer insertStart;
	//
	//		bgTree_d_vrs.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.
	//
	//		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
	//		insertTimeList.push_back((double) insertTime / 1000000);
	//	}
	//	cout << "BG-Tree_d_vrs Forward Insertion Finishes.\n";
	//
	//	// ��֤����ɾ����ȷ��
	//	if (verifyID) {
	//		for (auto kv: deleteNo) {
	//			Timer deleteStart;
	//			if (!bgTree_d_vrs.deleteSubscription(gen.subList[kv.first]))
	//				cout << "BG-Tree_d_vrs Forward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
	//			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
	//		}
	//		cout << "BG-Tree_d_vrs Forward Deletion Finishes.\n";
	//		for (auto kv: deleteNo) {
	//			bgTree_d_vrs.insert(gen.subList[kv.first],gen.subList);
	//		}
	//	}
	//
	//	// match
	//	for (int i = 0; i < pubs; i++) {
	//		int matchSubs = 0; // Record the number of matched subscriptions.
	//
	//		Timer matchStart;
	//
	//		bgTree_d_vrs.match_forward_native(gen.pubList[i], matchSubs, gen.subList);
	//
	//		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
	//		matchTimeList.push_back((double) eventTime / 1000000);
	//		matchSubList.push_back(matchSubs);
	//		if (i % interval == 0)
	//			cout << "BG-Tree_d_vrs Event " << i << " is matched forwardly.\n";
	//	}
	//	cout << endl;
	//
	//	if (display)
	//		bgTree_d_vrs.printBGTree();
	//	// output
	//	string outputFileName = "BGTree_d_vrs_f.txt";
	//	string content = expID
	//					 + " memory= " + Util::Int2String(bgTree_d_vrs.calMemory())
	//					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
	//					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
	//					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
	//					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
	//					 + " ms height = " + Util::Int2String(bgTree_d_vrs.getHeight())
	//					 + " numNode = " + Util::Int2String(bgTree_d_vrs.getNumNode())
	//					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
	//					 + " AvgHit = " + Util::Int2String(bgTree_d_vrs.hit / pubs)
	//					 + " numSub= " + Util::Int2String(subs)
	//					 + " subSize= " + Util::Int2String(cons)
	//					 + " numPub= " + Util::Int2String(pubs)
	//					 + " pubSize= " + Util::Int2String(m)
	//					 + " attTypes= " + Util::Int2String(atts)
	//                   + " attGroup= " + Util::Int2String(attrGroup)
	//                   +" attNumType= " + Util::Int2String(attNumType)
	//					 + " attDis= " + Util::Int2String(attDis)
	//					 + " valDis= " + Util::Int2String(valDis)
	//					 + " width= " + Util::Double2String(width)
	//					 + " alpha= " + Util::Double2String(alpha)
	//					 + " subp= " + Util::Double2String(subp)
	//					 + " mean= " + Util::Double2String(mean)
	//					 + " stddev= " + Util::Double2String(stddev)
	//					 + " valDom= " + Util::Double2String(valDom);
	//	Util::WriteData2Begin(outputFileName.c_str(), content);
	//
	//#ifdef DEBUG
	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BGTree_d_vrs_f= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);
	//#endif
	//
	//	outputFileName = "tmpData/BGTree_d_vrs_f.txt";
	//	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	//	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_BGTREE_d_vrs_backward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	//	BGTree_d_vrs bgTree_d_vrs;
	//
	//	vector<double> insertTimeList;
	//	vector<double> deleteTimeList;
	//	vector<double> matchTimeList;
	//	vector<double> matchSubList;
	//
	//	// insert
	//	for (int i = 0; i < subs; i++) {
	//		Timer insertStart;
	//
	//		bgTree_d_vrs.insert(gen.subList[i], gen.subList); // Insert sub[i] into data structure.
	//
	//		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
	//		insertTimeList.push_back((double) insertTime / 1000000);
	//	}
	//	cout << "BG-Tree_d_vrs Backward Insertion Finishes.\n";
	//
	//	// ��֤����ɾ����ȷ��
	//	if (verifyID) {
	//		for (auto kv: deleteNo) {
	//			Timer deleteStart;
	//			if (!bgTree_d_vrs.deleteSubscription(gen.subList[kv.first]))
	//				cout << "BG-Tree_d_vrs Backward: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
	//			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
	//		}
	//		cout << "BG-Tree_d_vrs Backward Deletion Finishes.\n";
	//		for (auto kv: deleteNo) {
	//			bgTree_d_vrs.insert(gen.subList[kv.first],gen.subList);
	//		}
	//	}
	//
	//	// match
	//	for (int i = 0; i < pubs; i++) {
	//		int matchSubs = 0; // Record the number of matched subscriptions.
	//
	//		Timer matchStart;
	//
	//		bgTree_d_vrs.match_backward_native(gen.pubList[i], matchSubs, gen.subList);
	//
	//		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
	//		matchTimeList.push_back((double) eventTime / 1000000);
	//		matchSubList.push_back(matchSubs);
	//		if (i % interval == 0)
	//			cout << "BG-Tree_d_vrs Event " << i << " is matched backwardly.\n";
	//	}
	//	cout << endl;
	//
	//	if (display)
	//		bgTree_d_vrs.printBGTree();
	//	// output
	//	string outputFileName = "BGTree_d_vrs_b.txt";
	//	string content = expID
	//					 + " memory= " + Util::Int2String(bgTree_d_vrs.calMemory())
	//					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
	//					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
	//					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
	//					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
	//					 + " ms height = " + Util::Int2String(bgTree_d_vrs.getHeight())
	//					 + " numNode = " + Util::Int2String(bgTree_d_vrs.getNumNode())
	//					 + "  maxNodeSize = " + Util::Int2String(MAXNodeSIZE)
	//					 + " BNS = " + Util::Int2String(BoundaryNumSub)
	//					 + " AvgHit = " + Util::Int2String(bgTree_d_vrs.hit / pubs)
	//					 + " numSub= " + Util::Int2String(subs)
	//					 + " subSize= " + Util::Int2String(cons)
	//					 + " numPub= " + Util::Int2String(pubs)
	//					 + " pubSize= " + Util::Int2String(m)
	//					 + " attTypes= " + Util::Int2String(atts)
	//                   + " attGroup= " + Util::Int2String(attrGroup)
	//                   +" attNumType= " + Util::Int2String(attNumType)
	//					 + " attDis= " + Util::Int2String(attDis)
	//					 + " valDis= " + Util::Int2String(valDis)
	//					 + " width= " + Util::Double2String(width)
	//					 + " alpha= " + Util::Double2String(alpha)
	//					 + " subp= " + Util::Double2String(subp)
	//					 + " mean= " + Util::Double2String(mean)
	//					 + " stddev= " + Util::Double2String(stddev)
	//					 + " valDom= " + Util::Double2String(valDom);
	//	Util::WriteData2Begin(outputFileName.c_str(), content);
	//
	//#ifdef DEBUG
	//	outputFileName = "ComprehensiveExpTime.txt";
	//	content = "BGTree_d_vrs_b= [";
	//	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	//	content[content.length() - 2] = ']';
	//	Util::WriteData2Begin(outputFileName.c_str(), content);
	//#endif
	//
	//	outputFileName = "tmpData/BGTree_d_vrs_b.txt";
	//	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	//	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_PSTREE(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	PSTree psTree;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	Sub2 sub; // ��Ҫת��һ��
	Cnt2 cnt2;
	cnt2.op = 3;
	vector<Sub2> subList;
	for (auto& iSub : gen.subList)
	{
		sub.id = iSub.id;
		sub.size = iSub.size;
		sub.constraints.resize(0);
		for (auto& iCnt : iSub.constraints)
		{
			cnt2.att = iCnt.att;
			cnt2.value[0] = iCnt.lowValue;
			cnt2.value[1] = iCnt.highValue;
			sub.constraints.push_back(cnt2);
		}
		subList.push_back(sub);
	}

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer insertStart;

		psTree.insert(subList[i]);

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);

		if (i % 200000 == 0)
		{
			cout << "PS-Tree Insert sub " << i << endl;
		}
	}
	cout << "PS-Tree Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
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

	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		psTree.MatchEvent(gen.pubList[i], matchSubs, subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "PS-Tree Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "PS-Tree: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "PSTB.txt";
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "PS-Tree= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/PSTB.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AWBTREE(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AWBTree awbTree(AWBPTREE);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto&& sub : gen.subList)
	{
		Timer insertStart;

		awbTree.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	awbTree.setbits();
	cout << "AWB+Tree Hybrid_opt Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
//		for (auto kv : deleteNo) {
//			Timer deleteStart;
//			if (!awbTree.deleteSubscription(gen.subList[kv.first]))
//				cout << "AWB+Tree Hybrid_opt: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
//			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
//		}
//		cout << "AWB+Tree Hybrid_opt Deletion Finishes.\n";
//		for (auto kv : deleteNo) {
//			awbTree.insert(gen.subList[kv.first]);
//		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		awbTree.hybrid(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AWB+Tree Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "AWB+Tree: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AWBTree= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AWBTree.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void run_AWBTREE_parallel(const intervalGenerator& gen, unordered_map<int, bool> deleteNo)
{
	AWBTree awbTree_p(AWBPTREE_PARALLEL);

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (auto&& sub : gen.subList)
	{
		Timer insertStart;

		awbTree_p.insert(sub); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	awbTree_p.setbits();
	cout << "AWB+Tree parallel: Insertion Finishes.\n";

	// ��֤����ɾ����ȷ��
	if (verifyID)
	{
//		for (auto kv : deleteNo) {
//			Timer deleteStart;
//			if (!awbTree_p.deleteSubscription(gen.subList[kv.first]))
//				cout << "AWB+Tree Hybrid_opt: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
//			deleteTimeList.push_back((double)deleteStart.elapsed_nano() / 1000000);
//		}
//		cout << "AWB+Tree Hybrid_opt Deletion Finishes.\n";
//		for (auto kv : deleteNo) {
//			awbTree_p.insert(gen.subList[kv.first]);
//		}
	}

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.

		Timer matchStart;

		awbTree_p.hybrid_p(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "AWB+Tree parallel: Event " << i << " is matched.\n";
	}
#ifdef DEBUG
	cout << "AWB+Tree parallel: AvgMatchNum= " << Util::Average(matchSubList) << ", matchTime= "
		 << Util::Double2String(Util::Average(matchTimeList))
		 << " ms\n";
#endif
	cout << endl;

	// output
	string outputFileName = "AWBTree_p.txt";
	string content = expID
					 + " memory= " + Util::Int2String(awbTree_p.calMemory())
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
					 + " attGroup= " + Util::Int2String(attrGroup)
					 + " attNumType= " + Util::Int2String(attNumType)
					 + " attDis= " + Util::Int2String(attDis)
					 + " valDis= " + Util::Int2String(valDis)
					 + " width= " + Util::Double2String(width)
					 + " alpha= " + Util::Double2String(alpha)
					 + " subp= " + Util::Double2String(subp)
					 + " mean= " + Util::Double2String(mean)
					 + " stddev= " + Util::Double2String(stddev)
					 + " valDom= " + Util::Double2String(valDom);
	Util::WriteData2Begin(outputFileName.c_str(), content);

#ifdef DEBUG
	outputFileName = "ComprehensiveExpTime.txt";
	content = "AWBTree_p= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData2Begin(outputFileName.c_str(), content);
#endif

	outputFileName = "tmpData/AWBTree_p.txt";
	content = Util::Double2String(Util::Average(matchTimeList)) + ", ";
	Util::WriteData2End(outputFileName.c_str(), content);
}

void measure_numMark(const intervalGenerator& gen)
{
	for (be = 0; be < 10; be++)
	{
		Rein rein(OriginalRein);
		HEM5 hem5(HEM5_DD);
		HEM hem0;
		for (int i = 0; i < subs; i++)
		{
			if (be == 0)
				rein.insert_backward_original(gen.subList[i]);
			hem5.insert(gen.subList[i]);
			hem0.insert(gen.subList[i]);
		}

		hem5.initBits();
		hem0.initBits();

		vector<int> reinMarkNum = rein.calMarkNumForBuckets();
		vector<int> hem5MarkNum = hem5.calMarkNumForBuckets();
		vector<int> hem0MarkNum = hem0.calMarkNumForBuckets();

		if (be == 0)
		{
			cout << "rein=[";
			for (int i = 0; i < reinMarkNum.size() - 1; i++)
				cout << reinMarkNum[i] << ", ";
			cout << reinMarkNum.back() << "]\n\n";
		}

		cout << "hem5DDbe" << be << "=[";
		for (int i = 0; i < hem5MarkNum.size() - 1; i++)
			cout << hem5MarkNum[i] << ", ";
		cout << hem5MarkNum.back() << "]\n";

		cout << "hem0be" << be << "=[";
		for (int i = 0; i < hem5MarkNum.size() - 1; i++)
			cout << hem0MarkNum[i] << ", ";
		cout << hem0MarkNum.back() << "]\n";
	}
}
