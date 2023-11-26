#include "run_models.h"

int main(int argc, char** argv)
{
	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	if (argc == 11)
	{
		expID = to_string(atoi(argv[1]));
		atts = atoi(argv[2]);
		cons = atoi(argv[3]);
		m = atoi(argv[4]);
		alpha = atof(argv[5]);
		width = atof(argv[6]);
		be = atoi(argv[7]);
		attrGroup = atoi(argv[8]);
		valDom = atoi(argv[9]);
		parallelDegree = atoi(argv[10]);
		cout << "argc=" << argc << endl;
		cout << argv[0] << "  " << argv[1] << "  " << argv[2] << "  " << argv[3] << "  " << argv[4] << "  " << argv[5]
			 << "  " << argv[6] << "  " << argv[7] << "  " << argv[8] << "  " << argv[9] << "  " << argv[10] << "  n="
			 << subs
			 << endl;
	}

	// bind to cpu core
	// int core_id = 8;
	// cpu_set_t mask;
	// CPU_ZERO(&mask);
	// CPU_SET(core_id, &mask);
	// sched_setaffinity(0, sizeof(mask), &mask);

	intervalGenerator gen(subs, pubs, atts, attrGroup, cons, m, attNumType, attDis, valDis, valDom, alpha, subp, width);
	gen.GenSubList();
	gen.GenPubList();
	// gen.GenSubList_g();
	// gen.GenPubList_g();

	unordered_map<int, bool> deleteNo;
	if (verifyID)
	{
		srand(time(NULL));
		int id;
		while (deleteNo.size() < 5000)
		{
			id = rand() % subs;
			while (deleteNo.count(id))
				id = rand() % subs;
			deleteNo[id] = true;
		}
	}

	run_rein(gen, deleteNo);
//	run_rein_forward_native(gen, deleteNo);
//	run_rein_forward_CBOMP(gen, deleteNo);
//	run_rein_hybrid(gen, deleteNo);
//	run_rein_hybrid_CBOMP(gen, deleteNo);
//	run_rein_forward_PGWO(gen, deleteNo);
//	run_rein_forward_PGWO_CBOMP(gen, deleteNo);
//	run_rein_hybrid_PGWO(gen, deleteNo);
//	run_rein_hybrid_PGWO_CBOMP(gen, deleteNo);
//	run_rein_forward_DMFT(gen,deleteNo);
//	run_rein_backward_DMFT(gen,deleteNo);
//	run_pRein(gen, deleteNo);

//	run_AdaRein_ORI(gen, deleteNo);
//	run_AdaRein_SSS(gen, deleteNo);
//	run_AdaRein_SSS_B(gen, deleteNo);
//	run_AdaRein_SSS_C(gen, deleteNo);
//	run_AdaRein_SSS_C_W(gen, deleteNo);
//	run_pAdaRein_SSS_C_W(gen, deleteNo);
//	run_p2AdaRein_SSS_C_W(gen, deleteNo);
//	run_AdaRein_DSS_W(gen, deleteNo);
//	run_AdaRein_DSS_B(gen, deleteNo);

	run_HEM(gen);
	// run_HEM1(gen);
	//	run_HEM2(gen);
	run_HEM3(gen);
	//	run_HEM4(gen);
	run_HEM5(gen, deleteNo);
//	run_HEM5_avxOR(gen, deleteNo);
//	 run_HEM5_parallel(gen, deleteNo);
	//  run_HEM5_avxOR_parallel(gen, deleteNo);
	run_HEM5_VAS(gen, deleteNo);
	run_HEM5_RAS(gen, deleteNo);
	run_HEM5_RAS_avxOR_parallel(gen, deleteNo);
    run_HEM3_D_VASO(gen, deleteNo);
    run_HEM3_D_RASO(gen, deleteNo);
    run_HEM3_D_RASO_parallel(gen, deleteNo);
    //	run_HEMSC(gen);
	//	run_HEMSR(gen);

    //run_Simple(gen, deleteNo);
	//	run_Simple2(gen, deleteNo);

	// run_tama_forward_native(gen, deleteNo);
	// run_tama_forward_C_BOMP(gen, deleteNo); // bTAMA6 C-BOMP
//	run_tama_backward1_DMFT_TAMA_C_BOMP(gen, deleteNo);// bTAMA7
	// run_tama_backward2_DMFT_TAMA_CBOMP(gen, deleteNo); // bTAMA8 DMFT
	// run_btama_backward(gen, deleteNo);
	// run_btama_forward_DMFT(gen, deleteNo);
//	 run_tama_forward_native_parallel_lock(gen,deleteNo); // parallel TAMA_FORWARD-lock very slow
//	 run_tama_forward_native_parallel_reduce(gen,deleteNo); // parallel TAMA_FORWARD-reduce
//	 run_tama_forward_C_BOMP_parallel(gen,deleteNo); // bTAMA6 parallel
//	 run_tama_backward2_DMFT_TAMA_CBOMP_parallel(gen,deleteNo); // bTAMA8 parallel

	// run_BGTREE_forward_native(gen, deleteNo);
//	run_BGTREE_forward_C_BOMP(gen, deleteNo);
//	run_BGTREE_backward_DMFT_fBGTree_CBOMP(gen, deleteNo);
	// run_BGTREE_backward_native(gen,deleteNo);
//	run_BGTREE_forward_DMFT_bBGTree(gen,deleteNo);
//
//	run_BGTREE_d_forward_native(gen, deleteNo);
//	run_BGTREE_d_forward_C_BOMP(gen,deleteNo);
//	run_BGTREE_d_backward_DMFT_fBGTree_CBOMP(gen,deleteNo);
//	run_BGTREE_d_backward_native(gen, deleteNo);
//	run_BGTREE_d_forward_DMFT_bBGTree(gen,deleteNo);
//	 run_BGTREE_d_vrs_backward_native(gen, deleteNo);

//	run_OpIndex(gen, deleteNo);
//		run_bOpIndex2(gen, deleteNo);
	//	run_PSTREE(gen, deleteNo);
//	run_AWBTREE(gen, deleteNo);
//	run_AWBTREE_parallel(gen, deleteNo);
	// measure_numMark(gen);

//	run_rein_forward_native(gen, deleteNo);
//	run_rein_forward_CBOMP(gen, deleteNo);
//	run_rein_hybrid(gen, deleteNo);
//	run_rein_hybrid_CBOMP(gen, deleteNo);
//	run_rein_forward_DMFT(gen,deleteNo);
//	run_rein_backward_DMFT(gen,deleteNo);
//	run_pRein(gen, deleteNo);
//	run_tama_forward_native(gen, deleteNo);
//	run_tama_forward_C_BOMP(gen, deleteNo); // bTAMA6 C-BOMP
//	run_tama_backward2_DMFT_TAMA_CBOMP(gen, deleteNo); // bTAMA8 DMFT
//	run_HEM5(gen, deleteNo);

//	run_AdaRein_ORI(gen, deleteNo);
//	run_HEM5(gen, deleteNo);
//	run_tama_forward_native(gen, deleteNo);
//	run_OpIndex(gen, deleteNo);
//	run_AWBTREE(gen, deleteNo);

	cout << "Done.\n";
	std::system("pause");

	//Util::Check(gen, "rein");

	return 0;
}
