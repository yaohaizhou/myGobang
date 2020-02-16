/*
 * @Descripttion: Board头文件，宏定义，调用库，定义成员函数和变量
 * @version: 五子棋第三天启发式搜索版本（启发式搜索+alpha-beta剪枝+极大极小值算法）
 * @Author: 周耀海 u201811260@hust.edu.cn
 * @LastEditTime: 2020-02-16 09:07:01
 */
#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include <iostream>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <graphics.h>
#include <algorithm>
///棋盘样式
#define N 15///棋盘为15*15的方格
#define WALL 3///棋盘四周围一圈墙
#define EMPTY 0///空地为0
#define BLACK 1///黑为1
#define WHITE 2///白为0
///下棋模式
#define DEBUG 0///1为开启debug，0为关闭debug，开启debug会在棋局最后显示棋盘各点的评估值
#define FIRST 0///1为人类执黑先手，0为AI执黑先手
#define TURN_ON_CUT2 1///1为打开二层剪枝操作，0为关闭
#define TURN_ON_CUT3 1///1为打开三层剪枝操作，0为关闭，建议两层剪枝都打开
#define HEURISTIC 1///1为打开启发式搜索，0为关闭，视情况打开
///AI Intelligence
#define AI_MAX_CHOICE 20///AI在启发式搜索时每一步考虑的最大位置数量
class Board
{
public:
    Board();
    ~Board() {};
    void printBoard();
    void out(int, int);
    void play();
    bool checkAvailable(int, int);
    bool checkSame(int, int);
    void checkEnd();
    int sameSum(int, int, int);
    void player1();
    void player2();
    int calculate(int, int);
    int sumMakeFive(int, int);
    int sumLiveFour(int, int);
    int sumLiveThree(int, int);
    int sumMakeFour(int, int);
    int sumFive(int, int);
    int getRow();
    int getCol();
    void getMouseLoc();
    int getScore(int, int);
    void AI_1_MAX();
    int AI_2_MIN(int);
    int AI_3_MAX(int,int);

    typedef struct order
    {
        int orderi;
        int orderj;
        int orderpoint;
    }Order;
    Order orderSort[N*N];

    static bool cmp(Order,Order);


private:
    int chess[N + 2][N + 2];///棋盘周围有一圈墙
    int row, col, turn;///row为纵向的行,col为横向的列,turn为哪方执子,黑为1,白为2
    std::array<int, 8> dx;///flat技术，8个dx和dy组成8个方向，指向一个棋子的周围8个方向，把各方向统一为直线考虑
    std::array<int, 8> dy;
    bool is_end;///结束标志，true为未结束，false为结束
    int distance;///返回距离值，在计算是否满足活四、活三时有用
    mouse_msg msg;///鼠标

};


#endif // BOARD_H_INCLUDED
