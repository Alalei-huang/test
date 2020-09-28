
#include"2014_Code.h"


int count1=0;
int main()
{

    cout<<"请输入原板长度length：";
    cin>>PLATE_LENGTH;
    cout<<"请输入原板宽度width：";
    cin>>PLATE_WIDTH;
    Num_of_Instances=15;
    cout<<"请输入算法类型（DPC,DPD,QHDP）：";
    cin>>CHAR;
    read_FILE();
    double ins_OFV[6][4][10];
    double ins_time[6][4][10];
    for(int ii=1;ii<6;ii++)//5类item
    {
        for(int jj=1;jj<4;jj++)//3类尺寸，6,8,10
        {
            for(int kk=1;kk<5;kk++)//4种砂眼
            {
                clock_t start, end;
                double time,times=0;
                double count_OFV=0;
                Num_of_Defects=0;
                strcpy(filename,matrix_ins[ii][jj][kk]);
                strcpy(defectfilename,matrix_def[ii][jj][kk]);
                readbatch(filename);
                readdefect(defectfilename);
                for(int m=0;m<Num_of_Instances;m++)
                {//Num_of_Defects=0;
                    Build_an_Instance();
                    for(int i=0;i<Type_of_Items;i++)
                    {
                        item[i].length=ins[m].item[i].length;
                        item[i].width=ins[m].item[i].width;
                        item[i].area=ins[m].item[i].area;
                        item[i].v=ins[m].item[i].v;
                    }
                    for(int i=0;i<Num_of_Defects;i++)
                    {
                        defect[i].x0=ins[m].defect[i].x0;
                        defect[i].y0=ins[m].defect[i].y0;
                        defect[i].x1=ins[m].defect[i].x1;
                        defect[i].y1=ins[m].defect[i].y1;
                    }

                    Init();
                    for(int i=0;i<Type_of_Items;i++)
                    {
                        len[i]=item[i].length;
                        wid[i]=item[i].width;//cout<<wid[i]<<"d";
                        len_no_D[i]=item[i].length;
                        wid_on_D[i]=item[i].width;
                    }
                    for(int i=0;i<Num_of_Defects;i++)
                    {
                        len[Type_of_Items+i]=defect[i].x1-defect[i].x0;
                        wid[Type_of_Items+i]=defect[i].y1-defect[i].y0;//cout<<wid[Type_of_Items+i]<<" ";
                        len_D[i]=defect[i].x1-defect[i].x0;
                        wid_D[i]=defect[i].y1-defect[i].y0;
                    }
                    start = clock();//获得CUP计时单元
                    Main_Operators();//cout<<numox<<" "<<numoy<<" "<<numx<<" "<<numy<<endl;
                    end = clock();
                    time=end-start;
                    cout<<m+1<<" "<<OFV<<" "<<time/1000.0<<" ";
                    count_OFV+=OFV;
                    times+=time;//cout<<" "<<(end-start)/1000.0<<endl;
                    cout<<" "<<round(count_OFV/(m+1))<<" "<<times/1000.0/(m+1)<<endl;
                    Free();
                }
                ins_OFV[ii][jj][kk]=count_OFV/Num_of_Instances;
                ins_time[ii][jj][kk]=times/1000.0/Num_of_Instances;
                printf("ins_%d%d_%dI_%dD_%d: \n",PLATE_LENGTH,PLATE_WIDTH,5*ii,kk,4+2*jj);
                printf("Ave_OFV:%.3f\n",ins_OFV[ii][jj][kk]);
                printf("Ave_Time:%.3f\n",ins_time[ii][jj][kk]);
                char file[40];
                sprintf(file,"result_%d%d_%s.xls",PLATE_LENGTH,PLATE_WIDTH,CHAR);
                FILE *fp=fopen(file,"a");
                fprintf(fp,"%d%d%d\t%.3f\t%.3f\n",5*ii,4+2*jj,kk,ins_OFV[ii][jj][kk],ins_time[ii][jj][kk]);
                if(kk==4)fprintf(fp,"\n");
                fclose(fp);
            }
        }
    }
    for(int ii=1;ii<6;ii++)//5类item
    {
        for(int jj=1;jj<4;jj++)//3类尺寸，6,8,10
        {
            for(int kk=1;kk<5;kk++)//4种砂眼
            {
                printf("%d%d%d\t%.3f\t%.3f: \n",5*ii,4+2*jj,kk,ins_OFV[ii][jj][kk],ins_time[ii][jj][kk]);
            }
        }
    }
    int d;
    cin>>d;
//    fclose(fp);
    return 0;
}

void Build_an_Instance()
{
    item = (struct Items *)malloc(sizeof(struct Items)*25);         //动态分配Item数组，最大705个空间
    defect = (struct Defects *)malloc(sizeof(struct Defects )*10);
    Length_sets.Z = (int *)malloc(sizeof(int )*405);
    Width_sets.Z = (int *)malloc(sizeof(int )*405);
    Length_sets_no_D.Z=(int *)malloc(sizeof(int )*405);
    Width_sets_no_D.Z =(int *)malloc(sizeof(int )*405);
    Length_D_sets.Z = (int *)malloc(sizeof(int )*405);
    Width_D_sets.Z = (int *)malloc(sizeof(int )*405);
    L_sets=(int *)malloc(sizeof(int )*405);
    W_sets=(int *)malloc(sizeof(int )*405);
    L_sets_no_D=(int *)malloc(sizeof(int )*405);
    W_sets_no_D=(int *)malloc(sizeof(int )*405);
    L_D_sets=(int *)malloc(sizeof(int )*405);
    W_D_sets=(int *)malloc(sizeof(int )*405);
}


