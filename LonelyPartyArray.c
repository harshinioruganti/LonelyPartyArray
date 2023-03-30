// Harshini Oruganti
// COP3502, Spring 2021, ha758478

#include "LonelyPartyArray.h"
#include <stdio.h>
#include <stdlib.h>

// creates LPA and initializes members
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
    int i;

    if (num_fragments <= 0 || fragment_length <= 0)
    {
        return NULL;
    }

    LonelyPartyArray *arr = malloc(sizeof(LonelyPartyArray));

    arr->num_fragments = num_fragments;
    arr->fragment_length = fragment_length;

    arr->num_active_fragments = 0;
    arr->size = 0;

    arr->fragments = malloc(sizeof(int*) * num_fragments);
    for (i = 0; i < arr->num_fragments; i++)
      arr->fragments[i] = NULL;

    arr->fragment_sizes = malloc(sizeof(int) * num_fragments);
    for (i = 0; i < arr->num_fragments; i++)
      arr->fragment_sizes[i] = 0;

    if (arr != NULL)
    {
        printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", arr->num_fragments * arr->fragment_length, num_fragments);
        return arr;
    }
    else
    {
        return NULL;
    }
}

// frees LPA and all allocated memory
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
          free(party->fragments[i]);
    }

    free(party->fragments);
    free(party->fragment_sizes);

    free(party);

    if (party != NULL)
    {
        printf("-> The LonelyPartyArray has returned to the void.\n");
    }

    return NULL;
}

// set corresponding index to key
int set(LonelyPartyArray *party, int index, int key)
{
    int row, col;
    int i;

    // party is null
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    row = index / (party->fragment_length);
    col = index % (party->fragment_length);

    // party in non-NULL and index is invalid
    if (!(index >= 0 && index <= party->num_fragments * party->fragment_length - 1))
    {
        printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // allocation of a new fragment in memory
    if (party->fragments[row] == NULL)
    {
        party->fragments[row] = malloc(sizeof(int) * party->fragment_length);

        for (i = 0; i < party->fragment_length; i++)
        {
            party->fragments[row][i] = UNUSED;
        }

        party->fragments[row][col] = key;
        party->num_active_fragments += 1;
        party->fragment_sizes[row] += 1;
        party->size += 1;

        printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", row, party->fragment_length, row * party->fragment_length, ((row + 1) * party->fragment_length) - 1);
    }
    // if col is unassigned
    else
    {
        if(party->fragments[row][col] == UNUSED)
        {
            party->fragments[row][col] = key;
            party->fragment_sizes[row] += 1;
            party->size += 1;
        }
        else
        {
            party->fragments[row][col] = key;
        }
  }

    return LPA_SUCCESS;
}

// get value at index
int get(LonelyPartyArray *party, int index)
{
    int row, col;

    // party is null
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    row = index / (party->fragment_length);
    col = index % (party->fragment_length);

    // party in non-NULL and index is invalid
    if (!(index >= 0 && index <= party->num_fragments * party->fragment_length - 1))
    {
        printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // party is NULL
    if (party->fragments[row] == NULL)
    {
        return UNUSED;
    }

    return party->fragments[row][col];
}

// deleted value stored at index
// if removal of value at index causes fragment to be empty, fragment is freed
int delete(LonelyPartyArray *party, int index)
{
    int row, col;

    // party is null
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    row = index / (party->fragment_length);
    col = index % (party->fragment_length);

    // if party is non-NULL and index is invalid
    if (!(index >= 0 && index <= party->num_fragments * party->fragment_length - 1))
    {
        printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // if fragment doesn't exist
    if (party->fragments[row] == NULL)
    {
        return LPA_FAILURE;
    }

    // corresponding fragment exists, corresponding cell doesnt hold value
    if (party->fragments[row][col] == UNUSED)
    {
        return LPA_FAILURE;
    }

    // if cell is not UNUSED
    party->fragments[row][col] = UNUSED;
    party->size -= 1;
    party->fragment_sizes[row] -= 1;

    // fragment becomes empty
    if (party->fragment_sizes[row] == 0)
    {
        free(party->fragments[row]);
        party->fragments[row] = NULL;
        party->num_active_fragments -= 1;

        printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", row, party->fragment_length, row * party->fragment_length, ((row + 1) * party->fragment_length) - 1);
    }

    return LPA_SUCCESS;
}

// determines whether key is located in a LPA fragment
int containsKey(LonelyPartyArray *party, int key)
{
    int i, j;

    if (party == NULL)
    {
      return 0;
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        for (j = 0; j < party->fragment_length; j++)
        {
            if (party->fragments[i][j] == key)
            {
                return 1;
            }
        }
    }

    return 0;
}

// determines whether a non-UNUSED value is stored at index
int isSet(LonelyPartyArray *party, int index)
{
    int row, col;

    row = index / (party->fragment_length);
    col = index % (party->fragment_length);

    // index is invalid
    if (!(index >= 0 && index <= party->num_fragments * party->fragment_length - 1))
    {
        return 0;
    }
    // fragment has not been alllocated
    else if (party->fragments[row] == NULL)
    {
        return 0;
    }
    // index is UNUSED
    else if (party->fragments[row][col] == UNUSED)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// prints value stored in index of LonelyPartyArray
int printIfValid(LonelyPartyArray *party, int index)
{
    if (party == NULL)
    {
        return LPA_FAILURE;
    }

    int row = index / (party->fragment_length), col = index % (party->fragment_length);

    if (!(index >= 0 && index <= party->num_fragments * party->fragment_length - 1))
    {
        return LPA_FAILURE;
    }
    else if (party->fragments[row] == NULL)
    {
        return LPA_FAILURE;
    }
    else if (party->fragments[row][col] == UNUSED)
    {
        return LPA_FAILURE;
    }
    else
    {
        printf("%d\n", party->fragments[row][col]);
        return LPA_SUCCESS;
    }
}

// resets lonelyPartyArray to state when it was first created
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
     int i;

    // party is NULL
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
    }

    // party is non-NULL
    if (party != NULL)
    {
        printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", party->num_fragments * party->fragment_length, party->num_fragments);
    }

    //  resetting LPA
    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
          free(party->fragments[i]);
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        party->fragments[i] = NULL;
    }

    for (i = 0; i < party->num_fragments; i++)
    {
        party->fragment_sizes[i] = 0;
    }

    party->size = 0;
    party->num_active_fragments = 0;

    return party;
}

// get function for party->size
int  getSize(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return -1;
    }

    return party->size;
}

