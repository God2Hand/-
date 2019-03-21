#include<stdio.h>
#include<string.h>
#include<time.h>
#define random(x) (rand()%x)
#define MAX 20 //�����ҵ��
#define RESS 100//�����Դ��
int Avaiable[RESS];//��������Դ���� Avaiable
char name[RESS];//��������Դ��
//n x m ���� �� MAX x RESS
int Max[MAX][RESS];//���������� Max
int Allocation[MAX][RESS];//������� Allocation
int Need[MAX][RESS];//������� Need

int request[MAX][RESS];//������� request

int Wnumber[MAX];//��¼���ھ������еĽ��̵�����������
int Rnumber[MAX];//��¼�������ж��еĽ��̵����������飬��������ת������
int Fnumber[MAX]= {RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS,RESS}; //��¼�Ѿ���ɶ��н��̵�����������
int n=5,W=5,F=0;//nΪRnumber��size,WΪWnumber��size,Fͬ��
int m=3;//m�ǳ�����Դ��

int a;//��Դ����ʱ����������

int pan;//�Ƿ�Ϊ��ȫ״̬����ȫ=1
int haveSet=0;//���벢�ɹ���������Դ��־=1

typedef struct job {
    int Finish;//�����Ƿ���ɣ����=1
    char State;//����״̬������R��W��F��������B��block����״̬��
    char Name[10];//������

    int Max[10];//���������������
    int Allocation[10];//�����Ѿ���������
}job;


int size = 0;//��ǰ������
job *Job;

