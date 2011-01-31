/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/hash.c,v 1.1 2001/05/01 21:23:10 emery Exp $
 *
 * $Log: hash.c,v $
 * Revision 1.1  2001/05/01 21:23:10  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <stdio.h>
#include <malloc.h>
#include <hash.h>

typedef struct _HashEntry {
    unsigned long	key;
    void    		*value; 
    struct _HashEntry   *next;
    } HashEntry, *HashEntryPtr;

struct _HashTable {
    unsigned long   prime;
#if HASH_STATS
    unsigned long   num_entries;
    unsigned int    *entry_counts;
#endif
    HashEntryPtr    *entries;
    };

#define NUM_PRIMES 20
static unsigned long candidate_primes[] = { 7, 17, 31, 61, 127, 257, 509,
                                            1031, 2053, 4111, 8287, 16097,
					    32083, 64157, 128021, 256517,
					    512287, 1024511, 2048239, 4096583 };

static HashEntryPtr free_list = NULL;


HashTable
Hash_New_Table(unsigned long maxsize)
{
    HashTable   new_hash = (HashTable)malloc(sizeof(struct _HashTable));
    int i;

    for ( i = 0 ; i < NUM_PRIMES - 1 && candidate_primes[i] < maxsize ; i++ );
    new_hash->prime = candidate_primes[i];
    new_hash->entries =
	(HashEntryPtr*)malloc(sizeof(HashEntryPtr) * new_hash->prime);
    for ( i = 0 ; i < new_hash->prime ; i++ )
        new_hash->entries[i] = NULL;

#if HASH_STATS
    new_hash->num_entries = 0;
    new_hash->entry_counts =
		(unsigned int*)malloc(sizeof(unsigned int) * new_hash->prime);
    for ( i = 0 ; i < new_hash->prime ; i++ )
        new_hash->entry_counts[i] = 0;
#endif

    return new_hash;
}


void*
Hash_Insert(HashTable table, unsigned long key, void *value)
{
    int             index = key % table->prime;
    HashEntryPtr    entry = table->entries[index];
    HashEntryPtr    new_entry;

    while ( entry )
    {
        if ( entry->key == key )
            return entry->value;
        entry = entry->next;
    }
    if ( free_list )
    {
	new_entry = free_list;
	free_list = free_list->next;
    }
    else
	new_entry = (HashEntryPtr)malloc(sizeof(HashEntry));
    new_entry->next = table->entries[index];
    new_entry->key = key;
    new_entry->value = value;
    table->entries[index] = new_entry;

#if HASH_STATS
    table->num_entries++;
    table->entry_counts[index]++;
    if ( table->num_entries >= table->prime )
	fprintf(stderr, "Hash table is full: %d %d\n",
		table->num_entries, table->prime);
#endif

    return (void*)-1;
}


void*
Hash_Get_Value(HashTable table, unsigned long key)
{
    HashEntryPtr    entry = table->entries[key % table->prime];

    while ( entry && entry->key != key )
        entry = entry->next;
    if ( entry )
        return entry->value;
    else
        return (void*)-1;
}


static void
Hash_Free_List(HashEntryPtr victim)
{
    if ( victim )
    {
        Hash_Free_List(victim->next);
	victim->next = free_list;
	free_list = victim;
    }
}


void
Hash_Clear(HashTable table)
{
    int i;

    for ( i = 0 ; i < table->prime ; i++ )
        if ( table->entries[i] )
	{
	    Hash_Free_List(table->entries[i]);
	    table->entries[i] = NULL;
	}

#if HASH_STATS
    table->num_entries = 0;
    for ( i = 0 ; i < table->prime ; i++ )
	table->entry_counts[i] = 0;
#endif

}

void
Hash_Free(HashTable table)
{
    Hash_Clear(table);
    free(table);
}


void*
Hash_Delete(HashTable table, unsigned long key)
{
    HashEntryPtr    entry = table->entries[key % table->prime];
    void            *val;

    if ( ! entry )
        return (void*)-1;

    if ( entry->key == key )
    {
        table->entries[key % table->prime] = entry->next;
        val = entry->value;
	entry->next = free_list;
	free_list = entry;

#if HASH_STATS
    table->num_entries--;
    table->entry_counts[index]--;
#endif

        return val;
    }

    while ( entry->next && entry->next->key != key )
        entry = entry->next;

    if ( entry->next && entry->next->key == key )
    {
        HashEntryPtr    temp = entry->next;

        val = entry->next->value;
        entry->next = entry->next->next;
	temp->next = free_list;
	free_list = temp;

#if HASH_STATS
    table->num_entries--;
    table->entry_counts[index]--;
#endif

        return val;
    }

    return (void*)-1;
}


void*
Hash_Traverse(HashTable table, int starting)
{
    static HashEntryPtr next;
    static int          index;

    if ( starting )
    {
        for ( index = 0 ;
	      index < table->prime && ! table->entries[index] ; index++ );
        if ( index < table->prime )
            next = table->entries[index];
        else
            next = NULL;
    }
    else
    {
	if ( next && next->next )
	    next = next->next;
	else
	{
	    for ( index++ ;
		  index < table->prime && ! table->entries[index] ; index++ );
	    if ( index < table->prime )
		next = table->entries[index];
	    else
		next = NULL;
	}
    }

    return ( next ? next->value : (void*)-1 );
}


#if HASH_STATS

void
Hash_Print_Stats(HashTable table, FILE *f)
{
    int	i;
    fprintf(f, "%d entries\n", table->num_entries);
    for ( i = 0 ; i < table->prime ; i++ )
	fprintf(f, "%d\n", table->entry_counts[i]);
}

#endif


