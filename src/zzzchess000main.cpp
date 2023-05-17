#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<algorithm>
#include <fstream>



using namespace std;

#define MAXR 20 //�����������
#define MAXC 20 //�����������
#define PLAYER 2 //��������
#define MOVELINE 10 //��Ҫȥ���ıߵ��������

#define cinInt(x) while(!(cin>>x)){cin.clear();cin.ignore();}
#define cinInt2(x,y) cinInt(x)cinInt(y)

typedef void(*func_void)();
typedef void(*func_void2)(int, int);
typedef void(*func_void4)(int, int, int, int);
typedef string(*func_string1)(int);
typedef string(*func_string2)(int, int);
typedef bool(*func_bool)();
typedef bool(*func_bool2)(int, int);
typedef bool(*func_bool4)(int, int, int, int);

const char* dataInTxt = "D:\\init.txt";//��ű������õ��ļ�
const int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };//��8����������ʵ��flat����
const int dc[] = { 0, 1, 1, 1, 0, -1, -1, -1 };//���Ͽ�ʼ��˳ʱ��ת�������Ͻ���
const int errValue = -1;//��Ч�㶼��Ϊ-1

//begin �������ݣ����������Ӳ���
int row, col;//��������һ��row�У����1-row��һ��col�У����1-col
int board[MAXR + 2][MAXC + 2];//���̵�ÿ����㣬=1��ʾһ�������ӣ�=2��ʾ����һ��������,0��ʾ�գ�-1��ʾ��Ч����
int boardStyle;//������ʽ��=0��ʾֻ�к��ߺ����ߵ����̣�=1��ʾ���˺����������ֵ����̣�=2��ʾ���˺��������ֻ���Ҫȥ����
int isMi[MAXR + 2][MAXC + 2];//ÿ������ǲ���������,��boardStyle=1��2ʱ�д˲���
int removeLineNum; //��Ҫȥ���ıߵ�����������boardStyle=2ʱ�д˲���
int removeLines[MOVELINE][4]; //��Ҫȥ���ıߵ���������
int chessman;//������ʽ
int displayContent;//������ʾ���ݣ�=0��ʾ��ʾ������������,=1��ʾ����ʾ
int turn;//�ֵ�˭��,=1,=2
int howToChangeTurn;//��λ����巽,=0��ʾ�����ֻ���=1��ʾ˫��������˳�������=2��ʾ˫���ֻ������һ�ο��Բٿض������
int turnOptNum[PLAYER + 1];//˫��һ�ο��Բٿ����ӵĸ�����howToChangeTurn=2ʱ�д˲��� 
int playMod;//��ʾ����ķ�ʽ��=0��ʾ���ӣ�=1��ʾ���ӣ�=2��ʾ����
int placeMod;//��ʾ���ӵľ������=0��ʾ�����ӣ�=1��ʾ�г�
int placeOkMod;//��ʾ�ܷ����ӵľ������,=0��ʾ�пո񼴿ɣ�=1��ʾ�пո���һ�Ҫ���Ӹı���������
int moveMod;//��ʾ�ƶ����ӵľ������=0��ʾ�Ƶ����ڴ���=1��ʾ��Ծ�����ӣ�=2��ʾ�ϻ���
int moveOkMod;//��ʾ�ܷ��ƶ����ӵľ������=0��ʾ�Ƶ����ڴ���=1��ʾ��Ծ�����ӣ�=2��ʾ�ϻ���
int ifEndMod;//��ʾ�Ƿ�����������=1��ʾһ�������¾ͻ���һ��,˫���������¾ͽ���,=2��ʾ�����壬=3��ʾ�ϻ���
int endNum;//��ʾʣ��������Ӿ��и�����ifEndMod=3ʱ�д˲���
//end ��������

bool isEnd;//��Ϸ�Ƿ����
vector<vector<int>>manual;
int temp[MAXR + 2][MAXC + 2]; //board�ı���

