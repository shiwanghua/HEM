#include "run_models.h"

void run_rein(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
	Rein rein;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rein.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "Rein Insertion Finishes.\n";

	// 验证插入删除正确性
	if (verifyID) {
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!rein.deleteSubscription(gen.subList[kv.first]))
				cout << "Rein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"Rein Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			rein.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		cout<<"REIN "<<i<<"\n";
		Timer matchStart;

		rein.match(gen.pubList[i], matchSubs);

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
					 + " memory= " + Util::Int2String(rein.calMemory())
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
					 + " stddev= " + Util::Double2String(stddev);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "Rein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
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

void run_BIOP(const intervalGenerator &gen) {
	BIOP rb;

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
	cout << "BIOP Insertion Finishes.\n";

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
			cout << "BIOP Event " << i << " is matched.\n";
	}
	cout << endl;

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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP0PS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
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
void run_BIOP1(const intervalGenerator &gen) {
	BIOP1 rb1;

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
	cout << "BIOP1 Insertion Finishes.\n";

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
			cout << "BIOP1 Event " << i << " is matched.\n";
	}

	if (display)
		rb1.printRelation();
	cout << endl;

	// output
	string outputFileName = "BIOP1.txt";
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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP1SS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
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
void run_BIOP2(const intervalGenerator &gen) {
	BIOP2 rb2;

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
	cout << "BIOP2 Insertion Finishes.\n";

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
			cout << "BIOP2 Event " << i << " is matched.\n";
	}

	if (display)
		rb2.printRelation(0);
	cout << endl;

	// output
	string outputFileName = "BIOP2.txt";
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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP2SD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
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

void run_BIOP3(const intervalGenerator &gen) {
	BIOP3 rb3;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb3.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BIOP3 Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb3.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb3.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP3 Event " << i << " is matched.\n";
	}

	if (display)
		rb3.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "BIOP3.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(rb3.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
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

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP3PD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
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

void run_BIOP4(const intervalGenerator &gen) {
	BIOP4 rb4;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb4.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BIOP4DS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb4.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb4.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP4DS Event " << i << " is matched.\n";
	}

	if (display)
		rb4.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "BIOP4.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(rb4.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rb4.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rb4.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(rb4.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rb4.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rb4.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP4DS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOP4BucketSize.txt";
	//rb4.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb4.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rb4.bucketSub.begin(), rb4.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb4.bucketSub.begin(), rb4.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb4.bucketSub.begin(), rb4.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb4.numBucket)
	//	content += " " + to_string(rb3.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOP5(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
	BIOP5 rb5;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		rb5.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BIOP5DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	rb5.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// 验证插入删除正确性
	if (verifyID) {
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!rb5.deleteSubscription(gen.subList[kv.first]))
				cout << "BIOP5DD: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"BIOP5DD Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			rb5.insert_online(gen.subList[kv.first]); // Bug: should use insert_online other than insert function!
		}
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		rb5.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP5DD Event " << i << " is matched.\n";
	}

	if (display)
		rb5.printRelation(1);
	cout << endl;

	// output
	string outputFileName = "BIOP5.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(rb5.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgDeleteTime= " + Util::Double2String(Util::Average(deleteTimeList))
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(rb5.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(rb5.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(rb5.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(rb5.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(rb5.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOP5DD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOP5BucketSize.txt";
	//rb5.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(rb5.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(rb5.bucketSub.begin(), rb5.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(rb5.bucketSub.begin(), rb5.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(rb5.bucketSub.begin(), rb5.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, rb5.numBucket)
	//	content += " " + to_string(rb5.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOPSC(const intervalGenerator &gen) {
	BIOPSC biopsc;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		biopsc.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BIOP-SC-DD Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	biopsc.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		//cout << "Begin to match event " << i << endl;
		biopsc.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP-SC-DD Event " << i << " is matched.\n";
	}

	if (display)
		biopsc.printRelation(1, 2);
	cout << endl;

	// output
	string outputFileName = "BIOPSC.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(biopsc.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(biopsc.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(biopsc.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(biopsc.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(biopsc.bitTime / pubs / 1000000)
					 + " ms numBuk= " + Util::Int2String(biopsc.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts)
					 + " lvls=" + Util::Int2String(lvls);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOPSCDD= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);

	//outputFileName = "BIOPSCBucketSize.txt";
	//biopsc.calBucketSize();
	//content = expID + " numBucket= " + Util::Int2String(biopsc.numBucket)
	//	//+ " sumBukSetSize= " + to_string(accumulate(biopsc.bucketSub.begin(), biopsc.bucketSub.end(), 0, [=](int acc, const auto& u) {return acc + u.size(); }))
	//	+ " maxBukSetSize= " + to_string((*max_element(biopsc.bucketSub.begin(), biopsc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })).size())
	//	+ " minBukSetSize= " + to_string(min_element(biopsc.bucketSub.begin(), biopsc.bucketSub.end(), [](const unordered_set<int>& u, const unordered_set<int>& v) {return u.size() < v.size(); })->size()) + " BucketSize:";
	//_for(i, 0, biopsc.numBucket)
	//	content += " " + to_string(biopsc.bucketSub[i].size());
	//Util::WriteData(outputFileName.c_str(), content);
}

void run_BIOPSR(const intervalGenerator &gen) {
	BIOPSR biopsr;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		biopsr.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "BIOP-SR-PS Insertion Finishes.\n";

	double initTime;
	Timer initStart;
	biopsr.initBits();
	initTime = (double) initStart.elapsed_nano() / 1000000.0;

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;
		//cout << "Begin to match event " << i << endl;
		biopsr.match(gen.pubList[i], matchSubs);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "BIOP-SR-PS Event " << i << " is matched.\n";
	}

	if (display)
		biopsr.printRelation();
	cout << endl;

	// output
	string outputFileName = "BIOPSR.txt";
	string content = expID
					 + " bits= " + Util::Int2String(be)
					 + " memory= " + Util::Int2String(biopsr.calMemory())
					 + " MB AvgMatchNum= " + Util::Double2String(Util::Average(matchSubList))
					 + " AvgInsertTime= " + Util::Double2String(Util::Average(insertTimeList))
					 + " ms InitTime= " + Util::Double2String(initTime)
					 + " ms AvgConstructionTime= " +
					 Util::Double2String(Util::Average(insertTimeList) + initTime / subs)
					 + " ms AvgMatchTime= " + Util::Double2String(Util::Average(matchTimeList))
					 + " ms AvgCmpTime= " + to_string(biopsr.compareTime / pubs / 1000000)
					 + " ms AvgMarkTime= " + to_string(biopsr.markTime / pubs / 1000000)
					 + " ms OrTime= " + to_string(biopsr.orTime / pubs / 1000000)
					 + " ms AvgBitTime= " + to_string(biopsr.bitTime / pubs / 1000000)
					 + " ms GroupSize=" + Util::Int2String(gs)
					 + " numBuk= " + Util::Int2String(biopsr.numBucket)
					 + " numSub= " + Util::Int2String(subs)
					 + " subSize= " + Util::Int2String(cons)
					 + " numPub= " + Util::Int2String(pubs)
					 + " pubSize= " + Util::Int2String(m)
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "BIOPSRPS= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}

void run_Simple(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
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
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!simple.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"Simple Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			simple.insert(gen.subList[kv.first]);
		}
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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "Simple= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}

void run_Simple2(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
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
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!simple2.deleteSubscription(gen.subList[kv.first]))
				cout << "Simple2: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"Simple2 Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			simple2.insert(gen.subList[kv.first]);
		}
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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "Simple2= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}

void run_tama(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
	printf("123\n");
	fflush(stdout);
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
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!tama.deleteSubscription(gen.subList[kv.first]))
				cout << "Tama: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"Tama Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			tama.insert(gen.subList[kv.first]);
		}
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
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "Tama= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}

