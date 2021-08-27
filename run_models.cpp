#include "run_models.h"

void run_rein(const intervalGenerator& gen) {
	Rein rein(valDom, subs, atts, buks);

	vector<double> insertTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer subStart;

		rein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = subStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "Rein Insertion Finish.\n";

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rein.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % 100==0)
			cout << "Rein Event" << i << " is matched.\n";
	}

	// output
	string outputFileName = "Rein.txt";
	string content = expID
		+ "\tmemory= " + Util::Int2String(rein.calMemory())
		+ " MB\tAvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ "\tAvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms\tAvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
		+ " ms\tAvgCmpTime= " + to_string(rein.compareTime / pubs / 1000000)
		+ " ms\tAvgMarkTime= " + to_string(rein.markTime / pubs / 1000000)
		+ " ms\tAvgBitTime= " + to_string(rein.bitTime / pubs / 1000000)
		+ " ms\tnumBuk= " + Util::Int2String(rein.numBucket)
		+ "\tnumSub= " + Util::Int2String(subs)
		+ "\tsubSize= " + Util::Int2String(cons)
		+ "\tnumPub= " + Util::Int2String(pubs)
		+ "\tpubSize= " + Util::Int2String(m)
		+ "\tattTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "ReinBucketSize.txt";
	//rein.calBucketSize();
	//content = expID + "\tnumBucket=" + Util::Int2String(rein.numBucket)
	//	//+ "\tsumBukSetSize=" + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ "\tmaxBukSetSize=" + to_string((*max_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ "\tminBukSetSize=" + to_string(min_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + "\tBucketSize:";
	//_for(i, 0, rein.numBucket)
	//	content += " " + to_string(rein.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_ReinBits(const intervalGenerator& gen) {
	ReinBits rb(valDom, subs, atts, buks, bits);

	vector<double> insertTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer subStart;

		rb.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = subStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "ReinBits Insertion Finish.\n";

	double initTime;
	Timer initStart;
	rb.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % 100 == 0)
			cout << "ReinBits Event" << i << " is matched.\n";
	}

	rb.printRelation();
	// output
	string outputFileName = "ReinBits.txt";
	string content = expID
		+ "\tbits=" + Util::Int2String(bits)
		+ "\tmemory=" + Util::Int2String(rb.calMemory())
		+ "MB\tAvgMatchNum=" + Util::Double2String(Util::Average(matchSubList))
		+ "\tAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList))
		+ "ms\tInitTime=" + Util::Double2String(initTime)
		+ "ms\tNewAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
		+ "ms\tAvgMatchTime=" + Util::Double2String(Util::Average(matchTimeList))
		+ "ms\tAvgCmpTime=" + to_string(rb.compareTime / pubs / 1000000)
		+ "ms\tAvgMarkTime=" + to_string(rb.markTime / pubs / 1000000)
		+ "ms\tOrTime=" + to_string(rb.orTime / pubs / 1000000)
		+ "ms\tAvgBitTime=" + to_string(rb.bitTime / pubs / 1000000)
		+ "ms\tnumBuk=" + Util::Int2String(rb.numBucket)
		+ "\tnumSub=" + Util::Int2String(subs)
		+ "\tsubSize=" + Util::Int2String(cons)
		+ "\tnumPub=" + Util::Int2String(pubs)
		+ "\tpubSize=" + Util::Int2String(m)
		+ "\tattTypes=" + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "ReinBitsBucketSize.txt";
	//rb.calBucketSize();
	//content = expID + "\tnumBucket=" + Util::Int2String(rb.numBucket)
	//	//+ "\tsumBukSetSize=" + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ "\tmaxBukSetSize=" + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ "\tminBukSetSize=" + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + "\tBucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

