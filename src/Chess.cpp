#include "Chess.h"
#include <iostream>


bool Chess::IsErrorPosition(int x, int y)
{
	if (x<1 || x>row)
		return true;
	if (y<1 || y>col)
		return true;

	return board[x][y].type == errValue;
}

int Chess::GetChessNum(int key)
{
	int sum = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			if (board[i][j].type == key)
				sum++;

	return sum;
}

int Chess::ChessmanInLine(int x, int y)
{
	if (board[x][y].type == -1 || board[x][y].type == 0)
		return 0;

	int sum = 0;
	if(seriesMod ==0)
	{

		for (int i = 0; i < 4; i++)
		{
			int lineSum = 0;
			// 判断i方向上有几个连续的相同棋子
			glm::vec2 tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i]][(int)tempPos.y + dc[i]].type)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				lineSum++;
			}
			// 判断i的反方向(i+4)上有几个连续的相同棋子
			tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i + 4]][(int)tempPos.y + dc[i + 4]].type)
			{
				tempPos.x += dr[i + 4];
				tempPos.y += dc[i + 4];
				lineSum++;
			}
			if (lineSum > sum)
				sum = lineSum;
		}
	}
	else if (seriesMod == 1)
	{
		for (int i = 1; i <= 3; i += 2)
		{
			int lineSum = 0;
			// 判断i方向上有几个连续的相同棋子
			glm::vec2 tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i]][(int)tempPos.y + dc[i]].type)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				lineSum++;
			}
			// 判断i的反方向(i+4)上有几个连续的相同棋子
			tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i + 4]][(int)tempPos.y + dc[i + 4]].type)
			{
				tempPos.x += dr[i + 4];
				tempPos.y += dc[i + 4];
				lineSum ++;
			}
			if (lineSum > sum)
				sum = lineSum;
		}
	}

	return sum + 1;
}


bool Chess::PlaceOk(int x, int y)
{

	if (IsErrorPosition(x, y))
		return false;
	if (board[x][y].type != 0)
		return false;

	if (placeOkMod == 0)// 仅落子
	{
		return true;
	}
	else if (placeOkMod == 1)// 必须夹吃
	{
		for (int i = 0; i < 8; i++)
		{
			glm::vec2 tempPos = glm::vec2(x, y);
			int num = 0;
			while (true)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				// 夹吃棋子数量

				//进行下一方向 ------------- 非棋盘点位 ------------    ------------------- 空点位 -------------------
				if (board[(int)tempPos.x][(int)tempPos.y].type == -1 || board[(int)tempPos.x][(int)tempPos.y].type == 0)
					break;
				// 如果是他方棋子则继续此方向
				else if (board[(int)tempPos.x][(int)tempPos.y].type == 3 - turn)
				{
					num++;
					continue;
				}
				// 如果是己方棋子
				else if (board[(int)tempPos.x][(int)tempPos.y].type == turn)
				{
					if (num == 0)// 如果相邻则进行下一方向
						break;
					else//   如果不相邻(num != 0)则夹吃成立
						return true;

				}
			}

		}
		return false;
	}

	std::cout << "错误:placeOkMod 错误" << std::endl;
	return false;
}

void Chess::Place(int x, int y)
{
	if (placeMod == 0)// 仅落子
	{
		board[x][y].type = turn;
	}
	else if (placeMod == 1)// 夹吃
	{
		board[x][y].type = turn;

		for (int i = 0; i < 8; i++)
		{
			glm::vec2 target = glm::vec2((float)x, (float)y);
			while (true)
			{
				target.x += dr[i];
				target.y += dc[i];
				// 如果是错误点位
				if (board[(int)target.x][(int)target.y].type == -1)
					break;
				// 如果是空位
				if (board[(int)target.x][(int)target.y].type == 0)
					break;
				// 如果是另一方棋子
				if (board[(int)target.x][(int)target.y].type == 3 - turn)
					continue;
				// 如果是己方棋子
				if (board[(int)target.x][(int)target.y].type == turn)
				{
					glm::vec2 passby = glm::vec2(x, y);
					while (true)
					{
						passby.x += dr[i];
						passby.y += dc[i];
						if (passby == target)
							break;
						board[(int)passby.x][(int)passby.y].type = turn;
					}
				}
			}
		}
	}
}


// (x,y)和(x2,y2)8向相隔number个(number为1时表示相邻)时返回真
bool Chess::CanMove(int x, int y, int x2, int y2, int number)
{
	if (number == 1 || number == 2)
	{
		if (abs(x - x2) == number && y == y2)
			return true;
		if (x == x2 && abs(y - y2) == number)
			return true;
		if (abs(x - x2) == number && abs(y - y2) == number && (board[x][y].isMi || board[x2][y2].isMi))
			return true;
		return false;
	}
	else
	{
		std::cout << "函数CanMove错误，错误的\"number\"\n";
		return false;
	}

}

bool Chess::MoveOk(int x, int y, int x2, int y2)
{
	if (board[x][y].type == -1|| board[x2][y2].type == -1)
	{
		std::cout << "错误的坐标\n";
		return false;
	}
	// 单人棋局可以不用管是谁的棋子，但双人棋局只能操纵自己的棋子
	if (howToChangeTurn != 1 && board[x][y].type != turn)
	{
		std::cout << "错误的棋子\n";
		return false;
	}
	//落点必须是空位
	if (board[x2][y2].type != 0)
		return false;
	switch (moveOkMod)
	{
	case 0://移到相邻空位
		return CanMove(x, y, x2, y2, 1);
	case 1://移子或跳子都可
		return CanMove(x, y, x2, y2, 1) || CanMove(x, y, x2, y2, 2);
	case 2://玩家一移子，玩家二移子或跳子
		if (turn == 1)
			return CanMove(x, y, x2, y2, 1);
		if (turn == 2)
			return CanMove(x, y, x2, y2, 1) || CanMove(x, y, x2, y2, 2);
		std::cout << "函数MoveOk错误的turn" << std::endl;
		return false;
	case 3://可以移动到任意空地
		return true;
	case 4://只能跳子
		return CanMove(x, y, x2, y2, 2);
	default:
		std::cout << "函数MoveOk错误的moveOkMod" << std::endl;
		return false;
	}

}

