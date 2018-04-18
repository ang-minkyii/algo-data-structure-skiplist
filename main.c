/***************************************************************************
*
*   File        : main.c
*   Student Id  : 565604
*   Name        : Mink Yii Ang
*
***************************************************************************/

#include "linkedlist.h"
#include "tasks.h"
#include "eq_util.h"

#include <time.h>
#include <math.h>
#include <string.h>


#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>



int main(int argc, char** argv)
{
	
	/* TODO: parse command line arguments */
	char* eq_csv_file = NULL;
	int resolution = 0;
	int K = 0;
	double elapsed_ms = 0.0;
	struct timeval start;
	struct timeval stop;
	
	if (argc != 4) {
		debug("More inputs required.");
		exit(EXIT_FAILURE);
	}else{
		eq_csv_file = (char *)malloc(256*sizeof(char));
		strcpy(eq_csv_file,argv[1]);
		resolution = atoi(argv[2]);
		K = atoi(argv[3]);
		if (K > resolution) {
			K = resolution;
		}
	}
	
	/* TODO: timing for Task 1 */
        gettimeofday(&start, NULL);
        task_1_find_longest_break_after_2k(eq_csv_file);
        gettimeofday(&stop, NULL);
        elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
        elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
        printf("TASK 1:%.2f seconds\n", elapsed_ms/1000);
        
        /* TODO: timing for Task 2 */
        gettimeofday(&start, NULL);
        task_2_compute_cell_scores(eq_csv_file, resolution, K);
        gettimeofday(&stop, NULL);
        elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
        elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
        printf("TASK 2:%.2f seconds\n", elapsed_ms/1000);
        
        /* TODO: timing for Task 3 */
        gettimeofday(&start, NULL);
        task_3_magnitude_statistics(eq_csv_file);    
        gettimeofday(&stop, NULL);
        elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
        elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
        printf("TASK 3:%.2f seconds\n", elapsed_ms/1000);
        return (EXIT_SUCCESS);
}
