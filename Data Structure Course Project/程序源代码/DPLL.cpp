//DPLL.cpp：此文件是对于DPLL算法的实现
#include"def.h"
int count_pos[MaxLitNum + 1], count_neg[MaxLitNum + 1];

status LiteralExist(cla c, int l) {//判断子句c是否包含文字l
	int flag = 0;
	if (!c) { return 0; }
	lit temp = c->first_l;
	while (temp) {
		if (temp->sta == l) { flag = 1; break; }
		else { temp = temp->next; }
	}
	return flag;
}

cnf DeleteLiteral(cnf S, int l) {//单子句规则化简S
	//以下是删除包含l的子句
	cla ctemp, cnext;
	lit ltemp, lnext;
	while (S->root && LiteralExist(S->root, l)) {
		ctemp = S->root;
		S->root = S->root->next;
		ltemp = ctemp->first_l;
		while (ltemp) {
			lnext = ltemp->next;
			free(ltemp);
			ltemp = lnext;
		}
		free(ctemp);
		S->c_num--;
	}
	ctemp = S->root;
	while (ctemp) {
		while (ctemp->next) {
			if (LiteralExist(ctemp->next, l)) {
				cnext = ctemp->next;
				ltemp = cnext->first_l;
				while (ltemp) {
					lnext = ltemp->next;
					free(ltemp);
					ltemp = lnext;
				}
				ctemp->next = cnext->next;
				free(cnext);
				S->c_num--;
			}
			else { ctemp = ctemp->next; }
		}
		ctemp = ctemp->next;
	}
	//以下是删除子句中的-l(待修改)
	ctemp = S->root;
	while (ctemp) {
		while (ctemp->first_l && (ctemp->first_l->sta + l == 0)) {
			ltemp = ctemp->first_l;
			ctemp->first_l = ctemp->first_l->next;
			free(ltemp);
		}
		ltemp = ctemp->first_l;
		while (ltemp) {
			while (ltemp->next) {
				if (ltemp->next->sta + l == 0) {
					lnext = ltemp->next;
					ltemp->next = lnext->next;
					free(lnext);
				}
				else { ltemp = ltemp->next; }
			}
			ltemp = ltemp->next;
		}
		ctemp = ctemp->next;
	}
	S->l_num--;
	return S;
}

status EmptyClauseExist(cnf S) {//判断是否存在空子句
	cla p = S->root;
	while (p) {
		if (!p->first_l) { return OK; }
		p = p->next;
	}
	return NOPE;
}

cla SingleClause(cnf S) {//判断是否存在单子句并返回第一个单子句
	cla p = S->root;
	lit q;
	while (p) {
		q = p->first_l;
		if (q) {
			if (!q->next) { return p; }
		}
		p = p->next;
	}
	return NULL;
}

int SelectClause(cnf S) {//寻找S中出现次数最多的一个文字并返回它的编号
	memset(count_pos, 0, MaxLitNum + 1);
	memset(count_neg, 0, MaxLitNum + 1);
	cla ctemp = S->root;
	lit ltemp;
	while (ctemp) {
		ltemp = ctemp->first_l;
		while (ltemp) {
			if (ltemp->sta > 0) { count_pos[ltemp->sta]++; }
			else { count_neg[abs(ltemp->sta)]++; }
			ltemp = ltemp->next;
		}
		ctemp = ctemp->next;
	}
	int maxi = 0, max = 0, flag=1;
	for (int i = 1; i <= ans.num; i++) {
		if (count_pos[i] > max) {
			max = count_pos[i];
			maxi = i;
			flag = 1;
		}
	}
	for (int i = 1; i <= ans.num; i++) {
		if (count_neg[i] > max) {
			max = count_neg[i];
			maxi = i;
			flag = 0;
		}
	}
	return flag ? maxi : -maxi;
}

cnf Sew(cnf S, int i) {//将数据为i的单子句作为新cnf的根，后续复制S，返回新cnf
	cnf news = (cnf)malloc(sizeof(CNFG));
	cla v = (cla)malloc(sizeof(CNode));
	lit u = (lit)malloc(sizeof(LNode));
	v->first_l = u; v->next = NULL;
	u->sta = i; u->next = NULL;
	news->root = v;
	news->c_num = S->c_num + 1;
	news->l_num = S->l_num;
	cla stemp = S->root, newtemp = news->root, cnext;
	lit slit, nlit, lnext;
	while (stemp) {
		cnext = (cla)malloc(sizeof(CNode));
		newtemp->next = cnext;
		slit = stemp->first_l;
		cnext->next = NULL;
		if (slit) {
			nlit = (lit)malloc(sizeof(LNode));
			cnext->first_l = nlit;
			nlit->sta = slit->sta; nlit->next = NULL;
			slit = slit->next;
			while (slit) {
				lnext = (lit)malloc(sizeof(LNode));
				lnext->next = NULL; lnext->sta = slit->sta;
				nlit->next = lnext;
				nlit = lnext;
				slit = slit->next;
			}
		}
		else { cnext->first_l = NULL; }
		newtemp = newtemp->next;
		stemp = stemp->next;
	}
	return news;
}

status FreeCNF(cnf S) {//销毁S
	cnf cntem = S;
	cla cltem, cnext;
	lit ltem, lnext;
	if (cntem) {
		cltem = cntem->root;
		if (cltem) {
			cnext = cltem->next;
			while (cltem && cnext) {
				ltem = cltem->first_l;
				if (ltem) {
					lnext = ltem->next;
					while (ltem && lnext) {
						free(ltem); ltem = lnext; lnext = lnext->next;
					}
					free(ltem);
				}
				free(cltem); cltem = cnext; cnext = cnext->next;
			}
			free(cltem);
		}
		free(cntem);
	}
	S = NULL;
	return OK;
}

status DPLL(cnf S,int flag) {
	int sc;
	cla sin = SingleClause(S);
	while (sin) {
		ans.LiteralState[abs(sin->first_l->sta)] = sin->first_l->sta > 0 ? pos : neg;
		S = DeleteLiteral(S, sin->first_l->sta);
		if (!S->root) {
			FreeCNF(S);
			return TRUE;
		}
		else if (EmptyClauseExist(S)) {
			FreeCNF(S);
			return FALSE;
		}
		sin = SingleClause(S);
	}//end of while
	if (flag == 1) { sc = S->root->first_l->sta; }
	else if (flag == 2) { sc = SelectClause(S); }
	cnf temp;
	temp = Sew(S, sc);
	if (DPLL(temp,flag)) {
		FreeCNF(S);
		return TRUE;
	}
	//FreeTree(temp);
	temp = (cnf)malloc(sizeof(CNFG));
	temp = Sew(S, -sc);
	FreeCNF(S);
	return DPLL(temp,flag);
}