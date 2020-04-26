# 四子棋

## 个人信息

杨雅儒，计85，2017011071。

## 实验环境

Windows10, Visual Studio Community 2019

## 算法设计

### 概览

整体上采用 UCT，即信心上限树算法。并进行了若干优化：

* **采用结点内存池机制**，避免多次 new 和 delete；

* **加入懒惰式垃圾回收机制**；
* **加入内存溢出判定，增强程序鲁棒性；**
* **加入博弈论的必胜必败态计算，并据此优化了 treePolicy 和 bestChild 函数**；
* **加入必胜必败的步数考虑，必胜时尽早获胜，必败时尽可能拖延**——这是因为“必败”建立在对方足够聪明的情况，故拖延政策可以让对方在不够聪明的情况下走错路的概率更大；
* **加入各类标记，避免重复计算，降低时间常数**。

### UCT 算法框架

同课程所介绍的框架**类似但不同**，下面为了展示，将课程所讲框架粘贴过来：

```
    function UCTSEARCH(S_0)
              以状态s_0创建根节点v_0;
            while 尚未用完计算时长 do:
                    v_l←TREEPOLICY(v_0);
                    ∆←DEFAULTPOLICY(s(v_l));
                    BACKUP(v_l,∆);
             end while
             return a(BESTCHILD(v_0,0));

    function TREEPOLICY(v)
            while 节点v不是终止节点 do:
                    if 节点v是可扩展的 then:
                            return EXPAND(v)
                    else:
                            v← BESTCHILD(v,c)
                 return v

    function EXPAND(v)
            选择行动a∈A(state(v))中尚未选择过的行动
                    向节点v添加子节点v'，使得s(v')= f(s(v),a),a(v')=a
                     return v'

    function BESTCHILD(v,c)
             return 〖argmax〗_(v'∈children of v) ((Q(v'))/(N(v'))+c√((2ln(N(v)))/(N(v'))))

    function DEFAULTPOLICY(s)
             while s不是终止状态 do:
                     以等概率选择行动a∈A(s)
                      s←f(s,a)
             return 状态s的收益

     function BACKUP(v,Δ)
               while v≠NULL do:
                       N(v)←N(v)+1
                       Q(v)←Q(v)+∆
                       ∆←1-∆
                       v←v的父节点

```

### 内存策略——结点内存池机制

采用一开始开辟一个较大的静态结点数组的方式，这个数组作为结点内存池，

### 改进——懒惰式垃圾回收机制

通过垃圾回收将不可能再走到的子树根节点加入回收栈，当内存池耗尽时，每次需要新节点





* 结点最后一次扩展方向的记录标记，避免重复枚举，降低复杂度；
* 加入结点扩展完毕标记，避免重复判断，降低复杂度。

## 对抗结果

|      | 胜数 | 负数 | 平数 | 胜率  |
| ---- | ---- | ---- | ---- | ----- |
| 先手 | 47   | 3    | 0    | 94.0% |
| 后手 | 49   | 1    | 0    | 98.0% |
| 总计 | 96   | 4    | 0    | 96.0% |