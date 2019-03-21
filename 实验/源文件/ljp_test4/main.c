#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#define maxsize 1000 //�ŵ�����������

//��l��left����r��right����־��ɨ�裨�������飬ģ�¿��ţ����˼��ܺá�

//ʹ�û�ѡ��
int geti()
{
    char ch;
    int i = 0;
    fflush(stdin);
    ch = getchar();
    while(ch == '\n')
    {
        printf("����Ϊ�գ�����������Ǹ�������\n");
        fflush(stdin);
        ch = getchar();
    }
    while(ch != '\n')
    {
        if(ch > '9' || ch < '0')
        {
            printf("�����������������Ǹ�������\n");
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

/*********************ð�������㷨**************************/
int *bubble(int cidao[],int m)
{
    int i,j;
    int temp;
    for(i=0; i<m; i++)  //ʹ��ð�ݷ�����С����˳������
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
/*********************�����ȷ�������㷨************************/
void FCFS(int cidao[],int m,int now)   //�ŵ������飬����Ϊm
{
    int sum=0;   //��Ѱ������
    int j,i;
    float ave;   //ƽ��Ѱ������
    sum+=abs(cidao[0]-now);
    printf("����ɨ������Ϊ��");
    for( i=0; i<m; i++) //�������ɨ������
    {
        printf("%d ", cidao[i]);
    }
    for(i=0,j=1; j<m; i++,j++) //��ƽ��Ѱ������
    {
        sum+=abs(cidao[j]-cidao[i]);
        ave=(float)(sum)/(float)(m);
    }
    printf("\n");
    printf("ƽ���ƶ��ŵ�����%f\n\n���س����������Ѱ��ʱ�������㷨����\n", ave);
}

/**********************���Ѱ��ʱ�����ȵ����㷨********************/
//����㷨��˼·����ӱ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SSTF(int cidao[],int m,int now)
{
    int k=1;
    int l,r;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //����ð�������㷨����
    printf("����ɨ������Ϊ��");
    if(cidao[m-1]<=now)   //����ǰ�ŵ��Ŵ�����������������ߣ���ֱ�������������θ�����������
    {
        for(i=m-1; i>=0; i--)
            printf("%d ", cidao[i]);
        sum=now-cidao[0];
    }
    if(cidao[0]>=now)   //����ǰ�ŵ���С��������������С�ߣ���ֱ�������������θ�����������
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])   //����ǰ�ŵ��Ŵ���������������С����С�������
    {
        while(cidao[k]<now)    //ȷ����ǰ�ŵ������ŵ������е�λ�ã�������㷨���õ��ˣ�����ֱ�Ӹ��ƺ������޸ģ���ʡʱ�䡣
        {
            k++;
        }//�ҵ����ڴ��ڵ�ǰ�ŵ���λ��

        //���������������ɨ���������飨���м������ң�
        l=k-1;//��ǰ��
        r=k;//��ǰ��ǰ��
        while((l>=0)&&(r<m))   //��ǰ�ŵ����������з�Χ��
        {
            //ѡ���뵱ǰ�ŵ����������������
            if((now-cidao[l])<=(cidao[r]-now))//���
            {
                printf("%d ", cidao[l]);
                sum+=now-cidao[l];
                now=cidao[l];
                l=l-1;
            }
            else//�ҽ�
            {
                printf("%d ", cidao[r]);
                sum+=cidao[r]-now;
                now=cidao[r];
                r=r+1;
            }
        }
        if(l==-1)   //��ͷ�ƶ������е���С�ţ��������ɨ����δɨ��Ĵŵ�
        {
            for(j=r; j<m; j++)
            {
                printf("%d ", cidao[j]);
            }
            sum+=cidao[m-1]-cidao[0];
        }
        else    //��ͷ�ƶ������е����ţ������ڲ�ɨ����δɨ��Ĵŵ�
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
    printf("ƽ���ƶ��ŵ�����%f\n\n���س�������ɨ���㷨����\n", ave);
}


/*************************ɨ������㷨*************************/
void SCAN(int cidao[],int m,int now)    //��Ҫ������ǰ�ŵ��ź��ƶ��۵��ƶ�����
{
    int k=1;
    int l,r,d;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //����ð�������㷨����
    if(cidao[m-1]<=now)    //����ǰ�ŵ��Ŵ�����������������ߣ���ֱ�������������θ�����������,�����ͬ���Ѱ������
    {
        printf("����ɨ������Ϊ��");
        for(i=m-1; i>=0; i--)
            printf("%d ", cidao[i]);
        sum=now-cidao[0];
    }
    if(cidao[0]>=now)     //����ǰ�ŵ���С��������������С�ߣ���ֱ�������������θ�����������,�����ͬ���Ѱ������
    {
        printf("����ɨ������Ϊ��");
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])   //����ǰ�ŵ��Ŵ���������������С����С�������
    {
        while(cidao[k]<now)
        {
            k++;
        }
        l=k-1;
        r=k;
        printf("������ɨ������㷨�ĵ�ǰ�ƶ��۵��ƶ��ķ��� (1 ��ʾ���� ��0��ʾ����) : ");
        d = geti();
        printf("����ɨ������Ϊ��");
        if(d==0)     //ѡ���ƶ��۷������ڣ���������ɨ��
        {
            for(j=l; j>=0; j--)
            {
                printf("%d ", cidao[j]);//�������ɨ�������
            }
            for(j=r; j<m; j++) //��ͷ�ƶ�����С�ţ���ı䷽������ɨ��δɨ��Ĵŵ�
            {
                printf("%d ", cidao[j]);//�������ɨ�������
            }
            sum=now-2*cidao[0]+cidao[m-1];
        }
        else     //ѡ���ƶ��۷������⣬��������ɨ��
        {
            for(j=r; j<m; j++)
            {
                printf("%d ", cidao[j]);//�������ɨ�������
            }
            for(j=l; j>=0; j--)  //��ͷ�ƶ������ţ���ı䷽������ɨ��δɨ��Ĵŵ�
            {
                printf("%d ", cidao[j]);
            }
            sum=-now-cidao[0]+2*cidao[m-1];
        }
    }
    ave=(float)(sum)/(float)(m);
    printf("\n");
    printf("ƽ���ƶ��ŵ�����%f\n\n���س�������ѭ��ɨ���㷨����\n", ave);
}

/************************ѭ��ɨ������㷨*****************************/
void CSCAN(int cidao[],int m,int now)
{
    int k=1;
    int l,r;
    int i,j,sum=0;
    float ave;
    cidao=bubble(cidao,m);    //����ð�������㷨����
    printf("����ɨ������Ϊ��");
    if(cidao[m-1]<=now)   //����ǰ�ŵ��Ŵ�����������������ߣ���ֱ�ӽ��ƶ����ƶ�����С�Ŵŵ��������������������
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=now-2*cidao[0]+cidao[m-1];
    }
    if(cidao[0]>=now) //����ǰ�ŵ���С��������������С�ߣ���ֱ�������������θ�����������,�����ͬ���Ѱ������
    {
        for(i=0; i<m; i++)
            printf("%d ", cidao[i]);
        sum=cidao[m-1]-now;
    }
    if(now>cidao[0]&&now<cidao[m-1])  //����ǰ�ŵ��Ŵ���������������С����С�������
    {
        while(cidao[k]<now)    //���򷴸��ش�������ɨ��
        {
            k++;
        }
        l=k-1;
        r=k;
        for(j=r; j<m; j++)
        {
            printf("%d ", cidao[j]);//����ӵ�ǰ�ŵ�����ɨ�������
        }
        for(j=0; j<r; j++)   //��ɨ�������Ŵŵ�����ͷֱ���ƶ�����С�Ŵŵ���������ɨ��δɨ��Ĵŵ�
        {
            printf("%d ", cidao[j]);
        }
        sum=2*cidao[m-1]+cidao[l]-now-2*cidao[0];
    }
    ave=(float)(sum)/(float)(m);
    printf("\n");
    printf("ƽ���ƶ��ŵ�����%f\n\n��ȫ���㷨���Ƚ�����ϡ�\n", ave);
}

void main()
{
    int cidao[maxsize],cidaoFCFS[maxsize],cidaoSSTF[maxsize],cidaoSCAN[maxsize],cidaoCSCAN[maxsize];
    int i=0,count,now;
    printf("��������ʴ������������");
    count = geti();
    printf("\nϵͳ����Զ�����Ҫ���ʵĴŵ�����:\n");
    for(i=0; i<count; i++)
    {
        cidao[i]=cidaoFCFS[i]=cidaoSSTF[i]=cidaoSCAN[i]=cidaoCSCAN[i]=rand()%40+80;     //������ɴŵ�����
        printf("%d ", cidao[i]);//����ŵ�����
    }

    printf("\n�����뵱ǰ�Ĵŵ��ţ�");
A:
    now = geti();//���뵱ǰ�ŵ���

    printf("\n�������ȷ����㷨��\n");
    FCFS(cidaoFCFS,count,now);
    getchar();
    printf("�����Ѱ��ʱ�������㷨��\n");
    SSTF(cidaoSSTF,count,now);
    getchar();
    printf("��ɨ���㷨��\n");
    SCAN(cidaoSCAN,count,now);
    getchar();
    printf("��ѭ��ɨ���㷨��\n");
    CSCAN(cidaoCSCAN,count,now);
    getchar();
}

