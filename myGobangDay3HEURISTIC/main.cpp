/*
 * @Descripttion: main����������Boardͷ�ļ���������Board����ĳ�Ա��������
 * @version: ���������������ʽ�����汾������ʽ����+alpha-beta��֦+����Сֵ�㷨��
 * @Author: ��ҫ�� u201811260@hust.edu.cn
 * @LastEditTime: 2020-02-16 08:58:47
 */
#include "Board.h"
using namespace std;

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

