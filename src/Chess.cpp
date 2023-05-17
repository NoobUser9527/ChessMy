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
	
	
	// ��ʼ������
	for (int i = 0; i <= MAXR + 1; i++)
		for (int j = 0; j <= MAXC + 1; j++)
		{
			board[i][j] = errValue;
			isMi[i][j] = 0;
		}
	std::cout << "�������̸��" << std::endl;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
		{
			std::cin>>board[i][j];
		}
	std::cout << "����������ʽ:";
	std::cout << "=0��ʾֻ�к��ߺ����ߵ����̣�=1��ʾ���˺����������ֵ����̣�=2��ʾ���˺��������ֻ���Ҫȥ����" << std::endl;
	std::cout << "������ʽΪ:";
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
	std::cout << "����chessman��displayContent:";
	std::cin >> chessman >> displayContent;
	std::cout << "����turn:";
	std::cin >> turn;
	std::cout << "����howToChangeTurn:";
	std::cin >> howToChangeTurn;
	if (howToChangeTurn == 2) {
		std::cin>>turnOptNum[1] >> turnOptNum[2];
	}
	std::cout << "����playMod(0��ʾ���ӣ�1��ʾ���ӣ�2��ʾ����):";
	std::cin >> playMod;
	if (playMod == 0)
	{
		std::cout << "����placeMod, placeOkMod:";
		std::cin >> placeMod >> placeOkMod;
	}
	else if (playMod == 1)
	{
		std::cout << "����moveMod, moveOkMod:";
		std::cin >> moveMod >> moveOkMod;
	}
	std::cout << "����ifEndMod:";
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


