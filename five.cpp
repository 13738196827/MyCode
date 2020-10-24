# include <stdio.h>
# include <windows.h>
# include <conio.h>
# include <time.h>
# include <graphics.h>
# include <malloc.h>

# define fan_qi_se(a) (a==1)?2:1//��ɫ���� ����ǰΪ�����Ӧ��������������Ӧ����
# define qi_zi(a) (a==1||a==2)?1:0//����β������ӱ�ţ�1��2�������1�������0

typedef struct zuo_biao_tong_ji//����ͳ��
{
	int fen_zhi;
	int y;
	int x;
	int jie_guo;
}ZBTJ;

typedef struct bu_shu_tong_ji//����ͳ��
{
	int bu_shu;
	int y;
	int x;
}BSTJ;

typedef struct fen_zhi_hui_zong//��ֵ����
{
	int fen_zhi;
	int shu_liang;
	int zu_dang;
	int rong_yu;
	int rong_yu_zu_dang;
	int sheng_yu_kong_jian;//���������ʣ��ռ�
}FZHZ;

typedef struct xi_tong_can_shu//ϵͳ����
{
	int nan_du = 11;//�Ѷ� Ĭ��Ϊ11��0�� 7���� 11����
	int mo_shi = 0;//Ĭ��Ϊ0��0�˻� 1��� 
	int sheng_fu = 0;//0ʤ��δ�� 1�׷�ʤ�� 2�ڷ�ʤ�� 3ƽ��
	int bu_shu = 0;//��ǰ���Ӵ���
	int huan_se = 2;//������ɫ �˻���սĬ���������
	int jie_mian = 0;//����0Ϊ������ 1Ϊ���ý���	
	int luo_zi_shun_xu = 0;//����˳�� Ĭ��Ϊ0��0�ر� 1��
}XTCS;

typedef struct lin_jie_biao
{
	int y = 0;
	int x = 0;
	lin_jie_biao* next = NULL;
}LJB;


//����ȫ�ֱ���
XTCS xtcs;
BSTJ bstj[225];
LJB ljb[17][17] = { 0 };//���ڴ��AIλ���жϹ�ϵ
int luo_zi_dian[17][17] = { 0 };//AI�����ӵ�

int qi_pan[17][17] =   //0δ���� 1���� 2���� 3��Ե
{
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
};





//��Ϸ��Ҫ����
void chu_shi_hua(void);//Ԥ�����ʼ��
void da_yin_qi_pan(void);//�����ӡ
void shu_biao_jiao_hu(MOUSEMSG);//��꽻��
int xia_zi(int, int, int);//���Ӻʹ������Ӽ�¼������0ʤ��δ�� 1�׷�ʤ 2�ڷ�ʤ 3ƽ��
int wu_lian_pan_ding(int, int, int);//�ж��Ƿ�ﵽ������ƽ��
void pan_ding_sheng_fu(int);//�ж�ʤ����һ��

//��Ϸģʽ
void hui_qi(int);//����
void chong_xin_kai_shi(void);//���¿�ʼ
void qi_zi_huan_se(int);//�˻���սʱ�л�ai������ɫ
void shun_xu(void);//����˳�����ʾ

//AI����
int zhu_yao_tong_ji(int, int, int);//ͳ����Ҫ��ֵ���븱��ֵ���
int ji_suan_zui_you_luo_zi(int, int, int);//ȫ�ֱ����ж����ӵ�����λ��

int cha_ru(ZBTJ*, int, ZBTJ);//�������������
int shan_chu(ZBTJ*, int, int);//����ָ��λ������

void kou_fen(int, int, int, int*);//��ȡ�赲ֵ���ж���ο۷�
int you_shi_zhi_tong_ji(int, int, int);//ͳ�Ƶ�ǰ�ⲽ�����Ӻ�ȫ���������
int you_shi_zhi_ji_suan(int, int, int);

void yu_ce_shu_pai_wu_lian(int, int, int, FZHZ*);//Ԥ�⵱ǰ|���Ƿ���ԴﵽԤ����������
void yu_ce_heng_pai_wu_lian(int, int, int, FZHZ*);//Ԥ�⵱ǰ-���Ƿ���ԴﵽԤ����������
void yu_ce_fan_xie_pai_wu_lian(int, int, int, FZHZ*);//Ԥ�⵱ǰ\���Ƿ���ԴﵽԤ����������
void yu_ce_zheng_xie_pai_wu_lian(int, int, int, FZHZ*);//Ԥ�⵱ǰ/���Ƿ���ԴﵽԤ����������

void shu_pai_ji_suan(FZHZ*, int, int, int, int);//���㵱ǰ|�ŷ�ֵ
void heng_pai_ji_suan(FZHZ*, int, int, int, int);//���㵱ǰ-�ŷ�ֵ
void fan_xie_pai_ji_suan(FZHZ*, int, int, int, int);//���㵱ǰ\�ŷ�ֵ
void zheng_xie_pai_ji_suan(FZHZ*, int, int, int, int);//���㵱ǰ/�ŷ�ֵ


int ji_suan_zui_you_luo_zi1(int qi_se);
int aa(void);




void gou_zao_lin_jie_biao(void);
void lian_jie_guan_xi(int y, int x);//AI�����ж����ӹ�ϵ
void gou_zao_jie_dian(LJB* ljb, int y, int x);//����ڵ�
void tian_jia_luo_zi_dian(int y, int x);//������ӵ�
void shan_chu_luo_zi_dian(int y, int x);//ɾ�����ӵ�

int main(void)
{
	chu_shi_hua();

	while (1)
	{
		if (MouseHit())
		{
			shu_biao_jiao_hu(GetMouseMsg());
		}
	}

	EndBatchDraw();//�ر�������ͼ
	closegraph();// �ر�ͼ�δ���

	return 0;
}

//��Ϸ��Ҫ����
void chu_shi_hua(void)
{
	initgraph(1024, 768, SHOWCONSOLE);// ��ʼ��ͼ�δ�����ʾ����̨
	BeginBatchDraw();//����������ͼ
	LOGFONT f;//������������Խṹ��
	gettextstyle(&f);//��ȡ��ǰ��������
	setbkmode(TRANSPARENT);//�������屳��Ϊ͸��
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);//��������
	da_yin_qi_pan();
	gou_zao_lin_jie_biao();//�����ڽӱ�

	return;
}

