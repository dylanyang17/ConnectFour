#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

class ChessBoard
{
public:
	static const int MAXN = 15;
	int* const* board;
	int top[MAXN];
	int turn;  // 1 表示对手回合，2 表示自己回合

	ChessBoard() = delete;

	ChessBoard(int m, int n, int lastX, int lastY, int noX, int noY, int* const* board, int *top);

	int move(int col);

	int getStatus();

	void saveBoard();

	void loadBoard();

private:
	int m, n, lastX, lastY, noX, noY;
	int tmpBoard[MAXN][MAXN];
	int tmpTop[MAXN];
	int tmpTurn;
	int tmpLastX, tmpLastY;
};

#endif