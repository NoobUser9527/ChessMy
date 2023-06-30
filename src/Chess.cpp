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
			// �ж�i�������м�����������ͬ����
			glm::vec2 tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i]][(int)tempPos.y + dc[i]].type)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				lineSum++;
			}
			// �ж�i�ķ�����(i+4)���м�����������ͬ����
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
			// �ж�i�������м�����������ͬ����
			glm::vec2 tempPos = glm::vec2(x, y);
			while (board[x][y].type == board[(int)tempPos.x + dr[i]][(int)tempPos.y + dc[i]].type)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				lineSum++;
			}
			// �ж�i�ķ�����(i+4)���м�����������ͬ����
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

	if (placeOkMod == 0)// ������
	{
		return true;
	}
	else if (placeOkMod == 1)// ����г�
	{
		for (int i = 0; i < 8; i++)
		{
			glm::vec2 tempPos = glm::vec2(x, y);
			int num = 0;
			while (true)
			{
				tempPos.x += dr[i];
				tempPos.y += dc[i];
				// �г���������

				//������һ���� ------------- �����̵�λ ------------    ------------------- �յ�λ -------------------
				if (board[(int)tempPos.x][(int)tempPos.y].type == -1 || board[(int)tempPos.x][(int)tempPos.y].type == 0)
					break;
				// �������������������˷���
				else if (board[(int)tempPos.x][(int)tempPos.y].type == 3 - turn)
				{
					num++;
					continue;
				}
				// ����Ǽ�������
				else if (board[(int)tempPos.x][(int)tempPos.y].type == turn)
				{
					if (num == 0)// ��������������һ����
						break;
					else//   ���������(num != 0)��гԳ���
						return true;

				}
			}

		}
		return false;
	}

	std::cout << "����:placeOkMod ����" << std::endl;
	return false;
}

