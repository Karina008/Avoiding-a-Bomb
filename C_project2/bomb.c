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

////맵.폭탄 배열 y 28 x 50
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

int main(void)		//메인함수
{
	CursorView(HIDE);
	ConsoleSet();
	
	for(;;)	{		
		pgame=BombMainScreen();
		life=10;
		if(pgame==1)   {		//1인용
			for(;;)   {			
				if(gotomain == 1)   {
					gotomain = 0;
					break;
				}
				Stagesetting(stage);		//변수들 초기화
				MapMake();					//맵 출력
				Player1_Draw();				//Player_1 출력
				while(1)   {				
					Player1_Moving();		//1인용 움직임함수
					bombcheck();
					BombBombPow();
					bombcheck();

					if(stageclrflag == 1)   {
						PlaySound(TEXT("next"), NULL, SND_ASYNC);		//스테이지 클리어 bgm
						Maprecover();
						CheckAllClear();
						stage += 1;
						score1+=pscore1;
						score2+=pscore2;
						break;
					}

					if(gameoverflag == 1 || escflag == 1)   {		//게임오버 확인			
						OverRestartMain();
						Maprecover();
						break;
					}
				}
			}
		}
		else if(pgame==2)   {		//2인용
			for(;;)   {			
				if(gotomain == 1)   {
					gotomain = 0;
					break;
				}
				Stagesetting(stage);		//변수들 초기화
				MapMake();					//맵출력
				if(p1_life != 1)			//p1_life가 1이 아닐때 출력 즉) 폭탄에 아직 닿지 않았을 때만 출력
					Player1_Draw();				//Player_1 출력
				if(p2_life != 1)			//p2_life가 1이 아닐때 출력 즉) 폭탄에 아직 닿지 않았을 때만 출력
					Player2_Draw();				//Player_2 출력
				while(1)   {					
					Player2_Moving();		//2인용 움직임 함수
					bombcheck();
					BombBombPow();
					bombcheck();

					if(stageclrflag == 1)   {
						PlaySound(TEXT("next"), NULL, SND_ASYNC);		//스테이지 클리어 bgm
						Maprecover();
						CheckAllClear();
						stage += 1;
						score1+=pscore1;
						score2+=pscore2;
						break;
					}

					if(gameoverflag == 1 || escflag == 1)   {		//게임오버 확인			
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
	SetConsoleTitle(TEXT("폭탄피하기!!!"));
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

void MapPrint(int mapx, int mapy)		//맵을 출력하는 함수.
{	
	SetColor(0xF0);		//검정
	if(map[mapnumb][mapy][mapx]==1)		//벽
		printf("▩");
	else if(map[mapnumb][mapy][mapx]==2)	{	//플레이어 위치
		player1_x = mapx;
		player1_y = mapy;
		player2_x = mapx+1;		//2인용 일때 Player_2는 Player_1의 오른쪽에
		player2_y = mapy;
	}
	else if(map[mapnumb][mapy][mapx]==3)	{	//도착지점
		SetColor(0xFB);		//하늘색
		printf("♣");		
		++stargetcount;		//♣의 전체 개수 확인
	}	
	else	{
		//
	}
}

void MapMake()		//맵을 만드는 함수.
{
	int mapy,mapx;
	for(mapy=0;mapy<MAP_SIZE_Y;mapy++)	{
		for(mapx=0;mapx<MAP_SIZE_X;mapx++)		{
			gotoxy(mapx*2,mapy);
			MapPrint(mapx, mapy);		//함수호출
		}		
		printf("\n");
	}
	gotoxy(102,2);
	SetColor(0xF5);		//자주색
	printf("< Stage %d>", stage);
	gotoxy(101,5);
	SetColor(0xF3);		//어두운 녹색
	printf("< P_1 Score >");
	gotoxy(104,7);
	SetColor(0xF8);		//회색
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


void BombErase(int bombnumb)		//폭탄 지우는 함수
{
	for(bomby=0;bomby<MAP_SIZE_Y;bomby++)	{
		for(bombx=0;bombx<MAP_SIZE_X;bombx++)		{
			gotoxy(bombx*2,bomby);			 
			SetColor(0xF4);
			if(bombpattern[stagearray][bombnumb][bomby][bombx]==6)		//폭탄
				printf(" ");		//삭제
			else {
				
			}
		}
		printf("\n");
	}
}

void BombPrinting(int bombnumb)			//폭탄 출력 함수
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

			if(bombpattern[stagearray][bombnumb][bomby][bombx]==6)		//배열에서 6의값은 폭탄을 나타내는 값이다.
				printf("◈");
			else	{
				
			}
		}
		printf("\n");
	}
	t1 = clock();
}

void BombBombPow ()			//폭탄을 시간에 따라 다음패턴으로 넘기는 함수
{
	if(bombstart == 0)	{		//출력된 폭탄이 없을 때. 즉) 처음 게임을 시작할 때
		BombPrinting(bombnumb);		//폭탄 출력함수 호출
		bombstart = 1;
	}
	
	t2 = clock();
	t3 = t2 - t1;
	if(t3 >= bombchangetime[stagearray][bombnumb])	{
		BombErase(bombnumb);		//폭탄 제거함수 호출
		if(bombnumb < bombmax[stagearray] - 1)
			bombnumb += 1;
		else
			bombnumb = 0;
		BombPrinting(bombnumb);		//폭탄 출력함수 호출
	}
}

void OverRestartMain()			//게임오버 당하면 나타내주는 함수
{
	
	system("cls");	
	if(gameoverflag == 1)	{
		gotoxy(40,8);
		SetColor(0xF6);
		printf("◈◈◈◈◈◈◈ Game Over ◈◈◈◈◈◈◈");
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
					if(score1>=100)			//게임오버 당했는데 재도전시 100점을 점수에서 뺀다
						score1-=100;	
					else if(score1>=0)		//만약 100점 미만이라면 0점으로 설정한다.
						score1=0;
					if(score2>=100)
						score2-=100;
					else if(score2>=0)
						score2=0;
					PlaySound(TEXT("restart"), NULL, SND_ASYNC);		//재시작 bgm
					p1_life =0;				//p1_life에 0을 저장한다. 즉) 아직 폭탄에 닿지 않은 상태를 나타낸다.
					p2_life =0;
					life--;					//life를 하나 감소시킨다.
					break;
				case '2':
					RecordScore();			//점수를 기록하기 위해 함수를 호출한다.
					break;
				case '3':
					p1_life =0;				//p1_life에 0을 저장한다. 즉) 아직 폭탄에 닿지 않은 상태를 나타낸다.
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


void PlayerErase1()			//Player_1의 다음위치를 출력하기전에 이전의 위치를 삭제한다.
{
	gotoxy(player1_x*2,player1_y);
	printf(" ");
}


void Player1_Draw()			//Player_1의 위치를 출력한다.
{	
	gotoxy(player1_x*2,player1_y);
	if(player1_x ==player2_x  &&  player1_y==player2_y && pgame==2)   {		//P1와 P2가 겹칠때
		SetColor(0xF9);		//파랑
		printf("●");
	}
	else   {
		SetColor(0xFC);		//빨강
		printf("★");
		if(pgame==2)   {
			gotoxy(player2_x*2, player2_y);
			SetColor(0xF2);
			printf("★");
		}
	}	
}

void Player1_Moving()		//1인용일때의 움직임 함수.
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
							
			case 119 :	//소문자w
			case 87 :   //대문자w
			case UP : px = 0; py = -1;	break;
			case 115 :	//소문자s
			case 83 :	//대문자s
			case DOWN : px = 0; py = 1; break;
			case 97 :	//소문자a
			case 65 :	//대문자a
			case LEFT : px = -1; py = 0; break;
			case 100 :	//소문자d
			case 68 :	//대문자d
			case RIGHT : px = 1; py = 0; break;
			case ESC : escflag = 1;	break;
			default : break;
		}	
		nextp = map[mapnumb][player1_y + py][player1_x + px];
		gotoxy(30,30);
		if(nextp != 1)		{			//nextp의 값이 1이면 벽을 나타내는 값이므로 플레이어를 이동시키지 않는다.
			PlayerErase1();
			pscore1--;
			player1_x += px;
			player1_y += py;
			Player1_Draw();
		}
		if(nextp == 3)		{			//nextp의 값이 3이면 도착지점이므로 점수에 50을 더한후 stargetcount를 감소시킨다.
			map[mapnumb][player1_y][player1_x] = 5;
			--stargetcount;
			score1+=50;
			gotoxy(101,5);
			SetColor(0xF3);
			printf("< P_1 Score >");
			gotoxy(104,7);
			SetColor(0xF8);
			printf("<%d>", score1);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//코인획득 suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}

		if(key == 43)		{	//+		이키를 입력시 다음스테이지로 넘어간다.
			stageclrflag = 1;
		}
		if(key == 95)		{	//-		이키를 입력시 이전스테이지로 넘어간다.
			stage -= 2;
			stageclrflag = 1;
		}
	}
}

void PlayerErase2()			//Player_2의 다음위치를 출력하기전에 이전의 위치를 삭제한다.
{
	gotoxy(player2_x*2,player2_y);
	printf(" ");
}

void Player2_Draw()			//Player_2의 위치를 출력한다.
{	
	gotoxy(player2_x*2,player2_y);
	if(player1_x ==player2_x  &&  player1_y==player2_y)   {
			SetColor(0xF9);
			printf("●");
	}
	else   {
		SetColor(0xF2);
		printf("★");
		gotoxy(player1_x*2, player1_y);
		SetColor(0xFC);
		printf("★");
	}	
}

void Player2_Moving()		//2인용일때의 움직임 함수.
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
			case 119 :												//소문자w
			case 87 :   	px1=0; py1=0; px2=0; py2=-1; break;  //대문자w
			case 115 :												//소문자s
			case 83 :		px1=0; py1=0; px2 = 0; py2 = 1; break;//대문자s
			case 100 :												//소문자d
			case 68 :		px1=0; py1=0; px2 = 1; py2 = 0; break;//대문자d
			case 97 :												//소문자a
			case 65 :		px1=0; py1=0; px2 = -1; py2 = 0; break;//대문자a			
			
			case ESC : escflag = 1;	break;
			default : break;
		}
		
		nextp1 = map[mapnumb][player1_y + py1][player1_x + px1];		
		nextp2 = map[mapnumb][player2_y + py2][player2_x + px2];
		gotoxy(30,30);
	
		if(nextp1 != 1 )   {			//nextp의 값이 1이면 벽을 나타내는 값이므로 플레이어를 이동시키지 않는다.
			PlayerErase1();
			if(p1_life == 0)   {				
				pscore1--;
				player1_x = player1_x + px1;
				player1_y += py1;
				Player1_Draw();
			}
		}

		if(nextp2 != 1 )   {			//nextp의 값이 1이면 벽을 나타내는 값이므로 플레이어를 이동시키지 않는다.
			PlayerErase2();
			if(p2_life == 0)   {
				pscore2--;
				player2_x = player2_x + px2;
				player2_y += py2;
				Player2_Draw();
			}
		}
		
		if(nextp1 == 3)		{			//nextp의 값이 3이면 도착지점이므로 점수에 50을 더한후 stargetcount를 감소시킨다.
			map[mapnumb][player1_y][player1_x] = 5;
			--stargetcount;
			score1+=50;
			gotoxy(101,5);
			SetColor(0xF3);
			printf("< P_1 Score >");
			gotoxy(104, 7);
			SetColor(0xF8);
			printf("<%d>", score1);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//코인획득 suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}
		if(nextp2 == 3)		{			//nextp의 값이 3이면 도착지점이므로 점수에 50을 더한후 stargetcount를 감소시킨다.
			map[mapnumb][player2_y][player2_x] = 5;
			--stargetcount;
			score2+=50;
			gotoxy(101,9);
			SetColor(0xF3);
			printf("< P_2 Score >");
			gotoxy(104,11);
			SetColor(0xF8);
			printf("<%d>", score2);
			PlaySound(TEXT("coin"), NULL, SND_ASYNC);		//코인획득 suound
			if(stargetcount == 0)
				stageclrflag = 1;
		}

		if(key == 43)		{		//+			이키를 입력시 다음스테이지로 넘어간다.
			stageclrflag = 1;
		}	
		if(key == 95)		{		//-			이키를 입력시 이전스테이지로 넘어간다.
			stage -= 2;
			stageclrflag = 1;
		}
	}
}

int bombtouchflag()				//Player_1이 폭탄에 닿았는지 확인하는 함수.
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

int bombtouchflag2()			//Player_2이 폭탄에 닿았는지 확인하는 함수.
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


void bombcheck()				//폭탄에 닿았다면 게임오버되도록 해주는 함수.
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
			PlaySound(TEXT("die"), NULL, SND_ASYNC);		//죽음 sound
		}
	}
}

