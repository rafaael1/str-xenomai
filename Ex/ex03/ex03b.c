#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define ITER 10

static RT_TASK  t1;
static RT_TASK  t2;

RT_SEM sem;

int global = 0;

void taskOne(void *arg) {
	int i;
	for (i=0; i < ITER; i++) {
		//rt_sem_v(&sem);
		printf("I am taskOne and global = %d................\n", ++global);
		rt_sem_p(&sem,0);
		//rt_sem_broadcast(&sem);
		rt_sem_v(&sem);
    	}
}

void taskTwo(void *arg) {
	int i;
	for (i=0; i < ITER; i++) {
        	//rt_sem_v(&sem);
		printf("I am taskTwo and global = %d................\n", --global);
		//rt_sem_p(&sem,0);
		rt_sem_v(&sem);
		rt_sem_p(&sem,0);

	}
}

int main(int argc, char* argv[]) {
	rt_task_create(&t1, "task1", 0, 1, 0);
	rt_task_create(&t2, "task2", 0, 1, 0);

	rt_sem_create(&sem, "GLOBAL COUNTER", 0, S_PRIO);
	rt_task_start(&t1, &taskOne, 0);
	rt_task_start(&t2, &taskTwo, 0);

//	rt_sem_delete(&sem);
	pause();
	return 0;
}
