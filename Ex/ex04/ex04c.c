#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>

#define N_TASKS 3
#define HIGH 	52
#define MID	51
#define LOW	50

RT_TASK t[N_TASKS];
RT_SEM mysync;

#define EXEC_TIME	2e8
#define SPIN_TIME	2e7

void demo(void *arg) {
	RTIME starttime, runtime;

	int num = *(int *) arg;
	printf("Task : %d\n", num);
	rt_sem_p(&mysync, TM_INFINITE);
	runtime = 0;
	while (runtime < EXEC_TIME) {
		rt_timer_spin(SPIN_TIME);
		runtime += SPIN_TIME;

		// No meio da execucao da tarefa de maior prioridade (t[2], num == 2),
		//	e feita a mudanca da priorida das outras tarefas, aumentando-as
		if (runtime == EXEC_TIME/2 && num == 2) {
			 rt_task_set_priority(&(t[1]),MID+10);
			 rt_task_set_priority(&(t[0]),LOW+10);
		}
		printf("Running Task : %d at ms : %d\n",num,runtime/1000000);
	}
	printf("End Task : %d\n",num);
}


void startup() {
	int i;
	char str[10];

	// semaphore to sync task startup on
	rt_sem_create(&mysync,"MySemaphore",0,S_PRIO);

	for (i = 0;i < N_TASKS;i++) {
		printf("start task : %d\n",i);
		sprintf(str,"task%d",i);
		rt_task_create(&(t[i]), str, 0, 50, 0);
		rt_task_start(&(t[i]), &demo, &i);
	}

	// assign priorities to tasks

	rt_task_set_priority(&(t[0]),LOW);
	rt_task_set_priority(&(t[1]),MID);
	rt_task_set_priority(&(t[2]),HIGH);

	printf("wake up all tasks\n");

	rt_sem_broadcast(&mysync);
}

int main(int argc, char * argv[]) {
	startup();
	printf("\nType CTRL+C to end this program\n\n");
	pause();
	return 0;
}

