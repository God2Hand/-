// 操作系统课设.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
//printf("\n******************************************************************************\n");

//内存块
struct BLOCK{
	int pre;//内存前序号
	int next;//内存后序号//包左不包右
	BLOCK* link  ;
	char  name ;
	int ram;//内存大小
};
//内存块链表
struct BList{
	BLOCK *head ;
	int ram;//空闲队列的ram是剩余内存，运行队列的ram是队列节点个数！
};


//定义作业的结构体
struct JCB{
	char name;
	char state;	//作业状态（收容C,运行R，完成F）
	int arrivetime;
	int servicetime;//运行时间
	int ram;
};
//定义进程的结构体
struct PCB{
	char name;
	char state;	//进程状态（就绪W,运行R，完成F）
	int roundtimeless;
	int servicetime;//剩余运行时间
	int ram;
};
//定义作业链表结点
struct JCBNODE{
	JCB p;//注意这里的p是数据结构体，不要和后面的程序中的指针弄混了
	JCBNODE* next;
};
//定义进程链表结点
struct PCBNODE{
	PCB p;//注意这里的p是数据结构体，不要和后面的程序中的指针弄混了
	PCBNODE* next;
};
//定义作业链表
struct JCBLINKER{
	JCBNODE* head;
	int length;
};
//定义进程链表
struct PCBLINKER{
	PCBNODE* head;
	int length;
};


//一些常量参数
const int JCBAMOUNT = 10;//作业总数
const int JCBDEADLINETIME = 50;//作业-截止时间（作业到达时间最大值）
const int JCBRAMMAX = 400;//作业申请的最大运行内存
const int JCBSERVICETIMEMAX = 100;//作业需要运行的最大时间
const int RAMMAX = 1000;//总内存的大小
const int ROUNDTIME = 30;//时间片轮转时间
//++++++++++++
const int RPMAXLENGTH = 2;//同时运行进程（多少道）
//++++++++++++
//全局变量
static int runtime = 0;//系统运行时间
static char NAME = 0;//完成作业的名字
static char tempName = ' ';//到达的进程名字

JCBLINKER allJCB;//全部（收容C）作业队列
JCBLINKER inJCB;//就绪作业队列
JCBLINKER runJCB;//运行作业队列++++++++++++++//每次加到头
JCBLINKER outJCB;//完成作业队列++++++++++++++//每次加到头

PCBLINKER wp;//就绪进程队列
PCBLINKER rp;//运行进程队列
PCBLINKER fp;//完成进程队列

BList rBList;//使用（运行run）内存队列
BList sBList;//空闲（spare）内存队列

//输出作业队列的信息
void visitJCBLinker(JCBLINKER jl) {
	JCBNODE * n = jl.head;
	int index = 0;
	if (jl.length == 0) {
		printf("队列为空。\n");
		printf("\n******************************************************************************\n");
		return;
	}
	printf("此时队列的长度为：%d个\n", jl.length);
	printf("作业名字\t到达时间\t运行时间\t所占内存\t状态\n");
	while (NULL != n && index < jl.length) {
		printf("%c\t\t%d\t\t%d\t\t%d\t\t%c\n", n->p.name, n->p.arrivetime, n->p.servicetime, n->p.ram, n->p.state);
		n = n->next; index++;
	}
	printf("\n******************************************************************************\n");
}

//输出进程队列信息
void visitPCBLinker(PCBLINKER pl) {
	PCBNODE * p = pl.head;
	if (0 == pl.length) {
		printf("队列为空。\n");
		printf("\n******************************************************************************\n");
		return;
	}
	printf("进程名字\t时间片剩余\t还需时间\t状态\n");
	while (NULL != p) {
		printf("%c\t\t%d\t\t%d\t\t%c\n", p->p.name, p->p.roundtimeless, p->p.servicetime, p->p.state);
		p = p->next;
	}
	printf("\n******************************************************************************\n");
}

