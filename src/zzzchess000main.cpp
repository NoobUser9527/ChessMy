#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<algorithm>
#include <fstream>



using namespace std;

#define MAXR 20 //棋盘最大行数
#define MAXC 20 //棋盘最大列数
#define PLAYER 2 //下棋人数
#define MOVELINE 10 //需要去掉的边的最大数量

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

const char* dataInTxt = "D:\\init.txt";//存放本地配置的文件
const int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };//用8个方向向量实现flat技术
const int dc[] = { 0, 1, 1, 1, 0, -1, -1, -1 };//从上开始，顺时针转，到左上结束
const int errValue = -1;//无效点都配为-1

//begin 配置数据，缩进代表子参数
int row, col;//整个棋盘一共row行，编号1-row，一共col列，编号1-col
int board[MAXR + 2][MAXC + 2];//棋盘的每个格点，=1表示一方的棋子，=2表示另外一方的棋子,0表示空，-1表示无效格子
int boardStyle;//棋盘样式，=0表示只有横线和竖线的棋盘，=1表示除了横竖还有米字的棋盘，=2表示除了横竖和米字还需要去掉边
int isMi[MAXR + 2][MAXC + 2];//每个格点是不是米字型,当boardStyle=1或2时有此参数
int removeLineNum; //需要去掉的边的数量，当当boardStyle=2时有此参数
int removeLines[MOVELINE][4]; //需要去掉的边的两点坐标
int chessman;//棋子样式
int displayContent;//额外显示内容，=0表示显示各方棋子数量,=1表示不显示
int turn;//轮到谁下,=1,=2
int howToChangeTurn;//如何换下棋方,=0表示正常轮换，=1表示双方可任意顺序操作，=2表示双方轮换但玩家一次可以操控多个棋子
int turnOptNum[PLAYER + 1];//双方一次可以操控棋子的个数，howToChangeTurn=2时有此参数 
int playMod;//表示行棋的方式，=0表示落子，=1表示移子，=2表示其他
int placeMod;//表示落子的具体规则，=0表示仅落子，=1表示夹吃
int placeOkMod;//表示能否落子的具体规则,=0表示有空格即可，=1表示有空格而且还要落子改变其他棋子
int moveMod;//表示移动棋子的具体规则，=0表示移到相邻处，=1表示跳跃并吃子，=2表示老虎棋
int moveOkMod;//表示能否移动棋子的具体规则，=0表示移到相邻处，=1表示跳跃并吃子，=2表示老虎棋
int ifEndMod;//表示是否结束具体规则，=1表示一方不可下就换另一方,双方都不能下就结束,=2表示五子棋，=3表示老虎棋
int endNum;//表示剩余多少棋子就判负，当ifEndMod=3时有此参数
//end 配置数据

bool isEnd;//游戏是否结束
vector<vector<int>>manual;
int temp[MAXR + 2][MAXC + 2]; //board的备份

bool IsErrLoca(int r, int c)
{
	if (r<1 || r>row)return true;
	if (c<1 || c>col)return true;
	return board[r][c] == errValue;
}

