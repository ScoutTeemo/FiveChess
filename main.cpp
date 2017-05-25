#include <graphics.h>
#include <conio.h>
#include <stack>


using namespace std;
#define CELLSIZE 37			//棋盘格子大小



enum {CHESS_EMPTY, CHESS_BLACK, CHESS_WHITE};

typedef struct OP{
	int state;
	int x;
	int y;
}OP;

typedef struct CHESSSTEP{
	int x;
	int y;
	IMAGE img;
}ChessStep;




//全局变量定义

int chess[15][15];

stack <ChessStep> ChessSteps;


int ChangePos(int pos)
{
	return (pos * CELLSIZE +CELLSIZE + 4) ;
}

void ChessInit()
{
	int n=0;
	
	//initgraph(800, 640);

	memset(chess, 0, sizeof(chess));		//清空数组
	while (!ChessSteps.empty())				//清空栈
		ChessSteps.pop();

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "五子棋");

	IMAGE img;	// 定义 IMAGE 对象
	//loadimage(&img, "chess.jpg");	// 读取图片到 img 对象中
	loadimage(&img, _T("IMAGE"), _T("Background"));
	putimage(0, 0, &img);	// 在坐标 (0, 0) 位置显示 IMAGE 对象
	loadimage(&img, _T("IMAGE"), _T("Background2"));
	putimage(600,0,&img);

	setlinecolor(BLACK);
	for(n=0;n<15;n++)
	{
		line(CELLSIZE+4 ,ChangePos(n) ,15*CELLSIZE+4 ,ChangePos(n) );
		line(ChangePos(n) ,CELLSIZE+4,ChangePos(n) ,15*CELLSIZE+4);
	}
	fillcircle( ChangePos(3) , ChangePos(3) , 3);
	fillcircle(ChangePos(11) , ChangePos(3) , 3);
	fillcircle( ChangePos(3) ,ChangePos(11) , 3);
	fillcircle(ChangePos(11) ,ChangePos(11) , 3);
	fillcircle( ChangePos(7) , ChangePos(7) , 3);


	RECT r = { 646, 200, 746, 240 }, m = { 646,270,746,310 };
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 23;
	
	settextstyle(&f);
	rectangle(646, 200, 746, 240);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("重新开始"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r.left  += 30;
	r.right += 30;
	rectangle(646, 270, 746, 310);
	f.lfHeight = 23;
	settextstyle(&f);
	drawtext(_T("悔 棋"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(646, 340, 746, 380);
	m.top += 70;
	m.bottom += 70;
	drawtext(_T("存 档"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(646, 410, 746, 450);
	m.top += 70;
	m.bottom += 70;
	drawtext(_T("读 档"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);




	
	FlushMouseMsgBuffer();
	while (_kbhit())
		_getch();
}

void SaveStep(int x, int y)
{
	ChessStep TempStep;
	getimage(&TempStep.img, ChangePos(x) - 9, ChangePos(y) - 9, 18, 18);
	TempStep.x = x;
	TempStep.y = y;

	ChessSteps.push(TempStep);
}

void BackStep()
{
	if (ChessSteps.empty())
		return;
	ChessStep TempStep = ChessSteps.top();
	ChessSteps.pop();
	putimage(ChangePos(TempStep.x) - 9, ChangePos(TempStep.y) - 9, &TempStep.img);
	chess[TempStep.y][TempStep.x] = CHESS_EMPTY;

}

void DownChess(int row,int column,int color)
{
	int rop2 = getrop2();
	COLORREF CurrentColor = getfillcolor();

	if(color == CHESS_WHITE)
		setfillcolor(WHITE);
	else if(color == CHESS_BLACK)
		setfillcolor(BLACK);
	fillcircle(ChangePos(row) ,ChangePos(column) ,8);

	chess[column][row] = color;

	setfillcolor(CurrentColor);
	setrop2(rop2);
}



bool judge(int x, int y,int ChessColor)
{
	//return false;
	int count = 1, n = 1;

	

	//竖
	while (chess[y - n][x] == ChessColor)
	{
		n++;
		count++;
		if (y - n == -1)
			break;
	}
	n = 1;
	while (chess[y + n][x] == ChessColor)
	{
		n++;
		count++;
		if (y + n == 15)
			break;
	}
	if (count >= 5)
		return true;


	
	//横
	count = 1;
	n = 1;
	while (chess[y][x - n] == ChessColor)
	{
		n++;
		count++;
		if (x - n == -1)
			break;
		
	}
	n = 1;
	while (chess[y][x + n] == ChessColor)
	{
		n++;
		count++;
		if (x + n == 15)
			break;

	}
	if (count >= 5)
		return true;

	//正斜
	count = 1;
	n = 1;
	while (chess[y - n][x - n] == ChessColor)
	{
		n++;
		count++;
		if (y - n == -1 || x - n == -1)
			break;

	}
	n = 1;
	while (chess[y + n][x + n] == ChessColor)
	{
		n++;
		count++;
		if (y + n == 15 || x + n == 15)
			break;

	}
	if (count >= 5)
		return true;

	//反斜
	count = 1;
	n = 1;
	while (chess[y + n][x - n] == ChessColor)
	{
		n++;
		count++;
		if (y + n == 15 || x - n == -1)
			break;

	}
	n = 1;
	while (chess[y - n][x + n] == ChessColor)
	{
		n++;
		count++;
		if (y - n == -1 || x + n == 15)
			break;

	}
	if (count >= 5)
		return true;


	return false;

}

void win(int winner)
{

	FlushMouseMsgBuffer();
	while (_kbhit())
		_getch();


	// 在屏幕中央输出字符串
	RECT r = { 0, 0, 791, 200 };
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 70;
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿 
	settextstyle(&f);
	if (winner == CHESS_BLACK)
		drawtext(_T("Winner is BLACK!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else if (winner == CHESS_WHITE)
		drawtext(_T("Winner is WHITE!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else if (winner == CHESS_EMPTY)
		drawtext(_T("It Ends in a Draw!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT n = { 0,135,791,160 };
	f.lfHeight = 20;
	settextstyle(&f);
	drawtext(_T("按任意键重新开始"), &n, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



	_getch();
	while (_kbhit())
		_getch();
	FlushMouseMsgBuffer();
}


OP GetMouseState()
{
	MOUSEMSG m;						// 定义鼠标消息
	OP Operation={0 ,-1 ,-1 };
	int row,column;					//row 行 column 列
	// 获取一条鼠标消息
	m = GetMouseMsg();

	if (m.uMsg == WM_LBUTTONDOWN)
	{

		if (m.x >= 646 && m.x <= 746)
		{

			if (m.y >= 200 && m.y <= 240)			//重新开始
			{
				Operation.state = 5;
				return Operation;

			}
			else if (m.y >= 270 && m.y <= 310)			//悔棋
			{
				Operation.state = 2;
				return Operation;
			}
			else if (m.y >= 340 && m.y <= 380)			//存档
			{
				Operation.state = 3;
				return Operation;
			}
			else if (m.y >= 410 && m.y <= 450)			//读档
			{
				Operation.state = 4;
				return Operation;
			}
		}
		else
		{

			Operation.state = 1;

			for (row = 0; row < 15; row++)
			{
				if (m.x >= (ChangePos(row) - (CELLSIZE / 2)) && m.x <= (ChangePos(row) + (CELLSIZE / 2)))
				{
					Operation.x = row;
				}
			}
			for (column = 0; column < 15; column++)
			{
				if (m.y >= (ChangePos(column) - (CELLSIZE / 2)) && m.y <= (ChangePos(column) + (CELLSIZE / 2)))
				{
					Operation.y = column;
				}
			}

			if(Operation.x == -1 || Operation.y == -1)
				Operation.state = 0;

		}

	}

	return Operation;
}



void SaveGame()
{
	FILE *fp;
	ChessStep TempStep;
	stack<ChessStep> TempSteps;
	int size, i;
	fp = fopen("gamesave.dat", "wb+");
	if (fp == NULL)
		exit(1);

	size = ChessSteps.size();
	fprintf(fp, "%d,", size);

	while (!ChessSteps.empty())
	{
		TempStep = ChessSteps.top();
		ChessSteps.pop();
		TempSteps.push(TempStep);
	}

	while (!TempSteps.empty())
	{
		TempStep = TempSteps.top();
		TempSteps.pop();
		ChessSteps.push(TempStep);
		fprintf(fp, "%d,%d,", TempStep.x, TempStep.y);
	}
	//fprintf(fp, "\0");

	fclose(fp);


}


int LoadGame()
{
	FILE *fp;
	int x, y, color = CHESS_BLACK, size, i;
	fp = fopen("gamesave.dat", "rb+");
	if (fp == NULL)
		exit(1);

	fscanf(fp, "%d,", &size);
	ChessInit();

	//while (fp != NULL)
	for(i = 0;i < size ;i++)
	{
		fscanf(fp, "%d,%d,", &x, &y);
		SaveStep(x, y);
		DownChess(x, y, color);

		if (color == CHESS_BLACK)
			color = CHESS_WHITE;
		else if (color == CHESS_WHITE)
			color = CHESS_BLACK;

	}


	fclose(fp);
	return color;
}


void game()
{
	int ChessNow = CHESS_BLACK;
	IMAGE ImgTemp;
	ChessStep step;
	OP op;

	ChessInit();

	while(1)
	{ 
		op = GetMouseState();
		if(!op.state)
			continue ;
		else if(op.state == 1)			//下棋操作
		{
			if(ChessNow == CHESS_BLACK)
			{
				if (chess[op.y][op.x] != CHESS_EMPTY)
					continue;
				SaveStep(op.x, op.y);
				DownChess(op.x ,op.y , CHESS_BLACK);
				
			
			}
			else if(ChessNow == CHESS_WHITE)
			{
				if (chess[op.y][op.x] != CHESS_EMPTY)
					continue;
				SaveStep(op.x, op.y);
				DownChess(op.x ,op.y , CHESS_WHITE);
				
			}
			
			
			if (judge(op.x, op.y, ChessNow))
			{
				win(ChessNow);
				break;
			}

			if (ChessSteps.size() == 15 * 15)
			{
				win(CHESS_EMPTY);
				break;
			}



			if (ChessNow == CHESS_BLACK)
				ChessNow = CHESS_WHITE;
			else if (ChessNow == CHESS_WHITE)
				ChessNow = CHESS_BLACK;



			

		}
		else if(op.state == 2)		//悔棋操作
		{
			BackStep();
			if (ChessNow == CHESS_BLACK)
				ChessNow = CHESS_WHITE;
			else if (ChessNow == CHESS_WHITE)
				ChessNow = CHESS_BLACK;
		}
		else if (op.state == 3)
		{
			SaveGame();
		}
		else if (op.state == 4)
		{
			ChessNow = LoadGame();
		}
		else if (op.state == 5)
		{
			break;
		}

	}


}



void main()
{
	initgraph(791, 600);
	ChessInit();
	
	while(1)
		game();

	_getch();
	closegraph();
}

