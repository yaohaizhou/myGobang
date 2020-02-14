#include <iostream>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <graphics.h>
using namespace std;
#define N 15///����Ϊ15*15�ķ���
#define WALL 3///��������ΧһȦǽ
#define EMPTY 0///�յ�Ϊ0
#define BLACK 1///��Ϊ1
#define WHITE 2///��Ϊ0
#define DEBUG 0///1Ϊ����debug��0Ϊ�ر�debug


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
    int AI_2_MIN();
    int AI_3_MAX(int);


private:
    int chess[N + 2][N + 2];///������Χ��һȦǽ
    int row, col, turn;///rowΪ�������,colΪ�������,turnΪ�ķ�ִ��,��Ϊ1,��Ϊ2
    array<int, 8> dx;
    array<int, 8> dy;
    bool is_end;
    int distance;
    mouse_msg msg;
    int scoreTurn;

};

int main()
{
    initgraph(1000, 500);
    Board board;
    board.printBoard();
    while(true)
    {
        board.player1();
        board.player2();
    }

    return 0;
}
Board::Board()
{
    int i, j;
    for (i = 1; i < N + 1; i++)
        for (j = 1; j < N + 1; j++)
            chess[i][j] = 0;///��ʼ���յ�
    for(i = 0; i < N + 2; i++)
    {
        chess[i][0] = WALL;///��ʼ��ǽ
        chess[i][N + 1] = WALL;
    }
    for(j = 0; j < N + 2; j++)
    {
        chess[0][j] = WALL;
        chess[N + 1][j] = WALL;
    }
    if(DEBUG)
    {
        ///����������+����
        chess[8][8] = BLACK;
        chess[9][8] = BLACK;
        chess[10][8] = BLACK;
        chess[11][8] = BLACK;
        chess[8][9] = WHITE;
        chess[9][9] = WHITE;
        chess[10][9] = WHITE;
        chess[11][9] = WHITE;
        ///�����ó���
        chess[8][1] = BLACK;
        chess[9][1] = BLACK;
        chess[10][1] = BLACK;
        chess[7][1] = WHITE;
        ///�����û���
        chess[8][4] = BLACK;
        chess[9][4] = BLACK;
        chess[11][4] = BLACK;
        ///����������
        chess[8][12] = BLACK;
        chess[9][12] = BLACK;
        chess[10][12] = BLACK;
        chess[11][12] = BLACK;
    }
    dx = { 1, 1, 0, -1, -1, -1, 0, 1 };///flat������8����������
    dy = { 0, 1, 1, 1, 0, -1, -1, -1 };
    is_end = true;
    msg = {0};

}
void Board::printBoard()///��ӡ����
{
    int i, j;
    setbkcolor(EGERGB(173, 92, 45));///���ñ�����ɫ
    setcolor(EGERGB(0, 0, 0));
    for(i = 60; i < 510; i += 30)
    {
        line(i, 60, i, 480);
    }
    for(i = 60; i < 510; i += 30)
    {
        line(60, i, 480, i);
    }
    for(i = 60; i < 510; i += 30)
    {
        for(j = 60; j < 510; j += 30)
        {
            out(i, j);
        }
    }
}
void Board::out(int i, int j)///��ӡ���Ӻ͸���
{
    if(chess[i / 30 - 1][j / 30 - 1] == 1)
    {
        setcolor(EGERGB(0, 0, 0));
        setfillcolor(EGERGB(0, 0, 0));
        fillellipse(i, j, 15, 15);
    }
    if(chess[i / 30 - 1][j / 30 - 1] == 2)
    {
        setcolor(EGERGB(255, 255, 255));
        setfillcolor(EGERGB(255, 255, 255));
        fillellipse(i, j, 15, 15);
    }
}
void Board::player1()///���һ
{
    turn = 1;
//    getMouseLoc();
    AI_1_MAX();
    chess[row][col] = 1;
    printBoard();
    checkEnd();
}
void Board::player2()///��Ҷ�
{
    turn = 2;
    getMouseLoc();
//    AI_1_MAX();
    chess[row][col] = 2;
    printBoard();
    checkEnd();
}
void Board::checkEnd()///����Ƿ����
{
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, getRow(), getCol()) + sameSum(u + 4, getRow(), getCol())) >= 4)
            is_end = false;
    }
    if(!is_end)
    {
        if(turn == 1)
        {
            setbkmode(TRANSPARENT);///�������͸����
            setcolor(EGERGB(0, 0, 0));
            setfont(20, 10, "Italic");
            outtextxy(550, 10, "Black Win!");
        }
        else if(turn == 2)
        {
            setbkmode(TRANSPARENT);///�������͸����
            setcolor(EGERGB(255, 255, 255));
            setfont(20, 10, "Italic");
            outtextxy(550, 10, "White Win!");
        }
        if(DEBUG)
        {
            ///������������ֵ
            for(int i = N; i > 0; i--)
            {
                for(int j = 1; j < N + 1; j++)
                {
                    setfont(20, 10, "Italic");
                    xyprintf(i * 30 + 510, j * 30 + 20, "%d", calculate(i, j));
                }
            }

        }
        getch();
        closegraph();
        exit(0);///ֱ���˳�����
    }
}
bool Board::checkSame(int row, int col)///����Ƿ���ͬ
{
    return (chess[row][col] == turn);
}
int Board::sameSum(int u, int row, int col)///�����ͬ����
{
    int sum = 0;
    int i;
    for(i = 1; checkSame(row + i * dx[u], col + i * dy[u]); i++)
    {
        sum++;
    }
    distance = i;
    return sum;
}
bool Board::checkAvailable(int row, int col) ///����Ƿ������
{
    return (row >= 1 && row <= 15 && col >= 1 && col <= 15 && chess[row][col] == 0);
}
///sumMakeFiveҪ��
///��8�������������㣬����ÿ�����򣬴ӵ�ǰλ����ǰ���죬
///��һ����ͬɫ�ĵ��ǿո������ո������ǰ����ֱ���ڶ�����ͬɫ�ĵ㣬
///ͬʱ�ӵ�ǰλ��������������ֱ����һ����ͬɫ�ĵ㣬 ���ϵ�ǰλ�ñ���
///���ͬɫ�������һ����4����ô�÷�����г���㡣
int Board::sumMakeFive(int row, int col)///���������
{
    int sum = 0, i, u;

    for(u = 0; u < 8; u++)
    {
        bool flag = true;
        int sumk = 0;
        for(i = 1; checkSame(row + i * dx[u], col + i * dy[u]) || flag; i++)
        {
            if(!checkSame(row + i * dx[u], col + i * dy[u]))
            {
                if(chess[row + i * dx[u]][col + i * dy[u]] != EMPTY)
                {
                    sumk -= 10;
                }
                flag = false;
            }
            sumk++;
        }
        for(i = -1; checkSame(row + i * dx[u], col + i * dy[u]); i--)
        {
            sumk++;
        }
        if(sumk == 4)
            sum++;
    }

    return sum;
}

