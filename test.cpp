#include "Rein.h"
#include "BIOP.h"
#include<iomanip>
#include <iostream>
using namespace std;

void print(char*, int = 0); // 默认值应该加在函数声明中，即int后面“=0”

// Don't modify main function.
int main222()
{
	char str[12] = "OneTwoThree";
	print(str);
	print(str, 2);
	print(str, 0);
	print(str, 44);
	return 0;
}

// 请在下面实现print函数
void print(char str[], int p) {
	static int numCall = 0;
	numCall++;
	if (p != 0) {
		for (int i = 0; i < numCall; i++)
			cout << "第" << numCall << "次调用:" << str << "\n";
	}
}