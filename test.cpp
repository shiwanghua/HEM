#include "Rein.h"
#include "HEM.h"
#include<iomanip>
#include <iostream>
//#include <omp.h>
//#include<mpi.h>
//#include <unistd.h>

//#include<boost/dynamic_bitset/dynamic_bitset.hpp>
//#include <boost/array.hpp>
//#include <string>
//#include <bitset>
//#include<boost/version.hpp>	//包含boost头文件
//#include<boost/config.hpp>
//#include<boost/thread/thread_pool.hpp>
//#include <iostream>
//#include <queue>
//#include <boost/bind/bind.hpp>
//#include <boost/thread.hpp>
//#include <boost/function.hpp>
//#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
//using namespace boost;
//using namespace boost::placeholders;

//inline uint64_t GetCPUCycle()
//{
//#ifdef __x86_64__
//	unsigned int lo, hi;
//	__asm__ __volatile__("lfence" : : : "memory");
//	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
//	return ((uint64_t)hi << 32) | lo;
//#elif __aarch64__
//	uint64_t v = 0;
//	asm volatile("isb" : : : "memory");
//	asm volatile("mrs %0, cntvct_el0" : "=r"(v));
//	return v;
//#else
//	printf("unknown arch\n");
//	return 0;
//#endif
//}

int gl = 0;
void my_task()
{
	printf("%d\n", gl);
	//sleep(1000);
	gl += 1;
	printf("%d\n", gl);
}

int mainss() {
	// Launch the pool with four threads.
	boost::asio::thread_pool pool(2);
	printf("%d\n", gl);
	// Submit a function to the pool.
	boost::asio::post(pool, my_task);
	pool.wait();
	printf("%d\n", gl);
	system("pause");
	return 0;
}



// 测试或运算
//	const int it = 18;
//	double percent[it] = {0.00005, 0.0001, 0.0005, 0.001, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.0175, 0.02,
//						  0.0225, 0.025, 0.0275, 0.03, 0.0325, 0.8};
//	const int n = 1000000;
//	int ones, t;
//	Util u;
//
//	cout << "n = " << n << "\n\n";
//	_for(i, 0, it) {
//		bitset<n> b1, b2, b3;
//		ones = percent[i] * n;
//		unordered_set<int> s;
//		while (s.size() < ones) {
//			t = rand() % n;
//			if (s.count(t) == 0)
//				s.insert(t);
//		}
//
////		Timer start1;
//		for (auto &&id: s)
//			b1[id] = 1;
////		int64_t t1 = start1.elapsed_nano();
//
//		Timer start2;
//		uint64_t ut2 = GetCPUCycle();
//		b2 = b2 | b1;
//		int64_t t2 = start2.elapsed_nano();
//		ut2 = GetCPUCycle() - ut2;
//
//		Timer start1;
//		uint64_t ut1 = GetCPUCycle();
//		for (auto &&id: s)
//			b3[id] = 1;
//		ut1 = GetCPUCycle() - ut1;
//		int64_t t1 = start1.elapsed_nano();
//
//		cout << "p" << percent[i] << " ones: " << ones << " mark: " << t1 << "ns/" << ut1 << " or: " << t2 <<"ns/"<<ut2<< "\n\n";
//	}

