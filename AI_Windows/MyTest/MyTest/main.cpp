#include <cstdio>
#include <iostream>
#include "Strategy.h"
#include "Point.h"
#include "ChessBoard.h"

const int M = 9, N = 9;
int top[N];
int board[M * N];
int noX = 3, noY = 3;

int main() {
	srand(100);
	for (int j = 0; j < N; ++j) top[j] = M;
	Point *p = getPoint(M, N, top, board, -1, -1, noX, noY);
	printf("%d %d\n", p->x, p->y);
	return 0;
}