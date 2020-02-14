---
title: C++写AI五子棋
date: 2020-02-13 21:42:04
tags: 技术

---

# 第一天

![1581601430068](1581601430068.png)

<!--more-->

刚学完C++有点心血来潮，想再挑战一下AI版五子棋的高峰。

## 主要成果

1. 这次采用类继承的方法定义了棋盘和所有成员函数，整体代码比以前用C写的规范很多。

2. 正常输出棋盘，实现双人对局功能。
3. AI方面完成评估函数，完成了对五连、活四、冲四、活三的模型搭建，并且对四种模型赋值。为AI上手做好准备。

![1581601544134](1581601544134.png)

## 遇到问题

1. 顶层设计有偏差。这次有两个函数checkSame和sameSum在顶层设计一开始就遗漏了传入参数，在后期修改的时候比较麻烦。这两个函数都需要把row和col作为参数，因为在统计相同棋子数量时，传入的是被评估点的位置坐标，而不是棋盘类中private定义的值。所以所有需要传入其他点坐标的函数，都需要传入参数。

2. 在函数执行顺序方面要留心。

   在我的sumLiveFour函数中有这个语句

   ```c++
   (sameSum(u + 4, row, col) + sameSum(u, row, col)
   ```

   在一开始的时候我将前后两项写反了，所以sameSum返回的distance的值是前面那项（u+4）对应的。导致calculate在计算活四的时候总是不对。所以说在想算法的同时要注意语句的前后顺序。这样的小问题很难察觉。

## 可以改进的地方

1. 添加图形界面
2. 用极大极小值算法+alpha-beta剪枝写出第一代AI

## 源码

https://github.com/zyhhhy/myGobangDay1



# 第二天

今天主要干了三件事，EGE配置GUI，Dependencies查找dll还有极大极小值算法实现初步AI。

## EGE部分

Codeblocks配置EGE教程。

https://blog.csdn.net/qq_18994119/article/details/80890068

EGE官网绘图教程。

https://xege.org/

EGE五子棋示例。

https://blog.csdn.net/duanghaha/article/details/80835449

本身EGE并不是很难，我最初目的也不是把界面整的花里胡哨的，只是想美化一下界面。此处略过不提。

## Dependencies

我一直在想，我本地编译出来的exe文件，如何在别人的电脑上独立运行。

使用Dependencies神器就可以找到exe调用的动态链接库，打包在一起，进而可以在未装EGE的电脑上独立运行。

使用操作很简单，在软件中选中编译出来的exe文件，得到下图所示。然后从系统库中复制出来需要的dll库，放到与exe同级的文件夹里面。如此在别人未配置环境的电脑上就能独立运行exe文件了。

![1581689653836](D:\hexo\source\_posts\1581689653836.png)

附Dependencies网盘下载 地址

链接: https://pan.baidu.com/s/1NnAeNeMpycRjdUl6wKxZYg 提取码: cgjb 

## 极大极小值算法和alpha-beta剪枝

原本的想法是这样的：在getScore函数中计算scoreAI和scorePeople，两个值分别代表当前坐标点对于AI方和人类方的评估分数，然后通过scoreAI-scorePeople得出综合优势。这个值越大则越有利于AI局面。

而极大极小值算法就是说AI在思考的时候假设人类会做出最不利于AI的选择，因此AI需要在不利中寻找有利因素。由此建立一棵决策树。

![](maxmin.png)

AI是MAX层，在所有下方叶子节点中选择极大值，而人类是MIN层，在所有下方叶子节点中选择极小值。

同时alpha-beta剪枝可以减少运算量。



## 源码

