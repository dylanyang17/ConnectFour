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

// �������ӵ�
Point UCT::search(int s)
{
	double inTime = timeNow();
	// TODO: ��С���� clock() �Ĵ���������ÿ x ��ģ�����һ��
	while (timeNow() - inTime < TIME_LIM) {

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

// �� s ��㣬�����ҵ��������չ�Ľ�㲢������չ����������չ���Ľ��
// ��ֱ�����������δ�ҵ��򷵻ؽ���������
int UCT::treePolicy(int s) {
	chessBoard->saveBoard();
	while (!node[s].isEnd) {
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
			s = node[s].son[node[s].bestColumn];
		}
	}
	chessBoard->loadBoard();
	return s;
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
