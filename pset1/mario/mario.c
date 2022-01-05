#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    //ask user for input
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
    
    for (int i = 0; i < n; i++) //loop as many times as input
    {
        for (int k = 1; k < n - i; k++) //create whitespace before
        {
            printf(" ");
        }
        for (int j = 0; j < i + 1; j++) //create pyramid 1
        {
            printf("#");
        }
        printf("  "); // create gap
        for (int k = 0; k < i + 1; k++) //create pyramid 2
        {
            printf("#");
        }
        printf("\n");
    }
}