// Computes the Coleman-Liau index of text inputted by user

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int index(string text);

int main(void)
{
    // Get text from user and print
    string text = get_string("Text: ");
    // Print CL grade according to requested convention
    int grade = index(text);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// Count number of letters
int count_letters(string text)
{
    int num = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isupper(text[i]) != 0 || islower(text[i]) != 0)
        {
            num++;
        }
    }
    return num;
}

// Count number of words
int count_words(string text)
{
    int num = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 21 && text[i] <= 126) && text[i + 1] == 32)
        {
            num++;
        }
    }
    num++;
    return num;
}

// Count number of sentences
int count_sentences(string text)
{
    int num = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((isupper(text[i]) != 0 || islower(text[i]) != 0) &&
            (text[i + 1] == 46 || text[i + 1] == 33 || text[i + 1] == 63))
        {
            num++;
        }
    }
    return num;
}

// Calculate Coleman-Liau index
int index(string text)
{
    int num_letters = count_letters(text);
    int num_words = count_words(text);
    int num_sentences = count_sentences(text);
    // Number words per 100 letters
    float letters_word = (float) num_letters / (float) num_words * 100;
    // Number sentences per 100 letters
    float sentences_word = (float) num_sentences / (float) num_words * 100;
    // Calculate index, rounding to integer
    float ind_float = round(0.0588 * (float) letters_word - 0.296 *
		(float) sentences_word - 15.8);
    // Convert index to integer
    int index = (int) ind_float;
    return index;
    printf("%i\n", index);
}
