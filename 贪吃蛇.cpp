/*
	1.���ݵķ������������ݳ�Աȥ����
	2.���̵ķ�������������
	3.������
*/
#define _CRT_SECURENO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>//�������������
/*
	��������
	������
	ʳ������
*/
//��������
typedef struct pointXY
{
	int x;
	int y;
}MYPOINT;
//�ߵ�����
struct Snake
{
	int num;    //�ߵĽ���
	MYPOINT xy[100];  //�������100��,����ڽṹ��������
	char postion;    //�ߵķ���
}snake;
//ʳ�������
struct Food
{
	MYPOINT fdxy;//ʳ���xy����
	int eatgrade;//�Ʒ�
	int flag;      //ʳ���Ƿ����
}food;
//����

enum movPostion{right=77,left=75,down=80,up=72};
HWND hwnd = NULL;//�������ʾ�����ڣ�������graphics
/*
	��ʼ���ߣ�initSnake
	���ߣ�drawSnake
	�ƶ��ߣ�moveSnake
	��������keyDown
	��ʼ��ʳ�initFood
	��ʳ�drawFood
	��ʳ�eatFood
	��������snakeDie
*/
void initSnake();
void drawSnake();
void moveSnake();
void keyDown();
void initFood();
void drawFood();
void eatFood();
int snakeDie();
void showGrade();
void pauseMoment();

int main()
{
	srand((unsigned int)time(NULL));        //Ҫʹ������������������仰��time.hͷ�ļ�
	hwnd = initgraph(640,480);
	setbkcolor(WHITE);//���ô�����ɫΪ��ɫ,ֻ��������ڲ������ɫ��Ҫ���ˢ�º���ʹ�ã����ڲŻ��ɫ
	   
	initSnake();
	while(1)
	{
		cleardevice();//��ɫû�䣬ˢ��һ�¾�ok~������ÿһ�α仯������Ҫˢ��һ�±�����
		if(food.flag == 0)
		{
			initFood();
		}
		drawFood();
		drawSnake();
		if(snakeDie())
		{
			break;
		}
		eatFood();
		showGrade();
		moveSnake();
		//ֻ�а�����ʱ����հ���
		//keyDown();
		if(_kbhit())             //_kbhit()�������ڿ��Ƿ񰴼���������˼�����ִ�а������ƺ�����
		{
			pauseMoment();
			keyDown();
		}
		Sleep(60);//��ֹ���ܵù���
	}

	getchar();//��ֹ����

	closegraph();

	return 0;
}
//�ߵ���Ҫģ��
void initSnake()
{
	//��ǰ���ڵ�����д�����Ϳ��ԣ���ʼ�����ݵĹ���
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;

	snake.xy[1].x = 10;
	snake.xy[1].y = 0;

	snake.xy[0].x = 20;
	snake.xy[0].y = 0;

	snake.num = 3;
	snake.postion = right;

	food.flag = 0;
	food.eatgrade = 0;
}
void drawSnake()
{
	for(int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);     //���α߿����ɫΪ��ɫ
		setfillcolor(GREEN);        //�����ɫΪ��ɫ
		fillrectangle(snake.xy[i].x,snake.xy[i].y,snake.xy[i].x + 10,snake.xy[i].y + 10);
	}
}
void  moveSnake()
{
	//���ƶ�ʱ�����˵�һ�ڣ�����ÿһ�ڶ���ǰ��һ�ڵ�����
	for(int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//��һ�ڵĴ���
	switch(snake.postion)
	{
	case right:
		snake.xy[0].x += 10;
		break;
	case left:
		snake.xy[0].x -= 10;
		break;
	case down:
		snake.xy[0].y += 10;
		break;
	case up:
		snake.xy[0].y -= 10;
		break;
	default:
		break;
	}
}
void keyDown()
{
	//����	1	warning C4996: 'getch': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _getch. See online help for details.	d:\visual studio 2010\С��Ϸ\̰����3\̰����3\̰����.cpp	131	1	̰����3
	//��һ����getch()����δ��_ʱ�ľ���
	char userKey = 0;
	userKey = _getch();
	switch(userKey)
	{
	case right:
		if(snake.postion != left)           //if������ھ�����ͷ�����ҾͲ�����������Ͳ������ң�����ͬ��
			snake.postion = right;
		break;
	case left:
		if(snake.postion != right)
			snake.postion = left;
		break;
	case down:
		if(snake.postion != up)
			snake.postion = down;
		break;
	case up:
		if(snake.postion != down)
			snake.postion = up;
		break;
	default:
		break;
	}
}
//ʳ���������
//ʳ��ĳ�ʼ��
void initFood()
{
	food.fdxy.x = rand() % 65*10;   //650��д��65*10����Ϊ�˷�ֹ����������������µ��߳Բ���ʳ��
	food.fdxy.y = rand() % 48*10;
	food.flag = 1;     //ʳ�����
}
//��ʳ��
void drawFood()
{
	setlinecolor(BLACK);     //���α߿����ɫΪ��ɫ
		setfillcolor(GREEN);        //�����ɫΪ��ɫ
	fillrectangle(food.fdxy.x,food.fdxy.y,food.fdxy.x + 10,food.fdxy.y + 10);
}
//��ʳ��
void eatFood()
{
	//1.�߱䳤��
	//2.ʳ��Ҫ��������
	//3.����Ҫ����
	if(snake.xy[0].x == food.fdxy.x&&snake.xy[0].y == food.fdxy.y)
	{
		snake.num++;
		food.eatgrade += 10;
		food.flag = 0;
	}
}
//������ʱ��������Ϸ����ʱ
int snakeDie()
{
	//ײǽ
	if(snake.xy[0].x > 640||snake.xy[0].y > 480||snake.xy[0].x < 0||snake.xy[0].y < 0)
	{
		outtextxy(200,200,"ײǽ������");
		MessageBox(hwnd,TEXT("GameOver!"),TEXT("��ײǽ�ˣ�"),MB_OK|MB_ICONERROR);
		return 1;
	}
	//�Լ����Լ�
	for(int i = 1; i < snake.num; i++)
	{
		if(snake.xy[0].x == snake.xy[i].x&&snake.xy[0].y == snake.xy[i].y)
		{
			outtextxy(200,200,"����ͼ�Լ��Ե��Լ���");
			MessageBox(hwnd,TEXT("GameOver!"),TEXT("��Ե����Լ���"),MB_OK|MB_ICONERROR);
			return 1;
		}
	}
	return 0;
}
void showGrade()
{
	char grade[100] = "";
	sprintf(grade,"%d",food.eatgrade);
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(520,20,"������");
	outtextxy(520+50,20,grade);
}
void pauseMoment()
{
	if(_getch() == 32)
	{
		while(_getch() != 32);
	}
}
