#include <graphics.h>
#include <conio.h>
#include <stack>
#include "ArtificialIntelligence.h"

#define AI_ON 1				//����AI
#define AI_DIFF 2			//1 RandomAlgorithm 2 normal

using namespace std;
#define CELLSIZE 37			//���̸��Ӵ�С


//ö�� �ṹ�嶨��
enum CHESS_STATE{ CHESS_EMPTY, CHESS_BLACK, CHESS_WHITE };

typedef struct {
	int state = 0;
	int x = -1;
	int y = -1;
}OP;

typedef struct  {
	int x{};
	int y{};
	IMAGE img;
}ChessStep;


#if AI_ON == 1
/*
typedef struct Situation {
	int alive5;
	int alive4;
	int rage4;
	int die4;
	int alive3;
	int sleep3;
	int die3;
	int alive2;
	int sleep2;
	int die2;
}SITUATION;


SITUATION ChessSituaion[15][15][2];

*/

#endif




//ȫ�ֱ�������

int chess[15][15];
stack <ChessStep> ChessSteps;


//����������
int ChangePos(int pos)
{
	return (pos * CELLSIZE + CELLSIZE + 4);
}

void ChangeChssColor(int *color)
{
	if (*color == CHESS_BLACK)
		*color = CHESS_WHITE;
	else if (*color == CHESS_WHITE)
		*color = CHESS_BLACK;

}

