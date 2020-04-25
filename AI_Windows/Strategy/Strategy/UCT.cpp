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
// �����򷵻�������ӽ������������򷵻� -1
// TODO: �ɿ����в��Ե�����ѡĳ���¶��ӣ��������Ʒֲ��ԣ�����չ�����ߵ�
int UCT::findExpandSon(int s) {
	for (int j = 0; j < n; ++j) {

	}
}

// �� s ��㣬�����ҵ��������չ�Ľ�㲢������չ����������չ���Ľ��
// ��ֱ�����������δ�ҵ��򷵻ؽ���������
int UCT::treePolicy(int s) {
	chessBoard->saveBoard();
	while (!node[s].isEnd) {
		int t = -1;
		if (!node[s].expandOver) {
			// �ж��Ƿ����չ
			t = findExpandSon(s);
			if (t == -1) node[s].expandOver = true;
		}
		if (!node[s].expandOver) {
			// ����չ
		}
		else {
			// ������չ
		}
	}
	chessBoard->loadBoard();
}


// ���ϸ�����Ϣ
void UCT::updateUp()
{

}



// ���һ���½ڵ㡣TODO���������պ����ڴ��ж�
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
