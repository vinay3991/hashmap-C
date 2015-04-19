#include "code.h"

static hash_size def_hashfunc(const char *key) {
	hash_size hash = 0;
	int i = 0;
	while (*key){
		hash = hash + pow(31,i)*(unsigned char) *key++;
		i++;
	}

	return hash;
}

HASHTBL *hashtbl_create(hash_size size, hash_size (*hashfunc)(const char *)) {
	HASHTBL *hashtbl;

	if (!(hashtbl = malloc(sizeof(HASHTBL))))
		return NULL;

	if (!(hashtbl->nodes = calloc(size, sizeof(struct hashnode_s*)))) {
		free(hashtbl);
		return NULL;
	}

	hashtbl->size = size;

	if (hashfunc)
		hashtbl->hashfunc = hashfunc;
	else
		hashtbl->hashfunc = def_hashfunc;

	return hashtbl;
}

int hashtbl_insert(HASHTBL *hashtbl, const char *key, void *data) {
	struct hashnode_s *node;
	hash_size hash = hashtbl->hashfunc(key) % hashtbl->size;
	printf("hash= %d \n",hash);
	node = hashtbl->nodes[hash];
	if(node)
		printf("collision detected \n");
	while (node) {
		if (!strcmp(node->key, key)) {
			node->data = data;
			return 0;
		}
		node = node->next;
	}
	if (!(node = malloc(sizeof(struct hashnode_s))))
		return -1;
	if (!(node->key = strdup(key))) {
		free(node);
		return -1;
	}
	//node->key = key;
	node->data = data;
	node->next = hashtbl->nodes[hash];
	hashtbl->nodes[hash] = node;
	return 0;
}

int hashtbl_remove(HASHTBL *hashtbl, const char *key) {
	struct hashnode_s *node, *prevnode = NULL;
	hash_size hash = hashtbl->hashfunc(key) % hashtbl->size;

	node = hashtbl->nodes[hash];
	while (node) {
		if (!strcmp(node->key, key)) {
			free(node->key);
			if (prevnode)
				prevnode->next = node->next;
			else
				hashtbl->nodes[hash] = node->next;
			free(node);
			return 0;
		}
		prevnode = node;
		node = node->next;
	}

	return -1;
}
void hashtbl_destroy(HASHTBL *hashtbl) {
	hash_size n;
	struct hashnode_s *node, *oldnode;

	for (n = 0; n < hashtbl->size; ++n) {
		node = hashtbl->nodes[n];
		while (node) {
			free(node->key);
			oldnode = node;
			node = node->next;
			free(oldnode);
		}
	}
	free(hashtbl->nodes);
	free(hashtbl);
}

void *hashtbl_get(HASHTBL *hashtbl, const char *key) {
	struct hashnode_s *node;
	hash_size hash = hashtbl->hashfunc(key) % hashtbl->size;

	/*	fprintf(stderr, "hashtbl_get() key=%s, hash=%d\n", key, hash);*/

	node = hashtbl->nodes[hash];
	while (node) {
		if (!strcmp(node->key, key))
			return node->data;
		node = node->next;
	}

	return NULL;
}

int hashtbl_resize(HASHTBL *hashtbl, hash_size size) {
	HASHTBL newtbl;
	hash_size n;
	struct hashnode_s *node, *nextnode;

	newtbl.size = size;
	newtbl.hashfunc = hashtbl->hashfunc;

	if (!(newtbl.nodes = calloc(size, sizeof(struct hashnode_s*))))
		return -1;

	for (n = 0; n < hashtbl->size; ++n) {
		for (node = hashtbl->nodes[n]; node; node = nextnode) {
			nextnode = node->next;
			hashtbl_insert(&newtbl, node->key, node->data);
			hashtbl_remove(hashtbl, node->key);
		}
	}

	free(hashtbl->nodes);
	hashtbl->size = newtbl.size;
	hashtbl->nodes = newtbl.nodes;

	return 0;
}
void change(char str[]){
	printf("%p \n",str);
}
int main() {
	HASHTBL *hashtbl;
	char *spain, *italy;
	char str[] = "GfG";
	printf("%p \n",str);
	printf("str= %s \n",str);
	change(str);

	if (!(hashtbl = hashtbl_create(16, NULL))) {
		fprintf(stderr, "ERROR: hashtbl_create() failed\n");
		exit(EXIT_FAILURE);
	}
	hashtbl_insert(hashtbl, "France", "Paris");
	hashtbl_insert(hashtbl, "England", "London");
	hashtbl_insert(hashtbl, "Sweden", "Stockholm");
	hashtbl_insert(hashtbl, "Germany", "Berlin");
	hashtbl_insert(hashtbl, "Norway", "Oslo");
	hashtbl_insert(hashtbl, "Italy", "Rome");
	hashtbl_insert(hashtbl, "Spain", "Madrid");
	hashtbl_insert(hashtbl, "Estonia", "Tallinn");
	hashtbl_insert(hashtbl, "Netherlands", "Amsterdam");
	hashtbl_insert(hashtbl, "Ireland", "Dublin");


	printf("After insert:\n");
	italy = hashtbl_get(hashtbl, "Italy");
	printf("Italy: %s\n", italy ? italy : "-");
	spain = hashtbl_get(hashtbl, "Spain");
	printf("Spain: %s\n", spain ? spain : "-");

	hashtbl_remove(hashtbl, "Spain");

	printf("After remove:\n");
	spain = hashtbl_get(hashtbl, "Spain");
	printf("Spain: %s\n", spain ? spain : "-");

	hashtbl_resize(hashtbl, 8);

	printf("After resize:\n");
	italy = hashtbl_get(hashtbl, "Italy");
	printf("Italy: %s\n", italy ? italy : "-");

	hashtbl_destroy(hashtbl);

	return 0;
}
