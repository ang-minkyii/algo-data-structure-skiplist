/***************************************************************************
 *
 *   File        : tasks.c
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/

#include "eq_util.h"
#include "skiplist.h"
#include "tasks.h"
#include "linkedlist.h"
#include "time.h"

#define MAXINPUT 100000
#define MAXYEART3 2015
#define MINYEART3 1985
#define MINYEART1 2000

void task_1_find_longest_break_after_2k(const char* eq_csv_file)
{
	FILE *input_file = NULL;
	eq_t *temp = NULL;
	int count = 0;
	eq_t *eq_array;
	//eq_t *head = NULL;
	int data_read = 0;
	int mem_realloc_count = 1;
	int index_of_longest_time = 0;

	if (eq_csv_file == NULL) {
		debug("Empty file");
		exit(EXIT_FAILURE);
	}
	else {
		eq_array = (eq_t*)safe_malloc(MAXINPUT*sizeof(eq_t));
		input_file = safe_open_file(eq_csv_file, "r");

		if (input_file != NULL) {
			while (1){
				temp = read_quake(input_file);
				if (temp != NULL){
					count += 1;
					if (temp->timestamp->year >= MINYEART1) {
						eq_array[data_read] = *temp;
						data_read += 1;
					}
				}
				else {
					break;
				}

				if (data_read == mem_realloc_count*MAXINPUT) {
					mem_realloc_count++;
					eq_array = safe_realloc(eq_array, mem_realloc_count*MAXINPUT * sizeof(eq_t));
				}
			}
			
			qsort(&eq_array[0], data_read, sizeof(eq_t), compare);
			consecutive_eq(eq_array, data_read, &index_of_longest_time);

			FILE* fp = safe_open_file("task_1.csv","w");
			fprintf(fp, "timestamp,latitide,longitude,magnitude\n");
			eq_print(fp,&eq_array[index_of_longest_time]);
			eq_print(fp,&eq_array[index_of_longest_time + 1]);
			fclose(fp);	

			eq_free(eq_array);
		}
	}
}

void task_2_compute_cell_scores(const char* eq_csv_file, int resolution, int K)
{
	//Skip list implementation - roughly 10 secs
	FILE *input_file = NULL;
	eq_t *temp = NULL;
	int data_read = 0;
	head_t ***map = NULL;
	list_t *k_list;
	int scaled_x = 0;
	int scaled_y = 0;
	head_t **k_array = NULL;

	if (eq_csv_file == NULL) {
		debug("File is empty");
		exit(EXIT_FAILURE);
	}
	else {
		input_file = safe_open_file(eq_csv_file, "r");
		k_list = list_new(list_delete);
		k_array = (head_t**)safe_malloc(K * sizeof(head_t*));
		for (int k = 0; k < K; k++) {
			k_array[k] = (head_t*)safe_malloc(sizeof(head_t));
		}
		map = (head_t***)safe_malloc(resolution * sizeof(head_t**));
		for (int i = 0; i < resolution; i++) {
			map[i] = (head_t**)safe_malloc(resolution * sizeof(head_t*));
			for (int j = 0; j < resolution; j++) {
				map[i][j] = new_head();
			}
		}

		if (input_file == NULL) {
			debug("Opened file is empty");
			exit(EXIT_FAILURE);
		}
		else {
			while (1) {
				temp = read_quake(input_file);
				if (temp != NULL) {
					scale_coordinates(temp, resolution);
					scaled_x = (int)((temp->x));
					scaled_y = (int)((temp->y));
					//printf("Scaled x and y is %d and %d\n",scaled_x,scaled_y);
					insert(map[scaled_x][scaled_y],temp);
					data_read += 1;
				}
				else {
					break;
				}
			}
		}
	}

	compute_scores(map, k_list, resolution, K);
	invert(k_array,k_list,K);

	FILE* fp = safe_open_file("task_2.csv", "w");
	fprintf(fp, "timestamp,latitide,longitude,magnitude,score\n");
	for (int i = (K-1); i >= 0; i--) {
		score_print(fp, k_array[i]);
	}
	fclose(fp);

	free(k_array);
	slist_free(map, resolution);
	list_free(k_list);
}


void task_3_magnitude_statistics(const char* eq_csv_file)
{
	FILE *input_file = NULL;
	eq_t *temp = NULL;
	eq_t *eq_array;
	list_t *stat_list = NULL;
	int data_read = 0;
	int mem_realloc_count = 1;

	if (eq_csv_file == NULL) {
		debug("Empty file");
		exit(EXIT_FAILURE);
	}
	else {
		stat_list = list_new(list_delete);
		eq_array = (eq_t*)safe_malloc(MAXINPUT * sizeof(eq_t));
		input_file = safe_open_file(eq_csv_file, "r");

		if (input_file != NULL) {
			while (1) {
				temp = read_quake(input_file);
				if (temp != NULL) {
					if (temp->timestamp->year >= MINYEART3 && temp->timestamp->year <= MAXYEART3) {
						eq_array[data_read] = *temp;
						data_read += 1;
					}
				}
				else {
					break;
				}

				if (data_read == mem_realloc_count*MAXINPUT) {
					mem_realloc_count++;
					eq_array = safe_realloc(eq_array, mem_realloc_count*MAXINPUT * sizeof(eq_t));
				}
			}
			qsort(&eq_array[0], data_read, sizeof(eq_t), compare);

			for (int i = 0; i < data_read; i++) {
				filter_quake(stat_list,&eq_array[i]);
			}

			node_t* temp_stat = stat_list->head;
			FILE* fp = safe_open_file("task_3.csv", "w");
			fprintf(fp, "year,magnitude,count,percentage\n");
			while (temp_stat != NULL) {
				stat_printf(fp, temp_stat->data);
				temp_stat = temp_stat->next;
			}
			fclose(fp);

			eq_free(temp);
			list_free(stat_list);
			eq_free(eq_array);
		}
	}
}