//输出内存情况
void visitedBlock(BList list) {
	BLOCK * p = list.head;
	if (0 == list.ram) {
		printf("该内存队列为空！\n");
		return;
	}
	while (NULL != p) {
        if ( (p->name >= 'A' && p->name <= 'Z') || (p->name >= 'a' && p->name <= 'z') ) {
            printf("进程%c 所占内存的形式 : 前：%d\t 后：%d\t 占内存：%d\n", p->name, p->pre, p->next, p->ram);
        } else {
            printf("空闲  所占内存的形式 : 前：%d\t 后：%d\t 占内存：%d\n", p->pre, p->next, p->ram);
        }
		p = p->link;
	}
}



/*
初始化程序运行所需要的所有数据，产生随机作业队列，对定义的全局变量初始化。
*/
void init() {
	printf("系统随机产生%d个作业（以时间升序排序）：**************************************\n", JCBAMOUNT);

	time_t t;//相当于长整型
	srand(time(&t));

	//系统随机产生%d个作业
	allJCB.head = (JCBNODE*)malloc(sizeof(JCBNODE));
	JCBNODE *q,*p = allJCB.head;
	p->p.name = 'A';
	p->p.state = 'C';
	p->p.ram = (rand() % (JCBRAMMAX - 1)) + 1;
	p->p.arrivetime = (rand() % (JCBDEADLINETIME - 1)) + 1;
	p->p.servicetime = (rand() % (JCBSERVICETIMEMAX- 1)) + 1;
	allJCB.length = 1;
	for (; allJCB.length <= JCBAMOUNT; allJCB.length++){
		p->next = (JCBNODE*)malloc(sizeof(JCBNODE));
		p = p->next;
		p->p.name = 'A' + allJCB.length;
		p->p.state = 'C';
		p->p.ram = (rand() % (JCBRAMMAX - 1)) + 1;
		p->p.arrivetime = (rand() % (JCBDEADLINETIME - 1)) + 1;
		p->p.servicetime = (rand() % (JCBSERVICETIMEMAX- 1)) + 1;
	}
	allJCB.length--;
	p->next = NULL;
    //以时间升序排序（冒泡排序，每次最后一个到位）
	for (int m = 0; m < allJCB.length - 1; m++){
		p = allJCB.head;
		q = p->next;
		if (p->p.arrivetime > q->p.arrivetime){
			allJCB.head = q;
			p->next = q->next;
			q->next = p;
		}
		p = allJCB.head;
		q = p->next;
		for (int i = 0; i < allJCB.length - m - 2 && NULL != q->next; i++){
			if (q->p.arrivetime > q->next->p.arrivetime){
				JCBNODE * tem = q->next->next;
				p->next = q->next;
				q->next->next = q;
				q->next = tem;
			}
			p = p->next;
			q = p->next;
		}
	}

	//相同到达时间的作业更改
	p = q = allJCB.head;
	while (p->next != NULL) {
        q = p->next;
        while (p->p.arrivetime == q->p.arrivetime) {
            q->p.arrivetime += 1;
            if (q->next != NULL) {
                q = q->next;
            }
        }
        p = p->next;
	}

    //输出作业队列信息
	visitJCBLinker(allJCB);

	//初始化作业队列和进程队列
	inJCB.head = NULL;
	inJCB.length = 0;
	outJCB.head = NULL;
	outJCB.length=0;
	runJCB.head = NULL;
	runJCB.length = 0;
	wp.head = NULL;
	wp.length = 0;
	rp.head = NULL;
	rp.length = 0;
	fp.head = NULL;
	fp.length = 0;
    //初始化内存运行队列和空闲队列
	rBList.head = NULL;
	rBList.ram = 0;
	sBList.head = (BLOCK*)malloc(sizeof(BLOCK));
	sBList.ram = RAMMAX;
	sBList.head->pre = 0;
	sBList.head->next = RAMMAX;
	sBList.head->ram = RAMMAX;
	sBList.head->name = 0;
	sBList.head->link = NULL;
}

