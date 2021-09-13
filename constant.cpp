#include "constant.h"
string expID = "280";       // 记录实验id
bool display = false;       // 是否打印映射关系
//const int subs = 1000000;   // Number of subscriptions.
const int pubs = 500;     // Number of publications.
int atts = 20;              // Total number of attributes, i.e. dimensions.
const int cons = 10;        // Number of constraints(predicates) in one sub.
int m = 20;                 // Number of constraints in one pub.
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 1;       // The distribution of values in subs and pubs. 0:uniform 固定宽度; 1: 随机宽度谓词(以width为最小宽度)
const int valDom = 1000000; // Cardinality of values.
const double alpha = 0;     // Parameter for Zipf distribution.
double width = 0;   // Width of a predicate.

// Rein
const int buks = 1000;      // Number of buckets.

// BIOP：Rein2
int be = 4;                 // 每个维度上 2 * 2^bits个bits数组, -1表示用桶数做为bits数组组数 bitExponent
const int lvls = 2;               // 层数

// Tama
const int level = 11;
