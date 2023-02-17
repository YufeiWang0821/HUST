#include"E1.h"
void initQueue(Queue* const p, int m) {//初始化p指队列：最多申请m个元素
    *(int**)&(p->elems) = (int*)malloc(sizeof(int) * (m > 0 ? m : 0));
    *(int*)&(p->max) = m > 0 ? m : 0;
	p->head = 0;
	p->tail = 0;
    return;
}
void initQueue(Queue* const p, const Queue& s) {//用s深拷贝初始化p指队列
	*(int**)&(p->elems) = (int*)malloc(s.max * sizeof(int));
	*(int*)&(p->max) = s.max;
	for (int i = 0; i < s.max; i++) { *(p->elems + i) = *(s.elems + i); }
	p->head = s.head, p->tail = s.tail;
	return;
}
void initQueue(Queue* const p, Queue&& s) {//用s移动初始化p指队列
	*(int**)&(p->elems) = s.elems;
	*(int*)&(p->max) = s.max;
	p->head = s.head, p->tail = s.tail;
	*(int**)&(s.elems) =nullptr;
	*(int*)&(s.max) = 0, s.head = 0, s.tail = 0;
	return;
}
int  number(const Queue* const p) {//返回p指队列的实际元素个数
	int ans = p->head < p->tail ? p->tail - p->head : (p->head == p->tail ? 0 : p->max - p->head + p->tail);
	return ans;
}
int  size(const Queue* const p) {//返回p指队列申请的最大元素个数max
	return p->max;
}
Queue* const enter(Queue* const p, int e) {//将e入队列尾部，并返回p
	if ((p->tail + 1) % p->max == p->head) { throw("Queue is full!"); }
	*(p->elems + p->tail) = e;
	p->tail++;
	if (p->tail == p->max) { p->tail = 0; }
	return p;
}
Queue* const leave(Queue* const p, int& e) {//从队首出元素到e，并返回p
	if (p->head == p->tail) { throw("Queue is empty!"); }
	e = *(p->elems + p->head);
	p->head++;
	if (p->head == p->max) { p->head = 0; }
	return p;
}
Queue* const assign(Queue* const p, const Queue& q) {//深拷贝赋s给队列并返回p
	if (p == &q) { return p; }
	if (p->elems) { free(p->elems); }
	*(int**)&(p->elems) = (int*)malloc(q.max * sizeof(int));
	*(int*)&(p->max) = q.max;
	for (int i = 0; i < q.max; i++) { *(p->elems + i) = *(q.elems + i); }
	p->head = q.head, p->tail = q.tail;
	return p;
}
Queue* const assign(Queue* const p, Queue&& q) {//移动赋s给队列并返回p
	if (p == &q) { return p; }
	if (p->elems) { free(p->elems); }
	*(int**)&(p->elems) = q.elems;
	*(int*)&(p->max) = q.max;
	p->head = q.head, p->tail = q.tail;
	*(int*)&(q.max) = 0, q.head = 0, q.tail = 0;
	*(int**)&(q.elems) = nullptr;
	return p;
}
char* print(const Queue* const p, char* s) {//打印p指队列至s并返回s
	int a = 0;
	a+=sprintf(s, "%d", *(p->elems + p->head));
	for (int i = p->head+1; i != p->tail; i++) {
		if (i == p->max) { i = 0; }
		a += sprintf(s + a, ",%d", *(p->elems + i));
	}
	return s;
}
void destroyQueue(Queue* const p) {//销毁p指向的队列
	if(p->elems)free(p->elems);
	*(int**)&(p->elems) = nullptr;
	p->head = 0, p->tail = 0;
	*(int*)&(p->max) = 0;
	return;
}