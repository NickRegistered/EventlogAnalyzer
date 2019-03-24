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


#define SCR_ROW 25             /*��Ļ����*/
#define SCR_COL 80             /*��Ļ����*/
#define MENU_NUM 5             /*���˵���Ŀ*/

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -2
#define INFEASTABLE -1
#define OVERFLOW -2

#define MAX_VERTEX_NUM 50   /*���ڵ���*/

typedef int status;

#define ENTRY_NUM      1024     /*��־��Ŀ����*/
#define LIST_INIT_SIZE 2048     /*���Ա��ʼ��С*/
#define LISTINCREMENT  512      /*���Ա����ݴ�С*/

#define DspLength      4096      /*��������*/
#define Delta_Time     5         /*�¼��佨��ʱ���ϵ��ʱ�䷶Χ*/
#define MAX_VERTEX_NUM 40        /*�¼�ͼ���ڵ���*/
#define min_sup        0.015     /*��С֧�ֶ�*/
#define confidence     0.7       /*��С���Ŷ�*/

#define BufSize        64
#define WEIGHT         1024
#define add            -1
using namespace std;

HANDLE gh_std_out; /*��׼����豸���*/
HANDLE gh_std_in;  /*��׼�����豸���*/
char *gp_scr_att = NULL; /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/

/**
*��Ļ������Ϣ�������ṹ
*/
typedef struct layer_node{
    char LayerNum;             /*�������ڲ���*/
    SMALL_RECT rcArea;         /*����������������*/
    CHAR_INFO *pContent;       /*�������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;             /*�������������ַ���Ԫԭ���Դ洢������*/
    struct layer_node *next;   /*��һ���ĵ�ַ*/
}LAYER;
LAYER *gp_top_layer = NULL;

/**
*�����ṹ
*/
typedef struct hot_area {
    SMALL_RECT *pArea;     /*������λ�����׵�ַ*/
    char *pSort;           /*������������׵�ַ*/
    char *pTag;            /*������������׵�ַ*/
    int num;               /*��������*/
} HOT_AREA;


/**
*��ǩ���ṹ
*/
typedef struct labe1_bundle {
    char **ppLabel;        /**< ��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;           /**< ��ǩ��λ�����׵�ַ*/
    int num;               /**< ��ǩ����*/
} LABEL_BUNDLE;

CHAR_INFO *buff_menubar_info;/*�˵����ַ���Ϣ������*/

/**
*�¼���Ϣ���ṹ
*/
struct EventData{
    int ID;          /**<�¼�ID*/
    int Type;        /**<�¼�����*/
    int Categary;    /**<�¼����*/
    tm   *Time;                 /**<�¼�ʱ��*/
    char Source[BufSize];       /**<�¼�Դ*/
    char Computer[BufSize];     /**<�����*/
    char User[BufSize];         /**<�û�*/
    char Dsp[DspLength];        /**<����*/
    bool flag;                  /**<����ͳ�Ƶı�Ǳ���*/
};

/**
*�¼���¼��
*��¼�¼���ԭʼ����
*/
typedef struct EventLog_List{
	EventData *elem;/**<����ַ*/
	int length;     /**<��*/
	int listsize;   /**<�����洢Ԫ�ظ���*/
}LogList;

/**
*�����ṹ
*/
struct ArcNode{
    int adjvex;             //�û�ָ��Ķ���λ��
    struct ArcNode *nextarc;//ָ����һ������ָ��
    int weight;             //�û������Ϣ��ָ��
};

/**
*�¼�ID�����ִ���
*/
typedef struct ID_DATA{
    int ID;
    int num;
}VertexType;

/**
*����ṹ
*/
typedef struct VNode{
    bool created;           //��־����ڱ��е�״̬
    VertexType info;        //������Ϣ
    ArcNode *firstarc;      //ָ���һ�������ö���Ļ���ָ��
};


/**
*�����ڽӱ��ͼ�ṹ
*/
struct ALGragh{
    bool created;       //��־ͼ��״̬����
    VNode *vertices;    //�ڽӱ�
    int vexnum;         //ͼ�н�����
};

/**
*����һЩָ��,�Ա��ҵ������ṹ
**/
struct STRUCTURES{
    LogList  List;      /**<ָ���¼��*/
    ALGragh  G;         /**<ͼ*/
};
STRUCTURES Sts;

char *main_menu[]={
                   "�ļ�",
                   "��Ϣ��ѯ",          /*ϵͳ���˵���*/
                   "��Ϣά��",
                   "ԭʼ����ͼ����",
                   "�����������",
                  };

char *sub_menu[]={
                  "[1]��ȡ����",
                  "[2]��������",    /*ϵͳ�Ӳ˵���*/
                  "[3]�˳�ϵͳ",

                  "[1]�¼���Ϣ����",
                  "[2]��ϸ��Ϣ��ѯ",

                  "[1]�����¼���Ϣ",
                  "[2]ɾ���¼�",

                  "[1]��������ͼ",
                  "[2]��������ѯ",

                  "[1]��ȡ��������",
                  "[2]��ѯ��������",
                  "[3]�����������",
                  "[4]ɾ����������",
                 };

int gi_sel_menu=1;                          /*��ѡ�е����˵���ţ���ֵΪ1*/
int gi_sel_sub_menu = 0;                    /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/
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

