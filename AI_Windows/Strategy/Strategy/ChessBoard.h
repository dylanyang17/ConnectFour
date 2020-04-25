#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

class ChessBoard
{
public:
	int lastX, lastY;
	static const int MAXN = 15;
	int* const* board;
	int top[MAXN];
	int turn;  // 1 ��ʾ���ֻغϣ�2 ��ʾ�Լ��غ�

	ChessBoard() = delete;

	ChessBoard(int m, int n, int lastX, int lastY, int noX, int noY, int* const* board, const int *top, int turn);

	void print();

	int move(int col);

	int getStatus();

	void saveBoard();

	void loadBoard();

private:
	int m, n, noX, noY;
	int tmpBoard[MAXN][MAXN];
	int tmpTop[MAXN];
	int tmpTurn;
	int tmpLastX, tmpLastY;
};

#endif