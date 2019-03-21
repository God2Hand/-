#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Free 0 //����״̬
#define Busy 1 //����״̬
#define OK 1//���
#define ERROR 0 //����
#define MAX_length 640 //����ڴ�ռ�Ϊ640KB

#define HEAD -1 //ͷ���ID��size��address��state�ȣ������ж�

typedef int Status;
int b;
typedef struct freearea//����һ��������˵����ṹ
{
    int ID;   //������
    int size;   //������С
    int address; //������ַ
    int state;   //״̬
} ElemType;

//----------  ���Ա��˫������洢�ṹ  ------------
typedef struct DuLNode //double linked list
{
    ElemType data;
    struct DuLNode *prior; //ǰ��ָ��
    struct DuLNode *next;  //���ָ��
} DuLNode,*DuLinkList;

DuLinkList block_first; //ͷ���
DuLinkList block_last;  //β���

Status allocBlock(int);//�ڴ����
Status freeBlock(int); //�ڴ����
Status First_fit(int,int);//�״���Ӧ�㷨
Status Best_fit(int,int); //�����Ӧ�㷨
void show();//�鿴����
Status Initblock();//�����ռ��

//ʹ�û�ѡ��
int geti()
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("����Ϊ�գ�����������Ǹ�������\n");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("�����������������Ǹ�������\n");
			return geti();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

Status Initblock()//������ͷ�����ڴ�ռ�����
{
    block_first=(DuLinkList)malloc(sizeof(DuLNode));
    block_last=(DuLinkList)malloc(sizeof(DuLNode));
    block_first->prior=NULL;
    block_first->next=block_last;
    block_last->prior=block_first;
    block_last->next=NULL;
    block_last->data.address=0;
    block_last->data.size=MAX_length;
    block_last->data.ID=0;
    block_last->data.state=Free;
    //+++++++++++++++++++++++++++++++++++
    block_first->data.ID = -1;
    block_first->data.address = -1;
    block_first->data.size = -1;
    block_first->data.state -1;
    //���߸��ܶ������ǰ��ָ�벻Ϊ0
    return OK;
}

//----------------------- �� �� �� �� -------------------------
Status allocBlock(int ch)
{
    int ID,request;
    printf("��������ҵ(������)��");
    ID = geti();
    while (ID == 0) {    //++++++++++++++++++++++++++++++++++ע������Ų�Ϊ0
        printf("\n�����Ų�Ϊ0�����������룡\n");
        ID = geti();
    }
    //�����������ͬ��ô�죬����չʾ
    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.state==Busy && p->data.ID==ID)
        {
            printf("\n�÷������Ѿ����ڲ��ұ����䣡\n");
            return ERROR;
            break;
        }
        p=p->next;
    }

    printf("��������Ҫ����������С(��λ:KB)��");
    request = geti();
    if(request<0 ||request==0)
    {
        printf("\n�����С�����ʣ������ԣ�\n");
        return ERROR;
    }

    if(ch==2) //ѡ�������Ӧ�㷨
    {
        if(Best_fit(ID,request)==OK)
            printf("");
        else
            printf("\n�ڴ治�㣬����ʧ�ܣ�\n");
        return OK;
    }
    else //Ĭ���״���Ӧ�㷨
    {
        if(First_fit(ID,request)==OK)
            printf("\n����ɹ���\n");
        else
            printf("\n�ڴ治�㣬����ʧ�ܣ�\n");
        return OK;
    }
}
//------------------ �״���Ӧ�㷨 -----------------------
Status First_fit(int ID,int request)//������ҵ����������
{
    //Ϊ������ҵ�����¿ռ��ҳ�ʼ��
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID=ID;
    temp->data.size=request;
    temp->data.state=Busy;

    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.state==Free && p->data.size==request)
        {
            //�д�Сǡ�ú��ʵĿ��п�
            p->data.state=Busy;
            p->data.ID=ID;
            //+++++++++++++++++++++++++++++++++++++++++
            free(temp);
            return OK;
            break;
        }
        if(p->data.state==Free && p->data.size>request)
        {
            //�п��п���������������ʣ��"
            temp->prior=p->prior;
            temp->next=p;
            temp->data.address=p->data.address;
            p->prior->next=temp;
            p->prior=temp;
            p->data.address=temp->data.address + temp->data.size;
            p->data.size -= request;
            return OK;
            break;
        }
        p=p->next;
    }
    return ERROR;
}
//--------------------  �����Ӧ�㷨  ------------------------
Status Best_fit(int ID,int request)
{
    int ch = MAX_length; //��¼��Сʣ��ռ䣬��ʼ��Ϊ����ڴ�
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID=ID;
    temp->data.size=request;
    temp->data.state=Busy;
    DuLNode *p=block_first->next;
    DuLNode *q=NULL; //��¼��Ѳ���λ��
    while(p)
    {
        if(p->data.state==Free && p->data.size==request)
        {
            //���п��Сǡ�ú���
            p->data.ID=ID;
            p->data.state=Busy;
            //+++++++++++++++++++++++++++++++++++++++++
            free(temp);
            return OK;
            break;
        }
        if(p->data.state==Free && p->data.size>request)
        {
            //���п���ڷ�������
            int rest = p->data.size-request;
            if (rest < ch) {
                ch = rest;//����ʣ����Сֵ
                q = p;//�������λ��ָ��
            }
        }
        p=p->next;
    }
    if(q==NULL) return ERROR;//û���ҵ����п�
    else
    {
        //�ҵ������λ�ò�ʵ�ַ���
        temp->prior=q->prior;
        temp->next=q;
        temp->data.address=q->data.address;
        q->prior->next=temp;
        q->prior=temp;
        q->data.address+=request;
        q->data.size=ch;
        return OK;
    }
}

