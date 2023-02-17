//CNF.cpp：此文件是对于合取范式CNF有关操作的函数的定义
#include"def.h"
Ans ans;
int GetNumber(FILE* fp) {//获取CNF文件中的数字
    char c;
    int flag = 1, num = 0;
    fscanf(fp, "%c", &c);
    if (c == '-') {
        flag = -1;
        fscanf(fp, "%c", &c);
    }
    else if (c == '0') {
        fscanf(fp, "%c", &c);
        if (c == '\r') { fscanf(fp, "%c", &c); }
        return num;
    }
    else if (feof(fp)) { return 0; }
    while (c != ' ' && c != '\n' && c != '\r') {
        num = num * 10 + c - '0';
        fscanf(fp, "%c", &c);
    }
    if (c == '\r') { fscanf(fp, "%c", &c); }
    return flag * num;
}

status reset(cnf S) {//重置S
    S->c_num = 0; S->l_num = 0;
    cla ctemp,cnext;
    lit ltemp,lnext;
    ctemp = S->root;
    while (ctemp) {
        cnext = ctemp->next;
        ltemp = ctemp->first_l;
        while (ltemp) {
            lnext = ltemp->next;
            free(ltemp);
            ltemp = lnext;
        }
        free(ctemp);
        ctemp = cnext;
    }
    S->root = NULL;
    memset(ans.LiteralState, 0, MaxLitNum + 1);
    return OK;
}

cnf OperateCNF(FILE* fp,cnf S) {//读取CNF文件，创建CNF结构
    char c;
    int temp,flag;
    cla clap,clapre;
    lit litp,litnext;
    reset(S);
    fscanf(fp, "%c", &c);
    while (c == 'c') {
        while (c == 'c') {
            while (c != '\n' && c != '\r') { fscanf(fp, "%c", &c); }
            fscanf(fp, "%c", &c);
            if (c == '\n'){ fscanf(fp, "%c", &c); }
        }
    }
    fscanf(fp, " cnf ");
    S->l_num = GetNumber(fp), S->c_num = GetNumber(fp);
    ans.num = S->l_num;
    while (1) {
        temp = GetNumber(fp);
        if (feof(fp)) { break; }
        clap = (cla)malloc(sizeof(CNode));
        clap->next = NULL;
        if (!S->root) { S->root = clap; }
        else { clapre->next = clap; }
        if (temp) {
            litp = (lit)malloc(sizeof(LNode));
            clap->first_l = litp;
            litp->sta = temp;
            litp->next = NULL;
            temp = GetNumber(fp);
            while (temp) {
                litnext = (lit)malloc(sizeof(LNode));
                litnext->sta = temp;
                litnext->next = NULL;
                litp->next = litnext;
                litp = litnext;
                temp = GetNumber(fp);
            }
        }
        else {
            clap->first_l = NULL;
        }
        clapre = clap;
    }
    return S;
}

status AnswerFile(char *filename) {//生成SAT问题的答案文件
    FILE* fp;
    int p = 0;
    while (filename[p] != 0) { p++; }
    while (filename[p] != '.') { p--; }
    filename[++p] = 'r';
    filename[++p] = 'e';
    filename[++p] = 's';
    filename[++p] = 0;
    fp = fopen(filename, "w");
    if(ans.state){ fprintf(fp, "s 1\r\n"); }
    else{ fprintf(fp, "s 0\r\n"); }
    fprintf(fp, "v ");
    for (int i = 1; i <= ans.num; i++) {
        if (ans.LiteralState[i]) { fprintf(fp, "%5d ", i * ans.LiteralState[i]); }
        else { fprintf(fp, "%5d ", i); }
    }
    fprintf(fp, "\r\n");
    fprintf(fp, "t %d\r\n", ans.time);
    fclose(fp);
    printf("SAT问题答案已保存至相应文件中\n");
    return OK;
}

void TraverseCNF(cnf S) {//遍历生成的CNF结构并输出
    printf("\n\n");
    cla c = S->root;
    lit l;
    while (c) {
        l = c->first_l;
        while (l) {
            printf("%5d ", l->sta);
            l = l->next;
        }
        printf("0\n");
        c = c->next;
    }
}