//初始化
void Init()
{
    memset(f,-1,sizeof(f));
    memset(index_ox,0,sizeof(index_ox));
    memset(index_oy,0,sizeof(index_oy));
    memset(index_x,0,sizeof(index_x));
    memset(index_y,0,sizeof(index_y));
    memset(f1,-1,sizeof(f1));
    numox=0;//栈内ox的数量
    numoy=0;
    numx=0;
    numy=0;

    object.length=PLATE_LENGTH;
    object.width=PLATE_WIDTH;
    object.x=0;
    object.y=0;
    min_Length=item[0].length;
    min_Width=item[0].width;
    min_item_Length=item[0].length;
    min_item_Width=item[0].width;
    min_Defect_Length=defect[0].x1-defect[0].x0;
    min_Defect_Width=defect[0].y1-defect[0].y0;
    //计算所有item的最小值
    for(int i=1;i<Type_of_Items;i++)
    {
        if(item[i].length<min_Length)min_Length=item[i].length;
        if(item[i].width<min_Width)min_Width=item[i].width;
        if(item[i].length<min_item_Length)min_item_Length=item[i].length;
        if(item[i].width<min_item_Width)min_item_Width=item[i].width;
    }
    for(int i=0;i<Num_of_Defects;i++)
    {
        if(defect[i].x1-defect[i].x0<min_Length)min_Length=defect[i].x1-defect[i].x0;
        if(defect[i].y1-defect[i].y0<min_Width)min_Width=defect[i].y1-defect[i].y0;
        if(defect[i].x1-defect[i].x0<min_Defect_Length)min_Defect_Length=defect[i].x1-defect[i].x0;
        if(defect[i].y1-defect[i].y0<min_Defect_Width)min_Defect_Width=defect[i].y1-defect[i].y0;
    }
    //初始化f1函数
    for(int i=0;i<Type_of_Items;i++)
    {
        int cc=1;
        for(int j=item[i].length;j<=object.length;j+=item[i].length)
        {
            int ccc=1;
            for(int k=item[i].width;k<=object.width;k+=item[i].width)
            {
                if(f1[j][k]<(ccc)*(cc)*item[i].area)
                    f1[j][k]=(ccc++)*(cc)*item[i].area;//cout<<i<<" "<<j<<" "<<k<<" "<<f1[j][k]<<endl;
            }
            cc++;
        }
    }
}

//循环读取多个文件
void read_FILE()
{
    char fname_ins[40]="",fname_def[40]="";
    for(int i = 1; i < 6; i++)
        for(int j = 1; j < 4; j++)
            for(int k=1;k<9;k++)
            {
                sprintf(fname_ins, "ins_%d%d_%dI_%dD_%d.xls",PLATE_LENGTH,PLATE_WIDTH, 5*i,k,4+2*j );//过命名规则生成文件名fname_ins
                sprintf(fname_def, "def_%d%d_%dI_%dD_%d.xls", PLATE_LENGTH,PLATE_WIDTH,5*i,k,4+2*j );
                strcat(matrix_ins[i][j][k],fname_ins);
                strcat(matrix_def[i][j][k],fname_def);
            }
}

//读取item信息
void readbatch(char filename[])
{
    int cas=0,cas1=0;
    ifstream fin(filename,std::ifstream::in);
    string str;
    while(getline(fin,str))   //读取每一行数据
    {
        istringstream Substr(str);//流
        if(strcmp(str.c_str(),"\n")==-1)
        {
            cas++;
            Type_of_Items=cas1;
            cas1=0;
            continue;
        }
        string it;                //it为一行中每一个数字，用string表示
        int num[6],count1=0;
        while(getline(Substr,it,'\t'))
        {
            num[count1++] = atoi(it.c_str());//将string转char再转int
        }
        ins[cas].item[cas1].length = num[0];
        ins[cas].item[cas1].width = num[1];
        ins[cas].item[cas1].v=1;
        ins[cas].item[cas1].area=num[2];
        //cout<<num[0]<<" "<<num[1]<<" "<<num[2]<<endl;
        //cout<<cas<<" "<<ins[cas].item[Type_of_Items].length<<" "<<ins[cas].item[Type_of_Items].width<<endl;
        cas1++;
    }

//    cout<<Type_of_Items<<endl;//exit(1);
//    for(int i=0;i<Num_of_Instances;i++)
//    {
//        for(int j=0;j<Type_of_Items;j++)
//        {
//            cout<<i<<" "<<ins[i].item[j].length<<" "<<ins[i].item[j].width<<endl;
//        }cout<<endl;
//    }
//    for(int i=0;i<Type_of_Items;i++)cout<<item[i].area<<endl;
    fin.close();
}

