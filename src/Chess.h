#pragma once

#include <vector>
#include <string>


class Chess
{
public:
	enum {
		MAXR = 20,		//�����������
		MAXC = 20,		//�����������
		PLAYER = 2,		//��������
		MOVELINE = 10,	//��Ҫȥ���ıߵ��������
		errValue = -1	//��Ч���ʶ
	};

	const char* ChessManOne = "��";
	const char* ChessManTow = "��";


	int dr[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };// ��8����������ʵ��flat����
	int dc[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };// ���Ͽ�ʼ��˳ʱ��ת�������Ͻ���


	int row = 1, col = 1;				// ��������һ��row�У����1-row��һ��col�У����1-col
	int board[MAXR + 2][MAXC + 2];	//���̵�ÿ����㣬=1��ʾһ�������ӣ�=2��ʾ����һ��������,0��ʾ�գ�-1��ʾ��Ч����
	int boardStyle;				//������ʽ��=0��ʾֻ�к��ߺ����ߵ����̣�=1��ʾ���˺����������ֵ����̣�=2��ʾ���˺��������ֻ���Ҫȥ����
	int isMi[MAXR + 2][MAXC + 2];	//ÿ������ǲ���������,��boardStyle=1��2ʱ�д˲���
	int removeLineNum;			//��Ҫȥ���ıߵ�����������boardStyle=2ʱ�д˲���
	int removeLines[MOVELINE][4];	//��Ҫȥ���ıߵ���������
	int chessman;				//������ʽ
	int displayContent;				//������ʾ���ݣ�=0��ʾ��ʾ������������,=1��ʾ����ʾ
	int turn;					//�ֵ�˭��,=1,=2
	int howToChangeTurn;			//��λ����巽,=0��ʾ�����ֻ���=1��ʾ˫��������˳�������=2��ʾ˫���ֻ������һ�ο��Բٿض������
	int turnOptNum[PLAYER + 1];	//˫��һ�ο��Բٿ����ӵĸ�����howToChangeTurn=2ʱ�д˲��� 
	int playMod;					//��ʾ����ķ�ʽ��=0��ʾ���ӣ�=1��ʾ���ӣ�=2��ʾ����
	int placeMod;				//��ʾ���ӵľ������=0��ʾ�����ӣ�=1��ʾ�г�
	int placeOkMod;					//��ʾ�ܷ����ӵľ������,=0��ʾ�пո񼴿ɣ�=1��ʾ�пո���һ�Ҫ���Ӹı���������
	int moveMod;				//��ʾ�ƶ����ӵľ������=0��ʾ�Ƶ����ڴ���=1��ʾ��Ծ�����ӣ�=2��ʾ�ϻ���
	int moveOkMod;					//��ʾ�ܷ��ƶ����ӵľ������=0��ʾ�Ƶ����ڴ���=1��ʾ��Ծ�����ӣ�=2��ʾ�ϻ���
	int ifEndMod;				//��ʾ�Ƿ�����������=1��ʾһ�������¾ͻ���һ��,˫���������¾ͽ���,=2��ʾ�����壬=3��ʾ�ϻ���
	int endNum;						//��ʾʣ��������Ӿ��и�����ifEndMod=3ʱ�д˲���

	// �Ƿ��г�ʼ����
	bool haveInitChessman = false;
	
	bool isEnd;//��Ϸ�Ƿ����
	std::vector<std::vector<int>> manual;//	const std::string& filepath����
	int temp[MAXR + 2][MAXC + 2]; //board�ı���
public:

	Chess();
	Chess(const std::string& filepath);
	~Chess();
	
	

	int GetChessNum(int key);

};

