#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#define maxsize 1000 //磁道访问序列数

//用l（left）和r（right）标志来扫描（有序）数组，模仿快排，这个思想很好。

//使用户选择
int geti()
{
    char ch;
    int i = 0;
    fflush(stdin);
    ch = getchar();
    while(ch == '\n')
    {
        printf("不能为空，请重新输入非负整数！\n");
        fflush(stdin);
        ch = getchar();
    }
    while(ch != '\n')
    {
        if(ch > '9' || ch < '0')
        {
            printf("输入错误，请重新输入非负整数！\n");
            return geti();
        }
        else
        {
            i =  i*10 + (ch - '0');
            ch = getchar();
        }
    }
    return i;
}

/*********************冒泡排序算法**************************/
int *bubble(int cidao[],int m)
{
    int i,j;
    int temp;
    for(i=0; i<m; i++)  //使用冒泡法按从小到大顺序排列
        for(j=i+1; j<m; j++)
        {
            if(cidao[i]>cidao[j])
            {
                temp=cidao[i];
                cidao[i]=cidao[j];
                cidao[j]=temp;
            }
        }
    return cidao;
}
/*********************先来先服务调度算法************************/
void FCFS(int cidao[],int m,int now)   //磁道号数组，个数为m
{
    int sum=0;   //总寻道长度
    int j,i;
    float ave;   //平均寻道长度
    sum+=abs(cidao[0]-now);
    printf("磁盘扫描序列为：");
    for( i=0; i<m; i++) //输出磁盘扫描序列
    {
        printf("%d ", cidao[i]);
    }
    for(i=0,j=1; j<m; i++,j++) //求平均寻道长度
    {
        sum+=abs(cidao[j]-cidao[i]);
        ave=(float)(sum)/(float)(m);
    }
    printf("\n");
    printf("平均移动磁道数：%f\n\n按回车键进行最短寻道时间优先算法调度\n", ave);
}

/**********************最短寻道时间优先调度算法********************/
//这个算法的思路最新颖~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SSTF(int cidao[],int m,int now)
{
    int k=1;
    int l,r;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //调用冒泡排序算法排序
    printf("磁盘扫描序列为：");
    if(cidao[m-1]<=now)   //若当前磁道号大于请求序列中最大者，则直接由外向内依次给予各请求服务
    {
        for(i=m-1; i>=0; i--)
            printf("%d ", cidao[i]);
        sum=now-cidao[0];
    }
    if(cidao[0]>=now)   //若当前磁道号小于请求序列中最小者，则直接由内向外依次给予各请求服务
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])   //若当前磁道号大于请求序列中最小者且小于最大者
    {
        while(cidao[k]<now)    //确定当前磁道在已排的序列中的位置，后面的算法都用到了，可以直接复制后少量修改，节省时间。
        {
            k++;
        }//找到等于大于当前磁道的位置

        //用左右两个标记来扫描有序数组（从中间向左右）
        l=k-1;//当前左
        r=k;//当前或当前右
        while((l>=0)&&(r<m))   //当前磁道在请求序列范围内
        {
            //选择与当前磁道最近的请求给予服务
            if((now-cidao[l])<=(cidao[r]-now))//左近
            {
                printf("%d ", cidao[l]);
                sum+=now-cidao[l];
                now=cidao[l];
                l=l-1;
            }
            else//右近
            {
                printf("%d ", cidao[r]);
                sum+=cidao[r]-now;
                now=cidao[r];
                r=r+1;
            }
        }
        if(l==-1)   //磁头移动到序列的最小号，返回外侧扫描仍未扫描的磁道
        {
            for(j=r; j<m; j++)
            {
                printf("%d ", cidao[j]);
            }
            sum+=cidao[m-1]-cidao[0];
        }
        else    //磁头移动到序列的最大号，返回内侧扫描仍未扫描的磁道
        {
            for(j=l; j>=0; j--)
            {
                printf("%d ", cidao[j]);
            }
            sum+=cidao[m-1]-cidao[0];
        }
    }
    ave=(float)(sum)/(float)(m);
    printf("\n");
    printf("平均移动磁道数：%f\n\n按回车键进行扫描算法调度\n", ave);
}


