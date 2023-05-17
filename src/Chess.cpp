#include "Chess.h"
#include <iostream>

Chess::Chess()
:row(0), col(0)
{
	for (int i = 0; i <= MAXR + 1; i++)
		for (int j = 0; j <= MAXC + 1; j++)
		{
			board[i][j] = errValue;
			isMi[i][j] = 0;
		}



}

Chess::Chess(const std::string& filepath)
{
	
	
	// 初始化棋盘
	for (int i = 0; i <= MAXR + 1; i++)
		for (int j = 0; j <= MAXC + 1; j++)
		{
			board[i][j] = errValue;
			isMi[i][j] = 0;
		}
	std::cout << "输入棋盘格点" << std::endl;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
		{
			std::cin>>board[i][j];
		}
	std::cout << "输入棋盘样式:";
	std::cout << "=0表示只有横线和竖线的棋盘，=1表示除了横竖还有米字的棋盘，=2表示除了横竖和米字还需要去掉边" << std::endl;
	std::cout << "棋盘样式为:";
	std::cin >> boardStyle;
	if (boardStyle == 1 || boardStyle == 2)
	{
		for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		{
			std::cin>>isMi[i][j];
		}
	}
	if (boardStyle == 2)
	{
		std::cin>>removeLineNum;
		for (int i = 0; i < removeLineNum; i++) 
		{
			for (int j = 0; j < 4; j++)
				std::cin>>removeLines[i][j];
		}
	}
	std::cout << "输入chessman和displayContent:";
	std::cin >> chessman >> displayContent;
	std::cout << "输入turn:";
	std::cin >> turn;
	std::cout << "输入howToChangeTurn:";
	std::cin >> howToChangeTurn;
	if (howToChangeTurn == 2) {
		std::cin>>turnOptNum[1] >> turnOptNum[2];
	}
	std::cout << "输入playMod(0表示落子，1表示移子，2表示其他):";
	std::cin >> playMod;
	if (playMod == 0)
	{
		std::cout << "输入placeMod, placeOkMod:";
		std::cin >> placeMod >> placeOkMod;
	}
	else if (playMod == 1)
	{
		std::cout << "输入moveMod, moveOkMod:";
		std::cin >> moveMod >> moveOkMod;
	}
	std::cout << "输入ifEndMod:";
	std::cin >> ifEndMod;
	if (ifEndMod == 3)
		std::cin >> endNum;
}

int Chess::GetChessNum(int key)
{
	int ans = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			if(board[i][j] == key)
				ans++;
			
	return ans;
}

Chess::~Chess()
{
}