//读取defect信息
void readdefect(char defectfilename[])
{
    int cas=0,cas1=0;
    ifstream fin(defectfilename,std::ifstream::in);
    string str;
    while(getline(fin,str))   //读取每一行数据
    {//cout<<str<<endl;
        istringstream Substr(str);//流
        if(strcmp(str.c_str(),"\n")==-1)
        {
            cas++;
            Num_of_Defects=cas1;
            cas1=0;
            continue;
        }
        string it;                //it为一行中每一个数字，用string表示
        int num[6],count1=0;
        while(getline(Substr,it,'\t'))
        {
            num[count1++] = atoi(it.c_str());//将string转char再转int
        }
        ins[cas].defect[cas1].x0 = num[0];
        ins[cas].defect[cas1].y0 = num[1];
        ins[cas].defect[cas1].x1 = num[2];
        ins[cas].defect[cas1].y1 = num[3];
        //cout<<num[0]<<" "<<num[1]<<" "<<num[2]<<" "<<num[3]<<endl;
        cas1++;
    }//Num_of_Defects=0;
//    cout<<Num_of_Instances<<endl;
//    cout<<Num_of_Defects<<endl;
//    for(int i=0;i<Num_of_Instances;i++)
//    {
//        for(int j=0;j<Num_of_Defects;j++)
//        {
//            cout<<ins[i].defect[j].x0<<endl;
//        }cout<<endl;
//    }
//    cout<<Num_of_Defects;
    fin.close();
}

//获得p(x)
int get_px(struct Sets sets)
{
    int px=sets.Z[0];
    for(int i=1;i<sets.length;i++)
    {
        if(px<sets.Z[i])px=sets.Z[i];
    }
    return px;
}

//获得q(y)
int get_qy(struct Sets sets)
{
    int qy=sets.Z[0];
    for(int i=1;i<sets.length;i++)
    {
        if(qy<sets.Z[i])qy=sets.Z[i];
    }
    return qy;
}

//求g(x,y),当前中间板全放一种item
int get_G(struct rectangle obj)
{//cout<<"abcd:"<<obj.x<<" "<<obj.y<<" "<<obj.length<<" "<<obj.width<<endl;
    int maxvalue=0;
    int aaa;
    for(int i=0;i<Type_of_Items;i++)
    {
        if(item[i].length<=obj.length&&item[i].width<=obj.width)//item能放进当前中间板
        {
            int value1=(obj.length/item[i].length);
            int value2=(obj.width/item[i].width);
            int value=value1*value2*item[i].area*item[i].v;//cout<<"ABCD:"<<item[i].v<<" "<<value1<<" "<<value2<<" "<<value<<endl;
            if(value>maxvalue)
            {aaa=i;
                maxvalue=value;
            }
        }
    }//cout<<"maxvalue:"<<maxvalue<<"  aaa:"<<aaa<<endl;
    return maxvalue;
}

//求无砂眼时F(x,y)=max[g(x,y),F(zx,y)+f(x-zx,y),F(x,zy)+F(x,y-zy)]
int get_F_pure(struct rectangle obj)
{//cout<<"pureobj:"<<obj.x<<" "<<obj.y<<" "<<obj.length<<" "<<obj.width<<endl;
    //if(f1[obj.length][obj.width]>0)return f1[obj.length][obj.width];
    if(obj.length<min_item_Length||obj.width<min_item_Width)
    {
        //f1[obj.length][obj.width]=0;
        return 0;
    }
    for(int i=0;i<Type_of_Items;i++)
    {
        if(obj.length==item[i].length&&obj.width==item[i].width)
        {
            return item[i].area;
        }
    }
    int ans=get_G(obj);//cout<<"ans:"<<ans<<" x:"<<obj.x<<" y:"<<obj.y<<endl;

    int flag1=0;
    //x方向试切F(zx,y)+f(x-zx,y)
    for(int i=0;Length_sets_no_D.Z[i]<=obj.length/2;i++)
    {
//        if(Length_sets_no_D.Z[i]<min_item_Length)continue;
//        if(Length_sets_no_D.Z[i]>obj.length-min_Length)continue;
        struct rectangle sub_obj1;
        struct rectangle sub_obj2;
        sub_obj1.x=obj.x;
        sub_obj1.y=obj.y;
        sub_obj1.length=Length_sets_no_D.Z[i];
        sub_obj1.width=obj.width;
        int px=obj.length-sub_obj1.length;
        for(int j=Length_sets_no_D.length-1;j>=0;j--)
        {
            if(Length_sets_no_D.Z[j]<=px)
            {
                px=Length_sets_no_D.Z[j];
                break;
            }
        }
//        int qy=get_qy(obj.width-Width_sets.Z[i]);
        sub_obj2.x=obj.x+Length_sets_no_D.Z[i];
        sub_obj2.y=obj.y;
        sub_obj2.length=px;
        sub_obj2.width=obj.width;
        int ans1;
        int ans2;
        if(f1[sub_obj1.length][sub_obj1.width]!=-1)
        {
            ans1=f1[sub_obj1.length][sub_obj1.width];
            //cout<<sub_obj1.x<<" "<<sub_obj1.y<<" "<<sub_obj1.length<<" "<<sub_obj1.width<<" "<<ans1<<endl;
        }
        else
        {
            ans1=get_F_pure(sub_obj1);
            f1[sub_obj1.length][sub_obj1.width]=ans1;
        }
        if(f1[sub_obj2.length][sub_obj2.width]!=-1)
        {
            ans2=f1[sub_obj2.length][sub_obj2.width];
        }
        else
        {
            ans2=get_F_pure(sub_obj2);
            f1[sub_obj2.length][sub_obj2.width]=ans2;
        }
        if(ans<ans1+ans2)
        {
            flag1=1;
            ans=ans1+ans2;
        }
    }
    //y方向试切F(x,zy)+F(x,y-zy)
    for(int i=0;Width_sets_no_D.Z[i]<=obj.width/2;i++)
    {//cout<<i<<" "<<Width_sets_no_D.Z[i]<<endl;
//        if(Width_sets_no_D.Z[i]<min_item_Width)continue;
//        if(Width_sets_no_D.Z[i]>obj.width-min_Width)continue;
        struct rectangle sub_obj1;
        struct rectangle sub_obj2;
        sub_obj1.x=obj.x;
        sub_obj1.y=obj.y;
        sub_obj1.length=obj.length;
        sub_obj1.width=Width_sets_no_D.Z[i];
        int qy=obj.width-Width_sets_no_D.Z[i];//cout<<"pureobj1_Y:"<<sub_obj1.x<<" "<<sub_obj1.y<<" "<<sub_obj1.length<<" "<<sub_obj1.width<<endl;
        for(int j=Width_sets_no_D.length-1;j>=0;j--)
        {
            if(Width_sets_no_D.Z[j]<=qy)
            {
                qy=Width_sets_no_D.Z[j];
                break;
            }
        }
        sub_obj2.x=obj.x;
        sub_obj2.y=obj.y+Width_sets_no_D.Z[i];
        sub_obj2.length=obj.length;
        sub_obj2.width=qy;//cout<<"pureobj2_Y:"<<sub_obj2.x<<" "<<sub_obj2.y<<" "<<sub_obj2.length<<" "<<sub_obj2.width<<endl;
        int ans1;
        int ans2;
        if(f1[sub_obj1.length][sub_obj1.width]!=-1)
        {
            ans1=f1[sub_obj1.length][sub_obj1.width];
        }
        else
        {
            ans1=get_F_pure(sub_obj1);
            f1[sub_obj1.length][sub_obj1.width]=ans1;
        }
        if(f1[sub_obj2.length][sub_obj2.width]!=-1)
        {
            ans2=f1[sub_obj2.length][sub_obj2.width];
        }
        else
        {
            ans2=get_F_pure(sub_obj2);
            f1[sub_obj2.length][sub_obj2.width]=ans2;
        }
        if(ans<ans1+ans2)
        {
            flag1=1;
            ans=ans1+ans2;
            //f[obj.x][obj.y][obj.length][obj.width]=ans;
        }
    }//cout<<ans<<endl;
    //f1[obj.length][obj.width]=ans;
    //if(f1[obj.length][obj.width]==get_G(obj)&&f1[obj.length][obj.width]!=0)cout<<"x:"<<obj.x<<" y:"<<obj.y<<" length:"<<obj.length<<" width:"<<obj.width<<" "<<get_G(obj)<<" "<<f1[obj.length][obj.width]<<endl;
    return ans;
}

