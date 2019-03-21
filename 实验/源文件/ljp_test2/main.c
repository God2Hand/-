#include<stdio.h>
#include<string.h>
#include<time.h>
#define random(x) (rand()%x)
#define MAX 20 //最大作业数
#define RESS 100//最大资源数
int Avaiable[RESS];//可利用资源向量 Avaiable
char name[RESS];//可利用资源名
//n x m 矩阵 即 MAX x RESS
int Max[MAX][RESS];//最大需求矩阵 Max
int Allocation[MAX][RESS];//分配矩阵 Allocation
int Need[MAX][RESS];//需求矩阵 Need

int request[MAX][RESS];//请求矩阵 request

int Wnumber[MAX];//记录正在就绪队列的进程的索引的数组
int Rnumber[MAX];//记录正在运行队列的进程的索引的数组，即并发轮转的数组
int Fnumber[MAX]= {RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS}; //记录已经完成队列进程的索引的数组
int n=5,W=5,F=0;//n为Rnumber的size,W为Wnumber的size,F同理
int m=3;//m是程序资源数

int a;//资源分配时候的索引标记

int pan;//是否为安全状态，安全=1
int haveSet=0;//申请并成功分配了资源标志=1

typedef struct job {
    int Finish;//进程是否完成，完成=1
    char State;//进程状态，出了R，W，F，增加了B（block阻塞状态）
    char Name[10];//进程名

    int Max[10];//进程最大需求向量
    int Allocation[10];//进程已经分配向量
}job;


int size = 0;//当前进程数
job *Job;

//*******************初始化函数*************************
    void init_put()
    {
        srand((int)time(0));
        int i,j;

        name[0]='A';
        name[1]='B';
        name[2]='C';
        Avaiable[0]=10;
        Avaiable[1]=15;
        Avaiable[2]=12;

        for(i=0; i<10; i++) //初始化十个进程
        {
            for(j=0; j<3; j++)
            {
                Job[i].Max[j]=random(6);
                Job[i].Allocation[j]=0;
            }
            Job[i].State='W';
            Job[i].Finish=0;
            size++;
        }
        strcpy(Job[0].Name,"p0");
        strcpy(Job[1].Name,"p1");
        strcpy(Job[2].Name,"p2");
        strcpy(Job[3].Name,"p3");
        strcpy(Job[4].Name,"p4");
        strcpy(Job[5].Name,"p5");
        strcpy(Job[6].Name,"p6");
        strcpy(Job[7].Name,"p7");
        strcpy(Job[8].Name,"p8");
        strcpy(Job[9].Name,"p9");

        for(i=0; i<5; i++)
        {
            Rnumber[i]=i;//将前五个进程放入运行队列
        }
        for(i=5; i<10; i++)
        {
            Wnumber[9-i]=i;//将后五个进程放入就绪队列
        }

        set_put(Rnumber);

        print(Rnumber,5,1);
    }
    //*******************初始化函数*************************
    //为每一个运行队列的线程设置三大矩阵
    void set_put(int Rnumber[])
    {
        int i,j;
        for(i=0; i<n; i++)
            for(j=0; j<m; j++)
            {
                Max[i][j]=Job[Rnumber[i]].Max[j];
                Allocation[i][j]= Job[Rnumber[i]].Allocation[j];
            }
        for(i=0; i<n; i++)
            for(j=0; j<m; j++)
                Need[i][j]=Max[i][j]-Allocation[i][j];

    }
