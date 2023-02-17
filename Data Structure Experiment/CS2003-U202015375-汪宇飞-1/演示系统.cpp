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
typedef struct {  //顺序表（顺序结构）的定义
	ElemType* elem;
	int length;
	int listsize;
}SqList;
typedef struct {  //线性表的集合类型定义
	struct {
		char name[30];
		SqList L;
	} elem[10];
	int length;
}LISTS;
LISTS Lists;
SqList L;  int op = 1;
ElemType e;
FILE* fp; char filename[30]; int i;

status InitList(SqList& L) {
	if (L.elem) { return INFEASIBLE; }
	L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L.elem) exit(OVERFLOW);
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}

status DestroyList(SqList& L) {
	if (!L.elem) { return INFEASIBLE; }
	L.length = 0;
	L.listsize = 0;
	free(L.elem);
	L.elem = NULL;
	return OK;
}

status ClearList(SqList& L) {
	if (!L.elem) { return INFEASIBLE; }
	ElemType* p, * q;
	int i = 1;
	p = L.elem;
	q = L.elem + L.length - 1;
	for (; p <= q; p++) { *p = 0; }
	L.length = 0;
	return OK;
}

status ListEmpty(SqList L) {
	if (!L.elem) { return INFEASIBLE; }
	if (!L.length) { return TRUE; }
	return FALSE;
}

status ListLength(SqList L) {
	if (!L.elem) { return INFEASIBLE; }
	return (L.length);
}

status GetElem(SqList L, int i, ElemType& e) {
	if (!L.elem) { return INFEASIBLE; }
	if ((i < 1) || (i > L.length)) { return ERROR; }
	ElemType* p;
	p = L.elem + i - 1;
	e = *p;
	return OK;
}// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。

status LocateElem(SqList L, ElemType e) {
	if (!L.elem) { return INFEASIBLE; }
	int i = 1, sta = ERROR;
	ElemType* p;
	p = L.elem;
	for (; i <= L.length; p++, i++) {
		if (*p == e) {
			sta = i;
			break;
		}
	}
	return sta;
}// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。

status PriorElem(SqList L, ElemType e, ElemType& pre) {
	if (!L.elem) { return INFEASIBLE; }
	ElemType* p;
	int i = 1, sta = ERROR;
	p = L.elem;
	if (*p == e) { return sta; }
	for (; i <= L.length; p++, i++) {
		if (*p == e) {
			pre = *(p - 1);
			sta = OK;
			break;
		}
	}
	return sta;
}// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。

status NextElem(SqList L, ElemType e, ElemType& next) {
	if (!L.elem) { return INFEASIBLE; }
	ElemType* p;
	int i = 1, sta = ERROR;
	p = L.elem;
	for (; i <= L.length; p++, i++) {
		if (*p == e) {
			if (i < L.length) {
				next = *(p + 1);
				sta = OK;
				break;
			}
			else { break; }
		}
	}
	return sta;
}// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。

status ListInsert(SqList& L, int i, ElemType e) {
	if (!L.elem) { return INFEASIBLE; }
	if ((i < 1) || (i > L.length + 1)) { return ERROR; }
	if (L.length == L.listsize) {
		ElemType* newbase;
		newbase = (ElemType*)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
		L.elem = newbase;
		L.listsize += LISTINCREMENT;
	}
	ElemType* p, * q;
	q = &(L.elem[i - 1]);
	for (p = &(L.elem[L.length - 1]); p >= q; p--) { *(p + 1) = *p; }
	*q = e;
	L.length++;
	return OK;
}// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。

status ListDelete(SqList& L, int i, ElemType& e) {
	if (!L.elem) { return INFEASIBLE; }
	if ((i < 1) || (i > L.length)) { return ERROR; }
	ElemType* p, * q;
	p = &(L.elem[i - 1]);
	e = *p;
	q = L.elem + L.length - 1;
	for (p++; p <= q; p++) { *(p - 1) = *p; }
	L.length--;
	return OK;
}// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。

status ListTrabverse(SqList L) {
	if (!L.elem) { return INFEASIBLE; }
	int i;
	printf("\n-----------all elements -----------------------\n");
	for (i = 0; i < L.length; i++) printf("%d ", L.elem[i]);
	printf("\n------------------ end ------------------------\n");
	return L.length;
} 

status SaveList(SqList L, char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
	if (!L.elem) { return INFEASIBLE; }
	FILE* fp;
	fp = fopen(FileName, "w");
	int i = 0;
	for (; i < L.length; i++) { fprintf(fp, "%d ", *(L.elem+i)); }
	fclose(fp);
	return OK;
}

