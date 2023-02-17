(*1*)
(* 函数nrev和revAppend定义如下，都用于实现表的翻转。
fun nrev [ ] = [ ]
| nrev (x::L) = nrev(L) @ [x];
fun revAppend ([ ], acc) = acc
| revAppend(x::L, acc) = revAppend(L, x::acc);
这两个函数的执行过程有什么不同？计算它们的执行时间(Work)，并给出分析过程。 *)
(*
nrev函数是将首元素放在最后，层层递归得到反转后的列表，work=O（n）
revAppend函数是另外设置了一个名为acc的list作为翻转后的列表，属于尾递归，在最后结果就是acc，work=O（n）
*)

(*2*)
(* 斐波那契数列定义为：F0=0，F1=1，Fn=Fn-2+Fn-1。根据该定义，给出相应的递归函数：
fun fib 0 = 1
 | fib 1 = 1
   | fib n = fib(n-1) + fib(n-2)
该函数在求解过程中不断重复计算同样的子问题，所以性能较差。
而每个斐波那契数都是前面两个的和，可以增加函数itfib，通过元组来储存执行参数和中间结果，
从而在计算过程中减少计算开销。itfib函数定义为：
itfib: (int, int, int) -> int
函数fib可以修改为函数itfib的计算，进行性能优化：
fun fib(n) = itfib(n, 0, 1);
编写函数itfib的实现代码。 *)
fun itfib(n,a,b)= if n<=1 then a else itfib(n-1,b,a+b);

(*3*)
(* 二叉树中，从根节点到任意一个结点的访问路径都可以表述为0或1形成的有序序列（‘0’代表向左走，‘1’代表向右走）。
用空列表[ ]表示从根到根的路径。如二叉树（Node(Node(Empty, 42, Empty), 21, Empty)）中，
先序遍历五个结点（含空的叶子结点）的访问路径分别表示为[ ], [0], [0,0], [0,1]和[1]；
而[1,1]为不可达路径，因为按路径[1]访问的结点为根节点的右子树（Empty），已经到达路径尽头。
编写一个递归函数paths，要求：
a. 函数定义为：paths: ’a tree -> int list list
b. 该函数用于生成二叉树T中所有结点的路径列表。如上例中计算的结果为：[[ ], [0], [0,0], [0,1], [1]]。
c. 代码中除了map函数之外，不能使用其他任何帮助函数.*)
datatype tree = Empty | Node of tree*int*tree;
fun paths Empty=[[]]
|paths(Node(Empty,x,Empty))=[[],[0],[1]]
|paths(Node(t1,x,t2))=[]::(map(fn A=>0::A)(paths t1))@(map(fn A=>1::A)(paths t2));