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

// �������ӵ�
Point UCT::search(int s, int* const* board)
{
	double inTime = timeNow();
	// TODO: ��С���� clock() �Ĵ���������ÿ x ��ģ�����һ��
	while (timeNow() - inTime < TIME_LIM) {

	}
	board[1][2] = 5;
	return Point(0, 0);
}

// �� s ��㣨��Ӧ����Ϊ board����ʼ�������ҵ��������չ�Ľ�㲢������չ����������չ���Ľ��
// ��ֱ�����������δ�ҵ��򷵻ؽ���������
// TODO: �ɿ����в��Ե�ѡ�㣬�������Ʒֲ��ԣ�����չ�����ߵ�
int UCT::treePolicy(int s, int* const* board) {
	saveBoard(board);
	while (!node[s].isEnd) {
		for (int j = 0; j < n; ++j) {
			if (board[0][j])
		}
	}
	loadBoard(board);
}


// ���ϸ�����Ϣ
void UCT::updateUp()
{

}

// ��������� tmpBoard
void UCT::saveBoard(int* const* board) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			tmpBoard[i][j] = board[i][j];
		}
	}
}

// ������� tmpBoard ȡ��
void UCT::loadBoard(int* const* board) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = tmpBoard[i][j];
		}
	}
}

// ���һ���½ڵ㡣TODO���������պ����ڴ��ж�
int UCT::newNode(bool &suc) {
	suc = true;
	node[poolPtr].init();
	return poolPtr++;
}
