#include "UCT.h"

// �������ӵ�
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

// ���ϸ�����Ϣ
void UCT::updateUp()
{

}

// ��ս����Ϣ
int UCT::clearNode(int s) {
	memset(node[s].son, 0, sizeof(node[s].son));
	node[s].parent = NULL;
	return s;
}

// ���һ���½ڵ㡣TODO���������պ����ڴ��ж�
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
