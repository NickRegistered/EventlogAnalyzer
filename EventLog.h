#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>

#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED


#define SCR_ROW 25             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/
#define MENU_NUM 5             /*主菜单数目*/

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -2
#define INFEASTABLE -1
#define OVERFLOW -2

#define MAX_VERTEX_NUM 50   /*最大节点数*/

typedef int status;

#define ENTRY_NUM      1024     /*日志条目数量*/
#define LIST_INIT_SIZE 2048     /*线性表初始大小*/
#define LISTINCREMENT  512      /*线性表扩容大小*/

#define DspLength      4096      /*描述长度*/
#define Delta_Time     5         /*事件间建立时序关系的时间范围*/
#define MAX_VERTEX_NUM 40        /*事件图最大节点数*/
#define min_sup        0.015     /*最小支持度*/
#define confidence     0.7       /*最小置信度*/

#define BufSize        64
#define WEIGHT         1024
#define add            -1
using namespace std;

HANDLE gh_std_out; /*标准输出设备句柄*/
HANDLE gh_std_in;  /*标准输入设备句柄*/
char *gp_scr_att = NULL; /*存放屏幕上字符单元属性值的缓冲区*/

/**
*屏幕窗口信息链结点结点结构
*/
typedef struct layer_node{
    char LayerNum;             /*弹出窗口层数*/
    SMALL_RECT rcArea;         /*弹出窗口区域坐标*/
    CHAR_INFO *pContent;       /*弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;             /*弹出窗口区域字符单元原属性存储缓冲区*/
    struct layer_node *next;   /*下一结点的地址*/
}LAYER;
LAYER *gp_top_layer = NULL;

/**
*热区结构
*/
typedef struct hot_area {
    SMALL_RECT *pArea;     /*热区定位数组首地址*/
    char *pSort;           /*热区类别数组首地址*/
    char *pTag;            /*热区序号数组首地址*/
    int num;               /*热区个数*/
} HOT_AREA;


/**
*标签束结构
*/
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

CHAR_INFO *buff_menubar_info;/*菜单条字符信息缓冲区*/

/**
*事件信息结点结构
*/
struct EventData{
    int ID;          /**<事件ID*/
    int Type;        /**<事件级别*/
    int Categary;    /**<事件类别*/
    tm   *Time;                 /**<事件时间*/
    char Source[BufSize];       /**<事件源*/
    char Computer[BufSize];     /**<计算机*/
    char User[BufSize];         /**<用户*/
    char Dsp[DspLength];        /**<描述*/
    bool flag;                  /**<用于统计的标记变量*/
};

/**
*事件记录表
*记录事件的原始数据
*/
typedef struct EventLog_List{
	EventData *elem;/**<基地址*/
	int length;     /**<表长*/
	int listsize;   /**<表最大存储元素个数*/
}LogList;

/**
*弧结点结构
*/
struct ArcNode{
    int adjvex;             //该弧指向的顶点位置
    struct ArcNode *nextarc;//指向下一条弧的指针
    int weight;             //该弧相关信息的指针
};

/**
*事件ID及出现次数
*/
typedef struct ID_DATA{
    int ID;
    int num;
}VertexType;

/**
*顶点结构
*/
typedef struct VNode{
    bool created;           //标志结点在表中的状态
    VertexType info;        //顶点信息
    ArcNode *firstarc;      //指向第一条依附该顶点的弧的指针
};


/**
*基于邻接表的图结构
*/
struct ALGragh{
    bool created;       //标志图的状态变量
    VNode *vertices;    //邻接表
    int vexnum;         //图中结点个数
};

/**
*保存一些指针,以便找到各个结构
**/
struct STRUCTURES{
    LogList  List;      /**<指向记录表*/
    ALGragh  G;         /**<图*/
};
STRUCTURES Sts;

char *main_menu[]={
                   "文件",
                   "信息查询",          /*系统主菜单名*/
                   "信息维护",
                   "原始关联图管理",
                   "关联规则管理",
                  };

char *sub_menu[]={
                  "[1]读取数据",
                  "[2]保存数据",    /*系统子菜单名*/
                  "[3]退出系统",

                  "[1]事件信息检索",
                  "[2]详细信息查询",

                  "[1]插入事件信息",
                  "[2]删除事件",

                  "[1]建立关联图",
                  "[2]关联结点查询",

                  "[1]获取关联规则",
                  "[2]查询关联规则",
                  "[3]插入关联规则",
                  "[4]删除关联规则",
                 };

int gi_sel_menu=1;                          /*被选中的主菜单项号，初值为1*/
int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/
int sub_menu_num[] = {3,2,2,2,4};

void RunSys();
void ExeFunction(int m, int s);
void ShowMenu(void);
void TagMainMenu(int num);
void LocSubMenu(int num,SMALL_RECT* rc);
void ClearScreen(void);
void ClearWindow(void);
void PopMenu(int num);
void PopOff(void);
void PopUp(SMALL_RECT *pRc,WORD att,LABEL_BUNDLE *pLabel,HOT_AREA *pHotArea);
void DrawBox(SMALL_RECT *pRc);
void TagSubMenu(int num);


SMALL_RECT ShowFunctions(char *Functions[], short Num);
status ChooseFun(SMALL_RECT *rc,short Num);


status InitStructures();
status InitGraph();

status ReadFromSys();
status ReadFromTxt();

status LoadFile();
status SaveAsTxt();
status ExitSys();

status Search_Event();
status Search_ID();
status Search_Time();
status Search_User();
status Search_Source();
status Search_Dsp();

status Detail_Event();

status Insert_Event();
status Delete_Event();

status Get_Frequent_items();
status SetGraph();
status ShowVex();

status GetRules();
status ShowRules();

status InsertVex(VertexType info);
status InsertArc(int id1,int id2,int weight);
status DeleteArc(int id1,int id2);

status InsertRule();
status DeleteRule();

status forecast();
status trace();

void cal_next(char *str, int *next, int len);
int KMP(char *str, int slen, char *ptr, int plen);
void GetDsp(char* str,FILE* fp);
void My_gets(char* str,FILE* fp);
int T_cmp(tm* Time1,tm* Time2);
int Hash(int ID);

#endif // ROOM_H_INCLUDED

