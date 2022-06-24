// Simulate a Tideman election

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool cycle(int end, int start);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            printf("ranks = %i\n", ranks[rank]);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
            printf("preferences[%i][%i] = %i\n", ranks[i], ranks[j], preferences[ranks[i]][ranks[j]]);
        }
    }
    return;
}


// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                printf("pair_winner[%i] = %i\n", pair_count, pairs[pair_count].winner);
                printf("pair_loser[%i] = %i\n", pair_count, pairs[pair_count].loser);
                pair_count++;
                printf("number of pairs: %i\n\n", pair_count);
            }
        }
    }
    printf("pair_winner[0] = %i\n", pairs[0].winner);
    printf("pair_loser[0] = %i\n", pairs[0].loser);
    printf("pair_winner[1] = %i\n", pairs[1].winner);
    printf("pair_loser[1] = %i\n", pairs[1].loser);
    printf("pair_winner[2] = %i\n", pairs[2].winner);
    printf("pair_loser[2] = %i\n", pairs[2].loser);
    printf("number of pairs: %i\n\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
// Used bubble sort method, temporarily place array values in index pair_count + 1 while swapping values
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                pairs[pair_count + 1].winner = pairs[j].winner;     // move lower pref to count + 1
                pairs[pair_count + 1].loser = pairs[j].loser;
                pairs[j].winner = pairs[j + 1].winner;              // move higher pref to higher location
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].winner = pairs[pair_count + 1].winner; // move lower pref to lower location
                pairs[j + 1].loser = pairs[pair_count + 1].loser;
                pairs[pair_count + 1].winner = 0;                   // erase count + 1
                pairs[pair_count + 1].loser = 0;
            }
        }
    }
    printf("pair_winner[0] = %i\n", pairs[0].winner);
    printf("pair_loser[0] = %i\n", pairs[0].loser);
    printf("pair_winner[1] = %i\n", pairs[1].winner);
    printf("pair_loser[1] = %i\n", pairs[1].loser);
    printf("pair_winner[2] = %i\n", pairs[2].winner);
    printf("pair_loser[2] = %i\n", pairs[2].loser);
    printf("number of pairs: %i\n\n", pair_count);
    return;
}

// Check for potential cycles
bool cycle(int end, int start)
{
    // Return true if cycle identified
    if (end == start)
    {
        return true;
    }
    // Check candidates for cycles using recursion
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i] == true)
        {
            if (cycle(i, start) == true)
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs if no cycle
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (cycle(pairs[i].loser, pairs[i].winner) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print winner
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                count++;
                if (count == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}
