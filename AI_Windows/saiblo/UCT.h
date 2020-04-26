#ifndef UCT_H_
#define UCT_H_
#include "Point.h"
#include "ChessBoard.h"
#include <cstring>
#include <stack>


class UCT {
public:

	UCT();

	UCT(int m, int n, ChessBoard* chessBoard);

	void init(int m, int n, ChessBoard* chessBoard);

	int realMove(int col);

	int calcBestColumn(int s, bool debug=false);

	// 局面结点，连边仅需要存储落子的列
	// 注意使用数组索引模拟指针，索引为 0 的结点被定义为 NULL
	struct Node {
		bool expandOver;  // 记忆化标记，是否可以确认不能再扩展
		int status;       // 结点状态。0: 未结束; 1: 对方胜利; 2: 己方胜利; 3: 平局
		int son[12];
		int parent;
		int parColumn;    // 从父亲沿哪条边走过来
		// bestColumn 表示按照公式的最优下一步走哪一列，son[bestColumn] 即对应结点，初始为 -1
		// 而 bestColumnScore 对应这个公式最大值
		// int bestColumn;
		double bestColumnScore;
		// tot 存放模拟总次数，win 存放模拟中胜利次数
		int tot, win;

		void init() {
			// TODO: 确保全都赋予了初值
			expandOver = false;
			status = 0;
			std::memset(son, 0, sizeof(son));
			parent = 0;
			parColumn = -1;
			// bestColumn = -1;
			tot = win = 0;
		}

		Node() {
			init();
		}
	};

	int search();

	int findExpandSon(int s);

	int expand(int s, int col, int &row);

	int treePolicy(int s);

	int defaultPolicy(int s);

	double calcScore(int s, int t);

	void updateUp(int s, int delta);

	int newNode();


private:
	// TODO: TIME_LIM 和 NODE_MAX 均需要进行调整（上调）
	const double TIME_LIM = 2.65;
	static const int NODE_MAX = 8000000;  
	// TODO: 参数 alpha，表示对探索较少方向的倾向程度
	const double alpha = 0.3;

	const int WATCH_INTERVAL = 1;  // 每隔 1 次模拟看一次时间

	int m, n;

	// 结点相关
	// TODO: 增加垃圾回收和内存不足判定（此时选择尽量优的即可，不再继续模拟）
	// 垃圾回收可使用 vector 存储需要回收的根节点，使用懒惰回收方法，回收一个结点后再将其子结点放入 vector
	Node node[NODE_MAX];
	int poolPtr;      // node 池指针
	int nowRoot;      // 当前真实所处结点，作为根节点
	bool debugOn = false;
	std::stack<int> trash;
	
	ChessBoard* chessBoard;  // 整体上维持为当前局面（计算时可能临时改变一段时间）
};


#endif