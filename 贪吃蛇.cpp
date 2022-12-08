/*
	1.数据的分析：基本数据成员去分析
	2.过程的分析：函数描述
	3.主函数
*/
#define _CRT_SECURENO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>//随机函数的种子
/*
	坐标属性
	蛇属性
	食物属性
*/
//坐标属性
typedef struct pointXY
{
	int x;
	int y;
}MYPOINT;
//蛇的属性
struct Snake
{
	int num;    //蛇的节数
	MYPOINT xy[100];  //蛇最多有100节,存放在结构体数组中
	char postion;    //蛇的方向
}snake;
//食物的属性
struct Food
{
	MYPOINT fdxy;//食物的xy坐标
	int eatgrade;//计分
	int flag;      //食物是否存在
}food;
//方向

enum movPostion{right=77,left=75,down=80,up=72};
HWND hwnd = NULL;//句柄，表示主窗口，包含在graphics
/*
	初始化蛇：initSnake
	画蛇：drawSnake
	移动蛇：moveSnake
	按键处理：keyDown
	初始化食物：initFood
	画食物：drawFood
	吃食物：eatFood
	蛇死亡：snakeDie
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
	srand((unsigned int)time(NULL));        //要使用随机数，就添加上这句话和time.h头文件
	hwnd = initgraph(640,480);
	setbkcolor(WHITE);//设置窗口颜色为白色,只有这个窗口并不会变色，要配合刷新函数使用，窗口才会变色
	   
	initSnake();
	while(1)
	{
		cleardevice();//颜色没变，刷新一下就ok~。坐标每一次变化，都需要刷新一下背景。
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
		//只有按键的时候接收按键
		//keyDown();
		if(_kbhit())             //_kbhit()函数用于看是否按键，如果按了键，就执行按键控制函数。
		{
			pauseMoment();
			keyDown();
		}
		Sleep(60);//防止蛇跑得过快
	}

	getchar();//防止闪屏

	closegraph();

	return 0;
}
//蛇的主要模块
void initSnake()
{
	//把前三节的属性写出来就可以，初始化数据的过程
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
		setlinecolor(BLACK);     //矩形边框的颜色为黑色
		setfillcolor(GREEN);        //填充颜色为绿色
		fillrectangle(snake.xy[i].x,snake.xy[i].y,snake.xy[i].x + 10,snake.xy[i].y + 10);
	}
}
void  moveSnake()
{
	//蛇移动时，除了第一节，后面每一节都是前面一节的坐标
	for(int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//第一节的处理
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
	//警告	1	warning C4996: 'getch': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _getch. See online help for details.	d:\visual studio 2010\小游戏\贪吃蛇3\贪吃蛇3\贪吃蛇.cpp	131	1	贪吃蛇3
	//上一行是getch()函数未加_时的警告
	char userKey = 0;
	userKey = _getch();
	switch(userKey)
	{
	case right:
		if(snake.postion != left)           //if语句用于决定蛇头，往右就不能向左，往左就不能往右，上下同理。
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
//食物随机出现
//食物的初始化
void initFood()
{
	food.fdxy.x = rand() % 65*10;   //650拆开写成65*10，是为了防止，因坐标问题而导致的蛇吃不上食物
	food.fdxy.y = rand() % 48*10;
	food.flag = 1;     //食物存在
}
//画食物
void drawFood()
{
	setlinecolor(BLACK);     //矩形边框的颜色为黑色
		setfillcolor(GREEN);        //填充颜色为绿色
	fillrectangle(food.fdxy.x,food.fdxy.y,food.fdxy.x + 10,food.fdxy.y + 10);
}
//吃食物
void eatFood()
{
	//1.蛇变长了
	//2.食物要重新生成
	//3.分数要增加
	if(snake.xy[0].x == food.fdxy.x&&snake.xy[0].y == food.fdxy.y)
	{
		snake.num++;
		food.eatgrade += 10;
		food.flag = 0;
	}
}
//蛇死亡时，就是游戏结束时
int snakeDie()
{
	//撞墙
	if(snake.xy[0].x > 640||snake.xy[0].y > 480||snake.xy[0].x < 0||snake.xy[0].y < 0)
	{
		outtextxy(200,200,"撞墙而亡！");
		MessageBox(hwnd,TEXT("GameOver!"),TEXT("你撞墙了！"),MB_OK|MB_ICONERROR);
		return 1;
	}
	//自己吃自己
	for(int i = 1; i < snake.num; i++)
	{
		if(snake.xy[0].x == snake.xy[i].x&&snake.xy[0].y == snake.xy[i].y)
		{
			outtextxy(200,200,"你试图自己吃掉自己！");
			MessageBox(hwnd,TEXT("GameOver!"),TEXT("你吃掉了自己！"),MB_OK|MB_ICONERROR);
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
	outtextxy(520,20,"分数：");
	outtextxy(520+50,20,grade);
}
void pauseMoment()
{
	if(_getch() == 32)
	{
		while(_getch() != 32);
	}
}
