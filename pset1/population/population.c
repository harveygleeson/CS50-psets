#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt for start size, must be bigger than 9
    int startSize;
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);
    //Prompt for end size which must be greater or equal to start size
    int endSize;
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);
    //Calculate number of years until we reach threshold and print
    int years = 0;
    while (startSize < endSize) 
    {
        startSize = startSize + startSize/3 - startSize/4;
        years = years + 1;
    }
    printf("Years: %i\n", years);
}