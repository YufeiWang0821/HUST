#include"E2.h"

extern const char* TestQUEUE(int& s);
int main() {
    int s = 0;
    const char* e = TestQUEUE(s);
	printf("按下任意键开始测试\n");
	getchar();
	printf("%s\n", e);
	getchar();
    return 0;
}