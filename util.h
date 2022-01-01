#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "data_structure.h"
#include "generator.h"
#include "chrono_time.h"
#include "printer.h"
#include <cmath>
#include <vector>
//#include <stdint.h>
//#include <inttypes.h>

using namespace std;

//#include <boost/asio.hpp>
//using namespace boost;
//typedef std::shared_ptr<std::thread> thread_ptr;
//typedef std::vector<thread_ptr> vecThreads;

//class ThreadPool_asio {
//public:
//	ThreadPool_asio(): threadNum_(0), stopped_(false), work_(io_){}
//
//	ThreadPool_asio(int num) : threadNum_(num), stopped_(false), work_(io_) {
//		for(int i=i; i<threadNum_; ++i) {
//			threads_.push_back(std::make_shared<std::thread>([&](){io_.run();}));
//		}
//	}
//
//	~ThreadPool_asio() {
//		stop();
//	}
//
//	void initThreads(int num){
//		threadNum_=num;
//		stopped_=false;
//		threads_.resize(0);
//		for(int i=i; i<threadNum_; ++i) {
//			threads_.push_back(std::make_shared<std::thread>([&](){io_.run();}));
//		}
//	}
//
//	template<typename F, typename...Args>
//	void post(F &&f, Args&&...args) {
//		cout<<"222\n";
//		io_.post(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//	}
//
//	void stop() {
//		if(!stopped_) {
//			io_.stop();
//			for(auto t : threads_) t->join();
//			stopped_ = true;
//		}
//	}
//
//	void join(){
//		for(auto t : threads_) t->join();
//	}
//
//private:
//	bool             stopped_;
//	vecThreads       threads_;
//	int              threadNum_;
//	asio::io_service io_;
//	asio::io_service::work work_;
//};

class Util {
public:
	static void Check(generator gen, string filename);

	static void Check(intervalGenerator gen, string filename);

	static double Average(const vector<double> &a);

	static std::string RemoveBlank(std::string str);

	static std::string RemoveLastZero(std::string str);

	static void WriteData2Begin(string fileName, string text);

	static void WriteData2End(string fileName, string text);

	static string Double2String(double value);

	static double String2Double(string str);

	static string Int2String(int value);

	static int String2Int(string str);

	static bool CheckExist(int value, vector<int> array);

	static bool IsPrime(int number);

	static int Fac(int n);

	static double N2U(double value, double mean, double var);     //normal distribution to uniform distribution
	static double E2U(double value, double mean);     //exponential distribution to uniform distribution
	static double P2U(double value, double mean, double scale);     //pareto distribution to uniform distribution

	static void
	OutputResult(string filename, int ope, int att, int ite, int subDis, int pubDis, double wid, int sub, int pub,
				 double zip, double total, double avg, double max, double min, double std, int matchNo,
				 string matchList);

	static void
	OutputStatistics(string filename, int pubs, int subs, int atts, int comAtts, int cons, double wid, int subDis,
					 int pubDis, double zipf, int testId, vector<int> matSubList, vector<double> matTimeList,
					 vector<vector<int> > priList, vector<vector<double> > detTimeList, int freq[]);

	static vector<double> ComputeDoubleStatistics(vector<double> data);

	static vector<double> ComputeIntStatistics(vector<int> data);

	static void Pub2dPub(Pub pub, dPub &dpub);

	static void bitsetOr(bitset<subs>& b1, bitset<subs>& b2); // b1=b1|b2;
};

#endif
