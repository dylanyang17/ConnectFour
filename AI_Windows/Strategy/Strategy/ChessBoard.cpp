#include "ChessBoard.h"
#include "Judge.h"
#include <cassert>

ChessBoard::ChessBoard(int m, int n, int noX, int noY, int* const* board, int* top)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
	this->board = board;
	for (int j = 0; j < n; ++j)
		this->top[j] = top[j];
	turn = 2;  // 注意初始化时一定为己方回合
}


// 在第 col 列走子，保证合法，返回走的位置的 x 值
int ChessBoard::move(int col) {
	int ret = --top[col];
	board[ret][col] = turn;
	// 保证 top[col]-1 为可走子处或 0
	if (col == noY && top[col] - 1 == noX)
		top[col]--;
	turn = 3 - turn;
	return ret;
}


// 检查局面的胜负状态
// 0: 未结束; 1: 对方胜利; 2: 己方胜利; 3: 平局
int ChessBoard::checkStatus(int lastX, int lastY) {
	if (board[lastX][lastY] == 1) {
		// 只可能是 0, 1, 3
		if (userWin(lastX, lastY, m, n, board)) {
			return 1;
		}
		else if (isTie(n, top)) {
			return 3;
		}
		else {
			return 0;
		}
	}
	else if (board[lastX][lastY] == 2) {
		// 只可能是 0, 2, 3
		if (machineWin(lastX, lastY, m, n, board)) {
			return 2;
		}
		else if (isTie(n, top)) {
			return 3;
		}
		else {
			return 0;
		}
	}
	else {
		assert(0); // TODO
	}
}


// 将局面存入 tmpBoard
void ChessBoard::saveBoard() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			tmpBoard[i][j] = board[i][j];
		}
	}
	for (int j = 0; j < n; ++j) {
		tmpTop[j] = top[j];
	}
	tmpTurn = turn;
}

// 将局面从 tmpBoard 取出
void ChessBoard::loadBoard() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = tmpBoard[i][j];
		}
	}
	for (int j = 0; j < n; ++j) {
		top[j] = tmpTop[j];
	}
	turn = tmpTurn;
}