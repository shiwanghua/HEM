#include "generator.h"
#include <cmath>

using namespace std;

void generator::GenSubList() {
	for (int i = 0; i < subs; i++) {
		Sub sub = GenOneSub(i, cons, atts, attDis, valDis, valDom, alpha);
		subList.push_back(sub);
	}
}

void intervalGenerator::GenSubList() {
	for (int i = 0; i < subs; i++) {
		IntervalSub sub = GenOneSub(i, cons, atts, attDis, valDis, valDom, alpha, width);
		subList.push_back(sub);
	}
}

// 谓词值非均匀分布
void intervalGenerator::GenSubList2() {
	int i = 0;
	double UniformProportion = 0.01;
	for (i = 0; i < UniformProportion * subs; i++) {
		IntervalSub sub = GenOneSub(i, cons, atts, attDis, valDis, valDom, alpha, width);
		subList.push_back(sub);
	}

	double most = UniformProportion + (1 - UniformProportion) * 0.8;
	while (i < most * subs) {
		IntervalSub sub;
		sub.id = i;
		sub.size = cons;

		if (attDis == 0)
			GenUniformAtts(sub, atts);
		else if (attDis == 1)
			GenZipfAtts(sub, atts, alpha);

		Gen28SubsPredicate(sub, 0.0, 0.2);
		subList.push_back(sub);
		i++;
	}
	while (i < subs) {
		IntervalSub sub;
		sub.id = i;
		sub.size = cons;

		if (attDis == 0)
			GenUniformAtts(sub, atts);
		else if (attDis == 1)
			GenZipfAtts(sub, atts, alpha);

		Gen28SubsPredicate(sub, 0.8, 1.0);
		subList.push_back(sub);
		i++;
	}
}

void generator::GenPubList() {
	for (int i = 0; i < pubs; i++) {
		Pub pub = GenOnePub(m, atts, attDis, valDis, valDom, alpha);
		pubList.push_back(pub);
	}
}

void intervalGenerator::GenPubList() {
	for (int i = 0; i < pubs; i++) {
		Pub pub = GenOnePub(i, m, atts, attDis, valDis, valDom, alpha);
		pubList.push_back(pub);
	}
}

void intervalGenerator::GenPubList2() {
	int id = 0;
	while (id < subp * pubs) {
		Pub pub;
		pub.id = id++;
//		cout<<"pubid= "<<id<<"\n";
		pub.size = m;
		int i = 0;
		// 让事件在前cons个维度上都有值, 维度较高时, 不至于没有订阅匹配
		while (i < min(pub.size, cons)) {
			Pair tmp;
			tmp.att = i;
			tmp.value = random(valDom);
			pub.pairs.push_back(tmp);
			i++;
		}
		vector<int> a;
		for (; i < pub.size; i++) {
			int x = random(atts - cons) + cons;
			while (CheckExist(a, x))
				x = random(atts - cons) + cons;
			a.push_back(x);
			Pair tmp;
			tmp.att = x;
			tmp.value = random(valDom);
			pub.pairs.push_back(tmp);
		}
		pubList.push_back(pub);
	}
//	cout<<"id= "<<id<<"\n";
	//	freopen("EventsDataset.txt", "r", stdin);
	ifstream infile;
	infile.open("EventsDataset.txt", ios::in);
	if (!infile.is_open()) {
		cout << "读取文件失败" << endl;
		return;
	}
	double v;
	int distance = 4000 / (pubs - subp * pubs + 1);
	for (int i = 0; i < 4000; i += distance) {
//		cout<<"i= "<<i<<"\n";
		if (i % distance == 0) {
			Pub pub;
			if (id == pubs)
				break;
			pub.id = id++;
//			cout<<id<<" ";
			pub.size = m;
			vector<int> a;
			int x;
			for (int j = 0; j < pub.size; j++) {
				// Second way High Dimension such as atts=900
//				x = random(atts);
//				while (CheckExist(a, x))
//					x = random(atts);
//				a.push_back(x);
				// Third way
//				x = zipfDistribution(atts, alpha);
//				while (CheckExist(a, x))
//					x = zipfDistribution(atts, alpha);
//				a.push_back(x);
				Pair tmp;
//				tmp.att = x;  // Second way Third way
				tmp.att = j;  // First way
				infile >> v;
				tmp.value = (int) v;
//				cout<<pub.id<<" "<<tmp.value<<" ";
				pub.pairs.push_back(tmp);
			}
//			cout<<"\n";
			pubList.push_back(pub);
			for (int j = 0; j < 50 - pub.size; j++)
				infile >> v; // 多余数据
		} else {
			for (int j = 0; j < 50; j++)
				infile >> v; // 多余数据
		}
	}
	infile.close();
}

