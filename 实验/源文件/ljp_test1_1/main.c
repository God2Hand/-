#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUll 0
/* 进程调度-短进程优先 */

// 进程控制块PCB
typedef struct pcb
{
    char name[12];//进程名
    int arrivalTime;//到达时间
    int startTime;//开始运行时间
    int finishTime;//结束运行时间
    int wholeTime;//需要运行的时间（进程的长度）
    struct pcb *link;//下一个指针
}pcb;

int num;//进程数量
void input();
pcb *ready=NULL,*p=NULL,*finish = NULL;//ready是未完成的进程的队列，finish是已经结束完成的进程的队列
int M;

int geti()	//使用户仅能输入整数
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("不能为空，请重新输入正整数！\n");
		printf("请输入进程数量:");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("输入错误，请重新输入正整数！\n");
			printf("请输入进程数量:");
			return geti();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

//输入线程数量，并创建线程、输出线程信息
void input()
{
    printf("请输入进程数量:");
    num=geti();
    M = num;
    struct pcb *q = ready;
    int  i = 0;
    srand( (unsigned)time( NULL ) );//初始化随机数
    printf("\n程序序自动生成以下进程:\n\n");
    for( i=0 ;i<num;i++)
    {
        char nameNO[5];
        p = (pcb *)malloc(sizeof(pcb));

        //进程名生成
        strcpy(p->name,"progress");
        itoa(i+1,nameNO,10);
        strncat(p->name,nameNO,5);
        printf("进程:%s\n",p->name);

        //到达时间随机生成（0~9）
        if(i==0)
            p->arrivalTime=0;
        else
            p->arrivalTime=rand()%10;
        printf("到达时间:%d\n",p->arrivalTime);

        //需要运行的时间随机生成（0~4）
        p->wholeTime=rand()%4+1;
        printf("需要运行的时间:%d\n",p->wholeTime);

        p->link = NULL;
        if(NULL == ready)
        {
            ready = p;
            q = ready;
        }
        else
        {
            q = ready;
            while(NULL != q->link)  //将q移动到就绪队列最后一个进程
            {
                q = q->link;
            }
            p->link = NULL;
            q->link = p;
            q=p;
        }
        printf("\n");
    }
    q= NULL;
    free(q);
    printf("按回车键开始进程的运行");
    char a;
    scanf("%c",&a);
}


/* 短进程先响应sjf算法 */
//同样响应时间下，队列靠前的肯定是先到达的
//nowTime 是现在执行的时间
pcb* sjf(int nowTime,int *after)
{
    int i = 0 ;
    pcb *nowProgress=NULL, *p = ready;
    int ProgressNum = 0; // 当前最短的是第几个线程
    int minTime =0; // 最短运行时间

    if(NULL != ready)
    {
        while(NULL != p) //遍历整个链表，查找出最短的进程，即运行时间最短
        {
            if(nowTime >= p->arrivalTime)
            {
                if(0 == minTime)  //首次赋值
                {
                    nowProgress = p;
                    minTime = p->wholeTime;
                }
                else
                {
                    if(p->wholeTime < minTime) //同响应时间取最前（即先到达）
                    {
                        nowProgress = p;
                        minTime = p->wholeTime;
                    }
                }

                *after = minTime+nowTime;
            }
            p = p->link;
        }
    }

    return nowProgress;
}


//输出参数p的线程队列
void output(pcb *p,int now_time)
{
    if(NULL == p)
    {
        printf("\n*************************************************\n");
        printf("\n当前时刻：%d，暂无进程在运行！\n",now_time);
        printf("按回车键到下一刻时间\n");
        printf("\n*************************************************\n");
        char a;
        scanf("%c",&a);
    }
    else
    {
            printf("进程：%s，运行时间：%d，到达时间:%d\n",p->name,p->wholeTime,p->arrivalTime);
    }
}

// 根据正在运行线程名，删除准备队列（ready）已经运行的进程p ，添加到完成队列（finish）队列中
void destory(pcb *p,int now_time)
{
    pcb *q = ready;
    pcb *f = NULL;
    if(strcmp(p->name,ready->name) == 0)  //第一个进程
    {
        ready = ready ->link;
    }
    else  //中间进程
    {
        q = ready;
        while(  (strcmp(q->link->name,p->name) != 0) && NULL != q->link)  //找到p要删除的位置
        {
            q= q->link;
        }
        q->link = p->link;
    }
    //将已经运行的进程添加到finish队列
    p->startTime = now_time - p->wholeTime;
    p->finishTime = now_time;

    if(NULL == finish)  //第一个完成的进程
    {
        finish = p;
        p->link = NULL;
    }
    else  //中间完成的进程
    {
        f = finish;
        while(NULL != f->link )
        {
            f = f->link;
        }
        f->link = p;
        p->link = NULL;
    }

    num--;  //等待进程减1
}

//全部进程完成后，输出统计信息
void outputAll()
{
    pcb *p = finish;
    printf("\n*********************统计结果*********************\n");
    float avgRevolve = 0;
    float avgSuperRevolve = 0;
    printf("\n进程        开始时间  结束时间 运行时间 到达时间\n");
    while(NULL != p)
    {
        avgRevolve += p->finishTime - p->arrivalTime;
        avgSuperRevolve += 1.0 * (avgRevolve) / p->wholeTime;
        printf("\n%s\t%d\t %d\t  %d\t   %d\n",p->name,p->startTime,p->finishTime,p->wholeTime,p->arrivalTime);
        p = p->link;
    }
    printf("\n  平均周转时间：%.2f,平均带权平均周转时间：%.2f\n",avgRevolve/M,avgSuperRevolve/M);
    printf("\n**************************************************\n");

    printf("\n程序运行结束，按回车键退出...\n");
    getchar();
}

//start here
int main()
{
    printf("\n***************进程调度-短进程优先***************\n");
    printf("\n");

    input();

    struct pcb *s = ready;
    int now_time = 0 ;
    struct pcb *nowProgress = NULL;  //当前运行的进程
    int *after = 0; //执行完一个进程之后的时间
    int i = 0 ;

    pcb *m = ready;

    while(num > 0)//一次运行一个进程
    {
        /* 调用短进程优先算法 */
        nowProgress = sjf(now_time,&after);

        if(NULL != nowProgress)  //当前有进程在运行
        {
            for(;now_time < after;now_time++)  //输出每个时刻运行的进程情况
            {
                printf("\n*************************************************\n");
                printf("\n当前时间：%d\n",now_time);
                printf("\n正在运行的进程：\n");
                output(nowProgress,now_time);
                printf("\n就绪队列中的进程：\n");
                m=ready;
                while(NULL != m)
                {
                    if(m != nowProgress)
                    {
                        if(m->arrivalTime <= now_time)
                            output(m,now_time);
                    }
                    m= m->link;
                }

                printf("\n已完成的进程：\n");
                m=finish;
                while(NULL != m)
                {
                    if(m != nowProgress)
                    {
                        if(m->arrivalTime <= now_time)
                            output(m,now_time);
                    }
                    m= m->link;
                }
                printf("\n按回车键到下一刻时间\n");
                printf("\n*************************************************\n");
                char a;
                scanf("%c",&a);
            }
            printf("\n");

            //ready -> finish
            destory(nowProgress,now_time);

            }
        else   //没有进程在运行
        {
            output(nowProgress,now_time);
            now_time ++;
        }

    }
    printf("全部进程已运行完毕，按回车键查看统计结果\n");
    char a;
    scanf("%c",&a);
    //输出统计结果
    outputAll();
    return 0;
}
