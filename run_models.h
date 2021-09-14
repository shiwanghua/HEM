//#pragma once
#ifndef _RUN_MODELS_H
#define _RUN_MODELS_H

#include "constant.h"
#include "Rein.h"
#include "BIOP.h"
#include "BIOP1.h"
#include "BIOP2.h"
#include "BIOP3.h"
#include "BIOP4.h"
#include "BIOP5.h"
#include "BIOPSC.h"
#include <numeric> 
#include<algorithm>

const int interval = 1000; // 每匹配这么多个事件输出一条日志信息

void run_rein(const intervalGenerator& gen);
void run_BIOP(const intervalGenerator& gen);
void run_BIOP1(const intervalGenerator& gen);
void run_BIOP2(const intervalGenerator& gen);
void run_BIOP3(const intervalGenerator& gen);
void run_BIOP4(const intervalGenerator& gen);
void run_BIOP5(const intervalGenerator& gen);
void run_BIOPSC(const intervalGenerator& gen);
#endif
