#include"cstdio"
#include"stdio.h"
#include"iostream"
#include"cmath"
#include"time.h"
#include"cstring"
#include"string"
#include"fstream"
#include"algorithm"
#include"malloc.h"
#include"sstream"
#define MAXFNAME 80    /*Maximum length of file names*/
#define MAXMOD 20000      /*Maximum number of modules*/
#define MAXDEF 1000
using namespace std;
typedef long long ll;

int term=0;
int sets=0;
int sets1=0;
int o[10000];  //������
int b[1000];
/*�м�����Ϣ*/
typedef struct rectangle {
	long int area;//���
	int length;//���򳤶�
	int width;//������
	int x;//���½Ǻ�����
	int y;//���½�������
} base_rectangle;

//����item�����Ϣ
struct Items{
	int area;//���
	int length;//���򳤶�
	int width;//������
	int x;//���½Ǻ�����
	int y;//���½�������
	double v;//Ȩֵ
};

//����ɰ�������Ϣ
struct Defects{
    int x0;//���½Ǻ�����
    int y0;//���½�������
    int x1;//���ϽǺ�����
    int y1;//���Ͻ�������
    int area;//���
};

//������ʱ��ÿ��item����
struct Instance{
    struct Items item[26];
    struct Defects defect[10];
};

//��¼��ɢ����Ϣ
struct Sets{
    int length;//��ɢ������
    int *Z;//��ɢ��Ԫ�ش���Z������
};

FILE *mod_file = NULL;
FILE *defect_file = NULL;
int no_inst;
int no_def;         /* number of defects to be created per instance */
base_rectangle defects[MAXDEF];        /* holds the different defects */
int defect_x[MAXDEF];  /* coordinates of lower left edge of the defect */
int defect_y[MAXDEF];
char filename[MAXFNAME];    /* name of the file to hold the items */
char defectfilename[MAXFNAME];      /* name of the file to hold the defects */
base_rectangle items[MAXMOD];  /* array of items */
base_rectangle object;   /* holds the large object */
int maxitemarea;               /* minimal and maximal area of item */
int minitemarea;
double interval_lb = 0.1;            /* lower and upper bound of interval for b */
double interval_ub = 0.9;

long r_n;
long c = 16807;
long p = 2147483647;
long q = 127773;
long t = 2836;
int Type_of_Items;
int Num_of_Defects=0;
int Num_of_Instances;
int len[505],wid[505];
int len_no_D[505],wid_on_D[505];
int len_D[505],wid_D[505];
int OFV;
int *L_sets;
int *W_sets;
int *L_sets_no_D;
int *W_sets_no_D;
int *L_D_sets;
int *W_D_sets;
int min_Length;//��ɰ�ۺ�item���ڵ���С����
int min_Width;
int min_Defect_Length;//��Сɰ�۵ĳ���
int min_Defect_Width;
int min_item_Length;//��item����С����
int min_item_Width;
int PLATE_WIDTH;
int PLATE_LENGTH;
char CHAR[10];
char matrix_ins[6][4][10][40],matrix_def[6][4][10][40];
char fname_ins[20]="",fname_def[20]="";
//double ****f;
//int f[205][105][205][105];//�����ɰ�۵��м��Ľ��
//int index_ox[200];//�����½Ǻ�������ջ
//int index_oy[200];
//int index_x[200];
//int index_y[200];
//int numox;//ջ��ox������
//int numoy;
//int numx;
//int numy;
//int f1[205][105]={-1};//�ҷ�û��ɰ�۵��м��Ľ��

short f[227][102][227][102];//�����ɰ�۵��м��Ľ��
//short f[155][155][155][155];
//short f[77][77][77][77];
//short f[122][55][122][55];
int index_ox[505];//�����½Ǻ�������ջ
int index_oy[505];
int index_x[505];
int index_y[505];
int numox;//ջ��ox������
int numoy;
int numx;
int numy;
int f1[226][226]={-1};//�ҷ�û��ɰ�۵��м��Ľ��

struct Items *item;
struct Instance ins[1805];
struct Defects *defect;
struct Sets Length_sets;
struct Sets Width_sets;
struct Sets Length_sets_no_D;//
struct Sets Width_sets_no_D;
struct Sets Length_D_sets;//
struct Sets Width_D_sets;

int *get_Sets(int n,int m,int a[]);//�󦸼���,a��1��ʼ,n��ʾ��Ʒ���࣬mΪ����������aΪ��Ʒ���
int *poscount(int* input, int len);//n����ȡ������������͵ĸ���--ɰ�����
double**** alloc4float(int n1,int n2,int n3,int n4); //��̬������ά���飬��¼�м�������ֵ
void free4float(float**** p4);
double randomnum();//�����������
void Build_an_Instance();
void instances_generator();
void readbatch(char filename[]);
void readdefect(char defectfilename[]);
void Main_Operators();
void Init();
void read_FILE();//ѭ����ȡ����ļ�
int get_G(struct rectangle obj);
int get_px(struct Sets sets);
int get_qy(struct Sets sets);
int if_pure(struct rectangle obj);
int get_F_pure(struct rectangle obj);
int get_F_conta(struct rectangle obj);
int search_pxqy(int a[],int n,int l,int r);//���ַ�����pxqy
int sub_Plate(struct rectangle obj1,struct rectangle obj2,int ans);
int if_cross(struct rectangle obj,int cut_x,int cut_y);
void Free();
void Print(struct rectangle obj);//����м��

