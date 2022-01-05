#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    //take guess from user within correct range
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
    
    //loop as many times as the user has guessed
    for (int i = 0; i < n; i++)
    {
        //add the whitespace
        for (int k = n - i; k > 1; k--)
        {
            printf(" ");
        }
        //print one # per row, increment each time
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
    printf("\n");
    }
}