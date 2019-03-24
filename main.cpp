#include "EventLog.h"
char over = 0;/*RunSys中用于判断是否结束运行的变量,只有运行ExitSys时才更改为1*/

unsigned long ul;

int main()
{
    COORD size = {SCR_COL , SCR_ROW};           /*窗口缓冲区大小*/
    WORD att = FOREGROUND_INTENSITY;
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);/*获取标准输出设备句柄*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);  /*获取标准输入设备句柄*/

    SetConsoleTitle("日志记录分析系统");         /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out , size);/*设置窗口缓冲区大小*/
    SetConsoleTextAttribute(gh_std_out , att);
    ClearWindow();                                /*清屏*/

    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char*)malloc(SCR_COL*SCR_ROW*sizeof(char));/*窗口字符属性*/
    gp_top_layer = (LAYER*)malloc(sizeof(LAYER));
    gp_top_layer->LayerNum = 0;                              /*弹出窗口层号为0*/
    gp_top_layer->rcArea.Left = 0;
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL-1;
    gp_top_layer->rcArea.Bottom = SCR_ROW-1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;

    ShowMenu();         /*显示系统主菜单*/
    RunSys();

    return 0;
}

/**
 * 函数名称: ShowMenu
 * 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
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
    for(i = 0; i<5; i++)        /*第一行第一列输出主菜单项*/
    {
        printf("  %s  ", main_menu[i]);
    }

    /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
    buff_menubar_info = (CHAR_INFO*)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    SMALL_RECT rcMenu = {0, 0, size.X - 1,0};

    /*将第一行内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
    ReadConsoleOutput(gh_std_out, buff_menubar_info , size,pos , &rcMenu);
    /*将这一行字符单元置为白底黑字*/
    for(i=0; i<size.X ; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*修改后的字符信息回写到第一行*/
    WriteConsoleOutput(gh_std_out, buff_menubar_info, size, pos, &rcMenu);
    TagMainMenu(gi_sel_menu);/*标记选中主菜单*/
    SetConsoleCursorPosition(gh_std_out, pos2);

    return;
}

/**
 * 函数名称: TagMainMenu
 * 函数功能: 在指定主菜单项上置选中标志.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD size;
    COORD pos={0,0};
    int PosA=2, PosB;
    char ch;
    int i;

    if(num == 0)/*num为0时去除标记*/
    {
        PosA=0;
        PosB=0;
    }

    else    /*否则，定位选中主菜单项的起止位置：PosA为起始位置，PosB为截止位置*/
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

    /*去除前面菜单项的标记*/
    for(i=0; i<PosA; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*标记选中菜单项，黑底白字*/
    for(i=PosA; i<PosB; i++)
    {
        (buff_menubar_info+i)->Attributes = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    }
    /*去除后面菜单项的标记*/
    for(i=PosB; i<size.X; i++)
    {
        (buff_menubar_info+i)->Attributes = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
    }
    /*将做好标记的信息回写到第一行*/
    SMALL_RECT rcMenu = {0,0,size.X-1,0};
    WriteConsoleOutput(gh_std_out,buff_menubar_info, size, pos, &rcMenu);

    return;
}

/**
 * 函数名称: ClearWindow
 * 函数功能: 清楚控制台窗口屏幕信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void ClearWindow(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;//控制台屏幕缓存信息
    COORD home={0,0};//第一行第一列位置
    unsigned long size;

    GetConsoleScreenBufferInfo(gh_std_out, &csbi);//获得标准输出的屏幕缓存信息
    size = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputAttribute(gh_std_out, csbi.wAttributes, size, home, &ul);
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);//用空格填充

    return;
}

/**
 * 函数名称: ClearScreen
 * 函数功能: 清除主菜单条下的屏幕信息.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void ClearScreen(void)
{
    unsigned long size;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD pos = {0,1}, home = {0,0};//pos表示第二行第一列位置

    GetConsoleScreenBufferInfo(gh_std_out, &csbi);
    size = csbi.dwSize.X*(csbi.dwSize.Y - 1);//屏幕第一行以下的大小

    FillConsoleOutputAttribute(gh_std_out, csbi.wAttributes, size, pos, &ul);
    FillConsoleOutputCharacter(gh_std_out, ' ', size, pos, &ul);//用空格填充
    SetConsoleCursorPosition(gh_std_out, home);//将光标设置到home位置，显示过多行数的功能结束后跳到第一行以显示主菜单
    SetConsoleCursorPosition(gh_std_out, pos);//将光标设置到pos位置

    return;
}

/**
 * 函数名称: PopMenu
 * 函数功能: 弹出指定主菜单项对应的子菜单.
 * 输入参数: num 指定的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
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

    if (num != gi_sel_menu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNum != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNum != 0) /*若已弹出该子菜单，则返回*/
    {
        return;
    }

    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/

    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += sub_menu_num[i-1];
    }
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = sub_menu_num[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/
    pos.X = rcPop.Left + 2;

    gi_sel_sub_menu = 0;
    return;
}

