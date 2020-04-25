#include "ChessBoard.h"

ChessBoard::ChessBoard(int m, int n, int noX, int noY, int* const* board)
{
	this->m = m;
	this->n = n;
	this->noX = noX;
	this->noY = noY;
	this->board = board;
}


// ��������� tmpBoard
void ChessBoard::saveBoard() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			tmpBoard[i][j] = board[i][j];
		}
	}
}

// ������� tmpBoard ȡ��
void ChessBoard::loadBoard() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = tmpBoard[i][j];
		}
	}
}