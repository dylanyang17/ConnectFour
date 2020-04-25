#include "UCT.h"
#include "Judge.h"
#include <ctime>
#include <random>
#include <cassert>
#include <cmath>
#define timeNow() ((double)clock()/CLOCKS_PER_SEC)
#define DEBUG    // TODO!!!

UCT::UCT(int m, int n, ChessBoard *chessBoard)
{
	this->m = m;
	this->n = n;
	this->chessBoard = chessBoard;
	this->nowRoot = newNode();
#ifdef DEBUG
	this->debugOn = true;
#endif
}

// 进行了真实的落子，返回落子的行数
// TODO: 垃圾回收
int UCT::realMove(int col) {
	if (node[nowRoot].son[col] == 0) {
		// 该儿子结点并未扩展
		int row;
		nowRoot = expand(nowRoot, col, row);
		return row;
	}
	else {
		nowRoot = node[nowRoot].son[col];
		return chessBoard->move(col);
	}
}


// TODO: 尝试记录 bestColumn，略微修改公式
int UCT::calcBestColumn(int s, bool debug) {
	double bestScore;
	int bestColumn = -1;
	for (int j = 0; j < n; ++j) {
		int son = node[s].son[j];
		if (son) {
			double score = calcScore(s, node[s].son[j]);
			if (debug) {
				fprintf(stderr, "（%d: %d, %d, %f)\n", j, node[son].win, node[son].tot, score);
			}
			if (bestColumn == -1 || score > bestScore) {
				bestScore = score;
				bestColumn = j;
			}
		}
	}
	assert(bestColumn != -1);  // TODO
	return bestColumn;
}


// 搜索最佳的落子列
int UCT::search()
{
	double inTime = timeNow();
	chessBoard->saveBoard();
	// TODO: 缩小调用 clock() 的次数，例如每 x 次模拟调用一次
#ifdef DEBUG
	for (int i = 1; i <= 100000; ++i) {
#else
	while (timeNow() - inTime < TIME_LIM) {
#endif
		// fprintf(stderr, "LALALA\n");
		int s = treePolicy(nowRoot);
		int delta = defaultPolicy(s);
		updateUp(s, delta);
		chessBoard->loadBoard();
	}
	int bestColumn = calcBestColumn(nowRoot, debugOn);
	return bestColumn;
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
int UCT::expand(int s, int col, int &row) {
	int t = newNode();
	node[t].parent = s;
	node[t].parColumn = col;
	node[s].son[col] = t;
	row = chessBoard->move(col);
	node[t].status = chessBoard->getStatus();
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
			assert(col != -1);  // TODO
			int row;
			s = expand(s, col, row);
			break;
		}
		else {
			// 不可扩展，则向最优的儿子结点走
			//assert(node[s].bestColumn != -1);  // TODO
			//chessBoard->move(node[s].bestColumn);
			//s = node[s].son[node[s].bestColumn];
			int bestColumn = calcBestColumn(s);
			chessBoard->move(bestColumn);
			s = node[s].son[bestColumn];
		}
	}
	return s;
}


// 从结点 s 开始进行对弈模拟，直至分出胜负，返回 s 结点收益值（即从**进入 s 状态者**角度考虑，胜: 1; 负: 0; 平: 0.5）
// TODO: 可以加入计分策略，双方按一定规则走子
int UCT::defaultPolicy(int s) {
	int turn = 3 - chessBoard->turn;  // DEBUG: 注意应从**落子进入 s 状态者**角度考虑，这样每次尽量往更大的方向走才是正确的
	int status;
	while ((status = chessBoard->getStatus()) == 0) {
		int col;
		do {
			col = rand() % m;
		} while (chessBoard->top[col] == 0);
		chessBoard->move(col);
	}
	if (turn == status) {
		return 1;
	}
	else if (status == 3) {
		return 0.5;
	}
	else {
		return 0;
	}
}


// 计算 s 为父亲时，儿子 t 对应的得分
double UCT::calcScore(int s, int t) {
	return (double)node[t].win / node[t].tot + alpha * sqrt(2 * log(node[s].tot) / node[t].tot);
}


// 向上更新信息
void UCT::updateUp(int s, int delta)
{
	int last = -1;
	do {
		node[s].tot++;
		node[s].win += delta;
		//if (last != -1) {  // TODO: 可以尝试使用 bestColumn 并且更改计分形态以改进算法
		//	double score = calcScore(s, last);
		//	if (node[s].bestColumn == -1 || node[s].bestColumnScore < score) {
		//		node[s].bestColumn = node[last].parColumn;
		//		node[s].bestColumnScore = score;
		//	}
		//}
		last = s;
		s = node[s].parent;
		delta = 1 - delta;
	} while (last != nowRoot);
}


// 获得一个新节点。TODO：垃圾回收和满内存判定
int UCT::newNode() {
	++poolPtr;
	node[poolPtr].init();
	return poolPtr;
}
