#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long lastdigit = 0;
    int x = 0;
    int y = 0;
    
    //get number
    long num = get_long("Number: ");
    
    for (int i = 0; i < 16; i++)
    {
        num /= 10;
        lastdigit = num % 10;
        lastdigit *= 2;
        num /= 10;   
        
        for (int j = 0; j < 2; j++)
        {
            x = lastdigit % 10;
            y += x;
            lastdigit /= 10;
        }
    }
    if (y % 10 == 0) {
        printf("VALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
} 

