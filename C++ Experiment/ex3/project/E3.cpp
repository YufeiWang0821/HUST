#include"E3.h"

STACK::STACK(int m):QUEUE(m),q(m){}

STACK::STACK(const STACK& s):QUEUE((QUEUE&)s),q(s.q) {}

STACK::STACK(STACK&& s)noexcept:QUEUE((QUEUE&&)s),q((QUEUE&&)s.q) {}

int STACK::size()const noexcept {
    return q.size() * 2;
}

STACK::operator int() const noexcept {
    return (int)q + QUEUE::operator int();
}

STACK& STACK::operator<<(int e) {
    int m = q.size();
    if ((int)(*this) == 2*m - 2) { throw("STACK is full!"); }
    if (QUEUE::operator int() != m - 1) { QUEUE::operator<<(e); }
    else { q << e; }
    return *this;
}

STACK& STACK::operator>>(int& e) {
    int n = this->operator int(), m = q.size(), temp1, temp2;
    if (!n) { throw("STACK is empty!"); }
    if (n < m) {
        n--;
        while (n) {
            QUEUE::operator>>(temp1);
            QUEUE::operator<<(temp1);
            n--;
        }
        QUEUE::operator>>(e);
    }
    else if (n == m ) {
        q >> e;
    }
    else {
        n--;
        while (n) {
            QUEUE::operator>>(temp1);
            q >> temp2;
            QUEUE::operator<<(temp2);
            q << temp1;
            n--;
        }
        QUEUE::operator>>(e);
        q >> temp2;
        QUEUE::operator<<(temp2);
    }
    return *this;
}

STACK& STACK::operator=(const STACK& s) {
    QUEUE::operator=((QUEUE&)s);
    q = (s.q);
    return *this;
}

STACK& STACK::operator=(STACK&& s)noexcept {
    if (&s == this) { return *this; }
    QUEUE::operator=((QUEUE&&)s);
    q = (QUEUE &&)s.q;
    
    return *this;
}

char* STACK::print(char* b)const noexcept {
    QUEUE::print(b);
    if (int(q)) {
        int l = strlen(b);
        l += sprintf(b + l, ",");
        q.print(b + l );
    }
    return b;
}

STACK::~STACK()noexcept {
    
}

extern const char* TestSTACK(int& s);
int main() {
    int s = 0;
    const char* e = TestSTACK(s);
    printf("按下任意键开始测试\n");
    getchar();
    printf("%s\n", e);
    getchar();
    return 0;
}