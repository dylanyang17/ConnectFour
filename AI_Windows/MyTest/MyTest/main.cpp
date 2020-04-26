#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "Strategy.h"
#include "Point.h"
#include "ChessBoard.h"

const int M = 9, N = 9;
int top[N];
int _board[M * N];
int lastX = -1, lastY = -1;
int noX = 2, noY = 2;
int turn = 1;

void loadSample1(int **board) {
	// 计算机只需要走 3 则立马胜利
	board[M-1][0] = board[M-1][1] = board[M-1][2] = 2;
	// top[0] = top[1] = top[2] = M - 1;
}

void loadSample2(int** board) {
	//
	//    0 1 2 3 4 5 6 7 8
	//	0 0 0 0 0 0 0 0 0 0
	//	1 0 0 0 0 0 0 0 0 0
	//	2 0 0 0 0 0 0 0 0 0
	//	3 0 0 0 X 0 0 0 0 0
	//	4 0 0 0 0 0 0 0 0 0
	//	5 0 0 0 0 2 2 0 0 0
	//	6 0 0 0 0 1 2 0 0 0
	//	7 0 0 0 1 2 1 0 0 0
	//	8 0 0 2 1 1 2 1 0 0
	board[M - 1][6] = board[M - 2][5] = board[M - 3][4] = 1;
	board[M - 1][5] = board[M - 2][4] = 2;
	board[M - 1][4] = 1;
	board[M - 1][3] = board[M - 2][3] = 1;
}

/*
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 2 0 0 0 0 0
0 0 0 0 0 0 1 1 2 0 0 0
0 2 0 0 0 1 2 1 2 0 0 0
0 2 0 0 1 2 1 1 9 0 0 0
2
*/

void load(int** board) {
	printf("请输入局面...\n");
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			scanf_s("%d", &board[i][j]);
			if (board[i][j] == 9) {
				board[i][j] = 0;
				noX = i, noY = j;
			}
		}
	}
	printf("先手：");
	scanf_s("%d", &turn);
}

int main() {
	srand(1000);

	// init
	int** board = new int* [M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}
	for (int j = 0; j < N; ++j) top[j] = M;

	// load Sample
	// load(board);
	// loadSample2(board);

	// update _board and top
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j]) top[j] = std::min(top[j], i);
			else if (top[j] < i) {
				printf("Wrong Sample!!!!!!!! %d %d %d %d\n", i, j, top[j], board[i][j]);
				return 0;
			}
		}
	}
	top[noY] = std::min(top[noY], noX);
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			_board[i * N + j] = board[i][j];
		}
	}

	// run
	ChessBoard chessBoard;
	chessBoard.init(M, N, lastX, lastY, noX, noY, board, top, turn);
	printf("初始局面：\n");
	chessBoard.print();
	// 用户先手
	int status;
	while ((status = chessBoard.getStatus()) == 0) {
		if (chessBoard.turn == 1) {
			printf("轮到玩家下子....\n");
			int y;
			scanf_s("%d", &y);
			int x = chessBoard.move(y);
			printf("%d %d\n", x, y);
			chessBoard.print();
		}
		else if (chessBoard.turn == 2) {
			printf("轮到电脑下子....\n");
			Point* p = getPoint(M, N, chessBoard.top, _board, chessBoard.lastX, chessBoard.lastY, noX, noY);
			printf("%d %d\n", p->x, p->y);
			chessBoard.move(p->y);
			chessBoard.print();
		}
	}
	printf("\n%s!\n", ((status == 1) ? "玩家获胜" : ((status == 2) ? "电脑获胜" : "平局")));
	return 0;
}