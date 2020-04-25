#ifndef UCT_H_
#define UCT_H_
#include "Point.h"
#include "ChessBoard.h"
#include <cstring>


class UCT {
public:
	UCT() = delete;

	UCT(int m, int n, int noX, int noY, ChessBoard* chessBoard);

	// 局面结点，连边仅需要存储落子的列
	// 注意使用数组索引模拟指针，索引为 0 的结点被定义为 NULL
	struct Node {
		bool isEnd;       // 是否为终止结点
		bool expandOver;  // 记忆化标记，是否可以确认不能再扩展
		int son[12];
		int parent;
		// bestColumn 表示按照公式的最优下一步走哪一列，son[bestColumn] 即对应结点，初始为 -1
		int bestColumn;

		void init() {
			// TODO: 确保全都赋予了初值
			std::memset(son, 0, sizeof(son));
			parent = 0;
			isEnd = expandOver = false;
			bestColumn = -1;
		}

		Node() {
			init();
		}
	};

	Point search(int s);

	int treePolicy(int s);

	void updateUp();

	int newNode(bool& suc);


private:
	// TODO: 常量均需要进行调整（上调）
	const double TIME_LIM = 2;
	static const int NODE_MAX = 1000000;  

	int m, n, noX, noY;

	// 结点相关
	// TODO: 增加垃圾回收和内存不足判定（此时选择尽量优的即可，不再继续模拟）
	Node node[NODE_MAX];
	int poolPtr = 0;  // node 池指针
	
	ChessBoard* chessBoard;  // 整体上维持为当前局面（计算时可能临时改变一段时间）
};


#endif