/*
 * @Descripttion: Board函数实现
 * @version: 五子棋第三天启发式搜索版本（启发式搜索+alpha-beta剪枝+极大极小值算法）
 * @Author: 周耀海 u201811260@hust.edu.cn
 * @LastEditTime: 2020-02-16 09:48:29
 */
#include "Board.h"
/**
 * @name: Board
 * @msg: 构造函数，用于初始化参数
 * @param none
 * @return: void
 */
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
    orderSort[AI_MAX_CHOICE]={0};

}
/**
 * @name: printBoard
 * @msg: 用于打印棋盘
 * @param none
 * @return: void
 */
void Board::printBoard()
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
/**
 * @name: out
 * @msg: 分类别打印棋子和格子
 * @param {int} i {int} j 代表对应点的行和列
 * @return: void
 */
void Board::out(int i, int j)
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
/**
 * @name: player1
 * @msg: 玩家一下棋操作，先下棋再打印棋盘，判断是否胜利
 * @param none
 * @return: void
 */
void Board::player1()
{
    turn = 1;
    if(FIRST)
    {
        getMouseLoc();
    }
    else
    {
        AI_1_MAX();
    }
    chess[row][col] = 1;
    printBoard();
    checkEnd();
}
/**
 * @name: player2
 * @msg: 玩家二下棋操作，先下棋再打印棋盘，判断是否胜利
 * @param none
 * @return: void
 */
void Board::player2()
{
    turn = 2;
    if(FIRST)
    {
        AI_1_MAX();
    }
    else
    {
        getMouseLoc();
    }
    chess[row][col] = 2;
    printBoard();
    checkEnd();
}
/**
 * @name: checkEnd
 * @msg: 检查是否结束，用判断五连的方法判断是否胜利，如果开启debug模式会显示棋盘所有点的打分值
 * @param none
 * @return: void
 */
void Board::checkEnd()
{
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, getRow(), getCol()) + sameSum(u + 4, getRow(), getCol())) >= 4)
            is_end = false;//赢了
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
/**
 * @name: checkSame
 * @msg: 检查是否相同
 * @param {int} row {int} col 传入的行和列，非全局变量行和列
 * @return: bool 相同则返回true，不同返回false
 */
bool Board::checkSame(int row, int col)
{
    return (chess[row][col] == turn);
}
/**
 * @name: sameSum
 * @msg: 检查在一个方向上相同棋子个数
 * @param {int} u 传入方向 {int} row {int} col 传入的行和列
 * @return: {int} sum 总数
 */
int Board::sameSum(int u, int row, int col)
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
/**
 * @name: checkAvailable
 * @msg: 检查是否可落子
 * @param {int} row {int} col  传入的行和列
 * @return: bool 可落子为true，不可落子为false
 */
bool Board::checkAvailable(int row, int col)
{
    return (row >= 1 && row <= 15 && col >= 1 && col <= 15 && chess[row][col] == 0);
}
///sumMakeFive要领
///在8个方向上求成五点，对于每个方向，从当前位置往前延伸，
///第一个不同色的点是空格，跳过空格继续往前延伸直到第二个不同色的点，
///同时从当前位置往反方向延伸直到第一个不同色的点， 算上当前位置本身，
///如果同色点的数量一共是4，那么该方向就有成五点。
/**
 * @name: sumMakeFive
 * @msg: 成五点总数，用来计算冲四的个数，成五点形式为YabcdeY，其中abcde中4个1,1个0，所以考虑可以跳过一个空格然后4个1。但是不可跳过对方棋子或者墙壁
 * @param {int} row {int} col  传入的行和列
 * @return: {int} sum 总数
 */
int Board::sumMakeFive(int row, int col)
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
/**
 * @name: sumLiveFour
 * @msg: 活四总数
 * @param {int} row {int} col  传入的行和列
 * @return: {int} sum 总数
 */
int Board::sumLiveFour(int row, int col)
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
/**
 * @name: sumMakeFour
 * @msg: 冲四总数=成五点总数-2*活四总数
 * @param {int} row {int} col  传入的行和列
 * @return: {int} sum 总数
 */
int Board::sumMakeFour(int row, int col)
{
    return sumMakeFive(row, col) - 2 * sumLiveFour(row, col);
}
/**
 * @name: sumLiveThree
 * @msg: 活三总数
 * @param {int} row {int} col  传入的行和列
 * @return: {int} sum 总数
 */
int Board::sumLiveThree(int row, int col)
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
/**
 * @name: calculate
 * @msg: 计算分值，如果这个点有棋子记0分，如果五连则50000分，如果活四则4320分，如果活三或者冲四则720分
 * @param {int} row {int} col  传入的行和列
 * @return: {int} point 打分值
 */
int Board::calculate(int row, int col)///
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
/**
 * @name: sumFive
 * @msg: 五连总数
 * @param {int} row {int} col  传入的行和列
 * @return: {int} sum 总数
 */
int Board::sumFive(int row, int col)
{
    int sum = 0;
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, row, col) + sameSum(u + 4, row, col)) >= 4)
            sum++;
    }
    return sum;
}
/**
 * @name: getRow
 * @msg: 获取行
 * @param none
 * @return: {int} row 全局变量行
 */
int Board::getRow()
{
    return row;
}
/**
 * @name: getCol
 * @msg: 获取列
 * @param {type}
 * @return: {int} col 全局变量列
 */