int search_pxqy(int a[],int n,int l,int r)
{
    int pxqy=0;
    int mid;
    while(l<=r)
    {
        mid=(l+r)/2;
        if(n==a[mid])return n;
        if(a[mid]>n)r=mid-1;
        if(a[mid]<n)
        {
            pxqy=a[mid];
            l=mid+1;
        }
    }
    return pxqy;
}

/*求有砂眼时max[F(ox,oy,zx,y)+F(ox+zx,oy,x-zx,y),
                F(ox,oy,x-zx,y)+F(ox+x-zx,oy,zx,y),
                F(ox,oy,x,zy)+F(ox,oy+zy,x,y-zy),
                F(ox,oy,x,y-zy)+F(ox,oy+y-zy,zy,y)]
*/
int get_F_conta(struct rectangle obj)
{//cout<<"obj:"<<obj.x<<" "<<obj.y<<" "<<obj.length<<" "<<obj.width<<endl;
    if(f[obj.x][obj.y][obj.length][obj.width]>-1)return f[obj.x][obj.y][obj.length][obj.width];
    //当前中间板尺寸小于任意item，返回0
    if(obj.length<min_item_Length||obj.width<min_item_Width)
    {
        return 0;
    }
    //当前中间板尺寸大于某个item，但是由于砂眼导致不能割
    for(int i=0;i<Num_of_Defects;i++)
    {
         if((defect[i].x0>=obj.x&&defect[i].x0<=obj.x+obj.length&&defect[i].y0>=obj.y&&defect[i].y0<=obj.y+obj.width)||
           (defect[i].x1>=obj.x&&defect[i].x1<=obj.x+obj.length&&defect[i].y0>=obj.y&&defect[i].y0<=obj.y+obj.width)||
           (defect[i].x0>=obj.x&&defect[i].x0<=obj.x+obj.length&&defect[i].y1>=obj.y&&defect[i].y1<=obj.y+obj.width)||
           (defect[i].x1>=obj.x&&defect[i].x1<=obj.x+obj.length&&defect[i].y1>=obj.y&&defect[i].y1<=obj.y+obj.width))//砂眼任意一个角在当前中间板上
        {
            int f=0;
            for(int j=0;j<Type_of_Items;j++)
            {
                if((item[j].length<=defect[i].x0-obj.x&&item[j].width<=obj.width)
                ||(item[j].length<=obj.x+obj.length-defect[i].x1&&item[j].width<=obj.width)
                ||(item[j].width<=defect[i].y0-obj.y&&item[j].length<=obj.length)
                ||(item[j].width<=obj.y+obj.width-defect[i].y1&&item[j].length<=obj.length))
                {
                    f=1;break;
                }
            }
            if(f==0)
            {
                return 0;
            }
        }
    }
    int ans=0;
    int flag1=0;
    int flag2=0;

    //x方向从左往右F(ox,oy,zx,y)+F(ox+zx,oy,x-zx,y)
     for(int i=0;i<Length_sets.length&&Length_sets.Z[i]<obj.length;i++)
    {
//        if(Length_sets.Z[i]<first_cut)continue;
//        if(Length_sets.Z[i]<min_Length)continue;
        //if(Length_sets.Z[i]>obj.length-min_item_Length/2)break;
        if(strcmp(CHAR,"DPD")==0&&Length_sets.Z[i]>obj.length-min_Length)break;
        //if(Length_sets.Z[i]>obj.length-min_Length)break;
//        if(if_cross(obj,Length_sets.Z[i],0)==0)continue;
        //左边子板F(ox,oy,zx,y)
        struct rectangle sub_obj1;//cout<<Length_sets.Z[i]<<" ";
        sub_obj1.x=obj.x;
        sub_obj1.y=obj.y;
        sub_obj1.length=Length_sets.Z[i];
        sub_obj1.width=obj.width;
        int px;
        if(strcmp(CHAR,"DPD")==0)
            px=search_pxqy(Length_sets.Z,obj.length-Length_sets.Z[i],0,Length_sets.length-1);
        else
        {
            px=obj.length-Length_sets.Z[i];
        }

//        for(int j=Length_sets.length-1;j>=0;j--)
//        {
//            if(Length_sets.Z[j]<=px)
//            {
//                px=Length_sets.Z[j];
//                break;
//            }
//        }
        struct rectangle sub_obj2;
        sub_obj2.x=obj.x+Length_sets.Z[i];
        sub_obj2.y=obj.y;
        sub_obj2.length=px;
        sub_obj2.width=obj.width;
        flag2=1;
        ans=sub_Plate(sub_obj1,sub_obj2,ans);
    }
    //y方向从下往上F(ox,oy,x,zy)+F(ox,oy+zy,x,y-zy)
    for(int i=0;i<Width_sets.length&&Width_sets.Z[i]<obj.width;i++)
    {
//        if(Width_sets.Z[i]<first_cut)continue;
//        if(Width_sets.Z[i]<min_Width)continue;
//        if(Width_sets.Z[i]>obj.width-min_item_Width)break;
        if(strcmp(CHAR,"DPD")==0&&Width_sets.Z[i]>obj.width-min_Width)break;
//if(Width_sets.Z[i]>obj.width-min_Width)break;
//        if(if_cross(obj,0,Width_sets.Z[i])==0)continue;
        //下边子板F(ox,oy,x,zy)
        struct rectangle sub_obj1;
        sub_obj1.x=obj.x;
        sub_obj1.y=obj.y;
        sub_obj1.length=obj.length;
        sub_obj1.width=Width_sets.Z[i];
        int qy;
        if(strcmp(CHAR,"DPD")==0)
        {
            qy=search_pxqy(Width_sets.Z,obj.width-Width_sets.Z[i],0,Width_sets.length-1);
        }
        else
        {
            qy=obj.width-Width_sets.Z[i];
        }
//        for(int j=Width_sets.length-1;j>=0;j--)
//        {
//            if(Width_sets.Z[j]<=qy)
//            {
//                qy=Width_sets.Z[j];break;
//            }
//        }
        //上边子板F(ox,oy+zy,x,y-zy)
        struct rectangle sub_obj2;
        sub_obj2.x=obj.x;
        sub_obj2.y=obj.y+Width_sets.Z[i];
        sub_obj2.length=obj.length;
        sub_obj2.width=qy;
flag2=1;
        ans=sub_Plate(sub_obj1,sub_obj2,ans);//cout<<" "<<ans<<endl;
    }

    if(strcmp(CHAR,"DPD")==0||strcmp(CHAR,"QHDP")==0)
    {
        //x方向从右往左F(ox,oy,x-zx,y)+F(ox+x-zx,oy,zx,y)
        for(int i=0;i<Length_sets.length&&Length_sets.Z[i]<obj.length;i++)
        {
    //        if(Length_sets.Z[i]<first_cut)continue;
            //if(Length_sets.Z[i]>obj.length-min_item_Length/2)break;
            if(strcmp(CHAR,"DPD")==0&&Length_sets.Z[i]>obj.length-min_Length)break;
//            if(if_cross(obj,obj.length-Length_sets.Z[i],0)==0)continue;
//if(Length_sets.Z[i]>obj.length-min_Length)break;
//int px=search_pxqy(Length_sets.Z,obj.length-Length_sets.Z[i],0,Length_sets.length-1);
            int px;
            if(strcmp(CHAR,"DPD")==0)
                px=search_pxqy(Length_sets.Z,obj.length-Length_sets.Z[i],0,Length_sets.length-1);
            else
            {
                px=obj.length-Length_sets.Z[i];
            }
//            for(int j=Length_sets.length-1;j>=0;j--)
//            {
//                if(Length_sets.Z[j]<=px)
//                {
//                    px=Length_sets.Z[j];break;
//                }
//            }
            //左边子板F(ox,oy,x-zx,y)
            struct rectangle sub_obj1;
            sub_obj1.x=obj.x;
            sub_obj1.y=obj.y;
            sub_obj1.length=px;
            sub_obj1.width=obj.width;

            //右边子板F(ox+x-zx,oy,zx,y)
            struct rectangle sub_obj2;
            sub_obj2.x=obj.x+obj.length-Length_sets.Z[i];
            sub_obj2.y=obj.y;
            sub_obj2.length=Length_sets.Z[i];
            sub_obj2.width=obj.width;
            flag2=1;
            ans=sub_Plate(sub_obj1,sub_obj2,ans);
        }
    }

    if(strcmp(CHAR,"DPD")==0||strcmp(CHAR,"QHDP")==0)
    {
            //y方向从上往下F(ox,oy,x,y-zy)+F(ox,oy+y-zy,x,zy)
        for(int i=0;i<Width_sets.length&&Width_sets.Z[i]<obj.width;i++)
        {
    //        if(Width_sets.Z[i]<first_cut)continue;
            //if(Width_sets.Z[i]>obj.width-min_item_Width/2)break;
            if(strcmp(CHAR,"DPD")==0&&Width_sets.Z[i]>obj.width-min_Width)break;
//            if(if_cross(obj,0,obj.width-Width_sets.Z[i])==0)continue;
//if(Width_sets.Z[i]>obj.width-min_Width)break;
// int qy=search_pxqy(Width_sets.Z,obj.width-Width_sets.Z[i],0,Width_sets.length-1);
            int qy;
            if(strcmp(CHAR,"DPD")==0)
            {
                qy=search_pxqy(Width_sets.Z,obj.width-Width_sets.Z[i],0,Width_sets.length-1);
            }
            else
            {
                qy=obj.width-Width_sets.Z[i];
            }
//            for(int j=Width_sets.length-1;j>=0;j--)
//            {
//                if(Width_sets.Z[j]<=qy)
//                {
//                    qy=Width_sets.Z[j];break;
//                }
//            }
            //下边子板F(ox,oy,x,y-zy)
            struct rectangle sub_obj1;
            sub_obj1.x=obj.x;
            sub_obj1.y=obj.y;
            sub_obj1.length=obj.length;
            sub_obj1.width=qy;

            struct rectangle sub_obj2;
            sub_obj2.x=obj.x;
            sub_obj2.y=obj.y+obj.width-Width_sets.Z[i];
            sub_obj2.length=obj.length;
            sub_obj2.width=Width_sets.Z[i];
            flag2=1;
            ans=sub_Plate(sub_obj1,sub_obj2,ans);//cout<<ans<<endl;
        }
    }
    if(strcmp(CHAR,"QHDP")==0)
    {
        int cut_x[20],cut_y[20];
        int Num_cut_x=0,Num_cut_y=0;
        for(int i=0;i<Num_of_Defects;i++)//获取当前中间板砂眼的位置
        {
            if(!(defect[i].x0>=obj.x+obj.length||defect[i].x1<=obj.x||defect[i].y0>=obj.y+obj.width||defect[i].y1<=obj.y))
            {
                if(defect[i].x0>obj.x&&defect[i].x0<obj.x+obj.length)cut_x[Num_cut_x++]=defect[i].x0-obj.x;
                if(defect[i].x1<obj.x+obj.length&&defect[i].x1>obj.x)cut_x[Num_cut_x++]=defect[i].x1-obj.x;
                if(defect[i].y0>obj.y&&defect[i].y0<obj.y+obj.width)cut_y[Num_cut_y++]=defect[i].y0-obj.y;
                if(defect[i].y1<obj.y+obj.width&&defect[i].y1>obj.y)cut_y[Num_cut_y++]=defect[i].y1-obj.y;
            }
        }
        //挨着砂眼竖切
        for(int i=0;i<Num_cut_x;i++)
        {
            //下边子板F(ox,oy,x,zy)
            struct rectangle sub_obj1;
            sub_obj1.x=obj.x;
            sub_obj1.y=obj.y;
            sub_obj1.length=cut_x[i];
            sub_obj1.width=obj.width;

            //上边子板F(ox,oy+zy,x,y-zy)
            struct rectangle sub_obj2;
            sub_obj2.x=obj.x+cut_x[i];
            sub_obj2.y=obj.y;
            sub_obj2.length=obj.length-cut_x[i];
            sub_obj2.width=obj.width;
            flag2=1;
            ans=sub_Plate(sub_obj1,sub_obj2,ans);
        }
        //挨着砂眼横切
        for(int i=0;i<Num_cut_y;i++)
        {
            //下边子板F(ox,oy,x,zy)
            struct rectangle sub_obj1;
            sub_obj1.x=obj.x;
            sub_obj1.y=obj.y;
            sub_obj1.length=obj.length;
            sub_obj1.width=cut_y[i];

            //上边子板F(ox,oy+zy,x,y-zy)
            struct rectangle sub_obj2;
            sub_obj2.x=obj.x;
            sub_obj2.y=obj.y+cut_y[i];
            sub_obj2.length=obj.length;
            sub_obj2.width=obj.width-cut_y[i];
            flag2=1;
            ans=sub_Plate(sub_obj1,sub_obj2,ans);
        }
    }

    if(flag2==0)return 0;
    return ans;
}

