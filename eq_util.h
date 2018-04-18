/***************************************************************************
 *
 *   File        : eq_util.c
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/
#ifndef EQ_UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define MAXLEVELOFMAG 10

/* structure which defines a timestamp */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int msec;
} timestamp_t;

/* the main structure holding the data for one earthquake */
typedef struct {
    timestamp_t* timestamp;
    float latitude;
    float longitude;
    float magnitude;
    float x;
    float y;
} eq_t;

/*structure to store statistics for yearly earthquake*/
typedef struct {
	int year;
	int *magnitude;
	float percentage;
	int total_eq_this_year;
} stat_t;

/* helper function which maps (lat,long) to (x,y) */
void map_coordinates(eq_t* eq);
/* helper function which parses the timestamp in the input data */
timestamp_t* parse_time(char* buf);
/* helper function which reads data for one earthquake */
eq_t* read_quake(FILE* fp);
/* given two timestamps compute the difference in seconds */
double time_difference(timestamp_t* a, timestamp_t* b);

/* helper functions which wrap around standard functions and perform error handling */
void* safe_malloc(size_t bytes);
void* safe_realloc(void* ptr, size_t bytes);
FILE* safe_open_file(const char* file_name, const char* mode);

/* helper function which print/free one eq_t object */
void eq_print(FILE* fp, eq_t* q);
void eq_free(void* q);

/*Printi message for debugging purpose*/
void debug(char *message);

void print_timestamp(timestamp_t *timestp);
int compare(const void *a, const void *b);
void scale_coordinates(eq_t *earthquake, int resolution);

eq_t *empty_eq();
stat_t *new_stat_node(int year);
void insert_magnitude(stat_t *stat_node, int mag);

void stat_printf(FILE *fp, stat_t *s_eq);
void consecutive_eq(eq_t *eq, int num_data, int *index);

#endif
