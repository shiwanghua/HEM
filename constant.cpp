#include "constant.h"
string expID = "680";       // Record the experiment ID number
const bool display = false; // whether to print the mapping relation
const bool verifyID = true; // whether to verify Insertion and Deletion function
const int interval = 100;   // after a interval of events is matched, print a log
const double subp = 0;      // subp比例的订阅的谓词定义在前cons个维度上 in order to make the number of matching subscriptions > 0
//const int subs = 1000000; // Number of subscriptions.
const int pubs = 500;       // Number of publications.
int atts = 20;              // Total number of attributes, i.e. dimensions.
int cons = 10;              // Number of constraints(predicates) in one sub.
int m = 20;                 // Number of constraints in one pub.
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform + fixed width | 1: random width >= $width + Uniform | 2: Zipf + fixed width | 3: Normal + fixed width | 4: Normal + two ends
const int valDom = 1000000; // Cardinality of values.
double alpha = 0;           // Parameter for Zipf distribution.
double width = 0.3;         // Width of a predicate.
double mean = 0.2;          // Mean of Normal Distribution. For events it's ( 1 - mean ).
double stddev = 1;          // Standard deviation: The square root of variance, representing the dispersion of values from the distribution mean. >0

// Rein
const int buks = 1000;       // Number of buckets.

// BIOP
int be = 5;                // bit exponent. In each dimension, there are 2 * 2^{be} bitsets.
int be2 = 10;               // When be=-1, be2 is adopted. In each dimension, there are 2 * be2 bitsets.
const int lvls = 2;         // BIOPSC number of levels
int gs = 5;                 // BIOPSR Group Size 一组有多少个维度

// Tama
const int level = 13;

//AdaRein
const double falsePositiveRate = 0.05;
