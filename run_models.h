// #pragma once
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
#include "HEM5_AS.h"
#include "HEMSC.h"
#include "HEMSR.h"

#include "Simple.h"
#include "Simple2.h"

#include "Tama.h"
#include "OpIndex.h"
#include "bTama.h"
#include "BGTree.h"
#include "BGTree_d.h"
#include "BGTree_d_vrs.h"
#include "PSTree.h"
#include "AWBTree.h"

#include <numeric>
#include <algorithm>

void run_rein(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);                    // REIN
void run_rein_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);     // fREIN
void run_rein_forward_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);      // fREIN-C
void run_rein_hybrid(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);             // hREIN
void run_rein_hybrid_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);       // hREIN-C
void run_rein_forward_PGWO(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);       // fREIN-PGWO
void run_rein_forward_PGWO_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo); // fREIN-C-PGWO
void run_rein_hybrid_PGWO(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);        // hREIN-PGWO
void run_rein_hybrid_PGWO_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);  // hREIN-C-PGWO
void run_rein_forward_DMFT(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);       // REIN-F
void run_rein_backward_DMFT(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);      // fREIN-C-B
void run_pRein(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_AdaRein_ORI(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_SSS(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_SDS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_DSS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_DDS(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_IBU(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_PPH(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_SSS_B(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_SSS_C(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_SSS_C_W(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_pAdaRein_SSS_C_W(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_p2AdaRein_SSS_C_W(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_DSS_W(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AdaRein_DSS_B(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
// void run_AdaRein_DSS_B_W(const intervalGenerator& gen, unordered_map<int, bool> deleteNo);

void run_HEM(const intervalGenerator &gen);
void run_HEM1(const intervalGenerator &gen);
void run_HEM2(const intervalGenerator &gen);
void run_HEM3(const intervalGenerator &gen);
void run_HEM4(const intervalGenerator &gen);
void run_HEM5(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_avxOR(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_avxOR_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_VAS(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_RAS(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM5_RAS_avxOR_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_HEM3_D_VASO(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM3_D_RASO(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_HEM3_D_RASO_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_HEMSC(const intervalGenerator &gen); // 订阅分类(宽度分层)+动动模式 Subscriptions Classification
void run_HEMSR(const intervalGenerator &gen); // 基于纯静模式的状态压缩优化 State Reduction

void run_Simple(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_Simple2(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_tama_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);                     // Original forward TANA
void run_tama_forward_native_parallel_lock(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);       // TAMA_FORWARD-FP1
void run_tama_forward_native_parallel_reduce(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);     // TAMA_FORWARD-FP2
void run_tama_forward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);                     // TAMA6-C-BOMP  TAMA6-CF
void run_tama_forward_C_BOMP_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);            // TAMA6-CFP
void run_tama_backward1_DMFT_TAMA_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);         // TAMA7-C-BOMP TAMA_FORWARD-CB1
void run_tama_backward2_DMFT_TAMA_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);          // TAMA8-C-BOMP TAMA_FORWARD-CB2
void run_tama_backward2_DMFT_TAMA_CBOMP_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo); // TAMA_FORWARD-CBP2
void run_btama_backward(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);                          // bTAMA
void run_btama_forward_DMFT(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);                      // bTAMA-F

void run_OpIndex(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_bOpIndex2(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_BGTREE_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_forward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_backward_DMFT_fBGTree_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_backward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_forward_DMFT_bBGTree(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_BGTREE_d_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_forward_C_BOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_backward_DMFT_fBGTree_CBOMP(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_backward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_forward_DMFT_bBGTree(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_BGTREE_d_vrs_forward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_BGTREE_d_vrs_backward_native(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void run_PSTREE(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AWBTREE(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);
void run_AWBTREE_parallel(const intervalGenerator &gen, unordered_map<int, bool> deleteNo);

void measure_numMark(const intervalGenerator &gen);
#endif
