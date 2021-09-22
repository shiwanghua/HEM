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
#include "BIOPSR.h"
#include <numeric> 
#include<algorithm>

void run_rein(const intervalGenerator& gen);
void run_BIOP(const intervalGenerator& gen);
void run_BIOP1(const intervalGenerator& gen);
void run_BIOP2(const intervalGenerator& gen);
void run_BIOP3(const intervalGenerator& gen);
void run_BIOP4(const intervalGenerator& gen);
void run_BIOP5(const intervalGenerator& gen);
void run_BIOPSC(const intervalGenerator& gen);
void run_BIOPSR(const intervalGenerator& gen);

#endif
