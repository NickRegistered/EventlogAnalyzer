#include "EventLog.h"
char over = 0;/*RunSys�������ж��Ƿ�������еı���,ֻ������ExitSysʱ�Ÿ���Ϊ1*/

unsigned long ul;

int main()
{
    COORD size = {SCR_COL , SCR_ROW};           /*���ڻ�������С*/
    WORD att = FOREGROUND_INTENSITY;
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);/*��ȡ��׼����豸���*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);  /*��ȡ��׼�����豸���*/

    SetConsoleTitle("��־��¼����ϵͳ");         /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out , size);/*���ô��ڻ�������С*/
    SetConsoleTextAttribute(gh_std_out , att);
    ClearWindow();                                /*����*/

    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char*)malloc(SCR_COL*SCR_ROW*sizeof(char));/*�����ַ�����*/
    gp_top_layer = (LAYER*)malloc(sizeof(LAYER));
    gp_top_layer->LayerNum = 0;                              /*�������ڲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL-1;
    gp_top_layer->rcArea.Bottom = SCR_ROW-1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;

    ShowMenu();         /*��ʾϵͳ���˵�*/
    RunSys();

    return 0;
}

/**
 * ��������: ShowMenu
 * ��������: ����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void ShowMenu(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    CONSOLE_CURSOR_INFO cci;
    COORD size;
    COORD pos = {0,0}, pos2={0,2};
    int i,j;
    int PosA = 2, PosB;
    char ch;

    GetConsoleScreenBufferInfo(gh_std_out , &csbi);
    size.X = csbi.dwSize.X;
    size.Y = 1;

    SetConsoleCursorPosition(gh_std_out , pos);
    for(i = 0; i<5; i++)        /*��һ�е�һ��������˵���*/
    {
        printf("  %s  ", main_menu[i]);
    }

    /*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
    buff_menubar_info = (CHAR_INFO*)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    SMALL_RECT rcMenu = {0, 0, size.X - 1,0};

    /*����һ�����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
    ReadConsoleOutput(gh_std_out, buff_menubar_info , size,pos , &rcMenu);
    /*����һ���ַ���Ԫ��Ϊ�׵׺���*/
    for(i=0; i<size.X ; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*�޸ĺ���ַ���Ϣ��д����һ��*/
    WriteConsoleOutput(gh_std_out, buff_menubar_info, size, pos, &rcMenu);
    TagMainMenu(gi_sel_menu);/*���ѡ�����˵�*/
    SetConsoleCursorPosition(gh_std_out, pos2);

    return;
}

