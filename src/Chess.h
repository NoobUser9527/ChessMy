#pragma once

#include <vector>
#include <string>


class Chess
{
public:
	enum {
		MAXR = 20,		//棋盘最大行数
		MAXC = 20,		//棋盘最大列数
		PLAYER = 2,		//下棋人数
		MOVELINE = 10,	//需要去掉的边的最大数量
		errValue = -1	//无效点标识
	};

	const char* ChessManOne = "●";
	const char* ChessManTow = "▲";


	int dr[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };// 用8个方向向量实现flat技术
	int dc[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };// 从上开始，顺时针转，到左上结束


	int row = 1, col = 1;				// 整个棋盘一共row行，编号1-row，一共col列，编号1-col
	int board[MAXR + 2][MAXC + 2];	//棋盘的每个格点，=1表示一方的棋子，=2表示另外一方的棋子,0表示空，-1表示无效格子
	int boardStyle;				//棋盘样式，=0表示只有横线和竖线的棋盘，=1表示除了横竖还有米字的棋盘，=2表示除了横竖和米字还需要去掉边
	int isMi[MAXR + 2][MAXC + 2];	//每个格点是不是米字型,当boardStyle=1或2时有此参数
	int removeLineNum;			//需要去掉的边的数量，当当boardStyle=2时有此参数
	int removeLines[MOVELINE][4];	//需要去掉的边的两点坐标
	int chessman;				//棋子样式
	int displayContent;				//额外显示内容，=0表示显示各方棋子数量,=1表示不显示
	int turn;					//轮到谁下,=1,=2
	int howToChangeTurn;			//如何换下棋方,=0表示正常轮换，=1表示双方可任意顺序操作，=2表示双方轮换但玩家一次可以操控多个棋子
	int turnOptNum[PLAYER + 1];	//双方一次可以操控棋子的个数，howToChangeTurn=2时有此参数 
	int playMod;					//表示行棋的方式，=0表示落子，=1表示移子，=2表示其他
	int placeMod;				//表示落子的具体规则，=0表示仅落子，=1表示夹吃
	int placeOkMod;					//表示能否落子的具体规则,=0表示有空格即可，=1表示有空格而且还要落子改变其他棋子
	int moveMod;				//表示移动棋子的具体规则，=0表示移到相邻处，=1表示跳跃并吃子，=2表示老虎棋
	int moveOkMod;					//表示能否移动棋子的具体规则，=0表示移到相邻处，=1表示跳跃并吃子，=2表示老虎棋
	int ifEndMod;				//表示是否结束具体规则，=1表示一方不可下就换另一方,双方都不能下就结束,=2表示五子棋，=3表示老虎棋
	int endNum;						//表示剩余多少棋子就判负，当ifEndMod=3时有此参数

	// 是否有初始棋子
	bool haveInitChessman = false;
	
	bool isEnd;//游戏是否结束
	std::vector<std::vector<int>> manual;//	const std::string& filepath棋谱
	int temp[MAXR + 2][MAXC + 2]; //board的备份
public:

	Chess();
	Chess(const std::string& filepath);
	~Chess();
	
	

	int GetChessNum(int key);

};

