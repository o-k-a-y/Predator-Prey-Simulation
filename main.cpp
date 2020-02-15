#include "board.h"

int main(int argc, char **argv)
{
    Board *b = Board::getInstance(argv[1]);
    b->simulate();

    return 0;
}