void Stagesetting(int stage)		//게임시작전 변수들을 초기화 해주는 함수.
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

void Maprecover()			//재시작을 위해 도착지점의 배열값을 3에서 5로 바꿨던 것을 다시 원래대로 되돌리는 함수.
{
	int mapy,mapx;	
	for(mapy=0;mapy<MAP_SIZE_Y;mapy++)	{
		for(mapx=0;mapx<MAP_SIZE_X;mapx++)		{
			if(map[mapnumb][mapy][mapx] == 5)
				map[mapnumb][mapy][mapx] = 3;
		}		
	}
}

void CheckAllClear()		//게임을 모두 클리어했을 때 나타내는 함수.
{
	if(stage == finalstage)	{
		system("cls");		
		gotoxy(35,8);
		SetColor(0xFC);
		printf("◈◈◈◈◈◈◈ Congratulation ~~!! ◈◈◈◈◈◈◈");
		SetColor(0xF9);
		gotoxy(35,10);
		printf("    ◈◈◈◈◈◈◈ All Clear ◈◈◈◈◈◈◈       ");		
		SetColor(0xF0);
		gotoxy(40,18);
		printf("아무 키나 누르시면 점수기록 화면으로 갑니다");	
		getch();
		RecordScore();
		stage = 1;
		gotomain = 1;		
	}
}

