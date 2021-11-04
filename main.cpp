#include "run_models.h"

int main(int argc, char **argv) {
	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom;
	//cin >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	if (argc == 5) {
		expID = to_string(atoi(argv[1]));
//		cons = atoi(argv[2]);
//		m = atoi(argv[2]);
//		atts=atoi(argv[2]);
		alpha=atof(argv[2]);
		be = atoi(argv[3]); // 输入一个bits指数
		width = atof(argv[4]);
		cout << "argc=" << argc << endl;
		cout << argv[0] << "  " << argv[1] << "  " << argv[2] << "  " << argv[3] << "  " << argv[4] << "  n=" << subs
			 << endl;
	}
	intervalGenerator gen(subs, pubs, atts, cons, m, attDis, valDis, valDom, alpha, subp, width);
	gen.GenSubList();//cout<<"Sub fin\n";
	gen.GenPubList2();
	//cout<<"pubs:"<<gen.pubList.size()<<endl;
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
//		for (auto i: deleteNo)
//			cout << i.first << "\n";
	}

	run_rein(gen,deleteNo);
	run_BIOP(gen);
	run_BIOP1(gen);
	run_BIOP2(gen);
	run_BIOP3(gen);
	run_BIOP4(gen);
	run_BIOP5(gen,deleteNo);
//	run_BIOPSC(gen);
//	run_BIOPSR(gen);
	run_Simple(gen,deleteNo);
//	run_Simple2(gen,deleteNo);
	run_opindex(gen, deleteNo);
	run_adarein(gen, deleteNo);
//	cout<<"haha!\n";
	run_tama(gen,deleteNo);

	cout << "Done.\n";
	system("pause");

	//Util::Check(gen, "rein");

	return 0;
}
