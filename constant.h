//#pragma once
#ifndef _CONST_H
#define _CONST_H
#include<iostream>
#include <immintrin.h> //AVX(include wmmintrin.h)

#define DEBUG

using namespace std;

extern string expID;       // 记录实验id
extern const bool display; // 是否打印映射关系
extern const bool verifyID;// whether to verify Insertion and Deletion function
extern const int interval; // 每匹配这么多个事件输出一条日志信息
extern const double subp;  // subp比例的订阅和事件的谓词定义在前cons个维度上
extern double realMatchNum;
const int subs = 1000000;  //1048576;//134217728;  // Number of subscriptions.
extern const int pubs;     // Number of publications.
extern int atts;           // Total number of attributes, i.e. dimensions.
extern int attrGroup;          // The number of attribute group in #atts attributes.
extern int cons;           // Number of constraints(predicates) in one sub.
extern int m;              // Number of constraints in one pub.
extern const int attNumType;  // 0: Fixed size of sub and pub. 1: Random size of sub and pub <= $cons and $m
extern const int attDis;   // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
extern const int valDis;   // The distribution of values in subs and pubs. 0:uniform + fixed width | 1: Uniform + random width >= $width | 2: Zipf + fixed width | 3: Normal Distribution + random width both value ends
extern int valDom;         // Cardinality of values.
extern double alpha;       // Parameter for Zipf distribution.
extern double width;       // Width of a predicate.
extern double mean;        // Mean of Normal Distribution. For events it's ( 1 - mean ).
extern double stddev;      // Standard deviation: The square root of variance, representing the dispersion of values from the distribution mean. >0

// Rein
extern const int buks;     // Number of buckets.
extern const int awRein_Ppoint; // HybridRein
extern const int awRein_level; // The levels of the forward counting matching of HybridRein
// Rein Model Type
enum {
	OriginalRein, // Rein
	ForwardRein, // FRein
	ForwardRein_CBOMP, // FRein_c
	HybridRein, // HRein, AWRein
	HybridRein_CBOMP,  // AWRein with C-BOMP
	SharedHybridRein,  // SHRein
	SharedHybridRein_CBOMP // SHRein_c
};

// AdaRein
//const int MAX_SUBS = 1000000;
//const int MAX_ATTS = 20; // Tama
//const int MAX_BUCKS = 1000;
extern const double falsePositiveRate;
extern const int adarein_level;
// extern const double divider;
enum {
	AdaRein_ORI,

	AdaRein_SSS,       // Static Succession Selection
	AdaRein_SSS_B,     // Static Succession Selection Backward
	AdaRein_SSS_C,     // Static Succession Selection Crossed
	AdaRein_SSS_C_PPH, // PeePHole
	AdaRein_SSS_C_W,   // Static Succession Selection Crossed WidthClassification
	pAdaRein_SSS_C_W,  // parallel Static Succession Selection Crossed WidthClassification

	AdaRein_SDS,       // Static Discretization Selection

	AdaRein_DSS,       // Dynamic Succession Selection
	AdaRein_DSS_W,     // Dynamic Succession Selection WidthClassification
	AdaRein_DSS_B,     // Dynamic Succession Selection Backward
	AdaRein_DSS_B_W,   // Dynamic Succession Selection Backward WidthClassification

	AdaRein_DDS_B,       // Dynamic Discretization Selection Backward
				       
	AdaRein_IBU        // Independent Bucket Unit (Static)
};

// pRein
extern int parallelDegree;

// HEM bRein
extern int be;             // 每个维度上 2 * 2^bits个bits数组
extern int be2;            // be=-1时每个维度上 2 * be2 个bits数组
extern const int lvls;     // HEMSC 层数
extern int gs;             // HEMSR 一组有多少个维度
extern const int blockSize;
extern const int ptrSize;
extern const int blockNum; // 或运算次数
extern const int ptrIncrement;
extern const __m256i mask;
enum {
	HEM5_DD,
	HEM5_DD_PARALLEL,
	HEM5_DD_AVXOR_PARALLEL,
	HEM5_DD_VAS,
	HEM5_DD_RAS,
	HEM5_DD_RAS_AVXOR_PARALLEL
};

// Tama
extern const int level;

// OpIndex
const int SEGMENTS = 32;
const int MAX_SIGNATURE = 61;

// BGTree
extern const int initH;           // Initial height
extern const int MAXNodeSIZE;
extern const int boundary; // BGTree_d

// PS-Tree
extern const int PSTree_level;

//AWBTree
extern const float awbTree_Ppoint;//partition point HybridRein(AWRein)
extern const unsigned short WCsize; //width cell size
extern const int awbTree_branch;


#endif

