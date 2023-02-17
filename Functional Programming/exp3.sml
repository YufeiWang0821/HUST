(*1*)
(*编写函数listToTree: int list -> tree，将一个表转换成一棵平衡树。
提示：可调用split函数，split函数定义如下：
如果L非空，则存在L1, x, L2，满足：
	split L = (L1, x, L2) 	且 
	L = L1 @ x :: L2 		且 
	length(L1)和length(L2)差值小于1 *)
datatype tree = Empty | Node of tree*int*tree;
fun split[]=([],0,[])
|split[x]=([],x,[])
|split(x::L)=
    let val (L1,y,L2)=split L
    in 
        if length(L1)>length(L2) then (L1,x,y::L2)
        else (y::L1,x,L2) 
    end;
fun listToTree[]=Empty
|listToTree[x]=Node(Empty,x,Empty)
|listToTree L=
    let val (L1,y,L2)=split L
    in Node(listToTree L1,y,listToTree L2)
    end;
(*2*)
(* 编写函数revT: tree -> tree，对树进行反转，使trav(revT t) = reverse(trav t)。（trav为树的中序遍历函数）。
假设输入参数为一棵平衡二叉树，验证程序的正确性，并分析该函数的执行性能（work和span）。 *)
fun revT Empty=Empty
|revT(Node(t1,x,t2))=Node(revT t2,x,revT t1);
(*正确性验证：
1.当树为空时，所得到的也为空，满足trav(revT t) = reverse(trav t)
2.当树为Node(t1, v, t2)时，对与t1和t2，由于中序遍历的顺序为先遍历左子树、再遍历根结点、最后遍历右子树
故将这一顺序倒转即可满足trav(revT t) = reverse(trav t)，即遍历顺序为“右子树->根结点->左子树”
故该程序正确
该函数的work为O（n） span为O（d）*)
(*3*)
(*编写函数binarySearch: tree * int -> bool。当输出参数1为有序树时，如果树中包含值为参数2的节点，则返回true；否则返回false。
要求：程序中请使用函数Int.compare（系统提供），不要使用<, =, >。
datatype order = GREATER | EQUAL | LESS
case Int.compare(x1, x2) of
	GREATER => (* x1 > x2 *)
        | EQUAL => (* x1 = x2 *)
        | LESS => (* x1 < x2 *)
*)
fun binarySearch (Empty,_) = false
|binarySearch (Node(t1,x,t2),a) =
  case Int.compare(a,x) of
  LESS => binarySearch(t1,a)
  |EQUAL => true
  |GREATER => binarySearch(t2,a);
(*4*)
(*一棵minheap树定义为：
1. t is Empty;
2. t is a Node(L, x, R), where R, L are minheaps and values(L), value(R) >= x
	(value(T)函数用于获取树T的根节点的值）
编写函数treecompare, SwapDown 和heapify：
treecompare: tree * tree -> order
(* when given two trees, returns a value of type order, based on which tree has a larger
value at the root node *) 
SwapDown: tree -> tree
(* REQUIRES the subtrees of t are both minheaps
* ENSURES swapDown(t) = if t is Empty or all of t’s immediate children are empty then
* just return t, otherwise returns a minheap which contains exactly the elements in t. *) 
heapify : tree -> tree 
(* given an arbitrary tree t, evaluates to a minheap with exactly the elements of t.  *)
分析SwapDown 和heapify两个函数的work和span。 *)
fun treecompare (Empty, Empty) = EQUAL
|treecompare(Empty, _ ) = LESS
|treecompare( _ , Empty) = GREATER
|treecompare(Node(_,a,_),Node(_,b,_ ))=Int.compare(a,b);

fun rootcompare (x,Empty) = (x ,Empty,false)
|rootcompare(x,Node(l,y,r)) = 
case Int.compare(x, y) 
of
GREATER=>(y,Node(l,x,r),true)
|_=>(x,Node(l,y,r),false);

fun SwapDown Empty = Empty
|SwapDown(Node(Empty,x,Empty))=Node(Empty,x,Empty)
|SwapDown(Node(l,x,r))= 
let
val (root1,left,leftcon)=rootcompare(x,l)
val (root2,right,rightcon)=rootcompare(x,r)
in
if leftcon then SwapDown(Node(SwapDown left,root1,r))else 
if rightcon then SwapDown(Node(l,root2,SwapDown right))else 
Node(l,x,r)
end;
(*该函数的work为O（log n） span为O（d）*)
fun heapify Empty = Empty
|heapify (Node (l,x,r)) = SwapDown(Node(heapify l,x,heapify r));
(*该函数的work为O（n*log n） span为O（d^2）*)