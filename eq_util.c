/***************************************************************************
 *
 *   File        : eq_util.c
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/

#include "eq_util.h"
#include "skiplist.h"
#include "linkedlist.h"

#define PI 3.141593
#define ZOOM 0
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MONTHSPERYEAR 12 /* months per calendar year */

 /* print a timestamp */
void time_print(FILE* fp, timestamp_t* t)
{
	fprintf(fp, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ\n",
		t->year, t->month, t->day, t->hour, t->min, t->sec, t->msec);
}

/* needed for time comparison */
long long my_timegm(struct tm* ptm)
{
	int m, y = ptm->tm_year;
	if ((m = ptm->tm_mon) < 2) {
		m += 12;
		--y;
	}
	return ((((long long)(y - 69) * 365u + y / 4 - y / 100 * 3 / 4 + (m + 2) * 153 / 5 - 446 + ptm->tm_mday) * 24u + ptm->tm_hour) * 60u + ptm->tm_min) * 60u + ptm->tm_sec;
}

double time_difference(timestamp_t* a, timestamp_t* b)
{
	struct tm ta;
	struct tm tb;
	ta.tm_hour = a->hour;
	ta.tm_min = a->min;
	ta.tm_sec = a->sec;
	ta.tm_mon = a->month - 1;
	ta.tm_mday = a->day;
	ta.tm_year = a->year - 1900;
	ta.tm_isdst = 0;
	tb.tm_hour = b->hour;
	tb.tm_min = b->min;
	tb.tm_sec = b->sec;
	tb.tm_mon = b->month - 1;
	tb.tm_mday = b->day;
	tb.tm_year = b->year - 1900;
	tb.tm_isdst = 0;
	long long at = my_timegm(&ta);
	long long bt = my_timegm(&tb);
	double seconds = (double)(at - bt);
	return seconds;
}