int sub_Plate(struct rectangle sub_obj1,struct rectangle sub_obj2,int ans)
{
//    cout<<"obj1:"<<sub_obj1.x<<" "<<sub_obj1.y<<" "<<sub_obj1.length<<" "<<sub_obj1.width<<endl;
//cout<<"obj2:"<<sub_obj2.x<<" "<<sub_obj2.y<<" "<<sub_obj2.length<<" "<<sub_obj2.width<<endl;
    int ans1;
    int ans2;
    if(if_pure(sub_obj1)==0)//如果该中间板有砂眼
    {
        if(f[sub_obj1.x][sub_obj1.y][sub_obj1.length][sub_obj1.width]>-1)
        {
            ans1=f[sub_obj1.x][sub_obj1.y][sub_obj1.length][sub_obj1.width];
        }
        else
        {
            ans1=get_F_conta(sub_obj1);
            f[sub_obj1.x][sub_obj1.y][sub_obj1.length][sub_obj1.width]=ans1;
        }
    }
    else
    {
        if(f1[sub_obj1.length][sub_obj1.width]>-1)
        {
            ans1=f1[sub_obj1.length][sub_obj1.width];
        }
        else
        {
            ans1=get_F_pure(sub_obj1);
            f1[sub_obj1.length][sub_obj1.width]=ans1;
        }
    }
    if(if_pure(sub_obj2)==0)
    {
        if(f[sub_obj2.x][sub_obj2.y][sub_obj2.length][sub_obj2.width]>-1)
        {
            ans2=f[sub_obj2.x][sub_obj2.y][sub_obj2.length][sub_obj2.width];
        }
        else
        {
            ans2=get_F_conta(sub_obj2);
            f[sub_obj2.x][sub_obj2.y][sub_obj2.length][sub_obj2.width]=ans2;
        }
    }
    else
    {
        if(f1[sub_obj2.length][sub_obj2.width]>-1)
        {
            ans2=f1[sub_obj2.length][sub_obj2.width];
        }
        else
        {
            ans2=get_F_pure(sub_obj2);
            f1[sub_obj2.length][sub_obj2.width]=ans2;
        }
    }
    if(ans<ans1+ans2)
    {
        ans=ans1+ans2;
    }
    return ans;
}

