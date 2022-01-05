#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars;
    int coins = 0;
    
    do //get user input
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    
    int cents = round(dollars * 100); //convert to cents
    while (cents >= 25) //count quarters
    {
        cents = cents - 25;
        coins++;
    }
    while (cents >= 10) //count dimes
    {
        cents = cents - 10;
        coins++;
    }
    while (cents >= 5) //count nickels
    {
        cents = cents - 5;
        coins++;
    }
    while (cents >= 1) //count pennies
    {
        cents = cents - 1;
        coins++;
    }
    printf("%i\n", coins); //print
}