bool IsErrLoca(int r, int c)
{
	if (r<1 || r>row)return true;
	if (c<1 || c>col)return true;
	return board[r][c] == errValue;
}

//begin ��ʼ��
void InitBoard()
{
	for (int i = 0; i <= MAXR + 1; i++)for (int j = 0; j <= MAXC + 1; j++)
	{
		board[i][j] = errValue, isMi[i][j] = 0;
	}
	std::cout << "�������̸��" << std::endl;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
	{
		cinInt(board[i][j]);
	}
	std::cout << "����������ʽ:";
	std::cout << "=0��ʾֻ�к��ߺ����ߵ����̣�=1��ʾ���˺����������ֵ����̣�=2��ʾ���˺��������ֻ���Ҫȥ����" << std::endl;
	cinInt(boardStyle);
	if (boardStyle == 1 || boardStyle == 2)
	{
		for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		{
			cinInt(isMi[i][j]);
		}
	}
	if (boardStyle == 2)
	{
		cinInt(removeLineNum);
		for (int i = 0; i < removeLineNum; i++) {
			for (int j = 0; j < 4; j++)cinInt(removeLines[i][j]);
		}
	}
}

void Init()//��ȡ���ã���ɱ�����ʼ��
{
	std::cout << "������,��:";
	cinInt2(row, col);
	if (row <= 0 || col <= 0)
	{
		isEnd = true;
		return;
	}
	InitBoard();
	std::cout << "����chessman��displayContent:";
	cinInt2(chessman, displayContent);
	std::cout << "����turn:";
	cinInt(turn);
	isEnd = false;

	std::cout << "����howToChangeTurn:";
	cinInt(howToChangeTurn);
	if (howToChangeTurn == 2) {
		cinInt2(turnOptNum[1], turnOptNum[2]);
	}

	std::cout << "����playMod(0��ʾ���ӣ�1��ʾ���ӣ�2��ʾ����):";
	cinInt(playMod);
	if (playMod == 0)
	{
		std::cout << "����placeMod, placeOkMod:";
		cinInt2(placeMod, placeOkMod);
	}
	else if (playMod == 1)
	{
		std::cout << "����moveMod, moveOkMod:";
		cinInt2(moveMod, moveOkMod);
	}
	std::cout << "����ifEndMod:";
	cinInt(ifEndMod);
	if (ifEndMod == 3)cinInt(endNum);
}
//end ��ʼ��



//begin ������ʽ
string getChessman0(int num)//�ڰ���
{
	if (num == 1)return "��";
	if (num == 2)return "��";
	return "?";
}
string getChessman1(int num)
{
	return "?";
}
func_string1 f_getChessman[] = { getChessman0, getChessman1 };
string getChessman(int num)
{
	return f_getChessman[chessman](num);
}
//end ������ʽ



//begin ��������
int getNum(int key)
{
	int ans = 0;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		ans += (board[i][j] == key);
	return ans;
}
//end ��������



//begin ����
string getString0(int r, int c)
{
	if (board[r][c] > 0)return getChessman(board[r][c]);
	if (r == 1)
	{
		if (c == 1)return "��";
		if (c == col)return "��";
		return "��";
	}
	if (r == row)
	{
		if (c == 1)return "��";
		if (c == col)return "��";
		return "��";
	}
	if (c == 1)return "��";
	if (c == col)return "��";
	return "��";
}
string getString1(int r, int c)
{
	if (board[r][c] > 0)return getChessman(board[r][c]);
	return "  ";
}
func_string2 f_getString[] = { getString0, getString1, getString1 };
string getString(int r, int c)
{
	return f_getString[boardStyle](r, c);
}
//end ����



//begin ��ʾ����
void addedDisplay0()
{
	cout << getChessman(1) << "��" << getNum(1) << "��,";
	cout << getChessman(2) << "��" << getNum(2) << "��\n";
}

void addedDisplay1()
{
	return;
}

