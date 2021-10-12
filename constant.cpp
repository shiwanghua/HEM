#include "constant.h"
string expID = "413";       // 记录实验id
bool display = 0;           // �?否打印映射关�?
const int interval = 100;   // 每匹配这么�?�个事件输出一条日志信�?
const double subp = 0;    // 有subp比例的�?�阅的所有谓词都在[0,cons-1]维度�?, 即前cons维度, 从而加大匹配个�?
//const int subs = 1000000; // Number of subscriptions.
const int pubs = 100;       // Number of publications.
int atts = 20;             // Total number of attributes, i.e. dimensions.
const int cons = 10;        // Number of constraints(predicates) in one sub.
int m = 20;                 // Number of constraints in one pub. 所有事件的前cons维都有定�?
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform 固定宽度; 1: 随机宽度谓词(�?width为最小�?�度)
const int valDom = 1000000; // Cardinality of values.
const double alpha = 0;     // Parameter for Zipf distribution.
double width = 0.3;         // Width of a predicate.

// Rein
const int buks = 1000;       // Number of buckets.

// BIOP：Rein2
int be = 0;                 // 每个维度0�? 2 * 2^bits个bits数组, -1表示用桶数做为bits数组组数 bitExponent
int be2 = 10;               // be等于-1就用be2个bitset
const int lvls = 2;         // 层数
const int gs = 4;           // 组的大小     

// Tama
const int level = 11;
