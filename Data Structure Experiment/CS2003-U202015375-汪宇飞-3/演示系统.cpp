#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status;
typedef int KeyType;
typedef struct {//二叉树结点类型定义
	KeyType  key;
	char others[20];
} TElemType; 
typedef struct BiTNode {  //二叉链表结点的定义
	TElemType  data;
	struct BiTNode* lchild, * rchild;
} BiTNode, * BiTree;
typedef struct QNode {//队列结点类型的定义
	BiTree data;
	struct QNode* next;
}QNode, * QueuePtr;
struct LinkQueue {//队列的定义
	QueuePtr front, rear;
};
typedef struct {  //二叉树的集合类型定义
	struct {
		char name[30];
		BiTree T;
	} elem[10];
	int length;
}TREES;

int op=1, keyword[100], len1, len2, flagCB;
BiTree T; TREES Trees; FILE* fp; char filename[30];
TElemType definition[100];

void visit(BiTree T)
{
	printf(" %d,%s", T->data.key, T->data.others);
}
status EnQueue(LinkQueue& Q, BiTree e) {//将二叉树结点插入e队列末尾
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	p->data = e; p->next = NULL;
	Q.rear->next = p; Q.rear = p;
	return OK;
}
BiTree DeQueue(LinkQueue& Q) {//若队列不空则删除队列的头结点，并返回其二叉树结点
	if (Q.front == Q.rear) { return NULL; }
	QueuePtr p = Q.front->next;
	Q.front->next = p->next;
	if (Q.rear == p) { Q.rear = Q.front; }
	BiTree t = p->data;
	free(p);
	return t;
}
status EmptyQueue(LinkQueue Q) {
	if (Q.front == Q.rear) { return ERROR; }
	return OK;
}
status CreateBiTree(BiTree& T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。*/
{
	if (flagCB == ERROR) { return flagCB; }
	int num = definition[len2].key;
	if (num == -1) { T = NULL; return flagCB; }
	else if (!num) { T = NULL; len2++; }
	else {
		for (int i = 0; i < len1 + 1; i++) {
			if (num == keyword[i]) { flagCB = ERROR; break; }
		}
		if (flagCB == OK) {
			T = (BiTree)malloc(sizeof(BiTNode));
			T->data.key = num;
			strcpy(T->data.others, definition[len2].others);
			keyword[len1++] = num;
			len2++;
			CreateBiTree(T->lchild, definition);
			CreateBiTree(T->rchild, definition);
		}
	}
	return flagCB;
}
status DestroyBiTree(BiTree& T) {//销毁二叉树
	if (!T) { return INFEASIBLE; }
	DestroyBiTree(T->lchild);
	DestroyBiTree(T->rchild);
	free(T);
	T = NULL;
	return OK;
}
status EmptyBiTree(BiTree T) {//二叉树判空
	if (!T) { return OK; }
	else { return ERROR; }
}
status ClearBiTree(BiTree& T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return INFEASIBLE; }
	ClearBiTree(T->lchild);
	ClearBiTree(T->rchild);
	free(T);
	T = NULL;
	return OK;
	/********** End **********/
}
int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return 0; }
	int l = BiTreeDepth(T->lchild);
	int r = BiTreeDepth(T->rchild);
	int depth = (l > r) ? l : r;
	return depth + 1;
	/********** End **********/
}
BiTNode* LocateNode(BiTree T, KeyType e)
//查找结点
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return NULL; }
	if (T->data.key == e) { return T; }
	BiTree flag = LocateNode(T->lchild, e);
	if (!flag) { flag = LocateNode(T->rchild, e); }
	return flag;
	/********** End **********/
}
status Assign(BiTree& T, KeyType e, TElemType value)
//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return ERROR; }
	BiTree flag = LocateNode(T, value.key);
	if (flag) {
		if (flag->data.key != e) { return ERROR; }
	}

	if (T->data.key == e) {
		T->data = value;
		return OK;
	}
	if (Assign(T->lchild, e, value)) { return OK; }
	else if (Assign(T->rchild, e, value)) { return OK; }
	else { return ERROR; }
	/********** End **********/
}
BiTNode* GetSibling(BiTree T, KeyType e)
//实现获得兄弟结点
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if ((!T) || T->data.key == e) { return NULL; }
	int lflag = (T->lchild) ? 1 : 0;
	int rflag = (T->rchild) ? 1 : 0;
	if (lflag && T->lchild->data.key == e) { return T->rchild; }
	if (rflag && T->rchild->data.key == e) { return T->lchild; }
	BiTree lt = GetSibling(T->lchild, e), rt = GetSibling(T->rchild, e);
	if (lt) { return lt; }
	else if (rt) { return rt; }
	else { return ERROR; }
	/********** End **********/
}
BiTree LocateParent(BiTree T, KeyType e) {
	//查找并返回关键字为e的结点的父母结点
	if ((!T) || (T->data.key == e)) { return NULL; }
	if ((T->lchild && T->lchild->data.key == e) || (T->rchild && T->rchild->data.key == e)) { return T; }
	BiTree lt = LocateParent(T->lchild, e), rt = LocateParent(T->rchild, e);
	if (lt) { return lt; }
	else if (rt) { return rt; }
	else { return NULL; }
}
BiTree RightestNode(BiTree T) {
	//寻找树最右端的结点
	if (!T->rchild) { return T; }
	return RightestNode(T->rchild);
}
BiTree LeftestNode(BiTree T) {
	//寻找树最右端的结点
	if (!T->lchild) { return T; }
	return LeftestNode(T->lchild);
}
status InsertNode(BiTree& T, KeyType e, int LR, TElemType c)
//插入结点。此题允许通过增加其它函数辅助实现本关任务
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return ERROR; }
	BiTree onl = LocateNode(T, c.key);
	if (onl) { return ERROR; }
	BiTree temp = (BiTree)malloc(sizeof(BiTNode));
	temp->data = c;
	if (LR == -1) {
		temp->rchild = T;
		temp->lchild = NULL;
		T = temp;
		return OK;
	}
	BiTree p = LocateNode(T, e);
	if (!p) { return ERROR; }
	if (LR) {
		temp->rchild = p->rchild;
		p->rchild = temp;
		temp->lchild = NULL;
		return OK;
	}
	else {
		temp->rchild = p->lchild;
		p->lchild = temp;
		temp->lchild = NULL;
		return OK;
	}
	/********** End **********/
}
status DeleteNode(BiTree& T, KeyType e)
//删除结点。此题允许通过增加其它函数辅助实现本关任务
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return ERROR; }
	BiTree p = LocateNode(T, e), prt = LocateParent(T, e);
	if (!p) { return ERROR; }
	int ldu = (p->lchild) ? 1 : 0, rdu = (p->rchild) ? 1 : 0;
	if (T->data.key == e) {
		if (ldu && rdu) {
			prt = T;
			T = prt->lchild;
			BiTree right = RightestNode(prt->lchild);
			right->rchild = prt->rchild;
			free(prt);
		}
		if ((!ldu) && (!rdu)) {
			free(T); T = NULL;
		}
		if (ldu && !(rdu)) {
			free(T); T = T->lchild;
		}
		if ((!ldu) && rdu) {
			free(T); T = T->rchild;
		}
		return OK;
	}
	int flag = (prt->rchild == p) ? 1 : 0;
	if ((!ldu) && (!rdu)) {
		if (flag) { prt->rchild = NULL; }
		else { prt->lchild = NULL; }
		free(p); return OK;
	}
	if (ldu && (!rdu)) {
		if (flag) { prt->rchild = p->lchild; }
		else { prt->lchild = p->lchild; }
		free(p); return  OK;
	}
	if ((!ldu) && rdu) {
		if (flag) { prt->rchild = p->rchild; }
		else { prt->lchild = p->rchild; }
		free(p); return  OK;
	}
	if (ldu && rdu) {
		if (flag) { prt->rchild = p->lchild; }
		else { prt->lchild = p->lchild; }
		BiTree right = RightestNode(p->lchild);
		right->rchild = p->rchild;
		free(p); return  OK;
	}
	return OK;
	/********** End **********/
}
status PreOrderTraverse(BiTree T, void (*visit)(BiTree))
//先序遍历二叉树T
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (T) {
		visit(T);
		PreOrderTraverse(T->lchild, visit);
		PreOrderTraverse(T->rchild, visit);
	}
	return OK;
	/********** End **********/
}
status InOrderTraverse(BiTree T, void (*visit)(BiTree))
//后序遍历二叉树T
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (T) {
		InOrderTraverse(T->lchild, visit);
		visit(T);
		InOrderTraverse(T->rchild, visit);
	}
	return OK;
	/********** End **********/
}
status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
//后序遍历二叉树T
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (T) {
		PostOrderTraverse(T->lchild, visit);
		PostOrderTraverse(T->rchild, visit);
		visit(T);
	}
	return OK;
	/********** End **********/
}
status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
//按层遍历二叉树T
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (!T) { return INFEASIBLE; }
	LinkQueue Q;
	Q.front = (QueuePtr)malloc(sizeof(QNode));
	Q.rear = Q.front;
	Q.front->next = NULL;
	BiTree root = T;
	EnQueue(Q, root);
	while (EmptyQueue(Q)) {
		root = DeQueue(Q);

		printf("%d,%s ", root->data.key, root->data.others);
		if (root->lchild) { EnQueue(Q, root->lchild); }
		if (root->rchild) { EnQueue(Q, root->rchild); }
	}
	return OK;
	/********** End **********/
}
void FileSave(BiTree& T, FILE* fp) {//二叉树写入已打开的文件的函数
	if (!T) { fprintf(fp, "%d ", 0); return; }
	fprintf(fp, "%d %s ", T->data.key, T->data.others);
	FileSave(T->lchild, fp);
	FileSave(T->rchild, fp);
	return;
}
void FileLoad(BiTree& T, FILE* fp) {//读取已打开的文件创建二叉树的函数
	int c; char s[20];
	if (fscanf(fp, "%d", &c) == -1) { return; }
	if (!c) { T = NULL; return; }
	T = (BiTree)malloc(sizeof(BiTNode));
	T->data.key = c;
	fscanf(fp, "%s", s);
	strcpy(T->data.others, s);
	FileLoad(T->lchild, fp);
	FileLoad(T->rchild, fp);
	return;
}
status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{
	if (!T) { return INFEASIBLE; }
	FILE* fp = fopen(FileName, "w");
	if (!fp) { return ERROR; }
	FileSave(T, fp);
	fclose(fp);
	return  OK;
}
status LoadBiTree(BiTree& T, char FileName[])
//读入文件FileName的结点数据，创建二叉树
{
	if (T) { return INFEASIBLE; }
	FILE* fp = fopen(FileName, "r");
	if (!fp) { return ERROR; }
	FileLoad(T, fp);
	fclose(fp);
	return  OK;
}
status AddBiTree(TREES& Trees, char TreeName[]) {
//Trees中增加一个名称为TreeName的二叉树
	strcpy(Trees.elem[Trees.length].name, TreeName);
	int flagAB=CreateBiTree(Trees.elem[Trees.length].T, definition);
	Trees.length++;
	return flagAB;
}
status RemoveBiTree(TREES& Trees, char TreeName[])
// Trees中删除一个名称为TreeName的二叉树
{
	for (int i = 0; i < Trees.length; i++) {
		if (!strcmp(Trees.elem[i].name, TreeName)) {
			for (int j = i; j < Trees.length; j++) { Trees.elem[j] = Trees.elem[j + 1]; }
			Trees.length--;
			return OK;
		}
	}
	return ERROR;
}
int LocateBiTree(TREES& Trees, char TreeName[])
// 在Trees中查找一个名称为TreeName的二叉树，成功返回逻辑序号，否则返回0
{
	for (int i = 0; i < Trees.length; i++) {
		if (!strcmp(Trees.elem[i].name, TreeName)) {
			return i + 1;
		}
	}
	return 0;
}
void main(int argc, char* argv[]) {
	while (op) {
		system("cls");	printf("\n\n");
		printf("                                   菜单                      \n");
		printf("-------------------------------------------------------------\n");
		printf("                 对于单个二叉树的操作\n");
		printf("    	  1. 创建二叉树       2. 销毁二叉树       3. 清空二叉树\n");
		printf("    	  4. 二叉树判空       5. 二叉树深度       6. 查找结点\n");
		printf("    	  7. 结点赋值         8. 获取兄弟结点     9. 获取父母结点\n");
		printf("    	  10.插入结点         11.删除结点         12.前序遍历\n");
		printf("    	  13.中序遍历         14.后序遍历         15.按层遍历\n");
		printf("    	  16.获取最右端结点   17.获取最左端结点    18.二叉树写入文件\n");
		printf("          19.二叉树读取文件\n");
		printf("-------------------------------------------------\n");
		printf("                 对于二叉树集合的操作\n");
		printf("    	  20.增加一个二叉树   21.删除一个二叉树    22.查找二叉树\n");
		printf("    	  23.二叉树插入结点   24.二叉树删除结点    25.二叉树先序遍历\n");
		printf("    	  26.二叉树中序遍历   27.二叉树后序遍历    28.二叉树按层遍历\n");
		printf("    	  29.二叉树写入文件   30.二叉树读取文件\n");
		printf("    	  0. 退出\n");
		printf("    请选择你的操作[0~30]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			int flag1, i1;
			i1 = 0, len1 = 0, len2 = 0, flagCB = OK;
			memset(keyword, 0, sizeof keyword);
			printf("请输入所要创建的二叉树的先序遍历\n");
			printf("如：1 a 2 b 0 null  0 null 3 c 4 d  0 null  0 null 5 e  0 null  0 null -1 null\n");
			printf("输入：");
			do {
				scanf("%d%s", &definition[i1].key, definition[i1].others);
			} while (definition[i1++].key != -1);
			if (T) { printf("二叉树已存在\n"); }
			else {
				flag1 = CreateBiTree(T, definition);
				if (flag1 == ERROR) { printf("关键字不唯一\n"); }
				else { printf("二叉树创建成功\n"); }
			}
			getchar(); getchar(); break;
		case 2:
			if (DestroyBiTree(T) == INFEASIBLE) { printf("二叉树不存在\n"); }
			else { printf("销毁成功\n"); }
			getchar(); getchar(); break;
		case 3:
			if (ClearBiTree(T) == INFEASIBLE) { printf("二叉树不存在\n"); }
			else { printf("清空成功\n"); }
			getchar(); getchar(); break;
		case 4:
			if (EmptyBiTree(T) == ERROR) { printf("二叉树不为空\n"); }
			else { printf("二叉树为空\n"); }
			getchar(); getchar(); break;
		case 5:
			if (!T) { printf("二叉树不存在\n"); }
			else { printf("二叉树深度为%d\n", BiTreeDepth(T)); }
			getchar(); getchar(); break;
		case 6:
			int i6;
			printf("请输入要查找结点的关键字：");
			scanf("%d", &i6);
			BiTree p6;
			p6 = LocateNode(T, i6);
			if (p6) printf("该结点的数据为：%d,%s\n", p6->data.key, p6->data.others);
			else printf("查找失败\n");
			getchar(); getchar(); break;
		case 7:
			int i7; TElemType v7;
			printf("请输入要赋值的结点关键字：");
			scanf("%d", &i7);
			printf("请输入所赋值的值：");
			scanf("%d %s", &v7.key, v7.others);
			if (Assign(T, i7, v7) == ERROR) { printf("赋值失败\n"); }
			else { printf("赋值成功\n"); }
			getchar(); getchar(); break;
		case 8:
			int i8;
			printf("请输入要查找兄弟结点的结点关键字：");
			scanf("%d", &i8);
			BiTree p8;
			p8 = GetSibling(T, i8);
			if (p8) printf("%d,%s", p8->data.key, p8->data.others);
			else printf("无兄弟结点");
			getchar(); getchar(); break;
		case 9:
			int i9;
			printf("请输入要查找父母结点的结点关键字：");
			scanf("%d", &i9);
			BiTree p9;
			p9 = LocateParent(T, i9);
			if (p9) printf("%d,%s", p9->data.key, p9->data.others);
			else printf("无父母结点");
			getchar(); getchar(); break;
		case 10:
			int i10, lr10; TElemType v10;
			printf("请输入插入方式：0.作为左孩子 1.作为右孩子 -1.作为根结点:");
			scanf("%d", &lr10);
			if (lr10 != -1) {
				printf("请输入要插入结点的父母结点关键字：");
				scanf("%d", &i10);
			}
			else { i10 = 1; }
			printf("请输入结点的值：");
			scanf("%d %s", &v10.key, v10.others);
			if (InsertNode(T, i10, lr10, v10) == ERROR) { printf("插入失败\n"); }
			else { printf("插入成功\n"); }
			getchar(); getchar(); break;
		case 11:
			int i11;
			printf("请输入要删除结点的关键字：");
			scanf("%d", &i11);
			if (DeleteNode(T, i11) == ERROR) { printf("删除失败\n"); }
			else { printf("删除成功\n"); }
			getchar(); getchar(); break;
		case 12:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				printf("前序遍历如下：\n");
				PreOrderTraverse(T, visit);
			}
			getchar(); getchar(); break;
		case 13:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				printf("中序遍历如下：\n");
				InOrderTraverse(T, visit);
			}
			getchar(); getchar(); break;
		case 14:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				printf("后序遍历如下：\n");
				PostOrderTraverse(T, visit);
			}
			getchar(); getchar(); break;
		case 15:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				printf("按层遍历如下：\n");
				LevelOrderTraverse(T, visit);
			}
			getchar(); getchar(); break;
		case 16:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				BiTree p16;
				p16 = RightestNode(T);
				printf("结点的值为：%d,%s", p16->data.key, p16->data.others);
			}
			getchar(); getchar(); break;
		case 17:
			if (!T) { printf("二叉树不存在\n"); }
			else {
				BiTree p17;
				p17 = LeftestNode(T);
				printf("结点的值为：%d,%s", p17->data.key, p17->data.others);
			}
			getchar(); getchar(); break;
		case 18:
			printf("请输入文件名称："); scanf("%s", filename);
			if (SaveBiTree(T, filename) == INFEASIBLE) { printf("二叉树不存在\n"); }
			else { printf("二叉树已写入文件\n"); }
			getchar(); getchar(); break;
		case 19:
			printf("请输入文件名称："); scanf("%s", filename);
			if (LoadBiTree(T, filename) == INFEASIBLE) { printf("二叉树已存在\n"); }
			else { printf("二叉树已读取文件\n"); }
			getchar(); getchar(); break;