/**
 * ��������: TagMainMenu
 * ��������: ��ָ�����˵�������ѡ�б�־.
 * �������: num ѡ�е����˵����
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD size;
    COORD pos={0,0};
    int PosA=2, PosB;
    char ch;
    int i;

    if(num == 0)/*numΪ0ʱȥ�����*/
    {
        PosA=0;
        PosB=0;
    }

    else    /*���򣬶�λѡ�����˵������ֹλ�ã�PosAΪ��ʼλ�ã�PosBΪ��ֹλ��*/
    {
        for(i=1; i<num; i++)
        {
            PosA += strlen(main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(main_menu[num-1]);
    }

    GetConsoleScreenBufferInfo(gh_std_out, &csbi);
    size.X = csbi.dwSize.X;
    size.Y = 1;

    /*ȥ��ǰ��˵���ı��*/
    for(i=0; i<PosA; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*���ѡ�в˵���ڵװ���*/
    for(i=PosA; i<PosB; i++)
    {
        (buff_menubar_info+i)->Attributes = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    }
    /*ȥ������˵���ı��*/
    for(i=PosB; i<size.X; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*�����ñ�ǵ���Ϣ��д����һ��*/
    SMALL_RECT rcMenu = {0,0,size.X-1,0};
    WriteConsoleOutput(gh_std_out,buff_menubar_info, size, pos, &rcMenu);

    return;
}

/**
 * ��������: ClearWindow
 * ��������: �������̨������Ļ��Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void ClearWindow(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;//����̨��Ļ������Ϣ
    COORD home={0,0};//��һ�е�һ��λ��
    unsigned long size;

    GetConsoleScreenBufferInfo(gh_std_out, &csbi);//��ñ�׼�������Ļ������Ϣ
    size = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputAttribute(gh_std_out, csbi.wAttributes, size, home, &ul);
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);//�ÿո����

    return;
}

/**
 * ��������: ClearScreen
 * ��������: ������˵����µ���Ļ��Ϣ.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void ClearScreen(void)
{
    unsigned long size;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD pos = {0,1}, home = {0,0};//pos��ʾ�ڶ��е�һ��λ��

    GetConsoleScreenBufferInfo(gh_std_out, &csbi);
    size = csbi.dwSize.X*(csbi.dwSize.Y - 1);//��Ļ��һ�����µĴ�С

    FillConsoleOutputAttribute(gh_std_out, csbi.wAttributes, size, pos, &ul);
    FillConsoleOutputCharacter(gh_std_out, ' ', size, pos, &ul);//�ÿո����
    SetConsoleCursorPosition(gh_std_out, home);//��������õ�homeλ�ã���ʾ���������Ĺ��ܽ�����������һ������ʾ���˵�
    SetConsoleCursorPosition(gh_std_out, pos);//��������õ�posλ��

    return;
}

/**
 * ��������: PopMenu
 * ��������: ����ָ�����˵����Ӧ���Ӳ˵�.
 * �������: num ָ�������˵����
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;

    if (num != gi_sel_menu)       /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (gp_top_layer->LayerNum != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNum != 0) /*���ѵ������Ӳ˵����򷵻�*/
    {
        return;
    }

    gi_sel_menu = num;    /*��ѡ�����˵�����Ϊָ�������˵���*/
    TagMainMenu(gi_sel_menu); /*��ѡ�е����˵����������*/
    LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/

    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += sub_menu_num[i-1];
    }
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = sub_menu_num[gi_sel_menu-1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD aLoc[labels.num];/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
    for (i=0; i<labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*����������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT aArea[areas.num];                    /*������������������λ��*/
    char aSort[areas.num];                      /*��������������������Ӧ���*/
    char aTag[areas.num];                         /*����������ÿ�������ı��*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*������λ*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1;           /*������˳����*/
    }
    areas.pArea = aArea;/*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;/*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;   /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*���������ڻ��߿�*/
    pos.X = rcPop.Left + 2;

    gi_sel_sub_menu = 0;
    return;
}

/**
 * ��������: PopUp
 * ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
 * �������: pRc ��������λ�����ݴ�ŵĵ�ַ
 *           att �������������ַ�����
 *           pLabel ���������б�ǩ����Ϣ��ŵĵ�ַ
             pHotArea ����������������Ϣ��ŵĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER *)malloc(sizeof(LAYER));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNum = gp_top_layer->LayerNum + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),pLabel->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNum;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)
                                    | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * ��������: PopOff
 * ��������: �رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void PopOff(void)
{
    LAYER *nextLayer;
    COORD size;
    COORD pos={0,0};
    char *pCh;
    int i, j;
    if((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {/*����Ϊ��������Ļ��Ϣʱ�����ùر�*/
        return;
    }

    nextLayer = gp_top_layer->next;

    /*�ָ��������ڸ�������ԭ���*/
    size.X = gp_top_layer->rcArea.Right-gp_top_layer->rcArea.Left+1;
    size.Y = gp_top_layer->rcArea.Bottom-gp_top_layer->rcArea.Top+1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));

    /*�ָ��ַ���Ԫ����*/
    pCh = gp_top_layer->pScrAtt;
    for(i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for(j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);/*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pScrAtt);
    gp_top_layer = nextLayer;
    return;
}

/**
 * ��������: DrawBox
 * ��������: ��ָ�����򻭱߿�.
 * �������: pRc �������λ����Ϣ�ĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};
    COORD pos = {pRc->Left,pRc->Top};/*��λ���������Ͻ�*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*�����Ͻ�*/

    for(pos.X=pRc->Left+1; pos.X<pRc->Right; pos.X++)/*���ϱ߿����*/
    {
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }

    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*�����Ͻ�*/

    for(pos.Y=pRc->Top+1; pos.Y<pRc->Bottom; pos.Y++)/*�����ұ߿����*/
    {
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1,pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }

    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*�����½�*/

    for(pos.X=pRc->Left+1; pos.X<pRc->Right; pos.X++)/*���±߿����*/
    {
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*�����½�*/

    return;
}

/**
 * ��������: TagSubMenu
 * ��������: ��ָ���Ӳ˵�������ѡ�б��.
 * �������: num ѡ�е��Ӳ˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    /*���㵯���Ӳ˵�������λ�ã������rcPop��*/
    LocSubMenu(gi_sel_menu, &rcPop);
    if((num<1) || (num==gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {/*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
        return;
    }

    pos.X = rcPop.Left+2;
    width = rcPop.Right-rcPop.Left-3;

    if(gi_sel_sub_menu != 0)/*ȡ��ԭѡ���Ӳ˵���ı��*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        /*�׵׺���*/
        att = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    }

    /*�ڶ����Ӳ˵����������*/
    pos.X = rcPop.Left+2;
    pos.Y = rcPop.Top+num;
    /*�ڵװ���*/
    att = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;/*�޸�ѡ���Ӳ˵����*/
    return;
}

/**
 * ��������: LocSubMenu
 * ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
 * �������: num ѡ�е����˵����
 * �������: rc �������λ����Ϣ�ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc=0;
    rc->Top = 1;    /*�����ϱ߶�λ�ڵڶ��У��к�Ϊ1*/
    rc->Left = 1;
    for(i=1; i<num; i++)
    {/*����������߽�λ�ú͵�һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(main_menu[i-1]) + 4;
        loc += sub_menu_num[i-1];
    }

    rc->Right = strlen(sub_menu[loc]);/*�ݴ��һ���Ӳ˵����ַ�������*/
    for(i=1; i<sub_menu_num[num-1]; i++)
    {/*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(sub_menu[loc+i]);
        if(rc->Right < len)
        {
            rc->Right = len;
        }
    }
            rc->Right += rc->Left+3;/*���������ұ߽�*/
            /*���������±ߵ��к�*/
        rc->Bottom = rc->Top + sub_menu_num[num-1]+1;
        if(rc->Right >= SCR_COL)/*�ұ߽�Խ�紦��*/
        {
            len = rc->Right - SCR_COL + 1;
            rc->Left -= len;
            rc->Right = SCR_COL - 1;
        }

    return;
}

/**
 * ��������: RunSys
 * ��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
 * �������: ��
 * �������: phead ����ͷָ��ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void RunSys()
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    InitStructures();

    while (over == 0)
    {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);

        if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/

            if (asc == 0) /*���Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNum == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {
                                gi_sel_menu = MENU_NUM;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > MENU_NUM)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40:
                            ClearScreen();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += sub_menu_num[i-1];
                    }  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {
                                gi_sel_menu = MENU_NUM;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)
                            {
                                num = sub_menu_num[gi_sel_menu-1];
                            }
                            if (strlen(sub_menu[loc+num-1]) == 0)
                            {
                                num--;
                            }
                            TagSubMenu(num);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > MENU_NUM)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > sub_menu_num[gi_sel_menu-1])
                            {
                                num = 1;
                            }
                            if (strlen(sub_menu[loc+num-1]) == 0)
                            {
                                num++;
                            }
                            TagSubMenu(num);
                            break;
                    }
                }
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNum != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        ExeFunction(4,1);
                        break;
                    case 77:  /*Alt+M*/
                        PopMenu(1);
                        break;
                    case 73: /*Alt+I*/
                        PopMenu(2);
                        break;
                    case 83: /*Alt+S*/
                        PopMenu(3);
                        break;
                    case 70: /*Alt+F*/
                        PopMenu(4);
                        break;
                }
            }

            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                if (gp_top_layer->LayerNum == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 13: /*�س�*/
                            ClearScreen();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }

                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        ExeFunction(gi_sel_menu, num);
                    }

                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += sub_menu_num[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+sub_menu_num[gi_sel_menu-1]; i++)
                        {
                            if (strlen(sub_menu[i])>0 && vkc==sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * ��������: ExeFunction
 * ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
 * �������: m ���˵����
 *           s �Ӳ˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ExeFunction(int m, int s)
{
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    status (*pFunction[13])();
    int i, loc;

    pFunction[0] = LoadFile;
    pFunction[1] = SaveAsTxt;
    pFunction[2] = ExitSys;

    pFunction[3] = Search_Event;
    pFunction[4] = Detail_Event;

    pFunction[5] = Insert_Event;
    pFunction[6] = Delete_Event;

    pFunction[7] = SetGraph;
    pFunction[8] = ShowVex;

    pFunction[9] = GetRules;
    pFunction[10] = ShowRules;
    pFunction[11] = InsertRule;
    pFunction[12] = DeleteRule;

    for(i=1, loc=0; i<m; i++)
    {//loc�ۼ����˵����Ӳ˵������Զ�λ�����ù���
        loc += sub_menu_num[i-1];
    }
    loc += s-1;//�����Ӳ˵����Եõ��û������õĹ��ܺ���
    if(pFunction[loc] != NULL)
    {
        ClearScreen();
        (*pFunction[loc])();
    }

}

/**
 * ��������: ShowFunctions
 * ��������: ��ʾÿ����ģ���µĹ��ܺ�������
 * �������: ��ģ���µĹ��ܺ������ơ��ַ�������
 * �������: ��ģ���µĹ��ܺ������ơ���ģ���¹��ܺ�������
 * �� �� ֵ: �������ľ���
 *
 * ����˵��:
 */
SMALL_RECT ShowFunctions(char *Functions[], short Num)
{
    COORD  pos;
    COORD size;
    WORD  att;
    SMALL_RECT rc_fun;
    short len;
    char  i, j, c='-';

    att = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;

    /*��ѭ�����ڼ���ѡ���ַ�����󳤶�*/
    len = strlen(Functions[0]);
    for(i=1; i<Num; i++)
    {
        if(strlen(Functions[i]) > len)
        {
            len = strlen(Functions[i]);
        }
    }

    /*��������С*/
    size.X = len + 6;
    size.Y = Num;

    rc_fun.Top = (SCR_ROW - Num)/2;
    rc_fun.Left = (SCR_COL - len - 7)/2;
    rc_fun.Bottom = rc_fun.Top + size.Y;
    rc_fun.Right = rc_fun.Left + size.X;

    pos.X = rc_fun.Left;
    pos.Y = rc_fun.Top;

    /*���õ�������ַ�����*/
    for(i=0; i<size.Y ;i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }

    pos.X = rc_fun.Left;
    pos.Y = rc_fun.Top;
    WriteConsoleOutputCharacter(gh_std_out, Functions[0], strlen(Functions[0]), pos, &ul);
    pos.Y ++;

    for(i=1; i<Num-1; i++)
    {
        pos.X = rc_fun.Left + 3;

        if(strlen(Functions[i]) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, Functions[i], strlen(Functions[i]), pos, &ul);
        }

        else
        {
            pos.X = rc_fun.Left;
            for(j=0; j<size.X; j++)
            {
                WriteConsoleOutputCharacter(gh_std_out, &c, 1, pos, &ul);
                pos.X ++;
            }

        }
        pos.Y++;
    }



    len = strlen("����");
    pos.X =rc_fun.Left + (size.X - len)/2;
    WriteConsoleOutputCharacter(gh_std_out, "����", len, pos, &ul);

    return rc_fun;
}

/**
 * ��������: ChooseFun
 * ��������: ѡ��ģ���еĺ���
 * �������: ģ��ѡ�������Σ�ѡ�����
 * �������: ѡ������
 * �� �� ֵ: ��
 * ����˵��:
 */
status ChooseFun(SMALL_RECT *rc,short Num)
{
    INPUT_RECORD inRec;
    WORD ATT, att;
    COORD pos;
    COORD size;
    short i;
    int choice;
    char vkc, asc, selected = 0;

    ATT = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;/*��ѡ��ʱ�ı���ַ�����*/
    att = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;                     /*δ��ѡ��ʱ���ַ�����*/

    size.X = rc->Right - rc->Left;
    size.Y = rc->Bottom - rc->Top;

    pos.X = rc->Left;
    pos.Y = rc->Top+2;

    /*��ǵ�һ��ѡ��*/
    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);
    choice = 1;

    while(!selected)/*��û��ѡ��ʱ*/
   {
        ReadConsoleInput(gh_std_in, &inRec, 1, &ul);
        if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                     && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
            {
                vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
                asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/

                if (asc == 0) /*���Ƽ��Ĵ���*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                        {
                            case 38:/*��*/
                                    FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);/*�ָ��������*/
                                    if(choice == Num + 1)
                                    {/*��ǰ���ڡ����ء��������������*/
                                        pos.Y-=2;
                                        choice--;
                                    }
                                    else if(choice == 1)
                                    {/*��ǰλ�ڵ�һ��*/
                                        pos.Y = rc->Bottom-1;
                                        choice = Num+1;
                                    }
                                    else
                                    {
                                        pos.Y--;
                                        choice--;
                                    }
                                    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);/*�����ѡ����*/
                                    break;

                            case 40:/*��*/
                                    FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);/*�ָ��������*/
                                    if(choice == Num + 1)
                                    {/*��ǰ���ڡ����ء���*/
                                        pos.Y = rc->Top + 2;
                                        choice = 1;
                                    }
                                    else if(choice == Num)
                                    {/*��ǰλ�����һ��ѡ�����Խ������*/
                                        pos.Y+=2;
                                        choice++;
                                    }
                                    else
                                    {
                                        pos.Y++;
                                        choice++;
                                    }
                                    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);/*�����ѡ����*/
                                    break;
                        }

                }

                else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                    switch (vkc)
                    {
                        case 13: /*�س�*/
                            selected = 1;
                            return choice;
                            break;
                    }
                }
            }
    }
}