/**
 * 函数名称: PopUp
 * 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
 * 输入参数: pRc 弹出窗口位置数据存放的地址
 *           att 弹出窗口区域字符属性
 *           pLabel 弹出窗口中标签束信息存放的地址
             pHotArea 弹出窗口中热区信息存放的地址
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*弹出窗口所在位置字符单元信息入栈*/
    size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER *)malloc(sizeof(LAYER));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNum = gp_top_layer->LayerNum + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),pLabel->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNum;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
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
 * 函数名称: PopOff
 * 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void PopOff(void)
{
    LAYER *nextLayer;
    COORD size;
    COORD pos={0,0};
    char *pCh;
    int i, j;
    if((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {/*顶层为主界面屏幕信息时，不用关闭*/
        return;
    }

    nextLayer = gp_top_layer->next;

    /*恢复弹出窗口覆盖区域原外观*/
    size.X = gp_top_layer->rcArea.Right-gp_top_layer->rcArea.Left+1;
    size.Y = gp_top_layer->rcArea.Bottom-gp_top_layer->rcArea.Top+1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));

    /*恢复字符单元属性*/
    pCh = gp_top_layer->pScrAtt;
    for(i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for(j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);/*释放动态存储区*/
    free(gp_top_layer->pScrAtt);
    gp_top_layer = nextLayer;
    return;
}

/**
 * 函数名称: DrawBox
 * 函数功能: 在指定区域画边框.
 * 输入参数: pRc 存放区域位置信息的地址
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};
    COORD pos = {pRc->Left,pRc->Top};/*定位在区域左上角*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画左上角*/

    for(pos.X=pRc->Left+1; pos.X<pRc->Right; pos.X++)/*画上边框横线*/
    {
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }

    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画右上角*/

    for(pos.Y=pRc->Top+1; pos.Y<pRc->Bottom; pos.Y++)/*画左右边框横线*/
    {
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1,pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }

    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画左下角*/

    for(pos.X=pRc->Left+1; pos.X<pRc->Right; pos.X++)/*画下边框横线*/
    {
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画右下角*/

    return;
}

/**
 * 函数名称: TagSubMenu
 * 函数功能: 在指定子菜单项上做选中标记.
 * 输入参数: num 选中的子菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    /*计算弹出子菜单的区域位置，存放在rcPop中*/
    LocSubMenu(gi_sel_menu, &rcPop);
    if((num<1) || (num==gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {/*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
        return;
    }

    pos.X = rcPop.Left+2;
    width = rcPop.Right-rcPop.Left-3;

    if(gi_sel_sub_menu != 0)/*取消原选中子菜单项的标记*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        /*白底黑字*/
        att = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    }

    /*在定制子菜单项上做标记*/
    pos.X = rcPop.Left+2;
    pos.Y = rcPop.Top+num;
    /*黑底白字*/
    att = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;/*修改选中子菜单项号*/
    return;
}

/**
 * 函数名称: LocSubMenu
 * 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: rc 存放区域位置信息的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc=0;
    rc->Top = 1;    /*区域上边定位在第二行，行号为1*/
    rc->Left = 1;
    for(i=1; i<num; i++)
    {/*计算区域左边界位置和第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(main_menu[i-1]) + 4;
        loc += sub_menu_num[i-1];
    }

    rc->Right = strlen(sub_menu[loc]);/*暂存第一个子菜单项字符串长度*/
    for(i=1; i<sub_menu_num[num-1]; i++)
    {/*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(sub_menu[loc+i]);
        if(rc->Right < len)
        {
            rc->Right = len;
        }
    }
            rc->Right += rc->Left+3;/*计算区域右边界*/
            /*计算区域下边的行号*/
        rc->Bottom = rc->Top + sub_menu_num[num-1]+1;
        if(rc->Right >= SCR_COL)/*右边界越界处理*/
        {
            len = rc->Right - SCR_COL + 1;
            rc->Left -= len;
            rc->Right = SCR_COL - 1;
        }

    return;
}

/**
 * 函数名称: RunSys
 * 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
 * 输入参数: 无
 * 输出参数: phead 主链头指针的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void RunSys()
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    InitStructures();

    while (over == 0)
    {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);

        if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                 && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

            if (asc == 0) /*控制键的处理*/
            {
                if (gp_top_layer->LayerNum == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
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
                else  /*已弹出子菜单时*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += sub_menu_num[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
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
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
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

            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                if (gp_top_layer->LayerNum == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 13: /*回车*/
                            ClearScreen();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }

                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        ExeFunction(gi_sel_menu, num);
                    }

                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += sub_menu_num[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=loc; i<loc+sub_menu_num[gi_sel_menu-1]; i++)
                        {
                            if (strlen(sub_menu[i])>0 && vkc==sub_menu[i][1])
                            { /*如果匹配成功*/
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
 * 函数名称: ExeFunction
 * 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
 * 输入参数: m 主菜单项号
 *           s 子菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ExeFunction(int m, int s)
{
    /*函数指针数组，用来存放所有功能函数的入口地址*/
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
    {//loc累加主菜单中子菜单个数以定位所调用功能
        loc += sub_menu_num[i-1];
    }
    loc += s-1;//加上子菜单数以得到用户所调用的功能函数
    if(pFunction[loc] != NULL)
    {
        ClearScreen();
        (*pFunction[loc])();
    }

}

/**
 * 函数名称: ShowFunctions
 * 函数功能: 显示每个子模块下的功能函数名称
 * 输入参数: 子模块下的功能函数名称、字符串个数
 * 输出参数: 子模块下的功能函数名称、子模块下功能函数个数
 * 返 回 值: 所在面板的矩形
 *
 * 调用说明:
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

    /*此循环用于计算选项字符串最大长度*/
    len = strlen(Functions[0]);
    for(i=1; i<Num; i++)
    {
        if(strlen(Functions[i]) > len)
        {
            len = strlen(Functions[i]);
        }
    }

    /*弹出面板大小*/
    size.X = len + 6;
    size.Y = Num;

    rc_fun.Top = (SCR_ROW - Num)/2;
    rc_fun.Left = (SCR_COL - len - 7)/2;
    rc_fun.Bottom = rc_fun.Top + size.Y;
    rc_fun.Right = rc_fun.Left + size.X;

    pos.X = rc_fun.Left;
    pos.Y = rc_fun.Top;

    /*设置弹出面板字符属性*/
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



    len = strlen("返回");
    pos.X =rc_fun.Left + (size.X - len)/2;
    WriteConsoleOutputCharacter(gh_std_out, "返回", len, pos, &ul);

    return rc_fun;
}

/**
 * 函数名称: ChooseFun
 * 函数功能: 选择模块中的函数
 * 输入参数: 模块选择面板矩形，选项个数
 * 输出参数: 选择的序号
 * 返 回 值: 无
 * 调用说明:
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

    ATT = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;/*被选中时的标记字符属性*/
    att = BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;                     /*未被选中时的字符属性*/

    size.X = rc->Right - rc->Left;
    size.Y = rc->Bottom - rc->Top;

    pos.X = rc->Left;
    pos.Y = rc->Top+2;

    /*标记第一个选项*/
    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);
    choice = 1;

    while(!selected)/*当没有选中时*/
   {
        ReadConsoleInput(gh_std_in, &inRec, 1, &ul);
        if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                     && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
            {
                vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
                asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

                if (asc == 0) /*控制键的处理*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
                        {
                            case 38:/*上*/
                                    FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);/*恢复被标记项*/
                                    if(choice == Num + 1)
                                    {/*当前处于“返回”项，向上跳过空行*/
                                        pos.Y-=2;
                                        choice--;
                                    }
                                    else if(choice == 1)
                                    {/*当前位于第一项*/
                                        pos.Y = rc->Bottom-1;
                                        choice = Num+1;
                                    }
                                    else
                                    {
                                        pos.Y--;
                                        choice--;
                                    }
                                    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);/*标记新选中项*/
                                    break;

                            case 40:/*下*/
                                    FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);/*恢复被标记项*/
                                    if(choice == Num + 1)
                                    {/*当前处于“返回”项*/
                                        pos.Y = rc->Top + 2;
                                        choice = 1;
                                    }
                                    else if(choice == Num)
                                    {/*当前位于最后一个选项，向下越过空行*/
                                        pos.Y+=2;
                                        choice++;
                                    }
                                    else
                                    {
                                        pos.Y++;
                                        choice++;
                                    }
                                    FillConsoleOutputAttribute(gh_std_out, ATT, size.X, pos, &ul);/*标记新选中项*/
                                    break;
                        }

                }

                else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                    switch (vkc)
                    {
                        case 13: /*回车*/
                            selected = 1;
                            return choice;
                            break;
                    }
                }
            }
    }
}

