#include "constant.h"
string expID = "1530";       // Record the experiment ID number
const bool display = false;  // whether to print the mapping relation, 有待逐步改成DEBUG宏定义判断
const bool verifyID = false; // whether to verify Insertion and Deletion function
const int interval = 500;    // after a interval of events is matched, print a log
const double subp = 0;       // subp比例的订阅的谓词定义在前cons个维度上 in order to make the number of matching subscriptions > 0		  
double realMatchNum=-1;      // The correct matching number in an experiment (Recorded in Rein).
//const int subs = 1000000;  // Number of subscriptions.
const int pubs = 1000;       // Number of publications.
int atts = 40;             // Total number of attributes, i.e. dimensions.
int attrGroup = 1;           // The number of attribute group in #atts attributes.
int cons = 10;                // Number of constraints(predicates) in one sub.
int m = 40;                // Number of attributes in one pub.
const int attNumType = 0;    // 0: Fixed size of sub and pub. 1: Random size of sub and pub, <= $cons and $m.
const int attDis = 0;        // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution | 2: sub Zipf, pub 满维
const int valDis = 0;        // The distribution of values in subs and pubs. 0:uniform + fixed width | 1: random width >= $width + Uniform | 2: Zipf + random >=$width | 3: Normal + fixed width | 4: Normal + two ends
int valDom = 1000000;        // Cardinality of values.
double alpha = 0;            // Parameter for Zipf distribution.
double width = 0.4;            // Width of a predicate.
double mean = 0.2;           // Mean of Normal Distribution. For events it's ( 1 - mean ).
double stddev = 5;           // Standard deviation: The square root of variance, representing the dispersion of values from the distribution mean. >0

// Rein
const int buks = 1000;       // Number of buckets.
const int awRein_Ppoint = 0.25 * valDom; // HybridRein
//const int awRein_level=4; // The levels of the forward counting matching of HybridRein

// AdaRein
const double falsePositiveRate = 0.05;
const int adarein_level = 5;
//const double divider=0.2;

// pRein pHEM5DD pHEM5DD-avxOR pHEM5DD-RAS-avxOR TAMA-P bTama-F/B-P
int parallelDegree = 5;

// HEM
int be = 4;                 // bit exponent. In each dimension, there are 2 * 2^{be} bitsets.
int be2 = 50;                // When be=-1, be2 is adopted. In each dimension, there are 2 * be2 bitsets.
const int lvls = 2;         // HEMSC number of levels
int gs = 5;                 // HEMSR Group Size 一组有多少个维度
// const int blockSize = 512;  // 每次对256位做逻辑运算
const int ptrSize = 64;     // 每次只能加载__int64 64位
const int blockNum = (subs + blockSize - 1) / blockSize; //  把做 subs 位的逻辑运算 简化为做 blockNum 次逻辑运算
const int ptrIncrement = blockSize / ptrSize;       //  需要加载的次数
const __m256i mask = _mm256_set1_epi32(0x80000000);

// Tama
const int level = 13;

// BG-Tree
const int initH = 10;           // Initial height
const int MAXNodeSIZE = 2000;
const int boundary = subs * 0.01; // BoundaryNumSub BNS
// boundary>>MAXNodeSIZE

// PS-Tree
const int PSTree_level = 4;

//AWBTree
const float awbTree_Ppoint = 0.2;//partition point 0.2
const unsigned short WCsize = 40; //width cell size 40
const int awbTree_branch = 2000; // 2000

AWBTree_p = [8.568169, 4.386530, 3.482756, 2.972734, 2.836162, 2.889741, 3.179476, 3.110848, 3.292921, 3.487096, 3.676609, 3.806958]
pHEM5 = [7.074398, 3.699263, 2.795546, 2.302519, 2.175965, 2.024251, 2.030027, 2.063560, 2.266511, 2.653135, 2.038113, 2.049435]
bTama6_p = [9.680253, 5.107294, 3.582288, 2.876039, 2.417366, 2.227147, 2.073855, 2.021690, 2.084245, 1.908335, 1.983311, 1.866529]
Rein = [25.026062, 24.936599, 24.832656, 25.645340, 24.732550, 25.276870, 25.887295, 25.024904, 25.705677, 25.396487, 24.666108, 24.800906]
AWBTree = [6.361334, 6.366255, 6.391283, 6.359521, 6.574019, 7.130220, 6.396807, 7.125382, 6.428486, 7.062830, 6.394400, 6.597237]
HEM5 = [8.031454, 7.876717, 7.802678, 8.218454, 7.788457, 7.846729, 8.373506, 7.858672, 9.686548, 7.865953, 8.206636, 8.319904]
Tama = [6.587494, 6.468266, 6.801501, 6.407236, 6.435439, 6.850017, 6.461399, 6.566833, 6.857723, 6.404602, 6.556920, 6.562363]
pTama_Reduce = [7.194918, 5.502383, 4.959971, 5.094288, 5.194821, 5.467490, 6.212275, 6.674954, 6.865750, 7.652251, 8.066628, 8.615759]
bTama8_p = [9.523389, 4.949102, 3.480178, 2.748821, 2.397993, 2.171812, 2.013974, 1.989769, 1.899493, 1.838742, 1.913386, 1.875977]
bTama6 = [9.448367, 9.608004, 9.519551, 9.692846, 9.722714, 10.239427, 9.854334, 9.743280, 9.586492, 9.642208, 9.382240, 10.133098]
bTama8 = [9.379183, 9.386883, 9.549441, 9.587036, 9.413746, 9.490939, 9.723052, 9.752092, 9.654387, 9.751807, 9.340889, 9.502624]
pTama_Lock = [285.032184, 369.489643, 483.888752, 627.644653, 794.425130, 968.835718, 1166.396499, 1322.012914, 1487.813576, 1624.311017, 1836.734447, 1966.735929]
