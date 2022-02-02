//#pragma once
#ifndef _RUN_MODELS_H
#define _RUN_MODELS_H

#include "constant.h"
#include "Rein.h"
#include "pRein.h"
#include "AdaRein.h"

#include "HEM.h"
#include "HEM1.h"
#include "HEM2.h"
#include "HEM3.h"
#include "HEM4.h"
#include "HEM5.h"
#include "HEM5_avxOR.h"
#include "HEMSC.h"
#include "HEMSR.h"

#include "Simple.h"
#include "Simple2.h"

#include "Tama.h"
#include "OpIndex.h"
#include "bTama.h"
#include "BGTree.h"
#include "BGTree_d.h"
#include "PSTree.h"
#include "AWBTree.h"

#include <numeric> 
#include<algorithm>

void run_rein(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_rein_forward_native(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_rein_forward_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_rein_hybrid(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_rein_hybrid_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_pRein(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_adarein(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);

void run_HEM(const intervalGenerator& gen);
void run_HEM1(const intervalGenerator& gen);
void run_HEM2(const intervalGenerator& gen);
void run_HEM3(const intervalGenerator& gen);
void run_HEM4(const intervalGenerator& gen);
void run_HEM5(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_HEM5_avxOR(const intervalGenerator& gen, unordered_map<int,bool> deleteNo);
void run_HEMSC(const intervalGenerator& gen);
void run_HEMSR(const intervalGenerator& gen);

void run_Simple(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_Simple2(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);

void run_tama(const intervalGenerator& gen,unordered_map<int,bool> deleteNo); // Original forward TANA
void run_btama_forward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);   // bTAMA6
void run_btama_backward1_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);  // bTAMA7
void run_btama_backward2_CBOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo); // bTAMA8


void run_OpIndex(const intervalGenerator& gen,unordered_map<int,bool> deleteNo);
void run_bOpIndex2(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_BGTREE_forward(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_forward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_backward_C_BOMP(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_forward(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);

void run_PSTREE(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_AWBTREE(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);

void measure_numMark(const intervalGenerator& gen);
#endif
