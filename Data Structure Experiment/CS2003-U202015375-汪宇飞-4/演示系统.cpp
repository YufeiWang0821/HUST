#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20
typedef int status;
typedef int KeyType;
typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef struct {
    KeyType  key;
    char others[20];
} VertexType; //顶点类型定义
typedef struct ArcNode {         //表结点类型定义
    int adjvex;              //顶点位置编号 
    struct ArcNode* nextarc;       //下一个表结点指针
} ArcNode;
typedef struct VNode {                //头结点及其数组类型定义
    VertexType data;           //顶点信息
    ArcNode* firstarc;           //指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];
typedef  struct {  //邻接表的类型定义
    AdjList vertices;          //头结点数组
    int vexnum, arcnum;         //顶点数、弧数
    GraphKind  kind;        //图的类型
} ALGraph;
typedef struct {  //图的集合类型定义
    struct {
        char name[30];
        ALGraph G;
    } elem[10];
    int length;
}Graphs;

VertexType V[30];
KeyType VR[100][2];
int op = 1, keyword[100][2], len = 0,visited[20];;//keyword[i][0]为第i个头结点的编号，keyword[i][1]是第i个头结点的关键字，visited[20]为深度搜索遍历中标志
ALGraph G; FILE* fp; char filename[30]; Graphs Gras;

void visit(VertexType v){
    printf(" %d %s", v.key, v.others);
}
status CreateGraph(ALGraph& G)
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    len = 0;
    for (int i = 0; V[i].key != -1; i++) {
        for (int j = 0; j <= len; j++) {
            if (V[i].key == keyword[j][1]) { return ERROR; }
        }
        keyword[len][0] = i;
        keyword[len++][1] = V[i].key;
        G.vertices[i].data = V[i];
        G.vertices[i].firstarc = NULL;
    }
    G.vexnum = len; G.arcnum = 0;
    if (G.vexnum > 20 || !G.vexnum) { return ERROR; }
    int stai, endi, stakey, endkey, staflag = 0, endflag = 0;
    ArcNode* stap, * endp, * L, * nex;
    for (int i = 0; VR[i][0] != -1; i++) {
        staflag = 0, endflag = 0;
        G.arcnum++;
        stakey = VR[i][0], endkey = VR[i][1];
        for (int j = 0; j < len; j++) {
            if (stakey == keyword[j][1]) {
                stai = keyword[j][0]; staflag = 1;
            }
            if (endkey == keyword[j][1]) {
                endi = keyword[j][0]; endflag = 1;
            }
        }
        if (!(staflag && endflag)) { return ERROR; }
        stap = (ArcNode*)malloc(sizeof(ArcNode)), endp = (ArcNode*)malloc(sizeof(ArcNode));
        stap->adjvex = endi, endp->adjvex = stai;
        if (!G.vertices[stai].firstarc) {
            G.vertices[stai].firstarc = stap; stap->nextarc = NULL;
        }
        else {
            stap->nextarc = G.vertices[stai].firstarc; G.vertices[stai].firstarc = stap;
        }
        if (!G.vertices[endi].firstarc) {
            G.vertices[endi].firstarc = endp; endp->nextarc = NULL;
        }
        else {
            endp->nextarc = G.vertices[endi].firstarc; G.vertices[endi].firstarc = endp;
        }
    }
    return OK;
}
status DestroyGraph(ALGraph& G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    ArcNode* L, * next;
    for (int i = 0; i < G.vexnum; i++) {
        L = G.vertices[i].firstarc;
        if (!L) { continue; }
        while (L) {
            next = L->nextarc;
            free(L);
            L = next;
        }
        G.vertices[i].firstarc = NULL;
    }
    G.arcnum = 0, G.vexnum = 0;
    return OK;
    /********** End **********/
}
int LocateVex(ALGraph G, KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int flag = -1;
    for (int i = 0; i < G.vexnum; i++) {
        if (u == G.vertices[i].data.key) { flag = i; break; }
    }
    return flag;
    /********** End **********/
}
status PutVex(ALGraph& G, KeyType u, VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data.key == value.key && G.vertices[i].data.key != u) { return ERROR; }
    }
    for (int i = 0; i < G.vexnum; i++) {
        if (u == G.vertices[i].data.key) {
            G.vertices[i].data = value;
            return OK;
        }
    }
    return ERROR;
    /********** End **********/
}
int FirstAdjVex(ALGraph G, KeyType u)
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data.key == u) {
            return G.vertices[i].firstarc->adjvex;
        }
    }
    return -1;
    /********** End **********/
}
int NextAdjVex(ALGraph G, KeyType v, KeyType w)
//根据u在图G中查找顶点，查找成功返回顶点v的邻接顶点相对于w的下一邻接顶点的位序，查找失败返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int wi, ansi,flag=0;
    for (int i = 0; i < G.vexnum; i++) {
        if (w == G.vertices[i].data.key) {
            wi = i;
            flag = 1;
            break;
        }
    }
    if (!flag) { return -1; }
    for (int i = 0; i < G.vexnum; i++) {
        if (v == G.vertices[i].data.key) {
            ArcNode* p = G.vertices[i].firstarc;
            while (p) {
                if (p->adjvex == wi) {
                    if (p->nextarc) { return p->nextarc->adjvex; }
                    else { return -1; }
                }
                else { p = p->nextarc; }
            }
        }
    }
    return -1;
    /********** End **********/
}
status InsertVex(ALGraph& G, VertexType v)
//在图G中插入顶点v，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (G.vexnum == 20) { return ERROR; }
    for (int i = 0; i < G.vexnum; i++) {
        if (v.key == G.vertices[i].data.key) { return ERROR; }
    }
    G.vertices[G.vexnum].data = v;
    G.vertices[G.vexnum++].firstarc = NULL;
    return OK;

    /********** End **********/
}
status DeleteVex(ALGraph& G, KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (G.vexnum == 1) { return ERROR; }
    int flagi = -1;
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data.key == v) { flagi = i; }
    }
    if (flagi == -1) { return ERROR; }
    ArcNode* p = G.vertices[flagi].firstarc, * next;
    while (p) {//待删除顶点的弧free并将弧树减少
        next = p->nextarc;
        free(p);
        p = next;
        G.arcnum--;
    }
    for (int i = flagi + 1; i <= G.vexnum; i++) {
        G.vertices[i - 1] = G.vertices[i];
    }
    G.vexnum--;
    for (int i = 0; i < G.vexnum; i++) {//修改其他顶点的弧
        p = next = G.vertices[i].firstarc;
        if (p && p->adjvex == flagi) {
            next = p->nextarc;
            free(p);
            G.vertices[i].firstarc = next;
        }
        p = next;
        while (p) {
            if (p->adjvex > flagi) { p->adjvex--; }
            next = p->nextarc;
            if (next && next->adjvex == flagi) {
                p->nextarc = next->nextarc;
                free(next);
                next = p->nextarc;
            }
            p = p->nextarc;
        }
    }
    return OK;
    /********** End **********/
}
status InsertArc(ALGraph& G, KeyType v, KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int flag = 0, vi, wi;
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data.key == v) { vi = i; flag++; }
        if (G.vertices[i].data.key == w) { wi = i; flag++; }
    }
    if (flag != 2) { return ERROR; }
    ArcNode* p = G.vertices[vi].firstarc;
    while (p) {
        if (p->adjvex == wi) { return ERROR; }
        p = p->nextarc;
    }
    ArcNode* vnode = (ArcNode*)malloc(sizeof(ArcNode));
    ArcNode* wnode = (ArcNode*)malloc(sizeof(ArcNode));
    vnode->adjvex = wi, wnode->adjvex = vi;
    vnode->nextarc = G.vertices[vi].firstarc; G.vertices[vi].firstarc = vnode;
    wnode->nextarc = G.vertices[wi].firstarc; G.vertices[wi].firstarc = wnode;
    G.arcnum++;
    return OK;
    /********** End **********/
}
status DeleteArc(ALGraph& G, KeyType v, KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int flagi = 0, vi, wi, flag = 0;
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data.key == v) { vi = i; flagi++; }
        if (G.vertices[i].data.key == w) { wi = i; flagi++; }
    }
    if (flagi != 2) { return ERROR; }
    ArcNode* p = G.vertices[vi].firstarc, * next;
    if (!p) { return ERROR; }
    if (p->adjvex == wi) {
        flag++;
        G.vertices[vi].firstarc = p->nextarc;
        free(p);
    }
    else {
        while (p->nextarc) {
            if (p->nextarc->adjvex == wi) {
                flag++;
                next = p->nextarc;
                p->nextarc = next->nextarc;
                free(next);
                break;
            }
            p = p->nextarc;
        }
    }
    p = G.vertices[wi].firstarc;
    if (!p) { return ERROR; }
    if (p->adjvex == vi) {
        flag++;
        G.vertices[wi].firstarc = p->nextarc;
        free(p);
    }
    else {
        while (p->nextarc) {
            if (p->nextarc->adjvex == vi) {
                flag++;
                next = p->nextarc;
                p->nextarc = next->nextarc;
                free(next);
                break;
            }
            p = p->nextarc;
        }
    }
    if (flag == 2) { G.arcnum--; return OK; }
    else { return ERROR; }
    /********** End **********/
}
void DFS(ALGraph& G, int v, void (*visit)(VertexType))
{
    ArcNode* p;//图的顶点的搜索指针
    visited[v] = 1;//置已经访问
    visit(G.vertices[v].data);//函数visited()代表了一类访问顶点v的操作
    p = G.vertices[v].firstarc;//p指向顶点v的第一条边
    while (p != NULL) {
        if (visited[p->adjvex] == 0)//若顶点未访问，则递归访问它
            DFS(G, p->adjvex, visit);
        p = p->nextarc;//p指向顶点v的下一条边的终点
    }
}
status DFSTraverse(ALGraph& G, void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    memset(visited, 0, 20);
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) { DFS(G, i, visit); }
    }
    return OK;
    /********** End **********/
}
status BFSTraverse(ALGraph& G, void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    memset(visited, 0, 20);
    ArcNode* p;
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            visit(G.vertices[i].data);
            visited[i] = 1;
            p = G.vertices[i].firstarc;
            while (p) {
                if (!visited[p->adjvex]) {
                    visit(G.vertices[p->adjvex].data);
                    visited[p->adjvex] = 1;
                }

                p = p->nextarc;
            }
        }
    }
    return OK;
    /********** End **********/
}
status SaveGraph(ALGraph G, char FileName[])
//将图的数据写入到文件FileName中
{
    // 请在这里补充代码，完成本关任务
    /********** Begin 1 *********/
    FILE* fp = fopen(FileName, "w");
    if (!fp) { return ERROR; }
    for (int i = 0; i < G.vexnum; i++) {
        fprintf(fp, "%d %s ", G.vertices[i].data.key, G.vertices[i].data.others);
    }
    fprintf(fp, "%d %c ", -1, 'n');
    ArcNode* p;
    for (int i = 0; i < G.vexnum; i++) {
        p = G.vertices[i].firstarc;
        fprintf(fp, "%c", '\n');
        while (p) {
            fprintf(fp, "%d ", p->adjvex);
            p = p->nextarc;
        }
        fprintf(fp, "%d", -1);
    }
    fclose(fp);
    return OK;
    /********** End 1 **********/
}
status LoadGraph(ALGraph& G, char FileName[])
//读入文件FileName的图数据，创建图的邻接表
{
    // 请在这里补充代码，完成本关任务
    /********** Begin 2 *********/
    FILE* fp = fopen(FileName, "r");
    if (!fp) { return ERROR; }
    int c; char s[20]; int i = 0;
    int exi[20][20] = { 0 };
    G.vexnum = 0, G.arcnum = 0;
    while (fscanf(fp, "%d %s", &c, s) != -1) {
        if (c == -1) { break; }
        G.vertices[i].data.key = c;
        strcpy(G.vertices[i].data.others, s);
        G.vertices[i++].firstarc = NULL;
        G.vexnum++;
    }
    for (i = 0; i < G.vexnum; i++) {
        ArcNode* p, * q;
        fscanf(fp, "%d", &c);//对第一个结点
        if (c == -1) { continue; }
        q = (ArcNode*)malloc(sizeof(ArcNode));
        q->adjvex = c, q->nextarc = NULL;
        G.vertices[i].firstarc = q;
        if (!exi[c][i]) { exi[i][c] = 1; }
        p = G.vertices[i].firstarc;
        while (fscanf(fp, "%d", &c)) {//对后续结点
            if (c == -1) { break; }
            q = (ArcNode*)malloc(sizeof(ArcNode));
            q->adjvex = c, q->nextarc = NULL;
            p->nextarc = q;
            p = p->nextarc;
            if (!exi[c][i]) { exi[i][c] = 1; }
        }
    }
    for (i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcnum += exi[i][j];
        }
    }
    return OK;
    /********** End 2 **********/
}
status AddGraph(Graphs& Gras, char GraName[]) {
//Gras中增加一个名称为GraName的图
    strcpy(Gras.elem[Gras.length].name, GraName);
    int flagAG = CreateGraph(Gras.elem[Gras.length].G);
    Gras.length++;
    return flagAG;
}
status RemoveGraph(Graphs& Gras, char GraName[])
// Gras中删除一个名称为GraName的图
{
    for (int i = 0; i < Gras.length; i++) {
        if (!strcmp(Gras.elem[i].name, GraName)) {
            for (int j = i; j < Gras.length; j++) { Gras.elem[j] = Gras.elem[j + 1]; }
            Gras.length--;
            return OK;
        }
    }
    return ERROR;
}
int LocateGraph(Graphs& Gras, char GraName[])
// 在Gras中查找一个名称为GraName的图，成功返回逻辑序号，否则返回0
{
    for (int i = 0; i < Gras.length; i++) {
        if (!strcmp(Gras.elem[i].name, GraName)) {
            return i + 1;
        }
    }
    return 0;
}
void main(int argc, char* argv[]) {
	while (op) {
		system("cls");	printf("\n\n");
		printf("                                   菜单                      \n");
		printf("-----------------------------------------------------------------------\n");
		printf("                            对于单个图的操作\n");
		printf("    	  1. 创建图            2. 销毁图              3. 查找顶点\n");
		printf("    	  4. 顶点赋值          5. 获得第一邻接点      6.获得下一邻接点\n");
		printf("    	  7. 插入顶点          8. 删除顶点            9. 插入弧\n");
		printf("    	  10.删除弧            11.深度优先搜索遍历    12.广度度优先搜索遍历\n");
		printf("    	  13.图写入文件        14.图读取文件\n");
        printf("-----------------------------------------------------------------------\n");
		printf("                            对于图集合的操作\n");
		printf("    	  15.增加一个图        16.删除一个图          17.查找图\n");
        printf("    	  18.查找顶点          19.顶点赋值            20.获得第一邻接点\n");
        printf("    	  21.获得下一邻接点    22.插入顶点            23.删除顶点\n");
        printf("    	  24.插入弧            25.删除弧              26.深度优先搜索遍历\n");
        printf("    	  27.广度优先搜索遍历  28.图写入文件          29.图读取文件\n");
		printf("    	  0. 退出\n");
		printf("    请选择你的操作[0~29]:");
		scanf("%d", &op);
		switch (op) {
			//以下为对单个图的操作
        case 1:
            printf("请输入图的数据，如：5 线性表 8 集合 7 二叉树 6 无向图 -1 nil  5 6  5 7 6 7 7 8 -1 -1\n");
            int i1,flag1;
            i1 = 0; 
            memset(V, 0, sizeof(V));
            memset(VR, 0, sizeof(VR));
            memset(keyword, 0, sizeof(keyword));
            do {
                scanf("%d%s", &V[i1].key, V[i1].others);
            } while (V[i1++].key != -1);
            i1 = 0;
            do {
                scanf("%d%d", &VR[i1][0], &VR[i1][1]);
            } while (VR[i1++][0] != -1);
            if (G.vexnum) { printf("图已存在\n"); }
            else {
                flag1 = CreateGraph(G);
                if (flag1 == ERROR) { printf("输入数据错误，无法创建\n"); }
                else { printf("图已创建成功\n"); }
            }
            getchar(); getchar(); break;
        case 2:
            if (!G.vexnum) { printf("图不存在\n"); }
            else if (DestroyGraph(G)==OK) { printf("图已销毁成功\n"); }
            getchar(); getchar(); break;
        case 3:
            int i3,flag3;
            printf("请输入要查找的顶点关键字:\n");
            scanf("%d", &i3);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                flag3 = LocateVex(G, i3);
                if (flag3 == -1) { printf("未找到该顶点\n"); }
                else{ printf("该顶点为第%d个顶点", flag3); }
            }
            getchar(); getchar(); break;
        case 4:
            int i4; VertexType v4;
            printf("请输入要赋值的顶点:\n");
            scanf("%d", &i4);
            printf("请输入要赋的值:\n");
            scanf("%d%s", &v4.key, v4.others);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (PutVex(G, i4, v4) == ERROR) { printf("赋值操作失败\n"); }
                else { printf("赋值成功\n"); }
            }
            getchar(); getchar(); break;
        case 5:
            int i5, flag5;
            printf("请输入要获得第一邻接点的顶点关键字:\n");
            scanf("%d", &i5); 
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                flag5 = FirstAdjVex(G, i5);
                if (flag5 == -1) { printf("查找失败\n"); }
                else { printf("第一邻接点为%d %s", G.vertices[flag5].data.key, G.vertices[flag5].data.others); }
            }
            getchar(); getchar(); break;
        case 6:
            int i61, i62, flag6;
            printf("请输入获取的起始顶点的关键字和要获取下一邻接点的顶点的关键字:\n");
            scanf("%d%d", &i61, &i62);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                flag6 = NextAdjVex(G, i61, i62);
                if (flag6 == -1) { printf("查找失败\n"); }
                else { printf("下一邻接点为%d %s", G.vertices[flag6].data.key, G.vertices[flag6].data.others); }
            }
            getchar(); getchar(); break;
        case 7:
            VertexType v7;
            printf("请输入插入顶点的值:\n");
            scanf("%d%s", &v7.key, v7.others);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (InsertVex(G, v7) == ERROR) { printf("插入失败\n"); }
                else { printf("插入成功\n"); }
            }
            getchar(); getchar(); break;
        case 8:
            int i8;
            printf("请输入要删除顶点的关键字:\n");
            scanf("%d", &i8);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (DeleteVex(G, i8) == ERROR) { printf("删除失败\n"); }
                else { printf("删除成功\n"); }
            }
            getchar(); getchar(); break;
        case 9:
            int i91, i92;
            printf("请输入要插入弧的两个顶点:\n");
            scanf("%d%d", &i91, &i92);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (InsertArc(G, i91, i92) == ERROR) { printf("插入失败\n"); }
                else { printf("插入成功\n"); }
            }
            getchar(); getchar(); break;
        case 10:
            int i101, i102;
            printf("请输入要删除弧的两个顶点:\n");
            scanf("%d%d", &i101, &i102);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (DeleteArc(G, i101, i102) == ERROR) { printf("删除失败\n"); }
                else { printf("删除成功\n"); }
            }
            getchar(); getchar(); break;
        case 11:
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                printf("深度优先搜索遍历如下:\n");
                DFSTraverse(G, visit);
            }
            getchar(); getchar(); break;
        case 12:
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                printf("广度优先搜索遍历如下:\n");
                BFSTraverse(G, visit);
            }
            getchar(); getchar(); break;
        case 13:
            printf("请输入文件名称："); scanf("%s", filename);
            if (!G.vexnum) { printf("图不存在\n"); }
            else {
                if (SaveGraph(G, filename) == ERROR) { printf("文件写入失败\n"); }
                else { printf("图已写入文件\n"); }
            }
            getchar(); getchar(); break;
        case 14:
            printf("请输入文件名称："); scanf("%s", filename);
            if (G.vexnum) { printf("图已存在\n"); }
            else {
                if (LoadGraph(G, filename) == ERROR) { printf("文件读取失败\n"); }
                else { printf("图已读取文件\n"); }
            }
            getchar(); getchar(); break;