int Board::sumLiveFour(int row, int col)///��������
{
    int sum = 0, u;
    for(u = 0; u < 4; u++)
    {
        if((sameSum(u + 4, row, col) + sameSum(u, row, col)) >= 3)
            ///����һ�£�ǰ��˳��һ������distance��һ��
            ///(sameSum(u,row,col) + sameSum(u+4,row,col)
            ///ԭ��д���������ģ�����ĺ��������У�ʹ�õ�distance���Ǻ����Ǹ�
            ///���Ի���һֱû��ƥ����
        {
            if(chess[row + distance * dx[u]][col + distance * dy[u]] == EMPTY && chess[row + (distance - 5)*dx[u]][col + (distance - 5)*dy[u]] == EMPTY)
                sum++;
        }
    }
    return sum;
}
int Board::sumMakeFour(int row, int col)///��������
{
    return sumMakeFive(row, col) - 2 * sumLiveFour(row, col);
}
int Board::sumLiveThree(int row, int col)///��������
{
    int u, sum = 0;
    for(u = 0; u < 4; u++)
    {
        if((sameSum(u + 4, row, col) + sameSum(u, row, col)) >= 2)
        {
            if(
                (chess[row + distance * dx[u]][col + distance * dy[u]] == EMPTY &&
                 chess[row + (distance - 4)*dx[u]][col + (distance - 4)*dy[u]] == EMPTY &&
                 chess[row + (distance - 5)*dx[u]][col + (distance - 5)*dy[u]] == EMPTY)
                ||
                (  chess[row + distance * dx[u]][col + distance * dy[u]] == EMPTY &&
                   chess[row + (distance + 1)*dx[u]][col + (distance + 1)*dy[u]] == EMPTY &&
                   chess[row + (distance - 4)*dx[u]][col + (distance - 4)*dy[u]] == EMPTY
                ))
                sum++;

        }
    }
    for(u = 0; u < 8; u++)
    {
        int sumk = 0, i;
        bool flag = true;
        for(i = 1; checkSame(row + i * dx[u], col + i * dy[u]) || flag; i++)
        {
            if(!checkSame(row + i * dx[u], col + i * dy[u]))
            {
                if(chess[row + i * dx[u]][col + i * dy[u]] != EMPTY)
                {
                    sumk -= 10;
                }
                flag = false;
            }
            sumk++;
        }
        if(sumk == 3)
            sum++;
    }
    return sum;
}

