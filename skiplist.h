#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linkedlist.h"

#define MAX_LEVEL 20
#define P 0.5



struct snode{
	void *data;
	struct snode **next;
};

typedef struct snode snode_t;

typedef struct {
	snode_t* header;
	int level;
	int num_of_elements;
	long double score;
}head_t;

float randf();
int random_level();
void seed_rand();
head_t* new_head();
snode_t* new_snode(int level, void *d);
void insert(head_t *head, void *d);
void printList(head_t *head);
void compute_scores(head_t ***map, list_t *list, int resolution, int K);
void slist_free(head_t*** map, int resolution);
void array_insert(list_t *list, head_t *item, int K);
void invert(head_t **k_array, list_t* list, int K);
void score_print(FILE *fp, head_t *k_input);

#endif