//以下为对二叉树集合的操作
		case 20:
			char newlistname[30]; int flag20;
			printf("请输入新二叉树的名称：");
			scanf("%s", newlistname);
			i1 = 0, len1 = 0, len2 = 0, flagCB = OK;
			memset(keyword, 0, sizeof keyword);
			printf("请输入所要创建的二叉树的先序遍历\n");
			printf("如：1 a 2 b 0 null  0 null 3 c 4 d  0 null  0 null 5 e  0 null  0 null -1 null\n");
			printf("输入：");
			do {
				scanf("%d%s", &definition[i1].key, definition[i1].others);
			} while (definition[i1++].key != -1);
			flag20 = AddBiTree(Trees,newlistname);
			if (flag20 == ERROR) { printf("关键字不唯一\n"); }
			else { printf("二叉树创建成功\n"); }
			getchar(); getchar(); break;
		case 21:
			char dename[30];
			printf("请输入需要删除的二叉树的名称：");
			scanf("%s", dename);
			if (RemoveBiTree(Trees, dename) == ERROR) { printf("删除失败！\n"); }
			else { printf("删除成功！\n"); }
			getchar(); getchar(); break;
		case 22:
			char findname[30];
			int flag17;
			printf("请输入需要查找的二叉树的名称：");
			scanf("%s", findname);
			flag17 = LocateBiTree(Trees, findname);
			if (flag17) { printf("该二叉树是第%d个二叉树\n", flag17); }
			else { printf("未找到该二叉树\n"); }
			getchar(); getchar(); break;
		case 23:
			int i231, i232, lr23; TElemType v23;
			printf("请输入需要插入的线性表序号：");
			scanf("%d", &i231);
			i231--;
			printf("请输入插入方式：0.作为左孩子 1.作为右孩子 -1.作为根结点:");
			scanf("%d", &lr23);
			if (lr23 != -1) {
				printf("请输入要插入结点的父母结点关键字：");
				scanf("%d", &i232);
			}
			else { i232 = 1; }
			printf("请输入插入的结点的值："); 
			scanf("%d %s", &v23.key,v23.others);
			if (InsertNode(Trees.elem[i231].T,i232,lr23,v23) == ERROR) { printf("插入失败\n"); }
			else { printf("插入成功\n"); }
			getchar(); getchar(); break;
		case 24:
			int i241, i242;
			printf("请输入需要删除的线性表序号：");
			scanf("%d", &i241);
			i241--;
			printf("请输入要删除结点的关键字："); 
			scanf("%d", &i242);
			if (DeleteNode(Trees.elem[i241].T,i242) == ERROR) { printf("删除失败\n"); }
			else { printf("删除成功\n"); }
			getchar(); getchar(); break;
		case 25:
			int i25;
			printf("请输入需要遍历的二叉树序号：");
			scanf("%d", &i25);
			if (i25 > Trees.length || i25 < 1) { printf("该二叉树不存在\n"); }
			else {
				i25--;
				printf("该二叉树名称为：%s 先序遍历如下\n", Trees.elem[i25].name);
				PreOrderTraverse(Trees.elem[i25].T, visit);
			}
			getchar(); getchar(); break;
		case 26:
			int i26;
			printf("请输入需要遍历的二叉树序号：");
			scanf("%d", &i26);
			if (i26 > Trees.length || i26 < 1) { printf("该二叉树不存在\n"); }
			else {
				i26--;
				printf("该二叉树名称为：%s 中序遍历如下\n", Trees.elem[i26].name);
				InOrderTraverse(Trees.elem[i26].T, visit);
			}
			getchar(); getchar(); break;
		case 27:
			int i27;
			printf("请输入需要遍历的二叉树序号：");
			scanf("%d", &i27);
			if (i27 > Trees.length || i27 < 1) { printf("该二叉树不存在\n"); }
			else {
				i27--;
				printf("该二叉树名称为：%s 后序遍历如下\n", Trees.elem[i27].name);
				PostOrderTraverse(Trees.elem[i27].T, visit);
			}
			getchar(); getchar(); break;
		case 28:
			int i28;
			printf("请输入需要遍历的二叉树序号：");
			scanf("%d", &i28);
			if (i28 > Trees.length || i28 < 1) { printf("该二叉树不存在\n"); }
			else {
				i28--;
				printf("该二叉树名称为：%s 按层遍历如下\n", Trees.elem[i28].name);
				LevelOrderTraverse(Trees.elem[i28].T, visit);
			}
			getchar(); getchar(); break;
		case 29:
			int i29;
			printf("请输入需要写入文件的二叉树序号：");
			scanf("%d", &i29);
			i29--;
			printf("请输入文件名称：");
			scanf("%s", filename);
			if (SaveBiTree(Trees.elem[i29].T, filename) == INFEASIBLE) { printf("二叉树不存在！\n"); }
			else { printf("二叉树元素已写入文件\n"); }
			getchar(); getchar(); break;
		case 30:
			int flag30, i30;
			char FileName30[30];
			printf("请输入文件名称：");
			scanf("%s", FileName30);
			printf("请选择：覆盖已有的二叉树（输入1）   集合内新建一个二叉树（输入2）\n");
			scanf("%d", &flag30);
			if (flag30 == 1) {
				printf("请输入需要覆盖的二叉树序号：");
				scanf("%d", &i30);
				if (i30 > Trees.length || i30 < 1) { printf("该二叉树不存在\n"); }
				else {
					i30--;
					DestroyBiTree(Trees.elem[i30].T);
					LoadBiTree(Trees.elem[i30].T, FileName30);
					printf("覆盖成功！\n");
				}
			}
			if (flag30 == 2) {
				if (LoadBiTree(Trees.elem[Trees.length].T, FileName30)) {
					char newname[30];
					printf("请输入新二叉树名称："); scanf("%s", newname);
					strcpy(Trees.elem[Trees.length].name, newname);
					Trees.length++;
					printf("二叉树读取成功，新二叉树为二叉树集合中第%d个\n", Trees.length);
				}
			}
			getchar(); getchar(); break;
		case 0:
			getchar();
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()