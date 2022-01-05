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
void lock_pairs(void);
void print_winner(void);
void merge(int pair[], int l, int m, int r);
void mergesort(int pair[], int l, int r);


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
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(name, candidates[k]) == 0)
        {
            ranks[rank] = k;
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
        for (int j = 1 + i; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0 + i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {    
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }    
            else if (preferences[j][i] > preferences[i][j])
            {    
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }    
        }
    }    
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    mergesort(pairs, 0, pair_count - 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    locked[pairs[0].winner][pairs[0].loser] = true;
    
    for (int i = 1; i < pair_count; i++)
    {
        for (int j = i - 1; j > -1; j--)
        {
            if (pairs[i].loser == pairs[j].winner)
                locked[pairs[i].winner][pairs[i].loser] = false;
                break;
        }
        locked[pairs[i].winner][pairs[i].loser] = true;
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[j][i] == true)
            {
                break;
            }
            else
            {
                return locked[j];
            }
        }
    }
}

void merge(pair[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    pair L[n1], R[n2]; //declare two subarrays
    
    for (int i = 0; i < n1; i++)
    {
        L[i] = pair[l + i];      //populate subarrays with half of main array each
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = pair[m + 1 + j];
    }
    
    i = 0; //pointers for subarrays and main array
    j = 0;
    k = l;
    
    while (i < n1 && j < n2) //while both arrays have content
    {
        if (preferences[L[i].winner][L[i].loser] >= preferences[R[j].winner][R[j].loser])   //if the value at current pointer for L is smaller than R
            arr[k] = L[i];      //populate the main array with that value
            i++;
        else
            arr[k] = R[j];
            j++
        k++;
    } 
    
    while (i < n1)      //if R runs out, fill main array with L
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2)      // same as above but fill with R
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(pair[], int l, int r)
{
    if (l < r)
    {
        m = l + (r - l) / 2;
        
        mergesort(arr, l, m);
        mergesort(arr, m + 1, r);
        
        merge(arr, l, m, r);
    }
}