int getOddEven(int k1, int k2)
{
	int ans = 0;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
	{
		if (board[i][j] > 0)ans += (i + k1) % 2 * ((j + k2) % 2);
	}
	return ans;
}
void addedDisplay2()
{
	cout << "(��,��)��" << getOddEven(0, 0) << "����(ż,ż)��" << getOddEven(1, 1) << "����";
	cout << "(��,ż)��" << getOddEven(0, 1) << "����(ż,��)��" << getOddEven(1, 0) << "��\n";
}

func_void f_addedDisplay[] = { addedDisplay0, addedDisplay1, addedDisplay2 };

void display0()
{
	cout << ' ';
	for (int i = 1; i <= col; i++)cout << ' ' << i % 10;
	for (int i = 1; i <= row; i++)
	{
		cout << endl << setw(2) << i;
		for (int j = 1; j <= col; j++)
		{
			cout << getString(i, j);
		}
	}
	cout << endl;
}

bool ismovedLine(int i1, int j1, int i2, int j2)
{
	for (int i = 0; i < removeLineNum; i++) {
		auto p = removeLines[i];
		if (p[0] == i1 && p[1] == j1 && p[2] == i2 && p[3] == j2)return true;
		if (p[0] == i2 && p[1] == j2 && p[2] == i1 && p[3] == j1)return true;
	}
	return false;
}
bool hasHorizontalLine(int i, int j)//���Һ���
{
	if (IsErrLoca(i, j) || IsErrLoca(i, j + 1))return false;
	return !ismovedLine(i, j, i, j + 1);
}
bool hasVerticalLine(int i, int j)//��������
{
	if (IsErrLoca(i, j) || IsErrLoca(i + 1, j))return false;
	return !ismovedLine(i, j, i + 1, j);
}
bool hasRightSlash(int i, int j)//�����µ�б��
{
	if (IsErrLoca(i, j) || IsErrLoca(i + 1, j + 1))return false;
	return isMi[i][j] || isMi[i + 1][j + 1] && !ismovedLine(i, j, i + 1, j + 1);
}
bool hasLeftSlash(int i, int j)//�ұߺ��±���������б��
{
	if (IsErrLoca(i + 1, j) || IsErrLoca(i, j + 1))return false;
	return isMi[i + 1][j] || isMi[i][j + 1] && !ismovedLine(i + 1, j, i, j + 1);
}
void display1()
{
	for (int i = 1; i <= col; i++)cout << "   " << i % 10;
	for (int i = 1; i <= row; i++)
	{
		cout << endl << setw(2) << i;
		for (int j = 1; j <= col; j++)
		{
			cout << getString(i, j);
			if (j == col)continue;
			if (hasHorizontalLine(i, j))cout << "��";
			else cout << "  ";
		}
		if (i == row)continue;
		cout << endl << "  ";
		for (int j = 1; j <= col; j++)
		{
			if (hasVerticalLine(i, j))cout << "��";
			else cout << "  ";
			if (j == col)break;
			if (hasRightSlash(i, j))cout << "�v";
			else if (hasLeftSlash(i, j))cout << "�u";
			else cout << "  ";
		}
	}
	cout << endl;
}

func_void f_display[] = { display0, display1, display1 };
void display()
{
	system("cls");
	f_display[boardStyle]();
	f_addedDisplay[displayContent]();
	cout << "�ֵ�" << turn << getChessman(turn) << "��\n";
}

void displayManual()//��ʾ����
{
	vector<int>tmp;
	for (auto it = manual.begin(); it != manual.end(); it++)
	{
		tmp = *it;
		for (auto it = tmp.begin(); it != tmp.end(); it++)cout << *it << " ";
	}
	Sleep(5000);
}
//end ��ʾ����



