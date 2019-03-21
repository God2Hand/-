#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUll 0
/* ���̵���-�̽������� */

// ���̿��ƿ�PCB
typedef struct pcb
{
    char name[12];//������
    int arrivalTime;//����ʱ��
    int startTime;//��ʼ����ʱ��
    int finishTime;//��������ʱ��
    int wholeTime;//��Ҫ���е�ʱ�䣨���̵ĳ��ȣ�
    struct pcb *link;//��һ��ָ��
}pcb;

int num;//��������
void input();
pcb *ready=NULL,*p=NULL,*finish = NULL;//ready��δ��ɵĽ��̵Ķ��У�finish���Ѿ�������ɵĽ��̵Ķ���
int M;

int geti()	//ʹ�û�������������
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("����Ϊ�գ�������������������\n");
		printf("�������������:");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("�������������������������\n");
			printf("�������������:");
			return geti();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

//�����߳��������������̡߳�����߳���Ϣ
void input()
{
    printf("�������������:");
    num=geti();
    M = num;
    struct pcb *q = ready;
    int  i = 0;
    srand( (unsigned)time( NULL ) );//��ʼ�������
    printf("\n�������Զ��������½���:\n\n");
    for( i=0 ;i<num;i++)
    {
        char nameNO[5];
        p = (pcb *)malloc(sizeof(pcb));

        //����������
        strcpy(p->name,"progress");
        itoa(i+1,nameNO,10);
        strncat(p->name,nameNO,5);
        printf("����:%s\n",p->name);

        //����ʱ��������ɣ�0~9��
        if(i==0)
            p->arrivalTime=0;
        else
            p->arrivalTime=rand()%10;
        printf("����ʱ��:%d\n",p->arrivalTime);

        //��Ҫ���е�ʱ��������ɣ�0~4��
        p->wholeTime=rand()%4+1;
        printf("��Ҫ���е�ʱ��:%d\n",p->wholeTime);

        p->link = NULL;
        if(NULL == ready)
        {
            ready = p;
            q = ready;
        }
        else
        {
            q = ready;
            while(NULL != q->link)  //��q�ƶ��������������һ������
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
    printf("���س�����ʼ���̵�����");
    char a;
    scanf("%c",&a);
}


/* �̽�������Ӧsjf�㷨 */
//ͬ����Ӧʱ���£����п�ǰ�Ŀ϶����ȵ����
//nowTime ������ִ�е�ʱ��
pcb* sjf(int nowTime,int *after)
{
    int i = 0 ;
    pcb *nowProgress=NULL, *p = ready;
    int ProgressNum = 0; // ��ǰ��̵��ǵڼ����߳�
    int minTime =0; // �������ʱ��

    if(NULL != ready)
    {
        while(NULL != p) //���������������ҳ���̵Ľ��̣�������ʱ�����
        {
            if(nowTime >= p->arrivalTime)
            {
                if(0 == minTime)  //�״θ�ֵ
                {
                    nowProgress = p;
                    minTime = p->wholeTime;
                }
                else
                {
                    if(p->wholeTime < minTime) //ͬ��Ӧʱ��ȡ��ǰ�����ȵ��
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


//�������p���̶߳���
void output(pcb *p,int now_time)
{
    if(NULL == p)
    {
        printf("\n*************************************************\n");
        printf("\n��ǰʱ�̣�%d�����޽��������У�\n",now_time);
        printf("���س�������һ��ʱ��\n");
        printf("\n*************************************************\n");
        char a;
        scanf("%c",&a);
    }
    else
    {
            printf("���̣�%s������ʱ�䣺%d������ʱ��:%d\n",p->name,p->wholeTime,p->arrivalTime);
    }
}

// �������������߳�����ɾ��׼�����У�ready���Ѿ����еĽ���p ����ӵ���ɶ��У�finish��������
void destory(pcb *p,int now_time)
{
    pcb *q = ready;
    pcb *f = NULL;
    if(strcmp(p->name,ready->name) == 0)  //��һ������
    {
        ready = ready ->link;
    }
    else  //�м����
    {
        q = ready;
        while(  (strcmp(q->link->name,p->name) != 0) && NULL != q->link)  //�ҵ�pҪɾ����λ��
        {
            q= q->link;
        }
        q->link = p->link;
    }
    //���Ѿ����еĽ�����ӵ�finish����
    p->startTime = now_time - p->wholeTime;
    p->finishTime = now_time;

    if(NULL == finish)  //��һ����ɵĽ���
    {
        finish = p;
        p->link = NULL;
    }
    else  //�м���ɵĽ���
    {
        f = finish;
        while(NULL != f->link )
        {
            f = f->link;
        }
        f->link = p;
        p->link = NULL;
    }

    num--;  //�ȴ����̼�1
}

//ȫ��������ɺ����ͳ����Ϣ
void outputAll()
{
    pcb *p = finish;
    printf("\n*********************ͳ�ƽ��*********************\n");
    float avgRevolve = 0;
    float avgSuperRevolve = 0;
    printf("\n����        ��ʼʱ��  ����ʱ�� ����ʱ�� ����ʱ��\n");
    while(NULL != p)
    {
        avgRevolve += p->finishTime - p->arrivalTime;
        avgSuperRevolve += 1.0 * (avgRevolve) / p->wholeTime;
        printf("\n%s\t%d\t %d\t  %d\t   %d\n",p->name,p->startTime,p->finishTime,p->wholeTime,p->arrivalTime);
        p = p->link;
    }
    printf("\n  ƽ����תʱ�䣺%.2f,ƽ����Ȩƽ����תʱ�䣺%.2f\n",avgRevolve/M,avgSuperRevolve/M);
    printf("\n**************************************************\n");

    printf("\n�������н��������س����˳�...\n");
    getchar();
}

//start here
int main()
{
    printf("\n***************���̵���-�̽�������***************\n");
    printf("\n");

    input();

    struct pcb *s = ready;
    int now_time = 0 ;
    struct pcb *nowProgress = NULL;  //��ǰ���еĽ���
    int *after = 0; //ִ����һ������֮���ʱ��
    int i = 0 ;

    pcb *m = ready;

    while(num > 0)//һ������һ������
    {
        /* ���ö̽��������㷨 */
        nowProgress = sjf(now_time,&after);

        if(NULL != nowProgress)  //��ǰ�н���������
        {
            for(;now_time < after;now_time++)  //���ÿ��ʱ�����еĽ������
            {
                printf("\n*************************************************\n");
                printf("\n��ǰʱ�䣺%d\n",now_time);
                printf("\n�������еĽ��̣�\n");
                output(nowProgress,now_time);
                printf("\n���������еĽ��̣�\n");
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

                printf("\n����ɵĽ��̣�\n");
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
                printf("\n���س�������һ��ʱ��\n");
                printf("\n*************************************************\n");
                char a;
                scanf("%c",&a);
            }
            printf("\n");

            //ready -> finish
            destory(nowProgress,now_time);

            }
        else   //û�н���������
        {
            output(nowProgress,now_time);
            now_time ++;
        }

    }
    printf("ȫ��������������ϣ����س����鿴ͳ�ƽ��\n");
    char a;
    scanf("%c",&a);
    //���ͳ�ƽ��
    outputAll();
    return 0;
}
