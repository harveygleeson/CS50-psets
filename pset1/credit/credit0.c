#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int odd = 0;
    int oddsum = 0;
    int even = 0;
    int add_digits = 0;
    int even_digit = 0;
    int checksum = 0;
    int firstnum = 0;
    
    long num = get_long("Number: ");
    for (int i = 0; i < 16; i++)
    {
        odd = num % 10;
        oddsum += odd;
        num /= 10;
        
        even = num % 10;
        even *= 2;
        
        for (int j = 0; j < 2; j++)
        {
           even_digit = even % 10;
           add_digits += even_digit;
           even /= 10;
        }
        num /= 10;
    }
    
    checksum = add_digits + oddsum;
    
    if (checksum % 10 == 0)
    {
        printf("VALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
    
}