//class cls {
//public:
//	float f = 19.9;
//};
//
//cls test() {
//	cls c;
//	c.f = 33.222;
//	return c;
//}
//
//int main() {
//	cls qq;
//	vector<cls> a;
//	a.emplace_back(qq);
//	a.emplace_back(test());
//	for (auto&& q : a) {
//		printf("%f ", q.f);
//	}
// }
//typedef boost::function<void(void)> MyTask;
//
////任务队列--noncopyable
//class MyTaskQueue : boost::noncopyable
//{
//private:
//	std::queue<MyTask> m_taskQueue;
//	boost::mutex m_mutex;//互斥锁
//	boost::condition_variable_any m_cond;//条件变量
//public:
//	void push_Task(const MyTask& task){
//		//加上互斥锁
//		boost::unique_lock<boost::mutex> lock(m_mutex);
//		m_taskQueue.push(task);
//		//通知其他线程启动
//		m_cond.notify_one();
//	}
//
//	MyTask pop_Task(){
//		//加上互斥锁
//		boost::unique_lock<boost::mutex> lock(m_mutex);
//		if(m_taskQueue.empty())
//		{
//			//如果队列中没有任务，则等待互斥锁
//			m_cond.wait(lock);//
//		}
//		//指向队列首部
//		MyTask task(m_taskQueue.front());
//		//出队列
//		m_taskQueue.pop();
//		return task;
//	}
//	int get_size()
//	{
//		return m_taskQueue.size();
//	}
//};
//
//class MyThreadPool : boost::noncopyable
//{
//private:
//	//任务队列
//	MyTaskQueue m_taskQueue;
//	//线程组
//	boost::thread_group m_threadGroup;
//	int m_threadNum;
//	volatile bool is_run;
//	void run(){//线程池中线程的处理函数
//		while(is_run){
//			//一直处理线程池的任务
//			MyTask task = m_taskQueue.pop_Task();
//			task();//运行bind的函数
//		}
//	}
//public:
//	MyThreadPool(int num):m_threadNum(num),is_run(false)//初始化列表
//	{
//
//	}
//	~MyThreadPool(){
//		stop();
//	}
//	void init()
//	{
//		if(m_threadNum <= 0) return;
//		is_run = true;
//		for (int i=0;i<m_threadNum;i++)
//		{
//			//生成多个线程，绑定run函数，添加到线程组
//			m_threadGroup.add_thread(
//				new boost::thread(boost::bind(&MyThreadPool::run,this)));
//		}
//	}
//	//停止线程池
//	void stop()
//	{
//		is_run = false;
//	}
//	//添加任务
//	void AddNewTask(const MyTask& task){
//		m_taskQueue.push_Task(task);
//	}
//	void wait()
//	{
//		m_threadGroup.join_all();//等待线程池处理完成！
//	}
//};
//
//typedef void (*pFunCallBack)(int i);
//
//void CallBackFun(int i)
//{
//	std::cout << i <<" call back!"<<std::endl;
//}
//
//void ProcFun(int ti,pFunCallBack callback)
//{
//	std::cout<<"I am Task "<<ti<<std::endl;
//	//task
//	for (int i=0;i<ti*100000000;i++)
//	{
//		i*i;
//	}
//	if(callback != NULL)callback(ti);
//}
//
//
//void CallBackFun2(int i)
//{
//	std::cout << i <<" call back! v2"<<std::endl;
//}
//
//int ProcFun2(int& ti)
//{
//	std::cout<<"I am Task "<<ti<<std::endl;
//	//task
//	for (int i=0;i<ti*100000000;i++)
//	{
//		i*i;
//	}
//	return ti;
//}

//int m22ain()
//{
//	MyThreadPool tp(2);
//	tp.init();
//
//	int taskNum = 40;
//	for (int i=0;i<taskNum;i++)
//	{
//		MyTask task = boost::bind(ProcFun,i+1,CallBackFun);
//		//放到线程池中处理，bind(f , i) will produce a "nullary" function object that takes no arguments and returns f(i),调用时，可传递任何类型的函数及参数！！！
//		tp.AddNewTask(task);
//	}
//
//	//等待线程池处理完成！
//	tp.wait();
//	return 0;
//}

//#include <mmintrin.h>  //MMX
//#include <xmmintrin.h> //SSE(include mmintrin.h)
//#include <emmintrin.h> //SSE2(include xmmintrin.h)
//#include <pmmintrin.h> //SSE3(include emmintrin.h)
//#include <tmmintrin.h> //SSSE3(include pmmintrin.h)
//#include <smmintrin.h> //SSE4.1(include tmmintrin.h)
//#include <nmmintrin.h> //SSE4.2(include smmintrin.h)
//#include <wmmintrin.h> //AES(include nmmintrin.h)
//#include <intrin.h>    //(include immintrin.h)

