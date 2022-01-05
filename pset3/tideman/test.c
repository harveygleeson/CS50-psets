#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int winner;
    int loser;
}
pair;

pair pairs[5];

int main(void)
{
    pairs[0].winner = 2;
    pairs[0].loser = 3;
    pairs[1].winner = 1;
    printf("Pairs winner: %i\n", pairs[0]);
}