void run_adarein(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
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
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!adarein.deleteSubscription(gen.subList[kv.first]))
				cout << "AdaRein: sub" << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"AdaRein Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			adarein.insert(gen.subList[kv.first]);
		}
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
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "AdaRein= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}

void run_opindex(const intervalGenerator &gen,unordered_map<int,bool> deleteNo) {
	OpIndex2 opindex;

	vector<double> insertTimeList;
	vector<double> deleteTimeList;
	vector<double> matchTimeList;
	vector<double> matchSubList;

	// Note: for OpIndex, it needs to calculate frequency first!
	double initTime;
	Timer initStart;
	opindex.calcFrequency(gen.subList);
	initTime = (double) initStart.elapsed_nano() / 1000000.0;
	cout << "OpIndex CalcFrequency Task Finishes.\n";

	// insert
	for (int i = 0; i < subs; i++) {
		Timer insertStart;

		opindex.insert(gen.subList[i]); // Insert sub[i] into data structure.

		int64_t insertTime = insertStart.elapsed_nano(); // Record inserting time in nanosecond.
		insertTimeList.push_back((double) insertTime / 1000000);
	}
	cout << "OpIndex Insertion Finishes.\n";

	if (!display) {// show pivot attribute
		int counter = 0;
		for (int i = 0; i < atts; i++) {
			cout << "Att " << i << ": " << opindex.isPivot[i] << ", ";
			if (opindex.isPivot[i])counter++;
			if (i > 0 && i % 5 == 0) cout << endl;
		}
		cout << "\nTotal pivot attribute: " << counter << endl;
	}

	// 验证插入删除正确性
	if (verifyID) {
		for(auto kv : deleteNo) {
			Timer deleteStart;
			if (!opindex.deleteSubscription(gen.subList[kv.first]))
				cout << "OpIndex: sub " << gen.subList[kv.first].id << " is failled to be deleted.\n";
			deleteTimeList.push_back((double) deleteStart.elapsed_nano() / 1000000);
		}
		cout<<"OpIndex Deletion Finishes.\n";
		for(auto kv : deleteNo) {
			opindex.insert(gen.subList[kv.first]);
		}
	}

	// match
	for (int i = 0; i < pubs; i++) {
		int matchSubs = 0; // Record the number of matched subscriptions.
		Timer matchStart;

		opindex.match(gen.pubList[i], matchSubs, gen.subList);

		int64_t eventTime = matchStart.elapsed_nano(); // Record matching time in nanosecond.
		matchTimeList.push_back((double) eventTime / 1000000);
		matchSubList.push_back(matchSubs);
		if (i % interval == 0)
			cout << "OpIndex Event " << i << " is matched.\n";
	}
	cout << endl;

	// output
	string outputFileName = "OpIndex.txt";
	string content = expID
					 + " memory= " + Util::Int2String(opindex.calMemory())
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
					 + " attTypes= " + Util::Int2String(atts);
	Util::WriteData(outputFileName.c_str(), content);

	outputFileName = "ComprehensiveExpTime.txt";
	content = "OpIndex= [";
	_for(i, 0, pubs) content += Util::Double2String(matchTimeList[i]) + ", ";
	content[content.length() - 2] = ']';
	Util::WriteData(outputFileName.c_str(), content);
}