//以下为对图集合的操作
        case 15:
            char newgraname[30];
            int i15 , flag15;
            i15 = 0;
            memset(V, 0, sizeof(V));
            memset(VR, 0, sizeof(VR));
            memset(keyword, 0, sizeof(keyword));
            printf("请输入新图的名称：");
            scanf("%s", newgraname);
            printf("请输入图的数据，如：5 线性表 8 集合 7 二叉树 6 无向图 -1 nil  5 6  5 7 6 7 7 8 -1 -1\n");
            do {
                scanf("%d%s", &V[i15].key, V[i15].others);
            } while (V[i15++].key != -1);
            i15 = 0;
            do {
                scanf("%d%d", &VR[i15][0], &VR[i15][1]);
            } while (VR[i15++][0] != -1);
            flag15 = AddGraph(Gras, newgraname);
            if (flag15 == ERROR) { printf("输入数据错误，无法创建\n"); }
            else { printf("图已创建成功\n"); }
            getchar(); getchar(); break;
        case 16:
            char dename[30];
            printf("请输入需要删除的图的名称：");
            scanf("%s", dename);
            if (RemoveGraph(Gras,dename) == ERROR) { printf("删除失败！\n"); }
            else { printf("删除成功！\n"); }
            getchar(); getchar(); break;
        case 17:
            char findname[30];
            int flag17;
            printf("请输入需要查找的图的名称：");
            scanf("%s", findname);
            flag17 = LocateGraph(Gras, findname);
            if (flag17) { printf("该图是第%d个图\n", flag17); }
            else { printf("未找到该图\n"); }
            getchar(); getchar(); break;
        case 18:
            int i181, i182, flag18;
            printf("请输入需要查找顶点的图序号:\n");
            scanf("%d", &i181);
            i181--;
            printf("请输入要查找的顶点关键字:\n");
            scanf("%d", &i182);
            flag18 = LocateVex(Gras.elem[i181].G, i182);
            if (flag18 == -1) { printf("未找到该顶点\n"); }
            else { printf("该顶点为第%d个顶点", flag18); }
            getchar(); getchar(); break;
        case 19:
            int i191, i192; VertexType v19;
            printf("请输入需要顶点赋值的图序号:\n");
            scanf("%d", &i191);
            i191--;
            printf("请输入要赋值的顶点:\n");
            scanf("%d", &i192);
            printf("请输入要赋的值:\n");
            scanf("%d%s", &v19.key, v19.others);
            if (PutVex(Gras.elem[i191].G, i192, v19) == ERROR) { printf("赋值操作失败\n"); }
            else { printf("赋值成功\n"); }
            getchar(); getchar(); break;
        case 20:
            int i201,i202, flag20;
            printf("请输入需要获得第一邻接点的图序号:\n");
            scanf("%d", &i201);
            i201--;
            printf("请输入要获得第一邻接点的顶点关键字:\n");
            scanf("%d", &i202);
            flag20 = FirstAdjVex(Gras.elem[i201].G, i202);
            if (flag20 == -1) { printf("查找失败\n"); }
            else { printf("第一邻接点为%d %s", Gras.elem[i201].G.vertices[flag20].data.key, Gras.elem[i201].G.vertices[flag20].data.others); }
            getchar(); getchar(); break;
        case 21:
            int i211, i212,i213, flag21;
            printf("请输入需要获得下一邻接点的图序号:\n");
            scanf("%d", &i211);
            i211--;
            printf("请输入获取的起始顶点的关键字和要获取下一邻接点的顶点的关键字:\n");
            scanf("%d%d", &i212, &i213);
            flag21 = NextAdjVex(Gras.elem[i211].G, i212, i213);
            if (flag21 == -1) { printf("查找失败\n"); }
            else { printf("下一邻接点为%d %s", Gras.elem[i211].G.vertices[flag21].data.key, Gras.elem[i211].G.vertices[flag21].data.others); }
            getchar(); getchar(); break;
        case 22:
            int i22;
            VertexType v22;
            printf("请输入需要插入顶点的图序号:\n");
            scanf("%d", &i22);
            i22--;
            printf("请输入插入顶点的值:\n");
            scanf("%d%s", &v22.key, v22.others);
            if (InsertVex(Gras.elem[i22].G,v22) == ERROR) { printf("插入失败\n"); }
            else { printf("插入成功\n"); }
            getchar(); getchar(); break;
        case 23:
            int i231, i232;
            printf("请输入需要删除顶点的图序号:\n");
            scanf("%d", &i231);
            i231--;
            printf("请输入要删除顶点的关键字:\n");
            scanf("%d", &i232);
            if (DeleteVex(Gras.elem[i231].G,i232) == ERROR) { printf("删除失败\n"); }
            else { printf("删除成功\n"); }
            getchar(); getchar(); break;
        case 24:
            int i241, i242, i243;
            printf("请输入需要插入弧的图序号:\n");
            scanf("%d", &i241);
            i241--;
            printf("请输入要插入弧的两个顶点:\n");
            scanf("%d%d", &i242, &i243);
            if (InsertArc(Gras.elem[i241].G, i242, i243) == ERROR) { printf("插入失败\n"); }
            else { printf("插入成功\n"); }
            getchar(); getchar(); break;
        case 25:
            int i251, i252, i253;
            printf("请输入需要删除弧的图序号:\n");
            scanf("%d", &i251);
            i251--;
            printf("请输入要删除弧的两个顶点:\n");
            scanf("%d%d", &i252, &i253);
            if (DeleteArc(Gras.elem[i251].G, i252, i253) == ERROR) { printf("删除失败\n"); }
            else { printf("删除成功\n"); }
            getchar(); getchar(); break;
        case 26:
            int i26;
            printf("请输入需要深度优先搜索的图序号:\n");
            scanf("%d", &i26);
            i26--;
            printf("该图名称为%s，其深度优先搜索遍历如下:\n",Gras.elem[i26].name);
            DFSTraverse(Gras.elem[i26].G, visit);
            getchar(); getchar(); break;
        case 27:
            int i27;
            printf("请输入需要广度优先搜索的图序号:\n");
            scanf("%d", &i27);
            i27--;
            printf("该图名称为%s，其广度优先搜索遍历如下:\n", Gras.elem[i27].name);
            BFSTraverse(Gras.elem[i27].G, visit);
            getchar(); getchar(); break;
        case 28:
            int i28;
            printf("请输入需要写入文件的二叉树序号：");
            scanf("%d", &i28);
            i28--;
            printf("请输入文件名称：");
            scanf("%s", filename);
            if (SaveGraph(Gras.elem[i28].G, filename) == ERROR) { printf("文件写入失败\n"); }
            else { printf("图已写入文件\n"); }
            getchar(); getchar(); break;
        case 29:
            int flag29, i29;
            char FileName29[30];
            printf("请输入文件名称：");
            scanf("%s", FileName29);
            printf("请选择：覆盖已有的图（输入1）   集合内新建一个图（输入2）\n");
            scanf("%d", &flag29);
            if (flag29 == 1) {
                printf("请输入需要覆盖的图序号：");
                scanf("%d", &i29);
                if (i29 > Gras.length || i29 < 1) { printf("该图不存在\n"); }
                else {
                    i29--;
                    DestroyGraph(Gras.elem[i29].G);
                    LoadGraph(Gras.elem[i29].G, FileName29);
                    printf("覆盖成功！\n");
                }
            }
            if (flag29 == 2) {
                if (LoadGraph(Gras.elem[Gras.length].G, FileName29)) {
                    char newname[30];
                    printf("请输入新图名称："); scanf("%s", newname);
                    strcpy(Gras.elem[Gras.length].name, newname);
                    Gras.length++;
                    printf("图读取成功，新图为图集合中第%d个\n", Gras.length);
                }
            }
            getchar(); getchar(); break;
		case 0:
		    getchar(); break;
        default:
            getchar(); break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()