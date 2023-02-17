(*1*)
fun all (your, base) =
case your of
0 => base
| _ => "are belong to us" :: all(your - 1, base);(*类型为fn:int * string list -> string list*)
fun funny (f, []) = 0
| funny (f, x::xs) = f(x, funny(f, xs));(*类型为fn:('a * int -> int) * 'a list -> int*)
(fn x => (fn y => x)) "Hello, World!";(*类型为fn:string->('a->string)*)
(*2*)
(* 用归纳法证明ins函数和isort函数的正确性
fun ins (x, [ ]) = [x]
    | ins (x, y::L) = case compare(x, y) of
		       GREATER => y::ins(x, L)
		    |        _ 	  => x::y::L
证明：
1.当有序表L长度为0，即为空表时，输出结果[x]为有序表
2.当有序表L长度不为0时，设其长度为k，对于长度小于k的有序表A：
	ins（x，y::A）将x与A的第一个元素y进行比较，
	若x<y则说明x比A的所有元素都要小，则成为A的第一个元素；
	否则说明说明x比A中的第一个元素大，应该与第二个元素比较，
	则y作为第一个元素，再次用该函数与后续有序表比较，此时有序表长度减一。
	一直进行至有序表长度为0或x<有序表第一个元素为止，最终得到的ins（x，A）也是有序表
3.所以ins(x, L) 为x::L的有序表,其中L的长度为k且为有序表
fun isort [ ] = [ ]
   |   isort (x::L) = ins (x, isort L)
证明：
1.当有序表L长度为0，即为空表时，输出结果即为空表，是有序表
2.当有序表L长度不为0时，设其长度为k，对于其第一个元素x有：
	去掉x后的有序表再次使用isort也为有序表，此时该有序表长度为1；
	对x和所得到的有序表使用ins函数将x插入后所得也是有序表
3.所以isort L所得到的是L的有序表
 *)
(*3*)
(* 分析下面菲波拉契函数的执行性能
fun fib n = if n<=2 then 1 else fib(n-1) + fib(n-2);
分析：由于要分别求fib(n-1)和fib(n-2)，故性能为W（n）is O（2^n）
fun fibber (0: int) : int * int = (1, 1)
  | fibber (n: int) : int * int =
      let val (x: int, y: int) = fibber (n-1)
      in (y, x + y)
      end
分析：由于对所有非负整数k，fib(2k) = fib(k)(2fib(k + 1) − fib(k))，fib(2k + 1) = fib(k + 1)2 + fib(k)2，故故性能为W（n）is O（n）
 *)
(*4*)
(* divisibleByThree : int -> bool 	*)
(* REQUIRES: n>=0				*)
(* ENSURES: divisibleByThree n evaluates to true if n is a multiple of 3 and to false otherwise *)
fun divisibleByThree(0:int):bool=true
|divisibleByThree(1:int):bool=false
|divisibleByThree(2:int):bool=false
|divisibleByThree n=if n>0 then
divisibleByThree(n-3)else divisibleByThree(n+3);
(*5*)
(* oddP : int -> bool 		*)
(* REQUIRES: n >= 0 		*)
(* ENSURES: oddP n evaluates to true iff n is odd. *)
fun oddP(0:int):bool=false
|oddP 1=true
|oddP n=oddP(n-2);
(*6*)
(* 编写函数 interleave: int list * int list -> int list
该函数能实现两个int list数据的合并，且两个list中的元素在结果中交替出现
直至其中一个int list数据结束，而另一个int list数据中的剩余元素则直接附加至结果数据的尾部。 *)
fun interleave([]:int list,L2:int list):int list=L2
|interleave(L1,[])=L1
|interleave(a::L1,b::L2)=a::b::interleave(L1,L2);
(*7*)
(* 编写函数reverse和reverse’，要求：
函数类型均为：int list->int list，功能均为实现输出表参数的逆序输出；
函数reverse不能借助任何帮助函数；函数reverse’可以借助帮助函数，时间复杂度为O(n)。 *)
fun reverse[]:int list=[]
|reverse[a]=[a]
|reverse(a::L)=reverse(L)@[a];
fun reverse'([],b)=b
|reverse'(a::L,b)=reverse'(L,a::b);
(*8*)
(* 给定一个数组A[1..n]，前缀和数组PrefixSum[1..n]定义为：	PrefixSum[i] = A[0]+A[1]+...+A[i-1]；
例如：PrefixSum [ ] = [ ]
	  PrefixSum [5,4,2] = [5, 9, 11]
	  PrefixSum [5,6,7,8] = [5,11,18,26] *)
fun add([],a)=[]
|add(b::L,a)=(a+b)::add(L,a);
fun PrefixSum[]=[]
|PrefixSum(a::L)= a::add(PrefixSum (L),a);

fun PrefixSumHelp([],a)=[]
|PrefixSumHelp(b::L,a)=(a+b)::PrefixSumHelp(L,a+b);
fun fastPrefixSum L=PrefixSumHelp(L,0);