#include"E4.h"
extern const char* TestMAT(int& s);
int main(int argc, char* argv[])
{
	int s = 0;
	const char* e = TestMAT(s);
	printf("按下任意键开始测试\n");
	getchar();
	printf("%s\n", e);
	getchar();
	return 0;
}
