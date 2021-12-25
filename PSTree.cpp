#include "PSTree.h"

void PSTree::InsertPredicate(const Cnt2& pred, InnerNodeLink pstree, LeafNodeLink& startNode, LeafNodeLink& endNode) {
	//LeafNodeLink startNode = NULL;
	//LeafNodeLink endNode = NULL;

	if (pred.op == 1) { // ">="
		startNode = Partition(pred.value[0], pred.op, pstree);
		endNode = pstree->l;
	}
	if (pred.op == 2) { // "<="
		startNode = pstree->g;
		endNode = Partition(pred.value[0], pred.op, pstree);
	}
	if (pred.op == 3) { // "in"

		startNode = Partition(pred.value[0], 1, pstree); // ">="
		endNode = Partition(pred.value[1], 2, pstree); // "<="
	}

	/*
	while (startNode != endNode->next) {
		//startNode->counter++;
		leafNodes.push_back(startNode);
		startNode = startNode->next;
	}
	*/
}

LeafNodeLink PSTree::Partition(const int& val, const int& op, InnerNodeLink pstreeRoot)
{
	//int64_t cost = 0;
	//Timer tmp;
	InnerNodeLink currNode = pstreeRoot;
	vector<pair<InnerNodeLink, int>> path;
	vector<int> elems;
	PartitionValue(val, elems);
	int i = 1;
	//cost = tmp.elapsed_nano();
	//cout << (double)cost / 1000000.0 << endl;
	//tmp.reset();
	for (auto elem : elems) {
		pair<InnerNodeLink, int> tmp(currNode, elem);
		path.push_back(tmp);
		if (currNode->p[elem] == NULL) // if the inner node not exist, creat it
		{
			currNode->p[elem] = new InnerNode(LENGTHS[i]);
		}
		i++;
		currNode = currNode->p[elem];
	}
	//cost = tmp.elapsed_nano();
	//cout << (double)cost / 1000000.0 << endl;
	//tmp.reset();
	if (currNode->e == NULL) {
		InnerNodeLink iRNode = GetRNode(path); //find the inner node adjacent to and to the right of current node
		//InnerNodeLink iLNode = GetLNode(iRNode, pstreeRoot); // find the minimal left inner node
		PartitionLeafNode(currNode, iRNode, op, val);
	}
	else
	{
		PartitionLeafNode(currNode, op, val);
	}
	//cost = tmp.elapsed_nano();
	//cout << (double)cost / 1000000.0 << endl << endl;
	//tmp.reset();
	//path.clear();
	//elems.clear();
	return currNode->e;
}

LeafNodeLink PSTree::MatchPair(const Pair& p, InnerNodeLink pstreeRoot)
{
	InnerNodeLink currNode = pstreeRoot;
	vector<pair<InnerNodeLink, int>> path; //record the path
	vector<int> elems;
	PartitionValue(p.value, elems); // partition value
	for (auto elem : elems) {
		pair<InnerNodeLink, int> tmp(currNode, elem);
		path.push_back(tmp);
		if (currNode->p[elem] != NULL)  //if inner node has been created, then travel it
			currNode = currNode->p[elem];
		else
		{
			InnerNodeLink iRNode = GetRNode(path);  //find the inner node adjacent to and to the right of current node
			//InnerNodeLink iLNode = GetLNode(currNode, pstreeRoot);  // find the minimal left inner node
			return iRNode->l;
		}
	}
	return currNode->e;
}

InnerNodeLink PSTree::GetRNode(vector<pair<InnerNodeLink, int>>& path) //find the adjacent right node, if not return root
{
	InnerNodeLink currNode = NULL;
	int currElem;
	//int high = psTree_level; //record the high of currNode to leaf node
	//while (!path.empty())
	vector<pair<InnerNodeLink, int>>::reverse_iterator p = path.rbegin();
	for (p; p != path.rend(); p++)
	{
		currNode = p->first;
		currElem = p->second;
		for (int i = currElem + 1; i < currNode->length; i++) {
			if (currNode->p[i] == NULL)continue;
			//if currNode have childe inner node
			currNode = currNode->p[i]; //get first child inner node
			while (currNode->e == NULL) { //if currnode do not have leaf node, find the inner node which have leaf node
				for (int k = 0; k < currNode->length; k++) {
					if (currNode->p[k] == NULL)continue;
					currNode = currNode->p[k];
					break;
				}
			}
			return currNode;
		}
	}
	return currNode;
}