/**
 * ��������: InitStructures
 * ��������: ��ʼ�����ݽṹ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status InitStructures()
{
    //��ʼ���¼���¼��
    Sts.List.elem = (EventData*)malloc(sizeof(EventData)*LIST_INIT_SIZE);
    if(NULL == Sts.List.elem)
    {//����ռ�ʧ��ʱ
        printf("�¼���¼��ռ�����ʧ�ܣ�\n");
        return ERROR;
    }
    printf("��ʼ���¼���¼��ɹ���\n");
    Sts.List.length = 0;//�¼���¼���ʼ����Ϊ0
    Sts.List.listsize = LIST_INIT_SIZE;

    //��ʼ���¼�ͼ
    InitGraph();

    return OK;
}

/**
 * ��������: InitGraph
 * ��������: ��ʼ��ͼ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status InitGraph()
{
    //��ʼ���¼�ͼ
    Sts.G.created = FALSE;//��ͼ����Ϊδ����
    Sts.G.vertices = (VNode*)malloc(sizeof(VNode)*MAX_VERTEX_NUM);//������ռ�
    if(NULL == Sts.G.vertices)
    {
        printf("���ռ�����ʧ�ܣ�\n");
        return ERROR;
    }
    for(int i = 0;i<MAX_VERTEX_NUM;i++) Sts.G.vertices[i].created = FALSE;//��ʼ�������״̬Ϊδ����
    printf("����ͼ��ʼ���ɹ���\n");
    Sts.G.vexnum = 0;//ͼ�н��������

    return OK;
}

/**
 * ��������: LoadFile
 * ��������: ���ü����ļ�����������ѡ���ϵͳ���ı��ļ�����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status LoadFile()
{
    status (*pFunction[2])();
    pFunction[0] = ReadFromSys;
    pFunction[1] = ReadFromTxt;

    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "������־�ļ�",
                        "",
                        "��ϵͳ��־����",
                        "���ı��ļ�����",
                        "",
                        "����",
                      };//���ع����µ��ӹ���������ʾ

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//��ʾ�ӹ���
    option = ChooseFun(rc, 2);//����û�ѡ��

    ClearScreen();

    int Ret = ERROR;
    if(option > 0 && option < 3 && pFunction[option] != NULL)
    {//����û�ѡ��Ϸ�
        Ret = (*pFunction[option-1])();
    }

    if(Ret != OK)
    {
        printf("��ȡ�¼�ʧ�ܣ�\n");
        return ERROR;
    }
}

/**
 * ��������: ReadFromSys
 * ��������: ��ϵͳ��ȡ��־
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status ReadFromSys()
{
    DWORD read_len, next_len;
    char Buffer[256], Data[512], *pchar;
    HANDLE Handle = OpenEventLog(NULL, "System");//���ϵͳ��־���
    char Name[64];

    Sts.List.length = 0;
    int counter = 0;

    if (Handle==NULL)
    {
        printf("��ϵͳ��־ʧ�ܣ�\n");
        return ERROR;
    }

    while(ReadEventLog( Handle,
                        EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,//��ʱ���ȡϵͳ��־
                        0,
                        (EVENTLOGRECORD*)Data,
                        sizeof(Data),
                        &read_len,
                        &next_len)//��ϵͳ��־��ȡ��¼
          )
    {
        for(int i=0; i<read_len && counter < ENTRY_NUM;)
        {

            EVENTLOGRECORD *ptr = (EVENTLOGRECORD*)(Data+i);
            Sts.List.elem[counter].Type = ptr->EventType;//�¼��ȼ�

            Sts.List.elem[counter].Categary = (short)ptr->EventCategory;//�¼�����

            Sts.List.elem[counter].ID = (unsigned short)ptr->EventID;//�¼�ID

            tm *ptm = (tm*)malloc(sizeof(tm));
            *ptm = *(localtime((const long *)&ptr->TimeWritten));//�¼�ʱ��
            Sts.List.elem[counter].Time = ptm;

            pchar = Data + sizeof(EVENTLOGRECORD); //pcharָ��SourceName[]
            strcpy(Sts.List.elem[counter].Source,pchar);

            pchar += strlen(pchar) + 1; //pcharָ��ComputerName[]
            strcpy(Sts.List.elem[counter].Computer,pchar);

            pchar += strlen(pchar) + 1;// pcharָ��UserSid
            if(ptr->UserSidLength)
            {
                DWORD Length = sizeof(Name), Length1 = sizeof(Buffer);
                SID_NAME_USE Type = SidTypeUser;
                SID *sid = (SID *)(Data + ptr->UserSidOffset);
                if(LookupAccountSid(NULL, sid, Name, &Length, Buffer, &Length1, &Type))  //�����û���
                    strcpy(Sts.List.elem[counter].User,Name);
                else//LookupAccountSid����ֵΪ��ʱ
                    strcpy(Sts.List.elem[counter].User,"��ȱ");
            }
            else
            {//UserSidLengthΪ0ʱ
                strcpy(Sts.List.elem[counter].User,"��ȱ");
            }

            if(ptr->DataOffset > ptr->StringOffset) //��ȡ�¼�����
            {
                strcpy(Sts.List.elem[counter].Dsp,"\0");//��ʼ���¼�����
                pchar = Data + i + ptr->StringOffset;
                for(short j = 0; j < ptr->NumStrings; j++)
                {
                    strcat(Sts.List.elem[counter].Dsp,pchar);
                    if(j < ptr->NumStrings-1)
                        strcat(Sts.List.elem[counter].Dsp," ");

                    pchar += strlen(pchar) + 1;
                }
                strcat(Sts.List.elem[counter].Dsp,"\n");
            }

            Sts.List.elem[counter].flag = 0;

            i+=ptr->Length;
            counter++;//�����¼�����+1
            Sts.List.length++;//�¼���¼��+1
        }
    }
       CloseEventLog(Handle);//�ر���־�ļ����

       printf("��ȡ�¼���¼�ɹ���\n");

       return OK;
}

/**
 * ��������: ReadFromTxt
 * ��������: ���ı��ļ������¼���Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status ReadFromTxt()
{
    FILE *fp;
    fp = fopen("EventLog.txt","r");
    if(NULL == fp)
    {
        printf("����־�ļ�ʧ�ܣ�\n");
        return ERROR;
    }

    Sts.List.length = 0;
    int i = 0;
    while(fscanf(fp,"%d",&Sts.List.elem[i].ID)== 1)
    {//���������һ���¼�ID
        fflush(stdin);
        tm *ptm = (tm*)malloc(sizeof(tm));
        fscanf(fp,"%d%d%d%d%d%d%d%d",//�����¼��ȼ�������ʱ��
                                        &Sts.List.elem[i].Type,
                                        &Sts.List.elem[i].Categary,
                                        &ptm->tm_year,
                                        &ptm->tm_mon,
                                        &ptm->tm_mday,
                                        &ptm->tm_hour,
                                        &ptm->tm_min,
                                        &ptm->tm_sec);
        fgetc(fp);
        //��ȡ�¼�Դ
        My_gets(Sts.List.elem[i].Source,fp);
        //��ȡ�¼�����
        My_gets(Sts.List.elem[i].Computer,fp);
        //��ȡ�¼��û�
        My_gets(Sts.List.elem[i].User,fp);
        //��ȡ�¼�����
        GetDsp(Sts.List.elem[i].Dsp,fp);

        ptm->tm_year -= 1900;//���û�����ʱ���������tmʱ�����͸�ʽһ��
        ptm->tm_mon -= 1;
        Sts.List.elem[i].Time = ptm;
        Sts.List.elem[i].flag = 0;//��ʼ��flag����
        Sts.List.length++;//�¼���¼�� ����+1
        i++;
    }

    printf("�ɹ���ȡ�¼���¼��\n");
    fclose(fp);

    VertexType temp;
    int id1, id2, weight, x;
    fp = fopen("EventGraph.txt","r");
    if(NULL == fp)
    {//������ļ�ʧ��
        printf("�򿪹���ͼ�ļ�ʧ�ܣ�\n");
        return ERROR;
    }

    if(fscanf(fp,"%d",&x) == 1)
    {//�������˱�ʾ�����Ŀ������ʱ
        Sts.G.created = TRUE;

        for(i=0;i<x;i++)
        {//��������Ϣ
            fscanf(fp,"%d %d",&temp.ID,&temp.num);
            InsertVex(temp);
        }

        while(fscanf(fp,"%d%d%d",&id1,&id2,&weight) == 3)
        {//���뻡����Ϣ���������ͼ
            InsertArc(id1,id2,weight);
        }
        printf("�ɹ���ȡ�¼�����ͼ��\n��������%d��\n",x);
    }

    fclose(fp);
    return OK;
}

/**
 * ��������: SaveAsTxt
 * ��������: ���¼���Ϣ���ı���ʽ����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status SaveAsTxt()
{
    FILE *fp;
    fp = fopen("EventLog.txt","w");
    if(NULL == fp)
    {//������ļ�ʧ��
        printf("����־�ļ�ʧ�ܣ�\n");
        return ERROR;
    }

    int i;
    for(i=0;i<Sts.List.length;i++)
    {//������־��¼
        fprintf(fp,"%d %d %d %.4d %.2d %.2d %.2d %.2d %.2d\n%s\n%s\n%s\n%s@\n",
                Sts.List.elem[i].ID,//�¼�ID
                Sts.List.elem[i].Type,//�¼�����
                Sts.List.elem[i].Categary,//�¼�����
                Sts.List.elem[i].Time->tm_year+1900, Sts.List.elem[i].Time->tm_mon+1,Sts.List.elem[i].Time->tm_mday,
                Sts.List.elem[i].Time->tm_hour, Sts.List.elem[i].Time->tm_min, Sts.List.elem[i].Time->tm_sec,//�¼�ʱ��
                Sts.List.elem[i].Source,Sts.List.elem[i].Computer,Sts.List.elem[i].User,//��������û�
                Sts.List.elem[i].Dsp);//�¼���������Ϣ
    }
    fclose(fp);
    printf("������־��¼�ɹ���\n");

    fp = fopen("EventGraph.txt","w");
    if(NULL == fp)
    {
        printf("�򿪹���ͼ�ļ�ʧ�ܣ�\n");
        return ERROR;
    }

    if(Sts.G.created == TRUE)
    {//��������˹���ͼ
        ArcNode *parc;

        fprintf(fp,"%d\n",Sts.G.vexnum);//д������Ŀ
        for(i=0;i<MAX_VERTEX_NUM;i++)
        {//д������Ϣ
            if(Sts.G.vertices[i].created == TRUE)
            fprintf(fp,"%d %d ",Sts.G.vertices[i].info.ID,Sts.G.vertices[i].info.num);
        }

        fputc('\n',fp);

        for(i=0;i<MAX_VERTEX_NUM;i++)
        {//д�뻡��Ϣ
            if(Sts.G.vertices[i].created == TRUE)
            {//���ͷ�ڵ�
                parc = Sts.G.vertices[i].firstarc;
                while(parc->nextarc)
                {//�����˽���µĻ�
                    if(parc->nextarc->weight != 0)//�������Ȩ�ز�Ϊ0����д�»�����Ϣ
                        fprintf(fp,"%d %d %d\n",Sts.G.vertices[i].info.ID,Sts.G.vertices[parc->nextarc->adjvex].info.ID,parc->nextarc->weight);
                    parc = parc->nextarc;
                }
            }
        }
    }

    fclose(fp);
    printf("�������ͼ�ɹ���\n");

    return OK;
}

/**
 * ��������: ExitSys()
 * ��������: �˳�ϵͳ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status ExitSys()
{
    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "�Ƿ񱣴��ļ���",
                        "",
                        "��",
                        "��",
                        "",
                        "����",
                      };//����������ʾ

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//������ʾ���ܺ���
    option = ChooseFun(rc, 2);//����û�ѡ��

    if(option == 1)
    {
        SaveAsTxt();
        over = 1;
    }
    else if(option == 2)
    {
        over = 1;
    }

    ClearScreen();
    return OK;
}

/**
 * ��������: Search_Event
 * ��������: ��ԭʼ�¼����в����¼�
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_Event()
{
    status (*pFunction[5])();
    pFunction[0] = Search_ID;
    pFunction[1] = Search_Time;
    pFunction[2] = Search_User;
    pFunction[3] = Search_Source;
    pFunction[4] = Search_Dsp;

    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "��Ϣ����",
                        "",
                        "���¼�ID����",
                        "���¼�ʱ�����",
                        "���¼��û�����",
                        "���¼���Դ����",
                        "���¼��������",
                        "",
                        "����",
                      };//����������ʾ

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 9);//������ʾ���ܺ���
    option = ChooseFun(rc, 5);//����û�ѡ��

    ClearScreen();
    fflush(stdin);

    if(option > 0 && option < 6 && pFunction[option] != NULL)
    {
        (*pFunction[option-1])();
    }
    return OK;
}

/**
 * ��������: Search_ID
 * ��������: �����¼�ID������������ͬID���¼�
            ���ɵõ����¼����¼����е���ţ����ڲ�ѯ��ϸ��Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_ID()
{
    int i;
    unsigned short ID;
    char find = 0;

    printf("��������Ҫ�������¼�ID:\n");
    scanf("%d",&ID);//����ID

    for(i=0; i<Sts.List.length; i++)
    {//˳������¼���¼
        if(Sts.List.elem[i].ID == ID)
        {//�¼���¼ID���û�����IDƥ��ʱ
            find = 1;//�޸ı�Ǳ���
            printf("���:%4d �¼�ID:%5d ʱ��:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd �û�:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//��ӡ�¼���Ϣ
        }
    }

    if(!find)
    {//��Ǳ���δ���޸�
        printf("�޷���Ҫ����¼�\n");
    }
    return OK;
}

/**
 * ��������: Search_Time
 * ��������: ������Ҫ������ʱ�䷶Χ����������Ҫ����¼����õ����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_Time()
{
    tm  *Start,*End;
    int i;
    char find = 0;

    Start = (tm*)malloc(sizeof(tm));
    End = (tm*)malloc(sizeof(tm));
    printf("�밴����������ʽ����ʱ��(yyyy/mm/dd hh:mm:ss)\n��2018��1��1�����������������2018/1/1 14:30:00\n");

    printf("����ʱ�䷶Χ���:");
    scanf("%d/%d/%d %d:%d:%d",//����ʱ��
                                 &Start->tm_year,
                                 &Start->tm_mon,
                                 &Start->tm_mday,
                                 &Start->tm_hour,
                                 &Start->tm_min,
                                 &Start->tm_sec);
    Start->tm_year -= 1900;
    Start->tm_mon -= 1;

    printf("����ʱ�䷶Χ�յ�:");
    scanf("%d/%d/%d %d:%d:%d",//����ʱ��
                                 &End->tm_year,
                                 &End->tm_mon,
                                 &End->tm_mday,
                                 &End->tm_hour,
                                 &End->tm_min,
                                 &End->tm_sec);
    End->tm_year -= 1900;//��ʱ���޸�����tmʱ�����͸�ʽһ��
    End->tm_mon -= 1;

    ClearScreen();
    /*��ϵͳ������¼���Ȼ��ʱ��Ϊ��*/
    for(i=0; i<Sts.List.length; i++)
    {
        if(T_cmp(End,Sts.List.elem[i].Time) <= 0)
        {//�������յ�����¼�ʱ����ֹͣ����
            find = 1;
            break;
        }
    }

    for(;i<Sts.List.length;i++)
    {
        if(T_cmp(Start,Sts.List.elem[i].Time) <= 0)//����������¼�ʱ����ֹͣ���
            break;
        printf("���:%4d �¼�ID:%5d ʱ��:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd �û�:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);
    }

    if(!find)
    {
        printf("�޷���Ҫ����¼�\n");
    }

    return OK;
}