//*******************��ʼ������*************************
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

        for(i=0; i<10; i++) //��ʼ��ʮ������
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
            Rnumber[i]=i;//��ǰ������̷������ж���
        }
        for(i=5; i<10; i++)
        {
            Wnumber[9-i]=i;//����������̷����������
        }

        set_put(Rnumber);

        print(Rnumber,5,1);
    }
    //*******************��ʼ������*************************
    //Ϊÿһ�����ж��е��߳������������
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
//*******************��ʾ����*************************
    //k=1 �鿴ϵͳĿǰ������Դ
    void print(int Xnumber[],int nm,int k)
    {
        int i,j;
        if(nm==0)
        {
            printf("�ö���û������\n");
            return;
        }
        if(k==1)
        {
            printf("ϵͳĿǰ������Դ\n");
            for(i=0; i<m; i++)
                printf("%c:%d, ", name[i], Avaiable[i]);
            printf("\n");
            Job[Xnumber[0]].State='R';
        }
        printf("\n             Max          Allocation        Need         ״̬  \n");
        printf("������    ");

        //�����Դ��
        for(j=0; j<3; j++)
        {
            for(i=0; i<m; i++)
                printf("%c   ", name[i]);
            printf("    ");
        }
        printf("\n");
        //������������������
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
    //**************��ȫ���㷨����************************************
    void safe()
    {
        int i,j;
        int f;//�ɷ�����Դ�Ƿ��㹻��־
        int w[RESS]= {0};//��������work
        for(i=0; i<m; i++)
            w[i]=Avaiable[i];//��ʼ��Ϊϵͳ��Դ
        int k=0;//�����
        int u=0;//��Դ��������
        int end = 0;//while������־=1
        while(1)
        {
            u=0;
            for(i=0; i<n; i++)//nΪ���ж���size
            {
                f=1;
                for(j=0; j<m; j++)//mΪϵͳ��Դ��
                {
                    if(Need[i][j]>w[j])//��Ҫ�Ĵ��ڿɷ���ģ�����
                    {
                        f=0;
                        u++;
                        if (u == n - k) {//δ��ɵ���Դ�����㣬����ȫ
                            end = 1;
                        }
                        break;
                    }
                }
                if(Job[i].Finish==0&&f==1)//����δ��ɲ���δ����
                {
                    for(j=0; j<m; j++)
                        w[j]=w[j]+Allocation[i][j];//��ɺ�黹ϵͳ��Դ
                    Job[i].Finish=1;//��־���
                    k=k+1;
                    //u=0;//��Դ�������¼���
                    break;
                }
            }
            if(k==n)//���н��̵�Finish=1�����㣬ϵͳ���ڰ�ȫ״̬
            {
                printf("\nϵͳ״̬��ϵͳ��ȫ\n");
                pan=1;
                break;
            }
            if(1 == end)
            {
                printf("\nϵͳ״̬������ȫ״̬\n");
                pan=0;
                break;
            }
        }//while : end
        //��ȫ�㷨��ɣ�����ɱ�־����
        for(i=0; i<n; i++)
            Job[i].Finish=0;
    }
    //*********������Դ����*************************
    void TT()
    {
        int i;
        int c=1;//�Ƿ�����ɹ�����=1
        pan=1;
        a=0;
        haveSet=0;
        printf("\n");

        //����������Դ����������֮�ڣ�
        for(i=0; i<m; i++)
        {
            if(Need[a][i]!=0)
                request[a][i]=random(Need[a][i]+1);//���
            else request[a][i]=0;
            printf("������Դ%c����:%d\n", name[i],request[a][i]);
        }

        //�������һ���������Դ���ڿɷ���ģ�����ȫ
        for(i=0; i<m; i++)
        {
            if(request[a][i]>Avaiable[i])
            {
                c=0;//���󲻳ɹ�
                pan=0;
                break;
            }
        }
        if(c==1)//����ɹ�
        {
            for(i=0; i<m; i++)
            {
                Avaiable[i]=Avaiable[i]-request[a][i];//ʣ��ɷ�����Դ-
                Need[a][i]=Need[a][i]-request[a][i];//��Ҫ����Դ����-
                Allocation[a][i]=Allocation[a][i]+request[a][i];//�������+
                Job[Rnumber[a]].Allocation[i]= Allocation[a][i];//���̷��䵽����Դ=��������
            }
            haveSet=1;
            safe();//�����Ҫִ�а�ȫ�㷨
        }
    }
    //**********����ʧ�ܹ黹���ݺ���*************************
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

    void pan0()//������Դʧ�ܣ���������߳�����
    {
        int tmp,i;
        tmp=  Rnumber[0];       //��ǰ���еĵ�һ�����̵�ȥ���λ�ã��ĳ�����״̬
        Job[tmp].State='B';
        for(i=0; i<n-1; i++)
        {
            Rnumber[i]=Rnumber[i+1];
        }
        Rnumber[n-1]=tmp;
    }




//ʹ�û�ѡ��
int geti()
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("����Ϊ�գ�������������������\n");
		printf("���������ѡ��");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("�������������������������\n");
			printf("���������ѡ��");
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
        printf("1.������Դ\n2.�鿴��������\n3.�鿴�Ѿ���ɶ���\n4.����\n");
        printf("************************\n");
        printf("���������ѡ��");

        T = geti();
        switch(T)
        {
        case 1:
            TT();
            if(pan==0)//���벻��ȫ�Ļ�
            {
                if(haveSet==0) {
                    printf("\n��ͬ������,��Դ���볬��Avaiable����ֵ��\n\n");
                }
                if(haveSet==1) {
                    TD();//����ʧ�ܣ��黹��Դ����ʵǰ����ƺ��ˣ����Ｘ����ִ�У����ǻ��ǿ����°�ȫ�㣩
                    printf("\n��ͬ�����룬δͨ����ȫ�㷨!\n\n");

                }
                pan0();//��������ʧ�ܵ��߳�
            }
            //ͬ������
            else
            {
                printf("\nͬ������!\n\n");
                pan1();
            }
            set_put(Rnumber);//Ϊÿһ�����ж��е��߳������������
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
            printf("\n�����߳���������ɣ���Enter���鿴��ɶ���...\n");
            getchar();

            set_put(Rnumber);
            printf("ϵͳĿǰ������Դ\n");
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