/**
 * 函数名称: InitStructures
 * 函数功能: 初始化数据结构
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status InitStructures()
{
    //初始化事件记录表
    Sts.List.elem = (EventData*)malloc(sizeof(EventData)*LIST_INIT_SIZE);
    if(NULL == Sts.List.elem)
    {//申请空间失败时
        printf("事件记录表空间申请失败！\n");
        return ERROR;
    }
    printf("初始化事件记录表成功！\n");
    Sts.List.length = 0;//事件记录表初始长度为0
    Sts.List.listsize = LIST_INIT_SIZE;

    //初始化事件图
    InitGraph();

    return OK;
}

/**
 * 函数名称: InitGraph
 * 函数功能: 初始化图
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status InitGraph()
{
    //初始化事件图
    Sts.G.created = FALSE;//将图设置为未创建
    Sts.G.vertices = (VNode*)malloc(sizeof(VNode)*MAX_VERTEX_NUM);//申请结点空间
    if(NULL == Sts.G.vertices)
    {
        printf("结点空间申请失败！\n");
        return ERROR;
    }
    for(int i = 0;i<MAX_VERTEX_NUM;i++) Sts.G.vertices[i].created = FALSE;//初始化各结点状态为未创建
    printf("关联图初始化成功！\n");
    Sts.G.vexnum = 0;//图中结点数置零

    return OK;
}

/**
 * 函数名称: LoadFile
 * 函数功能: 调用加载文件函数，可以选择从系统或文本文件加载
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status LoadFile()
{
    status (*pFunction[2])();
    pFunction[0] = ReadFromSys;
    pFunction[1] = ReadFromTxt;

    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "加载日志文件",
                        "",
                        "从系统日志加载",
                        "从文本文件加载",
                        "",
                        "返回",
                      };//加载功能下的子功能文字显示

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//显示子功能
    option = ChooseFun(rc, 2);//获得用户选项

    ClearScreen();

    int Ret = ERROR;
    if(option > 0 && option < 3 && pFunction[option] != NULL)
    {//如果用户选项合法
        Ret = (*pFunction[option-1])();
    }

    if(Ret != OK)
    {
        printf("读取事件失败！\n");
        return ERROR;
    }
}

/**
 * 函数名称: ReadFromSys
 * 函数功能: 从系统抽取日志
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status ReadFromSys()
{
    DWORD read_len, next_len;
    char Buffer[256], Data[512], *pchar;
    HANDLE Handle = OpenEventLog(NULL, "System");//获得系统日志句柄
    char Name[64];

    Sts.List.length = 0;
    int counter = 0;

    if (Handle==NULL)
    {
        printf("打开系统日志失败！\n");
        return ERROR;
    }

    while(ReadEventLog( Handle,
                        EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,//逆时序读取系统日志
                        0,
                        (EVENTLOGRECORD*)Data,
                        sizeof(Data),
                        &read_len,
                        &next_len)//从系统日志读取记录
          )
    {
        for(int i=0; i<read_len && counter < ENTRY_NUM;)
        {

            EVENTLOGRECORD *ptr = (EVENTLOGRECORD*)(Data+i);
            Sts.List.elem[counter].Type = ptr->EventType;//事件等级

            Sts.List.elem[counter].Categary = (short)ptr->EventCategory;//事件类型

            Sts.List.elem[counter].ID = (unsigned short)ptr->EventID;//事件ID

            tm *ptm = (tm*)malloc(sizeof(tm));
            *ptm = *(localtime((const long *)&ptr->TimeWritten));//事件时间
            Sts.List.elem[counter].Time = ptm;

            pchar = Data + sizeof(EVENTLOGRECORD); //pchar指向SourceName[]
            strcpy(Sts.List.elem[counter].Source,pchar);

            pchar += strlen(pchar) + 1; //pchar指向ComputerName[]
            strcpy(Sts.List.elem[counter].Computer,pchar);

            pchar += strlen(pchar) + 1;// pchar指向UserSid
            if(ptr->UserSidLength)
            {
                DWORD Length = sizeof(Name), Length1 = sizeof(Buffer);
                SID_NAME_USE Type = SidTypeUser;
                SID *sid = (SID *)(Data + ptr->UserSidOffset);
                if(LookupAccountSid(NULL, sid, Name, &Length, Buffer, &Length1, &Type))  //查找用户名
                    strcpy(Sts.List.elem[counter].User,Name);
                else//LookupAccountSid返回值为空时
                    strcpy(Sts.List.elem[counter].User,"暂缺");
            }
            else
            {//UserSidLength为0时
                strcpy(Sts.List.elem[counter].User,"暂缺");
            }

            if(ptr->DataOffset > ptr->StringOffset) //获取事件描述
            {
                strcpy(Sts.List.elem[counter].Dsp,"\0");//初始化事件描述
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
            counter++;//读入事件个数+1
            Sts.List.length++;//事件记录表长+1
        }
    }
       CloseEventLog(Handle);//关闭日志文件句柄

       printf("读取事件记录成功！\n");

       return OK;
}

/**
 * 函数名称: ReadFromTxt
 * 函数功能: 从文本文件读入事件信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status ReadFromTxt()
{
    FILE *fp;
    fp = fopen("EventLog.txt","r");
    if(NULL == fp)
    {
        printf("打开日志文件失败！\n");
        return ERROR;
    }

    Sts.List.length = 0;
    int i = 0;
    while(fscanf(fp,"%d",&Sts.List.elem[i].ID)== 1)
    {//如果读入了一个事件ID
        fflush(stdin);
        tm *ptm = (tm*)malloc(sizeof(tm));
        fscanf(fp,"%d%d%d%d%d%d%d%d",//读入事件等级，类别和时间
                                        &Sts.List.elem[i].Type,
                                        &Sts.List.elem[i].Categary,
                                        &ptm->tm_year,
                                        &ptm->tm_mon,
                                        &ptm->tm_mday,
                                        &ptm->tm_hour,
                                        &ptm->tm_min,
                                        &ptm->tm_sec);
        fgetc(fp);
        //读取事件源
        My_gets(Sts.List.elem[i].Source,fp);
        //读取事件主机
        My_gets(Sts.List.elem[i].Computer,fp);
        //读取事件用户
        My_gets(Sts.List.elem[i].User,fp);
        //读取事件描述
        GetDsp(Sts.List.elem[i].Dsp,fp);

        ptm->tm_year -= 1900;//将用户输入时间调整到与tm时间类型格式一致
        ptm->tm_mon -= 1;
        Sts.List.elem[i].Time = ptm;
        Sts.List.elem[i].flag = 0;//初始化flag变量
        Sts.List.length++;//事件记录表 长度+1
        i++;
    }

    printf("成功读取事件记录！\n");
    fclose(fp);

    VertexType temp;
    int id1, id2, weight, x;
    fp = fopen("EventGraph.txt","r");
    if(NULL == fp)
    {//如果打开文件失败
        printf("打开关联图文件失败！\n");
        return ERROR;
    }

    if(fscanf(fp,"%d",&x) == 1)
    {//当读入了表示结点数目的整数时
        Sts.G.created = TRUE;

        for(i=0;i<x;i++)
        {//读入结点信息
            fscanf(fp,"%d %d",&temp.ID,&temp.num);
            InsertVex(temp);
        }

        while(fscanf(fp,"%d%d%d",&id1,&id2,&weight) == 3)
        {//读入弧的信息并插入关联图
            InsertArc(id1,id2,weight);
        }
        printf("成功读取事件关联图！\n结点个数：%d！\n",x);
    }

    fclose(fp);
    return OK;
}

/**
 * 函数名称: SaveAsTxt
 * 函数功能: 将事件信息以文本形式保存
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status SaveAsTxt()
{
    FILE *fp;
    fp = fopen("EventLog.txt","w");
    if(NULL == fp)
    {//如果打开文件失败
        printf("打开日志文件失败！\n");
        return ERROR;
    }

    int i;
    for(i=0;i<Sts.List.length;i++)
    {//保存日志记录
        fprintf(fp,"%d %d %d %.4d %.2d %.2d %.2d %.2d %.2d\n%s\n%s\n%s\n%s@\n",
                Sts.List.elem[i].ID,//事件ID
                Sts.List.elem[i].Type,//事件类型
                Sts.List.elem[i].Categary,//事件种类
                Sts.List.elem[i].Time->tm_year+1900, Sts.List.elem[i].Time->tm_mon+1,Sts.List.elem[i].Time->tm_mday,
                Sts.List.elem[i].Time->tm_hour, Sts.List.elem[i].Time->tm_min, Sts.List.elem[i].Time->tm_sec,//事件时间
                Sts.List.elem[i].Source,Sts.List.elem[i].Computer,Sts.List.elem[i].User,//计算机和用户
                Sts.List.elem[i].Dsp);//事件的所有信息
    }
    fclose(fp);
    printf("保存日志记录成功！\n");

    fp = fopen("EventGraph.txt","w");
    if(NULL == fp)
    {
        printf("打开关联图文件失败！\n");
        return ERROR;
    }

    if(Sts.G.created == TRUE)
    {//如果创建了关联图
        ArcNode *parc;

        fprintf(fp,"%d\n",Sts.G.vexnum);//写入结点数目
        for(i=0;i<MAX_VERTEX_NUM;i++)
        {//写入结点信息
            if(Sts.G.vertices[i].created == TRUE)
            fprintf(fp,"%d %d ",Sts.G.vertices[i].info.ID,Sts.G.vertices[i].info.num);
        }

        fputc('\n',fp);

        for(i=0;i<MAX_VERTEX_NUM;i++)
        {//写入弧信息
            if(Sts.G.vertices[i].created == TRUE)
            {//获得头节点
                parc = Sts.G.vertices[i].firstarc;
                while(parc->nextarc)
                {//遍历此结点下的弧
                    if(parc->nextarc->weight != 0)//如果弧的权重不为0，则写下弧的信息
                        fprintf(fp,"%d %d %d\n",Sts.G.vertices[i].info.ID,Sts.G.vertices[parc->nextarc->adjvex].info.ID,parc->nextarc->weight);
                    parc = parc->nextarc;
                }
            }
        }
    }

    fclose(fp);
    printf("保存关联图成功！\n");

    return OK;
}

/**
 * 函数名称: ExitSys()
 * 函数功能: 退出系统
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status ExitSys()
{
    int option;
    SMALL_RECT *rc;
    char *Functions[]={
                        "是否保存文件？",
                        "",
                        "是",
                        "否",
                        "",
                        "返回",
                      };//功能名称显示

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//调用显示功能函数
    option = ChooseFun(rc, 2);//获得用户选项

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
 * 函数名称: Search_Event
 * 函数功能: 从原始事件集中查找事件
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
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
                        "信息检索",
                        "",
                        "按事件ID检索",
                        "按事件时间检索",
                        "按事件用户检索",
                        "按事件来源检索",
                        "按事件客体检索",
                        "",
                        "返回",
                      };//功能名称显示

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 9);//调用显示功能函数
    option = ChooseFun(rc, 5);//获得用户选项

    ClearScreen();
    fflush(stdin);

    if(option > 0 && option < 6 && pFunction[option] != NULL)
    {
        (*pFunction[option-1])();
    }
    return OK;
}

/**
 * 函数名称: Search_ID
 * 函数功能: 输入事件ID，检索所有相同ID的事件
            并可得到各事件在事件表中的序号，用于查询详细信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Search_ID()
{
    int i;
    unsigned short ID;
    char find = 0;

    printf("请输入需要检索的事件ID:\n");
    scanf("%d",&ID);//输入ID

    for(i=0; i<Sts.List.length; i++)
    {//顺序遍历事件记录
        if(Sts.List.elem[i].ID == ID)
        {//事件记录ID与用户输入ID匹配时
            find = 1;//修改标记变量
            printf("序号:%4d 事件ID:%5d 时间:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd 用户:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//打印事件信息
        }
    }

    if(!find)
    {//标记变量未被修改
        printf("无符合要求的事件\n");
    }
    return OK;
}

/**
 * 函数名称: Search_Time
 * 函数功能: 输入所要检索的时间范围，检索符合要求的事件并得到序号
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Search_Time()
{
    tm  *Start,*End;
    int i;
    char find = 0;

    Start = (tm*)malloc(sizeof(tm));
    End = (tm*)malloc(sizeof(tm));
    printf("请按照括号内形式输入时间(yyyy/mm/dd hh:mm:ss)\n如2018年1月1日下午两点半请输入2018/1/1 14:30:00\n");

    printf("检索时间范围起点:");
    scanf("%d/%d/%d %d:%d:%d",//读入时间
                                 &Start->tm_year,
                                 &Start->tm_mon,
                                 &Start->tm_mday,
                                 &Start->tm_hour,
                                 &Start->tm_min,
                                 &Start->tm_sec);
    Start->tm_year -= 1900;
    Start->tm_mon -= 1;

    printf("检索时间范围终点:");
    scanf("%d/%d/%d %d:%d:%d",//读入时间
                                 &End->tm_year,
                                 &End->tm_mon,
                                 &End->tm_mday,
                                 &End->tm_hour,
                                 &End->tm_min,
                                 &End->tm_sec);
    End->tm_year -= 1900;//将时间修改至与tm时间类型格式一致
    End->tm_mon -= 1;

    ClearScreen();
    /*从系统读入的事件自然以时间为序*/
    for(i=0; i<Sts.List.length; i++)
    {
        if(T_cmp(End,Sts.List.elem[i].Time) <= 0)
        {//当检索终点后于事件时间则停止过滤
            find = 1;
            break;
        }
    }

    for(;i<Sts.List.length;i++)
    {
        if(T_cmp(Start,Sts.List.elem[i].Time) <= 0)//如果起点后于事件时间则停止输出
            break;
        printf("序号:%4d 事件ID:%5d 时间:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd 用户:%s\n",
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
        printf("无符合要求的事件\n");
    }

    return OK;
}