/**
 * ��������: Search_User
 * ��������: �����û���User������������ͬ�û����¼�
            ���ɵõ����¼����¼����е���ţ����ڲ�ѯ��ϸ��Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_User()
{
    int i;
    char User[BufSize], find = 0;

    printf("��������Ҫ�������û�����");
    My_gets(User,stdin);//�����û���
    for(i=0; i<Sts.List.length; i++)
    {//˳������¼���¼
        if(strcmp(Sts.List.elem[i].User,User) == 0)
        {//���¼���¼�û�����ƥ��ʱ
            find = 1;//�޸ı�Ǳ���
            printf("���:%4d �¼�ID:%5d ʱ��:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd �û�:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//����¼���Ϣ
        }
    }

    if(!find)
    {//��Ǳ���δ���޸�ʱ
        printf("�޷���Ҫ����¼�\n");
    }
    return OK;

}

/**
 * ��������: Search_Source
 * ��������: �����¼�ԴSource�������¼�Դ�а���Source�ַ������¼�
            ���ɵõ����¼����¼����е���ţ����ڲ�ѯ��ϸ��Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_Source()
{
    int i;
    char Source[BufSize], find = 0;

    printf("��������Ҫ��������Դ��");
    My_gets(Source,stdin);//�����¼���Դ
    for(i=0; i<Sts.List.length; i++)
    {//˳������¼���¼
        if(KMP(Sts.List.elem[i].Source,strlen(Sts.List.elem[i].Source),Source,strlen(Source))!= -1)
        {//�û������ַ������¼���Դ�ַ������Ӵ�ʱ
            find = 1;//�޸ı�Ǳ���
            printf("���:%4d �¼�ID:%5d ʱ��:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd �û�:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//����¼���Ϣ
        }
    }

    if(!find)
    {//��Ǳ���δ���޸�ʱ
        printf("�޷���Ҫ����¼�\n");
    }
    return OK;

}

/**
 * ��������: Search_Dsp
 * ��������: �����������ݣ���������Ҫ����¼�������Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Search_Dsp()
{
    int i;
    char Dsp[BufSize], find = 0;

    printf("�������¼����壺");
    My_gets(Dsp,stdin);
    for(i=0; i<Sts.List.length; i++)
    {
        if(KMP(Sts.List.elem[i].Dsp,strlen(Sts.List.elem[i].Dsp),Dsp,strlen(Dsp))!= -1)
        {//�û�����������¼���¼�����Ӵ�ʱ
            find = 1;//�޸ı�Ǳ���
            printf("���:%4d �¼�ID:%5d ʱ��:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd �û�:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//����¼���Ϣ
        }
    }

    if(!find)
    {//��Ǳ���δ���޸�ʱ
        printf("�޷���Ҫ����¼�\n");
    }
    return OK;

}

/**
 * ��������: Detail_Event
 * ��������: ͨ���������ܵõ��¼����
            ����Ų�ѯ�¼���ϸ��Ϣ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Detail_Event()
{
    int num;
    char *pchar;

    printf("�������¼����:");
    scanf("%d",&num);

    if(num >= Sts.List.length)
    {//�û�����Ƿ����
        printf("������ų����¼�������\n");
        return ERROR;
    }
    //��ӡ�¼���ϸ��Ϣ
    printf("�¼�ID��  %5d\n",Sts.List.elem[num].ID);
    printf("�¼����%5d\n",Sts.List.elem[num].Categary);

    switch(Sts.List.elem[num].Type) //�¼�����
    {
        case EVENTLOG_SUCCESS:
            pchar= "�ɹ�";
            break;
        case EVENTLOG_ERROR_TYPE:
            pchar= "����";
            break;
        case EVENTLOG_WARNING_TYPE:
            pchar= "����";
            break;
        case EVENTLOG_INFORMATION_TYPE:
            pchar= "��Ϣ";
            break;
        case EVENTLOG_AUDIT_SUCCESS:
            pchar= "��Ƴɹ�";
            break;
        case EVENTLOG_AUDIT_FAILURE:
            pchar= "���ʧ��";
    }
    printf("�¼�����%s\n",pchar);

    printf("�¼�ʱ�䣺%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd\n",
                                                        Sts.List.elem[num].Time->tm_year+1900,
                                                        Sts.List.elem[num].Time->tm_mon+1,
                                                        Sts.List.elem[num].Time->tm_mday,
                                                        Sts.List.elem[num].Time->tm_hour,
                                                        Sts.List.elem[num].Time->tm_min,
                                                        Sts.List.elem[num].Time->tm_sec);
    printf("�¼�Դ��%s\n",Sts.List.elem[num].Source);
    printf("�û�����%s\n",Sts.List.elem[num].User);
    printf("�������%s\n",Sts.List.elem[num].Computer);
    printf("������%s",Sts.List.elem[num].Dsp);

}

/**
 * ��������: Insert_Event
 * ��������: �����¼���Ϣ�����뵽ԭʼ�¼�����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Insert_Event()
{
    /*��������¼���Ϣ*/
    EventData event;
    char Buffer[32];
    int i;

    tm *ptm;
    ptm = (tm*)malloc(sizeof(tm));

    fflush(stdin);//��ȡ�û��������ϸ�¼���Ϣ
    printf("�¼�ID��");
    scanf("%d",&event.ID);
    printf("�¼�����");
    getchar();
    My_gets(Buffer,stdin);
    if(strcmp(Buffer,"�ɹ�") == 0)
        event.Type = EVENTLOG_SUCCESS;
    else if(strcmp(Buffer,"��Ϣ") == 0)
        event.Type = EVENTLOG_INFORMATION_TYPE;
    else if(strcmp(Buffer,"����") == 0)
        event.Type = EVENTLOG_ERROR_TYPE;
    else if(strcmp(Buffer,"����") == 0)
        event.Type = EVENTLOG_WARNING_TYPE;
    else if(strcmp(Buffer,"��Ƴɹ�") == 0)
        event.Type = EVENTLOG_AUDIT_SUCCESS;
    else if(strcmp(Buffer,"���ʧ��") == 0)
        event.Type = EVENTLOG_AUDIT_FAILURE;

    printf("�¼����ࣺ");
    scanf("%d",&event.Categary);

    printf("�밴����������ʽ����ʱ��(yyyy/mm/dd hh:mm:ss)\n��2018��1��1�����������������2018/1/1 14:30:00\n");
    printf("�¼�ʱ�䣺");
    scanf("%d/%d/%d %d:%d:%d",//����ʱ��
                                 &ptm->tm_year,
                                 &ptm->tm_mon,
                                 &ptm->tm_mday,
                                 &ptm->tm_hour,
                                 &ptm->tm_min,
                                 &ptm->tm_sec);
    ptm->tm_year -= 1900;//��ʱ���޸�����tmʱ�����͸�ʽһ��
    ptm->tm_mon -= 1;
    event.Time = ptm;

    getchar();
    printf("�¼�Դ��");
    My_gets(event.Source,stdin);

    printf("�û�����");
    My_gets(event.User,stdin);

    printf("�������");
    My_gets(event.Computer,stdin);

    printf("�¼�������");
    My_gets(event.Dsp,stdin);

    if(Sts.List.length >= Sts.List.length)
    {//��ǰ�洢�ռ����������ӷ���
        EventData *newbase;
        newbase = (EventData *)realloc(Sts.List.elem, (Sts.List.listsize+LISTINCREMENT)*sizeof(EventData));
        if(!newbase) return(ERROR);    //�洢����ʧ��
        Sts.List.elem = newbase;               //�»�ַ
        Sts.List.listsize += LISTINCREMENT;    //���Ӵ洢����
    }

    for(i=Sts.List.length;i >= 0;i--)
    {
        if(T_cmp(event.Time,Sts.List.elem[i-1].Time) >= 0)//��������¼�ʱ�����ڵ�i���¼�ʱ��
        {//ֹͣ�ƶ�
            break;
        }
        Sts.List.elem[i] = Sts.List.elem[i-1];//��Ԫ���������1λ
    }

    Sts.List.length++;//��¼���+1
    Sts.List.elem[i] = event;//�ڿճ�����λ�ô������û������¼�

    return OK;
}

