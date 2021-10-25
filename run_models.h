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
#include "Simple.h"
#include "Simple2.h"
#include "Tama.h"
#include "AdaRein.h"
#include "OpIndex.h"
#include <numeric> 
#include<algorithm>

 // The subscription No to be deleted.

void run_rein(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_BIOP(const intervalGenerator& gen);
void run_BIOP1(const intervalGenerator& gen);
void run_BIOP2(const intervalGenerator& gen);
void run_BIOP3(const intervalGenerator& gen);
void run_BIOP4(const intervalGenerator& gen);
void run_BIOP5(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_BIOPSC(const intervalGenerator& gen);
void run_BIOPSR(const intervalGenerator& gen);
void run_Simple(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_Simple2(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_tama(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_adarein(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_opindex(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);

#endif