void Chess::MoveChess(int x, int y, int x2, int y2)
{
	board[x2][y2].type = board[x][y].type;
	board[x][y].type = 0;
}

void Chess::Move(int x, int y, int x2, int y2)
{
	switch (moveMod)
	{
	case 0://移子
		MoveChess(x, y, x2, y2);
		break;
	case 1://移子或跳吃
		MoveChess(x, y, x2, y2);
		if (abs(x - x2) == 2 || abs(y - y2) == 2)
			board[(x + x2) / 2][(y + y2) / 2].type = 0;
		break;
	case 2://玩家一移子, 玩家二移子或跳吃
		MoveChess(x, y, x2, y2);
		if (turn == 1)//如果是玩家一则结束
			break;
		// 如果是玩家二，并且是跳子
		if (turn == 2)
		{
			if (abs(x - x2) == 2 || abs(y - y2) == 2)
				board[(x + x2) / 2][(y + y2) / 2].type = 0;
			break;
		}
		std::cout << "Move函数,turn参数错误." << std::endl;
	case 3://若玩家二落点周围只有一个玩家一棋子，则吃掉
		MoveChess(x, y, x2, y2);
		if (turn == 1)
			break;
		if (turn == 2)
		{
			glm::vec2 change = glm::vec2(0.0f, 0.0f);
			int number = 0;
			for (int i = 0; i < 8; i++)
			{
				// -上下左右方向-  --- 自己为米字格 ---   ------- 所指方向为米字格 ---------
				if (i % 2 == 0 || (board[x2][y2].isMi || board[x2 + dr[i]][y2 + dc[i]].isMi))
				{
					if (board[x2 + dr[i]][y2 + dc[i]].type == 3 - board[x2][y2].type)//如果是他方棋子
					{
						change.x = (float)x2 + (float)dr[i];
						change.y = (float)y2 + (float)dc[i];
						number++;
					}

				}
			}
			if (number == 1)//如果周围只有一个棋子
				board[(int)change.x][(int)change.y].type = 0;
			break;
		}

	default:
		std::cout << "函数Move.错误的MoveOkMod." << std::endl;
	}
}


bool Chess::NonePlace()
{
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			if (PlaceOk(i, j))
				return false;
	return true;
}

bool Chess::IsEnd()
{
	switch (ifEndMod)
	{
	case 0://不可落子就结束
		if (NonePlace())
			winner = turn;
		return isEnd = NonePlace();
	case 1://一方不可落子就换另一方,双方都不能落则结束
		isEnd = NonePlace();

		if (!isEnd)
			return false;
		ChangeTurn();
		return isEnd = NonePlace();
	case 2://数子连珠
		if (NonePlace())
			return isEnd = true;

		for (int i = 1; i <= row; i++)
			for (int j = 1; j <= col; j++)
				if (ChessmanInLine(i, j) >= series)
				{
					winner = turn;
					return isEnd = true;
				}
		return false;
	case 3:// 老虎棋
		if (turn == 1)
		{
			isEnd = (GetChessNum(1) <= endNum);
			if (isEnd)
				winner = 2;
		}
		else// 各个棋子还能否移动,(从(i,j)到(i2, j2))
			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++)
					for (int i2 = 1; i2 <= row; i2++)
						for (int j2 = 1; j2 <= col; j2++)
							if (MoveOk(i, j, i2, j2))
								return isEnd = false;
		winner = 1;
		return isEnd = true;
	case 4://独立钻石
		if (GetChessNum(1) + GetChessNum(2) <= 1)
			return isEnd = true;
		else // 各个棋子还能否移动,(从(i,j)到(i2, j2))
			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++)
					for (int i2 = 1; i2 <= row; i2++)
						for (int j2 = 1; j2 <= col; j2++)
							if (MoveOk(i, j, i2, j2))
								return isEnd = false;
		return isEnd = true;
	default:
		std::cout << "函数IfEnd.错误的IfEndMod." << std::endl;
		return false;
	}
}

void Chess::ChangeTurn()
{
	if (howToChangeTurn == 0 || howToChangeTurn == 1)//正常轮换
	{
		turn = 3 - turn; // turn = 1就设置2,turn = 2就设置1
	}
	else if (howToChangeTurn == 2)//下设定数量后轮换
	{
		static int optNum = 0;
		optNum++;
		if (optNum == turnOptNum[turn])
		{
			turn = 3 - turn;
			optNum = 0;
		}
	}

}

bool Chess::PlayPlace(int x, int y)
{
	if (x == 0 && y == 0)
	{
		manual.Show(playMod);
		return true;
	}
	if (PlaceOk(x, y))
	{
		Place(x, y);
		manual.Insert(x, y, turn);
		IsEnd();
		ChangeTurn();
		return true;
	}
	return false;
}

bool Chess::PlayMove(int x, int y, int x2, int y2)
{
	if (x == 0 && y == 0)
	{
		manual.Show(playMod);
		return true;
	}
	if (MoveOk(x, y, x2, y2))
	{
		Move(x, y, x2, y2);
		ChangeTurn();
		manual.Insert(x, y, turn);
		manual.Insert(x2, y2, turn);
		IsEnd();
		return true;
	}
	return false;
}