// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 8649;

// Hash table
node *table[N];

// Dictionary size
int d_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word
    int h_value = hash(word);

    // Access hash table linked list at hash value location
    node *c = table[h_value];

    // Traverse linked list and return true if word found
    while (c != NULL)
    {
        if (strcasecmp(word, c->word) == 0)
        {
            return true;
        }
        c = c->next;
    }
    return false;
}

// Hash word to number
unsigned int hash(const char *word)
{
    // If word more than one letter than multiply first two ASCII codes
    if (strlen(word) >= 2)
    {
        return (tolower(word[0]) - 33) * (tolower(word[1]) - 33);
    }

    // If word one letter than return ASCII code
    else
    {
        return tolower(word[0]) - 33;
    }
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *d_pointer = fopen(dictionary, "r");
    if (d_pointer == NULL)
    {
        printf("Unable to open %s\n.", dictionary);
        return false;
    }

    // Vairable for scanned words
    char *w_scan[LENGTH + 1];

    // Scan words from dictionary
    while (fscanf(d_pointer, "%s", w_scan) != EOF)
    {

        // Create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copy word into node
        strcpy(n->word, w_scan);

        // Hash word
        int h_value = hash(w_scan);

        // Insert node into hash table
        n->next = table[h_value];
        table[h_value] = n;
        d_size++;
    }

    // Close dictionary
    fclose(d_pointer);

    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return d_size;
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *c = table[i];
        while (c != NULL)
        {
            node *tmp = c;
            c = c->next;
            free(tmp);
        }
        if (c == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
