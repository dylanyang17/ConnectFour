#ifndef UCT_H_
#define UCT_H_
#include "Point.h"
#include <cstring>


class UCT {
public:
	UCT() = delete;

	UCT(int m, int n, int noX, int noY);

	// �����㣬���߽���Ҫ�洢���ӵ���
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

	// TODO: ������Ҫ���� NODE_MAX
	static const int NODE_MAX = 1000000;  
	Node node[NODE_MAX];
	int poolPtr = 0;  // node ��ָ��
	// TODO: �����������պ��ڴ治���ж�����ʱѡ�����ŵļ��ɣ����ټ���ģ�⣩
};


#endif