#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define ITER 10
#define NUM_TASKS 5

static RT_TASK t[NUM_TASKS];

char task_name[NUM_TASKS][6] = {"task1", "task2", "task3", "task4", "task5"};// "task6", "task7"};

RT_SEM sem;

int global = 0;

int op = 0;

void task(void *arg) {
	int i;
	int num = * (int *)arg;
	for (i=0; i < ITER; i++) {
		//printf("rt_sem_p = %d................\n",rt_sem_p(&sem,0));
		rt_sem_p(&sem,0);
		//rt_sem_v(&sem);
		printf("I am task%d and global = %d................\n",num, op?--global:++global);
		op = !op;
		//rt_sem_p(&sem,0);
		//rt_sem_broadcast(&sem);
		rt_sem_v(&sem);
		//printf("rt_sem_VVVVV = %d................\n",rt_sem_v(&sem));
	}
}

int main(int argc, char* argv[]) {
	int c = 0;
	int v[NUM_TASKS];
	for (c = 0; c < NUM_TASKS; c++){
		rt_task_create(&t[c], task_name[c], 0, 10, 0);
	}

	rt_sem_create(&sem, "GLOBAL COUNTER", 0, S_FIFO);

	for (c = 0; c < NUM_TASKS; c++){
		v[c] = c+1;
		rt_task_start(&t[c], &task, &(v[c]));
	}

	rt_sem_v(&sem);
//	printf("rt_sem_VVVVV = %d................\n",rt_sem_v(&sem));

//	rt_sem_broadcast(&sem);

//	rt_sem_delete(&sem);
	pause();
	return 0;
}
