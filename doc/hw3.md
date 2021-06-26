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
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 第三次作业: Timing Analysis</td>     </tr>
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

# 第三次作业: Timing Analysis



## Exercise#1: Elmore delay
在这次练习中, 给定一个RC-tree, 我们需要根据*Elmore Delay Model*来计算delay和impulse. 不妨设点集和边集分别为$V$和$E$, 点$v \in V$的电容为$C_v$, 边$e \in E$的电阻为$R_e$. 设RC-tree的树根为$root$, 点$v \in V$的直接子节点为$son_v$, 父亲节点为$fa_v$且父边为$fe_v$.

我们分四步来计算delay和impulse. 首先, 计算每个节点的load, 记为$L_v$, 即为节点$v$子树中所有节点的电容之和, 即
$$
L_v = \sum_{u \in Son_v}L_u + C_v
$$
然后, 我们计算每个节点的delay, 对于点$v$, 其delay $d_v$等于其到根节点$root$上所有点的load乘上父边的电阻之和,  即
$$
d_v = d_{fa_v} + R_{fe_v} L_v
$$
特别地, 根节点的delay恒为0. 为了计算impulse, 我们先计算一个中间变量$ld_v$, 其为点$v$所在子树中所有点的"电容$\times$延迟"之和,
$$
ld_v = \sum_{u \in son_v} ld_u + d_vC_v
$$
然后利用如下公式就能算出impulse $\hat{s}_v$,
$$
\beta_v = \beta_{fa_v} + R_{fe_v}ld_v
$$

$$
\hat{s}_v^2 = 2\beta_v - d_v^2 
$$

我们可以使用树形动态规划算法来求解, 其中计算$L_v$和$ld_v$时是自底向上的, 计算$d_v$和$\beta_v$时是自顶向下的, 时间复杂度均为$O(|V|)$.



## Exercise#2: Setup Slack & Hold Slack

练习2的任务是给定一个有向无环的timing graph, 计算每个FF处的setup slack和hold slack. 不妨设点集和边集分别为$V$和$E$, 边$e \in E$的最小最大延迟分别为$d^{early}_e$和$d^{late}_e$, 起点和终点分别为$from_e$和$to_e$. 我们可以可以计算每个节点的最早到达时间$AT^{early}_v$和最晚到达时间$AT^{late}_v$:
$$
AT^{early}_{v} = \min_{e \in prev_v} AT^{early}_{from_e} + d^{early}_e
$$

$$
AT^{late}_{v} = \max_{e \in prev_v} AT^{late}_{from_e} + d^{late}_e
$$

其中$prev_v$是所有连向点$v$的边的集合, 按照拓扑序我们可以以$O(|V|+|E|)$的时间复杂度求解.

设时钟周期为$T$, 对于一个FF器件$F$, 设其clock pin为点$F_{CK} \in V$, 其data pin为点$F_{D} \in V$其可接受的最小steup time和hold time分别为$t^{setup}_F$和$t^{hold}_F$, 那么其setup slack和hold slack为:
$$
slack_{F}^{steup} = T + AT^{early}_{F_{CK}} -t^{setup}_F - AT^{late}_{F_D}
$$

$$
slack_{F}^{hold} =  AT^{early}_{F_{D}} -t^{hold}_F - AT^{late}_{F_{CK}}
$$

求解setup slack和hold slack的时间复杂度为$P(|V|)$.