int Board::calculate(int row, int col)///�����ֵ
{
    if(chess[row][col] != EMPTY)
        return 0;
    int point = 0;
    for(int u = 0; u < 8; u++)
        if(chess[row + dx[u]][col + dy[u]] != EMPTY && chess[row + dx[u]][col + dy[u]] != WALL)
            point++;
    if(sumFive(row, col))
        return 50000;
    point += 4320 * sumLiveFour(row, col) + 720 * (sumMakeFive(row, col) + sumLiveThree(row, col));
    return point;
}
int Board::sumFive(int row, int col)///��������
{
    int sum = 0;
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, row, col) + sameSum(u + 4, row, col)) >= 4)
            sum++;
    }
    return sum;
}
int Board::getRow()///��ȡ��
{
    return row;
}
int Board::getCol()///��ȡ��
{
    return col;
}
void Board::getMouseLoc()
{
    int mousex = 0, mousey = 0;
    for(; is_run;)
    {
        msg = getmouse();
        if(msg.is_down())
        {
            if((int)msg.x % 30 < 15)
                mousex = (int)msg.x - ((int)msg.x % 30);
            else
                mousex = (int)msg.x - ((int)msg.x % 30) + 30;
            if((int)msg.y % 30 < 15)
                mousey = (int)msg.y - ((int)msg.y % 30);
            else
                mousey = (int)msg.y - ((int)msg.y % 30) + 30;
        }
        if(checkAvailable(mousex / 30 - 1, mousey / 30 - 1))
        {
            row = mousex / 30 - 1;
            col = mousey / 30 - 1;
            break;
        }
        else
            continue;
    }
}
int Board::getScore(int row, int col)
{
////    int originalTurn=turn;
//    scoreTurn=turn;
//    turn = 3 - turn;
//    int scorePeople = calculate(row, col);
//    turn = scoreTurn;
//    int scoreAI = calculate(row, col);
////    turn = 3-turn;
//    return scoreAI - scorePeople;
    return calculate(row,col);
}
void Board::AI_1_MAX()
{
    if(chess[8][8] == 0)
    {
        row = 8;
        col = 8;
        return;
    }
    int i, j, score = -100000, temp = 0;
    for(i = 1; i < N + 1; i++)
    {
        for(j = 1; j < N + 1; j++)
        {
            if(!checkAvailable(i, j))
                continue;
            temp = getScore(i, j);
            chess[i][j] = turn;
            if(temp == 0)
            {
                chess[i][j] = EMPTY;
                continue;
            }
            if(temp == 50000)
            {
                row = i;
                col = j;
                return;
            }
            temp = AI_2_MIN();
            chess[i][j] = EMPTY;
            if(temp > score)
            {
                score = temp;
                row = i;
                col = j;
            }
        }
    }
    return;
}
int Board::AI_2_MIN()
{
    int score = 100000, temp = 0;
    for(int i = 1; i < N + 1; i++)
    {
        for(int j = 1; j < N + 1; j++)
        {
            if(!checkAvailable(i, j))
                continue;
            temp = getScore(i, j);
            chess[i][j] = 3 - turn;
            if(temp == 0)
            {
                chess[i][j] = EMPTY;
                continue;
            }
            if(temp == 50000)
            {
                chess[i][j] = EMPTY;
                return -50000;
            }
            temp = AI_3_MAX(temp);
            chess[i][j] = EMPTY;
            if(temp < score)
                score = temp;
        }
    }
    return score;
}
int Board::AI_3_MAX(int score2)
{
    int score = -100000, temp = 0;
    for(int i = 1; i < N + 1; i++)
    {
        for(int j = 1; j < N + 1; j++)
        {
            if(!checkAvailable(i, j))
                continue;
            temp = getScore(i, j);
            chess[i][j] = turn;
            if(temp == 0)
            {
                chess[i][j] = 0;
                continue;
            }
            if(temp == 50000)
            {
                chess[i][j] = 0;
                return 50000;
            }
            chess[i][j] = EMPTY;
            if(temp - score2 * 2 > score)
                score = temp - score2 * 2;
        }
    }
    return score;
}



