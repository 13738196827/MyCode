# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <malloc.h>
# include <conio.h>
# include <time.h>

struct she_hen  //��������ṹ��
{
	int x;
	int y;
	struct she_hen * pNext;
};
//hu_huan�������������ʱ���� ��Ϊ���п纯�����Զ����ȫ�ֱ���
struct she_hen t1;
struct she_hen t2;
int fen_shu = -1, nan_du = 5 ,nan_du_qu_xian = 5, su_du;   //���� �����Ѷȵ�Ŀ����� �Ѷ����� �ٶ�ͳ��
int di_tu[15][20] = //��ͼ15��20��    0Ϊ�յ� 1Ϊǽ 2Ϊʳ�� 3Ϊ��ͷ
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

//����ǰ������
void chu_shi_hua(struct she_hen *);//��ʼ������
void da_yin_tu_xiang(int, int);//��ӡͼ�� 0Ϊ�յ� 1Ϊǽ 2Ϊʳ�� 3Ϊ��ͷ
void da_yin_di_tu(int, int, int);//��ӡ��ͼѭ��
void yi_dong_gui_ze(char fang_xiang, struct she_hen * pHead);//�ߵ��ƶ�����
void hu_huan(struct she_hen *);//������������
void cl1(struct she_hen * , int, int);//��������������Ӧ����
void shi_wu(void);//�������ʳ��
struct she_hen * zhao_wei(struct she_hen *);//�ƶ����ҵ������ڵ���β����
char an_jian_chong_tu(char, char);//��ֹ�����������ͷ�����Լ�
void nan_du_di_zeng(void);//�Ѷȵ���������ͳ��
void gotoxy(int, int);//�ƶ�dos�ڵĹ��  system("cls");Ҳ�����Ч�������ܵ��ز���������
void you_xi_jie_shu(void);//��Ϸ������ʾ
void yin_cang_guang_biao(void);//����dos�ڵĹ��

int main(void)//������
{
    struct she_hen * pHead = (struct she_hen *)malloc(sizeof(struct she_hen));//������ͷ����
	pHead->pNext = NULL;
    char fang_xiang = 'd'; //Ĭ�ϳ��������ƶ�
	char t;
	
	chu_shi_hua(pHead);
    while(1)
	{
		
		if(kbhit())       //kbhit�м��̰�������ʱ����1
		{
			fang_xiang = an_jian_chong_tu(t = getch(), fang_xiang);
	    	yi_dong_gui_ze(fang_xiang, pHead);	
		}
        else
		{
			yi_dong_gui_ze(fang_xiang, pHead);
		}
		Sleep(su_du); //�ӳ�n����
	}
	return 0;
}



void chu_shi_hua(struct she_hen * pHead)//��ʼ������
{   
    cl1(pHead, 2, 4);  //��ͷ����
	cl1(pHead, 2, 3);  //����
	cl1(pHead, 2, 2);  //����
    
    //����dos����
	system("title ̰����");//����dos���ڱ���
	system("mode con cols=60 lines=25");//����dos���ڿ�ȸ߶�
	yin_cang_guang_biao();//����dos�ڵĹ��
	da_yin_di_tu(0, 15, 20);
	int x = 3, y = 7;
	char guang_biao ;
	
	while(1)//����ָ�����Ѷ�ѡ�����
	{
		gotoxy(2,16);
		printf("̰����");
		gotoxy(4,11);
		printf("�Ѷ�       ��ʼ�ٶ�");
		gotoxy(y,x);
		printf("dȷ����");
		gotoxy(6,11);
		printf("��          250");
		gotoxy(7,11);
		printf("��ͨ          200");
		gotoxy(8,11);
		printf("����          150");
		gotoxy(10,2);
		printf("�ƶ���wasd ��ͣ��p���ƶ���������Ϸ��");
		gotoxy(11,2);
		printf("����ر����뷨��֤������Сд��");
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
//������ʳ��������²���ӡ����
	di_tu[2][4] = 3;
	di_tu[2][3] = 3;
	di_tu[2][2] = 3;
	shi_wu();
	da_yin_di_tu(0, 15, 20);
	return;
}

void da_yin_tu_xiang(int y, int x) //��ӡͼ�� 0Ϊ�յ� 1Ϊǽ 2Ϊʳ�� 3Ϊ��ͷ
{
	gotoxy(y,x*2);
	switch(di_tu[y][x])
	{
	case 0:
		printf("  ");
		break;
	case 1:
		printf("��");
		break;
	case 2:
		printf("��");
		break;
	case 3:
		printf("��");
		break;
	}
	return;
}

void da_yin_di_tu(int min, int ymax, int xmax)//��ӡ��ͼѭ��
{
	int x, y;

	for(y=min;y<ymax;++y)      //15��
	{
		for(x=min;x<xmax;++x)  //20��
		{
			da_yin_tu_xiang(y, x);
		}
	}

	return;
}

void yi_dong_gui_ze(char fang_xiang, struct she_hen * pHead)//�ߵ��ƶ�����
{
	struct she_hen * p;
	p = pHead->pNext;
	
	if(fang_xiang == 'd')
	{
		if(di_tu[p->y][p->x+1]==2)//�Ե�ʳ��
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->x; 
            p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y][p->x+1]==0)//����յ�
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->x; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
            da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//����
		{
			you_xi_jie_shu();
		}
	}
	else if(fang_xiang == 'a')
	{
		if(di_tu[p->y][p->x-1]==2)//�Ե�ʳ��
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->x; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y][p->x-1]==0)//����յ�
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->x; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//����
		{
			you_xi_jie_shu();
		}
	}
	else if(fang_xiang == 's')
	{
		if(di_tu[p->y+1][p->x]==2)//�Ե�ʳ��
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->y; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y+1][p->x]==0)//����յ�
		{
			t1.y = p->y;
			t1.x = p->x;
			++p->y; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//����
		{
			you_xi_jie_shu();
		}
		
	}
	else if(fang_xiang == 'w')
	{
		if(di_tu[p->y-1][p->x]==2)//�Ե�ʳ��
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->y; 
			p = zhao_wei(p);
			hu_huan(p);
			cl1(pHead, t1.y, t1.x);
			shi_wu();
		}
		else if(di_tu[p->y-1][p->x]==0)//����յ�
		{
			t1.y = p->y;
			t1.x = p->x;
			--p->y; 
			p = zhao_wei(p);
			di_tu[p->y][p->x] = 0;
			da_yin_tu_xiang(p->y, p->x);
			hu_huan(p);
		}
		else//����
		{
			you_xi_jie_shu();
		}
		
	}
	
	return;
}

