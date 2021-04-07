# VLSI-CAD Homework #1
麦景 1700012751

---

关于编译和运行本代码的指示见`README.md`.

```bash

git submodule update
mkdir build && cd build
cmake ..
make
```

## Problem #1

`tautology checking` 问题的定义为给定一个单输出电路, 问是否对于任意输入, 输出恒定为假. 这个问题的一个常用的解法是构造Binary Decision Diagram(BDD), 并判断压缩后的Reduced Order Binary Decision Diagram(ROBDD)是否与输出恒为假的电路同构.  但是由于此问题本身是一个NP-hard问题, 因此目前不存在通用算法把任意BDD化简到最简形式. 一个通常的做法是在BDD内部进行遍历, 只需判断是否存在到达结果为真的路径即可. 同时, 注意到输入文件是`cube list`格式, 这实际上给出了一个特殊的Sum of Product(SOP)形式的电路; 其中对于每个product, 我们可以非常简单地构造其ROBDD; 构造完所有product的ROBDD后, 可以同时对所有的product对应的ROBDD进行OR操作的合并, 关键部分的伪代码如下, 其对应于文件`bdd.cpp`中的函数`BDDOperator::OR`:

```c++
BDD op(BDD g, BDD h) {
	if (g is a leaf or h is a leaf) // termination condition:
																  // either g = 0 or 1, or h = 0 or 1
		return proper BDD;
	var x = min(root(g), root(h)) // get the lowest order 
  var BDD fLo = op(negCofBDD(g, x), negCofBDD(h, x)); 
  BDD fHi = op(posCofBDD(g, x), posCofBDD(h, x)); 
  return combineBDD(x, fLo, fHi);
}
```

假设变量个数为$n$​, 在最坏情况下, 我们需要构造的BDD的节点数为$O(2^n)$, 最坏时空复杂度均为$O(2^n)$.

在编译完成本代码后, 输入`make run_autology`即可运行给出的所有benchmark, 其运行结果为:

| Benchmarks | 🤔 Is it tautology? |
| :--------: | :----------------: |
|  V20_c77   |         No         |
|   v2_c54   |        Yes         |
|  v17_c58   |         No         |
|  v11_c36   |         No         |
|   v5_c10   |         No         |
|  v19_c59   |         No         |
|  v12_c92   |         No         |
|  v16_c19   |         No         |
|   v3_c79   |        Yes         |
|  v13_c24   |         No         |
|            |                    |

## Problem #2

判断两个电路是否是完全等价是逻辑综合中的常见问题. 对于这两个电路, 把其输出异或后, 可以得到一个新的电路, 若这个电路的输出恒为假, 即是否是tautology, 则可判断这两个电路是完全等价的. 其中判断电路是否是tautology, 除了第一题中的使用BDD的做法, 我们还可以使用SAT solver.

