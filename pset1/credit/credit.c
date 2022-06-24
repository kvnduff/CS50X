// Credit card verification based on Luhn's algorithm (# digits, number prefix,
// see notes below)

#include <cs50.h>
#include <stdio.h>

string type_init(long num);
int dig_sum1(long num);
int dig_sum2(long num);

int main(void)
{
    // Get credit card number from user
    long num = get_long("Number: ");
    // Determine initial card type based on card prefix and length
    string type = type_init(num);
    // Start second last digit, multiply every second digit by 2, sum products
    int sum1 = dig_sum1(num);
    // Add sum of digits not multiplied by 2
    int sum2 = dig_sum2(num);
    // Change type to invalid if modulo of sum of sums is not 0
    int mod = (sum1 + sum2) % 10;
    if ((sum1 + sum2) % 10 != 0)
    {
        type = "INVALID";
    }
    // Print credit card type / validity
    printf("%s\n", type);
}

// Determine initial card type based on card prefix and length
string type_init(long num)
{
    if (num / 10000000000000 == 34 || num / 10000000000000 == 37)
    {
        return "AMEX";
    }
    else if (num / 100000000000000 >= 51 && num / 100000000000000 <= 55)
    {
        return "MASTERCARD";
    }
    else if (num / 1000000000000 == 4 || num / 1000000000000000 == 4)
    {
        return "VISA";
    }
    else
    {
        return "INVALID";
    }
}

// Start second last digit, multiply every second digit by 2, sum products
int dig_sum1(long num)
{
    int dig;
    long num_new = num;
    int dig_sum = 0;
    for (int i = 0; i < 10; i++)
    {
        dig = (num_new / 10) % 10 * 2;
        num_new = (num_new - dig) / 100;
        if (dig >= 10)
        {
            int dig0 = dig % 10;
            int dig1 = dig / 10;
            dig_sum += dig0 + dig1;
        }
        else
        {
            dig_sum += dig;
        }
    }
    return dig_sum;
}

// Add sum of digits not multiplied by 2
int dig_sum2(long num)
{
    int dig_oth;
    int dig_sum_oth = 0;
    long num_oth = num;
    for (int i = 0; i < 10; i++)
    {
        dig_oth = num_oth % 10;
        num_oth = (num_oth - dig_oth) / 100;
        dig_sum_oth += dig_oth;
    }
    return dig_sum_oth;
}
