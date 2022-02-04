#include "run_models.h"

int main(int argc, char **argv) {
	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	if (argc == 5) {
		expID = to_string(atoi(argv[1]));
//		cons = atoi(argv[2]);
		m = atoi(argv[2]);
//		atts=atoi(argv[2]);
//		alpha=atof(argv[2]);
		be2 = atoi(argv[3]); // 输入一个bits指数
		width = atof(argv[4]);
		cout << "argc=" << argc << endl;
		cout << argv[0] << "  " << argv[1] << "  " << argv[2] << "  " << argv[3] << "  " << argv[4] << "  n=" << subs
			 << endl;
	}

	intervalGenerator gen(subs, pubs, atts, cons, m, attDis, valDis, valDom, alpha, subp, width);
	gen.GenSubList();
	gen.GenPubList2();
	
	unordered_map<int,bool> deleteNo;
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

	run_rein(gen,deleteNo);
//	run_rein_forward_native(gen, deleteNo);
//	run_rein_forward_CBOMP(gen, deleteNo);
//	run_rein_hybrid(gen,deleteNo);
//	run_rein_hybrid_CBOMP(gen,deleteNo);
//  run_pRein(gen,deleteNo);
//	run_adarein(gen, deleteNo);

	run_HEM(gen);
//	run_HEM1(gen);
//	run_HEM2(gen);
//	run_HEM3(gen);
//	run_HEM4(gen);
//    run_HEM5(gen,deleteNo);
	//run_HEM5_avxOR(gen, deleteNo);
//	run_HEMSC(gen);
//	run_HEMSR(gen);

//	run_Simple(gen,deleteNo);
//	run_Simple2(gen,deleteNo);
//
//	run_tama(gen,deleteNo);
//	run_btama_forward_C_BOMP(gen, deleteNo); // bTAMA6
//	run_btama_backward1_C_BOMP(gen, deleteNo);// bTAMA7
//	run_btama_backward2_CBOMP(gen, deleteNo); // bTAMA8
//
	//run_BGTREE_forward(gen, deleteNo);
//	run_BGTREE_forward_C_BOMP(gen,deleteNo);
//	run_BGTREE_backward_C_BOMP(gen, deleteNo);
//	run_BGTREE_d_forward(gen, deleteNo);

//	run_OpIndex(gen, deleteNo);
//	run_bOpIndex2(gen,deleteNo);
	//run_PSTREE(gen, deleteNo);
//	run_AWBTREE(gen, deleteNo);
	//measure_numMark(gen);

	cout << "Done.\n";
	std::system("pause");

	//Util::Check(gen, "rein");

	return 0;
}
