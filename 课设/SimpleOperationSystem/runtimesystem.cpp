// ����ϵͳ����.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
//printf("\n******************************************************************************\n");

//�ڴ��
struct BLOCK{
	int pre;//�ڴ�ǰ���
	int next;//�ڴ�����//���󲻰���
	BLOCK* link  ;
	char  name ;
	int ram;//�ڴ��С
};
//�ڴ������
struct BList{
	BLOCK *head ;
	int ram;//���ж��е�ram��ʣ���ڴ棬���ж��е�ram�Ƕ��нڵ������
};


//������ҵ�Ľṹ��
struct JCB{
	char name;
	char state;	//��ҵ״̬������C,����R�����F��
	int arrivetime;
	int servicetime;//����ʱ��
	int ram;
};
//������̵Ľṹ��
struct PCB{
	char name;
	char state;	//����״̬������W,����R�����F��
	int roundtimeless;
	int servicetime;//ʣ������ʱ��
	int ram;
};
//������ҵ������
struct JCBNODE{
	JCB p;//ע�������p�����ݽṹ�壬��Ҫ�ͺ���ĳ����е�ָ��Ū����
	JCBNODE* next;
};
//�������������
struct PCBNODE{
	PCB p;//ע�������p�����ݽṹ�壬��Ҫ�ͺ���ĳ����е�ָ��Ū����
	PCBNODE* next;
};
//������ҵ����
struct JCBLINKER{
	JCBNODE* head;
	int length;
};
//�����������
struct PCBLINKER{
	PCBNODE* head;
	int length;
};


//һЩ��������
const int JCBAMOUNT = 10;//��ҵ����
const int JCBDEADLINETIME = 50;//��ҵ-��ֹʱ�䣨��ҵ����ʱ�����ֵ��
const int JCBRAMMAX = 400;//��ҵ�������������ڴ�
const int JCBSERVICETIMEMAX = 100;//��ҵ��Ҫ���е����ʱ��
const int RAMMAX = 1000;//���ڴ�Ĵ�С
const int ROUNDTIME = 30;//ʱ��Ƭ��תʱ��
//++++++++++++
const int RPMAXLENGTH = 2;//ͬʱ���н��̣����ٵ���
//++++++++++++
//ȫ�ֱ���
static int runtime = 0;//ϵͳ����ʱ��
static char NAME = 0;//�����ҵ������
static char tempName = ' ';//����Ľ�������

JCBLINKER allJCB;//ȫ��������C����ҵ����
JCBLINKER inJCB;//������ҵ����
JCBLINKER runJCB;//������ҵ����++++++++++++++//ÿ�μӵ�ͷ
JCBLINKER outJCB;//�����ҵ����++++++++++++++//ÿ�μӵ�ͷ

PCBLINKER wp;//�������̶���
PCBLINKER rp;//���н��̶���
PCBLINKER fp;//��ɽ��̶���

BList rBList;//ʹ�ã�����run���ڴ����
BList sBList;//���У�spare���ڴ����

//�����ҵ���е���Ϣ
void visitJCBLinker(JCBLINKER jl) {
	JCBNODE * n = jl.head;
	int index = 0;
	if (jl.length == 0) {
		printf("����Ϊ�ա�\n");
		printf("\n******************************************************************************\n");
		return;
	}
	printf("��ʱ���еĳ���Ϊ��%d��\n", jl.length);
	printf("��ҵ����\t����ʱ��\t����ʱ��\t��ռ�ڴ�\t״̬\n");
	while (NULL != n && index < jl.length) {
		printf("%c\t\t%d\t\t%d\t\t%d\t\t%c\n", n->p.name, n->p.arrivetime, n->p.servicetime, n->p.ram, n->p.state);
		n = n->next; index++;
	}
	printf("\n******************************************************************************\n");
}

//������̶�����Ϣ
void visitPCBLinker(PCBLINKER pl) {
	PCBNODE * p = pl.head;
	if (0 == pl.length) {
		printf("����Ϊ�ա�\n");
		printf("\n******************************************************************************\n");
		return;
	}
	printf("��������\tʱ��Ƭʣ��\t����ʱ��\t״̬\n");
	while (NULL != p) {
		printf("%c\t\t%d\t\t%d\t\t%c\n", p->p.name, p->p.roundtimeless, p->p.servicetime, p->p.state);
		p = p->next;
	}
	printf("\n******************************************************************************\n");
}

//����ڴ����
void visitedBlock(BList list) {
	BLOCK * p = list.head;
	if (0 == list.ram) {
		printf("���ڴ����Ϊ�գ�\n");
		return;
	}
	while (NULL != p) {
        if ( (p->name >= 'A' && p->name <= 'Z') || (p->name >= 'a' && p->name <= 'z') ) {
            printf("����%c ��ռ�ڴ����ʽ : ǰ��%d\t ��%d\t ռ�ڴ棺%d\n", p->name, p->pre, p->next, p->ram);
        } else {
            printf("����  ��ռ�ڴ����ʽ : ǰ��%d\t ��%d\t ռ�ڴ棺%d\n", p->pre, p->next, p->ram);
        }
		p = p->link;
	}
}