Sub generator::GenOneSub(int id, int size, int atts, int attDis, int valDis, int valDom, double alpha) {
	Sub sub;
	sub.id = id;
	sub.size = size;
	if (attDis == 0)
		GenUniformAtts(sub, atts);
	else if (attDis == 1)
		GenZipfAtts(sub, atts, alpha);
//	if (valDis == 0)
	GenUniformValues(sub, valDom);
	return sub;
}

IntervalSub intervalGenerator::GenOneSub(int id, int size, int atts, int attDis, int valDis, int valDom, double alpha,
										 double width) {
	IntervalSub sub;
	sub.id = id;
	sub.size = size;
//	sub.size = 1 + random(size);
	if (attDis == 0)
		GenUniformAtts(sub, atts);
	else //if (attDis == 1)
		GenZipfAtts(sub, atts, alpha);

	if (valDis == 0) // Uniform + fixed width
		GenUniformValues(sub);
	else if (valDis == 1) // Random width + Uniform >= width   还有一种两端都服从均匀分布的情况，等价于width=0
		GenUniformValues_w(sub);
	else if (valDis == 2) // Random width + Zipf >= width
		GenZipfValues(sub);
	else if (valDis == 3) // Normal Distribution + fixed width
		GenNormalValues(sub);
	else if (valDis == 4)   // Normal Distribution in two value ends
		GenNormalValues_t(sub);
	else cout << "Wrong valDis for subscriptions.\n";
	return sub;
}

Pub generator::GenOnePub(int m, int atts, int attDis, int valDis, int valDom, double alpha) {
	Pub pub;
	pub.size = m;
	if (attDis == 0)
		GenUniformAtts(pub, atts);
	else //if (attDis == 1)
		GenZipfAtts(pub, atts, alpha);
//	if (valDis == 0)
	GenUniformValues(pub, valDom);
	return pub;
}

Pub intervalGenerator::GenOnePub(int id, int m, int atts, int attDis, int valDis, int valDom, double alpha) {
	Pub pub;
	pub.id = id;
	pub.size = m;
//	pub.size = 1 + random(m);

	if (attDis == 0) // Uniform
		GenUniformAtts(pub, atts);
	else if (attDis == 1)
		GenZipfAtts(pub, atts, alpha);

	if (valDis == 0 || valDis == 1)
		GenUniformValues(pub);
	else if (valDis == 2) // Zipf
		GenZipfValues(pub);
	else if (valDis == 3 || valDis == 4) // Normal
		GenNormalValues(pub);
	else cout << "Wrong valDis for events.\n";
	return pub;
}

void generator::GenUniformAtts(Sub &sub, int atts) {
	vector<int> a;
	for (int i = 0; i < sub.size; i++) {
		int x = random(atts);
		while (CheckExist(a, x))
			x = random(atts);
		a.push_back(x);
		Cnt tmp;
		tmp.att = x;
		sub.constraints.push_back(tmp);
	}
}

void intervalGenerator::GenUniformAtts(IntervalSub &sub, int atts) {
	if (sub.id < subp * subs) {
		for (int i = 0; i < sub.size; i++) {
			IntervalCnt tmp;
			tmp.att = i;
			sub.constraints.push_back(tmp);
		}
	} else {
		vector<int> a;
		for (int i = 0; i < sub.size; i++) {
			int x = random(atts);
			while (CheckExist(a, x))
				x = random(atts);
			a.push_back(x);
			IntervalCnt tmp;
			tmp.att = x;
			sub.constraints.push_back(tmp);
		}
	}
}

void generator::GenUniformAtts(Pub &pub, int atts) {
	vector<int> a;
	for (int i = 0; i < pub.size; i++) {
		int x = random(atts);
		while (CheckExist(a, x))
			x = random(atts);
		a.push_back(x);
		Pair tmp;
		tmp.att = x;
		pub.pairs.push_back(tmp);
	}
}

void intervalGenerator::GenUniformAtts(Pub &pub, int atts) {
	if (pub.id < subp * pubs) {
		vector<int> a;
		int i = 0;
		// 让事件在前cons个维度上都有值, 维度较高时, 不至于没有订阅匹配
		while (i < min(pub.size, cons)) {
			Pair tmp;
			tmp.att = i;
			pub.pairs.push_back(tmp);
			i++;
		}
		for (; i < pub.size; i++) {
			int x = random(atts - cons) + cons;
			while (CheckExist(a, x))
				x = random(atts - cons) + cons;
			a.push_back(x);
			Pair tmp;
			tmp.att = x;
			pub.pairs.push_back(tmp);
		}
	} else {
		vector<int> a;
		for (int i = 0; i < pub.size; i++) {
			int x = random(atts);
			while (CheckExist(a, x))
				x = random(atts);
			a.push_back(x);
			Pair tmp;
			tmp.att = x;
			pub.pairs.push_back(tmp);
		}
	}
}

