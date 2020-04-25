#include <cstdio>
#include <iostream>
#include "Strategy.h"
#include "Point.h"
#include "ChessBoard.h"

const int M = 9, N = 9;
int top[N];
int _board[M * N];
int lastX = -1, lastY = -1;
int noX = 3, noY = 3;
int turn = 2;

void loadSample1(int **board) {
	// �����ֻ��Ҫ�� 3 ������ʤ��
	board[M-1][0] = board[M-1][1] = board[M-1][2] = 2;
	top[0] = top[1] = top[2] = M - 1;
	
}

int main() {
	srand(1000);

	//
	int** board = new int* [M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}
	for (int j = 0; j < N; ++j) top[j] = M;
	//

	// load Sample
	loadSample1(board);
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			_board[i * N + j] = board[i][j];
		}
	}
	//


	ChessBoard chessBoard(M, N, lastX, lastY, noX, noY, board, top, turn);
	printf("��ʼ���棺\n");
	chessBoard.print();
	// �û�����
	int status;
	while ((status = chessBoard.getStatus()) == 0) {
		if (chessBoard.turn == 1) {
			printf("�ֵ��������....\n");
			int y;
			scanf_s("%d", &y);
			int x = chessBoard.move(y);
			printf("%d %d\n", x, y);
			chessBoard.print();
		}
		else if (chessBoard.turn == 2) {
			printf("�ֵ���������....\n");
			Point* p = getPoint(M, N, chessBoard.top, _board, chessBoard.lastX, chessBoard.lastY, noX, noY);
			printf("%d %d\n", p->x, p->y);
			chessBoard.move(p->y);
			chessBoard.print();
		}
	}
	printf("\n%s!\n", ((status == 1) ? "��һ�ʤ" : ((status == 2) ? "���Ի�ʤ" : "ƽ��")));
	return 0;
}