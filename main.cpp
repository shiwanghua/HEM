#include "run_models.h"

int main(int argc, char **argv)
{
	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom;
	//cin >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	if (argc == 5)
	{
		expID = to_string(atoi(argv[1]));
		atts = m = atoi(argv[2]);
		be = atoi(argv[3]); // 输入一个bits指数
		width = atof(argv[4]);
		cout << "argc=" << argc << endl;
		cout << argv[0] << "  " << argv[1] << "  " << argv[2] << "  " << argv[3] << "  " << argv[4] << "  n=" << subs << endl;
	}
	intervalGenerator gen(subs, pubs, atts, cons, m, attDis, valDis, valDom, alpha, subp, width);
	gen.GenSubList();
	gen.GenPubList();

	run_rein(gen);
	run_BIOP(gen);
	run_BIOP1(gen);
	run_BIOP2(gen);
	run_BIOP3(gen);
	run_BIOP4(gen);
	run_BIOP5(gen);
	run_BIOPSC(gen);
	run_BIOPSR(gen);
	run_Simple(gen);
	run_Simple2(gen);
	run_tama(gen);
	

	cout << "Done.\n";
	system("pause");

	//Util::Check(gen, "rein");

	return 0;
}
