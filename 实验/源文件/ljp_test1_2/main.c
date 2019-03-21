#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define NULL 0
#define TIME 2//时间片长度
/* 进程调度-时间片轮转 */

int now_time=0;//时间
int num;//进程数量

//进程控制块pcb
typedef struct pcb{
    char name[12];//进程名
    int queue;//进程所在的队列
	int eTime;//进程在本队列可运行的时间片
    int wholeTime;//需要运行的时间
    int alreadyTime;//已运行的时间
    int finishTime;//结束运行时间
    int startTime;//开始时间，这里等同于到达时间
    char state;//进程的状态,就绪 W（Wait）、运行R（Run）、或完成F（Finish）
    struct pcb *link;//下一个指针
}pcb;
pcb	*ready = NULL, *pinsert = NULL, *p =NULL,*pFinish,*pFinish1,*finish;//pfend是逻一个辑队列的结尾，pinsert是就绪队列的下一个插入位置。

int geti()	//使用户仅能输入整数
{
	char ch;//第一种情况：正在进行的进程已经完成，立刻激活调度程序
	int i = 0;
	fflush(stdin);
	ch = getchar();//第一种情况：正在迀
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

void insert()//插入进程
{
	if(!ready ){//第一个，就绪
		ready = p;
		pinsert = p;
	} else if (ready->queue == 1) {//第一队列存在
		p->link = pinsert->link;
		pinsert->link = p;
		pinsert = p;
	}
}

void input()/*建立进程控制块函数*/
{
	int i;
	printf("请输入进程数量:");
	num = geti();
	srand( (unsigned)time( NULL ) );//初始化随机数
    printf("\n程序序自动生成以下进程:\n\n");
	for(i=0; i < num; i++)
	{
        char nameNO[5];
        p=(pcb *)malloc(sizeof(pcb));

        //进程名生成
        strcpy(p->name,"progress");
        itoa(i+1,nameNO,10);
        strncat(p->name,nameNO,5);
        printf("进程:%s\n",p->name);

        //需要运行的时间随机生成（0~6偶数）,状态:W,进程所在逻辑队列:1,需要运行时间
        //p->wholeTime=(rand()%3+1)*2;
        p->wholeTime=rand()%4+1;
        printf("需要运行的时间:%d\n\n",p->wholeTime);

		p->alreadyTime=0;
		p->state='W';
		p->queue =1;
		p->eTime = TIME;
		p->link=NULL;//第一种情况：正在迀
		insert();/*调用insert函数*/
	}
	printf("\n时间片长为%d\n",TIME);
    printf("\n按回车键开始进程的运行");
    char a;
    scanf("%c",&a);
}

void disp(pcb *pr)/*建立进程现实函数，用于显示当前进程*/
{
    if(pr->state=='F')
        printf("进程:%s,状态:%c,需要运行时间:%d,已经运行时间:%d,开始运行时间：%d,结束运行时间:%d\n",pr->name,pr->state,pr->wholeTime,pr->alreadyTime,pr->startTime,pr->finishTime);
    else
	    printf("进程:%s,状态:%c,进程所在逻辑队列:%d,需要运行时间:%d,已经运行时间:%d,在队列可停留时间:%d\n",pr->name,pr->state,pr->queue,pr->wholeTime,pr->alreadyTime,pr->eTime);
}

void check()/*建立进程查看函数*/
{
	pcb *pr;
	printf("\n正在运行的进程:\n");
	disp(ready);
	pr= ready ->link;
	printf("\n就绪队列中的进程:\n");/*显示就绪队列状态*/
	if (pr == NULL) {//第一种情况：正在进行的进程已经完成，立刻激活调度程序
        printf("空\n");
	}
	while(pr!=NULL)//第一种
	{
		disp(pr);
		pr=pr->link;
	}
	pr= finish->link;
	printf("\n已完成的进程:\n");/*显示就绪队列状态*/
	if (pr == NULL) {
        printf("空\n");
	}
	while(pr!=NULL)
	{
		disp(pr);
		pr=pr->link;
	}
}

//第一种情况：正在进行的进程已经完成，立刻激活调度程序
void destroy()/*建立进程撤销函数(进程findpos运行结束，撤销进程)*/
{
    ready -> state='F';//进程完成标志
    ready->finishTime=now_time+1;//下一个时刻完成，对应“eTime--”语句
	printf("\n进程%s已完成!\n",ready->name);
	//把已经完成的进程从就绪队列放到已完成队列中
	p = ready;
	ready = ready->link;

	p->link=NULL;
	//找到完成队列默认
	pFinish=finish;
	while(pFinish!=NULL){
       pFinish1=pFinish;
       pFinish=pFinish->link;
	}
	pFinish1->link=p;
}


void running()/*建立进程就绪函数(进程运行时间到，置就绪状态)*/
{
    //更改运行线程的运行时间
	(ready -> alreadyTime)++;
	//如果没开始，记录开始时间
	if(ready -> alreadyTime==1)
        ready->startTime=now_time;
    //可用时间片时间减一,状态:W,进程所在逻辑队列:1,需要运行时间
	ready ->eTime --;

    //进程切换时机
	if(ready->alreadyTime == ready->wholeTime){ //第一种情况：正在进行的进程已经完成，立刻激活调度程序
		destroy();
		return;
	}else if(ready ->eTime == 0){   //第二种情况：时间片用完，计时器中断处理程序被激活
		(ready -> queue)++;//放入下一逻辑队列
		ready->eTime = TIME;//重新给予时间片
		ready -> state='W';

		//----------------------
		if(!ready->link ||ready->queue < ready->link->queue) return;

        p = ready->link;

        ready->link = pinsert->link;
        pinsert->link = ready;
        pinsert = ready;

        ready = p;
	}
}


void outputAll()
{
    pcb *pr =finish->link;
    printf("\n*********************统计结果*********************\n");
    float avgRevolve = 0;
    float avgSuperRevolve = 0;
    printf("\n进程        开始时间  结束时间 运行时间\n");
    while(NULL != pr)
    {
        avgRevolve +=pr->finishTime - pr->startTime - 0;
        avgSuperRevolve += 1.0*(avgRevolve)/pr->wholeTime;
        printf("\n%s\t%d\t %d\t  %d\t\n",pr->name,pr->startTime,pr->finishTime,pr->wholeTime);
        pr = pr->link;
    }
    printf("\n  平均周转时间：%.2f,平均带权平均周转时间：%.2f\n",avgRevolve/num,avgSuperRevolve/num);
    printf("\n**************************************************\n");
}

void main()
{
    printf("\n***************进程调度-时间片轮转***************\n");
    printf("（当前时间片为 %d 个单位时间）\n", TIME);
    printf("\n");

	char ch;
	input();

	finish=(pcb *)malloc(sizeof(pcb));
    finish->link=NULL;

	while(ready != NULL)
	{
        printf("\n*********************************************************************************************\n");
	    printf("\n当前时间：%d\n",now_time);
		ready ->state = 'R';
		//查看进程
		check();

		/* 进程调度 */
		running();

        printf("\n按回车键到下一刻时间\n");
        printf("\n*********************************************************************************************\n");
		fflush(stdin);
		ch = getchar();

		//注意是先run再时间--的
        now_time++;
	}
    printf("全部进程已运行完毕，按回车键查看统计结果\n");
    char a;
    scanf("%c",&a);
    outputAll();
	getchar();
}