/*
判断系统是否运行（尚且 有收容作业 or 有就绪作业 or 有等待进程 or 有运行进程）
*/
bool isRun() {
	if (allJCB.length > 0 || inJCB.length > 0 || wp.length > 0 || rp.length > 0)
		return true;
	else
		return false;
}

/*
是否有作业到达（出现作业）。如果有，投入就绪作业队列
*/
bool appearJCB() {
    //如果收容作业中有作业
	if (allJCB.length > 0) {
        //如果第一个作业到达
		if (allJCB.head->p.arrivetime <= runtime) {
			JCBNODE *p = inJCB.head;
			JCBNODE *q = allJCB.head;
			//存储下全局名字
			tempName = q->p.name;
			//插入到到运行作业的末尾
			if (inJCB.length > 0) {
				//inJCB最后一个结点
				for (int i = 0; i < inJCB.length - 1; i++)
					p = p->next;
				p->next = q;
			}
			else{
				inJCB.head = q;
			}
            //指针处理，收容作业减1，就绪作业加1
			allJCB.head = allJCB.head->next;
			q->next = NULL;
			inJCB.length++;
			allJCB.length--;
			return true;
		}
	}
    return false;
}

/*
判断作业是否可以调入内存运行
*/
bool isJoinPCB() {
	if (FF()) {
		return true;
	} else {
		return false;
	}
}
/*
首次适应算法
*/
bool FF() {
	BLOCK * b = sBList.head;//空闲内存队列
	BLOCK * p = rBList.head;//插入前置节点
	BLOCK * pp = (BLOCK*)malloc(sizeof(BLOCK));//新内存节点

	//剩余内存比作业内存小，不能装入
	if (sBList.ram < inJCB.head->p.ram) {
        free(pp);
		return false;
	} else {
    //能装入
		while (b != NULL) {
			if (b->ram >= inJCB.head->p.ram) {
				pp->name = inJCB.head->p.name;
				pp->ram = inJCB.head->p.ram;
				pp->pre = b->pre;
				pp->next = b->pre + inJCB.head->p.ram;

				//分配得到的内存插入到运行队列末尾
				p = rBList.head;
				if (NULL == rBList.head) {
					rBList.head = pp;
					pp->link = NULL;
				} else {
					while (NULL != p->link) {
						p = p->link;
					}
					p->link = pp;
					pp->link = NULL;
				}
				//运行作业数量增加
				rBList.ram++;

				//计算节点剩余的
				p = sBList.head;
				b->pre = b->pre + pp->ram;
				b->ram = b->ram - pp->ram;
				//总量剩余
				sBList.ram -= pp->ram;

				//0为空或1碎片不要了，置空
				if (b->ram <= 1) {
					while (p != b && p->link != b)
						p = p->link;
					if (sBList.head == p) {
						sBList.head = NULL;
					} else {
						p->link = b->link;
					}
					free(b);
				}
				return true;//作业加入内存
			} else {
				b = b->link;
			}
		}
	}
	free(pp);
	return false;//作业装载到内存中失败。
}

/*
由作业产生进程，进程加入到 进程就绪队列（wp）中。
*/
bool JCBtoPCB() {
	PCBNODE* p = NULL;
	PCBNODE* q = wp.head;

	p = (PCBNODE*)malloc(sizeof(PCBNODE));
	p->p.name = inJCB.head->p.name;
	p->p.ram = inJCB.head->p.ram;
	p->p.servicetime = inJCB.head->p.servicetime;
	p->p.state = 'W';
	//时间片
	p->p.roundtimeless = ROUNDTIME;
	p->next = NULL;

	if (NULL == q){
		wp.head = p;
	} else {
		while (NULL != q->next)
			q = q->next;
		q->next = p;
	}
	wp.length++;
	return true;
}

