//#include "Rein.h"
//#include "BIOP.h"
//#include<iomanip>
//#include <iostream>
////#include <omp.h>
////#include<mpi.h>
////#include <unistd.h>
//
//using namespace std;

//void hello(int& a) {
//	int my_rank = omp_get_thread_num();
//	int thread_count = omp_get_num_threads();
//
//	a=my_rank;
//	//usleep(1000000);
//	printf("hello from rank %d of %d, a= %d\n", my_rank, thread_count,a);
//	fflush(stdout);
//}

//int maint(int argc, char **argv)
//
//{
//	int thread_count = 4;
//	int a=5;
//#pragma omp parallel num_threads(thread_count) default(none) shared(a)
//	hello(a);
//	system("pause");
//	return 0;
//}

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
