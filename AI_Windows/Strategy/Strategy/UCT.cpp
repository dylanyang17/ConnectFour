#include "UCT.h"
#include "Judge.h"
#include <ctime>
#include <random>
#include <cassert>
#include <cmath>
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

// �������ӵ�
Point UCT::search()
{
	double inTime = timeNow();
	chessBoard->saveBoard();
	// TODO: ��С���� clock() �Ĵ���������ÿ x ��ģ�����һ��
	while (timeNow() - inTime < TIME_LIM) {
		int s = treePolicy(nowRoot);
		int delta = defaultPolicy(s);
		updateUp(s, delta);
		chessBoard->loadBoard();
	}
	return Point(0, 0);
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
int UCT::expand(int s, int col) {
	int t = newNode();
	node[t].parent = s;
	node[t].parColumn = col;
	node[s].son[col] = t;
	chessBoard->move(col);
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
			int t = newNode();
			node[t].parent = s;
			node[s].son[col] = t;
			chessBoard->move(col);
			s = t;
			break;
		}
		else {
			// ������չ���������ŵĶ��ӽ����
			assert(node[s].bestColumn != -1);  // TODO
			chessBoard->move(node[s].bestColumn);
			s = node[s].son[node[s].bestColumn];
		}
	}
	return s;
}


// �ӽ�� s ��ʼ���ж���ģ�⣬ֱ���ֳ�ʤ�������� s �������ֵ������ s ״̬�����߽Ƕȿ��ǣ�ʤ: 1; ��: 0; ƽ: 0.5��
// TODO: ���Լ���Ʒֲ��ԣ�˫����һ����������
int UCT::defaultPolicy(int s) {
	int turn = chessBoard->turn;
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
		if (last != -1) {
			double score = calcScore(s, last);
			if (node[s].bestColumnScore)
		}
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
