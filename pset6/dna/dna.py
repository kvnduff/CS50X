# Identify DNA match using STR sequences

import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py db_filename dna_filename")
        sys.exit(1)

    # Store file paths
    db_path = sys.argv[1]
    dna_path = sys.argv[2]

    # Read database file into a list
    db_file = csv.DictReader(open(db_path, "r"))
    db_list = list(db_file)

    # Read dna file into a variable
    dna_file = open(dna_path, "r")
    dna_var = dna_file.read()

    # Find longest match of each STR in DNA sequence
    long_match = []
    for i in range(1, len(db_file.fieldnames)):
        long_match += [longest_match(dna_var, db_file.fieldnames[i])]

    # Check database for matching profiles

    # Iterate over profiles
    for i in range(0, len(db_list)):

        # Iterate over STRs
        num_matches = 0
        for j in range(1, len(db_file.fieldnames)):

            # If STR long_match equals profile STR length then count match
            if int(long_match[j - 1]) == int(db_list[i][db_file.fieldnames[j]]):
                num_matches += 1

                # If number matches equals number of STR then return matching profile
                if num_matches == len(long_match):
                    print(db_list[i][db_file.fieldnames[0]])
                    sys.exit(0)

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