int compare(const void *a, const void *b) {
	timestamp_t *aa = ((eq_t *)a)->timestamp;
	timestamp_t *bb = ((eq_t *)b)->timestamp;

	if (time_difference(aa, bb) > 0) {
		return 1;
	}
	else if (time_difference(aa, bb) < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

/* translates lat,long to x,y. ensure all x,y are in [0,1] */
void map_coordinates(eq_t* eq)
{
	float sinlat = sinf(eq->latitude * (PI / 180.0));
	sinlat = MIN(MAX(sinlat, -0.9999), 0.99625);
	float x = (0.5 + eq->longitude / 360);
	float y = (0.5 - logf((1.0 + sinlat) / (1.0 - sinlat)) / (4.0 * PI));
	y = y - 1;
	y *= -1;
	eq->x = MIN(x, 0.999);
	eq->y = MIN(y, 0.999);
}

/* a wrapper function around malloc which checks return values and exits on error */
void* safe_malloc(size_t bytes)
{
	//printf("safe_malloc() - IMPLEMENT ME!\n");
	void *newly_assigned_mem = NULL;
	newly_assigned_mem = (void *)malloc(bytes);
	if (newly_assigned_mem == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		return newly_assigned_mem;
	}
	return NULL;
}

/* a wrapper function around realloc which checks return values and exits on error */
void* safe_realloc(void* ptr, size_t bytes)
{
	void *new_array = NULL;
	new_array = (void*)realloc(ptr, bytes);
	if (new_array == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		return new_array;
	}
	return NULL;
}

/* function whcih parses the timestamp part of the input data */
timestamp_t* parse_time(char* buf)
{
	int year, month, day, hour, min, sec, msec;
	if (sscanf(buf, "%d-%d-%dT%d:%d:%d.%dZ", &year, &month, &day, &hour, &min, &sec, &msec) != 7) {
		printf("error parsing timestamp.\n");
		exit(EXIT_FAILURE);
	}

	timestamp_t* ts = (timestamp_t*)safe_malloc(sizeof(timestamp_t));
	ts->year = year;
	ts->month = month;
	ts->day = day;
	ts->hour = hour;
	ts->min = min;
	ts->sec = sec;
	ts->msec = msec;
	return ts;
}

/* a wrapper function around fopen which checks return values and exits on error */
FILE* safe_open_file(const char* file_name, const char* mode)
{
	FILE* fp = fopen(file_name, mode);
	if (fp == NULL) {
		perror("file open error.");
		exit(EXIT_FAILURE);
	}
	return fp;
}

/* a helper function which creates an eq_t object. returns NULL if everything has been read */
eq_t* read_quake(FILE* fp)
{
	char buf[1024];
	float latitude, longitude, magnitude;
	if (fscanf(fp, "%[^,],%f,%f,%f\n", buf, &latitude, &longitude, &magnitude) == 4) {
		eq_t* eq = (eq_t*)safe_malloc(sizeof(eq_t));
		eq->timestamp = parse_time(buf);
		eq->latitude = latitude;
		eq->longitude = longitude;
		eq->magnitude = magnitude;
		map_coordinates(eq);
		return eq;
	}
	return NULL;
}

stat_t *new_stat_node(int year) {
	stat_t *stat = (stat_t*)safe_malloc(sizeof(stat_t));
	stat->magnitude = (int*)safe_malloc(MAXLEVELOFMAG*sizeof(int));
	//set all magnitude to zero
	for (int i = 0; i < MAXLEVELOFMAG; i++) {
		stat->magnitude[i] = 0;
	}
	stat->year = year;
	stat->percentage = 0.0;
	stat->total_eq_this_year = 0;
	return stat;
}

void insert_magnitude(stat_t *stat_node, int mag) {
	if (stat_node == NULL) {
		debug("Statistic node is empty. Please initialise.");
		exit(EXIT_FAILURE);
	}
	else {
		if (mag < 1.0) {
			stat_node->magnitude[0]++;
		}
		else if (mag < 2.0) {
			stat_node->magnitude[1]++;
		}
		else if (mag < 3.0) {
			stat_node->magnitude[2]++;
		}
		else if (mag < 4.0) {
			stat_node->magnitude[3]++;
		}
		else if (mag < 5.0) {
			stat_node->magnitude[4]++;
		}
		else if (mag < 6.0) {
			stat_node->magnitude[5]++;
		}
		else if (mag < 7.0) {
			stat_node->magnitude[6]++;
		}
		else if (mag < 8.0) {
			stat_node->magnitude[7]++;
		}
		else if (mag < 9.0) {
			stat_node->magnitude[8]++;
		}
		else {
			stat_node->magnitude[9]++;
		}
		stat_node->total_eq_this_year++;
	}
}
eq_t* empty_eq() {
	eq_t* eq = (eq_t*)safe_malloc(sizeof(eq_t));
	eq->timestamp = NULL;
	eq->latitude = 0;
	eq->longitude = 0;
	eq->magnitude = 0;
	map_coordinates(eq);
	return eq;
}
/* helper function which frees all memory associated with an eq_t object */
void eq_free(void* q)
{
	//printf("eq_free() - IMPLEMENT ME!\n");
	//exit(EXIT_FAILURE);
	//free(((eq_t*)q)->timestamp);
	free(q);
	q = NULL;
	if (q != NULL) {
		eq_free(q);
	}
}

/* prints an eq_t object in the standard format required for the output files */
void eq_print(FILE* fp, eq_t* q)
{
	fprintf(fp, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ,%.3f,%.3f,%.1f\n",
		q->timestamp->year,
		q->timestamp->month,
		q->timestamp->day,
		q->timestamp->hour,
		q->timestamp->min,
		q->timestamp->sec,
		q->timestamp->msec,
		q->latitude,
		q->longitude,
		q->magnitude);
}

void print_timestamp(timestamp_t *timestp) {
	printf("%d-%02d-%02dT%02d:%02d:%02d.%03dZ\n",
		timestp->year,
		timestp->month,
		timestp->day,
		timestp->hour,
		timestp->min,
		timestp->sec,
		timestp->msec);
}

void consecutive_eq(eq_t *eq, int num_data, int *index) {
	double longest_time = 0.0;
	double computed_time = 0.0;
	for (int i = 0; i < (num_data-1); i++) {
		computed_time = time_difference((&eq[i])->timestamp, (&eq[i + 1])->timestamp);
		if (abs(computed_time) > longest_time) {
			longest_time = abs(computed_time);
			*index = i;
		}
	}
}

void debug(char *message) {
	printf("%s\n", message);
}

void scale_coordinates(eq_t *earthquake, int resolution) {
	earthquake->x = (earthquake->x)*resolution;
	earthquake->y = (earthquake->y)*resolution;
}

void stat_printf(FILE *fp, stat_t *s_eq) {
	int count_eq = 0;
	float percentage = 0.0;
	float total_percentage = 0.0;
	for (int i = 0; i < MAXLEVELOFMAG; i++) {
		if (i < MAXLEVELOFMAG / 2) {
			if (s_eq->magnitude[i] > 0) {
				count_eq = s_eq->magnitude[i];
				percentage = ((float)(count_eq) / (float)(s_eq->total_eq_this_year)) * 100;
				total_percentage = total_percentage + percentage;
				fprintf(fp, "%d,%d,%d,%2.1f\n", s_eq->year, i, s_eq->magnitude[i], total_percentage);
			}
		}else{
			count_eq = s_eq->magnitude[i];
			percentage = ((float)(count_eq) / (float)(s_eq->total_eq_this_year)) * 100;
			total_percentage = total_percentage + percentage;
			fprintf(fp, "%d,%d,%d,%2.1f\n", s_eq->year, i, s_eq->magnitude[i], total_percentage);
		}
	}
}


