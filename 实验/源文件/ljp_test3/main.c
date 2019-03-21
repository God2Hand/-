#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Free 0 //空闲状态
#define Busy 1 //已用状态
#define OK 1//完成
#define ERROR 0 //出错
#define MAX_length 640 //最大内存空间为640KB

#define HEAD -1 //头结点ID、size、address、state等，方便判断

typedef int Status;
int b;
typedef struct freearea//定义一个空闲区说明表结构
{
    int ID;   //分区号
    int size;   //分区大小
    int address; //分区地址
    int state;   //状态
} ElemType;

//----------  线性表的双向链表存储结构  ------------
typedef struct DuLNode //double linked list
{
    ElemType data;
    struct DuLNode *prior; //前趋指针
    struct DuLNode *next;  //后继指针
} DuLNode,*DuLinkList;

DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点

Status allocBlock(int);//内存分配
Status freeBlock(int); //内存回收
Status First_fit(int,int);//首次适应算法
Status Best_fit(int,int); //最佳适应算法
void show();//查看分配
Status Initblock();//开创空间表

//使用户选择
int geti()
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	while(ch == '\n'){
		printf("不能为空，请重新输入非负整数！\n");
		fflush(stdin);
		ch = getchar();
	}
	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			printf("输入错误，请重新输入非负整数！\n");
			return geti();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

Status Initblock()//开创带头结点的内存空间链表
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
    //或者给很多个加上前向指针不为0
    return OK;
}

//----------------------- 分 配 主 存 -------------------------
Status allocBlock(int ch)
{
    int ID,request;
    printf("请输入作业(分区号)：");
    ID = geti();
    while (ID == 0) {    //++++++++++++++++++++++++++++++++++注意分区号不为0
        printf("\n分区号不为0，请重新输入！\n");
        ID = geti();
    }
    //如果分区号相同怎么办，给个展示
    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.state==Busy && p->data.ID==ID)
        {
            printf("\n该分区号已经存在并且被分配！\n");
            return ERROR;
            break;
        }
        p=p->next;
    }

    printf("请输入需要分配的主存大小(单位:KB)：");
    request = geti();
    if(request<0 ||request==0)
    {
        printf("\n分配大小不合适，请重试！\n");
        return ERROR;
    }

    if(ch==2) //选择最佳适应算法
    {
        if(Best_fit(ID,request)==OK)
            printf("");
        else
            printf("\n内存不足，分配失败！\n");
        return OK;
    }
    else //默认首次适应算法
    {
        if(First_fit(ID,request)==OK)
            printf("\n分配成功！\n");
        else
            printf("\n内存不足，分配失败！\n");
        return OK;
    }
}
//------------------ 首次适应算法 -----------------------
Status First_fit(int ID,int request)//传入作业名及申请量
{
    //为申请作业开辟新空间且初始化
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID=ID;
    temp->data.size=request;
    temp->data.state=Busy;

    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.state==Free && p->data.size==request)
        {
            //有大小恰好合适的空闲块
            p->data.state=Busy;
            p->data.ID=ID;
            //+++++++++++++++++++++++++++++++++++++++++
            free(temp);
            return OK;
            break;
        }
        if(p->data.state==Free && p->data.size>request)
        {
            //有空闲块能满足需求且有剩余"
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
//--------------------  最佳适应算法  ------------------------
Status Best_fit(int ID,int request)
{
    int ch = MAX_length; //记录最小剩余空间，初始化为最大内存
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID=ID;
    temp->data.size=request;
    temp->data.state=Busy;
    DuLNode *p=block_first->next;
    DuLNode *q=NULL; //记录最佳插入位置
    while(p)
    {
        if(p->data.state==Free && p->data.size==request)
        {
            //空闲块大小恰好合适
            p->data.ID=ID;
            p->data.state=Busy;
            //+++++++++++++++++++++++++++++++++++++++++
            free(temp);
            return OK;
            break;
        }
        if(p->data.state==Free && p->data.size>request)
        {
            //空闲块大于分配需求
            int rest = p->data.size-request;
            if (rest < ch) {
                ch = rest;//更新剩余最小值
                q = p;//更新最佳位置指向
            }
        }
        p=p->next;
    }
    if(q==NULL) return ERROR;//没有找到空闲块
    else
    {
        //找到了最佳位置并实现分配
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

//-----------------------   主 存 回 收   --------------------
Status freeBlock(int ID)
{
    DuLNode *p=block_first->next;
    while(p)
    {
        if(p->data.ID==ID)
        {
            p->data.state=Free;
            p->data.ID=Free;
            if(p->prior->data.state==Free)//与前面的空闲块相连，合并到前面
            {
                p->prior->data.size+=p->data.size;
                p->prior->next=p->next;
                p->next->prior=p->prior;
                //+++++++++++++++++++++++记得释放剩下的p
                free(p);
                p = NULL;
                break;

            }
            if(p->next != NULL && p->next->data.state==Free)//与后面的空闲块相连，合并到后面
            {
                p->next->data.size+=p->data.size;
                p->next->prior=p->prior;
                p->prior->next=p->next;
                //+++++++++++++++++++++++记得释放剩下的p
                free(p);
                p = NULL;
                break;
            }
        }
        p=p->next;
    }
    return OK;
}

//---------------  显示主存分配情况 ------------------
void show()
{
    printf("\n******************************************\n");
    printf("\n               主存分配情况\n");

    DuLNode *p=block_first->next;
    while(p)
    {
        printf("      ______________________________      \n");
        printf("\n              分 区 号：");
        if(p->data.ID==Free)
            printf("Free\n");
        else
            printf("%d\n", p->data.ID);
        printf("              起始地址：%d\n", p->data.address);
        printf("              分区大小：%d KB\n", p->data.size);
        printf("              状    态：");
        if(p->data.state==Free)
            printf("空  闲\n");
        else
            printf("已分配\n");
        p=p->next;
    }
    printf("\n******************************************\n");
    printf("\n按回车键返回菜单\n");
    getchar();
}

//----------------------- 主  函  数---------------------------
void main()
{
    int ch;//算法选择标记
A:  system("cls");
    printf("\n***************请选择分配算法***************\n\n\n");
    printf("              1.首次适应算法\n\n              2.最佳适应算法\n\n              0.结束\n\n\n");
    printf("********************************************\n\n");
    printf("你的选择是：");
    ch = geti();

    if (ch == 0) {
        return 0;
    }

    Initblock(); //开创空间表

    int choice;  //操作选择标记
    system("cls");
    while(1)
    {
        if(ch==1)
            printf("\n***************首次适应算法***************\n\n\n");
        else
            printf("\n***************最佳适应算法***************\n\n\n");
        printf("             1.分配内存\n\n             2.回收内存\n\n             0.退出重新选择算法\n\n\n");
        printf("******************************************\n\n");
        printf(" 请输入您的操作 ：");

        choice = geti();
        if(choice==1)
        {
            allocBlock(ch); // 分配内存
            show();
        }
        else if(choice==2)  // 内存回收
        {
            int ID;
            printf("请输入您要释放的分区号：");
            ID = geti();
            while (ID == 0) {    //++++++++++++++++++++++++++++++++++注意分区号不为0
                printf("\n分区号不为0，请重新输入！\n");
                ID = geti();
            }
            //分区好不存在
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
                printf("\n该分区号不存在\n");
            }
            show();
        }
        else if(choice==0)
            goto A; //退出
        else //输入操作有误
        {
            printf("输入有误，请重试！\n");
            continue;
        }
    }
}