/**
 * ��������: Delete_Event
 * ��������: �����¼���Ϣ�����뵽ԭʼ�¼�����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Delete_Event()
{
    int i;
    printf("������Ҫɾ�����¼���ţ�");
    scanf("%d",&i);

    Sts.List.length--;//��¼���-1
    for(;i< Sts.List.length;i++)
    {//����Ҫɾ���¼�֮����¼�������ǰ�ƶ�
        Sts.List.elem[i] = Sts.List.elem[i + 1];
    }
    printf("�ɹ�ɾ���¼���\n");
    return OK;
}

/**
 * ��������: Get_Frequent_items
 * ��������: ��õ����¼�Ƶ���
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status Get_Frequent_items()
{
    InitGraph();

    int i,j;
    char find = 1;
    VertexType info;

    for(j=0;j<Sts.List.length;j++)
    {//��ʼ����Ǳ���
        Sts.List.elem[j].flag = 0;
    }

    for(i=0;find;)
    {//�����µ��¼�IDʱ
        find = 0;//���ò��ұ�Ǳ���
        info.num = 0;//��ʼ��������
        for(j=0;j<Sts.List.length;j++)
        {//ɨ���¼�������µ��¼�ID
            if(Sts.List.elem[j].flag == 0)
            {//����¼�δ�����������ID
                find = 1;
                Sts.List.elem[j].flag = 1;
                info.ID = Sts.List.elem[j].ID;
                break;
            }
        }
        for(;j<Sts.List.length;j++)
        {//ͳ�Ƹ�ID�Ĵ���
            if(Sts.List.elem[j].ID == info.ID)
            {
                Sts.List.elem[j].flag = 1;//����ͬID���¼����б�ǣ�������
                info.num++;
            }
        }

        if(info.num >= Sts.List.length * min_sup)
        {//�����ID�¼����ִ���������С֧�ֶ�,��ͼ�в���ڵ�
            InsertVex(info);
        }
    }

    Sts.G.created = TRUE;
    printf("��ȡƵ����ɹ���\n");

    return OK;
}

/**
 * ��������: SetGraph
 * ��������: ��������ͼ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status SetGraph()
{
    int Ret;
    Ret = Get_Frequent_items();

    if(Ret == OK)
    {
        int i,j;
        int t;

        for(i=0;i<Sts.List.length;i++)//��ʼ����Ǳ���
            Sts.List.elem[i].flag = 0;

        for(i=0;i<MAX_VERTEX_NUM;i++)
        {
            if(Sts.G.vertices[i].created == TRUE)//������Ѵ���ʱ
            {
                for(j=0;j<Sts.List.length;j++)
                {//�������ID��ͬ���¼���ͼ�н���ΪƵ���¼���
                    if(Sts.List.elem[j].ID == Sts.G.vertices[i].info.ID)
                     {
                         Sts.List.elem[j].flag = 1;
                     }
                }
            }
        }

        for(i = Sts.List.length - 1;i>=0;i--)
        {
            if(Sts.List.elem[i].flag == 0)//�����i���Ƶ����������
                continue;
            for(j=i-1;j>=0;j--)
            {
                if(Sts.List.elem[i].ID == Sts.List.elem[j].ID)//���ɨ������¼�ID�͵�ǰ�¼�ID��ͬ
                    break;

                if(T_cmp(Sts.List.elem[i].Time,Sts.List.elem[j].Time) > Delta_Time)//����¼�֮��ʱ������Delta_Time
                {
                    break;
                }

                if(Sts.List.elem[j].flag == 1)
                {//�������Ƶ�����ǣ������һ����
                    InsertArc(Sts.List.elem[i].ID,Sts.List.elem[j].ID,add);
                }
            }
        }
        printf("����ͼ������ɣ�\n");
    }
    return OK;
}

/**
 * ��������: ShowVex
 * ��������: ��ѯ����ͼ�н��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status ShowVex()
{
    int i;
    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//ɨ���¼�ͼ�ڽӱ�
        if(Sts.G.vertices[i].created == TRUE)//���״̬Ϊ�Ѵ���
            printf("�¼�ID��%5d ���ִ�����%3d\n",Sts.G.vertices[i].info.ID,Sts.G.vertices[i].info.num);//��������Ϣ
    }
    return OK;
}

/**
 * ��������: GetRules
 * ��������: ��ȡ��������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status GetRules()
{
    if(Sts.G.created == FALSE)
    {
        printf("��δ��������ͼ");
        return ERROR;
    }

    int i;
    ArcNode *parc,*temp;

    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//�������н��
        if(Sts.G.vertices[i].created == TRUE)
        {//������Ѵ���ʱ
            parc = Sts.G.vertices[i].firstarc;
            while(parc->nextarc)
            {//�����ý��Ļ�
                if(parc->nextarc->weight < Sts.G.vertices[i].info.num * confidence)
                    parc->nextarc->weight = 0;//�������ȨС��������Ŷ�����ȨΪ0

                parc = parc->nextarc;
            }
        }
    }
    printf("��ȡ��������ɹ���\n");
    return OK;
}

/**
 * ��������: ShowRules
 * ��������: ��ѯ��������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status ShowRules()
{
    int Ret;
    status (*pFunction[2])();
    pFunction[0] = forecast;
    pFunction[1] = trace;

    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "������־�ļ�",
                        "",
                        "�¼�Ԥ��",
                        "�¼�׷����Դ",
                        "",
                        "����",
                      };//��ʾ��������

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//������ʾ���ܺ���
    option = ChooseFun(rc, 2);//��ȡ�û�ѡ��

    ClearScreen();


    if(option > 0 && option < 3 && pFunction[option] != NULL)
    {
        Ret = (*pFunction[option-1])();
    }
}

/**
 * ��������: InsertVex
 * ��������: ���¼�ͼ�в����½��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status InsertVex(VertexType info)
{
    int index;
    int i = 0;

    do{//���Ŷ�ַ��
        if(i == MAX_VERTEX_NUM)
        {//���Ҵ��������������ʱ
            return ERROR;
        }

        index = Hash(info.ID + i);
        i++;//��indexָ���ĵ�ַ��ͻʱ,�������λ���Ѿ����������
    }while(Sts.G.vertices[index].created == TRUE);


    Sts.G.vertices[index].created = TRUE;//���õ���Ϊ�Ѵ���
    Sts.G.vertices[index].info = info;//д������Ϣ

    //��ʼ��ͷ�ڵ�
    ArcNode* head = (ArcNode*)malloc(sizeof(ArcNode));
    head->adjvex = -1;
    head->nextarc = NULL;
    head->weight = 0;
    Sts.G.vertices[index].firstarc = head;
    Sts.G.vexnum++;

    return OK;
}

/**
 * ��������: InsertArc
 * ��������: ���¼�ͼ�в��뻡��ָ��Ȩ�ء�Ȩ��Ϊ0ʱ������Ѵ��ڸû�������������Ȩֵ+1
 * �������: �����¼�ID
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status InsertArc(int id1,int id2,int weight)
{
    int i,index1,index2;

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//������Ҵ��������������
            printf("IDΪ%5d���¼������ڣ�\n",id1);
            return ERROR;
        }

        index1 = Hash(id1 + i);
        i++;
    }while(Sts.G.vertices[index1].info.ID != id1);//indexָ����λ�ô���¼�ID��id1����ʱ��������

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//������Ҵ��������������
            printf("IDΪ%5d���¼������ڣ�\n",id2);
            return ERROR;
        }

        index2 = Hash(id2 + i);
        i++;
    }while(Sts.G.vertices[index2].info.ID != id2);//indexָ����λ�ô���¼�ID��id1����ʱ��������

    ArcNode *parc,*tmp;
    parc = Sts.G.vertices[index1].firstarc;
    for(;parc->nextarc != NULL;parc = parc->nextarc)
    {
        if(parc->nextarc->adjvex == index2)//�������ָ��id2�¼��Ļ�
        {
            if(weight == add)//�������ʱָ������Ȩֵ����
                parc->nextarc->weight++;//��û�Ȩ+1
            else
                parc->nextarc->weight = weight;//����Ȩֵ����������
                break;
        }
    }

    if(parc->nextarc == NULL)
    {//���û��ָ��id2�¼��Ļ�,�����һ��ȨֵΪ1�Ļ�
        tmp = (ArcNode*)malloc(sizeof(ArcNode));
        tmp->adjvex = index2;
        if(weight == add)//�������ʱָ������Ȩֵ����
            tmp->weight = 1;//��û�Ȩ��ʼ��Ϊ1
        else
            tmp->weight = weight;//����Ȩֵ�����û�
        tmp->nextarc = Sts.G.vertices[index1].firstarc->nextarc;
        Sts.G.vertices[index1].firstarc->nextarc = tmp;
    }

    return OK;
}

/**
 * ��������: trace
 * ��������: ���ݹ�������׷����Դ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
 status trace()
 {
    int i,id;
    int find;
    ArcNode *parc;


    printf("�������¼���ID:");
    scanf("%d",&id);
    find = 0;//��ʼ�����ұ���
    printf("���ܵ���ID��%d�¼��������¼��У�\n",id);
    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//����ͼ�н��
        if(Sts.G.vertices[i].created == TRUE)
        {//�������Ѿ�����
            parc = Sts.G.vertices[i].firstarc;
            while(parc->nextarc)
            {//�������Ļ�
                if(Sts.G.vertices[parc->nextarc->adjvex].info.ID == id && parc->nextarc->weight > 0)
                {
                    find = 1;
                    printf("ID%6d\n",Sts.G.vertices[i].info.ID);

                }
                parc = parc->nextarc;
            }
        }
    }
    if(!find)printf("��\n");

    return OK;
 }

/**
 * ��������: forecast
 * ��������: ���ݹ�������Ԥ�⽫Ҫ�������¼�
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
 status forecast()
 {
    int i,id,index;
    int find;
    ArcNode *parc;

    printf("�������¼�ID:");
    scanf("%d",&id);

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//������Ҵ��������������
            printf("IDΪ%5d���¼������ڣ�\n",id);
            return ERROR;
        }

        index = Hash(id + i);
        i++;
    }while(Sts.G.vertices[index].info.ID != id);//indexָ����λ�ô���¼�ID��id1����ʱ��������

    find = 0;
    parc = Sts.G.vertices[index].firstarc;
    printf("��¼��ID��%d�¼����ܵ��µ��¼���:\n",id);
    while(parc->nextarc)
    {//�������л�
        if(parc->nextarc->weight > 0)
        {//��Ȩֵ��Ϊ0ʱ
            find = 1;//�޸ı�Ǳ���
            printf("ID:%6d\n",Sts.G.vertices[parc->nextarc->adjvex].info.ID);//����¼�ID
        }
        parc = parc->nextarc;
    }

    if(!find)printf("��\n");
    return OK;

 }

 /**
 * ��������: InsertRule
 * ��������: ���ϵͼ�в���һ����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status InsertRule()
{
    int id1,id2;
    printf("������Ҫ����Ĺ���(�¼�1ID->�¼�2ID)��\n�¼�1ID:");
    scanf("%d",&id1);

    printf("�¼�2ID:");
    scanf("%d",&id2);

    InsertArc(id1,id2,WEIGHT);//���û�����Ļ�����һ�������Ȩֵ

    printf("�����������ɹ���\n");
    return OK;
}

/**
 * ��������: DeleteRule
 * ��������: ɾ������ͼ�е�һ����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
status DeleteRule()
{
    int id1,id2;
    printf("������Ҫ����Ĺ���(�¼�1ID->�¼�2ID)��\n�¼�1ID:");
    scanf("%d",&id1);

    printf("�¼�2ID:");
    scanf("%d",&id2);

    InsertArc(id1,id2,0);//����Ҫɾ���Ļ�Ȩֵ��0

    printf("ɾ����������ɹ���\n");
    return OK;
}

/**
 * ��������: Hash
 * ��������: �����¼�ID�õ��¼��Ĵ洢λ��
 * �������: �¼�ID
 * �������: ��
 * �� �� ֵ: ID����Ӧ��λ��
 * ����˵��:
 */
