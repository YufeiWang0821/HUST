//此文件为头文件，声明各个函数和定义
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<ctype.h>

#define status int
#define MaxLitNum 4000
#define OK 1
#define NOPE 0
#define TRUE 1
#define FALSE 0
#define pos 1
#define neg -1

typedef struct LNode {//文字的结点结构
	int sta;//文字的编号
	struct LNode *next;
}LNode,*lit;
typedef struct CNode {//子句的结构
	struct CNode *next;
	struct LNode *first_l;//指向子句中的第一个文字
}CNode,*cla;
typedef struct CNFG {//合取范式CNF结构
	struct CNode *root;//第一个子句
	int c_num, l_num;//子句和文字的数量
}*cnf;
typedef struct Ans {//SAT问题求解的答案
	int time, state, num;//DPLL函数用时，SAT问题的可符合性，文字的个数
	int LiteralState[MaxLitNum+1];//可符合时各个文字的正负状态
};

//以下是对一些数据的声明
extern Ans ans;
extern int digcount;

//以下是对于SAT的函数声明
cnf OperateCNF(FILE* fp,cnf S);
status AnswerFile(char* filename);
status DPLL(cnf S,int flag);//DPLL算法函数
void TraverseCNF(cnf S);
status FreeCNF(cnf S);

//以下是对于数独的函数声明
void PrintSudoku(int a[][9]);
void CreateSudoku(int a[][9]);
status DigHole(int a[][9], int n);
status JudgeNumber(int a[][9], int row, int col, int n);