/**
 * 函数名称: Search_User
 * 函数功能: 输入用户名User，检索所有相同用户的事件
            并可得到各事件在事件表中的序号，用于查询详细信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Search_User()
{
    int i;
    char User[BufSize], find = 0;

    printf("请输入需要检索的用户名：");
    My_gets(User,stdin);//输入用户名
    for(i=0; i<Sts.List.length; i++)
    {//顺序遍历事件记录
        if(strcmp(Sts.List.elem[i].User,User) == 0)
        {//与事件记录用户名相匹配时
            find = 1;//修改标记变量
            printf("序号:%4d 事件ID:%5d 时间:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd 用户:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//输出事件信息
        }
    }

    if(!find)
    {//标记变量未被修改时
        printf("无符合要求的事件\n");
    }
    return OK;

}

/**
 * 函数名称: Search_Source
 * 函数功能: 输入事件源Source，检索事件源中包含Source字符串的事件
            并可得到各事件在事件表中的序号，用于查询详细信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Search_Source()
{
    int i;
    char Source[BufSize], find = 0;

    printf("请输入需要检索的来源：");
    My_gets(Source,stdin);//输入事件来源
    for(i=0; i<Sts.List.length; i++)
    {//顺序遍历事件记录
        if(KMP(Sts.List.elem[i].Source,strlen(Sts.List.elem[i].Source),Source,strlen(Source))!= -1)
        {//用户输入字符串是事件来源字符串的子串时
            find = 1;//修改标记变量
            printf("序号:%4d 事件ID:%5d 时间:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd 用户:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//输出事件信息
        }
    }

    if(!find)
    {//标记变量未被修改时
        printf("无符合要求的事件\n");
    }
    return OK;

}

/**
 * 函数名称: Search_Dsp
 * 函数功能: 根据描述内容，检索符合要求的事件简略信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Search_Dsp()
{
    int i;
    char Dsp[BufSize], find = 0;

    printf("请输入事件客体：");
    My_gets(Dsp,stdin);
    for(i=0; i<Sts.List.length; i++)
    {
        if(KMP(Sts.List.elem[i].Dsp,strlen(Sts.List.elem[i].Dsp),Dsp,strlen(Dsp))!= -1)
        {//用户输入客体是事件记录描述子串时
            find = 1;//修改标记变量
            printf("序号:%4d 事件ID:%5d 时间:%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd 用户:%s\n",
                                                        i,
                                                        Sts.List.elem[i].ID,
                                                        Sts.List.elem[i].Time->tm_year+1900,
                                                        Sts.List.elem[i].Time->tm_mon+1,
                                                        Sts.List.elem[i].Time->tm_mday,
                                                        Sts.List.elem[i].Time->tm_hour,
                                                        Sts.List.elem[i].Time->tm_min,
                                                        Sts.List.elem[i].Time->tm_sec,
                                                        Sts.List.elem[i].User);//输出事件信息
        }
    }

    if(!find)
    {//标记变量未被修改时
        printf("无符合要求的事件\n");
    }
    return OK;

}

/**
 * 函数名称: Detail_Event
 * 函数功能: 通过检索功能得到事件序号
            以序号查询事件详细信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Detail_Event()
{
    int num;
    char *pchar;

    printf("请输入事件序号:");
    scanf("%d",&num);

    if(num >= Sts.List.length)
    {//用户输入非法序号
        printf("输入序号超过事件集表长！\n");
        return ERROR;
    }
    //打印事件详细信息
    printf("事件ID：  %5d\n",Sts.List.elem[num].ID);
    printf("事件类别：%5d\n",Sts.List.elem[num].Categary);

    switch(Sts.List.elem[num].Type) //事件类型
    {
        case EVENTLOG_SUCCESS:
            pchar= "成功";
            break;
        case EVENTLOG_ERROR_TYPE:
            pchar= "错误";
            break;
        case EVENTLOG_WARNING_TYPE:
            pchar= "警告";
            break;
        case EVENTLOG_INFORMATION_TYPE:
            pchar= "信息";
            break;
        case EVENTLOG_AUDIT_SUCCESS:
            pchar= "审计成功";
            break;
        case EVENTLOG_AUDIT_FAILURE:
            pchar= "审计失败";
    }
    printf("事件级别：%s\n",pchar);

    printf("事件时间：%.4hd-%.2hd-%.2hd %.2hd:%.2hd:%.2hd\n",
                                                        Sts.List.elem[num].Time->tm_year+1900,
                                                        Sts.List.elem[num].Time->tm_mon+1,
                                                        Sts.List.elem[num].Time->tm_mday,
                                                        Sts.List.elem[num].Time->tm_hour,
                                                        Sts.List.elem[num].Time->tm_min,
                                                        Sts.List.elem[num].Time->tm_sec);
    printf("事件源：%s\n",Sts.List.elem[num].Source);
    printf("用户名：%s\n",Sts.List.elem[num].User);
    printf("计算机：%s\n",Sts.List.elem[num].Computer);
    printf("描述：%s",Sts.List.elem[num].Dsp);

}

/**
 * 函数名称: Insert_Event
 * 函数功能: 输入事件信息，插入到原始事件表中
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Insert_Event()
{
    /*读入插入事件信息*/
    EventData event;
    char Buffer[32];
    int i;

    tm *ptm;
    ptm = (tm*)malloc(sizeof(tm));

    fflush(stdin);//获取用户输入的详细事件信息
    printf("事件ID：");
    scanf("%d",&event.ID);
    printf("事件级别：");
    getchar();
    My_gets(Buffer,stdin);
    if(strcmp(Buffer,"成功") == 0)
        event.Type = EVENTLOG_SUCCESS;
    else if(strcmp(Buffer,"信息") == 0)
        event.Type = EVENTLOG_INFORMATION_TYPE;
    else if(strcmp(Buffer,"错误") == 0)
        event.Type = EVENTLOG_ERROR_TYPE;
    else if(strcmp(Buffer,"警告") == 0)
        event.Type = EVENTLOG_WARNING_TYPE;
    else if(strcmp(Buffer,"审计成功") == 0)
        event.Type = EVENTLOG_AUDIT_SUCCESS;
    else if(strcmp(Buffer,"审计失败") == 0)
        event.Type = EVENTLOG_AUDIT_FAILURE;

    printf("事件种类：");
    scanf("%d",&event.Categary);

    printf("请按照括号内形式输入时间(yyyy/mm/dd hh:mm:ss)\n如2018年1月1日下午两点半请输入2018/1/1 14:30:00\n");
    printf("事件时间：");
    scanf("%d/%d/%d %d:%d:%d",//读入时间
                                 &ptm->tm_year,
                                 &ptm->tm_mon,
                                 &ptm->tm_mday,
                                 &ptm->tm_hour,
                                 &ptm->tm_min,
                                 &ptm->tm_sec);
    ptm->tm_year -= 1900;//将时间修改至与tm时间类型格式一致
    ptm->tm_mon -= 1;
    event.Time = ptm;

    getchar();
    printf("事件源：");
    My_gets(event.Source,stdin);

    printf("用户名：");
    My_gets(event.User,stdin);

    printf("计算机：");
    My_gets(event.Computer,stdin);

    printf("事件描述：");
    My_gets(event.Dsp,stdin);

    if(Sts.List.length >= Sts.List.length)
    {//当前存储空间已满，增加分配
        EventData *newbase;
        newbase = (EventData *)realloc(Sts.List.elem, (Sts.List.listsize+LISTINCREMENT)*sizeof(EventData));
        if(!newbase) return(ERROR);    //存储分配失败
        Sts.List.elem = newbase;               //新基址
        Sts.List.listsize += LISTINCREMENT;    //增加存储容量
    }

    for(i=Sts.List.length;i >= 0;i--)
    {
        if(T_cmp(event.Time,Sts.List.elem[i-1].Time) >= 0)//如果插入事件时间先于第i个事件时间
        {//停止移动
            break;
        }
        Sts.List.elem[i] = Sts.List.elem[i-1];//将元素整体后移1位
    }

    Sts.List.length++;//记录表表长+1
    Sts.List.elem[i] = event;//在空出来的位置处插入用户输入事件

    return OK;
}