[ABC][https://github.com/vlab-cs-ucsb/ABC]是一个在VLSI领域常用的SAT solver. 其使用了`MiniSAT` 来求解SAT 问题. ABC常用的命令包括`miter`, `sat`(或 `dsat`), `write_cnf`等, 更具体的操作指令可见[此](http://vlsicad.eecs.umich.edu/BK/Slots/cache/www-cad.eecs.berkeley.edu/~alanmi/abc/). 其中在本问题中, 对于输入的两个BLIF格式的文件`f1.blif`和`f2.blif`, 我们可以使用如下命令来判断:

```bash
abc 01> miter f1.blif f2.blif
abc 02> strash
abc 03> write_cnf f12_miter.cnf (optional)
abc 03> dsat
UNSATISFIABLE Time = 0.02 sec
```

其中`UNSATISFIABLE`表示该SAT问题被判定为“不可满足”, 即输出恒为假, 这对应于我们给出的两个电路是完全等价的; 与之对应, 若SAT问题被判定为“可满足”, 这对应于我们给出的两个电路不是完全等价的.

在编译完成本代码后, 运行`make run_equivalence`即可测试. 给出的benchmark的结果为:

| Benchmarks |     Equivalence     |
| :--------: | :-----------------: |
|   router   | {1, 3, 5} {2,4, 6}  |
|  priority  | {1, 3, 4} {2, 5, 6} |
|    max     | {1, 2, 6} {3, 4, 5} |
|   cavlc    | {1, 2, 5} {3, 4, 6} |
| Int2float  | {1, 2, 3} {4, 5, 6} |
|            |                     |

## Problem #3

`technology mapping`问题是指把一个电路中的逻辑关系映射成standard cell library中, 使得该逻辑电路可以被映射到实际的物理器件上. 其中在本问题中, 允许的standard cell library为{INV, NAND2, NOR2, AOI21, AOI22}, 目标是优化使用尽可能少的cell数量和尽可能小的深度. 

如果该网表恰好是树形机构, 即每个逻辑门的fan out个数至多为一, 我们可以使用树形动态规划来求解这个问题. 注意到INV这个standard cell是不可能在最终的方案中直接相连(否则我们一定可以删去一个并构造出更优的解), 对于原电路中每个节点, 我们分别设置两个动态规划的状态, 分别表示如果等价表示该子树输出的, 以及该子树输出的取反, 至少需要多少个cell, 以及cell个数最少时至少需要多少深度. 假设节点个数为$n$, 此动态规划的时间和空间复杂度均为$O(n)$.

注意到实际问题中一个节点fan out可能大于一, 这时候一个常见的tradeoff是划分成多棵树然后分别进行动态规划, 这时候其实不能保证得出结果的最优性的, 本代码也采取了类似的做法. 其中此做法还可能导致如下两个问题:

- 划分为多棵树之后, 实际上各棵树之间重叠部分较多, 体现在动态规划状态的值上是其为指数增长的, 因此代码里使用了`long double`来存储.
- 如果如果在另一棵树中使用`AOI21`, `AOI22`等standard cell, 但在另一棵树树中使用了其内部节点, 这时候其实在生成最终方案的时候会有部分电路无法被重用, 因此在本代码中规定如果使用`AOI21`, `AOI22`等standard cell, 其内部节点的fan out个数只能为1.

编译完成本代码后, 运行`make run_techmap 1> /dev/null` 可以得到各benchmark的生成方案的统计结果; `ctest -R techmap`可以使用上题所述的ABC solver验证输出方案于原方案的等价性. 注意由于使用了`AOI21`, `AOI22`等standard cell, 使用ABC solver判定等价性在用时过长, 因此建议在运行`ctest`时设置timeout. 在[The EPFL Combinational Benchmark Suite](https://github.com/lsils/benchmarks)中运行结果的统计如下, 其中other包括常数输入, 恒等逻辑门等.

| Benchmarks | # of gates | INV   | NAND2 | NOR2  | AOI21 | AOI22 | Others | Max depth |
| ---------- | ---------- | ----- | ----- | ----- | ----- | ----- | ------ | --------- |
| dec        | 604        | 300   | 292   | 12    | 0     | 0     | 0      | 6         |
| voter      | 16240      | 5595  | 104   | 8328  | 1313  | 900   | 0      | 103       |
| router     | 390        | 163   | 67    | 87    | 35    | 11    | 27     | 87        |
| priority   | 1167       | 497   | 15    | 451   | 100   | 104   | 0      | 249       |
| i2c        | 1470       | 490   | 205   | 484   | 175   | 101   | 15     | 21        |
| bar        | 2831       | 1159  | 514   | 262   | 128   | 768   | 0      | 16        |
| hyp        | 241653     | 73615 | 31879 | 98045 | 9251  | 8183  | 0      | 32482     |
| mem_ctrl   | 47327      | 16712 | 1791  | 13918 | 12766 | 1866  | 1      | 134       |
| int2float  | 257        | 81    | 31    | 82    | 42    | 21    | 0      | 17        |
| adder      | 1151       | 388   | 128   | 506   | 1     | 1     | 0      | 256       |
| sin        | 6651       | 2595  | 724   | 2333  | 638   | 336   | 0      | 263       |
| div        | 73842      | 18167 | 547   | 53571 | 1541  | 16    | 0      | 4403      |
| arbiter    | 12413      | 6078  | 63    | 768   | 5504  | 0     | 0      | 87        |
| log2       | 36959      | 11901 | 3649  | 15646 | 4524  | 1239  | 0      | 489       |
| max        | 2616       | 1176  | 72    | 389   | 533   | 446   | 0      | 295       |
| multiplier | 24459      | 7144  | 102   | 9262  | 6155  | 1796  | 9      | 288       |
| sqrt       | 28028      | 7237  | 307   | 16665 | 2811  | 8     | 9      | 6054      |
| square     | 18533      | 4967  | 1861  | 8370  | 1746  | 1587  | 2      | 254       |
| cavlc      | 674        | 192   | 40    | 274   | 125   | 43    | 9      | 21        |
| ctrl       | 183        | 56    | 7     | 78    | 34    | 7     | 1      | 11        |

