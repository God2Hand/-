#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define WAIT "Wait"//就绪状态
#define RUN "Run"//运行状态
#define FINISH "Finish"//完成状态
#define PCBNUMBER 20 //设置进程测试最大数为20
/* 进程调度-高响应比优先 */

typedef struct pcb{
	char name[12];//进程名
	int arrivelTime;//到达时间
	int wholeTime;//需要运行时间
	int startTime;//开始时间
	int finishTime;//完成时间
	int turnoverTime;//周转时间
	float useWeightTurnoverTime;//带权周转时间
	char processStatus[10];//进程状态
};
static int currentTime = 0;//当前时间
static int finishNumber = 0;//进程完成数量
char pcbArray[PCBNUMBER][10];//存放数组名信息的二元数组
float priority[PCBNUMBER];//存放进程优先级的一元数组
int num;//进程数量

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

//创建pcb
void createpcb(struct pcb* pcb){
    printf("请输入进程数量:");
    num=geti();//N为全局变量
    int  i;
    srand( (unsigned)time( NULL ) );//初始化随机数
    printf("\n程序序自动生成以下进程:\n\n");
 	for(i = 0; i < num; i++){
        char nameNO[5];

        //进程名生成
        strcpy(pcb[i].name,"progress");
        itoa(i+1,nameNO,10);
        strncat(pcb[i].name,nameNO,5);
        printf("进程:%s\n",pcb[i].name);

        //到达时间随机生成
        if(i==0)
            pcb[i].arrivelTime=0;
        else
            pcb[i].arrivelTime=rand()%10;
        printf("到达时间:%d\n",pcb[i].arrivelTime);

        //需要运行的时间随机生成
        pcb[i].wholeTime=rand()%4+1;
        printf("需要运行的时间:%d\n\n",pcb[i].wholeTime);


	 	pcb[i].startTime = 0;
	 	pcb[i].finishTime = 0;
	 	pcb[i].turnoverTime = 0;
	 	pcb[i].useWeightTurnoverTime = 0.0;
	 	strcpy(pcb[i].processStatus, WAIT);
	}
    printf("按回车键开始进程的运行");
    char a;
    scanf("%c",&a);
}

//比较各个进程之间的到达时间,按升序排列
void compare(struct pcb* pcb){
    int i,j,min,minIndex;
	for(i = 0; i < num - 1; i++){
		min = pcb[i].arrivelTime;
		minIndex = i;
		for(j = i + 1; j < num; j++){
			if(pcb[j].arrivelTime < min){
				min = pcb[j].arrivelTime;
				minIndex = j;
			}
		}
		struct pcb temp = pcb[i];
		pcb[i] = pcb[minIndex];

		//字符数组只能定义的时候直接初始化
		//minIndex = 6 的时候报错
//		int k = j;
//		printf("%s, %d, %d, %d, %d, %d, %f, %s\n", pcb[k].name, pcb[k].arrivelTime, pcb[k].wholeTime, pcb[k].startTime, pcb[k].finishTime,pcb[k].turnoverTime,pcb[k].useWeightTurnoverTime,pcb[k].processStatus);

		pcb[minIndex] = temp;
	}
}

