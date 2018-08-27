#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>

RT_TASK hello_task;
RT_TASK demo_1;
RT_TASK demo_2;
RT_TASK demo_3;
RT_TASK demo_4;
RT_TASK demo_5;

// function to be executed by task
void demo(void *arg) {
	RT_TASK_INFO curtaskinfo;

	// printf("Hello World!\n");

	// inquire current task
	rt_task_inquire(NULL,&curtaskinfo);

	// print task name
	printf("Task name : %s \n", curtaskinfo.name);
}

void demoTaskOne(void *arg) {
	RT_TASK_INFO curtaskinfo;
	rt_task_inquire(NULL, &curtaskinfo);
	rt_printf("Task name: %s", curtaskinfo.name);
}

int main(int argc, char* argv[]) {
  	char  str[10];
	char  str1[12];
        char  str2[12];
        char  str3[12];
        char  str4[12];
        char  str5[12];

	printf("start task1\n");
	sprintf(str1,"Task One");
        sprintf(str2,"Task Two");
        sprintf(str3,"Task Three");
        sprintf(str4,"Task Four");
        sprintf(str5,"Task Five");

  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
	rt_task_create(&hello_task, str, 0, 50, 0);
        rt_task_create(&demo_1, str1, 0, 50, 0);
        rt_task_create(&demo_2, str2, 0, 50, 0);
        rt_task_create(&demo_3, str3, 0, 50, 0);
        rt_task_create(&demo_4, str4, 0, 50, 0);
        rt_task_create(&demo_5, str5, 0, 50, 0);

  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
	rt_task_start(&hello_task, &demo, 0);
        rt_task_start(&demo_1, &demo, 0);
        rt_task_start(&demo_2, &demo, 0);
        rt_task_start(&demo_3, &demo, 0);
        rt_task_start(&demo_4, &demo, 0);
        rt_task_start(&demo_5, &demo, 0);
}
