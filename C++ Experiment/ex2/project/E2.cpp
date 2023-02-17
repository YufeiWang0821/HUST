#include"E2.h"
QUEUE::QUEUE(int m) : elems(new int[m]), max(m) {
    head = 0, tail = 0;
}

QUEUE::QUEUE(const QUEUE& s) : elems(new int[s.max]), max(s.max) {
    head = s.head, tail = s.tail;
    for (int i = 0; i < s.max; ++i) { elems[i] = s.elems[i]; }
}

QUEUE::QUEUE(QUEUE&& q)noexcept : elems(q.elems), max(q.max) {
    head = q.head, tail = q.tail;
    *(int**)&(q.elems) = nullptr;
    *(int*)&(q.max) = 0, q.head = 0, q.tail = 0;
}

QUEUE::operator int() const noexcept {
    int ans = head < tail ? tail - head : (head == tail ? 0 : max - head + tail);
    return ans;
}

int QUEUE::size() const noexcept {
    return max;
}

QUEUE& QUEUE::operator<<(int e) {
    if ((tail + 1) % max == head) { throw("QUEUE is full!"); }
    *(elems +tail) = e;
    tail++;
    if (tail == max) { tail = 0; }
    return *this;
}

QUEUE& QUEUE::operator>>(int& e) {
    if (head == tail) { throw("QUEUE is empty!"); }
    e = *(elems + head);
    head++;
    if (head == max) {head = 0; }
    return *this;
}

QUEUE& QUEUE::operator=(const QUEUE& q){
    if (this == &q) { return *this; }
    if (elems) { delete elems; }
    *(int**)&(elems) = new int[q.max];
    *(int*)&(max) = q.max;
    for (int i = 0; i < q.max; i++) { elems[i] = q.elems[i]; }
    head = q.head, tail = q.tail;
    return *this;
}

QUEUE& QUEUE::operator=(QUEUE&& q)noexcept {
    if (this == &q) { return *this; }
    if (elems) { delete elems; }
    *(int**)&(elems) = q.elems;
    *(int*)&(max) = q.max;
    head = q.head, tail = q.tail;
    *(int**)&(q.elems) = nullptr;
    *(int*)&(q.max) = 0, q.head = 0, q.tail = 0;
    return *this;
}
char* QUEUE::print(char* s) const noexcept {
    int a = 0;
    a += sprintf(s, "%d", *(elems + head));
    for (int i = head + 1; i !=tail; i++) {
        if (i == max) { i = 0; }
        a += sprintf(s + a, ",%d", *(elems + i));
    }
    return s;
}

QUEUE::~QUEUE(){
    if(elems)delete elems;
    *(int**)&(elems) = nullptr;
    head = 0, tail = 0;
    *(int*)&(max) = 0;
}
