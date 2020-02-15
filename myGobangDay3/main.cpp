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

