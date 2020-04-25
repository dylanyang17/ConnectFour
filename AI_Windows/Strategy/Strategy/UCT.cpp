#include "UCT.h"
#include "Judge.h"
#include <ctime>
#define timeNow() ((double)clock()/CLOCKS_PER_SEC)

UCT::UCT(int m, int n, int noX, int noY, ChessBoard *chessBoard)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
	this->chessBoard = chessBoard;
}

// 搜索落子点
Point UCT::search(int s)
{
	double inTime = timeNow();
	// TODO: 缩小调用 clock() 的次数，例如每 x 次模拟调用一次
	while (timeNow() - inTime < TIME_LIM) {

	}
	return Point(0, 0);
}

// 判断 s 结点是否可以向一个新儿子扩展
// 可以则返回这个儿子结点的索引，否则返回 -1
// TODO: 可考虑有策略地优先选某个新儿子，例如加入计分策略，先扩展分数高的
int UCT::findExpandSon(int s) {
	for (int j = 0; j < n; ++j) {

	}
}

// 从 s 结点，向下找到最可能扩展的结点并进行扩展，返回新扩展出的结点
// 若直至局面结束仍未找到则返回结束局面结点
int UCT::treePolicy(int s) {
	chessBoard->saveBoard();
	while (!node[s].isEnd) {
		int t = -1;
		if (!node[s].expandOver) {
			// 判断是否可扩展
			t = findExpandSon(s);
			if (t == -1) node[s].expandOver = true;
		}
		if (!node[s].expandOver) {
			// 可扩展
		}
		else {
			// 不可扩展
		}
	}
	chessBoard->loadBoard();
}


// 向上更新信息
void UCT::updateUp()
{

}



// 获得一个新节点。TODO：垃圾回收和满内存判定
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