void hu_huan(struct she_hen * p)   ////������������
{
	t2.y = t1.y;
	t2.x = t1.x;
	t1.y = p->y;
	t1.x = p->x;
	p->y = t2.y;
	p->x = t2.x;

	return;
}

void cl1(struct she_hen * pHead, int y, int x)//��������������Ӧ����
{

	struct she_hen * p;
	p = pHead;
	while(p->pNext!=NULL)
	{
		p = p->pNext;
	}

	struct she_hen * pTail = p;
 
	struct she_hen * pNew = (struct she_hen *)malloc(sizeof(struct she_hen));  //����һ���µĶ�̬�ڴ���Ϊ�����ڵ���β
	pNew->y = y;
	pNew->x = x;
	pTail->pNext = pNew;
	pNew->pNext = NULL;
	
	return;
}

void shi_wu(void)//�������ʳ��
{
	srand((int)time(0));
	int x, y;
	do
	{
		y = rand()%(13)+1;
		x = rand()%(18)+1;
	}while(di_tu[y][x]!=0);//����������ڲ���0�����������µ������
	di_tu[y][x] = 2;//����ʳ��
	da_yin_tu_xiang(y, x);

	nan_du_di_zeng();//�����Ѷȵ���

	return;
}

struct she_hen * zhao_wei(struct she_hen * p)//�ƶ����ҵ������ڵ���β����
{	
	di_tu[p->y][p->x] = 3;//��ǰ�ƶ�
	da_yin_tu_xiang(p->y, p->x);
	p = p->pNext;
	while(p->pNext!=NULL)//���Ҷ�λ��β����
	{			
		hu_huan(p);
		p = p->pNext;
	}

	return p;//�����귵��
}

char an_jian_chong_tu(char t, char fang_xiang)//��ֹ�����������ͷ�����Լ�
{	
	if(t == 'w'||t == 'a'||t == 's'||t == 'd'||t == 'p') //�涨�ض�������ֹ������
	{
		if(t=='d'&&fang_xiang=='a')
		{
			t = fang_xiang;//������뵱ǰ�ж��෴��ָ������Ч��
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

void nan_du_di_zeng(void)//�Ѷȵ���������ͳ��
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
	printf("\n\n������%d �ٶȣ�%d ", fen_shu, su_du);
	return;
}

void gotoxy(int y, int x)//�ƶ�dos�ڵĹ��
{
    COORD pos = {x,y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);

	return;
}

void you_xi_jie_shu(void)//��Ϸ������ʾ
{
	gotoxy(5,15);
	printf("��Ϸ����\n");
	gotoxy(8,12);
	system("pause"); //�����������
	gotoxy(9,10);
	exit(-1);

	return;
}

void yin_cang_guang_biao(void)//����dos�ڵĹ��
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

	return;
}




























