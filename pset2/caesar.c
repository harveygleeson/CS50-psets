#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    int key;
    string input;
    
    if (argc == 2) // check for only 1 arg
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isalpha(argv[1][i]))
            {
                printf("Usage: ./caesar key\n");
                break;
            }
            else
            {
                break;
            }
        }
        
        key = atoi(argv[1]);    // convert key to int and then ask for input
        input = get_string("plaintext: ");
                
        for (int j = 0, m = strlen(input); j < m; j++)
        {
            if isalpha(input[j])
            {
                if isupper(input[j])
                {
                    input[j] = ((input[j] - 65 + key) % 26) + 65; //cipher
                }
                if islower(input[j])
                {
                    input[j] = ((input[j] - 97 + key) % 26) + 97; // cipher
                }
            }    
            else
            {
                input[j] = input[j]; // for other characters
            }
        }
        printf("ciphertext: %s\n", input); // output
        return 0;
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}