//打印用途
void printJob(struct pcb* pcb){
    printf("\n**************************************************************************************\n");
	printf("\n当前时间:%d\n", currentTime);
	int i,a,b,c;
	a=0;
	b=0;
	c=0;
    printf("\n正在运行的进程：\n");
	for(i = 0; i < num; i++){
         if(strcmp(pcb[i].processStatus, RUN) == 0)//如果进程为run状态，这样输出
			{
			    printf("进程:%s,到达时间:%d,需要运行时间:%d,开始时间:%d,进程状态:%s\n", pcb[i].name,pcb[i].arrivelTime,pcb[i].wholeTime,pcb[i].startTime,pcb[i].processStatus);
			    a=1;
			}
	}
	if(a==0)
        printf("当前时刻，暂无进程在运行！\n");

    printf("\n等待中的进程：\n");
	printf("进程         到达时间 需要运行时间 进程状态\n");
	for(i = 0; i < num; i++){
           if(strcmp(pcb[i].processStatus, WAIT) == 0)//如果进程为wait状态，这样输出
           {
               if(currentTime>=pcb[i].arrivelTime){
                   printf("%s\t%d\t   %d\t     %s\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].processStatus);
                   b=1;
               }
           }
	}
	if(b==0)
        printf("暂无进程在等待！\n");

    printf("\n已完成的进程：\n");
	printf("进程         到达时间 需要运行时间 开始时间 完成时间 周转时间 带权周转时间 进程状态\n");
	int d=0;
	for(i = 0; i < num; i++){
		if(strcmp(pcb[i].processStatus, FINISH) == 0)//如果进程为finish状态，这样输出
		{
		    printf("%s\t%d\t   %d\t     %d\t       %d\t%d\t %.2f\t    %s\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].startTime, pcb[i].finishTime, pcb[i].turnoverTime, pcb[i].useWeightTurnoverTime, pcb[i].processStatus);
		    c=1;
		    d++;
		}

	}
    if(c==0)
        printf("暂无已完成的进程！\n");

	if(d==num)
    {
        printf("\n全部进程已运行完毕，按回车键查看统计结果\n");
        printf("\n**************************************************************************************\n");
        char a;
        scanf("%c",&a);
    }else{
        printf("\n按回车键到下一刻时间\n");
        printf("\n**************************************************************************************\n");
        char a;
        scanf("%c",&a);
    }
}


//算法循环遍历部分
void loop(struct pcb* pcb, int i){
	pcb[i].startTime = currentTime;
	pcb[i].finishTime = pcb[i].startTime + pcb[i].wholeTime;
	pcb[i].turnoverTime = pcb[i].finishTime - pcb[i].arrivelTime;
	pcb[i].useWeightTurnoverTime = pcb[i].turnoverTime * 1.0 / pcb[i].wholeTime;
	strcpy(pcb[i].processStatus, RUN);
	while(1){
		if(currentTime == pcb[i].finishTime){
			strcpy(pcb[i].processStatus, FINISH);
			finishNumber++;
			if(finishNumber == num)
				printJob(pcb);
			currentTime--;
			break;
		}
		else{
			printJob(pcb);
			currentTime++;
		}
	}
}

//计算平均带权周转时间
float weightTurnoverTimeCount(struct pcb* pcb){
	float sum = 0.0;
	int i;
	for(i = 0; i < num; i++)
		sum += pcb[i].useWeightTurnoverTime;
	return sum / num;
}

//计算平均周转时间
float turnOverTimeCount(struct pcb* pcb){
	float sum = 0.0;
	int i;
	for(i = 0; i < num; i++)
		sum += pcb[i].turnoverTime;
	return sum / num;
}

//打印进程调度顺序，平均周转时间及平均带权周转时间
void printInfo(struct pcb* pcb){
    int i;
    printf("\n***************************************统计结果***************************************\n");
	printf("\n进程         到达时间 需要运行时间 开始时间 完成时间 周转时间 带权周转时间\n");
	for(i = 0; i < num; i++){
		printf("%s\t%d\t   %d\t       %d\t%d\t%d\t %.2f\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].startTime, pcb[i].finishTime, pcb[i].turnoverTime, pcb[i].useWeightTurnoverTime);

	}
	printf("\n①进程调度顺序为：%s -> %s -> %s -> %s -> %s\n", pcbArray[0], pcbArray[1], pcbArray[2], pcbArray[3], pcbArray[4]);
	printf("②平均周转时间为：%.2f\n",turnOverTimeCount(pcb));
	printf("③平均带权周转时间为：%.2f\n", weightTurnoverTimeCount(pcb));
    printf("\n**************************************************************************************\n");
}

//高响应比优先调度算法
void highestResponseRatioNext(struct pcb* pcb){
    //创建线程组
	createpcb(pcb);
	//按到达时间排序
	compare(pcb);

	int i = 0, j = 0;
	for(; finishNumber != num; currentTime++){
		float maxPriority = 0.0;
		int indexPriority = 0;
		if(currentTime < pcb[0].arrivelTime)//当前时间小于第一个节点到来时间时，直接打印
			printJob(pcb);
		else{
            int i;
			for(i = 0; i < num; i++){
				if(strcmp(pcb[i].processStatus, FINISH) != 0){
					int waitTime = currentTime - pcb[i].arrivelTime;
				 	priority[i] = (waitTime + pcb[i].wholeTime) * 1.0 / pcb[i].wholeTime;
				 	if(priority[i] > maxPriority){
			 			maxPriority = priority[i];
			 			indexPriority = i;
			 		}
				}
			}
			strcpy(pcbArray[j++], pcb[indexPriority].name);
			loop(pcb, indexPriority);
		}
	}
}


//主函数
int main(){
    printf("\n***************进程调度-最高响应比优先***************\n");
    printf("\n");

	//struct pcb pcb[num];
	struct pcb *pcb = malloc(sizeof(struct pcb) * num);

	highestResponseRatioNext(pcb);

	printInfo(pcb);//打印进程调度顺序，平均周转时间及平均带权周转时间
	currentTime = 0;//当前时间置位
	finishNumber = 0;//完成进程数量置位

	printf("程序完整结束！按Enter键退出...");
    getchar();

	free(pcb);
	return 0;
}