status LoadList(SqList& L, char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
{
	if (L.elem) { return INFEASIBLE; }
	L.elem = (ElemType*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);
	int i = 0;
	FILE* fp;
	fp = fopen(FileName, "r");
	while (fscanf(fp, "%d", L.elem + i) != -1) { i++; L.length++; }
	fclose(fp);
	return OK;
}

status AddList(LISTS& Lists, char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表，线性表数据又后台测试程序插入。
{
	strcpy(Lists.elem[Lists.length].name, ListName);
	Lists.elem[Lists.length].L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	Lists.elem[Lists.length].L.length = 0;
	Lists.elem[Lists.length].L.listsize = LIST_INIT_SIZE;
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
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~22]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (InitList(L) == OK) printf("线性表创建成功！\n");
			else printf("线性表创建失败！\n");
			getchar(); getchar();
			break;
		case 2:
			if (DestroyList(L) == OK) printf("线性表销毁成功！\n");
			else printf("线性表销毁失败！\n");
			getchar(); getchar();
			break;
		case 3:
			if (ClearList(L) == OK) printf("线性表清空成功！\n");
			else printf("线性表清空失败！\n");
			getchar(); getchar();
			break;
		case 4:
			if (ListEmpty(L) == TRUE){ printf("线性表为空！\n"); }
			else if (ListEmpty(L) == FALSE) { printf("线性表不为空！\n"); }
			else { printf("线性表不存在！\n"); }
			getchar(); getchar();
			break;
		case 5:
			if (ListLength(L) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else { printf("线性表的长度为%d\n", ListLength(L)); }
			getchar(); getchar();
			break;
		case 6:
			int i;
			printf("请输入需要获取元素的位置：\n");
			scanf("%d", &i);
			if (GetElem(L, i, e) == OK) { printf("元素获取成功！\n"); }
			else if (GetElem(L, i, e) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else { printf("i不合法！\n"); }
			getchar(); getchar();
			break;
		case 7:
			printf("请输入需要查找的元素：\n");
			scanf("%d", &e);
			if(LocateElem(L,e)==INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (!LocateElem(L, e)) { printf("未找到该元素！\n"); }
			else { printf("该元素是第%d个元素\n", LocateElem(L, e)); }
			getchar(); getchar();
			break;
		case 8:
			ElemType pre;
			printf("请输入需要查找前驱的元素：\n");
			scanf("%d", &e);
			if(PriorElem(L,e,pre)==INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (PriorElem(L, e, pre) == ERROR) { printf("前驱不存在！\n"); }
			else { printf("前驱为%d\n",pre); }
			getchar(); getchar();
			break;
		case 9:
			ElemType next;
			printf("请输入需要查找后继的元素：\n");
			scanf("%d", &e);
			if (NextElem(L, e, next) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (NextElem(L, e, next) == ERROR) { printf("后继不存在！\n"); }
			else { printf("后继为%d\n", next); }
			getchar(); getchar();
			break;
		case 10:
			int i10,flag10;
			printf("请输入插入的元素："); scanf("%d", &e);
			printf("请输入插入位置："); scanf("%d", &i10);
			if((flag10=ListInsert(L,i10,e))==INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (flag10 == ERROR) { printf("插入位置错误！\n"); }
			else { printf("已插入成功！\n"); }
			getchar(); getchar();
			break;
		case 11:
			int i11,flag11;
			printf("请输入删除位置："); scanf("%d", &i11);
			if ((flag11=ListDelete(L, i11, e)) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else if (flag11== ERROR) { printf("删除位置错误！\n"); }
			else { printf("已删除成功！\n"); }
			getchar(); getchar();
			break;
		case 12:
			if (ListTrabverse(L)==INFEASIBLE) printf("线性表是空表！\n");
			getchar(); getchar();
			break;
		case 13:
			printf("请输入文件名称：");
			scanf("%s", filename);
			if (SaveList(L, filename) == INFEASIBLE) { printf("线性表不存在！\n"); }
			else { printf("线性表元素已写入文件\n"); }
			getchar(); getchar();
			break;
		case 14:
			printf("请输入文件名称：");
			scanf("%s", filename);
			if (LoadList(L, filename) == INFEASIBLE) { printf("线性表已经存在！\n"); }
			else { printf("线性表已读取文件\n"); }
			getchar(); getchar();
			break;
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
			flag17=LocateList(Lists, findname);
			if(flag17){ printf("该线性表是第%d个线性表\n", flag17); }
			else { printf("未找到该线性表！\n"); }
			getchar(); getchar();
			break;
		case 18:
			int i181,i182,flag18;
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
			int i191, i192,flag19;
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
			i20--;
			printf("该线性表名称为：%s",Lists.elem[i20].name );
			if (ListTrabverse(Lists.elem[i20].L) == INFEASIBLE) printf("线性表是空表！\n");
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
			if (flag22 == 2) {
				if (LoadList(Lists.elem[Lists.length].L, FileName22)) {
					Lists.length++;
					printf("线性表读取成功，新线性表为线性表集合中第%d个\n", Lists.length);
				}
			}
			if (flag22 == 1) {
				printf("请输入需要覆盖的线性表序号：");
				scanf("%d", &i22);
				if (i22 > Lists.length || i22 < 1) { printf("该线性表不存在\n"); }
				else {
					i22--;
					ClearList(Lists.elem[i22].L);
					int i221 = 0; 
					FILE* fp22;
					fp22 = fopen(FileName22, "r");
					while (fscanf(fp22, "%d", Lists.elem[i22].L.elem + i221) != -1) { i221++; Lists.elem[i22].L.length++; }
					fclose(fp22);
					printf("覆盖成功！\n");
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