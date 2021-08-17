#include "run_models.h"

int main(int argc, char** argv)
{

	//freopen("paras.txt", "r", stdin);
	//cin >> subs >> pubs >> atts >> cons>> buks >> m >> attDis >> valDis >> valDom;
	//cin >> alpha >> width;
	//m = atts; // Note that Rein requires m == atts.

	// Initiate generator
	intervalGenerator gen(subs, pubs, atts, cons, m, attDis, valDis, valDom, alpha, width);
	gen.GenSubList();
	gen.GenPubList();

	//run_rein(gen);
	run_ReinBits(gen);

	cout << "Done.\n";
	system("pause");
	// check
	//Util::Check(gen, "rein");

	return 0;
}
