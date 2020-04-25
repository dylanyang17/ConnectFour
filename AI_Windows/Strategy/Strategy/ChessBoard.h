#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

class ChessBoard
{
public:
	static const int MAXN = 15;
	int* const* board;
	int top[MAXN];

	ChessBoard() = delete;

	ChessBoard(int m, int n, int noX, int noY, int* const* board, int *top);

	void saveBoard();

	void loadBoard();

private:
	int m, n, noX, noY;
	int tmpBoard[MAXN][MAXN];
	int tmpTop[MAXN];
};

#endif