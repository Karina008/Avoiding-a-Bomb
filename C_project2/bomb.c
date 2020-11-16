#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "BombMain.h"
#include "MapBombArray.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


#define CONSOLE_SIZE_X 120
#define CONSOLE_SIZE_Y 35
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27

enum {HIDE, SHOW};

int bomby, bombx;
int bombnumb = 0;
int stageclrflag;
int bombstart = 0;
int gameoverflag;
int nextbomb, nextbomb2;
int player1_x = 0, player1_y = 0;
int player2_x = 0, player2_y = 0;
int mapnumb = 0;
int stage = 1;
int stagearray = 0;
int stargetcount = 0;
int gotomain = 0;
int escflag = 0;
int score1=0, score2=0;
int pscore1=0, pscore2=0;
int pgame=0;
int p1_life=0, p2_life=0;
int finalstage = 5;
int touch1=0, touch2=0;
int life=0;
clock_t t1,t2,t3;

////��.��ź �迭 y 28 x 50
void CursorView(char show);
void ConsoleSet(void);
void SetColor(int color);
void gotoxy(int x,int y);
void MapPrint(int mapx, int mapy);
void MapMake();
void BombErase(int bombnumb);
void BombPrinting(int bombnumb);
void BombBombPow ();
void OverRestartMain();
void PlayerErase1();
void PlayerErase2();
void Player1_Draw();
void Player1_Moving();
void Player2_Draw();
void Player2_Moving();
int bombtouchflag();
int bombtouchflag2();
void bombcheck();
void Stagesetting(int stage);
void Maprecover();
void CheckAllClear();
void RecordScore();
void Ranking();

int main(void)		//�����Լ�
{
	CursorView(HIDE);
	ConsoleSet();
	
	for(;;)	{		
		pgame=BombMainScreen();
		life=10;
		if(pgame==1)   {		//1�ο�
			for(;;)   {			
				if(gotomain == 1)   {
					gotomain = 0;
					break;
				}
				Stagesetting(stage);		//������ �ʱ�ȭ
				MapMake();					//�� ���
				Player1_Draw();				//Player_1 ���
				while(1)   {				
					Player1_Moving();		//1�ο� �������Լ�
					bombcheck();
					BombBombPow();
					bombcheck();

					if(stageclrflag == 1)   {
						PlaySound(TEXT("next"), NULL, SND_ASYNC);		//�������� Ŭ���� bgm
						Maprecover();
						CheckAllClear();
						stage += 1;
						score1+=pscore1;
						score2+=pscore2;
						break;
					}

					if(gameoverflag == 1 || escflag == 1)   {		//���ӿ��� Ȯ��			
						OverRestartMain();
						Maprecover();
						break;
					}
				}
			}
		}
		else if(pgame==2)   {		//2�ο�
			for(;;)   {			
				if(gotomain == 1)   {
					gotomain = 0;
					break;
				}
				Stagesetting(stage);		//������ �ʱ�ȭ
				MapMake();					//�����
				if(p1_life != 1)			//p1_life�� 1�� �ƴҶ� ��� ��) ��ź�� ���� ���� �ʾ��� ���� ���
					Player1_Draw();				//Player_1 ���
				if(p2_life != 1)			//p2_life�� 1�� �ƴҶ� ��� ��) ��ź�� ���� ���� �ʾ��� ���� ���
					Player2_Draw();				//Player_2 ���
				while(1)   {					
					Player2_Moving();		//2�ο� ������ �Լ�
					bombcheck();
					BombBombPow();
					bombcheck();

					if(stageclrflag == 1)   {
						PlaySound(TEXT("next"), NULL, SND_ASYNC);		//�������� Ŭ���� bgm
						Maprecover();
						CheckAllClear();
						stage += 1;
						score1+=pscore1;
						score2+=pscore2;
						break;
					}

					if(gameoverflag == 1 || escflag == 1)   {		//���ӿ��� Ȯ��			
						OverRestartMain();
						Maprecover();
						break;
					}
				}
			}
		}
	}
	return 0;
}

void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

void ConsoleSet(void) //Console Window Base Setting
{
	system("mode con: cols=120 lines=35");
	SetConsoleTitle(TEXT("��ź���ϱ�!!!"));
	system("color F0");
}