//begin 初始化
void InitBoard()
{
	for (int i = 0; i <= MAXR + 1; i++)for (int j = 0; j <= MAXC + 1; j++)
	{
		board[i][j] = errValue, isMi[i][j] = 0;
	}
	std::cout << "输入棋盘格点" << std::endl;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
	{
		cinInt(board[i][j]);
	}
	std::cout << "输入棋盘样式:";
	std::cout << "=0表示只有横线和竖线的棋盘，=1表示除了横竖还有米字的棋盘，=2表示除了横竖和米字还需要去掉边" << std::endl;
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

void Init()//读取配置，完成变量初始化
{
	std::cout << "输入行,列:";
	cinInt2(row, col);
	if (row <= 0 || col <= 0)
	{
		isEnd = true;
		return;
	}
	InitBoard();
	std::cout << "输入chessman和displayContent:";
	cinInt2(chessman, displayContent);
	std::cout << "输入turn:";
	cinInt(turn);
	isEnd = false;

	std::cout << "输入howToChangeTurn:";
	cinInt(howToChangeTurn);
	if (howToChangeTurn == 2) {
		cinInt2(turnOptNum[1], turnOptNum[2]);
	}

	std::cout << "输入playMod(0表示落子，1表示移子，2表示其他):";
	cinInt(playMod);
	if (playMod == 0)
	{
		std::cout << "输入placeMod, placeOkMod:";
		cinInt2(placeMod, placeOkMod);
	}
	else if (playMod == 1)
	{
		std::cout << "输入moveMod, moveOkMod:";
		cinInt2(moveMod, moveOkMod);
	}
	std::cout << "输入ifEndMod:";
	cinInt(ifEndMod);
	if (ifEndMod == 3)cinInt(endNum);
}
//end 初始化



//begin 棋子样式
string getChessman0(int num)//黑白棋
{
	if (num == 1)return "●";
	if (num == 2)return "▲";
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
//end 棋子样式



//begin 棋子数量
int getNum(int key)
{
	int ans = 0;
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		ans += (board[i][j] == key);
	return ans;
}
//end 棋子数量



//begin 棋盘
string getString0(int r, int c)
{
	if (board[r][c] > 0)return getChessman(board[r][c]);
	if (r == 1)
	{
		if (c == 1)return "┌";
		if (c == col)return "┐";
		return "┬";
	}
	if (r == row)
	{
		if (c == 1)return "└";
		if (c == col)return "┘";
		return "┴";
	}
	if (c == 1)return "├";
	if (c == col)return "┤";
	return "┼";
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
//end 棋盘



//begin 显示界面
void addedDisplay0()
{
	cout << getChessman(1) << "有" << getNum(1) << "个,";
	cout << getChessman(2) << "有" << getNum(2) << "个\n";
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
	cout << "(奇,奇)有" << getOddEven(0, 0) << "个，(偶,偶)有" << getOddEven(1, 1) << "个，";
	cout << "(奇,偶)有" << getOddEven(0, 1) << "个，(偶,奇)有" << getOddEven(1, 0) << "个\n";
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
bool hasHorizontalLine(int i, int j)//往右横线
{
	if (IsErrLoca(i, j) || IsErrLoca(i, j + 1))return false;
	return !ismovedLine(i, j, i, j + 1);
}
bool hasVerticalLine(int i, int j)//往下竖线
{
	if (IsErrLoca(i, j) || IsErrLoca(i + 1, j))return false;
	return !ismovedLine(i, j, i + 1, j);
}
bool hasRightSlash(int i, int j)//往右下的斜线
{
	if (IsErrLoca(i, j) || IsErrLoca(i + 1, j + 1))return false;
	return isMi[i][j] || isMi[i + 1][j + 1] && !ismovedLine(i, j, i + 1, j + 1);
}
bool hasLeftSlash(int i, int j)//右边和下边连起来的斜线
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
			if (hasHorizontalLine(i, j))cout << "┄";
			else cout << "  ";
		}
		if (i == row)continue;
		cout << endl << "  ";
		for (int j = 1; j <= col; j++)
		{
			if (hasVerticalLine(i, j))cout << "┆";
			else cout << "  ";
			if (j == col)break;
			if (hasRightSlash(i, j))cout << "╲";
			else if (hasLeftSlash(i, j))cout << "╱";
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
	cout << "轮到" << turn << getChessman(turn) << "下\n";
}

void displayManual()//显示棋谱
{
	vector<int>tmp;
	for (auto it = manual.begin(); it != manual.end(); it++)
	{
		tmp = *it;
		for (auto it = tmp.begin(); it != tmp.end(); it++)cout << *it << " ";
	}
	Sleep(5000);
}
//end 显示界面



int numInLine(int row, int col, int u, int ref)//坐标（row,col）,方向向量u，返回该方向有多少该色棋子,不包括此坐标本身
{
	int i = row + dr[u], j = col + dc[u], sum = 0;
	while (board[i][j] == ref)sum++, i += dr[u], j += dc[u];
	return sum;
}
int numInLine(int row, int col, int u)//坐标（row,col）,方向向量u，返回该方向有多少连续同色棋子,不包括此坐标本身
{
	int ref = board[row][col];
	if (ref == 0)return 0;
	return numInLine(row, col, u, ref);
}
int neighborNum(int row, int col, int ref)//上下左右4个邻居里面，有几个该色棋子
{
	int ans = 0;
	for (int u = 0; u < 8; u += 2)if (numInLine(row, col, u, ref))ans++;
	return ans;
}
int maxNumInLine(int row, int col)//坐标（row,col），返回四条直线最多的有多少连续同色棋子,包括此坐标本身
{
	int ans = 0;
	for (int u = 0; u < 4; u++)ans = max(ans, numInLine(row, col, u) + numInLine(row, col, u + 4));
	return ans + 1;
}

//begin 下棋方控制
void changeTurn0() //黑白棋
{
	turn = 3 - turn; //turn在1,2之间轮换
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
//end 下棋方控制



//begin 落子
void place0(int r, int c) //仅落子，如五子棋
{
	return;
}
void place1(int r, int c)//夹吃，如黑白棋
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
//end 落子



//begin 移子
void move0(int r, int c, int r2, int c2)//仅移子
{
	board[r2][c2] = board[r][c], board[r][c] = 0;
}
void move1(int r, int c, int r2, int c2)//移子或跳吃子,吃掉被跳过的棋子
{
	move0(r, c, r2, c2);
	if (abs(r - r2) > 1 || abs(c - c2) > 1)board[(r + r2) / 2][(c + c2) / 2] = 0;
}
void move2(int r, int c, int r2, int c2)//双方不同
{
	if (turn == 1)move0(r, c, r2, c2);
	else move1(r, c, r2, c2);
}
void move3(int r, int c, int r2, int c2)//双方不同
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
//end 移子




//begin 能否落子
bool placeOk0(int r, int c)//五子棋,圈叉棋
{
	return true;
}
int getChangeNum(int r, int c)//落子前后有多少棋子改变
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
bool placeOk1(int r, int c)//黑白棋
{
	return getChangeNum(r, c) > 1;
}
func_bool2 f_placeOk[] = { placeOk0, placeOk1 };
bool placeOk(int r, int c) //能否落子
{
	if (r<1 || r>row || c<1 || c>col)return false;
	if (board[r][c] != 0)return false;
	return f_placeOk[placeOkMod](r, c);
}
//end 能否落子




//begin 能否移子
bool isJumpPos(int r, int c, int r2, int c2)
{
	return r == r2 && abs(c - c2) == 2 || c == c2 && abs(r - r2) == 2 ||
		abs(c - c2) == 2 && abs(r - r2) == 2 && isMi[(r + r2) / 2][(c + c2) / 2];
}
bool moveOkJump0(int r, int c, int r2, int c2)//跳吃,吃掉被跳过的任意棋子
{
	if (board[r2][c2] != 0)return false;//只能到空地
	return isJumpPos(r, c, r2, c2);
}
bool moveOkJump1(int r, int c, int r2, int c2)//跳吃,吃掉被跳过的对方棋子
{
	return moveOkJump0(r, c, r2, c2) && board[(r + r2) / 2][(c + c2) / 2] == 3 - board[r][c];
}
bool moveOkJump2(int r, int c, int r2, int c2)
{
	if (r > 1 && board[r2][c2] != 3 - board[r][c])return false;//只能跳到对方棋子并吃掉
	return isJumpPos(r, c, r2, c2) && board[(r + r2) / 2][(c + c2) / 2] != 0;
}
bool moveOk0(int r, int c, int r2, int c2)//移到相邻处
{
	if (board[r2][c2] != 0)return false;//只能到空地
	if (r == r2 && c == c2)return false;
	if (abs(r - r2) > 1 || abs(c - c2) > 1)return false;//不是8邻居	
	if (r == r2 || c == c2)return true;//上下左右
	return isMi[r][c] || isMi[r2][c2];//有斜线
}
bool moveOk1(int r, int c, int r2, int c2)//移子或跳吃
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
	if (board[r2][c2] != 0)return false;//只能到空地
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
	if (r > 1 && r2 == 1)return false;// 离开三角形就不能再回来
	if (moveOk0(r, c, r2, c2))return true;
	return moveOkJump2(r, c, r2, c2);
}
func_bool4 f_moveOk[] = { moveOk0, moveOk1, moveOk2, moveOk3, moveOk4, moveOk5 };
bool moveOk(int r, int c, int moveOkMod)//只能移自己的棋
{
	if (howToChangeTurn == 1)return true;//独立钻石棋
	return board[r][c] == turn;//一般只能移自己的棋
}
bool moveOk(int r, int c, int r2, int c2)
{
	if (IsErrLoca(r, c) || IsErrLoca(r2, c2))return false;
	return moveOk(r, c, moveOkMod) && f_moveOk[moveOkMod](r, c, r2, c2);
}
//end 能否移子






//begin 是否结束
bool hasPlaceOk()
{
	for (int i = 1; i <= row; i++)for (int j = 1; j <= col; j++)
		if (placeOk(i, j))return true;
	return false;
}
bool ifEnd0()//不可下就结束
{
	return isEnd = !hasPlaceOk();
}
bool ifEnd1()//黑白棋，一方不可下就换另一方,双方都不能下就结束
{
	isEnd = !hasPlaceOk();
	if (!isEnd)return false;
	changeTurn();
	return isEnd = !hasPlaceOk();
}
bool ifEnd2()//五子棋
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
bool ifEnd4()//圈叉棋
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
bool ifEnd7()//独立钻石棋
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
//end 是否结束




//begin 行棋
void play0()//输入坐标落子
{
	display();
	int r = errValue, c = errValue;
	cout << "输入落子坐标(行，列)\n输入0 0可查看棋谱\n";
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
		cout << "输入坐标有误，请重新输入";
		Sleep(1500);//暂停...ms
		play0();
	}
}
void play1()//输入坐标移子
{
	display();
	int r, c, r2, c2;
	cout << "输入移子坐标(行，列)->(行，列)\n输入0 0可查看棋谱\n";
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
		cout << "输入坐标有误，请重新输入";
		Sleep(1500);//暂停...ms
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
	cout << "游戏结束";
	Sleep(5000);
}
//end 行棋

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