void da_yin_qi_pan(void)
{
	int i, j;
	char s[50];
	// ��ʼ��ͼ�δ���
	setbkcolor(RGB(115, 115, 115));
	setlinecolor(BLACK);
	cleardevice();
	//��ʼ����������
	settextcolor(BLACK);//����������ɫ
	settextstyle(16, 0, _T("����"));




	//��ӡ����
	setfillcolor(RGB(205, 155, 29));
	fillrectangle(174, 46, 850, 722);//��ӡ���
	rectangle(204, 76, 820, 692);//��ӡ�ڿ�

	//��ӡ���̽�������
	for (i = 1; i < 14; ++i)
	{
		line(204 + (44 * i), 76, 204 + (44 * i), 692);//��ӡ����
		line(204, 76 + (44 * i), 820, 76 + (44 * i));//��ӡ����
	}

	//��ӡ�����
	setfillcolor(BLACK);
	solidcircle(512, 384, 5);
	solidcircle(204 + (44 * 3), 76 + (44 * 3), 5);
	solidcircle(204 + (44 * 11), 76 + (44 * 11), 5);
	solidcircle(204 + (44 * 11), 76 + (44 * 3), 5);
	solidcircle(204 + (44 * 3), 76 + (44 * 11), 5);


	//��������
	settextcolor(BLACK);//����������ɫ
	for (i = 0; i < 15; ++i)
	{
		sprintf(s, "%c", 'A' + i);
		outtextxy(200 + (44 * i), 50, s);
		sprintf(s, "%d", 1 + i);
		outtextxy(183, 69 + (44 * i), s);
	}

	//�����¼
	settextcolor(BLACK);//����������ɫ
	settextstyle(25, 0, _T("��Բ"));
	sprintf(s, "�����¼");
	outtextxy(35, 135, s);
	setfillcolor(RGB(240, 235, 213));//����ɫ
	fillrectangle(25, 180, 150, 640);//��ӡ����

	settextstyle(16, 0, _T("����"));
	for (i = 0, j = xtcs.bu_shu; j > 0 && i < 21; ++i, --j)
	{
		sprintf(s, "%0d��", bstj[j - 1].bu_shu);
		outtextxy(55, 200 + 20 * i, s);
		sprintf(s, "%c%0d", 'A' + (bstj[j - 1].x - 1), bstj[j - 1].y);
		outtextxy(85, 200 + 20 * i, s);
	}

	//��ӡ����
	for (int y = 1; y < 16; ++y)
	{
		for (int x = 1; x < 16; ++x)
		{
			if (qi_pan[y][x] == 1)
			{
				setfillcolor(WHITE);
				solidcircle(204 + (44 * (x - 1)), 76 + (44 * (y - 1)), 16);//��ӡ����
			}
			else if (qi_pan[y][x] == 2)
			{
				setfillcolor(BLACK);
				solidcircle(204 + (44 * (x - 1)), 76 + (44 * (y - 1)), 16);//��ӡ����
			}
		}
	}
	if (xtcs.luo_zi_shun_xu == 1)
	{
		shun_xu();
	}

	//��ʾʤ��
	settextcolor(YELLOW);//����������ɫ
	settextstyle(60, 0, _T("����"));
	RECT r = { 0, 0, 1024, 768 };
	if (xtcs.sheng_fu == 1)
	{
		drawtext(_T("  �׷�ʤ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (xtcs.sheng_fu == 2)
	{
		drawtext(_T("  �ڷ�ʤ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (xtcs.sheng_fu == 3)
	{
		drawtext(_T("  ƽ  �֣�"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//�Ҳ���Ϣ��ʾ
	settextcolor(BLACK);//����������ɫ
	settextstyle(26, 0, _T("��������"));

	if (xtcs.mo_shi == 0)
	{
		r = { 940 - 100, 100 - 20, 940 + 100, 100 + 20 };//��������
		if (xtcs.nan_du == 11)
		{
			drawtext(_T("��������(����)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (xtcs.nan_du == 7)
		{
			drawtext(_T("��������(����)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (xtcs.nan_du == 0)
		{
			drawtext(_T("��������(��)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else if (xtcs.mo_shi == 1)
	{
		r = { 920 - 100, 100 - 20, 920 + 100, 100 + 20 };//��������
		drawtext(_T("��������"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	r = { 920 - 100, 300 - 20, 920 + 100, 300 + 20 };//��������
	drawtext(_T("��������"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	r = { 900 - 50, 160 - 20, 900 + 50, 160 + 20 };//��������
	drawtext(_T("�� ɫ"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 900 - 50, 360 - 20, 900 + 50, 360 + 20 };//��������
	drawtext(_T("�� ɫ"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (xtcs.mo_shi == 1)
	{
		setfillcolor(WHITE);
		solidcircle(970, 160, 16);//��ӡ����

		setfillcolor(BLACK);
		solidcircle(970, 360, 16);//��ӡ����
	}
	else if (xtcs.mo_shi == 0)
	{
		if (xtcs.huan_se == 2)
		{
			setfillcolor(WHITE);
			solidcircle(970, 160, 16);//��ӡ����

			setfillcolor(BLACK);
			solidcircle(970, 360, 16);//��ӡ����
		}
		else
		{
			setfillcolor(BLACK);
			solidcircle(970, 160, 16);//��ӡ����

			setfillcolor(WHITE);
			solidcircle(970, 360, 16);//��ӡ����
		}

	}



	//�Ҳ�ѡ����
	settextcolor(BLACK);//����������ɫ
	settextstyle(24, 0, _T("��������"));
	setfillcolor(RGB(165, 165, 165));//���ɫ

	fillrectangle(940 - 50, 630 - 20, 940 + 50, 630 + 20);//��ӡ����
	r = { 940 - 50, 630 - 20, 940 + 50, 630 + 20 };//��������
	drawtext(_T("��Ϸ����"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(940 - 50, 560 - 20, 940 + 50, 560 + 20);//��ӡ����
	r = { 940 - 50, 560 - 20, 940 + 50, 560 + 20 };//��������
	drawtext(_T("���¿�ʼ"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(940 - 50, 490 - 20, 940 + 50, 490 + 20);//��ӡ����
	r = { 940 - 50, 490 - 20, 940 + 50, 490 + 20 };//��������
	drawtext(_T("��    ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//������λ�ñ�Ǻ�ɫ����
	if (xtcs.bu_shu > 0)
	{
		setlinecolor(RED);
		rectangle(221 + (44 * (bstj[xtcs.bu_shu - 1].x - 1)), 93 + (44 * (bstj[xtcs.bu_shu - 1].y - 1)), 187 + (44 * (bstj[xtcs.bu_shu - 1].x - 1)), 59 + (44 * (bstj[xtcs.bu_shu - 1].y - 1)));
	}

	//���ý���
	if (xtcs.jie_mian == 1)
	{
		settextcolor(BLACK);//����������ɫ
		setlinecolor(BLACK);

		setfillcolor(RGB(115, 115, 115));//�������ɫ
		fillrectangle(512 + 300, 384 + 200, 512 - 300, 384 - 200);

		settextstyle(50, 0, _T("��������"));
		r = { 772 + 40, 224 + 40, 772 - 40, 224 - 40 };//��������
		drawtext(_T("��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		settextstyle(24, 0, _T("��������"));
		r = { 290 + 60, 300 + 20, 290 - 60, 300 - 20 };//��������
		drawtext(_T("ģʽѡ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 370 + 20, 290 - 60, 370 - 20 };//��������
		drawtext(_T("����˳��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 440 + 20, 290 - 60, 440 - 20 };//��������
		drawtext(_T(" A  I �Ѷȣ�"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 510 + 20, 290 - 60, 510 - 20 };//��������
		drawtext(_T(" A  I ��ɫ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		if (xtcs.mo_shi == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(420 - 50, 300 - 20, 420 + 50, 300 + 20);//��ӡ����
		r = { 420 - 50, 300 - 20, 420 + 50, 300 + 20 };//��������
		drawtext(_T("�˻���ս"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.mo_shi == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(550 - 50, 300 - 20, 550 + 50, 300 + 20);//��ӡ����
		r = { 550 - 50, 300 - 20, 550 + 50, 300 + 20 };//��������
		drawtext(_T("˫�˶�ս"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



		setfillcolor(RGB(165, 165, 165));//���ɫ

		fillrectangle(680 - 50, 300 - 20, 680 + 50, 300 + 20);//��ӡ����
		r = { 680 - 50, 300 - 20, 680 + 50, 300 + 20 };//��������
		drawtext(_T("�� �� ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.luo_zi_shun_xu == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(420 - 50, 370 - 20, 420 + 50, 370 + 20);//��ӡ����
		r = { 420 - 50, 370 - 20, 420 + 50, 370 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.luo_zi_shun_xu == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(550 - 50, 370 - 20, 550 + 50, 370 + 20);//��ӡ����
		r = { 550 - 50, 370 - 20, 550 + 50, 370 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(420 - 50, 440 - 20, 420 + 50, 440 + 20);//��ӡ����
		r = { 420 - 50, 440 - 20, 420 + 50, 440 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 7)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(550 - 50, 440 - 20, 550 + 50, 440 + 20);//��ӡ����
		r = { 550 - 50, 440 - 20, 550 + 50, 440 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 11)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(680 - 50, 440 - 20, 680 + 50, 440 + 20);//��ӡ����
		r = { 680 - 50, 440 - 20, 680 + 50, 440 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.huan_se == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(420 - 50, 510 - 20, 420 + 50, 510 + 20);//��ӡ����
		r = { 420 - 50, 510 - 20, 420 + 50, 510 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.huan_se == 2)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//���ɫ
		}
		fillrectangle(550 - 50, 510 - 20, 550 + 50, 510 + 20);//��ӡ����
		r = { 550 - 50, 510 - 20, 550 + 50, 510 + 20 };//��������
		drawtext(_T("��  ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	FlushBatchDraw();//������ͼ

	return;
}

void shu_biao_jiao_hu(MOUSEMSG m)
{
	RECT r;
	char s[50];

	// ���������Ϣ
	int tx = 182, ty = 54;//���Χ���Ͻǵ�����
	int w = 44;//ÿ��ĳ����Ϊ44
	int x, y;

	if (xtcs.jie_mian == 0)
	{
		switch (m.uMsg)
		{

		case WM_MOUSEMOVE:
			LOGFONT f;//������������Խṹ��
			gettextstyle(&f);//��ȡ��ǰ��������
			settextcolor(BLACK);//����������ɫ
			setbkmode(OPAQUE);//�������屳��Ϊ͸��
			f.lfQuality = ANTIALIASED_QUALITY;//���忹���
			settextstyle(&f);//��������
			sprintf(s, "Y:%d  X:%d    ", m.y, m.x);
			outtextxy(10, 20, s);
			//sprintf(s, "%d", m.y);
			//outtextxy(10, 40, s);
			setbkmode(TRANSPARENT);//�������屳��Ϊ͸��
			break;

		case WM_LBUTTONDOWN://�������
			if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>630 - 20 && m.y < 630 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 630 - 20, 940 + 50, 630 + 20);
				r = { 940 - 50, 630 - 20, 940 + 50, 630 + 20 };//��������
				drawtext(_T("��Ϸ����"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>560 - 20 && m.y < 560 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 560 - 20, 940 + 50, 560 + 20);
				r = { 940 - 50, 560 - 20, 940 + 50, 560 + 20 };//��������
				drawtext(_T("���¿�ʼ"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>490 - 20 && m.y < 490 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 490 - 20, 940 + 50, 490 + 20);
				r = { 940 - 50, 490 - 20, 940 + 50, 490 + 20 };//��������
				drawtext(_T("��    ��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			break;

		case WM_LBUTTONUP://�������
			da_yin_qi_pan();
			if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>630 - 20 && m.y < 630 + 20)
			{
				xtcs.jie_mian = 1;
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>560 - 20 && m.y < 560 + 20)
			{
				chong_xin_kai_shi();
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>490 - 20 && m.y < 490 + 20)
			{
				hui_qi(xtcs.mo_shi);
			}
			else if (m.x > 0 + tx && m.x < 660 + tx && m.y + ty>0 && m.y < 660 + ty)//616+44=660 //����������λ�����ж�����
			{
				x = (m.x - tx) / w;
				y = (m.y - ty) / w;
				if (qi_pan[y + 1][x + 1] == 0 && xtcs.sheng_fu == 0)
				{
					pan_ding_sheng_fu(xia_zi(xtcs.huan_se, y + 1, x + 1));
					da_yin_qi_pan();
					if (xtcs.mo_shi == 0 && xtcs.sheng_fu == 0)//ģʽΪ�˻�
					{
						if (xtcs.bu_shu < 3 || xtcs.nan_du == 0)
						{
							pan_ding_sheng_fu(ji_suan_zui_you_luo_zi1(fan_qi_se(xtcs.huan_se)));
						}
						else
						{
							pan_ding_sheng_fu(ji_suan_zui_you_luo_zi(fan_qi_se(xtcs.huan_se), fan_qi_se(xtcs.huan_se), 0));
						}
					}
					else if (xtcs.mo_shi == 1 && xtcs.sheng_fu == 0)//ģʽΪ˫��
					{
						xtcs.huan_se = fan_qi_se(xtcs.huan_se);
					}
				}
			}
			da_yin_qi_pan();//�������ʱ��ͼ
			break;

		case WM_RBUTTONDOWN://�Ҽ�����
	/*		if (m.x > 0 + tx && m.x < 660 + tx && m.y + ty>0 && m.y < 660 + ty)//616+44=660
			{
				x = (m.x - tx) / w;
				y = (m.y - ty) / w;
				if (qi_pan[y + 1][x + 1] == 0)
				{
					xia_zi(1, y + 1, x + 1);
					//ji_suan_zui_you_luo_zi(1);
				}
			}
	*/
			while (xtcs.sheng_fu == 0)
			{
				pan_ding_sheng_fu(ji_suan_zui_you_luo_zi1(xtcs.huan_se));
				xtcs.huan_se = fan_qi_se(xtcs.huan_se);
				aa();
				da_yin_qi_pan();
				if (xtcs.sheng_fu == 0)
				{
					pan_ding_sheng_fu(ji_suan_zui_you_luo_zi(xtcs.huan_se, xtcs.huan_se, 0));
					xtcs.huan_se = fan_qi_se(xtcs.huan_se);
				}
				aa();
				da_yin_qi_pan();


			}

			break;
		case WM_MBUTTONDOWN://�м䰴��
			x = (m.x - tx) / w;
			y = (m.y - ty) / w;
			if (qi_pan[y + 1][x + 1] == 0)
			{
				//	sprintf(s, "%lf      ", zhu_yao_tong_ji(y + 1, x + 1, xtcs.huan_se));
				//	outtextxy(10, 40, s);
				printf("%d\n", zhu_yao_tong_ji(y + 1, x + 1, xtcs.huan_se));
			}
			break;
		}
	}
	else//���������Ϊ0�����÷�֧
	{
		switch (m.uMsg)
		{
		case WM_LBUTTONUP://�������
			if (m.x > 772 - 25 && m.x < 772 + 25 && m.y>224 - 25 && m.y < 224 + 25)//����
			{
				xtcs.jie_mian = 0;
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>300 - 20 && m.y < 300 + 20)
			{
				if (xtcs.mo_shi != 0)
				{
					xtcs.mo_shi = 0;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>300 - 20 && m.y < 300 + 20)//˫��
			{
				if (xtcs.mo_shi != 1)
				{
					xtcs.mo_shi = 1;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 680 - 50 && m.x < 680 + 50 && m.y>300 - 20 && m.y < 300 + 20)//�Զ���
			{
				//�Զ���
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>370 - 20 && m.y < 370 + 20)//����˳����
			{
				if (xtcs.luo_zi_shun_xu != 1)
				{
					xtcs.luo_zi_shun_xu = 1;
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>370 - 20 && m.y < 370 + 20)//����˳��ر�
			{
				if (xtcs.luo_zi_shun_xu != 0)
				{
					xtcs.luo_zi_shun_xu = 0;
				}
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>440 - 20 && m.y < 440 + 20)//��
			{
				if (xtcs.nan_du != 0)
				{
					xtcs.nan_du = 0;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>440 - 20 && m.y < 440 + 20)//����
			{
				if (xtcs.nan_du != 7)
				{
					xtcs.nan_du = 7;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 680 - 50 && m.x < 680 + 50 && m.y>440 - 20 && m.y < 440 + 20)//����
			{
				if (xtcs.nan_du != 11)
				{
					xtcs.nan_du = 11;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>510 - 20 && m.y < 510 + 20)//�ֺ�
			{
				if (xtcs.huan_se != 1)
				{
					xtcs.huan_se = 1;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>510 - 20 && m.y < 510 + 20)//�ְ�
			{
				if (xtcs.huan_se != 2)
				{
					xtcs.huan_se = 2;
					chong_xin_kai_shi();
				}
			}
			da_yin_qi_pan();
			break;
		}
	}

	FlushBatchDraw();//������ͼ

	return;
}

int xia_zi(int qi_se, int y, int x)
{
	if (xtcs.sheng_fu == 0)
	{
		qi_pan[y][x] = qi_se;//����

		//�������Ӽ�¼
		bstj[xtcs.bu_shu].y = y;
		bstj[xtcs.bu_shu].x = x;
		bstj[xtcs.bu_shu].bu_shu = xtcs.bu_shu + 1;
		++xtcs.bu_shu;

		tian_jia_luo_zi_dian(y, x);
	}

	return wu_lian_pan_ding(y, x, qi_se);
}

int wu_lian_pan_ding(int y, int x, int qi_se)
{
	int i, j;
	int Ty = y, Tx = x;
	int shu_liang;
	//���ح��
	while (1)
	{
		if (qi_pan[Ty + 1][Tx] != qi_se)//�������������ѭ��
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//��ǰ����ʤ��;
				}
			}//�������ڵ�ǰ������ɫ�������ѭ��
			Ty = y, Tx = x;//��ʼ����ʱ����
			break;//����whileѭ��
		}
		++Ty;
	}

	//���-��
	while (1)
	{
		if (qi_pan[Ty][Tx + 1] != qi_se)//�������������ѭ��
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//��ǰ����ʤ��;
				}
			}//�������ڵ�ǰ������ɫ�������ѭ��
			Ty = y, Tx = x;//��ʼ����ʱ����
			break;//����whileѭ��
		}
		++Tx;
	}

	//���\��
	while (1)
	{
		if (qi_pan[Ty + 1][Tx + 1] != qi_se)//�������������ѭ��
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, --Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//��ǰ����ʤ��;
				}
			}//�������ڵ�ǰ������ɫ�������ѭ��
			Ty = y, Tx = x;//��ʼ����ʱ����
			break;//����whileѭ��
		}
		++Ty, ++Tx;
	}

	//���/��
	while (1)
	{
		if (qi_pan[Ty + 1][Tx - 1] != qi_se)//�������������ѭ��
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, ++Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//��ǰ����ʤ��;
				}
			}//�������ڵ�ǰ������ɫ�������ѭ��
			Ty = y, Tx = x;//��ʼ����ʱ����
			break;//����whileѭ��
		}
		++Ty, --Tx;
	}

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				return 0;//��δ��ʤ��
			}
		}
	}

	return 3;//ƽ��
}

void pan_ding_sheng_fu(int win)
{
	da_yin_qi_pan();
	if (win == 1)
	{
		xtcs.sheng_fu = 1;//�׷�ʤ��
		MessageBox(NULL, "               �׷�ʤ����\n�������¿�ʼ����������Ϸ", "five", MB_SYSTEMMODAL);
	}
	else if (win == 2)
	{
		xtcs.sheng_fu = 2;//�ڷ�ʤ��
		MessageBox(NULL, "               �ڷ�ʤ����\n�������¿�ʼ����������Ϸ", "five", MB_SYSTEMMODAL);
	}
	else if (win == 3)
	{
		xtcs.sheng_fu = 3;//ƽ��
		MessageBox(NULL, "               ƽ    �֣�\n�������¿�ʼ����������Ϸ", "five", MB_SYSTEMMODAL);
	}
	return;
}

void hui_qi(int mo_shi)
{
	if (xtcs.bu_shu > 0)
	{
		if (mo_shi == 1)//˫�˶�ս����
		{
			--xtcs.bu_shu;
			qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
			xtcs.huan_se = fan_qi_se(xtcs.huan_se);
			shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//ɾ�����ӵ�
		}
		else if (mo_shi == 0 && xtcs.sheng_fu != xtcs.huan_se)//��ֹ�˻���սʱ���ʤ���������bug
		{
			for (int i = 0; xtcs.bu_shu >= 1 && i < 2; ++i)
			{
				--xtcs.bu_shu;
				qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
				shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//ɾ�����ӵ�
			}
		}
		else
		{
			--xtcs.bu_shu;
			qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
			shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//ɾ�����ӵ�
		}
	}
	xtcs.sheng_fu = 0;//��ʤ������

	return;
}

void chong_xin_kai_shi(void)
{
	int i, j;
	for (i = 1; i < 16; ++i)//����ȫ�����̲�����
	{
		for (j = 1; j < 16; ++j)
		{
			qi_pan[i][j] = 0;
			luo_zi_dian[i][j] = 0;
		}
	}
	xtcs.bu_shu = 0;
	xtcs.sheng_fu = 0;
	if (xtcs.huan_se == 1 && xtcs.mo_shi == 0)//�ò�������1˵�����Գֺ�
	{
		xia_zi(2, 8, 8);
	}

	return;
}

void qi_zi_huan_se(int qi_se)
{
	if (xtcs.mo_shi == 0)
	{
		xtcs.huan_se = qi_se;
		chong_xin_kai_shi();
		if (fan_qi_se(xtcs.huan_se) == 2)
		{
			xia_zi(2, 8, 8);
		}
	}

	return;
}

void shun_xu(void)
{
	char s[5];
	RECT r;
	settextstyle(12, 0, _T("��Բ"));

	for (int i = 0; i < xtcs.bu_shu; ++i)
	{
		if (qi_pan[bstj[i].y][bstj[i].x] == 2)//���Ǻ�������ʾ��ɫ��������ʾ��ɫ
		{
			settextcolor(WHITE);//����������ɫ
		}
		else
		{
			settextcolor(BLACK);//����������ɫ
		}
		r = { 204 + (44 * (bstj[i].x - 1)) + 15, 76 + (44 * (bstj[i].y - 1)) + 15, 204 + (44 * (bstj[i].x - 1)) - 15, 76 + (44 * (bstj[i].y - 1)) - 15 };//��������
		sprintf(s, "%d", bstj[i].bu_shu);
		drawtext(_T(s), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}



//AI����
int zhu_yao_tong_ji(int y, int x, int qi_se)
{
	int i, j, s1, s2;
	FZHZ fzhzt1[4] = { 0,0,0,0 };
	FZHZ fzhzt2[4] = { 0,0,0,0 };
	int zui_gao_fent1, zui_gao_fent2, sum;

	shu_pai_ji_suan(&fzhzt1[0], y, x, qi_se, qi_se);
	heng_pai_ji_suan(&fzhzt1[1], y, x, qi_se, qi_se);
	fan_xie_pai_ji_suan(&fzhzt1[2], y, x, qi_se, qi_se);
	zheng_xie_pai_ji_suan(&fzhzt1[3], y, x, qi_se, qi_se);

	//*******************************************************


	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i��j��Ȳ����м���
			{
				continue;
			}

			s1 = 0, s2 = 0;//��ʼ��s
			if (fzhzt1[i].shu_liang >= 3 && fzhzt1[j].shu_liang >= 3)
			{
				s1 += (fzhzt1[i].shu_liang - fzhzt1[i].zu_dang);
				if (fzhzt1[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt1[i].shu_liang >= 3 && fzhzt1[i].rong_yu_zu_dang == 2 && fzhzt1[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt1[j].shu_liang - fzhzt1[j].zu_dang);
				if (fzhzt1[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt1[j].shu_liang >= 3 && fzhzt1[j].rong_yu_zu_dang == 2 && fzhzt1[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//��sδ�ﵽ6������ִ���·��ж����
				{
					continue;
				}

				if ((s1 + s2) == 6 && fzhzt1[i].shu_liang == 3 && fzhzt1[j].shu_liang == 3)
				{
					if (fzhzt1[i].fen_zhi < 80000)
					{
						fzhzt1[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt1[i].fen_zhi < 130000)
					{
						fzhzt1[i].fen_zhi = 130000;
					}
				}
			}
		}
	}






	//*******************************************************

	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt1[j].fen_zhi < fzhzt1[i].fen_zhi)
		{
			j = i;
		}
	}

	zui_gao_fent1 = (5000 + fzhzt1[j].fen_zhi);//�������������������ȼ�






	shu_pai_ji_suan(&fzhzt2[0], y, x, fan_qi_se(qi_se), qi_se);
	heng_pai_ji_suan(&fzhzt2[1], y, x, fan_qi_se(qi_se), qi_se);
	fan_xie_pai_ji_suan(&fzhzt2[2], y, x, fan_qi_se(qi_se), qi_se);
	zheng_xie_pai_ji_suan(&fzhzt2[3], y, x, fan_qi_se(qi_se), qi_se);

	//*******************************************************


	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i��j��Ȳ����м���
			{
				continue;
			}

			s1 = 0, s2 = 0;//��ʼ��s
			if (fzhzt2[i].shu_liang >= 3 && fzhzt2[j].shu_liang >= 3)
			{
				s1 += (fzhzt2[i].shu_liang - fzhzt2[i].zu_dang);
				if (fzhzt2[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt2[i].shu_liang >= 3 && fzhzt2[i].rong_yu_zu_dang == 2 && fzhzt2[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt2[j].shu_liang - fzhzt2[j].zu_dang);
				if (fzhzt2[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt2[j].shu_liang >= 3 && fzhzt2[j].rong_yu_zu_dang == 2 && fzhzt2[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//��sδ�ﵽ6������ִ���·��ж����
				{
					continue;
				}

				if ((s1 + s2) == 6 && fzhzt2[i].shu_liang == 3 && fzhzt2[j].shu_liang == 3)
				{
					if (fzhzt2[i].fen_zhi < 80000)
					{
						fzhzt2[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt2[i].fen_zhi < 100000)
					{
						fzhzt2[i].fen_zhi = 100000;
					}
				}
			}
		}
	}




	//*******************************************************

	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt2[j].fen_zhi < fzhzt2[i].fen_zhi)
		{
			j = i;
		}
	}

	zui_gao_fent2 = fzhzt2[j].fen_zhi;

	sum = zui_gao_fent1 > zui_gao_fent2 ? zui_gao_fent1 : zui_gao_fent2;

	for (i = 0; i < 4; ++i)
	{
		sum += (0.1 * (fzhzt1[i].fen_zhi > fzhzt2[i].fen_zhi ? fzhzt1[i].fen_zhi : fzhzt2[i].fen_zhi));
	}




	return sum;
}

int ji_suan_zui_you_luo_zi(int qi_se, int yuan_qi_se, int ci_shu)//di_gui_kong_zhi���ڿ��Ƶݹ�
{
	if (ci_shu == xtcs.nan_du)
	{
		return 0;
	}
	ZBTJ tzbtj[10];
	ZBTJ zbtj[200];
	ZBTJ t;
	zbtj[0].fen_zhi = 0;
	tzbtj[0].jie_guo = -1000000000;
	int y, x, i, j;
	int top = 1, bottom = 0;
	int len = 0;
	int tsheng_fu = 0;//��ʱʤ��
	int zui_you_jie_guo = fan_qi_se(yuan_qi_se), lin_shi_jie_guo = 0;

	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			if (luo_zi_dian[y][x] > 0 && qi_pan[y][x] == 0)
			{
				t.fen_zhi = zhu_yao_tong_ji(y, x, qi_se);
				t.y = y;
				t.x = x;
				len = cha_ru(zbtj, len, t);
			}
		}
	}


	if (ci_shu != 0)
	{
		if (qi_se != yuan_qi_se)
		{
			while (zbtj[bottom].fen_zhi == zbtj[top].fen_zhi)
			{
				++top;
			}

			if (top != 1)
			{
				for (i = bottom, j = top; i <= j - 1; ++i)
				{
					t.jie_guo = you_shi_zhi_tong_ji(zbtj[i].y, zbtj[i].x, qi_se);
					if (tzbtj[bottom].jie_guo < t.jie_guo)
					{
						top = bottom;
					}
					if (tzbtj[bottom].jie_guo <= t.jie_guo)
					{
						zbtj[i].jie_guo = t.jie_guo;
						tzbtj[top] = zbtj[i];
						if (top != 9)
						{
							++top;
						}
					}
				}
			}
			else
			{
				tzbtj[0] = zbtj[0];
			}
			srand((int)time(0));
			bottom = rand() % (top);

			tsheng_fu = xia_zi(qi_se, tzbtj[bottom].y, tzbtj[bottom].x);
			if (tsheng_fu == 0)
			{
				lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
				hui_qi(3);
				return lin_shi_jie_guo;
			}
			else//����Ϊ0��һ���н���ˣ���ݻؽ��
			{
				hui_qi(3);
				return tsheng_fu;
			}
		}
		else
		{
			int xian_zhi = zbtj[0].fen_zhi * 0.6;
			while (zbtj[bottom].fen_zhi > xian_zhi && bottom < 15)//��ɾ�������峤��Ϊ0������ѭ��
			{
				tsheng_fu = xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
				if (tsheng_fu == 0)
				{
					lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
					if (yuan_qi_se == lin_shi_jie_guo)
					{
						hui_qi(3);
						return lin_shi_jie_guo;
					}
					else if (lin_shi_jie_guo == 3)//������ȼ�Ϊqi_se,3,0,fan_qi_se   ������ȼ������ϱߵ�if�����͵ݻأ�������ȼ�����������else if����������Ҳ�ᱻ�ݻ�
					{
						zui_you_jie_guo = lin_shi_jie_guo;
					}
					else if (lin_shi_jie_guo == 0 && zui_you_jie_guo != 3)//ִ�е���������Ž����Ϊ3�Ŀ�����ֻ������fan_qi_se��0
					{
						zui_you_jie_guo = lin_shi_jie_guo;
					}
					hui_qi(3);
					++bottom;
				}
				else//����Ϊ0��һ���н���ˣ���ݻؽ��
				{
					hui_qi(3);
					return tsheng_fu;
				}
			}

			return zui_you_jie_guo;//�ݻ����Ž��
		}
	}
	else if (ci_shu == 0)//�״ε��ý���÷�֧
	{
		while (1)
		{
			bottom = 0;
			int xian_zhi = zbtj[0].fen_zhi * 0.6;
			while (bottom < len && zbtj[bottom].fen_zhi > xian_zhi && bottom < 15)//��ɾ�������峤��Ϊ0������ѭ��
			{
				tsheng_fu = xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);

				if (tsheng_fu == 0)
				{
					lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
					if (lin_shi_jie_guo == yuan_qi_se)//δ�����Ϊʤ��ֱ������
					{
						hui_qi(3);
						return xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
					}
					else if (lin_shi_jie_guo == 0 || lin_shi_jie_guo == 3)//δ�����Ϊƽ�ֻ�δ����򴢴浱ǰ���
					{
						zbtj[bottom].jie_guo = lin_shi_jie_guo;
					}
					else//δ�����Ϊʧ��ɾ��
					{
						len = shan_chu(zbtj, len, bottom);
						hui_qi(3);
						continue;//ɾ�����������ݻᵽ��ǰ��λ����������Ҫ��һ����ֱ�������������©���
					}
				}
				else if (tsheng_fu == yuan_qi_se)//���Ϊʤ��ֱ������
				{
					hui_qi(3);
					return xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
				}
				else if (tsheng_fu == 0 || tsheng_fu == 3)//δ�����Ϊƽ�ֻ�δ����򴢴浱ǰ���
				{
					zbtj[bottom].jie_guo = tsheng_fu;
				}
				else//���Ϊʧ��ɾ��
				{
					len = shan_chu(zbtj, len, bottom);
					hui_qi(3);
					continue;//ɾ�����������ݻᵽ��ǰ��λ����������Ҫ��һ����ֱ�������������©���
				}

				hui_qi(3);
				++bottom;
			}
			if (len != 0)
			{
				for (i = 0; i < len && (zbtj[0].fen_zhi * 0.8) < zbtj[i].fen_zhi; ++i)
				{
					if (zbtj[i].jie_guo == 3)
					{
						return xia_zi(qi_se, zbtj[i].y, zbtj[i].x);
					}
				}
				for (i = 0; i < len && (zbtj[0].fen_zhi * 0.8) < zbtj[i].fen_zhi; ++i)
				{
					if (zbtj[i].jie_guo == 0)
					{
						return xia_zi(qi_se, zbtj[i].y, zbtj[i].x);
					}
				}
			}
			return ji_suan_zui_you_luo_zi1(yuan_qi_se);
		}
	}
	exit(-1);//��ʱ
}

int cha_ru(ZBTJ* zbtj, int len, ZBTJ newzbtj)
{
	int low = 0, high = len, mid;
	while (high >= low)
	{
		mid = (high + low) / 2;
		if (newzbtj.fen_zhi > zbtj[mid].fen_zhi)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	for (int i = len; i >= low; --i)
	{
		zbtj[i + 1] = zbtj[i];
	}
	zbtj[low] = newzbtj;

	return ++len;
}

int shan_chu(ZBTJ* zbtj, int len, int d)//dΪɾ�����±�(dΪ0����ɾ�������ڵ�һ��Ԫ��)
{
	for (; d < len; ++d)
	{
		zbtj[d] = zbtj[d + 1];
	}
	--len;
	return len;
}

void kou_fen(int zu_dang, int y, int x, int* sum)//�۷�
{
	if (*sum >= 100000 && zu_dang == 0)//����������赲ʱ�����ȼ�
	{
		*sum += 20000;
	}
	if (zu_dang == 1)//һ�����赲�������Զ�
	{
		*sum = *sum / 2;
	}
	else if (zu_dang >= 2)//�������赲������
	{
		*sum = 0;
	}

	return;
}

int you_shi_zhi_tong_ji(int y, int x, int qi_se)//����ֵ
{
	int i, j;
	int sum1 = 0, sum2 = 0;
	qi_pan[y][x] = qi_se;

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				if (qi_pan[i + 1][j] == qi_se || qi_pan[i][j + 1] == qi_se || qi_pan[i + 1][j + 1] == qi_se || qi_pan[i - 1][j] == qi_se || qi_pan[i][j - 1] == qi_se || qi_pan[i - 1][j - 1] == qi_se || qi_pan[i - 1][j + 1] == qi_se || qi_pan[i + 1][j - 1] == qi_se)
				{
					sum1 += you_shi_zhi_ji_suan(i, j, 1);
				}
				if (qi_pan[i + 1][j] == fan_qi_se(qi_se) || qi_pan[i][j + 1] == fan_qi_se(qi_se) || qi_pan[i + 1][j + 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j] == fan_qi_se(qi_se) || qi_pan[i][j - 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j - 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j + 1] == fan_qi_se(qi_se) || qi_pan[i + 1][j - 1] == fan_qi_se(qi_se))
				{
					sum2 += you_shi_zhi_ji_suan(i, j, 2);
				}
			}
		}
	}
	qi_pan[y][x] = 0;

	if (qi_se == 1)
	{
		return sum1 -= sum2;
	}
	else
	{
		return sum2 -= sum1;
	}
}

int you_shi_zhi_ji_suan(int y, int x, int qi_se)
{
	int i, j, s1, s2;
	FZHZ fzhzt1[4] = { 0,0,0,0 };
	FZHZ fzhzt2[4] = { 0,0,0,0 };
	int sum;

	shu_pai_ji_suan(&fzhzt1[0], y, x, qi_se, qi_se);
	heng_pai_ji_suan(&fzhzt1[1], y, x, qi_se, qi_se);
	fan_xie_pai_ji_suan(&fzhzt1[2], y, x, qi_se, qi_se);
	zheng_xie_pai_ji_suan(&fzhzt1[3], y, x, qi_se, qi_se);

	shu_pai_ji_suan(&fzhzt2[0], y, x, fan_qi_se(qi_se), qi_se);
	heng_pai_ji_suan(&fzhzt2[1], y, x, fan_qi_se(qi_se), qi_se);
	fan_xie_pai_ji_suan(&fzhzt2[2], y, x, fan_qi_se(qi_se), qi_se);
	zheng_xie_pai_ji_suan(&fzhzt2[3], y, x, fan_qi_se(qi_se), qi_se);

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i��j��Ȳ����м���
			{
				continue;
			}

			s1 = 0, s2 = 0;//��ʼ��s
			if (fzhzt1[i].shu_liang >= 3 && fzhzt1[j].shu_liang >= 3)
			{
				s1 += (fzhzt1[i].shu_liang - fzhzt1[i].zu_dang);
				if (fzhzt1[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt1[i].shu_liang >= 3 && fzhzt1[i].rong_yu_zu_dang == 2 && fzhzt1[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt1[j].shu_liang - fzhzt1[j].zu_dang);
				if (fzhzt1[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //�������赲��Ȼ���Ϸ��赲�۹�������ֱ�Ӽӷ�
					if (fzhzt1[j].shu_liang >= 3 && fzhzt1[j].rong_yu_zu_dang == 2 && fzhzt1[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//��sδ�ﵽ6������ִ���·��ж����
				{
					continue;
				}

				if ((s1 + s2) == 6)
				{
					if (fzhzt1[i].fen_zhi < 80000)
					{
						fzhzt1[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt1[i].fen_zhi < 130000)
					{
						fzhzt1[i].fen_zhi = 130000;
					}
				}
			}
		}
	}





	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt1[j].fen_zhi < fzhzt1[i].fen_zhi)
		{
			j = i;
		}
	}

	sum = fzhzt1[j].fen_zhi;//������������һЩ���ȼ�

	for (i = 0; i < 4; ++i)
	{
		sum += (0.1 * (fzhzt1[i].fen_zhi > fzhzt2[i].fen_zhi ? fzhzt1[i].fen_zhi : fzhzt2[i].fen_zhi));
	}




	return sum;
}


void yu_ce_shu_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//���|���Ƿ���ԴﵽԤ����������
{
	while (qi_pan[y + 1][x] == qi_se || qi_pan[y + 1][x] == 0)
	{
		++y;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_heng_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//���-���Ƿ���ԴﵽԤ����������
{
	while (qi_pan[y][x + 1] == qi_se || qi_pan[y][x + 1] == 0)
	{
		++x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_fan_xie_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//���\���Ƿ���ԴﵽԤ����������
{
	while (qi_pan[y + 1][x + 1] == qi_se || qi_pan[y + 1][x + 1] == 0)
	{
		++y, ++x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y, --x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_zheng_xie_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//���/���Ƿ���ԴﵽԤ����������
{
	while (qi_pan[y + 1][x - 1] == qi_se || qi_pan[y + 1][x - 1] == 0)
	{
		++y, --x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y, ++x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}


void shu_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//���ح��
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//����
	yu_ce_shu_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty - 1][Tx] != 0)
		{
			if (qi_pan[Ty - 1][Tx] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//��ʼ����ʱ����

	//����
	yu_ce_shu_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty + 1][Tx] != 0)
		{
			if (qi_pan[Ty + 1][Tx] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//λ�ù���

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void heng_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//���-��
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//����
	yu_ce_heng_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty][Tx - 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty][Tx - 1] != 0)
		{
			if (qi_pan[Ty][Tx - 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1][Tx1 + 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}


	Ty = y, Tx = x;//��ʼ����ʱ����


	//����
	yu_ce_heng_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty][Tx + 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty][Tx + 1] != 0)
		{
			if (qi_pan[Ty][Tx + 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1][Tx1 - 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//λ�ù���

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void fan_xie_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//���\��
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//������
	yu_ce_fan_xie_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty, ++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty, --Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx - 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty, --Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty - 1][Tx - 1] != 0)
		{
			if (qi_pan[Ty - 1][Tx - 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1 + 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//��ʼ����ʱ����


	//������
	yu_ce_fan_xie_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty, --Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty, ++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx + 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty, ++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty + 1][Tx + 1] != 0)
		{
			if (qi_pan[Ty + 1][Tx + 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1 - 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//λ�ù���

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void zheng_xie_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//���/��
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//������
	yu_ce_zheng_xie_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty, --Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty, ++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx + 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty, ++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty - 1][Tx + 1] != 0)
		{
			if (qi_pan[Ty - 1][Tx + 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1 - 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//��ʼ����ʱ����


	//������
	yu_ce_zheng_xie_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty, ++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //����һ�߾�ͷ������
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty, --Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx - 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty, --Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//�Ϸ�if���������û��Ҫִ�и�������Ըĳ�else if
	{
		if (qi_pan[Ty + 1][Tx - 1] != 0)
		{
			if (qi_pan[Ty + 1][Tx - 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1 + 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//ʹ���ʹ���󲻻����С��λ
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//λ�ù���

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}



int ji_suan_zui_you_luo_zi1(int qi_se)
{
	ZBTJ tzbtj[10];
	ZBTJ zbtj[226];


	zbtj[0].fen_zhi = 0;
	int y, x, i, j;
	int top = 0, bottom = 0;
	int t;


	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			if (qi_pan[y][x] == 0)
			{
				t = zhu_yao_tong_ji(y, x, qi_se);
				if (zbtj[bottom].fen_zhi < t)
				{
					top = bottom;
					zbtj[top].fen_zhi = t;
				}
				if (zbtj[bottom].fen_zhi == t)
				{
					zbtj[top].fen_zhi = t;
					zbtj[top].x = x;
					zbtj[top].y = y;
					if (top != 255)
					{
						++top;
					}
				}
			}
		}
	}

	tzbtj[0].jie_guo = -1000000000;
	if (top != 1)
	{
		for (i = bottom, j = top; i <= j - 1; ++i)
		{
			t = you_shi_zhi_tong_ji(zbtj[i].y, zbtj[i].x, qi_se);
			if (tzbtj[bottom].jie_guo < t)
			{
				top = bottom;
			}
			if (tzbtj[bottom].jie_guo <= t)
			{
				zbtj[i].jie_guo = t;
				tzbtj[top] = zbtj[i];
				if (top != 9)
				{
					++top;
				}
			}
		}
	}
	else
	{
		tzbtj[0] = zbtj[0];
	}

	srand((int)time(0));
	bottom = rand() % (top);



	return xia_zi(qi_se, tzbtj[bottom].y, tzbtj[bottom].x);
}

int aa(void)//����ֵ
{
	int i, j;
	int sum1 = 0, sum2 = 0;

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				if (qi_pan[i + 1][j] == 1 || qi_pan[i][j + 1] == 1 || qi_pan[i + 1][j + 1] == 1 || qi_pan[i - 1][j] == 1 || qi_pan[i][j - 1] == 1 || qi_pan[i - 1][j - 1] == 1 || qi_pan[i - 1][j + 1] == 1 || qi_pan[i + 1][j - 1] == 1)
				{
					sum1 += you_shi_zhi_ji_suan(i, j, 1);

				}
				if (qi_pan[i + 1][j] == 2 || qi_pan[i][j + 1] == 2 || qi_pan[i + 1][j + 1] == 2 || qi_pan[i - 1][j] == 2 || qi_pan[i][j - 1] == 2 || qi_pan[i - 1][j - 1] == 2 || qi_pan[i - 1][j + 1] == 2 || qi_pan[i + 1][j - 1] == 2)
				{
					sum2 += you_shi_zhi_ji_suan(i, j, 2);

				}
			}
		}
	}
	printf("%d\n", xtcs.bu_shu);
	printf("2 = %d\n", sum2);
	printf("1 = %d\n\n", sum1);

	return sum1;
}


void gou_zao_lin_jie_biao(void)
{
	int x, y;

	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			lian_jie_guan_xi(y, x);
		}		
	}

	return;
}

void lian_jie_guan_xi(int y, int x)//AI�����ж����ӹ�ϵ
{
	if (y - 1 < 16 && y - 1 > 0)//��1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x);
		if (y - 2 < 16 && y - 2 > 0)//��2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x);
		}
	}

	if (y + 1 < 16 && y + 1 > 0)//��1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x);
		if (y + 2 < 16 && y + 2 > 0)//��2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x);
		}
	}

	if (x - 1 < 16 && x - 1 > 0)//��1
	{
		gou_zao_jie_dian(&ljb[y][x], y, x - 1);
		if (x - 2 < 16 && x - 2 > 0)//��2
		{
			gou_zao_jie_dian(&ljb[y][x], y, x - 2);
		}
	}

	if (x + 1 < 16 && x + 1 > 0)//��1
	{
		gou_zao_jie_dian(&ljb[y][x], y, x + 1);
		if (x + 2 < 16 && x + 2 > 0)//��2
		{
			gou_zao_jie_dian(&ljb[y][x], y, x + 2);
		}
	}

	if (y - 1 < 16 && y - 1 > 0 && x - 1 < 16 && x - 1 > 0)//����1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x - 1);
		if (y - 2 < 16 && y - 2 > 0 && x - 2 < 16 && x - 2 > 0)//����2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x - 2);
		}
	}

	if (y + 1 < 16 && y + 1 > 0 && x + 1 < 16 && x + 1 > 0)//����1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x + 1);
		if (y + 2 < 16 && y + 2 > 0 && x + 2 < 16 && x + 2 > 0)//����2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x + 2);
		}
	}

	if (y - 1 < 16 && y - 1 > 0 && x + 1 < 16 && x + 1 > 0)//����1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x + 1);
		if (y - 2 < 16 && y - 2 > 0 && x + 2 < 16 && x + 2 > 0)//����2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x + 2);
		}
	}

	if (y + 1 < 16 && y + 1 > 0 && x - 1 < 16 && x - 1 > 0)//����1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x - 1);
		if (y + 2 < 16 && y + 2 > 0 && x - 2 < 16 && x - 2 > 0)//����2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x - 2);
		}
	}

	return;
}

void gou_zao_jie_dian(LJB* ljb, int y, int x)//����ڵ�
{
	while (ljb->next)
	{
		ljb = ljb->next;
	}
	LJB* p = (LJB*)malloc(sizeof(LJB));
	if (!p)
	{
		exit(-1);
	}
	p->x = x;
	p->y = y;
	p->next = NULL;
	ljb->next = p;

	return;
}

void tian_jia_luo_zi_dian(int y, int x)//������ӵ�
{
	LJB* p = ljb[y][x].next;

	while (p)
	{
		luo_zi_dian[p->y][p->x]++;
		p = p->next;
	}

	return;
}

void shan_chu_luo_zi_dian(int y, int x)//ɾ�����ӵ�
{
	LJB* p = ljb[y][x].next;

	while (p)
	{
		luo_zi_dian[p->y][p->x]--;
		p = p->next;
	}

	return;
}