int Hash(int ID)
{
    return (ID/1000 + ID%100)%MAX_VERTEX_NUM;
}

/**
 * ��������: T_cmp
 * ��������: �Ƚ�ʱ��ǰ�󣨾�ȷ���룩
 * �������: ����tm��ָ��
 * �������: ��
 * �� �� ֵ: ���Time1ʱ��������Time2�򷵻���ֵ��
            �������Time2�򷵻ظ�ֵ�����򷵻�0.
 * ����˵��:
 */
int T_cmp(tm* Time1,tm* Time2)
{
    int value;

    if(Time1->tm_year < Time2->tm_year)
        return 10000;
    else if(Time1->tm_year > Time2->tm_year)
        return -10000;
    else
    {//ʱ�������ͬʱ
        if(Time1->tm_mon < Time2->tm_mon)
            return 10000;
        else if(Time1->tm_mon > Time2->tm_mon)
            return -10000;
        else
        {//�·���ͬʱ
                value = (Time2->tm_sec - Time1->tm_sec)
                    + (Time2->tm_min - Time1->tm_min)*60
                    + (Time2->tm_hour - Time1->tm_hour)*60*60
                    + (Time2->tm_mday - Time1->tm_mday)*60*60*24;
                return value;
        }
    }
}

/**
 * ��������: My_gets
 * ��������: ���ļ��ж���һ����Ϣ��ȥ�����з�
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void My_gets(char* str,FILE* fp)
{
    char Buffer[BufSize];
    fgets(Buffer,BufSize,fp);
    Buffer[strlen(Buffer)-1] = '\0';
    strcpy(str,Buffer);
}

/**
 * ��������: GetDsp
 * ��������: ���ı���ȡ�¼�����
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void GetDsp(char* str,FILE* fp)
{
    char Dsp[DspLength];
    while(true){
        fgets(Dsp,DspLength,fp);
        if(Dsp[0] == '@')break;
        strcat(str,Dsp);
    }
    str++;
}

/**
 * ��������: cal_next
 * ��������: ��Ϊkmp�ĸ����㷨����ȡkmp��ptr����next����
 * �������: �ַ������ȣ�һ�����������Լ����ĳ���
 * �������: ��
 * �� �� ֵ: ��
 * ����˵��:
 */
