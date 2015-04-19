#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <math.h>

//HASHTBL is the primary reference to the hash table. It contains all information necessary to manipulate and look up the elements. All hash table functions expect a pointer to HASHTBL.

typedef size_t hash_size;
//<<HASHTBL>>=
typedef struct hashtbl {
	hash_size size;
	struct hashnode_s **nodes;
	hash_size (*hashfunc)(const char *);
} HASHTBL;
//The nodes member of HASHTBL is an array of pointers to the first element of a linked list. This element is represented by struct hashnode_s:

//<<node>>=
struct hashnode_s {
	char *key;
	void *data;
	struct hashnode_s *next;
};

//<<functions>>=
HASHTBL *hashtbl_create(hash_size size, hash_size (*hashfunc)(const char *));
void hashtbl_destroy(HASHTBL *hashtbl);
int hashtbl_insert(HASHTBL *hashtbl, const char *key, void *data);
int hashtbl_remove(HASHTBL *hashtbl, const char *key);
void *hashtbl_get(HASHTBL *hashtbl, const char *key);
int hashtbl_resize(HASHTBL *hashtbl, hash_size size);