/*
将inJCB到runJCB中，作业状态变化。
*/
bool JCBin2run() {
	JCBNODE *p = inJCB.head;
	p->p.state = 'R';
	if (inJCB.length == 1) {
		inJCB.head = NULL;
	} else {
		inJCB.head = inJCB.head->next;
	}
	inJCB.length--;
	p->next = NULL;
	if (runJCB.length == 0) {
		runJCB.head = p;
	} else {
		p->next = runJCB.head;
		runJCB.head = p;
	}
	runJCB.length++;
	return true;
}

/*
进程状态由 w 到 r。进程状态转换。队列位置变换。
*/
bool PCBw2r() {
	PCBNODE* p = wp.head;
	PCBNODE* q = rp.head;
	if (wp.length == 0 || rp.length >= RPMAXLENGTH) {
		return false;
	} else {
		p->p.state = 'R';
		if (wp.length == 1) {
			wp.head = NULL;
		} else {
			wp.head = wp.head->next;
		}
		wp.length--;

		p->next = NULL;
		if (rp.length == 0) {
            rp.head = p;
		} else {
            while (q->next != NULL) {
                q = q->next;
            }
            q->next = p;
		}
		rp.length++;
		return true;
	}
	return false;
}

//时间片轮转算法
void RoundRobin() {
    //作业运行
	rp.head->p.servicetime--;
	rp.head->p.roundtimeless--;

	if (rp.head->p.servicetime <= 0) {
    //进程作业完成
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 进程%c 运行结束 *******************************\n", rp.head->p.name);
		visitPCBLinker(rp);
		//进程 运行 -> 完成
		PCBr2f();
		printf("******************************* 结束 进程 队列 *******************************\n");
		visitPCBLinker(fp);
        //作业 运行 -> 完成
		JCBrun2out();
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 作业%c 运行完成 *******************************\n", outJCB.head->p.name);
        printf("作业完结情况：\n");
		visitJCBLinker(outJCB);

        //释放作业结束的内存
		freeBlock();
		NAME = 0;
		printf("\n******************************* 系统 内存 状态 *******************************\n");
		printf("\n空闲内存链：\n");
		visitedBlock(sBList);
		printf("\n运行内存链：\n");
		visitedBlock(rBList);
		printf("\n******************************************************************************\n");

		getchar();
		return;
	} else if (rp.head->p.roundtimeless <= 0) {
    //时间片到达
		rp.head->p.roundtimeless = ROUNDTIME;
		printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 时间片用完，进程%c 无法结束，开始时间片轮转算法。\n", rp.head->p.name);
		//进程 运行 -> 就绪
		PCBr2w();
		//进程 就绪 -> 运行
		PCBw2r();
		printf("\n******************************* 运行 进程 队列 *******************************\n");
		visitPCBLinker(rp);
		printf("\n******************************* 就绪 进程 队列 *******************************\n");
		visitPCBLinker(wp);

		getchar();
		return;
	}

	//运行进程切换
	PCBr2r();
	return;
}

/*
判断 进程状态由 r 到 f。进程状态转换。队列位置变换。
*/
bool PCBr2f() {
	PCBNODE* q = fp.head;
	rp.head->p.state = 'F';
	//完成进程的名字
	NAME = rp.head->p.name;
	if (rp.head->p.servicetime > 0) {
		return false;
	}
	if (NULL == q) {
		fp.head = rp.head;
	} else {
		while (NULL != q->next)
			q = q->next;
		q->next = rp.head;
	}
	q = rp.head;
	//运行进程队列去头
	rp.head = rp.head->next;
	q->next = NULL;
	rp.length--;
	fp.length++;

	return true;
}

/*
将运行完成的作业加入到作业完成队列当中（按名查找）
*/
bool JCBrun2out() {
	JCBNODE*p = runJCB.head;//最后指向完成的作业
	JCBNODE*pp = runJCB.head;//完成作业的前一个
	if (runJCB.length == 0){
		return false;
	} else if (runJCB.length == 1) {
		runJCB.head->next = outJCB.head;
		outJCB.head = runJCB.head;
		outJCB.length++;
		outJCB.head->p.state = 'F';
		runJCB.head = NULL;
		runJCB.length--;
		return true;
	} else {
		if (runJCB.head->p.name == NAME) {
			runJCB.head = runJCB.head->next;
		} else {
			while (p->p.name != NAME)
				p = p->next;
			while (pp->next != p)
				pp = pp->next;
			pp->next = p->next;
		}
		p->next = outJCB.head;
		runJCB.length--;
		outJCB.head = p;
		outJCB.length++;
		outJCB.head->p.state = 'F';
		return true;
	}
	return false;
}

