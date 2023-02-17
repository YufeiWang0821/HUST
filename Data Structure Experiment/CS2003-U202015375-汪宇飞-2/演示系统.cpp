/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int status;
typedef int ElemType; //数据元素类型定义
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int ElemType;

typedef struct LNode {  //单链表（链式结构）结点的定义
	ElemType data;
	struct LNode* next;
}LNode, * LinkList;
typedef struct {  //线性表的集合类型定义
	struct {
		char name[30];
		LinkList L;
	} elem[10];
	int length;
}LISTS;

//void free0(void* p);
int op = 1; ElemType e; LinkList L; int i;
FILE* fp; char filename[30]; LISTS Lists;

status InitList(LinkList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L) { return INFEASIBLE; }
	L = (LinkList)malloc(sizeof(LNode));
	L->next = NULL;
	return OK;
	/********** End **********/
}

status DestroyList(LinkList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	LinkList p;
	while (L) {
		p = L->next;
		free(L);
		L = p;
	}
	return OK;
	/********** End **********/
}

status ClearList(LinkList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	LinkList p, q;
	p = L->next;
	while (p){
		q = p->next;
		free(p);
		p = q;
	}
	L->next = NULL;
	return OK;
	/********** End **********/
}

status ListEmpty(LinkList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	if (!L->next) { return OK; }
	else { return FALSE; }
	/********** End **********/
}

int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	int i = 0;
	while (L->next) {
		i++;
		L = L->next;
	}
	return i;
	/********** End **********/
}

status GetElem(LinkList L, int i, ElemType& e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	if (!i) { return ERROR; }
	int temp = 0;
	while (L) {
		if (temp == i) {
			e = L->data;
			return OK;
		}
		temp++;
		L = L->next;
	}
	return ERROR;
	/********** End **********/
}

status LocateElem(LinkList L, ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号；如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	int i = 0;
	while (L) {
		if (L->data == e) { return i; }
		i++;
		L = L->next;
	}
	return ERROR;
	/********** End **********/
}

status PriorElem(LinkList L, ElemType e, ElemType& pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	if (!L->next) { return ERROR; }
	if (L->next->data == e) { return ERROR; }
	LinkList p;
	p = L;
	L = L->next;
	int sta = ERROR;
	while (L) {
		if (L->data == e) {
			pre = p->data;
			sta = OK;
			break;
		}
		L = L->next;
		p = p->next;
	}
	return sta;
	/********** End **********/
}

status NextElem(LinkList L, ElemType e, ElemType& next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	int sta = ERROR;
	while (L) {
		if (L->data == e) {
			if (L->next) {
				next = L->next->data;
				sta = OK;
				break;
			}
			else { break; }
		}
		L = L->next;
	}
	return sta;
	/********** End **********/
}

status ListInsert(LinkList& L, int i, ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	if (i < 1) { return ERROR; }
	int j = 0;
	LinkList p = L, q;
	while (p && j < i - 1) { p = p->next; j++; }
	if (!p || j > i - 1) { return ERROR; }
	q = (LinkList)malloc(sizeof(LNode));
	q->data = e;
	q->next = p->next;
	p->next = q;
	return OK;
	/********** End **********/
}

status ListDelete(LinkList& L, int i, ElemType& e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	LinkList p = L, q;
	int j = 0;
	while (p->next && j < i - 1) { p = p->next; j++; }
	if (!(p->next) || j > i - 1) { return ERROR; }
	q = p->next; p->next = q->next;
	e = q->data; free(q);
	return OK;
	/********** End **********/
}

status ListTraverse(LinkList L)
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!L) { return INFEASIBLE; }
	LinkList p = L->next;
	while (p) { printf("%d ", p->data); p = p->next; }
	return OK;
	/********** End **********/
}

status SaveList(LinkList L, char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
	if (L == NULL) { return INFEASIBLE; }
	else {
		FILE* fp;
		fp = fopen(FileName, "w");
		LinkList p = L;
		while (p->next) {
			p = p->next;
			fprintf(fp, "%d ", p->data);
		}
		fclose(fp);
		return OK;
	}
}