//判断cut线是否穿过砂眼，穿过返回1，不穿过返回0
int if_cross(struct rectangle obj,int cut_x,int cut_y)
{
    if(cut_y==0)//判断x是否穿过砂眼
    {
        for(int i=0;i<Num_of_Defects;i++)
        {
            if(defect[i].x0>=obj.x&&defect[i].x1<=obj.x+obj.length&&defect[i].y0>=obj.y&&defect[i].y1<=obj.y+obj.width)
            {
                if(obj.x+cut_x>defect[i].x0&&obj.x+cut_x<defect[i].x1)
                {
                    return 1;
                }
            }
        }
    }
    if(cut_x==0)//判断y是否穿过砂眼
    {
        for(int i=0;i<Num_of_Defects;i++)
        {
            if(defect[i].x0>=obj.x&&defect[i].x1<=obj.x+obj.length&&defect[i].y0>=obj.y&&defect[i].y1<=obj.y+obj.width)
            {
                if(obj.y+cut_y>defect[i].y0&&obj.y+cut_y<defect[i].y1)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

//判断中间板是否存在砂眼,flag=1为pure plate,flag=0为有砂眼
int if_pure(struct rectangle obj)
{
    int flag=0;//cout<<Num_of_Defects;
    for(int i=0;i<Num_of_Defects;i++)
    {
        if(defect[i].x1<=obj.x||defect[i].x0>=obj.x+obj.length||defect[i].y0>=obj.y+obj.width||defect[i].y1<=obj.y)
        {
            flag++;
        }
    }
    if(flag==Num_of_Defects)return 1;
    else return 0;

}

//求max[g(x,y),F(zx,y)+f(p(x-zx),y),F(x,zy)+F(x,q(y-zy))]中的最大值
int max_F(int g,int fx,int fy)
{
    int maxv=g;
    if(fx>maxv)maxv=fx;
    if(fy>maxv)maxv=fy;
    return maxv;
}

void Main_Operators()
{
    //求离散集
    if(strcmp(CHAR,"DPC")==0)
    {
        //增量为1（DPC）
        for(int i=1;i<=object.length;i++)
        {
            Length_sets.Z[i-1]=i;
            Length_sets_no_D.Z[i-1]=i;
        }
        Length_sets.length=object.length;
        Length_sets_no_D.length=object.length;
        for(int j=1;j<=object.width;j++)
        {
            Width_sets.Z[j-1]=j;
            Width_sets_no_D.Z[j-1]=j;
        }
        Width_sets.length=object.width;
        Width_sets_no_D.length=object.width;
    }
    else if(strcmp(CHAR,"DPD")==0)
    {
        //离散级含砂眼（DPD）
    //获取Ωx(x)集
        sets=0;
        L_sets=get_Sets(Type_of_Items+Num_of_Defects,object.length,len);//Ωx集---带砂眼
        for(int i=0;i<sets;i++)
        {
            Length_sets.Z[i]=L_sets[i];//cout<<L_sets[i]<<" ";
        }//cout<<endl<<endl;
        Length_sets.length=sets;
        sets=0;
        L_sets_no_D=get_Sets(Type_of_Items,object.length,len_no_D);//Ωx集---不带砂眼
        for(int i=0;i<sets;i++)
        {
            Length_sets_no_D.Z[i]=L_sets_no_D[i];//cout<<L_sets_no_D[i]<<" ";
        }//cout<<endl<<endl;
        Length_sets_no_D.length=sets;

        //获取Ωy(y)集
        sets=0;
        W_sets=get_Sets(Type_of_Items+Num_of_Defects,object.width,wid);//Ωy集---带砂眼
        for(int i=0;i<sets;i++)
        {
            Width_sets.Z[i]=W_sets[i];//cout<<W_sets[i]<<" ";
        }//cout<<endl<<endl;
        Width_sets.length=sets;
        sets=0;
        W_sets_no_D=get_Sets(Type_of_Items,object.width,wid_on_D);//Ωy集---不带砂眼
        for(int i=0;i<sets;i++)
        {
            Width_sets_no_D.Z[i]=W_sets_no_D[i];//cout<<W_sets_no_D[i]<<" ";
        }//cout<<endl<<endl;
        Width_sets_no_D.length=sets;
    }
    else if(strcmp(CHAR,"QHDP")==0)
    {
        sets=0;
        L_sets=get_Sets(Type_of_Items,object.length,len_no_D);//Ωx集---带砂眼
        for(int i=0;i<sets;i++)
        {
            Length_sets.Z[i]=L_sets[i];//cout<<L_sets[i]<<" ";
        }//cout<<endl<<endl;
        Length_sets.length=sets;
        sets=0;
        L_sets_no_D=get_Sets(Type_of_Items,object.length,len_no_D);//Ωx集---不带砂眼
        for(int i=0;i<sets;i++)
        {
            Length_sets_no_D.Z[i]=L_sets_no_D[i];//cout<<L_sets_no_D[i]<<" ";
        }//cout<<endl<<endl;
        Length_sets_no_D.length=sets;

        //获取Ωy(y)集
        sets=0;
        W_sets=get_Sets(Type_of_Items,object.width,wid_on_D);//Ωy集---带砂眼
        for(int i=0;i<sets;i++)
        {
            Width_sets.Z[i]=W_sets[i];//cout<<W_sets[i]<<" ";
        }//cout<<endl<<endl;
        Width_sets.length=sets;
        sets=0;
        W_sets_no_D=get_Sets(Type_of_Items,object.width,wid_on_D);//Ωy集---不带砂眼
        for(int i=0;i<sets;i++)
        {
            Width_sets_no_D.Z[i]=W_sets_no_D[i];//cout<<W_sets_no_D[i]<<" ";
        }//cout<<endl<<endl;
        Width_sets_no_D.length=sets;
    }


//    sets=0;
//    L_sets=get_Sets(Type_of_Items+Num_of_Defects,object.length,len);//Ωx集---带砂眼
//    for(int i=0;i<sets;i++)
//    {
//        All_length[L_sets[i]]=1;
//        All_length[PLATE_LENGTH-L_sets[i]]=1;
//    }//cout<<endl<<endl;
//    sets=0;
//    for(int i=1;i<=PLATE_LENGTH;i++)
//    {
//        if(All_length[i]==1)
//        {
//            Length_sets.Z[sets]=i;
//            Length_sets_no_D.Z[sets]=i;
//            sets++;
//        }
//    }
//    Length_sets.length=sets;
//    Length_sets_no_D.length=sets;
//
//
//    //获取Ωy(y)集
//    sets=0;
//    W_sets=get_Sets(Type_of_Items+Num_of_Defects,object.width,wid);//Ωy集---带砂眼
//    for(int i=0;i<sets;i++)
//    {
//        All_width[W_sets[i]]=1;
//        All_width[PLATE_WIDTH-W_sets[i]]=1;
//    }//cout<<endl<<endl;
//    sets=0;
//    for(int i=1;i<=PLATE_WIDTH;i++)
//    {
//        if(All_width[i]==1)
//        {
//            Width_sets.Z[sets]=i;
//            Width_sets_no_D.Z[sets]=i;
//            sets++;
//        }
//    }
//    Width_sets.length=sets;
//    Width_sets_no_D.length=sets;





    //增量为2
//    sets=0;
//    for(int i=1;i<=object.length;i+=2)
//    {
//        Length_sets.Z[i/2]=i;
//        Length_sets_no_D.Z[i/2]=i;
//        sets++;
//    }
//    Length_sets.length=sets;
//    Length_sets_no_D.length=sets;
//    sets=0;
//    for(int j=1;j<=object.width;j+=2)
//    {
//        Width_sets.Z[j/2]=j;
//        Width_sets_no_D.Z[j/2]=j;
//        sets++;
//    }
//    Width_sets.length=sets;
//    Width_sets_no_D.length=sets;

    if(Num_of_Defects==0)
    {
        OFV=get_F_pure(object);
    }
    else
    {
        OFV=get_F_conta(object);
    }
}

//求Ω集合,a从1开始,n表示物品种类，m为背包容量，a为物品体积
int *get_Sets(int n,int m,int a[])
{
    memset(b,0,sizeof(b));
    //for(int i=1;i<m;i++)f[i]=0;
    b[0]=1;
    for(int i=0;i<n;i++)
    {//cout<<a[i]<<" ";
        for(int j=b[i];j<=m;j++)
        {
            b[j]+=b[j-a[i]];
        }
    }
    for(int i=1;i<m;i++)
        if(b[i]!=0)
        {
            o[sets++]=i; //将Zx保存至o数组中
        }
    return o;
}

void Free()
{
    free(defect);
    free(L_sets);
    free(W_sets);
    free(L_sets_no_D);
    free(W_sets_no_D);
}