/*************************扫描调度算法*************************/
void SCAN(int cidao[],int m,int now)    //先要给出当前磁道号和移动臂的移动方向
{
    int k=1;
    int l,r,d;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //调用冒泡排序算法排序
    if(cidao[m-1]<=now)    //若当前磁道号大于请求序列中最大者，则直接由外向内依次给予各请求服务,此情况同最短寻道优先
    {
        printf("磁盘扫描序列为：");
        for(i=m-1; i>=0; i--)
            printf("%d ", cidao[i]);
        sum=now-cidao[0];
    }
    if(cidao[0]>=now)     //若当前磁道号小于请求序列中最小者，则直接由内向外依次给予各请求服务,此情况同最短寻道优先
    {
        printf("磁盘扫描序列为：");
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])   //若当前磁道号大于请求序列中最小者且小于最大者
    {
        while(cidao[k]<now)
        {
            k++;
        }
        l=k-1;
        r=k;
        printf("请输入扫描调度算法的当前移动臂的移动的方向 (1 表示向外 ，0表示向内) : ");
        d = geti();
        printf("磁盘扫描序列为：");
        if(d==0)     //选择移动臂方向向内，则先向内扫描
        {
            for(j=l; j>=0; j--)
            {
                printf("%d ", cidao[j]);//输出向内扫描的序列
            }
            for(j=r; j<m; j++) //磁头移动到最小号，则改变方向向外扫描未扫描的磁道
            {
                printf("%d ", cidao[j]);//输出向外扫描的序列
            }
            sum=now-2*cidao[0]+cidao[m-1];
        }
        else     //选择移动臂方向向外，则先向外扫描
        {
            for(j=r; j<m; j++)
            {
                printf("%d ", cidao[j]);//输出向外扫描的序列
            }
            for(j=l; j>=0; j--)  //磁头移动到最大号，则改变方向向内扫描未扫描的磁道
            {
                printf("%d ", cidao[j]);
            }
            sum=-now-cidao[0]+2*cidao[m-1];
        }
    }
    ave=(float)(sum)/(float)(m);
    printf("\n");
    printf("平均移动磁道数：%f\n\n按回车键进行循环扫描算法调度\n", ave);
}

/************************循环扫描调度算法*****************************/
void CSCAN(int cidao[],int m,int now)
{
    int k=1;
    int l,r;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //调用冒泡排序算法排序
    printf("磁盘扫描序列为：");
    if(cidao[m-1]<=now)   //若当前磁道号大于请求序列中最大者，则直接将移动臂移动到最小号磁道依次向外给予各请求服务
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=now-2*cidao[0]+cidao[m-1];
    }
    if(cidao[0]>=now) //若当前磁道号小于请求序列中最小者，则直接由内向外依次给予各请求服务,此情况同最短寻道优先
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])  //若当前磁道号大于请求序列中最小者且小于最大者
    {
        while(cidao[k]<now)    //单向反复地从内向外扫描
        {
            k++;
        }
        l=k-1;
        r=k;
        for(j=r; j<m; j++)
        {
            printf("%d ", cidao[j]);//输出从当前磁道向外扫描的序列
        }
        for(j=0; j<r; j++)   //当扫描完最大号磁道，磁头直接移动到最小号磁道，再向外扫描未扫描的磁道
        {
            printf("%d ", cidao[j]);
        }
        sum=2*cidao[m-1]+cidao[l]-now-2*cidao[0];
    }
    ave=(float)(sum)/(float)(m);
    printf("\n");
    printf("平均移动磁道数：%f\n\n【全部算法调度进行完毕】\n", ave);
}

void main()
{
    int cidao[maxsize],cidaoFCFS[maxsize],cidaoSSTF[maxsize],cidaoSCAN[maxsize],cidaoCSCAN[maxsize];
    int i=0,count,now;
    printf("请输入访问磁盘请求个数：");
    count = geti();
    printf("\n系统随机自动生成要访问的磁道序列:\n");
    for(i=0; i<count; i++)
    {
        cidao[i]=cidaoFCFS[i]=cidaoSSTF[i]=cidaoSCAN[i]=cidaoCSCAN[i]=rand()%40+80;     //随机生成磁道序列
        printf("%d ", cidao[i]);//输出磁道序列
    }

    printf("\n请输入当前的磁道号：");
A:
    now = geti();//输入当前磁道号

    printf("\n【先来先服务算法】\n");
    FCFS(cidaoFCFS,count,now);
    getchar();
    printf("【最短寻道时间优先算法】\n");
    SSTF(cidaoSSTF,count,now);
    getchar();
    printf("【扫描算法】\n");
    SCAN(cidaoSCAN,count,now);
    getchar();
    printf("【循环扫描算法】\n");
    CSCAN(cidaoCSCAN,count,now);
    getchar();
}

