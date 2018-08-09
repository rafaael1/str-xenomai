#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>

#define NUMBER_OF_TASKS 5

RT_TASK demo_task[NUMBER_OF_TASKS];

char numbers[NUMBER_OF_TASKS][12] = {"One", "Two", "Three", "Four", "Five"};

// function to be executed by task
void demo(void *arg) {
	RT_TASK_INFO curtaskinfo;
	int num = * (int *) arg;
	if (num < 4 && num >= 1) {
		long long t = (long long)(num)*1000*1000*1000;
		printf("TIME_NOW = %lld\n", t);

        	rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(t));

		while (1) {
        		rt_task_inquire(NULL,&curtaskinfo);
			printf("Task name : %s \n--Number: %d - TIME_NOW = %lld\n", curtaskinfo.name, num, rt_timer_ticks2ns(rt_timer_read())/1000000 );
			rt_task_wait_period(NULL);
		}
	}
	else {
		// printf("Hello World!\n");

		// inquire current task
		rt_task_inquire(NULL,&curtaskinfo);

		// print task name
		printf("Task name : %s \n--Number: %d\n", curtaskinfo.name, num);
	}
	return;
}

int main(int argc, char* argv[]) {
  	char  str[12];
	int c = 0;
	int v[NUMBER_OF_TASKS];
//	int num[NUMBER_OF_TASKS];

	printf("start tasks\n");
	for (c = 0; c < NUMBER_OF_TASKS; c++) {
		sprintf(str,"Task %s",numbers[c]);

		/* Create task
		* Arguments: &task,
		*            name,
		*            stack size (0=default),
		*            priority,
		*            mode (FPU, start suspended, ...)
		*/
		rt_task_create(&(demo_task[c]), str, 0, c*2, 0);
		printf("TASK %d CREATED - %s\n", c+1, str);
	}
        for (c = 0; c < NUMBER_OF_TASKS; c++) {
		v[c] = c+1;
		/*  Start task
		* Arguments: &task,
		*            task function,
		*            function argument
		*/
                printf("TASK %d PRE-START\n", v[c]);
		rt_task_start(&(demo_task[c]), &demo, &(v[c]));
                printf("TASK %d STARTED\n", v[c]);
	}
	printf("end program by CTRL-C\n");
	pause();
}