void generator::GenZipfAtts(Sub &sub, int atts, double alpha) {
	vector<int> a;
	for (int i = 0; i < sub.size; i++) {
		int x = zipfDistribution(atts, alpha);
		while (CheckExist(a, x))
			x = zipfDistribution(atts, alpha);
		a.push_back(x);
		Cnt tmp;
		tmp.att = x;
		sub.constraints.push_back(tmp);
	}
}

void intervalGenerator::GenZipfAtts(IntervalSub &sub, int atts, double alpha) {
	vector<int> a;
	for (int i = 0; i < sub.size; i++) {
		int x = zipfDistribution(atts, alpha);
		while (CheckExist(a, x))
			x = zipfDistribution(atts, alpha);
		a.push_back(x);
		IntervalCnt tmp;
		tmp.att = x;
		sub.constraints.push_back(tmp);
	}
}

void generator::GenZipfAtts(Pub &pub, int atts, double alpha) {
	vector<int> a;
	for (int i = 0; i < pub.size; i++) {
		int x = zipfDistribution(atts, alpha);
		while (CheckExist(a, x))
			x = zipfDistribution(atts, alpha);
		a.push_back(x);
		Pair tmp;
		tmp.att = x;
		pub.pairs.push_back(tmp);
	}
}

void intervalGenerator::GenZipfAtts(Pub &pub, int atts, double alpha) {
	vector<int> a;
	for (int i = 0; i < pub.size; i++) {
		int x = zipfDistribution(atts, alpha);
		while (CheckExist(a, x))
			x = zipfDistribution(atts, alpha);
		a.push_back(x);
		Pair tmp;
		tmp.att = x;
		pub.pairs.push_back(tmp);
	}
}

void generator::GenUniformValues(Sub &sub, int valDom) {
	for (int i = 0; i < sub.size; i++) {
		int x = random(valDom);
		sub.constraints[i].value = x;
		int y = random(99999);
		sub.constraints[i].op = y % 3;
	}
}

void intervalGenerator::GenUniformValues(IntervalSub &sub) {
	int fixedWidth = valDom * width;
	for (int i = 0; i < sub.size; i++) {
		int x = random(int(valDom * (1.0 - width)));
		int y = x + fixedWidth;
		sub.constraints[i].lowValue = x;
		sub.constraints[i].highValue = y;
	}
}

// Random width + Uniform, >=width
void intervalGenerator::GenUniformValues_w(IntervalSub &sub) {
	int w, x, y;
	int minRange = valDom * width;
	int maxVal = valDom - minRange;
	for (int i = 0; i < sub.size; i++) {
		w = random(maxVal) + minRange; // random width
		x = random(valDom - w);
		y = x + w;
		sub.constraints[i].lowValue = x;
		sub.constraints[i].highValue = y;
	}
}

// Random width + Zipf, >=width
void intervalGenerator::GenZipfValues(IntervalSub &sub) {
	int w, x, y;
	int minRange = valDom * width;
	int maxVal = valDom - minRange;
	_for(i, 0, sub.size) {
		w = random(maxVal) + minRange;
		x = zipfDistribution(valDom - w, alpha);
		y = x + w;
		sub.constraints[i].lowValue = x;
		sub.constraints[i].highValue = y;
		//cout << sub.id << ": " << sub.constraints[i].lowValue << " " << sub.constraints[i].highValue << endl;
	}
}

// Predicate value in Normal Distribution fixed width
void intervalGenerator::GenNormalValues(IntervalSub &sub) {
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine generator(seed);
	static std::normal_distribution<double> distribution(mean, stddev);
	double maxValue = 1 - width;
	double x, y;
	int range = width * valDom;
	_for(i, 0, sub.size) {
		x = distribution(generator);
		while (x >= maxValue || x < 0) x = distribution(generator);
		x = x * valDom;
		y = x + range;
		sub.constraints[i].lowValue = x;
		sub.constraints[i].highValue = y;
		//cout << sub.id << ": " << sub.constraints[i].lowValue << " " << sub.constraints[i].highValue << endl;
	}
}