void ChessInit()
{
	//initgraph(800, 640);

	memset(chess, 0, sizeof(chess));		//�������
	while (!ChessSteps.empty())				//���ջ
		ChessSteps.pop();

	const auto hwnd = GetHWnd();
	SetWindowText(hwnd, _T("������"));

	IMAGE img;	// ���� IMAGE ����
				//loadimage(&img, "chess.jpg");	// ��ȡͼƬ�� img ������
	loadimage(&img, _T("IMAGE"), _T("Background"));
	putimage(0, 0, &img);	// ������ (0, 0) λ����ʾ IMAGE ����
	loadimage(&img, _T("IMAGE"), _T("Background2"));
	putimage(600, 0, &img);

	setlinecolor(BLACK);
	for (int n = 0; n < 15; n++)
	{
		line(CELLSIZE + 4, ChangePos(n), 15 * CELLSIZE + 4, ChangePos(n));
		line(ChangePos(n), CELLSIZE + 4, ChangePos(n), 15 * CELLSIZE + 4);
	}
	fillcircle(ChangePos(3), ChangePos(3), 3);
	fillcircle(ChangePos(11), ChangePos(3), 3);
	fillcircle(ChangePos(3), ChangePos(11), 3);
	fillcircle(ChangePos(11), ChangePos(11), 3);
	fillcircle(ChangePos(7), ChangePos(7), 3);


	RECT r = { 646, 200, 746, 240 }, m = { 646,270,746,310 };
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 23;

	settextstyle(&f);
	rectangle(646, 200, 746, 240);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("���¿�ʼ"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r.left += 30;
	r.right += 30;
	rectangle(646, 270, 746, 310);
	f.lfHeight = 23;
	settextstyle(&f);
	drawtext(_T("�� ��"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(646, 340, 746, 380);
	m.top += 70;
	m.bottom += 70;
	drawtext(_T("�� ��"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(646, 410, 746, 450);
	m.top += 70;
	m.bottom += 70;
	drawtext(_T("�� ��"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rectangle(646, 480, 746, 520);
	m.top += 70;
	m.bottom += 70;
	drawtext(_T("�� ��"), &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);





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

void DownChess(int row, int column, int color)
{
	const int rop2 = getrop2();
	const COLORREF CurrentColor = getfillcolor();

	if (color == CHESS_WHITE)
		setfillcolor(WHITE);
	else if (color == CHESS_BLACK)
		setfillcolor(BLACK);
	fillcircle(ChangePos(row), ChangePos(column), 8);

	chess[column][row] = color;

	setfillcolor(CurrentColor);
	setrop2(rop2);
}



bool judge(int x, int y, int ChessColor)
{
	//return false;
	int count = 1, n = 1;



	//��
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



	//��
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

	//��б
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

	//��б
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

	return count >= 5;

}

void win(int winner)
{

	FlushMouseMsgBuffer();
	while (_kbhit())
		_getch();


	// ����Ļ��������ַ���
	RECT r = { 0, 0, 791, 200 };
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 70;
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ����� 
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
	drawtext(_T("�������λ�����¿�ʼ"), &n, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	MOUSEMSG m = GetMouseMsg();						// ���������Ϣ
	while (m.uMsg != WM_LBUTTONDOWN)
	{
		m = GetMouseMsg();
	}

	FlushMouseMsgBuffer();
}


OP GetMouseState()
{
	const MOUSEMSG m = GetMouseMsg();						// ���������Ϣ
	OP Operation;
	// ��ȡһ�������Ϣ


	if (m.uMsg == WM_LBUTTONDOWN)
	{

		if (m.x >= 646 && m.x <= 746)
		{

			if (m.y >= 200 && m.y <= 240)			//���¿�ʼ
			{
				Operation.state = 5;
				return Operation;

			}
			else if (m.y >= 270 && m.y <= 310)			//����
			{
				Operation.state = 2;
				return Operation;
			}
			else if (m.y >= 340 && m.y <= 380)			//�浵
			{
				Operation.state = 3;
				return Operation;
			}
			else if (m.y >= 410 && m.y <= 450)			//����
			{
				Operation.state = 4;
				return Operation;
			}
			else if (m.y >= 480 && m.y <= 520)			//�˳�
			{
				Operation.state = 6;
				return Operation;
			}
		}
		else
		{

			Operation.state = 1;

			for (int row = 0; row < 15; row++)
			{
				if (m.x >= (ChangePos(row) - (CELLSIZE / 2)) && m.x <= (ChangePos(row) + (CELLSIZE / 2)))
				{
					Operation.x = row;
				}
			}
			for (int column = 0; column < 15; column++)
			{
				if (m.y >= (ChangePos(column) - (CELLSIZE / 2)) && m.y <= (ChangePos(column) + (CELLSIZE / 2)))
				{
					Operation.y = column;
				}
			}

			if (Operation.x == -1 || Operation.y == -1)
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
	fopen_s(&fp,"gamesave.dat", "wb+");
	if (fp == nullptr)
		exit(1);

	const int size = ChessSteps.size();
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
	int x, y, color = CHESS_BLACK, size;
	fopen_s(&fp,"gamesave.dat", "rb+");
	if (fp == nullptr)
		exit(1);

	fscanf_s(fp, "%d,", &size);
	ChessInit();

	//while (fp != NULL)
	for (int i = 0; i < size; i++)
	{
		fscanf_s(fp, "%d,%d,", &x, &y);
		SaveStep(x, y);
		DownChess(x, y, color);

		ChangeChssColor(&color);


	}


	fclose(fp);
	return color;
}

/*

void ChessSituaionJudgeOnce(int x, int y, int black_count, int white_count)
{
	if (black_count == 5)
	{
		ChessSituaion[y][x][CHESS_BLACK - 1].alive5++;
	}
	else if (black_count == 4)
	{

	}






	if (white_count == 5)
	{
		ChessSituaion[y][x][CHESS_WHITE - 1].alive5++;
	}




}

*/

/*

void ChessSituaionJudge(int x, int y)
{
	int black_count = 1, white_count = 1, n = 1;
	//ChessSituaion[][][];


	//��
	black_count = 1;
	white_count = 1;
	n = 1;
	while (chess[y - n][x] == CHESS_BLACK)
	{
		n++;
		black_count++;
		if (y - n == -1)
			break;
	}
	n = 1;
	while (chess[y + n][x] == CHESS_BLACK)
	{
		n++;
		black_count++;
		if (y + n == 15)
			break;
	}

	n = 1;
	while (chess[y - n][x] == CHESS_WHITE)
	{
		n++;
		white_count++;
		if (y - n == -1)
			break;
	}
	n = 1;
	while (chess[y + n][x] == CHESS_WHITE)
	{
		n++;
		white_count++;
		if (y + n == 15)
			break;
	}
	
	if (black_count == 5) 
	{
		ChessSituaion[y][x][CHESS_BLACK - 1].alive5++;
	}
	else if (black_count == 4)
	{

	}



	ChessSituaionJudgeOnce(x, y, black_count, white_count);







	//��
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

	//��б
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

	//��б
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





}


*/


//void AI_Judge()
//{
//	ArtificialIntelligence AI;
//	Position pos = AI.getPosition(chess, CHESS_WHITE);
//
//
//
//}





OP GetAI()
{
	OP op;
	ArtificialIntelligence AI;
	op.state = 1;

#if AI_DIFF == 1
	op.x = rand() % 15;
	op.y = rand() % 15;
#elif AI_DIFF == 2


	const Position pos = AI.getPosition(chess, CHESS_BLACK);

	op.y = pos.x;
	op.x = pos.y;



#endif



	return op;
}



void game()
{
	int ChessNow = CHESS_BLACK;
	IMAGE ImgTemp;
	ChessStep step;
	OP op;

	ChessInit();

	while (true)
	{

#if AI_ON == 1

		if (ChessNow == CHESS_BLACK)
			op = GetMouseState();
		else
			op = GetAI();

#else

		op = GetMouseState();

#endif // AI_ON == 1


		if (!op.state)
			continue;
		else if (op.state == 1)			//�������
		{
			if (ChessNow == CHESS_BLACK)
			{
				if (chess[op.y][op.x] != CHESS_EMPTY)
					continue;
				SaveStep(op.x, op.y);
				DownChess(op.x, op.y, CHESS_BLACK);


			}
			else if (ChessNow == CHESS_WHITE)
			{
				if (chess[op.y][op.x] != CHESS_EMPTY)
					continue;
				SaveStep(op.x, op.y);
				DownChess(op.x, op.y, CHESS_WHITE);

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

			ChangeChssColor(&ChessNow);


		}
		else if (op.state == 2)		//�������
		{

#if AI_ON == 1
			BackStep(); 
			BackStep();
#else

			BackStep();
			ChangeChssColor(&ChessNow);

#endif 
			
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
		else if (op.state == 6)
		{
			closegraph();
			exit(0);
		}


	}


}



int main()
{
	initgraph(791, 600);
	ChessInit();

	while (true)
		game();

	
}

