#ifndef UCT_H_
#define UCT_H_
#include "Point.h"
#include "ChessBoard.h"
#include <cstring>


class UCT {
public:
	UCT() = delete;

	UCT(int m, int n, int noX, int noY, ChessBoard* chessBoard);

	// �����㣬���߽���Ҫ�洢���ӵ���
	// ע��ʹ����������ģ��ָ�룬����Ϊ 0 �Ľ�㱻����Ϊ NULL
	struct Node {
		bool isEnd;       // �Ƿ�Ϊ��ֹ���
		bool expandOver;  // ���仯��ǣ��Ƿ����ȷ�ϲ�������չ
		int son[12];
		int parent;
		// bestColumn ��ʾ���չ�ʽ��������һ������һ�У�son[bestColumn] ����Ӧ��㣬��ʼΪ -1
		int bestColumn;

		void init() {
			// TODO: ȷ��ȫ�������˳�ֵ
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
	// TODO: ��������Ҫ���е������ϵ���
	const double TIME_LIM = 2;
	static const int NODE_MAX = 1000000;  

	int m, n, noX, noY;

	// ������
	// TODO: �����������պ��ڴ治���ж�����ʱѡ�����ŵļ��ɣ����ټ���ģ�⣩
	Node node[NODE_MAX];
	int poolPtr = 0;  // node ��ָ��
	
	ChessBoard* chessBoard;  // ������ά��Ϊ��ǰ���棨����ʱ������ʱ�ı�һ��ʱ�䣩
};


#endif