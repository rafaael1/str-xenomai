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
	// printf("Hello World!\n");

	// inquire current task
	rt_task_inquire(NULL,&curtaskinfo);

	// print task name
	printf("Task name : %s \n--Number: %d\n", curtaskinfo.name, num);
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
}
