# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <malloc.h>
# include <conio.h>
# include <time.h>

struct she_hen  //蛇身坐标结构体
{
	int x;
	int y;
	struct she_hen * pNext;
};
//hu_huan函数里的两个临时变量 因为会有跨函数所以定义成全局变量
struct she_hen t1;
struct she_hen t2;
int fen_shu = -1, nan_du = 5 ,nan_du_qu_xian = 5, su_du;   //分数 提升难度的目标分数 难度曲线 速度统计
int di_tu[15][20] = //地图15排20列    0为空地 1为墙 2为食物 3为蛇头
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//函数前置声明
void chu_shi_hua(struct she_hen *);//初始化界面
void da_yin_tu_xiang(int, int);//打印图像 0为空地 1为墙 2为食物 3为蛇头
void da_yin_di_tu(int, int, int);//打印地图循环
void yi_dong_gui_ze(char fang_xiang, struct she_hen * pHead);//蛇的移动规则
void hu_huan(struct she_hen *);//互换链表坐标
void cl1(struct she_hen * , int, int);//创建蛇身并赋予相应坐标
void shi_wu(void);//随机生成食物
struct she_hen * zhao_wei(struct she_hen *);//移动并找到链表内的蛇尾坐标
char an_jian_chong_tu(char, char);//防止输入错误参数和反向吃自己
void nan_du_di_zeng(void);//难度递增与数据统计
void gotoxy(int, int);//移动dos内的光标  system("cls");也有这个效果但不能调控不好做界面
void you_xi_jie_shu(void);//游戏结束提示
void yin_cang_guang_biao(void);//隐藏dos内的光标

int main(void)//主函数
{
    struct she_hen * pHead = (struct she_hen *)malloc(sizeof(struct she_hen));//创建蛇头链表
	pHead->pNext = NULL;
    char fang_xiang = 'd'; //默认出生向右移动
	char t;
	
	chu_shi_hua(pHead);
    while(1)
	{
		
		if(kbhit())       //kbhit有键盘按键输入时给出1
		{
			fang_xiang = an_jian_chong_tu(t = getch(), fang_xiang);
	    	yi_dong_gui_ze(fang_xiang, pHead);	
		}
        else
		{
			yi_dong_gui_ze(fang_xiang, pHead);
		}
		Sleep(su_du); //延迟n毫秒
	}
	return 0;
}



void chu_shi_hua(struct she_hen * pHead)//初始化界面
{   
    cl1(pHead, 2, 4);  //蛇头坐标
	cl1(pHead, 2, 3);  //蛇身
	cl1(pHead, 2, 2);  //蛇身
    
    //调用dos命令
	system("title 贪吃蛇");//设置dos窗口标题
	system("mode con cols=60 lines=25");//设置dos窗口宽度高度
	yin_cang_guang_biao();//隐藏dos内的光标
	da_yin_di_tu(0, 15, 20);
	int x = 3, y = 7;
	char guang_biao ;
	
	while(1)//操作指南与难度选择界面
	{
		gotoxy(2,16);
		printf("贪吃蛇");
		gotoxy(4,11);
		printf("难度       初始速度");
		gotoxy(y,x);
		printf("d确定》");
		gotoxy(6,11);
		printf("简单          250");
		gotoxy(7,11);
		printf("普通          200");
		gotoxy(8,11);
		printf("困难          150");
		gotoxy(10,2);
		printf("移动：wasd 暂停：p（移动键继续游戏）");
		gotoxy(11,2);
		printf("（请关闭输入法保证键盘在小写）");
		gotoxy(13,26);
		guang_biao = getch();	
		if(guang_biao=='s')
		{
			if(y<=7)
			{
				++y;
			    gotoxy(y-1,x);
	        	printf("      ");
			}
		}
		else if(guang_biao=='w')
		{
			if(y>=7)
			{
				--y;
			    gotoxy(y+1,x);
	        	printf("      ");
			}
		}
		else if(guang_biao=='d')
		{
			if(y == 6)
			{
				su_du = 250;
			}
			else if(y == 7)
			{
				su_du = 200;
			}
			else if(y == 8)
			{
				su_du = 150;
			}
			break;
		}
	}
//将蛇与食物坐标放下并打印整体
	di_tu[2][4] = 3;
	di_tu[2][3] = 3;
	di_tu[2][2] = 3;
	shi_wu();
	da_yin_di_tu(0, 15, 20);
	return;
}

void da_yin_tu_xiang(int y, int x) //打印图像 0为空地 1为墙 2为食物 3为蛇头
{
	gotoxy(y,x*2);
	switch(di_tu[y][x])
	{
	case 0:
		printf("  ");
		break;
	case 1:
		printf("■");
		break;
	case 2:
		printf("○");
		break;
	case 3:
		printf("□");
		break;
	}
	return;
}

