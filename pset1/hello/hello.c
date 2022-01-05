#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask for user input name and print greeting
    string name = get_string("What's your name?\n");
    printf("Hello %s\n", name);
    int n = get_int("> ");
    int m = n/2;
    printf("%i\n", m);
}
