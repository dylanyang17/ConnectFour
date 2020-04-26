#include "UCT.h"
#include "Judge.h"
#include "Point.h"
#include "ChessBoard.h"
#include <ctime>
#include <random>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#define timeNow() ((double)clock()/CLOCKS_PER_SEC)
#define DEBUG

UCT::UCT()
{
	;
}

UCT::UCT(int m, int n, ChessBoard* chessBoard)
{
	init(m, n, chessBoard);
}

// 重开一盘棋局
void UCT::init(int m, int n, ChessBoard* chessBoard) {
	while (!trash.empty())  trash.pop();
	poolPtr = 0;
	this->m = m;
	this->n = n;
	this->chessBoard = chessBoard;
	this->nowRoot = newNode();
#ifdef DEBUG
	this->debugOn = true;
#endif

}

// 进行了真实的落子，返回落子的行数。进行了垃圾回收
int UCT::realMove(int col) {
	for (int j = 0; j < n; ++j) {
		if (j != col && node[nowRoot].son[j]) {
			trash.push(node[nowRoot].son[j]);
		}
	}
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
				fprintf(stderr, "（%d: %d, %d, %s, %d, %f)\n", j, node[son].win, node[son].tot, (node[son].certWin ? "certWin" : (node[son].certLose ? "certLose" : "none")), node[son].certStep, score);
			}
			if (bestColumn == -1 || score > bestScore) {
				bestScore = score;
				bestColumn = j;
			}
		}
	}
	// assert(bestColumn != -1);  // TODO
	return bestColumn;
}


// 搜索最佳的落子列
int UCT::search(double inTime)
{
	chessBoard->saveBoard();
	int cnt = 0;
	try {
		while (timeNow() - inTime < TIME_LIM) {
			for (int i = 1; i <= WATCH_INTERVAL; ++i) {
				int s = treePolicy(nowRoot);
				int delta = defaultPolicy(s);
				updateUp(s, delta);
				chessBoard->loadBoard();
			}
		}
	}
	catch (int e) {
		;
	}
	int bestColumn = calcBestColumn(nowRoot, debugOn);
	return bestColumn;
}

// 判断 s 结点是否可以向一个新儿子扩展
// 可以则返回选择的列，否则返回 -1
// TODO: 可考虑有策略地优先选某个新儿子，例如加入计分策略，先扩展分数高的
// TODO(done): 或者对选择的列加入记忆化，减小常数
int UCT::findExpandSon(int s) {
	for (int& j = node[s].lastSon; j < n; ++j) {
		if (node[s].son[j] == 0 && chessBoard->top[j] > 0) {
			++j;
			return j-1;
		}
	}
	return -1;
}

// 扩展，从结点 s （对应chessBoard）开始走 col 列
// 返回新扩展的结点
int UCT::expand(int s, int col, int& row) {
	int t = newNode();
	node[t].parent = s;
	node[t].parColumn = col;
	node[s].son[col] = t;
	row = chessBoard->move(col);
	node[t].status = chessBoard->getStatus();
	if (node[t].status == 1 || node[t].status == 2) {
		node[t].certLose = true;
		node[t].certStep = 0;
	}
	return t;
}

// 从 s 结点，向下找到最可能扩展的结点并进行扩展，返回新扩展出的结点
// 若直至必胜/败态仍未找到，则返回必胜/败态结点
int UCT::treePolicy(int s) {
	while (!(node[s].certLose || node[s].certWin)) {
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
int UCT::defaultPolicy(int s) {
	int turn = 3 - chessBoard->turn;  // DEBUG: 注意应从**落子进入 s 状态者**角度考虑，这样每次尽量往更大的方向走才是正确的
	int status;
	while ((status = chessBoard->getStatus()) == 0) {
		int col;
		do {
			col = rand() % n;  // DEBUG: 这里应该是 n !
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
// 特殊处理 t 为必胜/败态的情况
double UCT::calcScore(int s, int t) {
	if (node[t].certWin) {
		return -SCORE_INF + node[t].certStep;  // 越迟输越好
	}
	else if (node[t].certLose) {
		return SCORE_INF - node[t].certStep;   // 越早赢越好
	}
	else return (double)node[t].win / node[t].tot + alpha * sqrt(2 * log(node[s].tot) / node[t].tot);
}


// 向上更新信息
// 注意需要更新必胜/败状态
void UCT::updateUp(int s, int delta)
{
	int last = -1;
	do {
		node[s].tot++;
		node[s].win += delta;

		// 更新必胜/败状态
		if (last != -1) {
			if (node[last].certLose) {
				// 必胜态让 certStep 尽量小
				if (!node[s].certWin)  node[s].certStep = node[last].certStep + 1;
				else node[s].certStep = std::min(node[s].certStep, node[last].certStep + 1);
				node[s].certWin = true;
			}
			else if (node[last].certWin && node[s].expandOver) {
				// 确认是否所有后继都为 certWin，且必败态让 certStop 尽量大
				bool fail = false;
				int tmpStep = 0;
				for (int j = 0; j < n; ++j) {
					int son = node[s].son[j];
					if (son) {
						if (!node[son].certWin) {
							fail = true;
							break;
						}
						else {
							tmpStep = std::max(tmpStep, node[son].certStep);
						}
					}
				}
				if (!fail) {
					node[s].certLose = true;
					node[s].certStep = tmpStep;
				}
			}
		}

		last = s;
		s = node[s].parent;
		delta = 1 - delta;
	} while (last != nowRoot);
}


// 获得一个新节点。TODO (done)：垃圾回收和满内存判定
int UCT::newNode() {
	if (poolPtr < NODE_MAX - 1) {
		++poolPtr;
		node[poolPtr].init();
		return poolPtr;
	}
	else if (trash.size() > 0) {
		int s = trash.top();
		trash.pop();
		for (int j = 0; j < n; ++j) {
			if (node[s].son[j]) {
				trash.push(node[s].son[j]);
			}
		}
		node[s].init();
		return s;
	}
	else {
		throw 0;
	}
}