void SetColor(int color) //Console Color Set
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x,int y) //gotoxy Cursor Move
{
	COORD pos={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void MapPrint(int mapx, int mapy)		//���� ����ϴ� �Լ�.
{	
	SetColor(0xF0);		//����
	if(map[mapnumb][mapy][mapx]==1)		//��
		printf("��");
	else if(map[mapnumb][mapy][mapx]==2)	{	//�÷��̾� ��ġ
		player1_x = mapx;
		player1_y = mapy;
		player2_x = mapx+1;		//2�ο� �϶� Player_2�� Player_1�� �����ʿ�
		player2_y = mapy;
	}
	else if(map[mapnumb][mapy][mapx]==3)	{	//��������
		SetColor(0xFB);		//�ϴû�
		printf("��");		
		++stargetcount;		//���� ��ü ���� Ȯ��
	}	
	else	{
		//
	}
}

void MapMake()		//���� ����� �Լ�.
{
	int mapy,mapx;
	for(mapy=0;mapy<MAP_SIZE_Y;mapy++)	{
		for(mapx=0;mapx<MAP_SIZE_X;mapx++)		{
			gotoxy(mapx*2,mapy);
			MapPrint(mapx, mapy);		//�Լ�ȣ��
		}		
		printf("\n");
	}
	gotoxy(102,2);
	SetColor(0xF5);		//���ֻ�
	printf("< Stage %d>", stage);
	gotoxy(101,5);
	SetColor(0xF3);		//��ο� ���
	printf("< P_1 Score >");
	gotoxy(104,7);
	SetColor(0xF8);		//ȸ��
	printf("<%d>", score1);
	SetColor(0xFC);
	gotoxy(104, 13);
	printf("life = %d", life);
	if(pgame==2)   {
		gotoxy(101,9);
		SetColor(0xF3);
		printf("< P_2 Score >");
		gotoxy(104,11);
		SetColor(0xF8);
		printf("<%d>", score2);
	}
}


void BombErase(int bombnumb)		//��ź ����� �Լ�
{
	for(bomby=0;bomby<MAP_SIZE_Y;bomby++)	{
		for(bombx=0;bombx<MAP_SIZE_X;bombx++)		{
			gotoxy(bombx*2,bomby);			 
			SetColor(0xF4);
			if(bombpattern[stagearray][bombnumb][bomby][bombx]==6)		//��ź
				printf(" ");		//����
			else {
				
			}
		}
		printf("\n");
	}
}

void BombPrinting(int bombnumb)			//��ź ��� �Լ�
{

	for(bomby=0;bomby<MAP_SIZE_Y;bomby++)	{
		for(bombx=0;bombx<MAP_SIZE_X;bombx++)		{
			gotoxy(bombx*2,bomby);			
			if(bombnumb == 0)
				SetColor(0xF4);
			else if((bombnumb % 4) == 1 )
				SetColor(0xF1);
			else if((bombnumb % 4) == 2 ) 
				SetColor(0xF5);
			else if((bombnumb % 4) == 3 )
				SetColor(0xF3);
			else if((bombnumb % 4) == 0 )
				SetColor(0xF6);

			if(bombpattern[stagearray][bombnumb][bomby][bombx]==6)		//�迭���� 6�ǰ��� ��ź�� ��Ÿ���� ���̴�.
				printf("��");
			else	{
				
			}
		}
		printf("\n");
	}
	t1 = clock();
}

void BombBombPow ()			//��ź�� �ð��� ���� ������������ �ѱ�� �Լ�
{
	if(bombstart == 0)	{		//��µ� ��ź�� ���� ��. ��) ó�� ������ ������ ��
		BombPrinting(bombnumb);		//��ź ����Լ� ȣ��
		bombstart = 1;
	}
	
	t2 = clock();
	t3 = t2 - t1;
	if(t3 >= bombchangetime[stagearray][bombnumb])	{
		BombErase(bombnumb);		//��ź �����Լ� ȣ��
		if(bombnumb < bombmax[stagearray] - 1)
			bombnumb += 1;
		else
			bombnumb = 0;
		BombPrinting(bombnumb);		//��ź ����Լ� ȣ��
	}
}

void OverRestartMain()			//���ӿ��� ���ϸ� ��Ÿ���ִ� �Լ�
{
	
	system("cls");	
	if(gameoverflag == 1)	{
		gotoxy(40,8);
		SetColor(0xF6);
		printf("�¢¢¢¢¢¢� Game Over �¢¢¢¢¢¢�");
	}
	if(life==0)
		RecordScore();
	else   {
		SetColor(0xF9);
		gotoxy(44,12);
		printf("Restart Stage %d   : \" 1 \"",stage);	
		SetColor(0xFC);
		gotoxy(44,14);
		printf("Record score      : \" 2 \"");	
		gotoxy(44, 16);
		SetColor(0xFD);
		printf("Back to the Main  : \" 3 \"");
		for(;;)	{
			switch(getch())		{
				case '1':
					if(score1>=100)			//���ӿ��� ���ߴµ� �絵���� 100���� �������� ����
						score1-=100;	
					else if(score1>=0)		//���� 100�� �̸��̶�� 0������ �����Ѵ�.
						score1=0;
					if(score2>=100)
						score2-=100;
					else if(score2>=0)
						score2=0;
					PlaySound(TEXT("restart"), NULL, SND_ASYNC);		//����� bgm
					p1_life =0;				//p1_life�� 0�� �����Ѵ�. ��) ���� ��ź�� ���� ���� ���¸� ��Ÿ����.
					p2_life =0;
					life--;					//life�� �ϳ� ���ҽ�Ų��.
					break;
				case '2':
					RecordScore();			//������ ����ϱ� ���� �Լ��� ȣ���Ѵ�.
					break;
				case '3':
					p1_life =0;				//p1_life�� 0�� �����Ѵ�. ��) ���� ��ź�� ���� ���� ���¸� ��Ÿ����.
					p2_life =0;
					gotomain = 1;				
					stage = 1;
					break;
				default:
					continue;
			}
			break;
		}
	}	
}


void PlayerErase1()			//Player_1�� ������ġ�� ����ϱ����� ������ ��ġ�� �����Ѵ�.
{
	gotoxy(player1_x*2,player1_y);
	printf(" ");
}


void Player1_Draw()			//Player_1�� ��ġ�� ����Ѵ�.
{	
	gotoxy(player1_x*2,player1_y);
	if(player1_x ==player2_x  &&  player1_y==player2_y && pgame==2)   {		//P1�� P2�� ��ĥ��
		SetColor(0xF9);		//�Ķ�
		printf("��");
	}
	else   {
		SetColor(0xFC);		//����
		printf("��");
		if(pgame==2)   {
			gotoxy(player2_x*2, player2_y);
			SetColor(0xF2);
			printf("��");
		}
	}	
}

void Player1_Moving()		//1�ο��϶��� ������ �Լ�.
{
	int px = 0, py = 0;
	char key;
	int nextp;
	if(kbhit())	{		
		key = getch();		
		if(key == -32)
			key = getch();
		fflush(stdin);
		switch(key)		{
							
			case 119 :	//�ҹ���w
			case 87 :   //�빮��w
			case UP : px = 0; py = -1;	break;
			case 115 :	//�ҹ���s
			case 83 :	//�빮��s
			case DOWN : px = 0; py = 1; break;
			case 97 :	//�ҹ���a
			case 65 :	//�빮��a
			case LEFT : px = -1; py = 0; break;
			case 100 :	//�ҹ���d
			case 68 :	//�빮��d
			case RIGHT : px = 1; py = 0; break;
			case ESC : escflag = 1;	break;
			default : break;
		}	
		nextp = map[mapnumb][player1_y + py][player1_x + px];
		gotoxy(30,30);
		if(nextp != 1)		{			//nextp�� ���� 1�̸� ���� ��Ÿ���� ���̹Ƿ� �÷��̾ �̵���Ű�� �ʴ´�.
			PlayerErase1();
			pscore1--;
			player1_x += px;
			player1_y += py;
			Player1_Draw();
		}
		if(nextp == 3)		{			//nextp�� ���� 3�̸� ���������̹Ƿ� ������ 50�� ������ stargetcount�� ���ҽ�Ų��.
			map[mapnumb][player1_y][player1_x] = 5;
			--stargetcount;
			score1+=50;
			gotoxy(101,5);
			SetColor(0xF3);
			printf("< P_1 Score >");
			gotoxy(104,7);
			SetColor(0xF8);
			printf("<%d>", score1);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//����ȹ�� suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}

		if(key == 43)		{	//+		��Ű�� �Է½� �������������� �Ѿ��.
			stageclrflag = 1;
		}
		if(key == 95)		{	//-		��Ű�� �Է½� �������������� �Ѿ��.
			stage -= 2;
			stageclrflag = 1;
		}
	}
}

void PlayerErase2()			//Player_2�� ������ġ�� ����ϱ����� ������ ��ġ�� �����Ѵ�.
{
	gotoxy(player2_x*2,player2_y);
	printf(" ");
}

void Player2_Draw()			//Player_2�� ��ġ�� ����Ѵ�.
{	
	gotoxy(player2_x*2,player2_y);
	if(player1_x ==player2_x  &&  player1_y==player2_y)   {
			SetColor(0xF9);
			printf("��");
	}
	else   {
		SetColor(0xF2);
		printf("��");
		gotoxy(player1_x*2, player1_y);
		SetColor(0xFC);
		printf("��");
	}	
}

void Player2_Moving()		//2�ο��϶��� ������ �Լ�.
{
	int px1 = 0, py1 = 0;
	int px2 = 0, py2 = 0;
	char key;
	int nextp1;
	int nextp2;
	if(kbhit())	{		
		key = getch();
		if(key == -32)
			key = getch();
		fflush(stdin);
		switch(key)		{		
			case UP :  		px1 = 0; py1 = -1;   px2=0; py2=0;	break;			
			case DOWN : 	px1 = 0; py1 = 1;  px2=0; py2=0;break;			
			case LEFT : 	px1 = -1; py1 = 0; px2=0; py2=0;break;			
			case RIGHT :	px1 = 1; py1 = 0; px2=0; py2=0; break;
			case 119 :												//�ҹ���w
			case 87 :   	px1=0; py1=0; px2=0; py2=-1; break;  //�빮��w
			case 115 :												//�ҹ���s
			case 83 :		px1=0; py1=0; px2 = 0; py2 = 1; break;//�빮��s
			case 100 :												//�ҹ���d
			case 68 :		px1=0; py1=0; px2 = 1; py2 = 0; break;//�빮��d
			case 97 :												//�ҹ���a
			case 65 :		px1=0; py1=0; px2 = -1; py2 = 0; break;//�빮��a			
			
			case ESC : escflag = 1;	break;
			default : break;
		}
		
		nextp1 = map[mapnumb][player1_y + py1][player1_x + px1];		
		nextp2 = map[mapnumb][player2_y + py2][player2_x + px2];
		gotoxy(30,30);
	
		if(nextp1 != 1 )   {			//nextp�� ���� 1�̸� ���� ��Ÿ���� ���̹Ƿ� �÷��̾ �̵���Ű�� �ʴ´�.
			PlayerErase1();
			if(p1_life == 0)   {				
				pscore1--;
				player1_x = player1_x + px1;
				player1_y += py1;
				Player1_Draw();
			}
		}

		if(nextp2 != 1 )   {			//nextp�� ���� 1�̸� ���� ��Ÿ���� ���̹Ƿ� �÷��̾ �̵���Ű�� �ʴ´�.
			PlayerErase2();
			if(p2_life == 0)   {
				pscore2--;
				player2_x = player2_x + px2;
				player2_y += py2;
				Player2_Draw();
			}
		}
		
		if(nextp1 == 3)		{			//nextp�� ���� 3�̸� ���������̹Ƿ� ������ 50�� ������ stargetcount�� ���ҽ�Ų��.
			map[mapnumb][player1_y][player1_x] = 5;
			--stargetcount;
			score1+=50;
			gotoxy(101,5);
			SetColor(0xF3);
			printf("< P_1 Score >");
			gotoxy(104, 7);
			SetColor(0xF8);
			printf("<%d>", score1);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//����ȹ�� suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}
		if(nextp2 == 3)		{			//nextp�� ���� 3�̸� ���������̹Ƿ� ������ 50�� ������ stargetcount�� ���ҽ�Ų��.
			map[mapnumb][player2_y][player2_x] = 5;
			--stargetcount;
			score2+=50;
			gotoxy(101,9);
			SetColor(0xF3);
			printf("< P_2 Score >");
			gotoxy(104,11);
			SetColor(0xF8);
			printf("<%d>", score2);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//����ȹ�� suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}

		if(key == 43)		{		//+			��Ű�� �Է½� �������������� �Ѿ��.
			stageclrflag = 1;
		}	
		if(key == 95)		{		//-			��Ű�� �Է½� �������������� �Ѿ��.
			stage -= 2;
			stageclrflag = 1;
		}
	}
}

int bombtouchflag()				//Player_1�� ��ź�� ��Ҵ��� Ȯ���ϴ� �Լ�.
{
	nextbomb = bombpattern[stagearray][bombnumb][player1_y][player1_x];
	if(nextbomb == 6)   {
		PlaySound(TEXT("die"), NULL, SND_ASYNC);
		player1_x = 53;
		player1_y = 20;
		p1_life=1;
		return 1;
	}
	else    
		return 0;
	
}

int bombtouchflag2()			//Player_2�� ��ź�� ��Ҵ��� Ȯ���ϴ� �Լ�.
{
	nextbomb2 = bombpattern[stagearray][bombnumb][player2_y][player2_x];
	if(nextbomb2 == 6)   {
		PlaySound(TEXT("die"), NULL, SND_ASYNC);
		player2_x = 55;
		player2_y = 20;
		p2_life=1;
		return 1;
	}
	else 
		return 0;
}


void bombcheck()				//��ź�� ��Ҵٸ� ���ӿ����ǵ��� ���ִ� �Լ�.
{
	
	if(pgame==1)   {
		if(bombtouchflag() ==1)
			gameoverflag = 1;
	}
	if(pgame==2)   {
		if(p1_life == 0)
			touch1 = bombtouchflag();
		if(p2_life == 0)
		touch2 = bombtouchflag2();
		if( touch1 == 1 && touch2 == 1)	{	
			gameoverflag = 1;
			PlaySound(TEXT("die"), NULL, SND_ASYNC);		//���� sound
		}
	}
}

void Stagesetting(int stage)		//���ӽ����� �������� �ʱ�ȭ ���ִ� �Լ�.
{
	pscore1=300;
	pscore2=300;
	stagearray = stage - 1;
	system("cls");
	mapnumb = stagearray;
	bombnumb = 0;
	stargetcount = 0;
	stageclrflag = 0;
	gameoverflag = 0;
	escflag = 0;
	
}

void Maprecover()			//������� ���� ���������� �迭���� 3���� 5�� �ٲ�� ���� �ٽ� ������� �ǵ����� �Լ�.
{
	int mapy,mapx;	
	for(mapy=0;mapy<MAP_SIZE_Y;mapy++)	{
		for(mapx=0;mapx<MAP_SIZE_X;mapx++)		{
			if(map[mapnumb][mapy][mapx] == 5)
				map[mapnumb][mapy][mapx] = 3;
		}		
	}
}

void CheckAllClear()		//������ ��� Ŭ�������� �� ��Ÿ���� �Լ�.
{
	if(stage == finalstage)	{
		system("cls");		
		gotoxy(35,8);
		SetColor(0xFC);
		printf("�¢¢¢¢¢¢� Congratulation ~~!! �¢¢¢¢¢¢�");
		SetColor(0xF9);
		gotoxy(35,10);
		printf("    �¢¢¢¢¢¢� All Clear �¢¢¢¢¢¢�       ");		
		SetColor(0xF0);
		gotoxy(40,18);
		printf("�ƹ� Ű�� �����ø� ������� ȭ������ ���ϴ�");	
		getch();
		RecordScore();
		stage = 1;
		gotomain = 1;		
	}
}

void CopyScore()		//���ĵ� "Score2.txt"�� "Score.txt"�� ����� �ű�� ���� �Լ�.
{
	char name[10][20];
	int score[10];
	int i=0, k=0;
	FILE *fp;
	FILE *fp1;
	fp=fopen("Score.txt", "w");
	fp1=fopen("Score2.txt", "r");
	if(fp1==NULL)   {
		printf("file open error!!");
	}
	for(i=0; i<10 && !feof(fp1) ; i++)   {			
		fscanf(fp1,"%s %d ", name[i], &score[i]);		
		k++;
	}
	for(i=0; i<k; i++)   		
		fprintf(fp, "%s\t%d\n",  name[i], score[i]);
	
	fclose(fp);
	fclose(fp1);
}

void Ranking()		//"Score.txt"�� ��ϵ� �Լ��� �������ķ� �����Ͽ� "Score2.txt"�� ���
{
	char name[10][20];
	int score[10];
	int i=0, j=0, temp=0;
	int k=0;
	char temp2[20];
	FILE *fp;
	FILE *fp1;
	fp=fopen("Score.txt", "r");
	fp1=fopen("Score2.txt", "w");
	if(fp==NULL)   {
		printf("file open error!!");
	}
	for(i=0; i<15 && !feof(fp) ; i++)   {
		fscanf(fp,"%s %d ", name[i], &score[i]);		
		k++;
	}
	
	for(i=0;i<9;i++)   {		//�������� �̿�
        for(j=0;j<9;j++)   {
            if(score[j] < score[j+1])   {
                temp=score[j];				//���� ����
                score[j]=score[j+1];
                score[j+1]=temp;

				strcpy(temp2, name[j]);		//�̸� ����
				strcpy(name[j], name[j+1]);
				strcpy(name[j+1], temp2);               
            }
        }
    }
	
	fflush(stdin);	
	for(i=0; i<k; i++)   {		
		fprintf(fp1, "%s\t%d\n", name[i], score[i]);
	}
	fclose(fp1);
	fclose(fp);
	CopyScore();		//���ĵ� "Score2.txt"�� "Score.txt"�� ����� �ű�� ���� ȣ��
}

void ReadScore()			//"Score.txt"�� �ҷ��� ����ϴ� �Լ�.
{	
	char name[10][20];
	int score[10];
	FILE *fd;
	int i=0, j=15;
	
	if((fd=fopen("Score.txt","r"))==NULL)   {
		printf("file open error");
	}

	for(i=0; i<10 && !feof(fd) ; i++)   {
		gotoxy(40,j+i);
		fscanf(fd,"%s %d ", name[i], &score[i]);
		printf("%d. %10s    %5d\n", i+1, name[i], score[i]);
	}	
	fclose(fd);
}

void RecordScore()			//"Score.txt"�� �̸��� ������ ����ϴ� �Լ�.
{
	FILE *fp;
	char name[20], name2[20];
	fp=fopen("Score.txt", "a");
	system("cls");
	if(fp==NULL)   {
		printf("file open error!!");
	}
	if(finalstage==stage)   {			//������ ��� Ŭ����� ���.
		gotoxy(35,8);
		SetColor(0xFC);
		printf("�¢¢¢¢¢¢� Congratulation ~~!! �¢¢¢¢¢¢�");
		SetColor(0xF9);
		gotoxy(35,10);
		printf("    �¢¢¢¢¢¢� All Clear �¢¢¢¢¢¢�       ");	
	}
	else   {							//���ӿ����� ���.
		gotoxy(40,8);
		SetColor(0xFC);
		printf("�¢¢¢¢¢¢� Game Over �¢¢¢¢¢¢�");
	}
	SetColor(0xF0);
	gotoxy(50,10);
	if(pgame==2)   {					//2�ο��� �� �����ڸ� ��Ÿ��.
		if(score1>score2)
			printf("Player_1�� �¸��Դϴ�!");
		else if(score1<score2)
			printf("Player_2�� �¸��Դϴ�!");
		else
			printf("���º��Դϴ�!");
	}
	gotoxy(40,12);
	printf("enter Player_1 name : ");			//Player_1�� �������
	fscanf(stdin, "%s", name);
	fprintf(fp, "%s\t%d\n", name, score1);	
	gotoxy(40, 14);	
	printf("your name : %s\tyour score : %d\n", name, score1);
	if(pgame==2)   {	
		gotoxy(40, 16);
		printf("enter Player_2 name : ");		//Player_2�� �������
		fscanf(stdin, "%s", name2);
		fprintf(fp, "%s\t%d\n", name2, score2);
		gotoxy(40, 18);		
		printf("your name : %s\tyour score : %d\n", name2, score2);
	}
	gotoxy(40, 20);
	printf("�ƹ� Ű�� �����ø� �������� ���ư��ϴ�");		
	stage = 1;
	gotomain = 1;
	getch();
	fclose(fp);
	Ranking();			//����� �������ķ� �����ϱ����� �Լ�ȣ��
}