//-----------------------   �� �� �� ��   --------------------
Status freeBlock(int ID)
{
    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.ID==ID)
        {
            p->data.state=Free;
            p->data.ID=Free;
            if(p->prior->data.state==Free)//��ǰ��Ŀ��п��������ϲ���ǰ��
            {
                p->prior->data.size+=p->data.size;
                p->prior->next=p->next;
                p->next->prior=p->prior;
                //+++++++++++++++++++++++�ǵ��ͷ�ʣ�µ�p
                free(p);
                p = NULL;
                break;

            }
            if(p->next != NULL && p->next->data.state==Free)//�����Ŀ��п��������ϲ�������
            {
                p->next->data.size+=p->data.size;
                p->next->prior=p->prior;
                p->prior->next=p->next;
                //+++++++++++++++++++++++�ǵ��ͷ�ʣ�µ�p
                free(p);
                p = NULL;
                break;
            }
        }
        p=p->next;
    }
    return OK;
}

//---------------  ��ʾ���������� ------------------
void show()
{
    printf("\n******************************************\n");
    printf("\n               ����������\n");

    DuLNode *p=block_first->next;
    while(p)
    {
        printf("      ______________________________      \n");
        printf("\n              �� �� �ţ�");
        if(p->data.ID==Free)
            printf("Free\n");
        else
            printf("%d\n", p->data.ID);
        printf("              ��ʼ��ַ��%d\n", p->data.address);
        printf("              ������С��%d KB\n", p->data.size);
        printf("              ״    ̬��");
        if(p->data.state==Free)
            printf("��  ��\n");
        else
            printf("�ѷ���\n");
        p=p->next;
    }
    printf("\n******************************************\n");
    printf("\n���س������ز˵�\n");
    getchar();
}

//----------------------- ��  ��  ��---------------------------
void main()
{
    int ch;//�㷨ѡ����
A:  system("cls");
    printf("\n***************��ѡ������㷨***************\n\n\n");
    printf("              1.�״���Ӧ�㷨\n\n              2.�����Ӧ�㷨\n\n              0.����\n\n\n");
    printf("********************************************\n\n");
    printf("���ѡ���ǣ�");
    ch = geti();

    if (ch == 0) {
        return 0;
    }

    Initblock(); //�����ռ��

    int choice;  //����ѡ����
    system("cls");
    while(1)
    {
        if(ch==1)
            printf("\n***************�״���Ӧ�㷨***************\n\n\n");
        else
            printf("\n***************�����Ӧ�㷨***************\n\n\n");
        printf("             1.�����ڴ�\n\n             2.�����ڴ�\n\n             0.�˳�����ѡ���㷨\n\n\n");
        printf("******************************************\n\n");
        printf(" ���������Ĳ��� ��");

        choice = geti();
        if(choice==1)
        {
            allocBlock(ch); // �����ڴ�
            show();
        }
        else if(choice==2)  // �ڴ����
        {
            int ID;
            printf("��������Ҫ�ͷŵķ����ţ�");
            ID = geti();
            while (ID == 0) {    //++++++++++++++++++++++++++++++++++ע������Ų�Ϊ0
                printf("\n�����Ų�Ϊ0�����������룡\n");
                ID = geti();
            }
            //�����ò�����
            DuLNode *p=block_first->next;
            while(p)
            {
                if(p->data.state==Busy && p->data.ID==ID)
                {
                    freeBlock(ID);
                    break;
                }
                p=p->next;
            }
            if (p == NULL) {
                printf("\n�÷����Ų�����\n");
            }
            show();
        }
        else if(choice==0)
            goto A; //�˳�
        else //�����������
        {
            printf("�������������ԣ�\n");
            continue;
        }
    }
}




