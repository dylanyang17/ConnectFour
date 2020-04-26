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

	// �����㣬���߽���Ҫ�洢���ӵ���
	// ע��ʹ����������ģ��ָ�룬����Ϊ 0 �Ľ�㱻����Ϊ NULL
	struct Node {
		bool expandOver;  // ���仯��ǣ��Ƿ����ȷ�ϲ�������չ
		int status;       // ���״̬��0: δ����; 1: �Է�ʤ��; 2: ����ʤ��; 3: ƽ��
		int son[12];
		int parent;
		int parColumn;    // �Ӹ������������߹���
		// bestColumn ��ʾ���չ�ʽ��������һ������һ�У�son[bestColumn] ����Ӧ��㣬��ʼΪ -1
		// �� bestColumnScore ��Ӧ�����ʽ���ֵ
		// int bestColumn;
		double bestColumnScore;
		// tot ���ģ���ܴ�����win ���ģ����ʤ������
		int tot, win;

		void init() {
			// TODO: ȷ��ȫ�������˳�ֵ
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
	// TODO: TIME_LIM �� NODE_MAX ����Ҫ���е������ϵ���
	const double TIME_LIM = 2.65;
	static const int NODE_MAX = 8000000;  
	// TODO: ���� alpha����ʾ��̽�����ٷ��������̶�
	const double alpha = 0.3;

	const int WATCH_INTERVAL = 1;  // ÿ�� 1 ��ģ�⿴һ��ʱ��

	int m, n;

	// ������
	// TODO: �����������պ��ڴ治���ж�����ʱѡ�����ŵļ��ɣ����ټ���ģ�⣩
	// �������տ�ʹ�� vector �洢��Ҫ���յĸ��ڵ㣬ʹ��������շ���������һ�������ٽ����ӽ����� vector
	Node node[NODE_MAX];
	int poolPtr;      // node ��ָ��
	int nowRoot;      // ��ǰ��ʵ������㣬��Ϊ���ڵ�
	bool debugOn = false;
	std::stack<int> trash;
	
	ChessBoard* chessBoard;  // ������ά��Ϊ��ǰ���棨����ʱ������ʱ�ı�һ��ʱ�䣩
};


#endif