#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* node type */
typedef struct node node_t;

struct node {
    void* data;
    node_t* next;
};

/* linked list type */
typedef struct {
    int num_elements;
    node_t* head;
    node_t* tail;
    void (*del)(void*);
} list_t;

void list_process_all(node_t* p, void (*process)(void*));
void* list_pop_front(list_t* list);
void list_push_front(list_t* list, void* d);
void list_push_back(list_t* list, void* d);
list_t* list_new(void (*delfunc)(void*));
void list_free(list_t* list);
void list_insert_in_order(list_t* list, void* d, int cmp(const void*, const void*));
void list_delete(void * item);
int magnitude_cmp(const void *eq_1, const void *eq_2);
int score_cmp(const void *score1, const void *score2);
node_t* new_node(void *d);
void print_list(void *d);
void filter_quake(list_t *list, void *eq);
void traverse_list_n_match(list_t *list, void *eq);

#endif