//*******************显示函数*************************
    //k=1 查看系统目前可用资源
    void print(int Xnumber[],int nm,int k)
    {
        int i,j;
        if(nm==0)
        {
            printf("该队列没有数据\n");
            return;
        }
        if(k==1)
        {
            printf("系统目前可用资源\n");
            for(i=0; i<m; i++)
                printf("%c:%d, ", name[i], Avaiable[i]);
            printf("\n");
            Job[Xnumber[0]].State='R';
        }
        printf("\n             Max          Allocation        Need         状态  \n");
        printf("进程名    ");

        //输出资源名
        for(j=0; j<3; j++)
        {
            for(i=0; i<m; i++)
                printf("%c   ", name[i]);
            printf("    ");
        }
        printf("\n");
        //输出进程名和三大矩阵
        for(i=0; i<nm; i++)
        {
            printf("   %s     ", Job[Xnumber[i]].Name);
            for(j=0; j<m; j++)
                printf("%d   ", Job[Xnumber[i]].Max[j]);
            printf("    ");
            for(j=0; j<m; j++)
                printf("%d   ", Job[Xnumber[i]].Allocation[j]);
            printf("    ");
            for(j=0; j<m; j++)
                printf("%d   ", Job[Xnumber[i]].Max[j]-Job[Xnumber[i]].Allocation[j]);
            printf("    ");
            printf(" %c     ", Job[Xnumber[i]].State);
            printf("\n");
        }


    }
    //**************安全性算法函数************************************
    void safe()
    {
        int i,j;
        int f;//可分配资源是否足够标志
        int w[RESS]= {0};//工作向量work
        for(i=0; i<m; i++)
            w[i]=Avaiable[i];//初始化为系统资源
        int k=0;//完成数
        int u=0;//资源不够次数
        int end = 0;//while结束标志=1
        while(1)
        {
            u=0;
            for(i=0; i<n; i++)//n为运行队列size
            {
                f=1;
                for(j=0; j<m; j++)//m为系统资源数
                {
                    if(Need[i][j]>w[j])//需要的大于可分配的，不足
                    {
                        f=0;
                        u++;
                        if (u == n - k) {//未完成的资源都不足，不安全
                            end = 1;
                        }
                        break;
                    }
                }
                if(Job[i].Finish==0&&f==1)//进程未完成并且未不足
                {
                    for(j=0; j<m; j++)
                        w[j]=w[j]+Allocation[i][j];//完成后归还系统资源
                    Job[i].Finish=1;//标志完成
                    k=k+1;
                    //u=0;//资源不足重新计数
                    break;
                }
            }
            if(k==n)//所有进程的Finish=1都满足，系统处于安全状态
            {
                printf("\n系统状态：系统安全\n");
                pan=1;
                break;
            }
            if(1 == end)
            {
                printf("\n系统状态：不安全状态\n");
                pan=0;
                break;
            }
        }//while : end
        //安全算法完成，把完成标志归零
        for(i=0; i<n; i++)
            Job[i].Finish=0;
    }
    //*********申请资源函数*************************
    void TT()
    {
        int i;
        int c=1;//是否请求成功。是=1
        pan=1;
        a=0;
        haveSet=0;
        printf("\n");

        //随机请求各资源数（在所需之内）
        for(i=0; i<m; i++)
        {
            if(Need[a][i]!=0)
                request[a][i]=random(Need[a][i]+1);//随机
            else request[a][i]=0;
            printf("申请资源%c数量:%d\n", name[i],request[a][i]);
        }

        //如果任意一个请求的资源大于可分配的，不安全
        for(i=0; i<m; i++)
        {
            if(request[a][i]>Avaiable[i])
            {
                c=0;//请求不成功
                pan=0;
                break;
            }
        }
        if(c==1)//请求成功
        {
            for(i=0; i<m; i++)
            {
                Avaiable[i]=Avaiable[i]-request[a][i];//剩余可分配资源-
                Need[a][i]=Need[a][i]-request[a][i];//需要的资源矩阵-
                Allocation[a][i]=Allocation[a][i]+request[a][i];//分配矩阵+
                Job[Rnumber[a]].Allocation[i]= Allocation[a][i];//进程分配到的资源=分配矩阵的
            }
            haveSet=1;
            safe();//分配后要执行安全算法
        }
    }
    //**********申请失败归还数据函数*************************
    void TD()
    {
        int i;
        for(i=0; i<m; i++)
        {
            Avaiable[i]=Avaiable[i]+request[a][i];
            Need[a][i]=Need[a][i]+request[a][i];
            Allocation[a][i]=Allocation[a][i]-request[a][i];
            Job[Rnumber[a]].Allocation[i]= Allocation[a][i];
        }
        haveSet=0;
    }

    void pan0()//请求资源失败，把请求的线程阻塞
    {
        int tmp,i;
        tmp=  Rnumber[0];       //当前运行的第一个进程调去最后位置，改成阻塞状态
        Job[tmp].State='B';
        for(i=0; i<n-1; i++)
        {
            Rnumber[i]=Rnumber[i+1];
        }
        Rnumber[n-1]=tmp;
    }




//使用户选择
int geti()
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("不能为空，请重新输入正整数！\n");
		printf("请输入你的选择：");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("输入错误，请重新输入正整数！\n");
			printf("请输入你的选择：");
			return geti();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

int main()
{
    Job = (job *)malloc(sizeof(job) * MAX);

    int k=1;
    int T;
    int V=1;

    init_put();

    safe();

    while(V!=0)
    {
        printf("\n");
        printf("************************\n");
        printf("1.申请资源\n2.查看就绪队列\n3.查看已经完成队列\n4.结束\n");
        printf("************************\n");
        printf("请输入你的选择：");

        T = geti();
        switch(T)
        {
        case 1:
            TT();
            if(pan==0)//申请不安全的话
            {
                if(haveSet==0) {
                    printf("\n不同意申请,资源申请超过Avaiable允许值！\n\n");
                }
                if(haveSet==1) {
                    TD();//请求失败，归还资源（其实前面控制好了，这里几乎不执行，但是还是控制下安全点）
                    printf("\n不同意申请，未通过安全算法!\n\n");

                }
                pan0();//阻塞请求失败的线程
            }
            //同意申请
            else
            {
                printf("\n同意申请!\n\n");
                pan1();
            }
            set_put(Rnumber);//为每一个运行队列的线程设置三大矩阵
            print(Rnumber,n,1);
            safe();
            break;
        case 2:
            print(Wnumber,W,0);
            safe();
            break;
        case 3:
            print(Fnumber,F,0);
            safe();
            break;
        case 4:
            V=0;
            break;
        }
        if (n <= 0) {
            V = 0;
            printf("\n所有线程已运行完成，按Enter键查看完成队列...\n");
            getchar();

            set_put(Rnumber);
            printf("系统目前可用资源\n");
            int abc;
            for(abc=0; abc<m; abc++)
                printf("%c:%d, ", name[abc], Avaiable[abc]);
            printf("\n");
            print(Fnumber,F,0);
            safe();
            getchar();
        }
    }
    return 0;
}

