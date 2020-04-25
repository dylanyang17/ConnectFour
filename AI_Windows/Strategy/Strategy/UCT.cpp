#include "UCT.h"
#include "Judge.h"
#include <ctime>
#define timeNow() ((double)clock()/CLOCKS_PER_SEC)

UCT::UCT(int m, int n, int noX, int noY)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
}

// 搜索落子点
Point UCT::search(int s, int* const* board)
{
	double inTime = timeNow();
	// TODO: 缩小调用 clock() 的次数，例如每 x 次模拟调用一次
	while (timeNow() - inTime < TIME_LIM) {

	}
	board[1][2] = 5;
	return Point(0, 0);
}

// 从 s 结点（对应局面为 board）开始，向下找到最可能扩展的结点并进行扩展，返回新扩展出的结点
// 若直至局面结束仍未找到则返回结束局面结点
// TODO: 可考虑有策略地选点，例如加入计分策略，先扩展分数高的
int UCT::treePolicy(int s, int* const* board) {
	saveBoard(board);
	while (!node[s].isEnd) {
		for (int j = 0; j < n; ++j) {
			if (board[0][j])
		}
	}
	loadBoard(board);
}


// 向上更新信息
void UCT::updateUp()
{

}

// 将局面存入 tmpBoard
void UCT::saveBoard(int* const* board) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			tmpBoard[i][j] = board[i][j];
		}
	}
}

// 将局面从 tmpBoard 取出
void UCT::loadBoard(int* const* board) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = tmpBoard[i][j];
		}
	}
}

// 获得一个新节点。TODO：垃圾回收和满内存判定
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