/**
 * 函数名称: Delete_Event
 * 函数功能: 输入事件信息，插入到原始事件表中
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Delete_Event()
{
    int i;
    printf("请输入要删除的事件序号：");
    scanf("%d",&i);

    Sts.List.length--;//记录表表长-1
    for(;i< Sts.List.length;i++)
    {//将所要删除事件之后的事件依次往前移动
        Sts.List.elem[i] = Sts.List.elem[i + 1];
    }
    printf("成功删除事件！\n");
    return OK;
}

/**
 * 函数名称: Get_Frequent_items
 * 函数功能: 获得单个事件频繁项集
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status Get_Frequent_items()
{
    InitGraph();

    int i,j;
    char find = 1;
    VertexType info;

    for(j=0;j<Sts.List.length;j++)
    {//初始化标记变量
        Sts.List.elem[j].flag = 0;
    }

    for(i=0;find;)
    {//发现新的事件ID时
        find = 0;//重置查找标记变量
        info.num = 0;//初始化计数器
        for(j=0;j<Sts.List.length;j++)
        {//扫描事件表查找新的事件ID
            if(Sts.List.elem[j].flag == 0)
            {//如果事件未被标记则是新ID
                find = 1;
                Sts.List.elem[j].flag = 1;
                info.ID = Sts.List.elem[j].ID;
                break;
            }
        }
        for(;j<Sts.List.length;j++)
        {//统计该ID的次数
            if(Sts.List.elem[j].ID == info.ID)
            {
                Sts.List.elem[j].flag = 1;//将相同ID的事件进行标记，并计数
                info.num++;
            }
        }

        if(info.num >= Sts.List.length * min_sup)
        {//如果该ID事件出现次数满足最小支持度,在图中插入节点
            InsertVex(info);
        }
    }

    Sts.G.created = TRUE;
    printf("获取频繁项集成功！\n");

    return OK;
}

/**
 * 函数名称: SetGraph
 * 函数功能: 建立关联图
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status SetGraph()
{
    int Ret;
    Ret = Get_Frequent_items();

    if(Ret == OK)
    {
        int i,j;
        int t;

        for(i=0;i<Sts.List.length;i++)//初始化标记变量
            Sts.List.elem[i].flag = 0;

        for(i=0;i<MAX_VERTEX_NUM;i++)
        {
            if(Sts.G.vertices[i].created == TRUE)//当结点已创建时
            {
                for(j=0;j<Sts.List.length;j++)
                {//标记与结点ID相同的事件（图中结点均为频繁事件）
                    if(Sts.List.elem[j].ID == Sts.G.vertices[i].info.ID)
                     {
                         Sts.List.elem[j].flag = 1;
                     }
                }
            }
        }

        for(i = Sts.List.length - 1;i>=0;i--)
        {
            if(Sts.List.elem[i].flag == 0)//如果第i项非频繁项则跳过
                continue;
            for(j=i-1;j>=0;j--)
            {
                if(Sts.List.elem[i].ID == Sts.List.elem[j].ID)//如果扫描过的事件ID和当前事件ID相同
                    break;

                if(T_cmp(Sts.List.elem[i].Time,Sts.List.elem[j].Time) > Delta_Time)//如果事件之间时间差大于Delta_Time
                {
                    break;
                }

                if(Sts.List.elem[j].flag == 1)
                {//如果带有频繁项标记，则插入一条弧
                    InsertArc(Sts.List.elem[i].ID,Sts.List.elem[j].ID,add);
                }
            }
        }
        printf("关联图建立完成！\n");
    }
    return OK;
}

/**
 * 函数名称: ShowVex
 * 函数功能: 查询关联图中结点
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status ShowVex()
{
    int i;
    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//扫描事件图邻接表
        if(Sts.G.vertices[i].created == TRUE)//结点状态为已创建
            printf("事件ID：%5d 出现次数：%3d\n",Sts.G.vertices[i].info.ID,Sts.G.vertices[i].info.num);//输出结点信息
    }
    return OK;
}

/**
 * 函数名称: GetRules
 * 函数功能: 获取关联规则
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status GetRules()
{
    if(Sts.G.created == FALSE)
    {
        printf("尚未建立关联图");
        return ERROR;
    }

    int i;
    ArcNode *parc,*temp;

    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//遍历所有结点
        if(Sts.G.vertices[i].created == TRUE)
        {//当结点已创建时
            parc = Sts.G.vertices[i].firstarc;
            while(parc->nextarc)
            {//遍历该结点的弧
                if(parc->nextarc->weight < Sts.G.vertices[i].info.num * confidence)
                    parc->nextarc->weight = 0;//如果弧的权小于最低置信度则标记权为0

                parc = parc->nextarc;
            }
        }
    }
    printf("获取关联规则成功！\n");
    return OK;
}

/**
 * 函数名称: ShowRules
 * 函数功能: 查询关联规则
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
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
                        "加载日志文件",
                        "",
                        "事件预测",
                        "事件追踪索源",
                        "",
                        "返回",
                      };//显示功能内容

    rc=(SMALL_RECT*)malloc(sizeof(SMALL_RECT));
    *rc=ShowFunctions(Functions, 6);//调用显示功能函数
    option = ChooseFun(rc, 2);//获取用户选项

    ClearScreen();


    if(option > 0 && option < 3 && pFunction[option] != NULL)
    {
        Ret = (*pFunction[option-1])();
    }
}

/**
 * 函数名称: InsertVex
 * 函数功能: 在事件图中插入新结点
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status InsertVex(VertexType info)
{
    int index;
    int i = 0;

    do{//开放定址法
        if(i == MAX_VERTEX_NUM)
        {//查找次数超过表的容量时
            return ERROR;
        }

        index = Hash(info.ID + i);
        i++;//当index指引的地址冲突时,即插入的位置已经被创建结点
    }while(Sts.G.vertices[index].created == TRUE);


    Sts.G.vertices[index].created = TRUE;//将该点标记为已创建
    Sts.G.vertices[index].info = info;//写入结点信息

    //初始化头节点
    ArcNode* head = (ArcNode*)malloc(sizeof(ArcNode));
    head->adjvex = -1;
    head->nextarc = NULL;
    head->weight = 0;
    Sts.G.vertices[index].firstarc = head;
    Sts.G.vexnum++;

    return OK;
}

/**
 * 函数名称: InsertArc
 * 函数功能: 在事件图中插入弧，指定权重。权重为0时：如果已存在该弧，则这条弧的权值+1
 * 输入参数: 两个事件ID
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status InsertArc(int id1,int id2,int weight)
{
    int i,index1,index2;

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//如果查找次数超过最大结点数
            printf("ID为%5d的事件不存在！\n",id1);
            return ERROR;
        }

        index1 = Hash(id1 + i);
        i++;
    }while(Sts.G.vertices[index1].info.ID != id1);//index指引的位置存放事件ID与id1不等时继续查找

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//如果查找次数超过最大结点数
            printf("ID为%5d的事件不存在！\n",id2);
            return ERROR;
        }

        index2 = Hash(id2 + i);
        i++;
    }while(Sts.G.vertices[index2].info.ID != id2);//index指引的位置存放事件ID与id1不等时继续查找

    ArcNode *parc,*tmp;
    parc = Sts.G.vertices[index1].firstarc;
    for(;parc->nextarc != NULL;parc = parc->nextarc)
    {
        if(parc->nextarc->adjvex == index2)//如果存在指向id2事件的弧
        {
            if(weight == add)//如果调用时指定增加权值操作
                parc->nextarc->weight++;//则该弧权+1
            else
                parc->nextarc->weight = weight;//否则权值赋给这条弧
                break;
        }
    }

    if(parc->nextarc == NULL)
    {//如果没有指向id2事件的弧,则添加一条权值为1的弧
        tmp = (ArcNode*)malloc(sizeof(ArcNode));
        tmp->adjvex = index2;
        if(weight == add)//如果调用时指定增加权值操作
            tmp->weight = 1;//则该弧权初始化为1
        else
            tmp->weight = weight;//否则将权值赋给该弧
        tmp->nextarc = Sts.G.vertices[index1].firstarc->nextarc;
        Sts.G.vertices[index1].firstarc->nextarc = tmp;
    }

    return OK;
}

/**
 * 函数名称: trace
 * 函数功能: 根据关联规则追踪索源
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
 status trace()
 {
    int i,id;
    int find;
    ArcNode *parc;


    printf("请输入事件的ID:");
    scanf("%d",&id);
    find = 0;//初始化查找变量
    printf("可能导致ID：%d事件发生的事件有：\n",id);
    for(i=0;i<MAX_VERTEX_NUM;i++)
    {//遍历图中结点
        if(Sts.G.vertices[i].created == TRUE)
        {//如果结点已经创建
            parc = Sts.G.vertices[i].firstarc;
            while(parc->nextarc)
            {//遍历结点的弧
                if(Sts.G.vertices[parc->nextarc->adjvex].info.ID == id && parc->nextarc->weight > 0)
                {
                    find = 1;
                    printf("ID%6d\n",Sts.G.vertices[i].info.ID);

                }
                parc = parc->nextarc;
            }
        }
    }
    if(!find)printf("无\n");

    return OK;
 }

/**
 * 函数名称: forecast
 * 函数功能: 根据关联规则预测将要发生的事件
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
 status forecast()
 {
    int i,id,index;
    int find;
    ArcNode *parc;

    printf("请输入事件ID:");
    scanf("%d",&id);

    i=0;
    do{
        if(i==MAX_VERTEX_NUM)
        {//如果查找次数超过最大结点数
            printf("ID为%5d的事件不存在！\n",id);
            return ERROR;
        }

        index = Hash(id + i);
        i++;
    }while(Sts.G.vertices[index].info.ID != id);//index指引的位置存放事件ID与id1不等时继续查找

    find = 0;
    parc = Sts.G.vertices[index].firstarc;
    printf("记录中ID：%d事件可能导致的事件有:\n",id);
    while(parc->nextarc)
    {//遍历所有弧
        if(parc->nextarc->weight > 0)
        {//弧权值不为0时
            find = 1;//修改标记变量
            printf("ID:%6d\n",Sts.G.vertices[parc->nextarc->adjvex].info.ID);//输出事件ID
        }
        parc = parc->nextarc;
    }

    if(!find)printf("无\n");
    return OK;

 }

 /**
 * 函数名称: InsertRule
 * 函数功能: 向关系图中插入一条弧
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status InsertRule()
{
    int id1,id2;
    printf("请输入要插入的规则(事件1ID->事件2ID)：\n事件1ID:");
    scanf("%d",&id1);

    printf("事件2ID:");
    scanf("%d",&id2);

    InsertArc(id1,id2,WEIGHT);//将用户输入的弧赋予一个极大的权值

    printf("插入关联规则成功！\n");
    return OK;
}

/**
 * 函数名称: DeleteRule
 * 函数功能: 删除关联图中的一条弧
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
status DeleteRule()
{
    int id1,id2;
    printf("请输入要插入的规则(事件1ID->事件2ID)：\n事件1ID:");
    scanf("%d",&id1);

    printf("事件2ID:");
    scanf("%d",&id2);

    InsertArc(id1,id2,0);//将所要删除的弧权值清0

    printf("删除关联规则成功！\n");
    return OK;
}

/**
 * 函数名称: Hash
 * 函数功能: 根据事件ID得到事件的存储位置
 * 输入参数: 事件ID
 * 输出参数: 无
 * 返 回 值: ID所对应的位置
 * 调用说明:
 */
