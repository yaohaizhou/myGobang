#include <iostream>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <graphics.h>
using namespace std;
#define N 15///棋盘为15*15的方格
#define WALL 3///棋盘四周围一圈墙
#define EMPTY 0///空地为0
#define BLACK 1///黑为1
#define WHITE 2///白为0
#define DEBUG 0///1为开启debug，0为关闭debug


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
    int chess[N + 2][N + 2];///棋盘周围有一圈墙
    int row, col, turn;///row为纵向的行,col为横向的列,turn为哪方执子,黑为1,白为2
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
            chess[i][j] = 0;///初始化空地
    for(i = 0; i < N + 2; i++)
    {
        chess[i][0] = WALL;///初始化墙
        chess[i][N + 1] = WALL;
    }
    for(j = 0; j < N + 2; j++)
    {
        chess[0][j] = WALL;
        chess[N + 1][j] = WALL;
    }
    if(DEBUG)
    {
        ///测试用五连+活四
        chess[8][8] = BLACK;
        chess[9][8] = BLACK;
        chess[10][8] = BLACK;
        chess[11][8] = BLACK;
        chess[8][9] = WHITE;
        chess[9][9] = WHITE;
        chess[10][9] = WHITE;
        chess[11][9] = WHITE;
        ///测试用冲四
        chess[8][1] = BLACK;
        chess[9][1] = BLACK;
        chess[10][1] = BLACK;
        chess[7][1] = WHITE;
        ///测试用活四
        chess[8][4] = BLACK;
        chess[9][4] = BLACK;
        chess[11][4] = BLACK;
        ///测试用五连
        chess[8][12] = BLACK;
        chess[9][12] = BLACK;
        chess[10][12] = BLACK;
        chess[11][12] = BLACK;
    }
    dx = { 1, 1, 0, -1, -1, -1, 0, 1 };///flat技术的8个方向向量
    dy = { 0, 1, 1, 1, 0, -1, -1, -1 };
    is_end = true;
    msg = {0};

}
void Board::printBoard()///打印棋盘
{
    int i, j;
    setbkcolor(EGERGB(173, 92, 45));///设置背景颜色
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
void Board::out(int i, int j)///打印棋子和格子
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
void Board::player1()///玩家一
{
    turn = 1;
//    getMouseLoc();
    AI_1_MAX();
    chess[row][col] = 1;
    printBoard();
    checkEnd();
}
void Board::player2()///玩家二
{
    turn = 2;
    getMouseLoc();
//    AI_1_MAX();
    chess[row][col] = 2;
    printBoard();
    checkEnd();
}
void Board::checkEnd()///检查是否结束
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
            setbkmode(TRANSPARENT);///文字填充透明化
            setcolor(EGERGB(0, 0, 0));
            setfont(20, 10, "Italic");
            outtextxy(550, 10, "Black Win!");
        }
        else if(turn == 2)
        {
            setbkmode(TRANSPARENT);///文字填充透明化
            setcolor(EGERGB(255, 255, 255));
            setfont(20, 10, "Italic");
            outtextxy(550, 10, "White Win!");
        }
        if(DEBUG)
        {
            ///测试用输出打分值
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
        exit(0);///直接退出程序
    }
}
bool Board::checkSame(int row, int col)///检查是否相同
{
    return (chess[row][col] == turn);
}
int Board::sameSum(int u, int row, int col)///检查相同个数
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
bool Board::checkAvailable(int row, int col) ///检查是否可落子
{
    return (row >= 1 && row <= 15 && col >= 1 && col <= 15 && chess[row][col] == 0);
}
///sumMakeFive要领
///在8个方向上求成五点，对于每个方向，从当前位置往前延伸，
///第一个不同色的点是空格，跳过空格继续往前延伸直到第二个不同色的点，
///同时从当前位置往反方向延伸直到第一个不同色的点， 算上当前位置本身，
///如果同色点的数量一共是4，那么该方向就有成五点。
int Board::sumMakeFive(int row, int col)///成五点总数
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

int Board::sumLiveFour(int row, int col)///活四总数
{
    int sum = 0, u;
    for(u = 0; u < 4; u++)
    {
        if((sameSum(u + 4, row, col) + sameSum(u, row, col)) >= 3)
            ///记念一下，前后顺序不一样导致distance不一样
            ///(sameSum(u,row,col) + sameSum(u+4,row,col)
            ///原本写的是这样的，后面的函数后运行，使用的distance就是后面那个
            ///所以活四一直没有匹配上
        {
            if(chess[row + distance * dx[u]][col + distance * dy[u]] == EMPTY && chess[row + (distance - 5)*dx[u]][col + (distance - 5)*dy[u]] == EMPTY)
                sum++;
        }
    }
    return sum;
}
int Board::sumMakeFour(int row, int col)///冲四总数
{
    return sumMakeFive(row, col) - 2 * sumLiveFour(row, col);
}
int Board::sumLiveThree(int row, int col)///活三总数
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

int Board::calculate(int row, int col)///计算分值
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
int Board::sumFive(int row, int col)///五连总数
{
    int sum = 0;
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, row, col) + sameSum(u + 4, row, col)) >= 4)
            sum++;
    }
    return sum;
}
int Board::getRow()///获取行
{
    return row;
}
int Board::getCol()///获取列
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



