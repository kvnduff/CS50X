# Credit card verification based on Luhn's algorithm (# digits, number prefix, see notes below)

from cs50 import get_int

# Get credit card number from user
number = get_int("Number: ")

# Determine card type according to card number


def type(number):
    if (number // 10000000000000 == 34) or (number // 10000000000000 == 37):
        return "AMEX"
    elif (number // 100000000000000 >= 51) and (number // 100000000000000 <= 55):
        return "MASTERCARD"
    elif (number // 1000000000000 == 4) or (number // 1000000000000000 == 4):
        return "VISA"
    else:
        return "INVALID"


# Print type
print(type(number))