int numInLine(int row, int col, int u, int ref)//���꣨row,col��,��������u�����ظ÷����ж��ٸ�ɫ����,�����������걾��
{
	int i = row + dr[u], j = col + dc[u], sum = 0;
	while (board[i][j] == ref)sum++, i += dr[u], j += dc[u];
	return sum;
}
int numInLine(int row, int col, int u)//���꣨row,col��,��������u�����ظ÷����ж�������ͬɫ����,�����������걾��
{
	int ref = board[row][col];
	if (ref == 0)return 0;
	return numInLine(row, col, u, ref);
}
int neighborNum(int row, int col, int ref)//��������4���ھ����棬�м�����ɫ����
{
	int ans = 0;
	for (int u = 0; u < 8; u += 2)if (numInLine(row, col, u, ref))ans++;
	return ans;
}
int maxNumInLine(int row, int col)//���꣨row,col������������ֱ�������ж�������ͬɫ����,���������걾��
{
	int ans = 0;
	for (int u = 0; u < 4; u++)ans = max(ans, numInLine(row, col, u) + numInLine(row, col, u + 4));
	return ans + 1;
}

//begin ���巽����
void changeTurn0() //�ڰ���
{
	turn = 3 - turn; //turn��1,2֮���ֻ�
}
void changeTurn2()
{
	static int optNum = 0;
	optNum++;
	if (optNum == turnOptNum[turn])turn = 3 - turn, optNum = 0;
}
func_void f_changeTurn[] = { changeTurn0, changeTurn0, changeTurn2 };
void changeTurn()
{
	return f_changeTurn[howToChangeTurn]();
}
//end ���巽����



//begin ����
void place0(int r, int c) //�����ӣ���������
{
	return;
}
void place1(int r, int c)//�гԣ���ڰ���
{
	int k = board[r][c], tr, tc;
	for (int i = 0; i < 8; i++)
	{
		tr = r, tc = c;
		while (true)
		{
			tr += dr[i], tc += dc[i];
			if (tr <= 0 || tr >= row + 1 || tc <= 0 || tc >= col + 1)break;
			if (board[tr][tc] == 3 - k)continue;
			if (board[tr][tc] == k)
			{
				for (int jr = r + dr[i], jc = c + dc[i]; jr != tr || jc != tc; jr += dr[i], jc += dc[i])
					board[jr][jc] = k;
			}
			break;
		}
	}
}
func_void2 f_place[] = { place0, place1 };
void place(int r, int c)
{
	board[r][c] = turn;
	f_place[placeMod](r, c);
}
//end ����



//begin ����
void move0(int r, int c, int r2, int c2)//������
{
	board[r2][c2] = board[r][c], board[r][c] = 0;
}
void move1(int r, int c, int r2, int c2)//���ӻ�������,�Ե�������������
{
	move0(r, c, r2, c2);
	if (abs(r - r2) > 1 || abs(c - c2) > 1)board[(r + r2) / 2][(c + c2) / 2] = 0;
}
void move2(int r, int c, int r2, int c2)//˫����ͬ
{
	if (turn == 1)move0(r, c, r2, c2);
	else move1(r, c, r2, c2);
}
void move3(int r, int c, int r2, int c2)//˫����ͬ
{
	move0(r, c, r2, c2);
	if (turn == 1)return;
	if (neighborNum(r2, c2, 3 - board[r2][c2]) != 1)return;
	for (int u = 0; u < 8; u += 2)
		if (board[r2 + dr[u]][c2 + dc[u]] == 3 - board[r2][c2])board[r2 + dr[u]][c2 + dc[u]] = 0;
}
func_void4 f_move[] = { move0, move1, move2, move3 };
void move(int r, int c, int r2, int c2)
{
	f_move[moveMod](r, c, r2, c2);
}
//end ����




//begin �ܷ�����
bool placeOk0(int r, int c)//������,Ȧ����
{
	return true;
}
int getChangeNum(int r, int c)//����ǰ���ж������Ӹı�
{
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			temp[i][j] = board[i][j];
	place(r, c);
	int changeNum = 0;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		changeNum += (temp[i][j] != board[i][j]), board[i][j] = temp[i][j];
	return changeNum;
}
bool placeOk1(int r, int c)//�ڰ���
{
	return getChangeNum(r, c) > 1;
}
func_bool2 f_placeOk[] = { placeOk0, placeOk1 };
bool placeOk(int r, int c) //�ܷ�����
{
	if (r<1 || r>row || c<1 || c>col)return false;
	if (board[r][c] != 0)return false;
	return f_placeOk[placeOkMod](r, c);
}
//end �ܷ�����




