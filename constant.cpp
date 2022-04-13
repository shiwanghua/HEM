#include "constant.h"
string expID = "1126";        // Record the experiment ID number
const bool display = false;   // whether to print the mapping relation, 有待逐步改成DEBUG宏定义判断
const bool verifyID = false; // whether to verify Insertion and Deletion function
const int interval = 100;    // after a interval of events is matched, print a log
const double subp = 0;       // subp比例的订阅的谓词定义在前cons个维度上 in order to make the number of matching subscriptions > 0		  
double realMatchNum=-1;      // The correct matching number in an experiment (Recorded in Rein).
//const int subs = 1000000;  // Number of subscriptions.
const int pubs = 100;       // Number of publications.
int atts = 20;               // Total number of attributes, i.e. dimensions.
int attrGroup = 1;         // The number of attribute group in #atts attributes.
int cons = 10;                // Number of constraints(predicates) in one sub.
int m = 20;                  // Number of attributes in one pub.
const int attNumType = 0;    // 0: Fixed size of sub and pub. 1: Random size of sub and pub, <= $cons and $m.
const int attDis = 0;        // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution | 2: sub Zipf, pub 满维
const int valDis = 0;        // The distribution of values in subs and pubs. 0:uniform + fixed width | 1: random width >= $width + Uniform | 2: Zipf + fixed width | 3: Normal + fixed width | 4: Normal + two ends
int valDom = 1000000;  // Cardinality of values.
double alpha = 3;            // Parameter for Zipf distribution.
double width = 0.5;          // Width of a predicate.
double mean = 0.2;           // Mean of Normal Distribution. For events it's ( 1 - mean ).
double stddev = 1;           // Standard deviation: The square root of variance, representing the dispersion of values from the distribution mean. >0

// Rein
const int buks = 1000;       // Number of buckets.
const int awRein_Ppoint = 0.25 * valDom; // HybridRein
//const int awRein_level=4; // The levels of the forward counting matching of HybridRein

// HEM
int be = 1;                 // bit exponent. In each dimension, there are 2 * 2^{be} bitsets.
int be2 = 50;                // When be=-1, be2 is adopted. In each dimension, there are 2 * be2 bitsets.
const int lvls = 2;         // HEMSC number of levels
int gs = 5;                 // HEMSR Group Size 一组有多少个维度
const int blockSize = 256;  // 每次对256位做逻辑运算
const int ptrSize = 64;     // 每次只能加载__int64 64位
const int blockNum = (subs + blockSize - 1) / blockSize; //  把做 subs 位的逻辑运算 简化为做 blockNum 次逻辑运算
const int ptrIncrement = blockSize / ptrSize;       //  需要加载的次数
const __m256i mask = _mm256_set1_epi32(0x80000000);

// Tama
const int level = 13;

//AdaRein
const double falsePositiveRate = 0.05;
const int adarein_level = 2;

// pRein
int parallelDegree = 4;

// BG-Tree
const int initH = 10;           // Initial height
const int MAXNodeSIZE = 2000;
const int boundary = subs * 0.01; // BoundaryNumSub BNS
// boundary>>MAXNodeSIZE

// PS-Tree
const int PSTree_level = 4;

//AWBTree
const float awbTree_Ppoint = 0.25;//partition point
const unsigned short WCsize = 40; //width cell size
const int awbTree_branch = 2000; // 2000

