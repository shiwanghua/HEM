//#pragma once
#ifndef _RUN_MODELS_H
#define _RUN_MODELS_H

#include "constant.h"
#include "rein.h"
#include "ReinBits.h"
#include "ReinBits1.h"
#include "ReinBits2.h"
#include "ReinBits3.h"
#include "ReinBits4.h"
#include "ReinBits5.h"
#include <numeric> 
#include<algorithm>

void run_rein(const intervalGenerator& gen);
void run_ReinBits(const intervalGenerator& gen);
void run_ReinBits1(const intervalGenerator& gen);
void run_ReinBits2(const intervalGenerator& gen);
void run_ReinBits3(const intervalGenerator& gen);
void run_ReinBits4(const intervalGenerator& gen);
void run_ReinBits5(const intervalGenerator& gen);

#endif