InnerNodeLink PSTree::GetLNode(const InnerNodeLink iRNode, const InnerNodeLink pstreeRoot)
{
	return InnerNodeLink();
}

void PSTree::PartitionValue(const int& val, vector<int>& elems)
{
	//int64_t cost = 0;
	//Timer tmp;
	elems.resize(psTree_level);
	for (int i = 0; i < psTree_level; i++) {
		elems[i] = (val >> movebits[psTree_level - i - 1]) & (LENGTHS[i] - 1);
	}
	//cost = tmp.elapsed_nano();
	//cout << (double)cost / 1000000.0 << endl;
}

void PSTree::PartitionLeafNode(InnerNodeLink currNode, InnerNodeLink iRNode, const int& op, const int& val)
{
	LeafNodeLink newNode = new LeafNode; //creat new leaf node
	//newNode->counter = iRNode->l->counter; //copy counter
	//copy signature
	newNode->signature.bit = iRNode->l->signature.bit;

	//set space record
	//newNode->spaceId[0] = val;
	//newNode->spaceId[1] = iRNode->l->spaceId[1];
	//iRNode->l->spaceId[1] = val;

	//copy sublist
	//newNode->subLinkedList.assign(iRNode->l->subLinkedList.begin(), iRNode->l->subLinkedList.end());
	newNode->subLinkedList = iRNode->l->subLinkedList;

	//add new node to leaf nodes
	newNode->next = iRNode->l->next;
	iRNode->l->next = newNode;
	//link currNode l and g
	currNode->g = newNode;
	currNode->l = iRNode->l;
	//link e
	if (op == 1)
		currNode->e = newNode;  // >=
	else
		currNode->e = iRNode->l; // <=
	//link right node l
	if (iRNode->e == iRNode->l)iRNode->e = newNode;
	iRNode->l = newNode;
}

void PSTree::PartitionLeafNode(InnerNodeLink currNode, const int& op, const int& val)
{
	if (op == 1) { // >=
		if (currNode->e != currNode->g) {
			if (currNode->e != currNode->l)return;
			LeafNodeLink newNode = new LeafNode; //creat new leaf node
			//newNode->counter = currNode->l->counter; //copy counter
			//copy signature
			//newNode->signature = BloomFilter();
			newNode->signature.bit = currNode->l->signature.bit;

			//set space record
			//newNode->spaceId[0] = newNode->spaceId[1] = val;


			//copy sublist
			//newNode->subLinkedList.assign(currNode->l->subLinkedList.begin(), currNode->l->subLinkedList.end());
			newNode->subLinkedList = currNode->l->subLinkedList;

			//add new node to leaf nodes
			newNode->next = currNode->l->next;
			currNode->l->next = newNode;
			//link currNode e
			currNode->e = newNode;
		}
	}
	else { // <=
		if (currNode->e != currNode->l) {
			if (currNode->e != currNode->g)return;
			LeafNodeLink newNode = new LeafNode; //creat new leaf node
			//newNode->counter = currNode->g->counter; //copy counter
			//copy signature
			//newNode->signature = BloomFilter();
			newNode->signature.bit = currNode->g->signature.bit;

			//set space record
			//newNode->spaceId[0] = newNode->spaceId[1] = val;

			//copy sublist
			//newNode->subLinkedList.assign(currNode->g->subLinkedList.begin(), currNode->g->subLinkedList.end());
			newNode->subLinkedList = currNode->g->subLinkedList;

			//add new node to leaf nodes
			newNode->next = currNode->l->next;
			currNode->l->next = newNode;
			//link currNode e
			currNode->e = newNode;
		}
	}
}

