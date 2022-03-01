#ifndef _GENERATOR_H
#define _GENERATOR_H
#include "data_structure.h"
#include <chrono>
#include <random>
#include<fstream>
#include "constant.h"

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

class generator {
    void GenUniformAtts(Sub &sub, int atts);

    void GenUniformAtts(Pub &pub, int atts);

    void GenZipfAtts(Sub &sub, int atts, double alpha);

    void GenZipfAtts(Pub &pub, int atts, double alpha);

    void GenUniformValues(Sub &sub, int valDom);

    void GenUniformValues(Pub &pub, int valDom);

    bool CheckExist(vector<int> a, int x);

    int random(int x);

    int zipfDistribution(int n, double alpha);

    Sub GenOneSub(int id, int size, int atts, int attDis, int valDis, int valDom, double alpha);

    Pub GenOnePub(int m, int atts, int attDis, int valDis, int valDom, double alpha);

public:
    vector<Sub> subList;
    vector<Pub> pubList;
    int subs, pubs, atts, cons, m, attDis, valDis, valDom;
    double alpha;

    generator(int subs, int pubs, int atts, int cons, int m, int attDis, int valDis, int valDom, double alpha) :
            subs(subs), pubs(pubs), atts(atts), cons(cons), m(m), attDis(attDis), valDis(valDis), valDom(valDom),
            alpha(alpha)
    {}

    void GenSubList();

    void GenPubList();
};


class intervalGenerator {
	void GenUniformAtts(IntervalSub &sub, int atts);
	void GenUniformAtts_g(IntervalSub &sub);  // Attribute Group Version

	void GenUniformAtts(Pub &pub, int atts);
	void GenUniformAtts_g(Pub &pub); // Attribute Group Version

	void GenZipfAtts(IntervalSub &sub, int atts, double alpha);
	void GenZipfAtts_g(IntervalSub &sub);

	void GenZipfAtts(Pub &pub, int atts, double alpha);
	void GenZipfAtts_g(Pub &pub);

	void GenUniformValues(IntervalSub &sub); // fixed width
	void GenUniformValues_w(IntervalSub &sub);  // random width >=$width
	void Gen28SubsPredicate(IntervalSub &sub, double l, double h);

	void GenUniformValues(Pub &pub);

	void GenZipfValues(IntervalSub &sub); // random width
	void GenZipfValues(Pub &pub);

	void GenNormalValues(IntervalSub &sub); // fixed width
	void GenNormalValues_t(IntervalSub &sub); // Normal + two ends
	void GenNormalValues(Pub &pub);

	bool CheckExist(vector<int> a, int x);

	int random(int x);

	int zipfDistribution(int n, double alpha);

	IntervalSub GenOneSub(int id, int size, int atts, int attDis, int valDis, int valDom, double alpha, double width);
	IntervalSub GenOneSub_g(int id);

	Pub GenOnePub(int id, int m, int atts, int attDis, int valDis, int valDom, double alpha);
	Pub GenOnePub_g(int id);

public:
	vector<IntervalSub> subList;
	vector<Pub> pubList;
	int subs, pubs, atts, cons, m, attNumType, attDis, valDis, valDom, attrGroupNum;
	int groupSize; // groupSize = atts/attrGroupNum
	double alpha, width, subp;
	// 0 -- fixed width, 1 -- maxWidth, -1 -- minWidth
	// 0 ~ [width,width], 1 ~ [0,width], -1 ~ [width,1]
	int widthType;

	intervalGenerator(int subs, int pubs, int atts, int attGrpNum, int cons, int m,int attNumType, int attDis, int valDis, int valDom, double alpha,
					  double p, double width = 0.5) :
		subs(subs), pubs(pubs), atts(atts), attrGroupNum(attGrpNum), cons(cons), m(m), attNumType(attNumType), attDis(attDis), valDis(valDis), valDom(valDom),
		alpha(alpha), subp(p), width(width) {
		groupSize = atts / attrGroupNum; // 假设是整数倍
		srand(time(NULL));  // 不加的话订阅、事件、匹配结果每次都一样
	}

	void GenSubList();

	void GenSubList2(); // 产生取值不均匀的订阅

	void GenSubList_g(); // 按属性组生成订阅


	void GenPubList();

	void GenPubList2(); // Read from xlsx file

	void GenPubList_g(); // 按属性组生成事件

};
#endif
