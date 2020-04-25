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

// ��������ʵ�����ӣ��������ӵ�����
// TODO: ��������
int UCT::realMove(int col) {
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
				fprintf(stderr, "��%d: %d, %d, %f)\n", j, node[son].win, node[son].tot, score);
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


// ������ѵ�������
int UCT::search()
{
	double inTime = timeNow();
	chessBoard->saveBoard();
	// TODO: ��С���� clock() �Ĵ���������ÿ x ��ģ�����һ��
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

// �ж� s ����Ƿ������һ���¶�����չ
// �����򷵻�ѡ����У����򷵻� -1
// TODO: �ɿ����в��Ե�����ѡĳ���¶��ӣ��������Ʒֲ��ԣ�����չ�����ߵ�
// TODO: ���߶�ѡ����м�����仯����С����
int UCT::findExpandSon(int s) {
	for (int j = 0; j < n; ++j) {
		if (node[s].son[j] == 0 && chessBoard->top[j] > 0) {
			return j;
		}
	}
	return -1;
}

// ��չ���ӽ�� s ����ӦchessBoard����ʼ�� col ��
// ��������չ�Ľ��
int UCT::expand(int s, int col, int &row) {
	int t = newNode();
	node[t].parent = s;
	node[t].parColumn = col;
	node[s].son[col] = t;
	row = chessBoard->move(col);
	node[t].status = chessBoard->getStatus();
	return t;
}

// �� s ��㣬�����ҵ��������չ�Ľ�㲢������չ����������չ���Ľ��
// ��ֱ�����������δ�ҵ��򷵻ؽ���������
int UCT::treePolicy(int s) {
	while (node[s].status == 0) {
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
// TODO: ���Լ���Ʒֲ��ԣ�˫����һ����������
int UCT::defaultPolicy(int s) {
	int turn = 3 - chessBoard->turn;  // DEBUG: ע��Ӧ��**���ӽ��� s ״̬��**�Ƕȿ��ǣ�����ÿ�ξ���������ķ����߲�����ȷ��
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


// ���� s Ϊ����ʱ������ t ��Ӧ�ĵ÷�
double UCT::calcScore(int s, int t) {
	return (double)node[t].win / node[t].tot + alpha * sqrt(2 * log(node[s].tot) / node[t].tot);
}


// ���ϸ�����Ϣ
void UCT::updateUp(int s, int delta)
{
	int last = -1;
	do {
		node[s].tot++;
		node[s].win += delta;
		//if (last != -1) {  // TODO: ���Գ���ʹ�� bestColumn ���Ҹ��ļƷ���̬�ԸĽ��㷨
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


// ���һ���½ڵ㡣TODO���������պ����ڴ��ж�
int UCT::newNode() {
	++poolPtr;
	node[poolPtr].init();
	return poolPtr;
}
