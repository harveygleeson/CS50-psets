#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// declare the functions
float countletters(string word);
float countwords(string word);
float countsentences(string words);
float clindex(float letters, float words, float sentences);

int main(void)
{
    string input = get_string("Text: "); // get input
    
    // calculate the totals for the input
    float total_letters = countletters(input);
    float total_words = countwords(input);
    float total_sentences = countsentences(input);
    
    float index = clindex(total_letters, total_words, total_sentences);
    int grade = round(index); // round the index
    
    // print correct grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
     
}
// calculate letters
float countletters(string word) 
{
    float total_letters = 0;
    
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if isalpha(word[i])
        {
            total_letters++;
        }
    }
    return total_letters;
}
// calculate words
float countwords(string word) 
{
    float word_count = 1;
    
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isspace(word[i]))
        {
            word_count++;
        }
    }
    return word_count;
}
// calculate sentences
float countsentences(string words) 
{
    float totalsent = 0;
    
    for (int i = 0, n = strlen(words); i < n; i++)
    {
        if (words[i] == 33 || words[i] == 46 || words[i] == 63)
        {
            totalsent++;
        }
    }
    return totalsent;
}
// calculate index
float clindex(float letters, float words, float sentences) {
    float l = (letters / words) * 100; 
    float s = (sentences / words) * 100;
    
    float index = (0.0588 * l - 0.296 * s - 15.8);
    
    return index;
}