//begin �ܷ�����
bool isJumpPos(int r, int c, int r2, int c2)
{
	return r == r2 && abs(c - c2) == 2 || c == c2 && abs(r - r2) == 2 ||
		abs(c - c2) == 2 && abs(r - r2) == 2 && isMi[(r + r2) / 2][(c + c2) / 2];
}
bool moveOkJump0(int r, int c, int r2, int c2)//����,�Ե�����������������
{
	if (board[r2][c2] != 0)return false;//ֻ�ܵ��յ�
	return isJumpPos(r, c, r2, c2);
}
bool moveOkJump1(int r, int c, int r2, int c2)//����,�Ե��������ĶԷ�����
{
	return moveOkJump0(r, c, r2, c2) && board[(r + r2) / 2][(c + c2) / 2] == 3 - board[r][c];
}
bool moveOkJump2(int r, int c, int r2, int c2)
{
	if (r > 1 && board[r2][c2] != 3 - board[r][c])return false;//ֻ�������Է����Ӳ��Ե�
	return isJumpPos(r, c, r2, c2) && board[(r + r2) / 2][(c + c2) / 2] != 0;
}
bool moveOk0(int r, int c, int r2, int c2)//�Ƶ����ڴ�
{
	if (board[r2][c2] != 0)return false;//ֻ�ܵ��յ�
	if (r == r2 && c == c2)return false;
	if (abs(r - r2) > 1 || abs(c - c2) > 1)return false;//����8�ھ�	
	if (r == r2 || c == c2)return true;//��������
	return isMi[r][c] || isMi[r2][c2];//��б��
}
bool moveOk1(int r, int c, int r2, int c2)//���ӻ�����
{
	return moveOk0(r, c, r2, c2) || moveOkJump1(r, c, r2, c2);
}
bool moveOk2(int r, int c, int r2, int c2)
{
	if (turn == 1)return moveOk0(r, c, r2, c2);
	else return moveOk1(r, c, r2, c2);
}
bool moveOk3(int r, int c, int r2, int c2)
{
	if (board[r2][c2] != 0)return false;//ֻ�ܵ��յ�
	return true;
}
bool moveOk4(int r, int c, int r2, int c2)
{
	return moveOkJump0(r, c, r2, c2);
}
bool moveOk5(int r, int c, int r2, int c2)
{
	if (turn == 1)return moveOk0(r, c, r2, c2);
	if (r == 1 && c == 2 && r2 == 1 && c2 == 4)return true;
	if (r == 1 && c == 4 && r2 == 1 && c2 == 2)return true;
	if (r > 1 && r2 == 1)return false;// �뿪�����ξͲ����ٻ���
	if (moveOk0(r, c, r2, c2))return true;
	return moveOkJump2(r, c, r2, c2);
}
func_bool4 f_moveOk[] = { moveOk0, moveOk1, moveOk2, moveOk3, moveOk4, moveOk5 };
bool moveOk(int r, int c, int moveOkMod)//ֻ�����Լ�����
{
	if (howToChangeTurn == 1)return true;//������ʯ��
	return board[r][c] == turn;//һ��ֻ�����Լ�����
}
bool moveOk(int r, int c, int r2, int c2)
{
	if (IsErrLoca(r, c) || IsErrLoca(r2, c2))return false;
	return moveOk(r, c, moveOkMod) && f_moveOk[moveOkMod](r, c, r2, c2);
}
//end �ܷ�����






