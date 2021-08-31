#include "run_models.h"

void run_rein(const intervalGenerator& gen) {
	Rein rein;

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
		if (i % interval ==0)
			cout << "Rein Event" << i << " is matched.\n";
	}

	// output
	string outputFileName = "Rein.txt";
	string content = expID
		+ " memory= " + Util::Int2String(rein.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
		+ " ms AvgCmpTime= " + to_string(rein.compareTime / pubs / 1000000)
		+ " ms AvgMarkTime= " + to_string(rein.markTime / pubs / 1000000)
		+ " ms AvgBitTime= " + to_string(rein.bitTime / pubs / 1000000)
		+ " ms numBuk= " + Util::Int2String(rein.numBucket)
		+ " numSub= " + Util::Int2String(subs)
		+ " subSize= " + Util::Int2String(cons)
		+ " numPub= " + Util::Int2String(pubs)
		+ " pubSize= " + Util::Int2String(m)
		+ " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "ReinBucketSize.txt";
	//rein.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rein.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rein.bucketSub.begin(), rein.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rein.numBucket)
	//	content += " " + to_string(rein.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOP(const intervalGenerator& gen) {
	BIOP rb;

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
	cout << "BIOP Insertion Finish.\n";

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
		if (i % interval == 0)
			cout << "BIOP Event" << i << " is matched.\n";
	}

	if (display)
		rb.printRelation();
	// output
	string outputFileName = "BIOP.txt";
	string content = expID
		+ " bits= " + Util::Int2String(be)
		+ " memory= " + Util::Int2String(rb.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms InitTime= " + Util::Double2String(initTime)
		+ " ms NewAvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
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
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOPBucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

// 静静模式
void run_BIOP1(const intervalGenerator& gen) {
	BIOP1 rb1;

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
	cout << "BIOP1 Insertion Finishes.\n";

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
		if (i % interval == 0)
			cout << "BIOP1 Event" << i << " is matched.\n";
	}
	if(display)
		rb1.printRelation();
	// output
	string outputFileName = "BIOP1.txt";
	string content = expID
		+ " bits= " + Util::Int2String(be)
		+ " memory= " + Util::Int2String(rb1.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms InitTime= " + Util::Double2String(initTime)
		+ " ms NewAvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
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
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOP1BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}


// 静动模式
void run_BIOP2(const intervalGenerator& gen) {
	BIOP2 rb2;

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
	cout << "BIOP2 Insertion Finish.\n";

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
		if (i % interval == 0)
			cout << "BIOP2 Event" << i << " is matched.\n";
	}

	if (display)
	    rb2.printRelation(0);
	// output
	string outputFileName = "BIOP2.txt";
	string content = expID
		+ " bits= " + Util::Int2String(be)
		+ " memory= " + Util::Int2String(rb2.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms InitTime= " + Util::Double2String(initTime)
		+ " ms NewAvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
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
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOP2BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rein.bucketSub.begin(), rein.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb.bucketSub.begin(), rb.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb.numBucket)
	//	content += " " + to_string(rb.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOP3(const intervalGenerator& gen) {
	BIOP3 rb3;

	vector<double> insertTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++)
	{
		Timer subStart;

		rb3.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = subStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double)insertTime / 1000000);
	}
	cout << "BIOP3 Insertion Finish.\n";

	double initTime;
	Timer initStart;
	rb3.initBits();
	initTime = (double)initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++)
	{
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb3.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double)eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP3 Event" << i << " is matched.\n";
	}

	if (display)
		rb3.printRelation(0);
	// output
	string outputFileName = "BIOP3.txt";
	string content = expID
		+ " bits= " + Util::Int2String(be)
		+ " memory= " + Util::Int2String(rb3.calMemory())
		+ " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
		+ " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
		+ " ms InitTime= " + Util::Double2String(initTime)
		+ " ms NewAvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
		+ " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
		+ " ms AvgCmpTime= " + to_string(rb3.compareTime / pubs / 1000000)
		+ " ms AvgMarkTime= " + to_string(rb3.markTime / pubs / 1000000)
		+ " ms OrTime= " + to_string(rb3.orTime / pubs / 1000000)
		+ " ms AvgBitTime= " + to_string(rb3.bitTime / pubs / 1000000)
		+ " ms numBuk= " + Util::Int2String(rb3.numBucket)
		+ " numSub= " + Util::Int2String(subs)
		+ " subSize= " + Util::Int2String(cons)
		+ " numPub= " + Util::Int2String(pubs)
		+ " pubSize= " + Util::Int2String(m)
		+ " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOP3BucketSize.txt";
	//rb.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb3.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rb3.bucketSub.begin(), rb3.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb3.bucketSub.begin(), rb3.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb3.bucketSub.begin(), rb3.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb3.numBucket)
	//	content += " " + to_string(rb3.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOP4(const intervalGenerator& gen) {

}

void run_BIOP5(const intervalGenerator& gen) {

}