// 静静模式
void run_ReinBits1(const intervalGenerator& gen) {
	ReinBits1 rb1(valDom, subs, atts, buks, bits);

	vector<double> insertTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer subStart;

		rb1.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = subStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "ReinBits1 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb1.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb1.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % 100 == 0)
			cout << "ReinBits1 Event" << i << " is matched.\n";
	}

	rb1.printRelation(0);
	// output
	string outputFileName = "ReinBits1.txt";
	string content = expID
		+ "\tbits=" + Util::Int2String(bits)
		+ "\tmemory=" + Util::Int2String(rb1.calMemory())
		+ "MB\tAvgMatchNum=" + Util::Double2String(Util::Average(matchSubList))
		+ "\tAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList))
		+ "ms\tInitTime=" + Util::Double2String(initTime)
		+ "ms\tNewAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
		+ "ms\tAvgMatchTime=" + Util::Double2String(Util::Average(matchTimeList))
		+ "ms\tAvgCmpTime=" + to_string(rb1.compareTime / pubs / 1000000)
		+ "ms\tAvgMarkTime=" + to_string(rb1.markTime / pubs / 1000000)
		+ "ms\tOrTime=" + to_string(rb1.orTime / pubs / 1000000)
		+ "ms\tAvgBitTime=" + to_string(rb1.bitTime / pubs / 1000000)
		+ "ms\tnumBuk=" + Util::Int2String(rb1.numBucket)
		+ "\tnumSub=" + Util::Int2String(subs)
		+ "\tsubSize=" + Util::Int2String(cons)
		+ "\tnumPub=" + Util::Int2String(pubs)
		+ "\tpubSize=" + Util::Int2String(m)
		+ "\tattTypes=" + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "ReinBits1BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + "\tnumBucket=" + Util::Int2String(rb.numBucket)
	//	//+ "\tsumBukSetSize=" + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ "\tmaxBukSetSize=" + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ "\tminBukSetSize=" + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + "\tBucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}


// 静动模式
void run_ReinBits2(const intervalGenerator& gen) {
	ReinBits2 rb2(valDom, subs, atts, buks, bits);

	vector<double> insertTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer subStart;

		rb2.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = subStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "ReinBits2 Insertion Finish.\n";

	double initTime;
	Timer initStart;
	rb2.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb2.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % 100 == 0)
			cout << "ReinBits2 Event" << i << " is matched.\n";
	}

	rb2.printRelation(0);
	// output
	string outputFileName = "ReinBits2.txt";
	string content = expID
		+ "\tbits=" + Util::Int2String(bits)
		+ "\tmemory=" + Util::Int2String(rb2.calMemory())
		+ "MB\tAvgMatchNum=" + Util::Double2String(Util::Average(matchSubList))
		+ "\tAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList))
		+ "ms\tInitTime=" + Util::Double2String(initTime)
		+ "ms\tNewAvgInsertTime=" + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
		+ "ms\tAvgMatchTime=" + Util::Double2String(Util::Average(matchTimeList))
		+ "ms\tAvgCmpTime=" + to_string(rb2.compareTime / pubs / 1000000)
		+ "ms\tAvgMarkTime=" + to_string(rb2.markTime / pubs / 1000000)
		+ "ms\tOrTime=" + to_string(rb2.orTime / pubs / 1000000)
		+ "ms\tAvgBitTime=" + to_string(rb2.bitTime / pubs / 1000000)
		+ "ms\tnumBuk=" + Util::Int2String(rb2.numBucket)
		+ "\tnumSub=" + Util::Int2String(subs)
		+ "\tsubSize=" + Util::Int2String(cons)
		+ "\tnumPub=" + Util::Int2String(pubs)
		+ "\tpubSize=" + Util::Int2String(m)
		+ "\tattTypes=" + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "ReinBits2BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + "\tnumBucket=" + Util::Int2String(rb.numBucket)
	//	//+ "\tsumBukSetSize=" + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ "\tmaxBukSetSize=" + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ "\tminBukSetSize=" + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + "\tBucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_ReinBits3(const intervalGenerator& gen) {

}

void run_ReinBits4(const intervalGenerator& gen) {

}

void run_ReinBits5(const intervalGenerator& gen) {

}
