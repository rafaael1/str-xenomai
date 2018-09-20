#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>

#define NTASKS 5
#define NSAMPLES 10000

RT_TASK periodic_task;
RTIME time_values[NSAMPLES];
//RTIME time_diffs[NSAMPLES-1];
unsigned int current_sample = NSAMPLES;


RT_SEM mysync;

#define EXECTIME   2e8   // execution time in ns
#define SPINTIME   1e7   // spin time in ns

void periodic(void *arg) {
	while (current_sample != 0) {
		current_sample--;
		time_values[current_sample] = rt_timer_read();
		rt_task_wait_period(NULL);
	}
	printf("\nTERMINOU\n\n" );
	rt_sem_broadcast(&mysync);
}

void calc_time_diffs(unsigned int nsamples, RTIME *time_diff) {
	int c = 0;
	for(c = 0; c < nsamples-1; c++) {
		time_diff[c] =  time_values[c]  - time_values[c+1];
	}
}
RTIME calc_average_time(unsigned int nsamples, RTIME *time_diff) {
	RTIME average = 0;
	int c = 0;
	for(c = 0; c < nsamples-1; c++) {
		average += time_diff[c];
	}
	average = average/nsamples;
	return average;
}


void write_RTIMES(char * filename, unsigned int number_of_values, RTIME *time_values) {
	unsigned int n=0;
	FILE *file;
	file = fopen(filename,"w");
	while (n<number_of_values) {
		fprintf(file,"%u,%llu\n",n,time_values[n]);
		n++;
	} 
	fclose(file);
}

//startup code
void startup() {
	char  str[20];
	printf("start task\n");
	sprintf(str,"periodic task");

	rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);

	rt_task_create(&periodic_task, str, 0, 50, 0);
	rt_task_set_periodic(&periodic_task, TM_NOW, rt_timer_ns2ticks(100000));

	//	rt_task_set_mode(0,XNRRB,NULL);

	rt_task_start(&periodic_task, &periodic, 0);

	rt_sem_p(&mysync,TM_INFINITE);
}

int main(int argc, char* argv[]) {

	RTIME *time_diff;
	time_diff = calloc(NSAMPLES, sizeof(RTIME));
	RTIME average = 0;
	
	printf("\nANTES\n\n" );
	startup();
	printf("\nDEPOIS\n\n" );

	calc_time_diffs(NSAMPLES, time_diff);
	write_RTIMES("time_diff.csv",NSAMPLES,time_diff);
	average=calc_average_time(NSAMPLES,time_diff);
	printf("average  %llu\n", average); 

	
	printf("\nType CTRL-C to end this program\n\n" );
	pause();
} 
