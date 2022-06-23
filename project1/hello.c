// Prompts user for name and prints name

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What is your name?");
    printf("Your name is: %s\n", name);
}