/*
��ʼ��������������Ҫ���������ݣ����������ҵ���У��Զ����ȫ�ֱ�����ʼ����
*/
void init() {
	printf("ϵͳ�������%d����ҵ����ʱ���������򣩣�**************************************\n", JCBAMOUNT);

	time_t t;//�൱�ڳ�����
	srand(time(&t));

	//ϵͳ�������%d����ҵ
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
    //��ʱ����������ð������ÿ�����һ����λ��
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

	//��ͬ����ʱ�����ҵ����
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

    //�����ҵ������Ϣ
	visitJCBLinker(allJCB);

	//��ʼ����ҵ���кͽ��̶���
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
    //��ʼ���ڴ����ж��кͿ��ж���
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
�ж�ϵͳ�Ƿ����У����� ��������ҵ or �о�����ҵ or �еȴ����� or �����н��̣�
*/
bool isRun() {
	if (allJCB.length > 0 || inJCB.length > 0 || wp.length > 0 || rp.length > 0)
		return true;
	else
		return false;
}

/*
�Ƿ�����ҵ���������ҵ��������У�Ͷ�������ҵ����
*/
bool appearJCB() {
    //���������ҵ������ҵ
	if (allJCB.length > 0) {
        //�����һ����ҵ����
		if (allJCB.head->p.arrivetime <= runtime) {
			JCBNODE *p = inJCB.head;
			JCBNODE *q = allJCB.head;
			//�洢��ȫ������
			tempName = q->p.name;
			//���뵽��������ҵ��ĩβ
			if (inJCB.length > 0) {
				//inJCB���һ�����
				for (int i = 0; i < inJCB.length - 1; i++)
					p = p->next;
				p->next = q;
			}
			else{
				inJCB.head = q;
			}
            //ָ�봦��������ҵ��1��������ҵ��1
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
�ж���ҵ�Ƿ���Ե����ڴ�����
*/
bool isJoinPCB() {
	if (FF()) {
		return true;
	} else {
		return false;
	}
}
/*
�״���Ӧ�㷨
*/
bool FF() {
	BLOCK * b = sBList.head;//�����ڴ����
	BLOCK * p = rBList.head;//����ǰ�ýڵ�
	BLOCK * pp = (BLOCK*)malloc(sizeof(BLOCK));//���ڴ�ڵ�

	//ʣ���ڴ����ҵ�ڴ�С������װ��
	if (sBList.ram < inJCB.head->p.ram) {
        free(pp);
		return false;
	} else {
    //��װ��
		while (b != NULL) {
			if (b->ram >= inJCB.head->p.ram) {
				pp->name = inJCB.head->p.name;
				pp->ram = inJCB.head->p.ram;
				pp->pre = b->pre;
				pp->next = b->pre + inJCB.head->p.ram;

				//����õ����ڴ���뵽���ж���ĩβ
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
				//������ҵ��������
				rBList.ram++;

				//����ڵ�ʣ���
				p = sBList.head;
				b->pre = b->pre + pp->ram;
				b->ram = b->ram - pp->ram;
				//����ʣ��
				sBList.ram -= pp->ram;

				//0Ϊ�ջ�1��Ƭ��Ҫ�ˣ��ÿ�
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
				return true;//��ҵ�����ڴ�
			} else {
				b = b->link;
			}
		}
	}
	free(pp);
	return false;//��ҵװ�ص��ڴ���ʧ�ܡ�
}

/*
����ҵ�������̣����̼��뵽 ���̾������У�wp���С�
*/
bool JCBtoPCB() {
	PCBNODE* p = NULL;
	PCBNODE* q = wp.head;

	p = (PCBNODE*)malloc(sizeof(PCBNODE));
	p->p.name = inJCB.head->p.name;
	p->p.ram = inJCB.head->p.ram;
	p->p.servicetime = inJCB.head->p.servicetime;
	p->p.state = 'W';
	//ʱ��Ƭ
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
��inJCB��runJCB�У���ҵ״̬�仯��
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
����״̬�� w �� r������״̬ת��������λ�ñ任��
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

//ʱ��Ƭ��ת�㷨
void RoundRobin() {
    //��ҵ����
	rp.head->p.servicetime--;
	rp.head->p.roundtimeless--;

	if (rp.head->p.servicetime <= 0) {
    //������ҵ���
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ����%c ���н��� *******************************\n", rp.head->p.name);
		visitPCBLinker(rp);
		//���� ���� -> ���
		PCBr2f();
		printf("******************************* ���� ���� ���� *******************************\n");
		visitPCBLinker(fp);
        //��ҵ ���� -> ���
		JCBrun2out();
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ��ҵ%c ������� *******************************\n", outJCB.head->p.name);
        printf("��ҵ��������\n");
		visitJCBLinker(outJCB);

        //�ͷ���ҵ�������ڴ�
		freeBlock();
		NAME = 0;
		printf("\n******************************* ϵͳ �ڴ� ״̬ *******************************\n");
		printf("\n�����ڴ�����\n");
		visitedBlock(sBList);
		printf("\n�����ڴ�����\n");
		visitedBlock(rBList);
		printf("\n******************************************************************************\n");

		getchar();
		return;
	} else if (rp.head->p.roundtimeless <= 0) {
    //ʱ��Ƭ����
		rp.head->p.roundtimeless = ROUNDTIME;
		printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ʱ��Ƭ���꣬����%c �޷���������ʼʱ��Ƭ��ת�㷨��\n", rp.head->p.name);
		//���� ���� -> ����
		PCBr2w();
		//���� ���� -> ����
		PCBw2r();
		printf("\n******************************* ���� ���� ���� *******************************\n");
		visitPCBLinker(rp);
		printf("\n******************************* ���� ���� ���� *******************************\n");
		visitPCBLinker(wp);

		getchar();
		return;
	}

	//���н����л�
	PCBr2r();
	return;
}

/*
�ж� ����״̬�� r �� f������״̬ת��������λ�ñ任��
*/
bool PCBr2f() {
	PCBNODE* q = fp.head;
	rp.head->p.state = 'F';
	//��ɽ��̵�����
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
	//���н��̶���ȥͷ
	rp.head = rp.head->next;
	q->next = NULL;
	rp.length--;
	fp.length++;

	return true;
}

/*
��������ɵ���ҵ���뵽��ҵ��ɶ��е��У��������ң�
*/
bool JCBrun2out() {
	JCBNODE*p = runJCB.head;//���ָ����ɵ���ҵ
	JCBNODE*pp = runJCB.head;//�����ҵ��ǰһ��
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
�ͷ���ҵ�������ڴ棨������
*/
bool freeBlock() {
	BLOCK *p1 = rBList.head;//��ҵռ�õ��ڴ�ָ��
	BLOCK *p2 = rBList.head;//ǰ��ָ��
	BLOCK *p3 = sBList.head;

	//�����ڴ���г��p1
	if (rBList.ram == 1) {
		rBList.head = NULL;
		rBList.ram--;
	} else {
		while ((p1->name!=NAME))
			p1 = p1->link;
		printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> �ͷ���ҵ%c ���ڴ��ָ�룺\nǰ��%d ,�� %d��\n",p1->name, p1->pre, p1->next);
		while (p2 != p1 && p2->link != p1)
			p2 = p2->link;
		if (p2 == p1) { //��ͷ�ڵ�
			rBList.head = p1->link;
		} else {
			p2->link = p1->link;
		}
		rBList.ram--;
		p1->link = NULL;
		p1->name = 0;
	}
    //p1��������ڴ����
	p2 = sBList.head;//p1��ַ֮��
	p3 = sBList.head;//p2ǰһ��ָ�룬��p1��ַ֮ǰ
	if (sBList.ram == 0) {
		sBList.head = p1;
		sBList.ram += p1->ram;
	} else {
		while (NULL != p2 && p2->pre < p1->next)
			p2 = p2->link;
		while (NULL != p3 && p3 != p2 && p3->link != p2) {
			p3 = p3->link;
		}

		//����or�ϲ�
        //һ��p2��ͷ��p1ֻ�к�
		if (p2 == sBList.head) {
			if (p2->pre <= (p1->next + 1)) {    //�ϲ���ǰ�ĺ�+1 == ���ǰ+1 ���ߴ�1
				p2->pre -= p1->ram;
				p2->ram += p1->ram;
				sBList.ram += p1->ram;
			} else {    //�嵽ǰ��
				p1->link = p2;
				sBList.head = p1;
				sBList.ram += p1->ram;
			}
		} else if (NULL == p2) {
        //�������ж��ж���p1��ַ֮ǰ��p1ֻ��ǰ
			if (p3->next >= p1->pre - 1) {  //�ϲ�����β�ĺ� == �����ǰ-1 ���ߴ�1
				p3->next += p1->ram;
				p3->ram += p1->ram;
				sBList.ram += p1->ram;
			} else { //���룬�����β����
				p3->link = p1;
				sBList.ram += p1->ram;
			}
		} else {
        //�����м䣬p1��ǰ�к�
			if ((p3->next >= p1->pre-1 ) && (p2->pre <= p1->next+1 )) { //�ϲ�ǰ��
				sBList.ram -= p3->ram;
				p3->ram = p1->ram + p2->ram + p3->ram;
				p3->next = p3->pre + p3->ram;
				p3->link = p2->link;
				sBList.ram += p3->ram;
			} else if (p2->pre <= p1->next+1 ) {    //ֻ�ϲ���
				p2->pre -= p1->ram;
				p2->ram += p1->ram;
				sBList.ram += p1->ram;
			} else if (p3->next >= p1->pre-1 ) {    //ֻ�ϲ�ǰ
				p3->next += p1->ram;
				p3->ram += p1->ram;
				sBList.ram += p1->ram;
			} else {    //���ϲ�������
				p3->link = p1;
				p1->link = p2;
				sBList.ram += p1->ram;
			}
		}
	}
	return true;
}

/*
����״̬�� r �� w������״̬ת��������λ�ñ任��
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
	//���н��̶���ȥͷ
	rp.head = rp.head->next;
	q->next = NULL;
	rp.length--;
	wp.length++;
	return true;
}

/*
�����л������н��̷ŵ�ĩβ
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
	int a = 0;//�����ַ�
    int i = 0;

    printf("------------------------------------------------------------------------------\n");
    printf("******************************************************************************\n");
    printf("***************************** ��ӭ����  ����ϵͳ *****************************\n");
    printf("******************************************************************************\n");
    printf("------------------------------------------------------------------------------\n\n");

    printf("------------------------------------------------------------------------------\n");
    //��ʼ�����򣬲��������ҵ����
	init();
	printf("------------------------------------------------------------------------------\n");

	printf("\n\n\n-------------------------------- ϵͳ��ʼ���� --------------------------------\n");
    //ϵͳ���У����� ��������ҵ or �о�����ҵ or �еȴ����� or �����н��̣�
	while (true==isRun()) {
        //ϵͳʱ���¼��0~n
		runtime++;
		printf("\n---------------------------- ��ǰϵͳʱ��Ϊ��%4d ----------------------------\n",runtime);

        //����ҵ���������ҵ��������У�Ͷ�������ҵ���к����
		if (true==appearJCB()) {
			printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ��ҵ%c ���\n",tempName);
			printf("\n******************************* ���� ��ҵ ���� *******************************\n");
			visitJCBLinker(inJCB);
		}
		//�Ѿ�����ҵ������̣���Ϊ����״̬
		if (inJCB.length > 0) {
			if (true==isJoinPCB()) {
				printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> �ڴ��㹻����ҵ%c ����װ�����ڴ��С�\n",tempName);
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ������ҵ%c �ύ���С�\n",tempName);
				tempName = ' ';
				//����ҵ�������̣����̼��뵽 ���̾������У�wp���С�
				JCBtoPCB();
				printf("\n******************************* ���� ���� ���� *******************************\n");
				visitPCBLinker(wp);

                //��inJCB��runJCB�У���ҵ״̬�仯��
				JCBin2run();
				printf("\n******************************* ���� ��ҵ ���� *******************************\n");
				visitJCBLinker(inJCB);
				printf("\n******************************* ���� ��ҵ ���� *******************************\n");
				visitJCBLinker(runJCB);

				printf("\n\n******************************* ϵͳ �ڴ� ״̬ *******************************\n");
				printf("\n�����ڴ���:\n");
				visitedBlock(sBList);
				printf("\n��ҵ�ڴ���:\n");
				visitedBlock(rBList);
				printf("\n******************************************************************************\n");

				a = 0; getchar();
			} else {
				printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ʣ���ڴ�ռ��޷�װ����ҵ��\n");
				visitedBlock(sBList);
			}
		}

        //�н�����Ҫ���У���������or���н��̣�
        if (wp.length > 0 || rp.length > 0) {
			if (wp.length >0 && rp.length < RPMAXLENGTH) {
				printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ���������ύ�����н��̡�\n");
				//����״̬�� w �� r������״̬ת��������λ�ñ任��
				for (i = 0; i < RPMAXLENGTH; i++) {
                    PCBw2r();
				}
				printf("\n******************************* ���� ���� ���� *******************************\n");
				visitPCBLinker(wp);
				printf("\n******************************* ���� ���� ���� *******************************\n");
				visitPCBLinker(rp);

				a = 0; getchar();
			}
			//��������
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ��������\n");
			int length = rp.length;
			//ʱ��Ƭ��ת�㷨
			for (i = 0; i < length; i++) {
                RoundRobin();
			}
			printf("\n******************************* ���� ���� ���� *******************************\n");
            visitPCBLinker(rp);
		}

	}

	printf("\n\n----------------------------- ������ʱ��Ϊ��%d! -----------------------------\n", runtime);
	printf("---------------------------- ���н���,����'0'�˳� ----------------------------\n");

	//���롰0����������
	a = getchar();
	while (a != '0')
		a = getchar();
}