//begin �Ƿ����
bool hasPlaceOk()
{
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		if (placeOk(i, j))return true;
	return false;
}
bool ifEnd0()//�����¾ͽ���
{
	return isEnd = !hasPlaceOk();
}
bool ifEnd1()//�ڰ��壬һ�������¾ͻ���һ��,˫���������¾ͽ���
{
	isEnd = !hasPlaceOk();
	if (!isEnd)return false;
	changeTurn();
	return isEnd = !hasPlaceOk();
}
bool ifEnd2()//������
{
	if (!hasPlaceOk())
	{
		return isEnd = true;
	}
	vector<int>tmp = *(manual.end() - 1);
	if (maxNumInLine(tmp[0], tmp[1]) >= 5)return isEnd = true;
	return false;
}
bool hasMoveOk()
{
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		for (int ii = 1; ii <= row; ii++)for (int jj = 1; jj <= col; jj++)
			if (moveOk(i, j, ii, jj))return true;
	return false;
}
bool ifEnd3()
{
	if (turn == 1)isEnd = (getNum(1) <= endNum);
	else isEnd = !hasMoveOk();
	return isEnd;
}
bool ifEnd4()//Ȧ����
{
	vector<int>tmp = *(manual.end() - 1);
	if (maxNumInLine(tmp[tmp.size() - 2], tmp[tmp.size() - 1]) >= 3)return isEnd = true;
	return false;
}
bool ifEnd5()
{
	if (getNum(2) == 3)playMod = 1, moveMod = moveOkMod = 0;
	return ifEnd4();
}
bool ifEnd6()
{
	if (getNum(2) == 3)playMod = 1, moveMod = 0, moveOkMod = 3;
	return ifEnd4();
}
bool ifEnd7()//������ʯ��
{
	if (getNum(1) + getNum(2) <= 1)isEnd = true;
	else isEnd = !hasMoveOk();
	return isEnd;
}
func_bool f_ifEnd[] = { ifEnd0, ifEnd1, ifEnd2, ifEnd3, ifEnd4, ifEnd5, ifEnd6, ifEnd7 };
bool ifEnd()
{
	return f_ifEnd[ifEndMod]();
}
//end �Ƿ����




//begin ����
void play0()//������������
{
	display();
	int r = errValue, c = errValue;
	cout << "������������(�У���)\n����0 0�ɲ鿴����\n";
	cinInt2(r, c);
	if (r == 0 && c == 0)
	{
		displayManual();
		return play0();
	}
	if (placeOk(r, c))
	{
		place(r, c);
		vector<int>tmp;
		tmp.insert(tmp.end(), r);
		tmp.insert(tmp.end(), c);
		manual.insert(manual.end(), tmp);

	}
	else
	{
		cout << "����������������������";
		Sleep(1500);//��ͣ...ms
		play0();
	}
}
void play1()//������������
{
	display();
	int r, c, r2, c2;
	cout << "������������(�У���)->(�У���)\n����0 0�ɲ鿴����\n";
	cinInt2(r, c);
	if (r == 0 && c == 0)
	{
		displayManual();
		return play1();
	}
	cinInt2(r2, c2);
	if (moveOk(r, c, r2, c2))
	{
		move(r, c, r2, c2);
		vector<int>tmp;
		tmp.insert(tmp.end(), r);
		tmp.insert(tmp.end(), c);
		tmp.insert(tmp.end(), r2);
		tmp.insert(tmp.end(), c2);
		manual.insert(manual.end(), tmp);
	}
	else
	{
		cout << "����������������������";
		Sleep(1500);//��ͣ...ms
		play1();
	}
}
func_void f_play[] = { play0, play1 };
void play()
{
	while (isEnd == false)
	{
		f_play[playMod]();
		display();
		changeTurn();
		ifEnd();
	}
	cout << "��Ϸ����";
	Sleep(5000);
}
//end ����

int main()
{
	//freopen(dataInTxt, "r", stdin);
	//ifstream fin;
	//fin.open("D:\init.txt", ios::in);

	Init();
	//freopen("CON", "r", stdin);
	//fin.open("CON", std::ios::in);


	//cin.clear();
	play();
	return 0;
}