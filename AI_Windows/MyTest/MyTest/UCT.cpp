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

// �ؿ�һ�����
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

// ��������ʵ�����ӣ��������ӵ���������������������
int UCT::realMove(int col) {
	for (int j = 0; j < n; ++j) {
		if (j != col && node[nowRoot].son[j]) {
			trash.push(node[nowRoot].son[j]);
		}
	}
	if (node[nowRoot].son[col] == 0) {
		// �ö��ӽ�㲢δ��չ
		int row;
		nowRoot = expand(nowRoot, col, row);
		return row;
	}
	else {
		nowRoot = node[nowRoot].son[col];
		return chessBoard->move(col);
	}
}


// TODO: ���Լ�¼ bestColumn����΢�޸Ĺ�ʽ
int UCT::calcBestColumn(int s, bool debug) {
	double bestScore;
	int bestColumn = -1;
	for (int j = 0; j < n; ++j) {
		int son = node[s].son[j];
		if (son) {
			double score = calcScore(s, node[s].son[j]);
			if (debug) {
				fprintf(stderr, "��%d: %d, %d, %s, %d, %f)\n", j, node[son].win, node[son].tot, (node[son].certWin ? "certWin" : (node[son].certLose ? "certLose" : "none")), node[son].certStep, score);
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


// ������ѵ�������
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

// �ж� s ����Ƿ������һ���¶�����չ
// �����򷵻�ѡ����У����򷵻� -1
// TODO: �ɿ����в��Ե�����ѡĳ���¶��ӣ��������Ʒֲ��ԣ�����չ�����ߵ�
// TODO(done): ���߶�ѡ����м�����仯����С����
int UCT::findExpandSon(int s) {
	for (int& j = node[s].lastSon; j < n; ++j) {
		if (node[s].son[j] == 0 && chessBoard->top[j] > 0) {
			++j;
			return j-1;
		}
	}
	return -1;
}

// ��չ���ӽ�� s ����ӦchessBoard����ʼ�� col ��
// ��������չ�Ľ��
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

// �� s ��㣬�����ҵ��������չ�Ľ�㲢������չ����������չ���Ľ��
// ��ֱ����ʤ/��̬��δ�ҵ����򷵻ر�ʤ/��̬���
int UCT::treePolicy(int s) {
	while (!(node[s].certLose || node[s].certWin)) {
		int col = -1;
		if (!node[s].expandOver) {
			// �ж��Ƿ����չ���������ҵ����õ��� col
			col = findExpandSon(s);
			if (col == -1) node[s].expandOver = true;
		}
		if (!node[s].expandOver) {
			// ����չ
			assert(col != -1);  // TODO
			int row;
			s = expand(s, col, row);
			break;
		}
		else {
			// ������չ���������ŵĶ��ӽ����
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


// �ӽ�� s ��ʼ���ж���ģ�⣬ֱ���ֳ�ʤ�������� s �������ֵ������**���� s ״̬��**�Ƕȿ��ǣ�ʤ: 1; ��: 0; ƽ: 0.5��
int UCT::defaultPolicy(int s) {
	int turn = 3 - chessBoard->turn;  // DEBUG: ע��Ӧ��**���ӽ��� s ״̬��**�Ƕȿ��ǣ�����ÿ�ξ���������ķ����߲�����ȷ��
	int status;
	while ((status = chessBoard->getStatus()) == 0) {
		int col;
		do {
			col = rand() % n;  // DEBUG: ����Ӧ���� n !
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


// ���� s Ϊ����ʱ������ t ��Ӧ�ĵ÷�
// ���⴦�� t Ϊ��ʤ/��̬�����
double UCT::calcScore(int s, int t) {
	if (node[t].certWin) {
		return -SCORE_INF + node[t].certStep;  // Խ����Խ��
	}
	else if (node[t].certLose) {
		return SCORE_INF - node[t].certStep;   // Խ��ӮԽ��
	}
	else return (double)node[t].win / node[t].tot + alpha * sqrt(2 * log(node[s].tot) / node[t].tot);
}


// ���ϸ�����Ϣ
// ע����Ҫ���±�ʤ/��״̬
void UCT::updateUp(int s, int delta)
{
	int last = -1;
	do {
		node[s].tot++;
		node[s].win += delta;

		// ���±�ʤ/��״̬
		if (last != -1) {
			if (node[last].certLose) {
				// ��ʤ̬�� certStep ����С
				if (!node[s].certWin)  node[s].certStep = node[last].certStep + 1;
				else node[s].certStep = std::min(node[s].certStep, node[last].certStep + 1);
				node[s].certWin = true;
			}
			else if (node[last].certWin && node[s].expandOver) {
				// ȷ���Ƿ����к�̶�Ϊ certWin���ұذ�̬�� certStop ������
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


// ���һ���½ڵ㡣TODO (done)���������պ����ڴ��ж�
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