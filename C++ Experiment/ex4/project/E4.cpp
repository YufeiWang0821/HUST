#include"E4.h"

template <typename T>
MAT<T>::MAT(int r, int c):e(new T[r*c]),r(r),c(c) {/*矩阵定义*/ }
template <typename T>
MAT<T>::MAT(const MAT& a):e(new T[a.r*a.c]),r(a.r),c(a.c) {//深拷贝构造
	for (int i = 0; i < a.r * a.c; i++) { e[i] = a.e[i]; }
}
template <typename T>
MAT<T>::MAT(MAT&& a)noexcept:e(a.e),r(a.r),c(a.c) {//移动构造
	*(T**)&(a.e) = nullptr;
	*(int*)&(a.c) = 0, * (int*)&(a.r) = 0;
}
template <typename T>
MAT<T>::~MAT()noexcept {
	if (e) { delete e; *(T**)&e = nullptr; }
	*(int*)&(c) = 0, * (int*)&(r) = 0;
}
template <typename T>
T*const MAT<T>::operator[ ](int r) {//取矩阵r行的第一个元素地址，r越界抛异常
	if (r < this->r&&r>=0) {
		return &e[(r)*(c)];
	}
	throw("!!");
}
template <typename T>
MAT<T> MAT<T>::operator+(const MAT& a)const {//矩阵加法，不能加抛异常
	if ((r != a.r) || (c != a.c)) { throw("！！"); }
	MAT ne(a.r, a.c);
	for (int i = 0; i < r*c; i++) { ne.e[i] = e[i] + a.e[i]; }
	return ne;
}
template <typename T>
MAT<T> MAT<T>::operator-(const MAT& a)const {//矩阵减法，不能减抛异常
	if ((r != a.r) || (c != a.c)) { throw("！！"); }
	MAT ne(a.r, a.c);
	for (int i = 0; i < r * c; i++) { ne.e[i] = e[i] - a.e[i]; }
	return ne;
}
template <typename T>
MAT<T> MAT<T>::operator*(const MAT& a)const {//矩阵乘法，不能乘抛异常
	if (c != a.r) { throw("!!"); }
	MAT ne(r, a.c);
	for (int i = 0; i < r ; i++) {
		for (int j = 0; j < a.c; j++) {
			ne.e[i * a.c + j] = 0;
			for (int k = 0; k < c; k++) { ne.e[i * a.c + j] += e[i * c + k] * a.e[k * a.c + j]; }
		}
	}
	return ne;
}
template <typename T>
MAT<T> MAT<T>::operator~()const {//矩阵转置
	MAT ne(c,r);
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			ne.e[i * r + j] = e[j * c + i];
		}
	}
	return ne;
}
template <typename T>
MAT<T>& MAT<T>::operator=(const MAT& a) {//深拷贝赋值运算
	if (this == &a) { return *this; }
	if (e) { delete e; }
	*(T**)&(e) = new T[r * c];
	*(int*)&(c) = a.c, * (int*)&(r) = a.r;
	for (int i = 0; i < a.r * a.c; i++) { e[i] = a.e[i]; }
	return *this;
}
template <typename T>
MAT<T>& MAT<T>::operator=(MAT&& a)noexcept {//移动赋值运算
	if (this == &a) { return *this; }
	if (e) { delete e; }
	*(T**)&(e) = a.e;
	*(int*)&(c) = a.c, * (int*)&(r) = a.r;
	*(T**)&(a.e) = nullptr;
	*(int*)&(a.c) = 0, * (int*)&(a.r) = 0;
	return *this;
}
template <typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a) {//“+=”运算
	return *this = *this + a;
}
template <typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a) {//“-=”运算
	return *this = *this - a;
}
template <typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a) {//“*=”运算
	return *this = *this * a;
}
//print输出至s并返回s：列用空格隔开，行用回车结束
template <typename T>
char* MAT<T>::print(char* s)const noexcept {
	int a = 0;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c-1; j++) {
			a += sprintf(s+a, "%6lld ", e[i * c + j]);
		}
		a += sprintf(s+a, "%6lld\n", e[(i+1)*c - 1]);
	}
	return s;
}