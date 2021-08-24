//#pragma once
#ifndef _RUN_MODELS_H
#define _RUN_MODELS_H

#include "constant.h"
#include "rein.h"
#include "ReinBits.h"
#include "ReinBits2.h"
#include <numeric> 
#include<algorithm>

void run_rein(const intervalGenerator& gen);
void run_ReinBits(const intervalGenerator& gen);
void run_ReinBits2(const intervalGenerator& gen);

#endif
