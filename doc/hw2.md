<div class="cover" style="page-break-after:always;font-family:方正公文仿宋;width:100%;height:100%;border:none;margin: 0 auto;text-align:center;">
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:10%;">
        </br>
        <img src="https://upload.wikimedia.org/wikipedia/commons/f/f5/Peking_University_Name.svg" alt="校名" style="width:60%;"/>
    </div>
    </br></br></br>
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:40%;">
        <img src="https://upload.wikimedia.org/wikipedia/commons/f/f4/Peking_University_seal.svg" alt="校徽" style="width:80%;"/>
	</div>
    </br></br></br></br>
    <span style="font-family:华文黑体Bold;text-align:center;font-size:20pt;margin: 10pt auto;line-height:30pt;">芯片设计自动化与智能优化</span>
    <p style="text-align:center;font-size:14pt;margin: 0 auto">2020-2021年春季学期 </p>
    </br>
    </br>
    <table style="border:none;text-align:center;width:72%;font-family:仿宋;font-size:14px; margin: 0 auto;">
    <tbody style="font-family:方正公文仿宋;font-size:12pt;">
        	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">题　　目</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 第二次作业: Physical Design</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">授课教师</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">林亦波 </td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">姓　　名</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 麦景</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">学　　号</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">1700012751 </td>     </tr>
    </tbody>              
    </table>
</div>



<!-- 注释语句：导出PDF时会在这里分页 -->

# 第二次作业: Physical Design



## Problem #1

### (a)

以下通过表格的方式列出每次step交换俩俩点的gain. 由于对称性, 这里只列出表格的上半部分. 并且注意到如果当前两个点已经在同一个partition内, 那么交换两个点是没有意义的, 这里直接不考虑这种情况, 初始cut size为$\frac{7}{3}$

**Step 1**:

$P_1 = \{a, b, c, d \vert e, f, g, h\}$

|      |  a    |   b   |  c    |    d  |   e   |   f |   g   |  h   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|   a   |  -    |  -    |    -  |   -   | $\frac{1}{2}$ | $-\frac{1}{2}$ | $-\frac{1}{2}$ | $-\frac{1}{2}$ |
|   b   |   -   |   -   |    -  |    -  | $-\frac{1}{2}$ | $-\frac{3}{2}$ | $\frac{1}{2}$ | $-\frac{3}{2}$ |
|   c   |      |      |   -   |  -   | 0 | $ - \frac{3}{2}$ | $\frac{1}{2}$ | $-1$ |
|    d  |      |      |      |   -   | 0 | $\frac{1}{2}$ | $\frac{3}{2}$ | $-1$ |
|    e  |      |      |      |      |  -    |   -   |   -   |  -    |
|   f   |      |      |      |      |      |   -   |   -   |  -    |
|   g   |      |      |      |      |      |      |   -   |   -   |
|   h   |      |      |      |      |      |      |      |   -   |

所以在step 1中调换点d和点g, 获得增益 $\frac{3}{2}$, 且在接下来的步骤中固定点d和点g, 此时的cut size为$\frac{7}{3}-\frac{3}{2}=\frac{5}{6}$.

**Step 2**:

$P_2 = \{a, b, c, g \vert d, e, f, h\}$

|      |  a    |   b   |  c    |    d  |   e   |   f |   g   |  h   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|   a   |  -    |  -    |    -  |   -   | -1   | -1 | - | -2 |
|   b   |   -   |   -   |    -  |    -  | 0 | -1 | - | $ - \frac{5}{3}$ |
|   c   |      |      |   -   |  -   | -1 | $ - \frac{3}{2}$ | - | -2 |
|    d  |      |      |      |   -   | - | - | - | - |
|    e  |      |      |      |      |  -    |   -   |   -   |  -    |
|   f   |      |      |      |      |      |   -   |   -   |  -    |
|   g   |      |      |      |      |      |      |   -   |   -   |
|   h   |      |      |      |      |      |      |      |   -   |

此时不存在正增益, 该pass结束, 此时的cut size为$\frac{5}{6}$.

### (b)

在该无权图中, 初始的cut size为4.

**Step 1**:

$P_1 = \{a, b, c, d \vert e, f, g, h\}$

此时移动cell的gain为:

|   cell   |   a   |   b   |   c   |  d    |    e  |   f   |   g   |   h   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|   gain  | 0 | -1 | 0 | 1 | 0 | -1 | 1 | -1 |

故在step 1中移动点d, 移动后cut size为3, 在接下来的步骤中固定d.

**Step 2**:

注意到由于partition大小限制, 此时点a,b,c不能被移动. 此时移动cell的gain为:

