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
int o[10000];  //Ω集合
int b[1000];
/*中间板的信息*/
typedef struct rectangle {
	long int area;//面积
	int length;//横向长度
	int width;//纵向宽度
	int x;//左下角横坐标
	int y;//左下角纵坐标
} base_rectangle;

//定义item相关信息
struct Items{
	int area;//面积
	int length;//横向长度
	int width;//纵向宽度
	int x;//左下角横坐标
	int y;//左下角纵坐标
	double v;//权值
};

//定义砂眼相关信息
struct Defects{
    int x0;//左下角横坐标
    int y0;//左下角纵坐标
    int x1;//右上角横坐标
    int y1;//右上角纵坐标
    int area;//面积
};

//读数据时存每组item数据
struct Instance{
    struct Items item[26];
    struct Defects defect[10];
};

//记录离散集信息
struct Sets{
    int length;//离散集个数
    int *Z;//离散集元素存于Z数组中
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
int min_Length;//存砂眼和item在内的最小长度
int min_Width;
int min_Defect_Length;//最小砂眼的长度
int min_Defect_Width;
int min_item_Length;//存item的最小长度
int min_item_Width;
int PLATE_WIDTH;
int PLATE_LENGTH;
char CHAR[10];
char matrix_ins[6][4][10][40],matrix_def[6][4][10][40];
char fname_ins[20]="",fname_def[20]="";
//double ****f;
//int f[205][105][205][105];//存放有砂眼的中间板的结果
//int index_ox[200];//将左下角横坐标入栈
//int index_oy[200];
//int index_x[200];
//int index_y[200];
//int numox;//栈内ox的数量
//int numoy;
//int numx;
//int numy;
//int f1[205][105]={-1};//芬芳没有砂眼的中间板的结果

short f[227][102][227][102];//存放有砂眼的中间板的结果
//short f[155][155][155][155];
//short f[77][77][77][77];
//short f[122][55][122][55];
int index_ox[505];//将左下角横坐标入栈
int index_oy[505];
int index_x[505];
int index_y[505];
int numox;//栈内ox的数量
int numoy;
int numx;
int numy;
int f1[226][226]={-1};//芬芳没有砂眼的中间板的结果

struct Items *item;
struct Instance ins[1805];
struct Defects *defect;
struct Sets Length_sets;
struct Sets Width_sets;
struct Sets Length_sets_no_D;//
struct Sets Width_sets_no_D;
struct Sets Length_D_sets;//
struct Sets Width_D_sets;

int *get_Sets(int n,int m,int a[]);//求Ω集合,a从1开始,n表示物品种类，m为背包容量，a为物品体积
int *poscount(int* input, int len);//n个数取任意个数相加求和的个数--砂眼求和
double**** alloc4float(int n1,int n2,int n3,int n4); //动态开辟四维数组，记录中间板的最优值
void free4float(float**** p4);
double randomnum();//随机生成样例
void Build_an_Instance();
void instances_generator();
void readbatch(char filename[]);
void readdefect(char defectfilename[]);
void Main_Operators();
void Init();
void read_FILE();//循环读取多个文件
int get_G(struct rectangle obj);
int get_px(struct Sets sets);
int get_qy(struct Sets sets);
int if_pure(struct rectangle obj);
int get_F_pure(struct rectangle obj);
int get_F_conta(struct rectangle obj);
int search_pxqy(int a[],int n,int l,int r);//二分法查找pxqy
int sub_Plate(struct rectangle obj1,struct rectangle obj2,int ans);
int if_cross(struct rectangle obj,int cut_x,int cut_y);
void Free();
void Print(struct rectangle obj);//输出中间板

