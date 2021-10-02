#include "constant.h"
string expID = "337";       // 记录实验id
bool display = 0;           // 是否打印映射关系
const int interval = 100;   // 每匹配这么多个事件输出一条日志信息
const double subp = 0;    // 有subp比例的订阅的所有谓词都在[0,cons-1]维度上, 即前cons维度, 从而加大匹配个数
//const int subs = 1000000; // Number of subscriptions.
const int pubs = 500;       // Number of publications.
int atts = 30;             // Total number of attributes, i.e. dimensions.
const int cons = 5;        // Number of constraints(predicates) in one sub.
int m = 30;                 // Number of constraints in one pub. 所有事件的前cons维都有定义
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform 固定宽度; 1: 随机宽度谓词(以width为最小宽度)
const int valDom = 1000000; // Cardinality of values.
const double alpha = 0;     // Parameter for Zipf distribution.
double width = 0.3;         // Width of a predicate.

// Rein
const int buks = 600;       // Number of buckets.

// BIOP：Rein2
int be = 4;                 // 每个维度0上 2 * 2^bits个bits数组, -1表示用桶数做为bits数组组数 bitExponent
const int lvls = 2;         // 层数
const int gs = 4;           // 组的大小     

// Tama
const int level = 11;
