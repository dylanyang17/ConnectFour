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

int main() {
	srand(100);

	//
	int** board = new int* [M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}
	//

	for (int j = 0; j < N; ++j) top[j] = M;
	ChessBoard chessBoard(M, N, lastX, lastY, noX, noY, board, top, 1);
	// 用户先手
	while (1) {
		if (chessBoard.turn == 1) {
			printf("轮到玩家下子....\n");
			int y;
			scanf("%d", &y);
			chessBoard.move(y);
			chessBoard.print();
		}
		else if (chessBoard.turn == 2) {
			printf("轮到电脑下子....\n");
			for (int j = 0; j < N; ++j) top[j] = chessBoard.top[j];
			Point* p = getPoint(M, N, top, _board, chessBoard.lastX, chessBoard.lastY, noX, noY);
			printf("%d %d\n", p->x, p->y);
		}
	}
	return 0;
}