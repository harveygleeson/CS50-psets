// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Word counter
int word_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    
    int index = hash(word);

    node *cursor = table[index];
    
    while (strcasecmp(cursor -> word, word) != 0)
    {
        if (cursor -> next == NULL)
        {
            return false;
        }
        cursor = cursor -> next;
    } 
 
    return true; 
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    int c = 0;

    while (c == toupper(*word++))
    {
        hash = ((hash << 5) + hash) + c; 
    }
    
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    
    if (fp == NULL)
    {
        return false;
    }
    
    char buffer[LENGTH + 1];
    
    while (fscanf(fp, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        
        if (n == NULL)
        {
            free(n);
            fclose(fp);
            return false;
        }
        
        strcpy(n -> word, buffer);
        n -> next = NULL;
        
        int index = hash(buffer);
        
        
        n -> next = table[index];
        table[index] = n;
        word_counter++;


    }
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        
        while (cursor != NULL)
        {
            node *tmp = cursor;
            
            cursor = cursor -> next;
            free(tmp);
        }
        
    }
    return true;
}
