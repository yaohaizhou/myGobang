#include <iostream>
#include <vector>
#include <array>
#include <windows.h>
#include <iomanip>
#include <cstdio>
using namespace std;
#define N 15///棋盘为15*15的方格
#define WALL 3///棋盘四周围一圈墙
#define EMPTY 0///空地为0
#define BLACK 1///黑为1
#define WHITE 2///白为0
#define DEBUG 0
class Board
{
public:
    Board();
    ~Board() {};
    void printBoard();
    void out(int, int);
    void play();
    bool checkAvailable();
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
private:
    int chess[N + 2][N + 2];///棋盘周围有一圈墙
    int row, col, turn;///row为纵向的行,col为横向的列,turn为哪方执子,黑为1,白为2
    array<int, 8> dx;
    array<int, 8> dy;
    bool is_end;
    int distance;

};


int main()
{
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

}
void Board::printBoard()///打印棋盘
{
    system("cls");
    int row, col;
    for (row = N; row >= 1; row--)
    {
        cout << '\n' << setw(2) << row;
        for (col = 1; col <= N; col++)
        {
            out(row, col);
        }
    }
    cout << endl;
    cout << "  ";
    char letter = 'A';
    for (col = 1; col <= N; col++)
        printf(" %c", col + letter - 1);
    cout << endl << endl;
    cout << "Please input the location: ";

}
void Board::out(int i, int j)///打印棋子和格子
{
    if (chess[i][j] == 1)
    {
        cout << "○";
        return;
    }
    if (chess[i][j] == 2)
    {
        cout << "●";
        return;
    }
    if (chess[i][j] == -1)
    {
        cout << "△";
        return;
    }
    if (chess[i][j] == -2)
    {
        cout << "▲";
        return;
    }
    if (i == N)
    {
        if (j == 1)
        {
            cout << "┏ ";
            return;
        }
        if (j == N)
        {
            cout << "┓ ";
            return;
        }
        {
            cout << ("┯ ");
            return;
        }
    }
    if (i == 1)
    {
        if (j == 1)
        {
            cout << "┗ ";
            return;
        }
        if (j == N)
        {
            cout << "┛ ";
            return;
        }
        {
            cout << "┷ ";
            return;
        }
    }
    if (j == 1)
    {
        cout << "┠ ";
        return;
    }
    if (j == N)
    {
        cout << "┨ ";
        return;
    }
    {
        cout << "┼ ";
        return;
    }
}
void Board::player1()///玩家一
{
    cin >> row ;
    cin >> col ;
    turn = 1;
    while(!checkAvailable())
    {
        cout << "The place is not available!\n";
        cin >> row >> col;
    }
    chess[row][col] = 1;
    printBoard();
    checkEnd();
}
void Board::player2()///玩家二
{
    cin >> row ;
    cin >> col ;
    turn = 2;
    while(!checkAvailable())
    {
        cout << "The place is not available!\n";
        cin >> row >> col;
    }
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
        (turn == 1 ? cout << "\nGame Over\nPlayer1 win\n\n" : cout << "\nGame Over\nPlayer2 win\n\n");
        if(DEBUG)
        {
            ///测试用输出打分值
            for(int i = N; i > 0; i--)
            {
                for(int j = 1; j < N + 1; j++)
                    cout << ' ' << calculate(i, j);
                cout << '\n';
            }
        }
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
bool Board::checkAvailable()///检查是否可落子
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
//    if(sum)cout << "distance:" << distance;
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
    point += 50000 * sumFive(row, col) + 4320 * sumLiveFour(row, col) + 720 * (sumMakeFive(row, col) + sumLiveThree(row, col));
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
