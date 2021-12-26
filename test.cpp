#include "Rein.h"
#include "HEM.h"
#include<iomanip>
#include <iostream>
//#include <omp.h>
//#include<mpi.h>
//#include <unistd.h>

using namespace std;

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

bitset<subs>*& vectorToBitset(vector<int>& v) {
	bitset<subs>* bs = new bitset<subs>;
	cout << "btf2: " << bs << " " << &bs << "\n";
	for (auto a : v)
		(*bs)[a] = 1;
	v.resize(0);
	return bs;
}

//int& t(double d) {
//	int ans = d;
//	cout << "ans= " << &ans << "\n";
//	return ans;
//}

int m7ain(int argc, char** argv)
{
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
	bitset<1000000> b;
	b.set();
	b[999999] = 0;
	b[12345] = 0;
	cout << b.count();
	system("pause");
	return 0;
}

	

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