int Board::getCol()
{
    return col;
}
/**
 * @name: getMouseLoc
 * @msg: 获取鼠标位置并且定位到真实棋盘上，棋盘从60，60开始画起，每隔30一条线，所以要进行四舍五入和整数化
 * @param none
 * @return: void
 */
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
/**
 * @name: getScore
 * @msg: 计算当前点打分值，但是是以某一方的当前点利益为参考的，其实更好的方法是scoreAI-scoreHuman得出对于AI最有利，对于人类最不利的点
 * @param {int} row {int} col  传入的行和列
 * @return: {int} calculate(row,col) 当前点打分值
 */
int Board::getScore(int row, int col)
{
    return calculate(row, col);
}
/**
 * @name: AI_1_MAX
 * @msg: 第一层若采用启发式搜素，会对225个点进行一个排序，得分高的优先进行第二层的运算，相当于修改第二层的顺序，有利于剪枝
 * 若不采用启发式搜索，就对225个点依次调用二层运算
 * 第一层代表AI的利益，所以取极大值
 * @param none
 * @return: void
 */
void Board::AI_1_MAX()
{
    if(chess[8][8] == 0)
    {
        row = 8;
        col = 8;
        return;
    }
    int i, j, score = -100000, temp = 0;

    if(HEURISTIC)
    {
        ///启发式搜索
        for(i=1;i<N+1;i++)
        {
            for(j=1;j<N+1;j++)
            {
                orderSort[(i-1)*15+(j-1)].orderi=i;///给结构体成员赋值i/j/point
                orderSort[(i-1)*15+(j-1)].orderj=j;
                orderSort[(i-1)*15+(j-1)].orderpoint=getScore(i,j);
            }
        }
        std::sort(orderSort,orderSort+N*N,cmp);
        for(i=0;i<AI_MAX_CHOICE;i++)
        {
            if(!checkAvailable(orderSort[i].orderi,orderSort[i].orderj))continue;
            if(orderSort[i].orderpoint==0)continue;
            if(orderSort[i].orderpoint==50000)
            {
                row=orderSort[i].orderi;
                col=orderSort[i].orderj;
                return;
            }
            chess[orderSort[i].orderi][orderSort[i].orderj]=turn;
            temp=AI_2_MIN(score);
            chess[orderSort[i].orderi][orderSort[i].orderj]=EMPTY;
            if(temp>score)
            {
                score=temp;
                row=orderSort[i].orderi;
                col=orderSort[i].orderj;
            }
        }
    }
    else
    {
        for(i = 1; i < N + 1; i++)
        {
            for(j = 1; j < N + 1; j++)
            {
                if(!checkAvailable(i, j))
                    continue;//此处可以直接加上checkAround
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
                temp = AI_2_MIN(score);
                chess[i][j] = EMPTY;
                if(temp > score)
                {
                    score = temp;
                    row = i;
                    col = j;
                }///第一层取极大值
            }
        }
    }


    return;
}
/**
 * @name: AI_2_MIN
 * @msg: 模拟对手进行二层运算，取极小值
 * @param {int} score1 从第一层下来的数据，这个数据维护第一层的极大值，所以如果第二层算出来的点比这个值小，说明就不需要再算了，因为AI不可能选这个方案了
 * @return: {int} score 维护第二层的极小值
 */
int Board::AI_2_MIN(int score1)
{
    int score = 100000, temp = 0;
    for(int i = 1; i < N + 1; i++)
    {
        bool cut1 = false;
        for(int j = 1; j < N + 1; j++)
        {
            if(!checkAvailable(i, j))
                continue;
            int originalTurn = turn;
            turn = 3 - turn;
            temp = getScore(i, j);
            turn = originalTurn;
            chess[i][j] = 3 - turn; ///模拟对手下棋
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
            temp = AI_3_MAX(score,temp);
            chess[i][j] = EMPTY;
            if(temp < score)
                score = temp;///第二层取极小值
            if(TURN_ON_CUT2)
            {
                if( score1 > temp)
                {
                    cut1 = true;
                    break;
                }
            }
        }
        if(TURN_ON_CUT2)
        {
            if(cut1)
                break;
        }
    }
    return score;
}
/**
 * @name: AI_3_MAX
 * @msg: 第三层是AI利益，取极大。如果第三层算出来的点比第二层的极小值要大，说明第三层这个点不可能使得第二层的极小值变小了，不需要再算了。
 * 其实极大极小值算法是上下两层数据斜对角的关系
 * @param {int} score2 从第二层传下来的维护极小值 {int} tempp 第二层当前计算值，用于提高难度/速度
 * @return: {int} score 从第三层返回极大值
 */
int Board::AI_3_MAX(int score2,int tempp)
{
    int score = -100000, temp = 0;
    for(int i = 1; i < N + 1; i++)
    {
        bool cut2 = false;
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
            if(temp - tempp * 3 > score)
                score = temp - tempp * 3;
            if(TURN_ON_CUT3)
            {
                if(temp > score2)
                {
                    cut2 = true;
                    break;
                }
            }
        }
        if(TURN_ON_CUT3)
        {
            if(cut2)
                break;
        }
    }
    return score;
}
/**
 * @name: cmp
 * @msg: > 为降序 < 为升序，结构体数组按照成员orderpoint降序排列
 * @param {Order} x {Order} y 两个结构体
 * @return: bool
 */
bool Board::cmp(Order x,Order y)
{
    return (x.orderpoint) > (y.orderpoint);
}