//void printBitset(bitset<subs>& b, string s) {
//	cout <<"\n" << s << ":\n";
//	int k;
//	_for(i, 0, subs / ptrSize) {
//		k = i * ptrSize;
//		cout << "\t";
//		_for(j, k, k + ptrSize) {
//			cout << b[j];
//		}
//		cout << "\n";
//	}
//	cout << "\n";
//}

//cout << mask.m256i_i32[0] << "  " << mask.m256i_i32[1] << endl;


//cout << "Before:\n";
//printBitset(b1, "b1");
//printBitset(b2, "b2");

//bitsetOr(b1, b2);

//cout << "After:\n";
//printBitset(b1, "b1");
//printBitset(b2, "b2");


//float* a = new float;
//*a = 11;
//__m512 m;
//m = _mm512_load_ps(a);
//for (int i = 0; i < 16; i++)
//    cout << m.m512_f32[i] << "\n";


/*boost::dynamic_bitset<unsigned long long> mb(n),b2(n);
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;
	mb[1] = 1;
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;
	mb = mb | b2;
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;*/

//void hello(int& a) {
//	int my_rank = omp_get_thread_num();
//	int thread_count = omp_get_num_threads();
//
//	a = my_rank;
//	//usleep(1000000);
//	printf("hello from rank %d of %d, a= %d\n", my_rank, thread_count, a);
//	fflush(stdout);
//}
//
//void vectorToBitset(vector<int>& v, bitset<subs>*& bt) {
//	cout << "btf1: " << bt << " " << &bt << "\n";
//	for (int i = 0; i < v.size(); i++)
//		(*bt)[v[i]] = 1;
//	v.resize(0);
//}

//bitset<subs>*& vectorToBitset(vector<int>& v) {
//	bitset<subs>* bs = new bitset<subs>;
//	cout << "btf2: " << bs << " " << &bs << "\n";
//	for (auto a : v)
//		(*bs)[a] = 1;
//	v.resize(0);
//	return bs;
//}

//int& t(double d) {
//	int ans = d;
//	cout << "ans= " << &ans << "\n";
//	return ans;
//}

//	int thread_count = 4;
//	int a=5;
//#pragma omp parallel num_threads(thread_count) default(none) shared(a)
//	hello(a);
//bitset<subs>* bs = new bitset<subs>;
//vector<int> v{ 5 };
//cout << "bs1: " << bs << " " << &bs << "\n";
//cout << v.size() << " " << (*bs)[5] << "\n";
//vectorToBitset(v, bs);
//cout << v.size() << " " << (*bs)[5] << "\n";
//v.push_back(999999);
//cout << "bs2: " << bs << " " << &bs << "\n";
//delete bs;
//cout << "bs3: " << bs << " " << &bs << "\n";
//bs = vectorToBitset(v);
//cout << "bs4: " << bs << " " << &bs << "\n";
//cout << v.size() << (*bs)[999999] << " " << (*bs)[5];

//int a = 1;
//cout << "a: " << &a << "\n";
//t(a);
//cout << "a: " << &a << "\n";
//	bitset<1000000> b;
//	b.set();
//	b[999999] = 0;
//	b[12345] = 0;
//	cout << b.count();


//int myid, numprocs, namelen;
//	cout<<MPI_MAX_PROCESSOR_NAME<<"---\n";
//	char processor_name[MPI_MAX_PROCESSOR_NAME];
//	MPI_Init(&argc, &argv);        // starts MPI
//	MPI_Comm_rank(MPI_COMM_WORLD, &myid);  // get current process id
//	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);      // get number of processes
//	MPI_Get_processor_name(processor_name, &namelen);
//	if (myid == 0) printf("number of processes: %d\n...", numprocs);
//	printf("%s: Hello world from process %d \n", processor_name, myid);
//	MPI_Finalize();