|   cell   |   a   |   b   |   c   |  d    |    e  |   f   |   g   |   h   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|   gain  | - | - | - | - | -1 | -1 | 1 | -2 |

故在step 2中移动点g, 移动后cut size为2, 在接下来的步骤中固定点g.

**Step 3**:

此时移动cell的gain为:

|   cell   |   a   |   b   |   c   |  d    |    e  |   f   |   g   |   h   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|   gain  | -2 | 0 | -1 | - | -2 | -1 | - | -2 |

此时不存在正增益, 该pass结束, 此时cut size为2.

### (c)

**Edge Coarsening(EC)**算法过程如下:

(1) visit a: a的邻居包括b, c, g, 连接权重分别为$\frac{1}{2}$, $\frac{1}{2}$和1, 故合并a和g, 得到聚类$C_1=\{a, g\}$, 并标记a和g.

(2) visit b: a的未标记邻居包括c, d, e和f, 连接权重分别为$\frac{1}{2}$, $\frac{1}{3}$, $\frac{1}{3}$和$\frac{1}{3}$, 按照字典序应合并b和c, 得到聚类$C_2=\{b,c\}$, 并标记b和c.

(3) visit c: c已被标记, 跳过.

(4) visit d: d的未标记邻居包括e, f和h, 连接权重分别为$\frac{5}{6}$, $\frac{1}{3}$和$\frac{1}{2}$, 故合并d和e, 得到聚类$C_3=\{d, e\}$, 并标记d和e.

(5) visit e: e已经被标记, 跳过.

(6) visit f: f的未标记邻居只有h, 故合并f和h, 得到聚类$C_4=\{f, h\}$.

故最终聚类结果为$C_1=\{a,g\}$, $C_2=\{b,c\}$, $C_3=\{d,e\}$和$C_4=\{f,h\}$.

**Modified Hyperedge Coarsening(MHC)**算法过程如下:

根据权重排序后网表的顺序为$n_6$, $n_4$,$n_1$, $n_3$, $n_5$和$n_2$.

(1) visit $n_6$: 合并f和h, 得到聚类$C_1=\{f, h\}$并标记f和h.

(2) visit $n_4$: 合并a和g, 得到聚类$C_2=\{a, g\}$并标记点a和g.

(3) visit $n_1=\{a,b,c\}$: 由于a已经被标记, 合并b和c, 得到聚类$C_3=\{b,c\}$并标记点b和c.

(4) visit $n_3=\{c,f,g\}$: c,f,g都已经被标记.

(5) visit $n_5=\{d, e, h\}$: 由于h已经被标记, 合并d和e, 得到聚类$C_4=\{d, e\}$, 并标记点d和e. 

(6) 此时所有点都已合并, 算法提前结束.

故最终聚类结果为$C_1=\{f,h\}$, $C_2=\{a,g\}$, $C_3=\{b,c\}$和$C_4=\{d,e\}$.



## Problem #4

### (a)

不妨仅考虑x轴方向, 设网表$e$所连接的所有pin的横坐标为$x_1, x_2, \ldots, x_n$. 那么有:
$$
HPWL^x(e) = \max_{1 \leq i \leq n} x_i - \min_{1 \leq i \leq n} x_i
$$
显然$\max x_i$和$ - \min x_i$都是凸函数, 故$HPWL^x(e)$是凸函数.

### (b)

$$
f(\mathbf{x}, \mathbf{y}) = \sum_{i \neq j}(x_i-x_j)^2+(y_i-y_j)^2
$$

对于其中任意一个二次项$(x_i-x_j)^2$或$(y_i-y_j)^2$, 其都是凸函数, 故凸函数的和仍是凸函数.

### (c)

log-sum-exp函数为: $f(x)=\log \sum_{k=1}^{n} \exp x_{k}$, 其二阶导为:
$$
\nabla^{2} f(x)=\frac{1}{\mathbf{1}^{T} z} \operatorname{diag}(z)-\frac{1}{\left(\mathbf{1}^{T} z\right)^{2}} z z^{T}
$$
为了证明$\nabla^{2} f(x) \succeq 0$, 仅需证明对于所有的$v$, 有$v^{T} \nabla^{2} f(x) v \geq 0$:
$$
v^{T} \nabla^{2} f(x) v=\frac{\left(\sum_{k} z_{k} v_{k}^{2}\right)\left(\sum_{k} z_{k}\right)-\left(\sum_{k} v_{k} z_{k}\right)^{2}}{\left(\sum_{k} z_{k}\right)^{2}} \geq 0
$$
利用Cauchy-Schwarz Inequality容易证明上式不等号成立.