void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]��ʼ��Ϊ-1��-1��ʾ��������ͬ�����ǰ׺������׺
    int k = -1;//k��ʼ��Ϊ-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//�����һ����ͬ����ôk�ͱ��next[k]��ע��next[k]��С��k�ģ�����kȡ�κ�ֵ��
        {
            k = next[k];//��ǰ����
        }
        if (str[k + 1] == str[q])//�����ͬ��k++
        {
            k = k + 1;
        }
        next[q] = k;//����ǰ����k��ֵ��������ͬ�����ǰ׺������׺��������next[q]
    }
}

/**
 * ��������: KMP
 * ��������: ����str����ptr�����ֵ�λ��
 * �������: str��str�ĳ��ȣ�ptr�Լ�ptr�ĳ���
 * �������: ��
 * �� �� ֵ: ptr��str���״γ��ֵ�λ�ã���û�г����򷵻�-1
 * ����˵��:
 */
int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);//����next����
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr��str��ƥ�䣬��k>-1����ʾptr��str�в���ƥ�䣩
            k = next[k];//��ǰ����
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//˵��k�ƶ���ptr����ĩ��
        {
            //cout << "��λ��" << i-plen+1<< endl;
            //k = -1;//���³�ʼ����Ѱ����һ��
            //i = i - plen + 1;//i��λ����λ�ã����forѭ��i++���Լ�������һ��������Ĭ�ϴ�������ƥ���ַ������Բ����ص�������л������ͬѧָ������
            return i-plen+1;//������Ӧ��λ��
        }
    }
    return -1;
}
