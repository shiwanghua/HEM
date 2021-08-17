#pragma once
#include<iostream>
using namespace std;

const string expID = "37";  // 记录实验id
const int subs = 1000000;   // Number of subscriptions.
const int pubs = 200;       // Number of publications.
const int atts = 20;        // Total number of attributes, i.e. dimensions.
const int cons = 10;        // Number of constraints(predicates) in one sub.
const int m = 20;           // Number of constraints in one pub.
const int attDis = 0;       // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
const int valDis = 0;       // The distribution of values in subs and pubs. 0:uniform
const int valDom = 1000000; // Cardinality of values.
const double alpha = 0.0;   // Parameter for Zipf distribution.
const double width = 0.3;   // Width of a predicate.

// Rein
const int buks = 1000;      // Number of buckets.

// ReinBits：Rein2
const int bits = 2;         // 每个维度上 2 * 2^bits个bits数组

// Tama
const int level = 11;



