#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include <iostream>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <graphics.h>
///棋盘样式
#define N 15///棋盘为15*15的方格
#define WALL 3///棋盘四周围一圈墙
#define EMPTY 0///空地为0
#define BLACK 1///黑为1
#define WHITE 2///白为0
///下棋模式
#define DEBUG 0///1为开启debug,0为关闭debug
#define FIRST 0///1为人类执黑先手,0为AI执黑先手
#define TURN_ON_CUT2 1///1为打开二层剪枝操作,0为关闭
#define TURN_ON_CUT3 1///1为打开三层剪枝操作,0为关闭
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


private:
    int chess[N + 2][N + 2];///棋盘周围有一圈墙
    int row, col, turn;///row为纵向的行,col为横向的列,turn为哪方执子,黑为1,白为2
    std::array<int, 8> dx;
    std::array<int, 8> dy;
    bool is_end;
    int distance;
    mouse_msg msg;
    int scoreTurn;

};


#endif // BOARD_H_INCLUDED
