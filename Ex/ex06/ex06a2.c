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

#define N_RUN 12

RT_TASK t[N_TASKS];
RT_SEM mysync;

#define EXEC_TIME	2e8
#define SPIN_TIME	2e7
#define CLOCK_RES 1e-9 

void lowDemo(void *arg);
void midDemo(void *arg);
void highDemo(void *arg);

void lowDemo(void *arg) {
	int c;
	int j;
	rt_sem_v(&mysync);
	rt_sem_p(&mysync, TM_INFINITE);
	printf("Low priority task locks semaphore\n");

	// Inicia a tarefa de media prioridade
	j=1;
	rt_task_start(&(t[j]), &midDemo, &(j));

	printf("Low priority task unlocks semaphore\n");
	rt_sem_v(&mysync);

	for (c = 0; c< (N_RUN/4); c++) {
		rt_sem_p(&mysync, TM_INFINITE);
		printf("Low priority task locks semaphore\n");

		printf("Low priority task unlocks semaphore\n");
		rt_sem_v(&mysync);
	}

	printf("..........................................Low priority task ends\n");

	rt_sem_v(&mysync);
}

void midDemo(void *arg) {
	int c;
	//rt_sem_p(&mysync,TM_INFINITE);
	int k;
	k=2;
	rt_task_start(&(t[k]), &highDemo, &(k));

	for (c = 0; c< N_RUN; c++) {
        printf("Medium priority task running\n");
    }
	printf("..........................................Medium priority task ends\n");
}

void highDemo(void *arg) {
	int c;

	printf("High priority task tries to lock semaphore\n");
	rt_sem_p(&mysync,TM_INFINITE);
	
	for (c = 0; c< (N_RUN/4); c++) {
		printf("High priority task locks semaphore\n");
		rt_sem_p(&mysync, TM_INFINITE);

		printf("High priority task unlocks semaphore\n");
		rt_sem_v(&mysync);
	}
	printf("..........................................High priority task ends\n");
}

void demo(void *arg) {
	RTIME starttime, runtime;
	RT_TASK_INFO curtaskinfo;
	int num = *(int *) arg;
	printf("Task : %d\n", num);
	rt_task_inquire(NULL,&curtaskinfo);
	//rt_task_set_mode(0,XNRRB,NULL);
	rt_sem_p(&mysync,TM_INFINITE);
//	rt_task_set_periodic(NULL,TM_NOW,2*1e7);
//	rt_task_wait_period(NULL);
	switch (curtaskinfo.prio) {
		case LOW:
			printf("Low priority task locks semaphore\n");
			rt_sem_v(&mysync);
			//rt_sem_p(&mysync, TM_INFINITE);

			printf("Low priority task unlocks semaphore\n");
			rt_sem_v(&mysync);
			break;
		case HIGH:
			printf("High priority task tries to lock semaphore\n");
			rt_sem_p(&mysync, TM_INFINITE);
			printf("High priority task locks semaphore\n");
			//rt_sem_v(&mysync);
			//printf("High priority task unlocks semaphore\n");
			break;
	}
	runtime = 0;
	while (runtime < EXEC_TIME) {
		rt_timer_spin(SPIN_TIME);
		runtime += SPIN_TIME;
		// No meio da execucao da tarefa de maior prioridade (t[2], num == 2),
		//	e feita a mudanca da priorida das outras tarefas, aumentando-as
//		if (runtime-SPIN_TIME == EXEC_TIME/2) {
//			 rt_task_set_priority(&(t[num]),40-num);
//		}
		// pegando informacoes da tarefa atual para facilitar o entendimento da mudanca
		// de prioridade em tempo de execucao
//		rt_task_inquire(NULL,&curtaskinfo);
		switch (curtaskinfo.prio) {
			case LOW:
				printf("Low priority task locks semaphore\n");
				rt_sem_p(&mysync, TM_INFINITE);
				printf("Low priority task unlocks semaphore\n");
				rt_sem_v(&mysync);
				break;
			case MID:
				printf("Medium task running\n");
				break;
			case HIGH:
				printf("High priority task tries to lock semaphore\n");
				rt_sem_p(&mysync, TM_INFINITE);
				printf("High priority task locks semaphore\n");
				rt_sem_v(&mysync);
				printf("High priority task unlocks semaphore\n");
				break;
		}
		printf("Running Task : %d, Priority : %d at ms : %d\n", num, curtaskinfo.prio, runtime/1000000);
		//rt_task_wait_period(NULL);
	}
//	printf("End Task : %d\n",num);
	switch (curtaskinfo.prio) {
		case LOW:
			printf("..........................................Low priority task ends\n");
			break;
		case MID:
			printf("..........................................Medium priority task ends\n");
			break;
		case HIGH:
			printf("..........................................High priority task ends\n");
			break;
	}


}
void startup() {
	int i,j,k;
	char str[10];
	RTIME quantum = rt_timer_ns2ticks(1000000);
	// semaphore to sync task startup on
	rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);
	for (i = 0;i < N_TASKS;i++) {
		sprintf(str,"task%d",i);
		rt_task_create(&(t[i]), str, 0, (50+i), 0);
//		rt_task_set_mode(0,XNRRB,NULL);
		//rt_task_slice(&t[i],100000);
		//rt_task_set_periodic(&(t[i]),200000000*(1),rt_timer_ns2ticks(200000));
//		rt_task_start(&(t[i]), &demo, &i);
	}
	// assign priorities to tasks
	rt_task_set_priority(&(t[0]),LOW);
	rt_task_set_priority(&(t[1]),MID);
	rt_task_set_priority(&(t[2]),HIGH);
	i=0;
	rt_task_start(&(t[i]), &lowDemo, &(i));
	/*j=1;
	rt_task_start(&(t[j]), &demo, &(j));*/
/*	k=2;
	rt_task_start(&(t[k]), &demo, &(k));
*/
//	rt_task_set_priority(&(t[0]),LOW);
//	rt_task_set_priority(&(t[1]),MID);
//	rt_task_set_priority(&(t[2]),HIGH);
	//printf("wake up all tasks\n");
	//rt_sem_broadcast(&mysync);
}
int main(int argc, char * argv[]) {
	startup();
	printf("\nType CTRL+C to end this program\n\n");
	pause();
	return 0;
}