/*
释放作业结束的内存（按名）
*/
bool freeBlock() {
	BLOCK *p1 = rBList.head;//作业占用的内存指针
	BLOCK *p2 = rBList.head;//前向指针
	BLOCK *p3 = sBList.head;

	//运行内存队列抽出p1
	if (rBList.ram == 1) {
		rBList.head = NULL;
		rBList.ram--;
	} else {
		while ((p1->name!=NAME))
			p1 = p1->link;
		printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 释放作业%c 的内存块指针：\n前：%d ,后： %d。\n",p1->name, p1->pre, p1->next);
		while (p2 != p1 && p2->link != p1)
			p2 = p2->link;
		if (p2 == p1) { //是头节点
			rBList.head = p1->link;
		} else {
			p2->link = p1->link;
		}
		rBList.ram--;
		p1->link = NULL;
		p1->name = 0;
	}
    //p1加入空闲内存队列
	p2 = sBList.head;//p1编址之后
	p3 = sBList.head;//p2前一个指针，即p1编址之前
	if (sBList.ram == 0) {
		sBList.head = p1;
		sBList.ram += p1->ram;
	} else {
		while (NULL != p2 && p2->pre < p1->next)
			p2 = p2->link;
		while (NULL != p3 && p3 != p2 && p3->link != p2) {
			p3 = p3->link;
		}

		//插入or合并
        //一、p2是头，p1只有后
		if (p2 == sBList.head) {
			if (p2->pre <= (p1->next + 1)) {    //合并：前的后+1 == 后的前+1 或者大1
				p2->pre -= p1->ram;
				p2->ram += p1->ram;
				sBList.ram += p1->ram;
			} else {    //插到前面
				p1->link = p2;
				sBList.head = p1;
				sBList.ram += p1->ram;
			}
		} else if (NULL == p2) {
        //二、空闲队列都在p1编址之前，p1只有前
			if (p3->next >= p1->pre - 1) {  //合并，队尾的后 == 插入的前-1 或者大1
				p3->next += p1->ram;
				p3->ram += p1->ram;
				sBList.ram += p1->ram;
			} else { //插入，插入队尾后面
				p3->link = p1;
				sBList.ram += p1->ram;
			}
		} else {
        //三、中间，p1有前有后
			if ((p3->next >= p1->pre-1 ) && (p2->pre <= p1->next+1 )) { //合并前后
				sBList.ram -= p3->ram;
				p3->ram = p1->ram + p2->ram + p3->ram;
				p3->next = p3->pre + p3->ram;
				p3->link = p2->link;
				sBList.ram += p3->ram;
			} else if (p2->pre <= p1->next+1 ) {    //只合并后
				p2->pre -= p1->ram;
				p2->ram += p1->ram;
				sBList.ram += p1->ram;
			} else if (p3->next >= p1->pre-1 ) {    //只合并前
				p3->next += p1->ram;
				p3->ram += p1->ram;
				sBList.ram += p1->ram;
			} else {    //不合并，插入
				p3->link = p1;
				p1->link = p2;
				sBList.ram += p1->ram;
			}
		}
	}
	return true;
}

/*
进程状态由 r 到 w。进程状态转换。队列位置变换。
*/
bool PCBr2w() {
	PCBNODE* q = wp.head;
	rp.head->p.state = 'W';
	if (NULL == q){
		wp.head = rp.head;
	} else {
		while (NULL != q->next)
			q = q->next;
		q->next = rp.head;
	}
	q = rp.head;
	//运行进程队列去头
	rp.head = rp.head->next;
	q->next = NULL;
	rp.length--;
	wp.length++;
	return true;
}

