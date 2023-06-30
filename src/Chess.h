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
				if (playMod == 0)//落子
					std::cout << "玩家" << tmp->turn << "落点(" << tmp->x << ", " << tmp->y << ")" << std::endl;
				else if (playMod == 1)
					if (i % 2 == 0)
						std::cout << "玩家" << tmp->turn << "将点(" << tmp->x << ", " << tmp->y << ")";
					else
						std::cout << "移至" << "(" << tmp->x << ", " << tmp->y << ")" << std::endl;
		}
	}manual;//棋谱

	//     3 2 1 
	//      \|/ 
	//   4--- ---0
	//      /|\
  	//     5 6 7
	// 8向指示,从右开始，逆时针转，到右上结束
	int dr[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
	int dc[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

	enum {
		MAXR = 20,		// 棋盘最大行数
		MAXC = 20,		// 棋盘最大列数
		PLAYER = 2,		// 下棋人数
		MOVELINE = 10,	// 需要去掉的边的最大数量
		errValue = -1	// 无效点标识
	};

	struct {
		int type = -1;		//=-1无效点，=0空棋盘，=1玩家一，=2玩家二
		bool isMi = false;	//该点是否为米字点
		glm::vec3 position;	//该点在窗口上的坐标
	}board[MAXR + 2][MAXC + 2];	// 棋盘的每个格点


	int row = 1, col = 1;	// 棋盘共row行，col列，从1开始到row(col)	
	int turn = 1;			// 轮到谁下,=1,=2

	
	int howToChangeTurn = 0;// 如何换下棋方,=0各一次，=1单人棋局，=2双方轮换但一次可以操控多个棋子
	
	int turnOptNum[PLAYER + 1] = { 0, 1, 1 };// howToChangeTurn=2时双方所能操纵的棋子数量，turnOptNum[1(2)]表示玩家一(二)

	int playMod = 0;// 行棋方式，=0落子，=1移子，=2其他

	// playMod为0时
	int placeMod = 0;	// 落子的具体规则，=0仅落子，=1可以夹吃
	int placeOkMod = 0;	// 能否落子的具体规则，=0有空即可，=1有空且必须改变其它棋子

	// playMod为1时
	int moveMod = 0;// 移子的具体规则，=0移到相邻处，=1跳跃或吃子，=2玩家一移子, 玩家二移子或跳吃，=3若玩家二落点周围只有一个玩家一棋子，则吃掉
	int moveOkMod = 0;	// 能否移子的具体规则，=0移到相邻空位，=1移子或跳子都可，=2玩家一移子，玩家二移子或跳子，=3移动到任意空地，=4只能跳子
	
	int ifEndMod = 1;	// 结束具体规则，=0不可落子就结束，=1表示一方不可下就换另一方,双方都不能下就结束,=2表示数子连珠，=3表示老虎棋，=4独立砖石

	int series = 0;			// ifEndMod=2时所需棋子数量
	bool seriesMod = false;	// 默认=false表示连珠直线斜线都可，=true表示只能斜线
	
	int endNum = 0;		// ifEndMod=3时所剩棋子数量
	
	bool isEnd = false;// 游戏是否结束
	
	int winner = 0;
public:

	bool PlayPlace(int x, int y);
	bool PlayMove(int x, int y, int x2, int y2);
	
	int GetChessNum(int key);
	int ChessmanInLine(int x, int y);

private:
	
	bool IsErrorPosition(int x, int y);
	
	bool PlaceOk(int x, int y);// 是否可以落子
	void Place(int x, int y);// 落子

	bool CanMove(int x, int y, int x2, int y2, int number);
	bool MoveOk(int x, int y, int x2, int y2);// 是否可以移子

	void MoveChess(int x, int y, int x2, int y2);
	void Move(int x, int y, int x2, int y2);// 移子

	bool NonePlace();
	bool IsEnd();

	void ChangeTurn();
};

