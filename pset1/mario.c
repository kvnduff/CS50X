// Prints mario-style pyramids according to user inputted height

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for height between 1 and 8
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    // Create variables for number of spaces and hashes
    int space = height - 1;
    int hash = 1;
    {
    // Iterate through pyramid levels
    for (int i = 0; i < height; i++)
    {
        // Print left-side spaces
        for (int j = 0; j < space; j++)
        {
            printf(" ");
        }
        // Print left-side hashes
        for (int k = 0; k < hash; k++)
        {
            printf("#");
        }
        // Print central spaces
        printf("  ");
        // Print right-side hashes
        for (int l = 0; l < hash; l++)
        {
            printf("#");
        }
        // Print line return for next level and iterate variables
        printf("\n");
        space--;
        hash++;
        }
    }
}
