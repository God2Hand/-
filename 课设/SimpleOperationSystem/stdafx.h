#ifndef STDAFX_H_INCLUDED
#define STDAFX_H_INCLUDED

//头文件包含
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//结构体命名定义
typedef struct BLOCK BLOCK;
typedef struct BList BList;
typedef struct JCB JCB;
typedef struct PCB PCB;
typedef struct JCBNODE JCBNODE;
typedef struct PCBNODE PCBNODE;
typedef struct JCBLINKER JCBLINKER;
typedef struct PCBLINKER PCBLINKER;

//函数声明
void init();
void visitJCBLinker(JCBLINKER jl);
void visitPCBLinker(PCBLINKER pl);
void visitedBlock(BList list);
bool isRun();
bool isJoinPCB();
bool appearJCB();
bool FF();
bool JCBrun();
bool JCBin2run();
bool JCBrun2out();
bool JCBtoPCB();
bool PCBw2r();
bool PCBr2w();
bool PCBr2f();
void RoundRobin();
bool freeBlock();
bool PCBr2r();

int main1();
int main2();

#endif // STDAFX_H_INCLUDED
