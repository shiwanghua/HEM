#include "Rein.h"
#include "BIOP.h"
#include<iomanip>

void p(int n) {
	int d = 0;
	int nn = n;
	while (nn) {
		d++;
		nn /= 10;
	}
	for (int i = 0; i < 5 - d; i++)
		printf("_");
	printf("%d",n);
}

int main2(int argc, char** argv)
{
	p(1256);
	printf("+");
	p(20);
	printf("=");
	p(1276);
	
	system("pause");

	/*Timer s;
	bool bits[100000000] = { 0 };
	for (int i = 0; i < 100000000; i++)
		bits[rand() % 100000000] = rand()%2;
	cout << s.elapsed_nano() << endl;

	Timer s2;
	bool bits2[100000000] = { 0 };
	for (int i = 0; i < 100000000; i++)
		bits2[rand()%100000000] =rand()%2;
	cout << s2.elapsed_nano() << endl;

	Timer s3;
	for(int i=0;i<100000000;i++)
		bits[i] |= bits2[i];
	cout << s3.elapsed_nano()<<endl;*/

	//cout << bits2[10934] << endl;
	//system("pause");
	/*Timer s3;
	int bits3[1000000];
	cout << s3.elapsed_nano()<<endl;*/

	//long long size;
	//size = 3929399552;
	//size = size / 1024 / 1024; // MB
	//cout<< (int)size;

	// if (   (double)(countSum - currentSum) / (double)subList.size() > subList[0].constraints.size() + 

	//        log(falsePositive + 1) / log((subList[0].constraints[0].highValue - subList[0].constraints[0].lowValue) / (double)1000000))

	//if (9.0 > 10 + log(1.0 + 1) / log(0.8))
	//	cout << "skip it";

	//if (-3.10628)
	//	cout << "skip it2";

	return 0;
}