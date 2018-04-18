#include "stdio.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "skiplist.h"
#include "eq_util.h"
#include "linkedlist.h"

float randf() {
	return (float)rand() / RAND_MAX;  
}

void seed_rand() {
	srand((unsigned)time(NULL));
}

int random_level() {

	int lvl = 0;
	while (randf() < P && lvl < MAX_LEVEL)
	{
		lvl++;
	}
	return lvl;     
}

head_t* new_head() {
	head_t *head = (head_t*)safe_malloc(sizeof(head_t));
	assert(head != NULL);
	//error here since 0 isnt type snode_t
	head->header = new_snode(MAX_LEVEL, empty_eq());
	head->level = 0;
	head->num_of_elements = 0;
	head->score = 0;

	for (int i = 0; i <= MAX_LEVEL; i++) {
		head->header->next[i] = NULL;
	}

	return head;
}

snode_t* new_snode(int level, void *d) {
	snode_t *newnode;
	newnode = (snode_t*)safe_malloc(sizeof(snode_t));
	newnode->next = (snode_t**)safe_malloc((level+1)*sizeof(snode_t));
	newnode->data = d;
	return newnode;
}

void insert(head_t *head, void *d) {
	int lvl = 0;
	snode_t *temp = head->header;
	snode_t *update[MAX_LEVEL + 1];
	float mag = ((eq_t*)d)->magnitude;
	memset(update, 0, MAX_LEVEL + 1);

	for (int i = head->level; i >= 0; i--) {
		while (temp->next[i] != NULL && ((eq_t*)(temp->next[i]->data))->magnitude > mag) {
			temp = temp->next[i];
		}
		update[i] = temp;
	}

	lvl = random_level();
	if (lvl > head->level) {
		for (int j = (head->level + 1); j <= lvl; j++) {
			update[j] = head->header;
		}
		head->level = lvl;
	}

	//debug("Creating new node");
	temp = new_snode(lvl, d);
	for (int k = 0; k <= lvl; k++) {
		//debug("Swapping nodes");
		temp->next[k] = update[k]->next[k];
		update[k]->next[k] = temp;
	}
	//debug("Swapping complete");
	head->num_of_elements++;
}

void printList(head_t *head) {
	snode_t *temp = head->header;
	while (temp != NULL) {
		printf("%.2f --> ", ((eq_t*)(temp->data))->magnitude);
		temp = temp->next[0];
	}
	debug("null\n");
}

void compute_scores(head_t ***map, list_t *list, int resolution, int K) {
	long double sum = 0.0;
	snode_t *tmp = NULL;
	float score_mag = 0;
	int count = 0;

	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			if (map[i][j]->num_of_elements > 0) {
				if (map[i][j]->num_of_elements > K) {
					tmp = map[i][j]->header->next[0];
					while (tmp != NULL && count < ((K+1) / 2)) {
						score_mag = ((eq_t*)(tmp->data))->magnitude;
						sum = sum + powf(31.62278, score_mag);
						tmp = tmp->next[0];
						count++;
					}
				}
				else {
					tmp = map[i][j]->header->next[0];
					while (tmp != NULL && count < (((map[i][j]->num_of_elements)+1) / 2)) {
						score_mag = ((eq_t*)(tmp->data))->magnitude;
						sum = sum + powf(31.62278, score_mag);
						tmp = tmp->next[0];
						count++;
					}
				}
			}
			map[i][j]->score = sum;
			if (map[i][j]->score != 0.0) {
				array_insert(list, map[i][j], K);
			}
			//printf("Score is %.2lf\n", map[i][j]->score);
			sum = 0;
			count = 0;
		}
	}
}

void remove_skiplist(head_t *slist) {
	snode_t *temp;
	snode_t *curr;

	temp = slist->header;
	while (temp != NULL) {
		curr = temp;
		//remove all level here
		temp = temp->next[0];
		free(curr);
	}
	slist = NULL;
}

void array_insert(list_t *list, head_t *item, int K) {
	//node_t *temp = list->head;

	if (list->num_elements == 0) {
		list_push_front(list,item);
	}else if (list->num_elements < K) {
		list_insert_in_order(list,item,score_cmp);
	}
	else {
		list_insert_in_order(list, item, score_cmp);
		list_pop_front(list);

		if (list->num_elements != K) {
			exit(EXIT_FAILURE);
		}
	}
}

void invert(head_t **k_array, list_t* list, int K) {
	node_t* temp = list->head;

	for (int i = (K-1); i >= 0; i--) {
		k_array[i] = temp->data;
		temp = temp->next;
	}
}

void slist_free(head_t*** map, int resolution){

	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			remove_skiplist(map[i][j]);
		}
		free(map[i]);
	}
	free(map);
}

void score_print(FILE *fp, head_t *k_input) {
	eq_t *q = (eq_t*)(k_input->header->next[0]->data);
	float scr = k_input->score;

	fprintf(fp, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ,%.3f,%.3f,%.1f,%.2lf\n",
		q->timestamp->year,
		q->timestamp->month,
		q->timestamp->day,
		q->timestamp->hour,
		q->timestamp->min,
		q->timestamp->sec,
		q->timestamp->msec,
		q->latitude,
		q->longitude,
		q->magnitude,
		scr);
}