/*
进程切换，运行进程放到末尾
*/
bool PCBr2r() {
    PCBNODE* q = rp.head;
    while (q->next != NULL) {
        q = q->next;
    }
    q->next = rp.head;
    rp.head = rp.head->next;
    q->next->next = NULL;
}



int main1() {
	int a = 0;//输入字符
    int i = 0;

    printf("------------------------------------------------------------------------------\n");
    printf("******************************************************************************\n");
    printf("***************************** 欢迎来到  调度系统 *****************************\n");
    printf("******************************************************************************\n");
    printf("------------------------------------------------------------------------------\n\n");

    printf("------------------------------------------------------------------------------\n");
    //初始化程序，产生随机作业队列
	init();
	printf("------------------------------------------------------------------------------\n");

	printf("\n\n\n-------------------------------- 系统开始运行 --------------------------------\n");
    //系统运行（尚且 有收容作业 or 有就绪作业 or 有等待进程 or 有运行进程）
	while (true==isRun()) {
        //系统时间记录，0~n
		runtime++;
		printf("\n---------------------------- 当前系统时间为：%4d ----------------------------\n",runtime);

        //有作业到达（出现作业）。如果有，投入就绪作业队列后输出
		if (true==appearJCB()) {
			printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 作业%c 到达。\n",tempName);
			printf("\n******************************* 就绪 作业 队列 *******************************\n");
			visitJCBLinker(inJCB);
		}
		//把就绪作业加入进程，变为运行状态
		if (inJCB.length > 0) {
			if (true==isJoinPCB()) {
				printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 内存足够，作业%c 可以装载入内存中。\n",tempName);
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 就绪作业%c 提交运行。\n",tempName);
				tempName = ' ';
				//由作业产生进程，进程加入到 进程就绪队列（wp）中。
				JCBtoPCB();
				printf("\n******************************* 就绪 进程 队列 *******************************\n");
				visitPCBLinker(wp);

                //将inJCB到runJCB中，作业状态变化。
				JCBin2run();
				printf("\n******************************* 就绪 作业 队列 *******************************\n");
				visitJCBLinker(inJCB);
				printf("\n******************************* 运行 作业 队列 *******************************\n");
				visitJCBLinker(runJCB);

				printf("\n\n******************************* 系统 内存 状态 *******************************\n");
				printf("\n空闲内存链:\n");
				visitedBlock(sBList);
				printf("\n作业内存链:\n");
				visitedBlock(rBList);
				printf("\n******************************************************************************\n");

				a = 0; getchar();
			} else {
				printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 剩余内存空间无法装载作业。\n");
				visitedBlock(sBList);
			}
		}

        //有进程需要运行（就绪进程or运行进程）
        if (wp.length > 0 || rp.length > 0) {
			if (wp.length >0 && rp.length < RPMAXLENGTH) {
				printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 就绪进程提交到运行进程。\n");
				//进程状态由 w 到 r。进程状态转换。队列位置变换。
				for (i = 0; i < RPMAXLENGTH; i++) {
                    PCBw2r();
				}
				printf("\n******************************* 就绪 进程 队列 *******************************\n");
				visitPCBLinker(wp);
				printf("\n******************************* 运行 进程 队列 *******************************\n");
				visitPCBLinker(rp);

				a = 0; getchar();
			}
			//进程运行
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 进程运行\n");
			int length = rp.length;
			//时间片轮转算法
			for (i = 0; i < length; i++) {
                RoundRobin();
			}
			printf("\n******************************* 运行 进程 队列 *******************************\n");
            visitPCBLinker(rp);
		}

	}

	printf("\n\n----------------------------- 总运行时间为：%d! -----------------------------\n", runtime);
	printf("---------------------------- 运行结束,输入'0'退出 ----------------------------\n");

	//输入“0”结束程序
	a = getchar();
	while (a != '0')
		a = getchar();
}




