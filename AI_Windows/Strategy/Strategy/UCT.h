#ifndef UCT_H_
#define UCT_H_
#include "Point.h"
#include <cstring>


class UCT {
public:
	UCT() = delete;

	UCT(int m, int n, int noX, int noY);

	// 局面结点，连边仅需要存储落子的列
	struct Node {
		Node* son[12];
		Node* parent;

		void init() {
			std::memset(son, 0, sizeof(son));
			parent = NULL;
		}

		Node() {
			init();
		}
	};

	Point search(int s, int* const* board);

	void updateUp();

	int clearNode(int s);

	int newNode(bool& suc);



private:

	int m, n, noX, noY;

	// TODO: 可能需要增大 NODE_MAX
	static const int NODE_MAX = 1000000;  
	Node node[NODE_MAX];
	int poolPtr = 0;  // node 池指针
	// TODO: 增加垃圾回收和内存不足判定（此时选择尽量优的即可，不再继续模拟）
};


#endif