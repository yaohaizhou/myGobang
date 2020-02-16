/*
 * @Descripttion: Boardͷ�ļ����궨�壬���ÿ⣬�����Ա�����ͱ���
 * @version: ���������������ʽ�����汾������ʽ����+alpha-beta��֦+����Сֵ�㷨��
 * @Author: ��ҫ�� u201811260@hust.edu.cn
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
///������ʽ
#define N 15///����Ϊ15*15�ķ���
#define WALL 3///��������ΧһȦǽ
#define EMPTY 0///�յ�Ϊ0
#define BLACK 1///��Ϊ1
#define WHITE 2///��Ϊ0
///����ģʽ
#define DEBUG 0///1Ϊ����debug��0Ϊ�ر�debug������debug������������ʾ���̸��������ֵ
#define FIRST 0///1Ϊ����ִ�����֣�0ΪAIִ������
#define TURN_ON_CUT2 1///1Ϊ�򿪶����֦������0Ϊ�ر�
#define TURN_ON_CUT3 1///1Ϊ�������֦������0Ϊ�رգ����������֦����
#define HEURISTIC 1///1Ϊ������ʽ������0Ϊ�رգ��������
///AI Intelligence
#define AI_MAX_CHOICE 20///AI������ʽ����ʱÿһ�����ǵ����λ������
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
    int chess[N + 2][N + 2];///������Χ��һȦǽ
    int row, col, turn;///rowΪ�������,colΪ�������,turnΪ�ķ�ִ��,��Ϊ1,��Ϊ2
    std::array<int, 8> dx;///flat������8��dx��dy���8������ָ��һ�����ӵ���Χ8�����򣬰Ѹ�����ͳһΪֱ�߿���
    std::array<int, 8> dy;
    bool is_end;///������־��trueΪδ������falseΪ����
    int distance;///���ؾ���ֵ���ڼ����Ƿ�������ġ�����ʱ����
    mouse_msg msg;///���

};


#endif // BOARD_H_INCLUDED
