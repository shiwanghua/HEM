#include "BGTree.h"

BGTree::BGTree() {
	numSub = 0;
	boundaryNumSub = subs / 32;
	numNode = 1;
	height = 1;
	initHeight = initH;
	root = new bluenode(0, valDom - 1, 1, 1, {},nullptr, nullptr, nullptr, nullptr, nullptr);
	initBlueNode(root);
}

BGTree::~BGTree() {
	releaseBlueNode(root);
	delete root;
}

void BGTree::initBlueNode(bluenode* r) {
	if (r->levelid == initHeight)return;

}

void BGTree::initGreenNode(lgreennode* r) {
	if (r->levelid == initHeight)return;
}

void BGTree::initGreenNode(rgreennode* r) {
	if (r->levelid == initHeight)return;
}

void BGTree::releaseBlueNode(bluenode* r) {

}

void BGTree::releaseGreenNode(lgreennode* r) {

}

void BGTree::releaseGreenNode(rgreennode* r) {

}

void BGTree::insert(IntervalSub sub) {

}

void BGTree::match(const Pub& pub, int& matchSubs) {

}

int BGTree::calMemory() {

}

void BGTree::printBGTree() {

}