void PSTree::insert(Sub2& sub)
{
	//Timer tmp;
	//int64_t cost = 0;
	//cout << "sub: " << sub.id << endl;
	//tmp.reset();
	int accPred = SelectAccPred(sub);
	//cost = tmp.elapsed_nano();
	//cout <<"set accPred: "<< (double)cost / 1000000.0 << endl;
	//cout << "for each pred: " << endl;
	//tmp.reset();
	for (auto pred : sub.constraints) {
		//vector<LeafNodeLink> leafNodes;
		LeafNodeLink startNode, endNode;
		//tmp.reset();
		InsertPredicate(pred, pstrees[pred.att], startNode, endNode);
		//cost = tmp.elapsed_nano();
		//cout << (double)cost / 1000000.0 << endl;

		//tmp.reset();
		if (pred.att == accPred)
		{
			while (startNode != endNode->next) {
				//for each leaf nodes insert sub
				//++startNode->counter;
				startNode->subLinkedList.push_back(sub.id);
				if (startNode->next)
					startNode = startNode->next;
				else break;
			}
		}
		else
		{
			while (startNode != endNode->next) {
				//for each leaf nodes set signature
				//++startNode->counter;
				startNode->signature.SetKey(sub.id);
				if (startNode->next)
					startNode = startNode->next;
				else break;
			}
		}
		//cost = tmp.elapsed_nano();
		//cout << (double)cost / 1000000.0 << endl << endl;
		//leafNodes.clear();
	}
	//cost = tmp.elapsed_nano();
	//cout << (double)cost / 1000000.0 << endl << endl;
	//cout << endl;
}

void PSTree::MatchEvent(const Pub& pub, int& matchSubs, const vector<Sub2>& subList)
{
	vector<LeafNodeLink> leafNodes(attNum, NULL); //all leaf nodes which match event
	for (auto p : pub.pairs) { //find matched leaf nodes
		LeafNodeLink leafNode = MatchPair(p, pstrees[p.att]);
		leafNodes[p.att] = leafNode;
	}
	for (auto node : leafNodes) {
		if (node == NULL)continue;
		for (auto subId : node->subLinkedList) {
			bool isCandidate = true;
			for (auto pred : subList[subId].constraints) {
				if (pred.att == subAccPred[subId])continue;
				if (leafNodes[pred.att] == NULL || !leafNodes[pred.att]->signature.VaryExist(subId)) {
					isCandidate = false;
					break;
				}
			}
			if (isCandidate)
				if (Match(pub, subList[subId]))
					matchSubs++;
		}
	}
	return ;
}

bool PSTree::Match(const Pub& pub, const Sub2& sub)
{
	for (auto p : pub.pairs) {
		if (p.att == subAccPred[sub.id])continue; //pass accPred
		for (auto pred : sub.constraints) {
			if (p.att != pred.att) continue; //find att in sub preds
			switch (pred.op) //check whether even pair match sub pred
			{
			case 1: { // ">="
				if (p.value < pred.value[0])return false;
				break;
			}
			case 2: { // "<="
				if (p.value > pred.value[0])return false;
				break;
			}
			case 3: { // "in"
				if (p.value < pred.value[0])return false;
				if (p.value > pred.value[1])return false;
				break;
			}
			default:
				break;
			}
		}
	}
	return true;
}

int PSTree::SelectAccPred(Sub2& sub)
{
	int max = valDomain + 1;
	int accPred = 0;
	for (auto pred : sub.constraints) {
		int width = pred.value[1] - pred.value[0];
		if (width < max) {
			max = width;
			accPred = pred.att;
		}
	}
	subAccPred[sub.id] = accPred;
	return accPred;
}

int PSTree::calMemory()
{
	size_t sum = 0;
	//inerNum = leafNum = 0;
	for (int i = 0; i < attNum; i++) {
		//inerNum++;
		//leafNum++;
		sum += (sizeof(InnerNodeLink) + sizeof(InnerNode) + sizeof(LeafNode) + mem_size(pstrees[i]) + mem_size(pstrees[i]->g));
	}
	//cout << "inner node num: " << inerNum << " leaf node num: " << leafNum << endl;
	sum = sum / 1024 / 1024; // MB
	return (int)sum;
}

size_t PSTree::mem_size(InnerNodeLink node)
{
	size_t tmp = 0;
	if (node->e == NULL) { //is inner node
		for (int i = 0; i < node->length; i++) {
			if (node->p[i] != NULL) {
				//inerNum++;
				tmp += sizeof(InnerNode) + mem_size(node->p[i]);
			}
		}
	}
	return tmp;
}

size_t PSTree::mem_size(LeafNodeLink node)
{
	size_t tmp = 0;
	while (node->next != NULL) {
		node = node->next;
		//leafNum++;
		tmp += (sizeof(LeafNode) + sizeof(int) * node->subLinkedList.size());
	}
	return tmp;
}
