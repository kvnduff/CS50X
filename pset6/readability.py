# Computes the Coleman-Liau index of text inputted by user

from cs50 import get_string

# Text input
text = get_string("Text: ")

# Count letters
def count_letter(text):
    count = 0
    for c in text:
        if (ord(c) >= 65 and ord(c) <= 90) or (ord(c) >= 97 and ord(c) <= 122):
            count += 1
    return count

# Count words
def count_word(text):
    count = 1
    for i in range(len(text) - 1):
        if ord(text[i]) == 32 and ((ord(text[i + 1]) >= 21 and ord(text[i + 1]) <= 126)):
            count += 1
    return count

# Count sentences
def count_sentence(text):
    count = 0
    for c in text:
        if ord(c) == 33 or ord(c) == 46 or ord(c) == 63:
            count += 1
    return count

# Calculate Coleman-Liau index
def index(text):
    num_letters = count_letter(text)
    print(num_letters)
    num_words = count_word(text)
    print(num_words)
    num_sentences = count_sentence(text)
    print(num_sentences)
    # Number words per 100 letters
    letters_word = num_letters / num_words * 100
    print(letters_word)
    # Number sentences per 100 letters
    sentences_word = num_sentences / num_words * 100
    print(sentences_word)
    # Calculate index, rounding to integer
    index = int(round(0.0588 * letters_word - 0.296 * sentences_word - 15.8))
    # Print grade level according to instructions
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")

# Print Coleman-Liau index
index(text)
