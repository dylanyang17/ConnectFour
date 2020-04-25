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
	node[s].son[col] = t;
	int row = chessBoard->move(col);
	node[t].status = chessBoard->getStatus(row, col);
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
			s = node[s].son[node[s].bestColumn];
		}
	}
	return s;
}


// �ӽ�� s ��ʼ���ж���ģ�⣬ֱ���ֳ�ʤ������������ֵ��ʤ: 1; ��: 0; ƽ: 0.5����ֻ�������ϲ�����Ϊ��
// TODO: ���Լ���Ʒֲ��ԣ�˫����һ����������
int defaultPolicy(int s) {

}


// ���ϸ�����Ϣ
void UCT::updateUp()
{

}


// ���һ���½ڵ㡣TODO���������պ����ڴ��ж�
int UCT::newNode() {
	++poolPtr;
	node[poolPtr].init();
	return poolPtr;
}
