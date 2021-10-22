#include "constant.h"
string expID = "524";       // Record the experiment ID number
const bool display = false; // whether to print the mapping relation
const bool verifyID = false;// whether to verify Insertion and Deletion function
const int interval = 100;   // after a interval of events is matched, print a log
const double subp = 0.5;    // subp比例的订阅的谓词定义在前cons个维度上 鏈塻ubp姣斾緥鐨�??㈤�?�鐨�?墍鏈夎皳璇嶉兘鍦�?0,cons-1]缁村害锟??, 鍗冲墠cons缁村�?, 浠庤�?屽�?�澶у�?閰嶄釜锟??
//const int subs = 1000000; // Number of subscriptions.
const int pubs = 500;       // Number of publications.
int atts = 200;              // Total number of attributes, i.e. dimensions.
int cons = 5;        // Number of constraints(predicates) in one sub.
int m = 20;                 // Number of constraints in one pub. 鎵€鏈�?�簨浠剁殑鍓峜ons缁撮兘鏈夊畾�???
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform 鍥哄畾瀹藉�?; 1: 闅忔満瀹藉害璋撹瘝(�???width涓烘渶灏忥拷?锟藉�?)
const int valDom = 1000000; // Cardinality of values.
const double alpha = 0;     // Parameter for Zipf distribution.
double width = 0.1;         // Width of a predicate.

// Rein
const int buks = 1000;       // Number of buckets.

// BIOP锛歊ein2
int be = 4;                 // 姣忎釜缁村�??0�??? 2 * 2^bits涓猙its鏁扮�?, -1琛ㄧず鐢ㄦ《鏁板仛涓�?�its鏁扮粍缁�?�? bitExponent
int be2 = 10;               // be绛�?�簬-1灏辩�?be2涓猙itset
const int lvls = 2;         // BIOPSC number of levels
int gs = 5;          // Group Size

// Tama
const int level = 13;

//AdaRein
const double falsePositiveRate = 10;
