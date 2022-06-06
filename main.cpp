#include "run_models.h"

int main(int argc, char **argv) {
	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	if (argc == 11) {
		expID = to_string(atoi(argv[1]));
		atts = atoi(argv[2]);
		cons = atoi(argv[3]);
		m = atoi(argv[4]);
		alpha = atof(argv[5]);
		width = atof(argv[6]);
		be = atoi(argv[7]); // 输入一个bits指数
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
//	gen.GenSubList_g();
//	gen.GenPubList_g();

	unordered_map<int, bool> deleteNo;
	if (verifyID) {
		srand(time(NULL));
		int id;
		while (deleteNo.size() < 5000) {
			id = rand() % subs;
			while (deleteNo.count(id))
				id = rand() % subs;
			deleteNo[id] = true;
		}
	}

	run_rein(gen, deleteNo);
	////	run_rein_forward_native(gen, deleteNo);
	////	run_rein_forward_CBOMP(gen, deleteNo);
	////	run_rein_hybrid(gen, deleteNo);
	////	run_rein_hybrid_CBOMP(gen, deleteNo);
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
//	run_HEM(gen);
	////	run_HEM1(gen);
	////	run_HEM2(gen);
	////	run_HEM3(gen);
	////	run_HEM4(gen);
	run_HEM5(gen, deleteNo);
//	run_HEM5_avxOR(gen, deleteNo);
	// run_HEM5_parallel(gen, deleteNo);
	// run_HEM5_avxOR_parallel(gen, deleteNo);
	run_HEM5_VAS(gen, deleteNo);
//if(parallelDegree<5)
//	run_HEM5_RAS(gen, deleteNo);
//	run_HEM5_RAS_avxOR_parallel(gen, deleteNo);
	////	run_HEMSC(gen);
	////	run_HEMSR(gen);
	////
	//run_Simple(gen, deleteNo);
	//	run_Simple2(gen, deleteNo);
	//
	run_tama(gen, deleteNo);
	//	run_btama_forward_C_BOMP(gen, deleteNo); // bTAMA6
	//	run_btama_backward1_C_BOMP(gen, deleteNo);// bTAMA7
	//	run_btama_backward2_CBOMP(gen, deleteNo); // bTAMA8
	//
	//	run_BGTREE_forward_native(gen, deleteNo);
	//	run_BGTREE_forward_C_BOMP(gen, deleteNo);
	//	run_BGTREE_backward_C_BOMP(gen, deleteNo);
	//	run_BGTREE_d_forward_native(gen, deleteNo);
	//	run_BGTREE_d_backward_native(gen, deleteNo);
	//	run_BGTREE_d_vrs_backward_native(gen, deleteNo);
	//
	run_OpIndex(gen, deleteNo);
	//	run_bOpIndex2(gen, deleteNo);
	//	run_PSTREE(gen, deleteNo);
	//	run_AWBTREE(gen, deleteNo);
	//	measure_numMark(gen);

	cout << "Done.\n";
	std::system("pause");

	//Util::Check(gen, "rein");

	return 0;
}
