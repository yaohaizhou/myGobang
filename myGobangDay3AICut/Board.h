#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include <iostream>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <graphics.h>
///������ʽ
#define N 15///����Ϊ15*15�ķ���
#define WALL 3///��������ΧһȦǽ
#define EMPTY 0///�յ�Ϊ0
#define BLACK 1///��Ϊ1
#define WHITE 2///��Ϊ0
///����ģʽ
#define DEBUG 0///1Ϊ����debug,0Ϊ�ر�debug
#define FIRST 0///1Ϊ����ִ������,0ΪAIִ������
#define TURN_ON_CUT2 1///1Ϊ�򿪶����֦����,0Ϊ�ر�
#define TURN_ON_CUT3 1///1Ϊ�������֦����,0Ϊ�ر�
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
    int chess[N + 2][N + 2];///������Χ��һȦǽ
    int row, col, turn;///rowΪ�������,colΪ�������,turnΪ�ķ�ִ��,��Ϊ1,��Ϊ2
    std::array<int, 8> dx;
    std::array<int, 8> dy;
    bool is_end;
    int distance;
    mouse_msg msg;
    int scoreTurn;

};


#endif // BOARD_H_INCLUDED
