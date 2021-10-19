#include "constant.h"
string expID = "459";       // 璁板綍瀹為獙id
bool display = 0;           // 锟??鍚︽墦鍗版槧灏勫叧锟??
const int interval = 100;   // 姣忓尮閰嶈繖涔堬拷?锟戒釜浜嬩欢杈撳嚭涓€鏉℃棩蹇椾俊锟??
const double subp = 0;      // 鏈塻ubp姣斾緥鐨勮?㈤槄鐨勬墍鏈夎皳璇嶉兘鍦╗0,cons-1]缁村害锟??, 鍗冲墠cons缁村害, 浠庤€屽姞澶у尮閰嶄釜锟??
//const int subs = 1000000; // Number of subscriptions.
const int pubs = 500;       // Number of publications.
int atts = 20;              // Total number of attributes, i.e. dimensions.
const int cons = 10;        // Number of constraints(predicates) in one sub.
int m = 20;                 // Number of constraints in one pub. 鎵€鏈変簨浠剁殑鍓峜ons缁撮兘鏈夊畾锟??
const int attDis = 1;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform 鍥哄畾瀹藉害; 1: 闅忔満瀹藉害璋撹瘝(锟??width涓烘渶灏忥拷?锟藉害)
const int valDom = 1000000; // Cardinality of values.
const double alpha = 2;     // Parameter for Zipf distribution.
double width = 0.5;         // Width of a predicate.

// Rein
const int buks = 1000;       // Number of buckets.

// BIOP锛歊ein2
int be = 4;                 // 姣忎釜缁村害0锟?? 2 * 2^bits涓猙its鏁扮粍, -1琛ㄧず鐢ㄦ《鏁板仛涓篵its鏁扮粍缁勬暟 bitExponent
int be2 = 10;               // be绛変簬-1灏辩敤be2涓猙itset
const int lvls = 2;         // BIOPSC number of levels
const int gs = 10;          // 缁勭殑澶у皬

// Tama
const int level = 13;

//AdaRein
const double falsePositiveRate = 1;

