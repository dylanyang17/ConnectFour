#include "UCT.h"
#include "Judge.h"
#include <ctime>
#include <cassert>
#define timeNow() ((double)clock()/CLOCKS_PER_SEC)

UCT::UCT(int m, int n, int noX, int noY, ChessBoard *chessBoard)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
	this->chessBoard = chessBoard;
	this->nowRoot = newNode();
}

// 搜索落子点
Point UCT::search()
{
	double inTime = timeNow();
	chessBoard->saveBoard();
	// TODO: 缩小调用 clock() 的次数，例如每 x 次模拟调用一次
	while (timeNow() - inTime < TIME_LIM) {
		int s = treePolicy(nowRoot);
		int delta = defaultPolicy(s);
		updateUp(s, delta);
		chessBoard->loadBoard();
	}
	return Point(0, 0);
}

// 判断 s 结点是否可以向一个新儿子扩展
// 可以则返回选择的列，否则返回 -1
// TODO: 可考虑有策略地优先选某个新儿子，例如加入计分策略，先扩展分数高的
// TODO: 或者对选择的列加入记忆化，减小常数
int UCT::findExpandSon(int s) {
	for (int j = 0; j < n; ++j) {
		if (node[s].son[j] == 0 && chessBoard->top[j] > 0) {
			return j;
		}
	}
	return -1;
}

// 扩展，从结点 s （对应chessBoard）开始走 col 列
// 返回新扩展的结点
int UCT::expand(int s, int col) {
	int t = newNode();
	node[t].parent = s;
	node[s].son[col] = t;
	int row = chessBoard->move(col);
	node[t].status = chessBoard->getStatus(row, col);
	return t;
}

// 从 s 结点，向下找到最可能扩展的结点并进行扩展，返回新扩展出的结点
// 若直至局面结束仍未找到则返回结束局面结点
int UCT::treePolicy(int s) {
	while (node[s].status == 0) {
		int col = -1;
		if (!node[s].expandOver) {
			// 判断是否可扩展，可以则找到放置的列 col
			col = findExpandSon(s);
			if (col == -1) node[s].expandOver = true;
		}
		if (!node[s].expandOver) {
			// 可扩展
			int t = newNode();
			node[t].parent = s;
			node[s].son[col] = t;
			chessBoard->move(col);
			s = t;
			break;
		}
		else {
			// 不可扩展，则向最优的儿子结点走
			assert(node[s].bestColumn != -1);  // TODO
			s = node[s].son[node[s].bestColumn];
		}
	}
	return s;
}


// 从结点 s 开始进行对弈模拟，直至分出胜负，返回收益值（胜: 1; 负: 0; 平: 0.5），只是理论上不可能为负
// TODO: 可以加入计分策略，双方按一定规则走子
int defaultPolicy(int s) {

}


// 向上更新信息
void UCT::updateUp()
{

}


// 获得一个新节点。TODO：垃圾回收和满内存判定
int UCT::newNode() {
	++poolPtr;
	node[poolPtr].init();
	return poolPtr;
}
