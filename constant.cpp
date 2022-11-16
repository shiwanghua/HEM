#include "constant.h"
string expID = "1520";       // Record the experiment ID number
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

// pRein pHEM5DD pHEM5DD-avxOR pHEM5DD-RAS-avxOR
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

// bTama7 = [5.962530, 5.188889, 4.421234, 3.829108, 3.161978, 2.493683, 2.012853, 1.563810, 1.119772]
// HEM5 = [3.960543, 4.013972, 2.726451, 1.981608, 1.643803, 1.528571, 1.578585, 1.660534, 1.779366]
// Rein = [5.025923, 5.445188, 5.008369, 4.830441, 5.290256, 6.064501, 7.165692, 8.204872, 9.425269]
// BGTree = [10.565614, 10.538955, 10.069476, 9.347731, 8.632596, 8.655023, 7.426536, 5.927116, 3.962853]
// bTama8 = [6.113363, 5.373554, 4.564207, 3.944776, 3.405798, 2.530594, 2.038193, 1.550895, 1.087566]
// Tama = [11.757325, 11.762547, 11.377197, 10.758907, 10.113908, 10.192658, 9.143969, 7.671799, 5.662435]
// bTama6 = [5.882992, 5.331468, 4.392576, 3.734359, 3.120649, 2.447185, 1.979264, 1.512233, 1.066858]

Rein = [2.664667, 3.010818, 2.908030, 3.073742, 3.360381, 3.915198, 4.645796, 5.436102, 6.273794]
HEM5 = [2.359900, 2.330890, 1.647440, 1.295291, 1.122749, 1.078898, 1.102729, 1.191126, 1.218804]
Tama = [5.062257, 5.160872, 4.448119, 3.827322, 3.919963, 3.399017, 3.083890, 2.727604, 2.163465]
BGTree1 = [4.988428, 4.180999, 3.889811, 2.997181, 2.460201, 2.061078, 1.656653]
BGTree2 = [4.237138, 3.700077, 3.887929, 2.720784, 2.300669, 1.940185, 1.613157]
BGTree_d_f = [4.651387, 4.430231, 4.260319, 4.483789, 4.070096, 3.702875, 3.353151]
bTama6 = [4.849175, 4.208232, 3.518227, 2.916535, 2.383723, 1.837066, 1.417925, 1.059460, 0.689654]
bTama7 = [5.262942, 4.352638, 3.640907, 2.958713, 2.413382, 1.893755, 1.435684, 1.086202, 0.715681]
BGTree = [4.909205, 4.762395, 4.411733, 3.681613, 3.343769, 3.326247, 2.949817, 2.568127, 2.185528]
bTama8 = [4.382805, 3.618107, 3.180977, 2.581640, 2.155547, 1.689677, 1.331483, 1.031713, 0.687108]