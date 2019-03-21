#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define WAIT "Wait"//����״̬
#define RUN "Run"//����״̬
#define FINISH "Finish"//���״̬
#define PCBNUMBER 20 //���ý��̲��������Ϊ20
/* ���̵���-����Ӧ������ */

typedef struct pcb{
	char name[12];//������
	int arrivelTime;//����ʱ��
	int wholeTime;//��Ҫ����ʱ��
	int startTime;//��ʼʱ��
	int finishTime;//���ʱ��
	int turnoverTime;//��תʱ��
	float useWeightTurnoverTime;//��Ȩ��תʱ��
	char processStatus[10];//����״̬
};
static int currentTime = 0;//��ǰʱ��
static int finishNumber = 0;//�����������
char pcbArray[PCBNUMBER][10];//�����������Ϣ�Ķ�Ԫ����
float priority[PCBNUMBER];//��Ž������ȼ���һԪ����
int num;//��������

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

//����pcb
void createpcb(struct pcb* pcb){
    printf("�������������:");
    num=geti();//NΪȫ�ֱ���
    int  i;
    srand( (unsigned)time( NULL ) );//��ʼ�������
    printf("\n�������Զ��������½���:\n\n");
 	for(i = 0; i < num; i++){
        char nameNO[5];

        //����������
        strcpy(pcb[i].name,"progress");
        itoa(i+1,nameNO,10);
        strncat(pcb[i].name,nameNO,5);
        printf("����:%s\n",pcb[i].name);

        //����ʱ���������
        if(i==0)
            pcb[i].arrivelTime=0;
        else
            pcb[i].arrivelTime=rand()%10;
        printf("����ʱ��:%d\n",pcb[i].arrivelTime);

        //��Ҫ���е�ʱ���������
        pcb[i].wholeTime=rand()%4+1;
        printf("��Ҫ���е�ʱ��:%d\n\n",pcb[i].wholeTime);


	 	pcb[i].startTime = 0;
	 	pcb[i].finishTime = 0;
	 	pcb[i].turnoverTime = 0;
	 	pcb[i].useWeightTurnoverTime = 0.0;
	 	strcpy(pcb[i].processStatus, WAIT);
	}
    printf("���س�����ʼ���̵�����");
    char a;
    scanf("%c",&a);
}

//�Ƚϸ�������֮��ĵ���ʱ��,����������
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

		//�ַ�����ֻ�ܶ����ʱ��ֱ�ӳ�ʼ��
		//minIndex = 6 ��ʱ�򱨴�
//		int k = j;
//		printf("%s, %d, %d, %d, %d, %d, %f, %s\n", pcb[k].name, pcb[k].arrivelTime, pcb[k].wholeTime, pcb[k].startTime, pcb[k].finishTime,pcb[k].turnoverTime,pcb[k].useWeightTurnoverTime,pcb[k].processStatus);

		pcb[minIndex] = temp;
	}
}