// maximum number of elements that party can hold
int getCapacity(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return -1;
    }

    return party->num_fragments * party->fragment_length;
}

// maximum number of elements that can be held by active fragments in party
int getAllocatedCellCount(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return -1;
    }

    return party->num_active_fragments * party->fragment_length;
}

// total bytes that standard array of maximum elements that party can hold would use
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return 0;
    }

    return (long long unsigned int)getCapacity(party) * sizeof(int);
}

// number of bytes taken up in memory by the LPA
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
    int i;
    int total = 0;
    if (party == NULL)
    {
        return 0;
    }

    // bytes taken up by LPA pointer
    total += (sizeof(party));

    // bytes taken up by LPA struct
    total += (sizeof(LonelyPartyArray));

    // bytes taken up by fragments array
    total += (sizeof(long long unsigned int) * party->num_fragments);

    // bytes taken up by fragment_sizes array
    total += (sizeof(int) * party->num_fragments);

    // bytes taken up by active fragments
    total += ((sizeof(int) * party->fragment_length) * party->num_active_fragments);

    return total;
}

// creates a party clone
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
    int i, j;
    LonelyPartyArray *clone = malloc(sizeof(LonelyPartyArray));

    // party is NULL
    if (party == NULL)
    {
        return NULL;
    }

    // call to malloc fails
    if (clone == NULL)
    {
        free(clone);
        return NULL;
    }

    // creates copies of data contained in party
    clone->num_fragments = party->num_fragments;
    clone->fragment_length = party->fragment_length;
    clone->size = party->size;
    clone->num_active_fragments = party->num_active_fragments;
    clone->fragments = malloc(sizeof(int*) * clone->num_fragments);
    clone->fragment_sizes = malloc(sizeof(int) * clone->num_fragments);

    // copy of fragments array
    for (i = 0; i < clone->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
        {
            clone->fragments[i] = malloc(sizeof(int) * clone->fragment_length);
        }
    }

    // copying each fragment
    for (i = 0; i < clone->num_fragments; i++)
    {
        for (j = 0; j < clone->fragment_length; j++)
        {
            if (clone->fragments[i] != NULL)
            {
                clone->fragments[i][j] = party->fragments[i][j];
            }
        }
    }

    // copy of fragment_sizes array
    for (i = 0; i < clone->fragment_length; i++)
    {
        clone->fragment_sizes[i] = party->fragment_sizes[i];
    }

    // party is non-null
    if (party != NULL)
    {
        printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n", party->num_fragments * party->fragment_length, party->num_fragments);
    }

    return clone;
}

// difficulty of assignment on scale of 1.0 to 5.0
double difficultyRating(void)
{
    return 4.5;
}

// total hours spent on LonelyPartyArray programming assignment
double hoursSpent(void)
{
    return 17;
}
