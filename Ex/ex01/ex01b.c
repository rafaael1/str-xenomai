#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <alchemy/task.h>
#include <errno.h>

RT_TASK demo_task;

void demo(void *arg)
{
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;

  // hello world
  rt_printf("Hello World!\n");

  // inquire current task
  curtask=rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);

  // print task name
  rt_printf("Task name : %s \n", curtaskinfo.name);
}

int main(int argc, char* argv[])
{
	char  str[10] ;
	int retval;
  	// Perform auto-init of rt_print buffers if the task doesn't do so
  	//rt_print_auto_init(1);

	// Lock memory : avoid memory swapping for this program
	mlockall(MCL_CURRENT|MCL_FUTURE);

	rt_printf("start task\n");

	/*
	* Arguments: &task,
	*            name,
	*            stack size (0=default),
	*            priority,
	*            mode (FPU, start suspended, ...)
	*/
	sprintf(str,"hello");
	rt_task_create(&demo_task, str, 0, 50, 0);

  	/*
   	* Arguments: &task,
   	*            task function,
   	*  	          function argument
   	*/
	retval = rt_task_start(NULL, &demo, 0);
	if (retval < 0 ) {
		rt_printf("Starting error %d : %s\n",-retval,strerror(-retval));
	}
	else {
		rt_printf("taskOne started\n");
	}
}
