#include "UCT.h"

// 搜索落子点
Point UCT::search(int s, int* const* board)
{
	// TODO
	return Point(0, 0);
}

UCT::UCT(int m, int n, int noX, int noY)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
}

// 向上更新信息
void UCT::updateUp()
{

}

// 清空结点信息
int UCT::clearNode(int s) {
	memset(node[s].son, 0, sizeof(node[s].son));
	node[s].parent = NULL;
	return s;
}

// 获得一个新节点。TODO：垃圾回收和满内存判定
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
