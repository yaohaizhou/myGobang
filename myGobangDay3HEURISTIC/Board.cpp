/*
 * @Descripttion: Board����ʵ��
 * @version: ���������������ʽ�����汾������ʽ����+alpha-beta��֦+����Сֵ�㷨��
 * @Author: ��ҫ�� u201811260@hust.edu.cn
 * @LastEditTime: 2020-02-16 09:48:29
 */
#include "Board.h"
/**
 * @name: Board
 * @msg: ���캯�������ڳ�ʼ������
 * @param none
 * @return: void
 */
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
    orderSort[AI_MAX_CHOICE]={0};

}
/**
 * @name: printBoard
 * @msg: ���ڴ�ӡ����
 * @param none
 * @return: void
 */
void Board::printBoard()
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
/**
 * @name: out
 * @msg: ������ӡ���Ӻ͸���
 * @param {int} i {int} j �����Ӧ����к���
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
 * @msg: ���һ����������������ٴ�ӡ���̣��ж��Ƿ�ʤ��
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
 * @msg: ��Ҷ�����������������ٴ�ӡ���̣��ж��Ƿ�ʤ��
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
 * @msg: ����Ƿ���������ж������ķ����ж��Ƿ�ʤ�����������debugģʽ����ʾ�������е�Ĵ��ֵ
 * @param none
 * @return: void
 */
void Board::checkEnd()
{
    for(int u = 0; u < 4; u++)
    {
        if((sameSum(u, getRow(), getCol()) + sameSum(u + 4, getRow(), getCol())) >= 4)
            is_end = false;//Ӯ��
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
/**
 * @name: checkSame
 * @msg: ����Ƿ���ͬ
 * @param {int} row {int} col ������к��У���ȫ�ֱ����к���
 * @return: bool ��ͬ�򷵻�true����ͬ����false
 */
bool Board::checkSame(int row, int col)
{
    return (chess[row][col] == turn);
}
/**
 * @name: sameSum
 * @msg: �����һ����������ͬ���Ӹ���
 * @param {int} u ���뷽�� {int} row {int} col ������к���
 * @return: {int} sum ����
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
 * @msg: ����Ƿ������
 * @param {int} row {int} col  ������к���
 * @return: bool ������Ϊtrue����������Ϊfalse
 */
bool Board::checkAvailable(int row, int col)
{
    return (row >= 1 && row <= 15 && col >= 1 && col <= 15 && chess[row][col] == 0);
}
///sumMakeFiveҪ��
///��8�������������㣬����ÿ�����򣬴ӵ�ǰλ����ǰ���죬
///��һ����ͬɫ�ĵ��ǿո������ո������ǰ����ֱ���ڶ�����ͬɫ�ĵ㣬
///ͬʱ�ӵ�ǰλ��������������ֱ����һ����ͬɫ�ĵ㣬 ���ϵ�ǰλ�ñ���
///���ͬɫ�������һ����4����ô�÷�����г���㡣
/**
 * @name: sumMakeFive
 * @msg: ���������������������ĵĸ������������ʽΪYabcdeY������abcde��4��1,1��0�����Կ��ǿ�������һ���ո�Ȼ��4��1�����ǲ��������Է����ӻ���ǽ��
 * @param {int} row {int} col  ������к���
 * @return: {int} sum ����
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
 * @msg: ��������
 * @param {int} row {int} col  ������к���
 * @return: {int} sum ����
 */
int Board::sumLiveFour(int row, int col)
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
/**
 * @name: sumMakeFour
 * @msg: ��������=���������-2*��������
 * @param {int} row {int} col  ������к���
 * @return: {int} sum ����
 */
int Board::sumMakeFour(int row, int col)
{
    return sumMakeFive(row, col) - 2 * sumLiveFour(row, col);
}
/**
 * @name: sumLiveThree
 * @msg: ��������
 * @param {int} row {int} col  ������к���
 * @return: {int} sum ����
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
 * @msg: �����ֵ���������������Ӽ�0�֣����������50000�֣����������4320�֣�����������߳�����720��
 * @param {int} row {int} col  ������к���
 * @return: {int} point ���ֵ
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
 * @msg: ��������
 * @param {int} row {int} col  ������к���
 * @return: {int} sum ����
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
 * @msg: ��ȡ��
 * @param none
 * @return: {int} row ȫ�ֱ�����
 */
int Board::getRow()
{
    return row;
}
/**
 * @name: getCol
 * @msg: ��ȡ��
 * @param {type}
 * @return: {int} col ȫ�ֱ�����
 */
int Board::getCol()
{
    return col;
}
/**
 * @name: getMouseLoc
 * @msg: ��ȡ���λ�ò��Ҷ�λ����ʵ�����ϣ����̴�60��60��ʼ����ÿ��30һ���ߣ�����Ҫ�������������������
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
 * @msg: ���㵱ǰ����ֵ����������ĳһ���ĵ�ǰ������Ϊ�ο��ģ���ʵ���õķ�����scoreAI-scoreHuman�ó�����AI��������������������ĵ�
 * @param {int} row {int} col  ������к���
 * @return: {int} calculate(row,col) ��ǰ����ֵ
 */
int Board::getScore(int row, int col)
{
    return calculate(row, col);
}
/**
 * @name: AI_1_MAX
 * @msg: ��һ������������ʽ���أ����225�������һ�����򣬵÷ָߵ����Ƚ��еڶ�������㣬�൱���޸ĵڶ����˳�������ڼ�֦
 * ������������ʽ�������Ͷ�225�������ε��ö�������
 * ��һ�����AI�����棬����ȡ����ֵ
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
        ///����ʽ����
        for(i=1;i<N+1;i++)
        {
            for(j=1;j<N+1;j++)
            {
                orderSort[(i-1)*15+(j-1)].orderi=i;///���ṹ���Ա��ֵi/j/point
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
                    continue;//�˴�����ֱ�Ӽ���checkAround
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
                }///��һ��ȡ����ֵ
            }
        }
    }


    return;
}
/**
 * @name: AI_2_MIN
 * @msg: ģ����ֽ��ж������㣬ȡ��Сֵ
 * @param {int} score1 �ӵ�һ�����������ݣ��������ά����һ��ļ���ֵ����������ڶ���������ĵ�����ֵС��˵���Ͳ���Ҫ�����ˣ���ΪAI������ѡ���������
 * @return: {int} score ά���ڶ���ļ�Сֵ
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
            chess[i][j] = 3 - turn; ///ģ���������
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
                score = temp;///�ڶ���ȡ��Сֵ
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
 * @msg: ��������AI���棬ȡ�������������������ĵ�ȵڶ���ļ�СֵҪ��˵������������㲻����ʹ�õڶ���ļ�Сֵ��С�ˣ�����Ҫ�����ˡ�
 * ��ʵ����Сֵ�㷨��������������б�ԽǵĹ�ϵ
 * @param {int} score2 �ӵڶ��㴫������ά����Сֵ {int} tempp �ڶ��㵱ǰ����ֵ����������Ѷ�/�ٶ�
 * @return: {int} score �ӵ����㷵�ؼ���ֵ
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
 * @msg: > Ϊ���� < Ϊ���򣬽ṹ�����鰴�ճ�Աorderpoint��������
 * @param {Order} x {Order} y �����ṹ��
 * @return: bool
 */
bool Board::cmp(Order x,Order y)
{
    return (x.orderpoint) > (y.orderpoint);
}