void da_yin_di_tu(int min, int ymax, int xmax)//打印地图循环
{
	int x, y;

	for(y=min;y<ymax;++y)      //15排
	{
		for(x=min;x<xmax;++x)  //20列
		{
			da_yin_tu_xiang(y, x);
		}
	}

	return;
}

void yi_dong_gui_ze(char fang_xiang, struct she_hen * pHead)//蛇的移动规则
{
	struct she_hen * p;
	p = pHead->pNext;
	
	if(fang_xiang == 'd')
	{
		if(di_tu[p->y][p->x+1]==2)//吃到食物
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->x; 
            p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y][p->x+1]==0)//走向空地
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->x; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
            da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//死亡
		{
			you_xi_jie_shu();
		}
	}
	else if(fang_xiang == 'a')
	{
		if(di_tu[p->y][p->x-1]==2)//吃到食物
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->x; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y][p->x-1]==0)//走向空地
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->x; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//死亡
		{
			you_xi_jie_shu();
		}
	}
	else if(fang_xiang == 's')
	{
		if(di_tu[p->y+1][p->x]==2)//吃到食物
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->y; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y+1][p->x]==0)//走向空地
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->y; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//死亡
		{
			you_xi_jie_shu();
		}
		
	}
	else if(fang_xiang == 'w')
	{
		if(di_tu[p->y-1][p->x]==2)//吃到食物
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->y; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y-1][p->x]==0)//走向空地
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->y; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//死亡
		{
			you_xi_jie_shu();
		}
		
	}
	
	return;
}

void hu_huan(struct she_hen * p)   ////互换链表坐标
{
	t2.y = t1.y;
	t2.x = t1.x;
	t1.y = p->y;
	t1.x = p->x;
	p->y = t2.y;
	p->x = t2.x;

	return;
}

void cl1(struct she_hen * pHead, int y, int x)//创建蛇身并赋予相应坐标
{

	struct she_hen * p;
	p = pHead;
	while(p->pNext!=NULL)
	{
		p = p->pNext;
	}

	struct she_hen * pTail = p;
 
	struct she_hen * pNew = (struct she_hen *)malloc(sizeof(struct she_hen));  //分配一个新的动态内存作为链表内的蛇尾
	pNew->y = y;
	pNew->x = x;
	pTail->pNext = pNew;
	pNew->pNext = NULL;
	
	return;
}

void shi_wu(void)//随机生成食物
{
	srand((int)time(0));
	int x, y;
	do
	{
		y = rand()%(13)+1;
		x = rand()%(18)+1;
	}while(di_tu[y][x]!=0);//若随机坐标内不是0则重新生成新的随机数
	di_tu[y][x] = 2;//生成食物
	da_yin_tu_xiang(y, x);

	nan_du_di_zeng();//进行难度递增

	return;
}

struct she_hen * zhao_wei(struct she_hen * p)//移动并找到链表内的蛇尾坐标
{	
	di_tu[p->y][p->x] = 3;//向前移动
	da_yin_tu_xiang(p->y, p->x);
	p = p->pNext;
	while(p->pNext!=NULL)//翻找定位蛇尾坐标
	{			
		hu_huan(p);
		p = p->pNext;
	}

	return p;//将坐标返回
}

char an_jian_chong_tu(char t, char fang_xiang)//防止输入错误参数和反向吃自己
{	
	if(t == 'w'||t == 'a'||t == 's'||t == 'd'||t == 'p') //规定特定按键防止误输入
	{
		if(t=='d'&&fang_xiang=='a')
		{
			t = fang_xiang;//若输出与当前行动相反的指令则无效化
		}
		else if(t=='a'&&fang_xiang=='d')
		{
			t = fang_xiang;
		}
		else if(t=='w'&&fang_xiang=='s')
		{
			t = fang_xiang;
		}
		else if(t=='s'&&fang_xiang=='w')
		{
			t = fang_xiang;
		}
	}
	else
	{
		t = fang_xiang;
	}

	return t;
}

void nan_du_di_zeng(void)//难度递增与数据统计
{
	++fen_shu;
	if(nan_du-fen_shu == 0)
	{
		if(nan_du>0)
		{
		su_du -= 10;
		++nan_du_qu_xian;
		nan_du = nan_du+nan_du_qu_xian;
		}
	}
	gotoxy(13,0);
	printf("\n\n分数：%d 速度：%d ", fen_shu, su_du);
	return;
}

void gotoxy(int y, int x)//移动dos内的光标
{
    COORD pos = {x,y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);

	return;
}

void you_xi_jie_shu(void)//游戏结束提示
{
	gotoxy(5,15);
	printf("游戏结束\n");
	gotoxy(8,12);
	system("pause"); //按任意键继续
	gotoxy(9,10);
	exit(-1);

	return;
}

void yin_cang_guang_biao(void)//隐藏dos内的光标
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

	return;
}




