// Predicate value in NormalDsitribution two ends
void intervalGenerator::GenNormalValues_t(IntervalSub &sub) {
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine generator(seed);
	static std::normal_distribution<double> distribution(mean, stddev);
	_for(i, 0, sub.size) {
		double x = distribution(generator);
		while (x >= 1 || x < 0) x = distribution(generator);
		double y = distribution(generator);
		while (y >= 1 || y < x) y = distribution(generator);
		sub.constraints[i].lowValue = x * valDom;
		sub.constraints[i].highValue = y * valDom;
		//cout << sub.id << ": " << sub.constraints[i].lowValue << " " << sub.constraints[i].highValue << endl;
	}
}

// 根据28原则生成订阅
void intervalGenerator::Gen28SubsPredicate(IntervalSub &sub, double l, double h) {
	int minV = valDom * l;
	int maxV = valDom * h;
	int range = maxV - minV;
	int w, x, y;
	for (int i = 0; i < sub.size; i++) {
		w = random(range);
		x = random(range - w) + minV;
		y = x + w;
		sub.constraints[i].lowValue = x;
		sub.constraints[i].highValue = y;
	}
}

void generator::GenUniformValues(Pub &pub, int valDom) {
	for (int i = 0; i < pub.size; i++) {
		int x = random(valDom);
		pub.pairs[i].value = x;
	}
}

void intervalGenerator::GenUniformValues(Pub &pub) {
	for (int i = 0; i < pub.size; i++) {
		int x = random(valDom);
		pub.pairs[i].value = x;
	}
}

void intervalGenerator::GenZipfValues(Pub &pub) {
	for (int i = 0; i < pub.size; i++) {
		pub.pairs[i].value = zipfDistribution(valDom, alpha);
	}
}

void intervalGenerator::GenNormalValues(Pub &pub) {
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine generator(seed);
	static std::normal_distribution<double> distribution(1 - mean, stddev);
	for (int i = 0; i < pub.size; i++) {
		double x = distribution(generator);
		while (x < 0 || x >= 1) x = distribution(generator);
		pub.pairs[i].value = x * valDom;
	}
}

bool generator::CheckExist(vector<int> a, int x) {
	for (int i = 0; i < a.size(); i++)
		if (a[i] == x)
			return true;
	return false;
}

bool intervalGenerator::CheckExist(vector<int> a, int x) {
	for (int i = 0; i < a.size(); i++)
		if (a[i] == x)
			return true;
	return false;
}

int generator::zipfDistribution(int n, double alpha) {
	// alpha > 0
	static bool first = true;
	static double c = 0;          // Normalization constant
	double z;                     // Uniform random number (0 < z < 1)
	double sum_prob;              // Sum of probabilities
	double zipf_value = 0;            // Computed exponential value to be returned
	int i;                     // Loop counter

	if (first) {
		for (i = 1; i <= n; i++)
			c = c + (1.0 / pow((double) i, alpha));
		c = 1.0 / c;
		first = false;
	}

	while (true) {
		// Pull a uniform random number (0 < z < 1)
		do {
			z = (double) rand() / RAND_MAX;
		} while ((z == 0) || (z == 1));

		// Map z to the value
		sum_prob = 0;
		for (i = 1; i <= n; i++) {
			sum_prob = sum_prob + c / pow((double) i, alpha);
			if (sum_prob >= z) {
				zipf_value = i;
				break;
			}
		}

		if (zipf_value >= 1 && zipf_value <= n)
			return int(zipf_value - 1);
	}
}


int intervalGenerator::zipfDistribution(int n, double alpha) {
	// alpha > 0
	static bool first = true;
	static double c = 0;          // Normalization constant
	double z;                     // Uniform random number (0 < z < 1)
	double sum_prob;              // Sum of probabilities
	double zipf_value = 0;            // Computed exponential value to be returned
	int i;                     // Loop counter

	if (first) {
		for (i = 1; i <= n; i++)
			c = c + (1.0 / pow((double) i, alpha));
		c = 1.0 / c;
		first = false;
	}

	while (true) {
		// Pull a uniform random number (0 < z < 1)
		do {
			z = (double) rand() / RAND_MAX;
		} while ((z == 0) || (z == 1));

		// Map z to the value
		sum_prob = 0;
		for (i = 1; i <= n; i++) {
			sum_prob = sum_prob + c / pow((double) i, alpha);
			if (sum_prob >= z) {
				zipf_value = i;
				break;
			}
		}

		if (zipf_value >= 1 && zipf_value <= n)
			return int(zipf_value - 1);
	}
}

int generator::random(int x) {
	return (int) (x * (rand() / (RAND_MAX + 1.0)));
}

int intervalGenerator::random(int x) {
	return (int) (x * (rand() / (RAND_MAX + 1.0)));
}