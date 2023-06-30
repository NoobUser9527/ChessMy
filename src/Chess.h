#pragma once

#include <iostream>
#include <string>

#include "glm/glm.hpp"


class Chess
{
public:
	struct Manual {
	private:
		int x;
		int y;
		int turn;
		Manual* next = nullptr;
	public:
		Manual() :x(0), y(0), turn(0), next(nullptr) {};
		Manual(int x, int y, int turn) :x(x), y(y), turn(turn) {};
		~Manual()
		{
			Manual* temp;
			
			while(next != nullptr)
			{
				temp = next;
				next = next->next;
				free(temp);
			}
		};

		void Insert(int x, int y, int turn)
		{
			Manual* temp = new Manual(x, y, turn);
			if (next == nullptr)
				next = temp;
			else
			{
				Manual* tail = next;
				for (; tail->next != nullptr; tail = tail->next);
				tail->next = temp;
			}
		}

		void Show(int playMod)
		{
			Manual* tmp = next;
			for (int i = 0; tmp != nullptr; tmp = tmp->next,i++)
				if (playMod == 0)//����
					std::cout << "���" << tmp->turn << "���(" << tmp->x << ", " << tmp->y << ")" << std::endl;
				else if (playMod == 1)
					if (i % 2 == 0)
						std::cout << "���" << tmp->turn << "����(" << tmp->x << ", " << tmp->y << ")";
					else
						std::cout << "����" << "(" << tmp->x << ", " << tmp->y << ")" << std::endl;
		}
	}manual;//����

	//     3 2 1 
	//      \|/ 
	//   4--- ---0
	//      /|\
  	//     5 6 7
	// 8��ָʾ,���ҿ�ʼ����ʱ��ת�������Ͻ���
	int dr[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
	int dc[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

	enum {
		MAXR = 20,		// �����������
		MAXC = 20,		// �����������
		PLAYER = 2,		// ��������
		MOVELINE = 10,	// ��Ҫȥ���ıߵ��������
		errValue = -1	// ��Ч���ʶ
	};

	struct {
		int type = -1;		//=-1��Ч�㣬=0�����̣�=1���һ��=2��Ҷ�
		bool isMi = false;	//�õ��Ƿ�Ϊ���ֵ�
		glm::vec3 position;	//�õ��ڴ����ϵ�����
	}board[MAXR + 2][MAXC + 2];	// ���̵�ÿ�����


	int row = 1, col = 1;	// ���̹�row�У�col�У���1��ʼ��row(col)	
	int turn = 1;			// �ֵ�˭��,=1,=2

	
	int howToChangeTurn = 0;// ��λ����巽,=0��һ�Σ�=1������֣�=2˫���ֻ���һ�ο��Բٿض������
	
	int turnOptNum[PLAYER + 1] = { 0, 1, 1 };// howToChangeTurn=2ʱ˫�����ܲ��ݵ�����������turnOptNum[1(2)]��ʾ���һ(��)

	int playMod = 0;// ���巽ʽ��=0���ӣ�=1���ӣ�=2����

	// playModΪ0ʱ
	int placeMod = 0;	// ���ӵľ������=0�����ӣ�=1���Լг�
	int placeOkMod = 0;	// �ܷ����ӵľ������=0�пռ��ɣ�=1�п��ұ���ı���������

	// playModΪ1ʱ
	int moveMod = 0;// ���ӵľ������=0�Ƶ����ڴ���=1��Ծ����ӣ�=2���һ����, ��Ҷ����ӻ����ԣ�=3����Ҷ������Χֻ��һ�����һ���ӣ���Ե�
	int moveOkMod = 0;	// �ܷ����ӵľ������=0�Ƶ����ڿ�λ��=1���ӻ����Ӷ��ɣ�=2���һ���ӣ���Ҷ����ӻ����ӣ�=3�ƶ�������յأ�=4ֻ������
	
	int ifEndMod = 1;	// �����������=0�������Ӿͽ�����=1��ʾһ�������¾ͻ���һ��,˫���������¾ͽ���,=2��ʾ�������飬=3��ʾ�ϻ��壬=4����שʯ

	int series = 0;			// ifEndMod=2ʱ������������
	bool seriesMod = false;	// Ĭ��=false��ʾ����ֱ��б�߶��ɣ�=true��ʾֻ��б��
	
	int endNum = 0;		// ifEndMod=3ʱ��ʣ��������
	
	bool isEnd = false;// ��Ϸ�Ƿ����
	
	int winner = 0;
public:

	bool PlayPlace(int x, int y);
	bool PlayMove(int x, int y, int x2, int y2);
	
	int GetChessNum(int key);
	int ChessmanInLine(int x, int y);

private:
	
	bool IsErrorPosition(int x, int y);
	
	bool PlaceOk(int x, int y);// �Ƿ��������
	void Place(int x, int y);// ����

	bool CanMove(int x, int y, int x2, int y2, int number);
	bool MoveOk(int x, int y, int x2, int y2);// �Ƿ��������

	void MoveChess(int x, int y, int x2, int y2);
	void Move(int x, int y, int x2, int y2);// ����

	bool NonePlace();
	bool IsEnd();

	void ChangeTurn();
};

