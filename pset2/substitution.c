// Encrypt messages using a substitution cipher

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string cipher_tran(string plaintext, string key);
string cipher_case(string plaintext, string ciphertext);

int main(int argc, string argv[])
{
    // Store key in string variable
    string key = argv[1];
    // Check number of command line arguments (1 required)
    if (argc != 2)
    {
        printf("Usage: ./substitution must contain 1 argument\n");
        return 1;
    }
    // Check key length (26 characters required)
    if (strlen(argv[1]) != 26)
    {
        printf("Usage: key must be 26 characters\n");
        return 1;
    }
    // Check character type (must be alphabetical)
    string arg = argv[1];
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(arg[i]) == 0)
        {
            printf("Usage: key must contain only alphabetical characters\n");
            return 1;
        }
    }
    // Check for duplicate key characters (must be unique)
    for (int i = 0; i < 25; i++)
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (key[i] == key[j])
            {
                printf("Usage: key must contain every letter of the alphabet once\n");
                return 1;
            }
        }
    }

    // Prompt user for plaintext
    string plaintext = get_string("plaintext:  ");

    // Translate plaintext to ciphertext
    string ciphertext = cipher_tran(plaintext, key);

    // Retain plaintext case in ciphertext
    ciphertext = cipher_case(plaintext, ciphertext);
    printf("ciphertext: %s\n", ciphertext);
}

// Translate plaintext to ciphertext
string cipher_tran(string plaintext, string key)
{
    char cipher[strlen(plaintext)];
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (islower(plaintext[i]))
        {
            cipher[i] = key[plaintext[i] - 97];
        }
        else if (isupper(plaintext[i]))
        {
            cipher[i] = key[plaintext[i] - 65];
        }
        else if (!isalpha(plaintext[i]))
        {
            cipher[i] = plaintext[i];
        }
    }
    string ciphertext = cipher;
    return ciphertext;
}

// Retain case of plaintext in ciphertext
string cipher_case(string plaintext, string ciphertext)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (islower(ciphertext[i]) && isupper(plaintext[i]))
        {
            ciphertext[i] = ciphertext[i] - 32;
        }
        else if (isupper(ciphertext[i]) && islower(plaintext[i]))
        {
            ciphertext[i] = ciphertext[i] + 32;
        }
    }
    return ciphertext;
}