status LoadList(LinkList& L, char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
{
	if (L != NULL) { return INFEASIBLE; }
	else
	{
		ElemType c;
		FILE* fp;
		fp = fopen(FileName, "r");
		L = (LinkList)malloc(sizeof(LNode));
		L->next = NULL;
		LinkList p = L, q;
		while (!feof(fp)) {
			fscanf(fp, "%d", &c);
			if (!feof(fp)) {
				q = (LinkList)malloc(sizeof(LNode));
				q->data = c; q->next = NULL;
				p->next = q; p = p->next;
			}
		}
		fclose(fp);
		return OK;
	}
}

status AddList(LISTS& Lists, char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表
{
	strcpy(Lists.elem[Lists.length].name, ListName);
	Lists.elem[Lists.length].L = (LinkList)malloc(LIST_INIT_SIZE * sizeof(LNode));
	Lists.elem[Lists.length].L->next = NULL;
	Lists.length++;
	return OK;
}

status RemoveList(LISTS& Lists, char ListName[])
// Lists中删除一个名称为ListName的线性表
{
	for (int i = 0; i < Lists.length; i++) {
		if (!strcmp(Lists.elem[i].name, ListName)) {
			for (int j = i; j < Lists.length; j++) { Lists.elem[j] = Lists.elem[j + 1]; }
			Lists.length--;
			return OK;
		}
	}
	return ERROR;
}

int LocateList(LISTS Lists, char ListName[])
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{
	for (int i = 0; i < Lists.length; i++) {
		if (!strcmp(Lists.elem[i].name, ListName)) {
			return i + 1;
		}
	}
	return 0;
}

void main(int argc, char* argv[]) {
	while (op) {
		system("cls");	printf("\n\n");
		printf("                         菜单                      \n");
		printf("-------------------------------------------------\n");
		printf("                 对于单个线性表的操作\n");
		printf("    	  1. 创建线性表       2. 销毁线性表\n");
		printf("    	  3. 清空线性表       4. 线性表判空\n");
		printf("    	  5. 线性表长度       6. 获取元素\n");
		printf("    	  7. 查找元素         8. 获取前驱元素\n");
		printf("    	  9. 获取后继元素     10. 插入元素\n");
		printf("    	  11.删除元素         12.遍历线性表\n");
		printf("    	  13.线性表写入文件   14.线性表读取文件\n");
		printf("-------------------------------------------------\n");
		printf("                 对于线性表集合的操作\n");
		printf("    	  15.增加一个线性表   16.删除一个线性表\n");
		printf("    	  17.查找线性表       18.线性表插入元素\n");
		printf("    	  19.线性表删除元素   20.线性表遍历\n");
		printf("    	  21.线性表写入文件   22.线性表读取文件\n");
		printf("    	  0. 退出\n");
		printf("    请选择你的操作[0~22]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (InitList(L) == INFEASIBLE) { printf("线性表已存在\n"); }
			else { printf("线性表创建成功\n"); }
			getchar(); getchar(); break;
		case 2:
			if (DestroyList(L) == INFEASIBLE) { printf("线性表不存在\n"); }
			else { printf("线性表销毁成功\n"); }
			getchar(); getchar(); break;
		case 3:
			if (ClearList(L) == INFEASIBLE) { printf("线性表不存在\n"); }
			else { printf("线性表清空成功\n"); }
			getchar(); getchar(); break;
		case 4:
			if (ListEmpty(L)== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (ListEmpty(L) == FALSE) { printf("线性表不为空\n"); }
			else { printf("线性表为空\n"); }
			getchar(); getchar(); break;
		case 5:
			if (ListLength(L)== INFEASIBLE) { printf("线性表不存在\n"); }
			else { printf("线性表长度为%d", ListLength(L)); }
			getchar(); getchar(); break;
		case 6:
			printf("请输入要获取的元素位置："); scanf("%d", &i);
			if (GetElem(L,i,e)== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (GetElem(L, i, e) == ERROR) { printf("i的范围不合法\n"); }
			else { printf("元素获取成功，为%d\n", e); }
			getchar(); getchar(); break;
		case 7:
			printf("请输入要查找的元素："); scanf("%d", &e);
			if (LocateElem(L,e)== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (LocateElem(L, e) == ERROR) { printf("线性表中无该元素\n"); }
			else { printf("该元素是线性表中第%d个元素\n", LocateElem(L, e)); }
			getchar(); getchar(); break;
		case 8:
			ElemType pre;
			printf("请输入要获取前驱的元素："); scanf("%d", &e);
			if (PriorElem(L,e,pre)== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (PriorElem(L, e, pre) == ERROR) { printf("该元素前驱不存在\n"); }
			else { printf("该元素前驱为%d\n", pre); }
			getchar(); getchar(); break;
		case 9:
			ElemType next;
			printf("请输入要获取后继的元素："); scanf("%d", &e);
			if (NextElem(L,e,next)== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (NextElem(L, e, next) == ERROR) { printf("该元素后继不存在\n"); }
			else { printf("该元素后继为%d\n", next); }
			getchar(); getchar(); break;
		case 10:
			int flag10;
			printf("请输入要插入的元素："); scanf("%d", &e);
			printf("请输入要插入的位置："); scanf("%d", &i);
			if ((flag10=ListInsert(L,i,e))== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (flag10 == ERROR) { printf("插入位置错误\n"); }
			else { printf("插入成功\n"); }
			getchar(); getchar(); break;
		case 11:
			int flag11;
			printf("请输入要删除的元素的位置:"); scanf("%d", &i);
			if ((flag11=ListDelete(L,i,e))== INFEASIBLE) { printf("线性表不存在\n"); }
			else if (flag11 == ERROR) { printf("删除位置错误\n"); }
			else { printf("已成功删除，该元素为%d\n",e); }
			getchar(); getchar(); break;
		case 12:
			if (ListTraverse(L)== INFEASIBLE) { printf("线性表不存在\n"); }
			getchar(); getchar(); break;
		case 13:
			printf("请输入文件名称："); scanf("%s", filename);
			if (SaveList(L,filename)== INFEASIBLE) { printf("线性表不存在\n"); }
			else { printf("线性表已写入文件\n"); }
			getchar(); getchar(); break;
		case 14:
			printf("请输入文件名称："); scanf("%s", filename);
			if (LoadList(L, filename) == INFEASIBLE) { printf("线性表已存在\n"); }
			else { printf("线性表已读取文件\n"); }
			getchar(); getchar();
			break;
//以下为对线性表集合的操作
		case 15:
			char newlistname[30];
			printf("请输入新线性表的名称：");
			scanf("%s", newlistname);
			if (AddList(Lists, newlistname)) { printf("线性表创建成功！\n"); }
			getchar(); getchar();
			break;
		case 16:
			char dename[30];
			printf("请输入需要删除的线性表的名称：");
			scanf("%s", dename);
			if (RemoveList(Lists, dename) == ERROR) { printf("删除失败！\n"); }
			else { printf("删除成功！\n"); }
			getchar(); getchar();
			break;
		case 17:
			char findname[30];
			int flag17;
			printf("请输入需要查找的线性表的名称：");
			scanf("%s", findname);
			flag17 = LocateList(Lists, findname);
			if (flag17) { printf("该线性表是第%d个线性表\n", flag17); }
			else { printf("未找到该线性表！\n"); }
			getchar(); getchar();
			break;
		case 18:
			int i181, i182, flag18;
			printf("请输入需要插入的线性表序号：");
			scanf("%d", &i181);
			i181--;
			printf("请输入插入的元素："); scanf("%d", &e);
			printf("请输入插入位置："); scanf("%d", &i182);
			if ((flag18 = ListInsert(Lists.elem[i181].L, i182, e)) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (flag18 == ERROR) { printf("插入位置错误！\n"); }
			else { printf("已插入成功！\n"); }
			getchar(); getchar();
			break;
		case 19:
			int i191, i192, flag19;
			printf("请输入需要删除的线性表序号：");
			scanf("%d", &i191);
			i191--;
			printf("请输入删除位置："); scanf("%d", &i192);
			if ((flag19 = ListDelete(Lists.elem[i191].L, i192, e)) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (flag19 == ERROR) { printf("删除位置错误！\n"); }
			else { printf("已删除成功！\n"); }
			getchar(); getchar();
			break;
		case 20:
			int i20;
			printf("请输入需要遍历的线性表序号：");
			scanf("%d", &i20);
			if(i20 > Lists.length || i20 < 1) { printf("该线性表不存在\n"); }
			else {
				i20--;
				if (ListTraverse(Lists.elem[i20].L) == INFEASIBLE) { printf("线性表不存在！\n"); }
				else { printf("该线性表名称为：%s\n", Lists.elem[i20].name); }
			}
			getchar(); getchar();
			break;
		case 21:
			int i21;
			printf("请输入需要写入文件的线性表序号：");
			scanf("%d", &i21);
			i21--;
			printf("请输入文件名称：");
			scanf("%s", filename);
			if (SaveList(Lists.elem[i21].L, filename) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else { printf("线性表元素已写入文件\n"); }
			getchar(); getchar();
			break;
		case 22:
			int flag22, i22;
			char FileName22[30];
			printf("请输入文件名称：");
			scanf("%s", FileName22);
			printf("请选择：覆盖已有的线性表（输入1）   集合内新建一个线性表（输入2）\n");
			scanf("%d", &flag22);
			if (flag22 == 1) {
				printf("请输入需要覆盖的线性表序号：");
				scanf("%d", &i22);
				if (i22 > Lists.length || i22 < 1) { printf("该线性表不存在\n"); }
				else {
					i22--;
					ClearList(Lists.elem[i22].L);
					FILE* fp22;
					ElemType c22;
					LinkList q,p = Lists.elem[i22].L;
					fp22 = fopen(FileName22, "r");
					int k;
					while ((k=fscanf(fp22, "%d", &c22)) != -1) {
						q = (LinkList)malloc(sizeof(LNode));
						q->data = c22; q->next = NULL;
						p->next = q; p = q;
					}
					p->next = NULL;
					fclose(fp22);
				}
				printf("覆盖成功！\n");
			}
			if (flag22 == 2) {
				if (LoadList(Lists.elem[Lists.length].L, FileName22)) {
					char newname[30];
					printf("请输入新线性表名称："); scanf("%s", newname);
					strcpy(Lists.elem[Lists.length].name, newname);
					Lists.length++;
					printf("线性表读取成功，新线性表为线性表集合中第%d个\n", Lists.length);
				}
			}
			getchar(); getchar();
			break;
		case 0:
			getchar();
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()