#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define	MAX_WORDLEN		8

typedef struct _l {
     char        *string;
     struct _l   *next;
} LISTITEM;

//  --------------------------------------------------------------------

LISTITEM *add_list(LISTITEM *list, char *newstring)
{
    if(list == NULL) {           // append to an empty list   
        list = malloc( sizeof(LISTITEM) );
        if(list == NULL) {
            perror("add_list");
            exit(EXIT_FAILURE);
        }
        list->string  =  strdup(newstring);
        list->next    =  NULL;
    }
    else {                       // append to an existing list
        LISTITEM *p = list;

        while(p->next != NULL) { // walk to the end of the list  
	    if(strcmp(p->string, newstring) == 0) {
		return list;
	    }
            p  =  p->next;
        }
        p->next = malloc( sizeof(LISTITEM) );
        if(p->next == NULL) {
            perror("add_list");
            exit(EXIT_FAILURE);
        }
        p          =  p->next;   // append after the last item
        p->string  =  strdup(newstring);
        p->next    =  NULL;
    }
    return list;
}

void print_list(LISTITEM *list)
{
    LISTITEM	*p = list;

    while(p != NULL) {
	printf("%s", p->string);
	if(p->next != NULL)
	    printf(" -> ");
	p	= p->next;
    }
    if(list != NULL)
	printf("\n");
}

bool find_list(LISTITEM *list, char *wanted)
{
    LISTITEM	*p = list;

    while(p != NULL) {
	if(strcmp(p->string, wanted) == 0) {
//	    printf("%s\n", p->string);
	    return true;
	}
	p	= p->next;
    }
    return false;
}

//  --------------------------------------------------------------------

#define	HASHTABLE_SIZE		500000

LISTITEM *hashtable[HASHTABLE_SIZE];	// all initialised to NULL

#include <stdint.h>

//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT
//
//  see:  https://en.cppreference.com/w/c/types/integer

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

void add_hashtable(char *string)
{
    uint32_t h   = hash_string(string) % HASHTABLE_SIZE;

    hashtable[h] = add_list(hashtable[h], string);
}

bool find_hashtable(char *string)
{
    uint32_t h	= hash_string(string) % HASHTABLE_SIZE;

    return find_list(hashtable[h], string);
}

//  --------------------------------------------------------------------

void random_word(char word[], int maxlen)
{
    int len = 1 + rand() % (maxlen-1);

    for(int w=0 ; w<len ; ++w) {
        word[w] = 'a' + rand() % 26;
    }
    word[len] = '\0';
}

long microseconds(void)
{
    struct timeval  t;

    gettimeofday( &t, NULL );
    return (t.tv_sec * 1000000) + t.tv_usec;
}

int main(int argc, char *argv[])
{
    if(argc > 1) {
	int	n	= atoi(argv[1]);
	long	t0, t1;

//  ADD ITEMS
	srand(1);
	t0	= microseconds();

	for(int i=0 ; i<n ; ++i) {
	    char word[MAX_WORDLEN+1];

	    random_word(word, sizeof word);

	    add_hashtable(word);
	}

//  FIND THE SAME ITEMS
	srand(1);
	t1	= microseconds();

	for(int i=0 ; i<n ; ++i) {
	    char word[MAX_WORDLEN+1];

	    random_word(word, sizeof word);

	    find_hashtable(word);
	}
	printf("%-10s\t%10i\t%10lius\t%10lius\n",
		argv[0], n, microseconds() - t0, microseconds() - t1);
    }
    return 0;
}