int Hash(int ID)
{
    return (ID/1000 + ID%100)%MAX_VERTEX_NUM;
}

/**
 * 函数名称: T_cmp
 * 函数功能: 比较时间前后（精确到秒）
 * 输入参数: 两个tm类指针
 * 输出参数: 无
 * 返 回 值: 如果Time1时间上先于Time2则返回正值，
            如果后于Time2则返回负值，否则返回0.
 * 调用说明:
 */
int T_cmp(tm* Time1,tm* Time2)
{
    int value;

    if(Time1->tm_year < Time2->tm_year)
        return 10000;
    else if(Time1->tm_year > Time2->tm_year)
        return -10000;
    else
    {//时间年份相同时
        if(Time1->tm_mon < Time2->tm_mon)
            return 10000;
        else if(Time1->tm_mon > Time2->tm_mon)
            return -10000;
        else
        {//月份相同时
                value = (Time2->tm_sec - Time1->tm_sec)
                    + (Time2->tm_min - Time1->tm_min)*60
                    + (Time2->tm_hour - Time1->tm_hour)*60*60
                    + (Time2->tm_mday - Time1->tm_mday)*60*60*24;
                return value;
        }
    }
}

/**
 * 函数名称: My_gets
 * 函数功能: 从文件中读入一行信息并去掉换行符
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void My_gets(char* str,FILE* fp)
{
    char Buffer[BufSize];
    fgets(Buffer,BufSize,fp);
    Buffer[strlen(Buffer)-1] = '\0';
    strcpy(str,Buffer);
}

/**
 * 函数名称: GetDsp
 * 函数功能: 从文本获取事件描述
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
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
 * 函数名称: cal_next
 * 函数功能: 作为kmp的辅助算法，获取kmp中ptr串的next数组
 * 输入参数: 字符串长度，一个整形数组以及它的长度
 * 输出参数: 无
 * 返 回 值: 无
 * 调用说明:
 */
void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}

/**
 * 函数名称: KMP
 * 函数功能: 查找str串中ptr串出现的位置
 * 输入参数: str，str的长度，ptr以及ptr的长度
 * 输出参数: 无
 * 返 回 值: ptr在str中首次出现的位置，若没有出现则返回-1
 * 调用说明:
 */
int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);//计算next数组
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//说明k移动到ptr的最末端
        {
            //cout << "在位置" << i-plen+1<< endl;
            //k = -1;//重新初始化，寻找下一个
            //i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
            return i-plen+1;//返回相应的位置
        }
    }
    return -1;
}