//��ӡ��;
void printJob(struct pcb* pcb){
    printf("\n**************************************************************************************\n");
	printf("\n��ǰʱ��:%d\n", currentTime);
	int i,a,b,c;
	a=0;
	b=0;
	c=0;
    printf("\n�������еĽ��̣�\n");
	for(i = 0; i < num; i++){
         if(strcmp(pcb[i].processStatus, RUN) == 0)//�������Ϊrun״̬���������
			{
			    printf("����:%s,����ʱ��:%d,��Ҫ����ʱ��:%d,��ʼʱ��:%d,����״̬:%s\n", pcb[i].name,pcb[i].arrivelTime,pcb[i].wholeTime,pcb[i].startTime,pcb[i].processStatus);
			    a=1;
			}
	}
	if(a==0)
        printf("��ǰʱ�̣����޽��������У�\n");

    printf("\n�ȴ��еĽ��̣�\n");
	printf("����         ����ʱ�� ��Ҫ����ʱ�� ����״̬\n");
	for(i = 0; i < num; i++){
           if(strcmp(pcb[i].processStatus, WAIT) == 0)//�������Ϊwait״̬���������
           {
               if(currentTime>=pcb[i].arrivelTime){
                   printf("%s\t%d\t   %d\t     %s\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].processStatus);
                   b=1;
               }
           }
	}
	if(b==0)
        printf("���޽����ڵȴ���\n");

    printf("\n����ɵĽ��̣�\n");
	printf("����         ����ʱ�� ��Ҫ����ʱ�� ��ʼʱ�� ���ʱ�� ��תʱ�� ��Ȩ��תʱ�� ����״̬\n");
	int d=0;
	for(i = 0; i < num; i++){
		if(strcmp(pcb[i].processStatus, FINISH) == 0)//�������Ϊfinish״̬���������
		{
		    printf("%s\t%d\t   %d\t     %d\t       %d\t%d\t %.2f\t    %s\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].startTime, pcb[i].finishTime, pcb[i].turnoverTime, pcb[i].useWeightTurnoverTime, pcb[i].processStatus);
		    c=1;
		    d++;
		}

	}
    if(c==0)
        printf("��������ɵĽ��̣�\n");

	if(d==num)
    {
        printf("\nȫ��������������ϣ����س����鿴ͳ�ƽ��\n");
        printf("\n**************************************************************************************\n");
        char a;
        scanf("%c",&a);
    }else{
        printf("\n���س�������һ��ʱ��\n");
        printf("\n**************************************************************************************\n");
        char a;
        scanf("%c",&a);
    }
}


//�㷨ѭ����������
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

//����ƽ����Ȩ��תʱ��
float weightTurnoverTimeCount(struct pcb* pcb){
	float sum = 0.0;
	int i;
	for(i = 0; i < num; i++)
		sum += pcb[i].useWeightTurnoverTime;
	return sum / num;
}

//����ƽ����תʱ��
float turnOverTimeCount(struct pcb* pcb){
	float sum = 0.0;
	int i;
	for(i = 0; i < num; i++)
		sum += pcb[i].turnoverTime;
	return sum / num;
}

//��ӡ���̵���˳��ƽ����תʱ�估ƽ����Ȩ��תʱ��
void printInfo(struct pcb* pcb){
    int i;
    printf("\n***************************************ͳ�ƽ��***************************************\n");
	printf("\n����         ����ʱ�� ��Ҫ����ʱ�� ��ʼʱ�� ���ʱ�� ��תʱ�� ��Ȩ��תʱ��\n");
	for(i = 0; i < num; i++){
		printf("%s\t%d\t   %d\t       %d\t%d\t%d\t %.2f\n", pcb[i].name, pcb[i].arrivelTime, pcb[i].wholeTime, pcb[i].startTime, pcb[i].finishTime, pcb[i].turnoverTime, pcb[i].useWeightTurnoverTime);

	}
	printf("\n�ٽ��̵���˳��Ϊ��%s -> %s -> %s -> %s -> %s\n", pcbArray[0], pcbArray[1], pcbArray[2], pcbArray[3], pcbArray[4]);
	printf("��ƽ����תʱ��Ϊ��%.2f\n",turnOverTimeCount(pcb));
	printf("��ƽ����Ȩ��תʱ��Ϊ��%.2f\n", weightTurnoverTimeCount(pcb));
    printf("\n**************************************************************************************\n");
}

//����Ӧ�����ȵ����㷨
void highestResponseRatioNext(struct pcb* pcb){
    //�����߳���
	createpcb(pcb);
	//������ʱ������
	compare(pcb);

	int i = 0, j = 0;
	for(; finishNumber != num; currentTime++){
		float maxPriority = 0.0;
		int indexPriority = 0;
		if(currentTime < pcb[0].arrivelTime)//��ǰʱ��С�ڵ�һ���ڵ㵽��ʱ��ʱ��ֱ�Ӵ�ӡ
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


//������
int main(){
    printf("\n***************���̵���-�����Ӧ������***************\n");
    printf("\n");

	//struct pcb pcb[num];
	struct pcb *pcb = malloc(sizeof(struct pcb) * num);

	highestResponseRatioNext(pcb);

	printInfo(pcb);//��ӡ���̵���˳��ƽ����תʱ�估ƽ����Ȩ��תʱ��
	currentTime = 0;//��ǰʱ����λ
	finishNumber = 0;//��ɽ���������λ

	printf("����������������Enter���˳�...");
    getchar();

	free(pcb);
	return 0;
}
