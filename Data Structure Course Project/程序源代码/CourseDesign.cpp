#include"def.h"
int op = 1, sudokuop, sudoku[9][9], answer[9][9], setted[9][9];
char filepath[100];
FILE* fp;
cnf S;

void main() {
	clock_t beg, end;
	while (op) {
		system("cls");
		printf("\n\n");
		printf("\t\t\t\t欢迎使用本系统，请选择要使用的功能：\n");
		printf("\t\t\t\t1.SAT问题  2.数独  0.退出\n");
		printf("\t\t\t\t\t选择：");
		scanf("%d", &op);
		switch (op) {
		case 0:
			getchar();
			break;
		case 1:
			printf("请输入CNF文件路径：\n");
			scanf("%s", filepath);
			fp = fopen(filepath, "r");
			if (!fp) {
				printf("打开文件失败\n");
				getchar(); getchar();
				break;
			}
			S = (cnf)malloc(sizeof(CNFG));
			S->root = NULL;
			S = OperateCNF(fp,S);
			//TraverseCNF(S);
			int flag;
			printf("\n请选择决策方式：\n");
			printf("1.选择出现的第一个文字  2.选择出现次数最多的文字\n\n");
			printf("选择：");
			scanf("%d", &flag);
			beg = clock();
			ans.state = DPLL(S,flag);
			end = clock();
			ans.time = end - beg;
			if (ans.state) {
				printf("该SAT是可满足的\n\n");
				for (int i = 1; i <= ans.num; i++) {					
					if (ans.LiteralState[i]) { printf("%5d ", ans.LiteralState[i] * i); }
					else { printf("%5d", i); }
					if (!(i % 15)) printf("\n");
				}
				printf("\n\n");
			}
			else { printf("该SAT是不可满足的\n\n"); }
			printf("DPLL算法用时为%d ms\n\n", ans.time);
			AnswerFile(filepath);
			getchar(); getchar();
			break;
		case 2:
			sudokuop = 1;
			while (sudokuop) {
				system("cls");
				printf("\n\n");
				printf("\t\t\t\t请选择要进行的操作：\n");
				printf("\t\t\t\t1.生成完整数独  2.试玩数独游戏  0.退出\n");
				printf("\t\t\t\t\t选择：");
				scanf("%d", &sudokuop);
				switch (sudokuop) {
				case 0:
					break;
				case 1:
					CreateSudoku(sudoku);
					PrintSudoku(sudoku);
					getchar(); getchar();
					break;
				case 2:
					CreateSudoku(sudoku);
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) { answer[i][j] = sudoku[i][j]; }
					}
					if (!DigHole(sudoku,49)) { printf("生成数独游戏失败\n"); }
					else {
						for (int i = 0; i < 9; i++) {
							for (int j = 0; j < 9; j++) { setted[i][j] = sudoku[i][j]; }
						}
						int playop = 1, m, n, k;
						while (playop) {
							system("cls");
							printf("\n\n");
							printf("\t\t\t\t数独游戏\n");
							PrintSudoku(sudoku);
							printf("\t\t\t\t请选择要进行的操作：\n");
							printf("\t\t\t1.填入     2.删除     0.查看答案\n\t\t\t\t     ");
							scanf("%d", &playop);
							switch (playop) {
							case 0:
								printf("\t\t\t\t答案如下\n");
								PrintSudoku(answer);
								break;
							case 1:
								printf("\t\t\t\t输入要填入的位置以及要填入的数字\n");
								printf("\t\t\t\t如3 5 7表示第3行第5列填入数字7\n\t\t\t\t     ");
								scanf("%d%d%d", &n, &m, &k);
								if (m >=1 && m <=9 && n>=1 && n <= 9&&k>=1&&k<=9){
									if (setted[n - 1][m - 1]) {
										printf("\t\t\t\t该位置无法填入\n");
										getchar(); getchar();
										continue;
									}
									if (!JudgeNumber(sudoku, n-1, m-1, k)) {
										printf("\t\t\t\t填入的数字不符合数独规则\n");
										getchar(); getchar();
										continue;
									}
									sudoku[n - 1][m - 1] = k;
									printf("\t\t\t\t填入成功\n");
									getchar(); getchar();
									continue;
								}
								else {
									printf("\t\t\t\t请输入正确的位置以及数字编号\n");
									getchar(); getchar();
									continue;
								}
							case 2:
								printf("\t\t\t\t输入要删除的位置\n");
								printf("\t\t\t\t如3 5表示第3行第5列删除数字\n\t\t\t\t     ");
								scanf("%d%d", &n, &m);
								if (m >= 1 && m <= 9 && n >= 1 && n <= 9) {
									if (setted[n-1][m-1]) {
										printf("\t\t\t\t该位置无法删除\n");
										getchar(); getchar();
										continue;
									}
									sudoku[n - 1][m - 1] = 0;
									printf("\t\t\t\t删除成功\n");
									getchar(); getchar();
									continue;
								}
								else {
									printf("\t\t\t\t请输入正确的位置以及数字编号\n");
									getchar(); getchar();
									continue;
								}
							default:
								printf("\t\t\t\t无该选项\n");
								getchar(); getchar();
								continue;
							}
						}
					}
					getchar(); getchar();
					break;
				default:
					printf("无该选项\n");
					getchar();
					break;
				}//end of switch
			}//end of while
			getchar();
			break;
		default:
			printf("无该选项\n");
			getchar(); getchar();
			break;
		}//end of switch
	}//end of while
	printf("\n\t\t\t\t使用结束 欢迎再次使用本系统\n\n");
}//end of main