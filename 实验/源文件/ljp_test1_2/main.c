#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define NULL 0
#define TIME 2//ʱ��Ƭ����
/* ���̵���-ʱ��Ƭ��ת */

int now_time=0;//ʱ��
int num;//��������

//���̿��ƿ�pcb
typedef struct pcb{
    char name[12];//������
    int queue;//�������ڵĶ���
	int eTime;//�����ڱ����п����е�ʱ��Ƭ
    int wholeTime;//��Ҫ���е�ʱ��
    int alreadyTime;//�����е�ʱ��
    int finishTime;//��������ʱ��
    int startTime;//��ʼʱ�䣬�����ͬ�ڵ���ʱ��
    char state;//���̵�״̬,���� W��Wait��������R��Run���������F��Finish��
    struct pcb *link;//��һ��ָ��
}pcb;
pcb	*ready = NULL, *pinsert = NULL, *p =NULL,*pFinish,*pFinish1,*finish;//pfend����һ�������еĽ�β��pinsert�Ǿ������е���һ������λ�á�

int geti()	//ʹ�û�������������
{
	char ch;//��һ����������ڽ��еĽ����Ѿ���ɣ����̼�����ȳ���
	int i = 0;
	fflush(stdin);
	ch = getchar();//��һ������������|
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

void insert()//�������
{
	if(!ready ){//��һ��������
		ready = p;
		pinsert = p;
	} else if (ready->queue == 1) {//��һ���д���
		p->link = pinsert->link;
		pinsert->link = p;
		pinsert = p;
	}
}

void input()/*�������̿��ƿ麯��*/
{
	int i;
	printf("�������������:");
	num = geti();
	srand( (unsigned)time( NULL ) );//��ʼ�������
    printf("\n�������Զ��������½���:\n\n");
	for(i=0; i < num; i++)
	{
        char nameNO[5];
        p=(pcb *)malloc(sizeof(pcb));

        //����������
        strcpy(p->name,"progress");
        itoa(i+1,nameNO,10);
        strncat(p->name,nameNO,5);
        printf("����:%s\n",p->name);

        //��Ҫ���е�ʱ��������ɣ�0~6ż����,״̬:W,���������߼�����:1,��Ҫ����ʱ��
        //p->wholeTime=(rand()%3+1)*2;
        p->wholeTime=rand()%4+1;
        printf("��Ҫ���е�ʱ��:%d\n\n",p->wholeTime);

		p->alreadyTime=0;
		p->state='W';
		p->queue =1;
		p->eTime = TIME;
		p->link=NULL;//��һ������������|
		insert();/*����insert����*/
	}
	printf("\nʱ��Ƭ��Ϊ%d\n",TIME);
    printf("\n���س�����ʼ���̵�����");
    char a;
    scanf("%c",&a);
}

void disp(pcb *pr)/*����������ʵ������������ʾ��ǰ����*/
{
    if(pr->state=='F')
        printf("����:%s,״̬:%c,��Ҫ����ʱ��:%d,�Ѿ�����ʱ��:%d,��ʼ����ʱ�䣺%d,��������ʱ��:%d\n",pr->name,pr->state,pr->wholeTime,pr->alreadyTime,pr->startTime,pr->finishTime);
    else
	    printf("����:%s,״̬:%c,���������߼�����:%d,��Ҫ����ʱ��:%d,�Ѿ�����ʱ��:%d,�ڶ��п�ͣ��ʱ��:%d\n",pr->name,pr->state,pr->queue,pr->wholeTime,pr->alreadyTime,pr->eTime);
}

void check()/*�������̲鿴����*/
{
	pcb *pr;
	printf("\n�������еĽ���:\n");
	disp(ready);
	pr= ready ->link;
	printf("\n���������еĽ���:\n");/*��ʾ��������״̬*/
	if (pr == NULL) {//��һ����������ڽ��еĽ����Ѿ���ɣ����̼�����ȳ���
        printf("��\n");
	}
	while(pr!=NULL)//��һ��
	{
		disp(pr);
		pr=pr->link;
	}
	pr= finish->link;
	printf("\n����ɵĽ���:\n");/*��ʾ��������״̬*/
	if (pr == NULL) {
        printf("��\n");
	}
	while(pr!=NULL)
	{
		disp(pr);
		pr=pr->link;
	}
}

//��һ����������ڽ��еĽ����Ѿ���ɣ����̼�����ȳ���
void destroy()/*�������̳�������(����findpos���н�������������)*/
{
    ready -> state='F';//������ɱ�־
    ready->finishTime=now_time+1;//��һ��ʱ����ɣ���Ӧ��eTime--�����
	printf("\n����%s�����!\n",ready->name);
	//���Ѿ���ɵĽ��̴Ӿ������зŵ�����ɶ�����
	p = ready;
	ready = ready->link;

	p->link=NULL;
	//�ҵ���ɶ���Ĭ��
	pFinish=finish;
	while(pFinish!=NULL){
       pFinish1=pFinish;
       pFinish=pFinish->link;
	}
	pFinish1->link=p;
}


void running()/*�������̾�������(��������ʱ�䵽���þ���״̬)*/
{
    //���������̵߳�����ʱ��
	(ready -> alreadyTime)++;
	//���û��ʼ����¼��ʼʱ��
	if(ready -> alreadyTime==1)
        ready->startTime=now_time;
    //����ʱ��Ƭʱ���һ,״̬:W,���������߼�����:1,��Ҫ����ʱ��
	ready ->eTime --;

    //�����л�ʱ��
	if(ready->alreadyTime == ready->wholeTime){ //��һ����������ڽ��еĽ����Ѿ���ɣ����̼�����ȳ���
		destroy();
		return;
	}else if(ready ->eTime == 0){   //�ڶ��������ʱ��Ƭ���꣬��ʱ���жϴ�����򱻼���
		(ready -> queue)++;//������һ�߼�����
		ready->eTime = TIME;//���¸���ʱ��Ƭ
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
    printf("\n*********************ͳ�ƽ��*********************\n");
    float avgRevolve = 0;
    float avgSuperRevolve = 0;
    printf("\n����        ��ʼʱ��  ����ʱ�� ����ʱ��\n");
    while(NULL != pr)
    {
        avgRevolve +=pr->finishTime - pr->startTime - 0;
        avgSuperRevolve += 1.0*(avgRevolve)/pr->wholeTime;
        printf("\n%s\t%d\t %d\t  %d\t\n",pr->name,pr->startTime,pr->finishTime,pr->wholeTime);
        pr = pr->link;
    }
    printf("\n  ƽ����תʱ�䣺%.2f,ƽ����Ȩƽ����תʱ�䣺%.2f\n",avgRevolve/num,avgSuperRevolve/num);
    printf("\n**************************************************\n");
}

void main()
{
    printf("\n***************���̵���-ʱ��Ƭ��ת***************\n");
    printf("����ǰʱ��ƬΪ %d ����λʱ�䣩\n", TIME);
    printf("\n");

	char ch;
	input();

	finish=(pcb *)malloc(sizeof(pcb));
    finish->link=NULL;

	while(ready != NULL)
	{
        printf("\n*********************************************************************************************\n");
	    printf("\n��ǰʱ�䣺%d\n",now_time);
		ready ->state = 'R';
		//�鿴����
		check();

		/* ���̵��� */
		running();

        printf("\n���س�������һ��ʱ��\n");
        printf("\n*********************************************************************************************\n");
		fflush(stdin);
		ch = getchar();

		//ע������run��ʱ��--��
        now_time++;
	}
    printf("ȫ��������������ϣ����س����鿴ͳ�ƽ��\n");
    char a;
    scanf("%c",&a);
    outputAll();
	getchar();
}