void CopyScore()		//정렬된 "Score2.txt"를 "Score.txt"로 기록을 옮기기 위한 함수.
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

void Ranking()		//"Score.txt"에 기록된 함수를 버블정렬로 정렬하여 "Score2.txt"에 기록
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
	
	for(i=0;i<9;i++)   {		//버블정렬 이용
        for(j=0;j<9;j++)   {
            if(score[j] < score[j+1])   {
                temp=score[j];				//점수 정렬
                score[j]=score[j+1];
                score[j+1]=temp;

				strcpy(temp2, name[j]);		//이름 정렬
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
	CopyScore();		//정렬된 "Score2.txt"를 "Score.txt"로 기록을 옮기기 위해 호출
}

void ReadScore()			//"Score.txt"를 불러와 출력하는 함수.
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

void RecordScore()			//"Score.txt"에 이름과 점수를 기록하는 함수.
{
	FILE *fp;
	char name[20], name2[20];
	fp=fopen("Score.txt", "a");
	system("cls");
	if(fp==NULL)   {
		printf("file open error!!");
	}
	if(finalstage==stage)   {			//게임을 모두 클리어시 기록.
		gotoxy(35,8);
		SetColor(0xFC);
		printf("◈◈◈◈◈◈◈ Congratulation ~~!! ◈◈◈◈◈◈◈");
		SetColor(0xF9);
		gotoxy(35,10);
		printf("    ◈◈◈◈◈◈◈ All Clear ◈◈◈◈◈◈◈       ");	
	}
	else   {							//게임오버시 기록.
		gotoxy(40,8);
		SetColor(0xFC);
		printf("◈◈◈◈◈◈◈ Game Over ◈◈◈◈◈◈◈");
	}
	SetColor(0xF0);
	gotoxy(50,10);
	if(pgame==2)   {					//2인용일 때 승패자를 나타냄.
		if(score1>score2)
			printf("Player_1의 승리입니다!");
		else if(score1<score2)
			printf("Player_2의 승리입니다!");
		else
			printf("무승부입니다!");
	}
	gotoxy(40,12);
	printf("enter Player_1 name : ");			//Player_1의 기록저장
	fscanf(stdin, "%s", name);
	fprintf(fp, "%s\t%d\n", name, score1);	
	gotoxy(40, 14);	
	printf("your name : %s\tyour score : %d\n", name, score1);
	if(pgame==2)   {	
		gotoxy(40, 16);
		printf("enter Player_2 name : ");		//Player_2의 기록저장
		fscanf(stdin, "%s", name2);
		fprintf(fp, "%s\t%d\n", name2, score2);
		gotoxy(40, 18);		
		printf("your name : %s\tyour score : %d\n", name2, score2);
	}
	gotoxy(40, 20);
	printf("아무 키나 누르시면 메인으로 돌아갑니다");		
	stage = 1;
	gotomain = 1;
	getch();
	fclose(fp);
	Ranking();			//기록을 버블정렬로 정렬하기위해 함수호출
}