void Chess::Place(int x, int y)
{
	if (placeMod == 0)// ������
	{
		board[x][y].type = turn;
	}
	else if (placeMod == 1)// �г�
	{
		board[x][y].type = turn;

		for (int i = 0; i < 8; i++)
		{
			glm::vec2 target = glm::vec2((float)x, (float)y);
			while (true)
			{
				target.x += dr[i];
				target.y += dc[i];
				// ����Ǵ����λ
				if (board[(int)target.x][(int)target.y].type == -1)
					break;
				// ����ǿ�λ
				if (board[(int)target.x][(int)target.y].type == 0)
					break;
				// �������һ������
				if (board[(int)target.x][(int)target.y].type == 3 - turn)
					continue;
				// ����Ǽ�������
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


// (x,y)��(x2,y2)8�����number��(numberΪ1ʱ��ʾ����)ʱ������
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
		std::cout << "����CanMove���󣬴����\"number\"\n";
		return false;
	}

}

bool Chess::MoveOk(int x, int y, int x2, int y2)
{
	if (board[x][y].type == -1|| board[x2][y2].type == -1)
	{
		std::cout << "���������\n";
		return false;
	}
	// ������ֿ��Բ��ù���˭�����ӣ���˫�����ֻ�ܲ����Լ�������
	if (howToChangeTurn != 1 && board[x][y].type != turn)
	{
		std::cout << "���������\n";
		return false;
	}
	//�������ǿ�λ
	if (board[x2][y2].type != 0)
		return false;
	switch (moveOkMod)
	{
	case 0://�Ƶ����ڿ�λ
		return CanMove(x, y, x2, y2, 1);
	case 1://���ӻ����Ӷ���
		return CanMove(x, y, x2, y2, 1) || CanMove(x, y, x2, y2, 2);
	case 2://���һ���ӣ���Ҷ����ӻ�����
		if (turn == 1)
			return CanMove(x, y, x2, y2, 1);
		if (turn == 2)
			return CanMove(x, y, x2, y2, 1) || CanMove(x, y, x2, y2, 2);
		std::cout << "����MoveOk�����turn" << std::endl;
		return false;
	case 3://�����ƶ�������յ�
		return true;
	case 4://ֻ������
		return CanMove(x, y, x2, y2, 2);
	default:
		std::cout << "����MoveOk�����moveOkMod" << std::endl;
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
	case 0://����
		MoveChess(x, y, x2, y2);
		break;
	case 1://���ӻ�����
		MoveChess(x, y, x2, y2);
		if (abs(x - x2) == 2 || abs(y - y2) == 2)
			board[(x + x2) / 2][(y + y2) / 2].type = 0;
		break;
	case 2://���һ����, ��Ҷ����ӻ�����
		MoveChess(x, y, x2, y2);
		if (turn == 1)//��������һ�����
			break;
		// �������Ҷ�������������
		if (turn == 2)
		{
			if (abs(x - x2) == 2 || abs(y - y2) == 2)
				board[(x + x2) / 2][(y + y2) / 2].type = 0;
			break;
		}
		std::cout << "Move����,turn��������." << std::endl;
	case 3://����Ҷ������Χֻ��һ�����һ���ӣ���Ե�
		MoveChess(x, y, x2, y2);
		if (turn == 1)
			break;
		if (turn == 2)
		{
			glm::vec2 change = glm::vec2(0.0f, 0.0f);
			int number = 0;
			for (int i = 0; i < 8; i++)
			{
				// -�������ҷ���-  --- �Լ�Ϊ���ָ� ---   ------- ��ָ����Ϊ���ָ� ---------
				if (i % 2 == 0 || (board[x2][y2].isMi || board[x2 + dr[i]][y2 + dc[i]].isMi))
				{
					if (board[x2 + dr[i]][y2 + dc[i]].type == 3 - board[x2][y2].type)//�������������
					{
						change.x = (float)x2 + (float)dr[i];
						change.y = (float)y2 + (float)dc[i];
						number++;
					}

				}
			}
			if (number == 1)//�����Χֻ��һ������
				board[(int)change.x][(int)change.y].type = 0;
			break;
		}

	default:
		std::cout << "����Move.�����MoveOkMod." << std::endl;
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
	case 0://�������Ӿͽ���
		if (NonePlace())
			winner = turn;
		return isEnd = NonePlace();
	case 1://һ���������Ӿͻ���һ��,˫���������������
		isEnd = NonePlace();

		if (!isEnd)
			return false;
		ChangeTurn();
		return isEnd = NonePlace();
	case 2://��������
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
	case 3:// �ϻ���
		if (turn == 1)
		{
			isEnd = (GetChessNum(1) <= endNum);
			if (isEnd)
				winner = 2;
		}
		else// �������ӻ��ܷ��ƶ�,(��(i,j)��(i2, j2))
			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++)
					for (int i2 = 1; i2 <= row; i2++)
						for (int j2 = 1; j2 <= col; j2++)
							if (MoveOk(i, j, i2, j2))
								return isEnd = false;
		winner = 1;
		return isEnd = true;
	case 4://������ʯ
		if (GetChessNum(1) + GetChessNum(2) <= 1)
			return isEnd = true;
		else // �������ӻ��ܷ��ƶ�,(��(i,j)��(i2, j2))
			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++)
					for (int i2 = 1; i2 <= row; i2++)
						for (int j2 = 1; j2 <= col; j2++)
							if (MoveOk(i, j, i2, j2))
								return isEnd = false;
		return isEnd = true;
	default:
		std::cout << "����IfEnd.�����IfEndMod." << std::endl;
		return false;
	}
}

void Chess::ChangeTurn()
{
	if (howToChangeTurn == 0 || howToChangeTurn == 1)//�����ֻ�
	{
		turn = 3 - turn; // turn = 1������2,turn = 2������1
	}
	else if (howToChangeTurn == 2)//���趨�������ֻ�
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