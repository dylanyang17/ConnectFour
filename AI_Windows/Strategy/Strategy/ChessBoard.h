#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

class ChessBoard
{
public:
	int* const* board;

	ChessBoard() = delete;

	ChessBoard(int m, int n, int noX, int noY, int* const* board);

	void saveBoard();

	void loadBoard();

private:
	int m, n, noX, noY;
	int tmpBoard[15][15];
};

#endif