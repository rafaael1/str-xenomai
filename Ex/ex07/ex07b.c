#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>
#include <rtdm/gpio.h>

#define NTASKS 5
RT_TASK demo_task[NTASKS];
RT_SEM mysync;

#define PINO_22 "/dev/rtdm/pinctrl-bcm2835/gpio22"
#define PINO_23 "/dev/rtdm/pinctrl-bcm2835/gpio23"
#define PINO_24 "/dev/rtdm/pinctrl-bcm2835/gpio24"


void read_gpio();
void write_gpio();

// DEFININDO OS PINOS GPIO a serem utilizados
int fd_LED, fd_SIDE, fd_SWITCH;
int value_LED = 1, value_SIDE, value_SWITCH;
int ret_LED, ret_SIDE, ret_SWITCH;

int side_trigger = GPIO_TRIGGER_EDGE_RISING;
int xeno_trigger = GPIO_TRIGGER_EDGE_FALLING|GPIO_TRIGGER_EDGE_RISING;


int read_SIDE() {
	/*
	int fd, value, ret;
	fd = open("/dev/rtdm/pinctrl-bcm2835/gpio23/", O_RDONLY|O_NONBLOCK);
	ret = ioctl(fd, GPIO_RTIOC_DIR_IN);
	*/
	ret_SIDE = read(fd_SIDE, &value_SIDE, sizeof(value_SIDE));
	return value_SIDE;
}

void write_LED(int v) {	//	v = 0 = LOW, v = 1 = HIGH
	/*
	int fd, value, ret;
	// value = 1;
	value = v;
	fd = open("/dev/rtdm/pinctrl-bcm2835/gpio23/", O_WRONLY);
	ret = ioctl(fd, GPIO_RTIOC_DIR_OUT, &value);
	*/
	
	value_LED = v;
	ret_LED = write(fd_LED, &value_LED, sizeof(value_LED));
	return;
}


int read_SWITCH() {
	/*
	int fd, value, ret;
	// value = 1;
	value = v;
	fd = open("/dev/rtdm/pinctrl-bcm2835/gpio23/", O_RDONLY);
	int xeno_trigger = GPIO_TRIGGER_EDGE_FALLING|GPIO_TRIGGER_EDGE_RISING;
	ret = ioctl(fd, GPIO_RTIOC_IRQEN, &xeno_trigger);
	*/
	ret_SWITCH = write(fd_SWITCH, &value_SWITCH, sizeof(value_SWITCH));
	return;
}


void blink_led(void *arg) {
	RTIME starttime, runtime;
	int num=*(int *)arg;
	printf("Task  : %d\n",num);
	rt_sem_p(&mysync,0);
	
	rt_task_set_periodic(NULL,TM_NOW, rt_timer_ns2ticks(500*1000*1000)); // define a tarefa com período de 500ms
	while(1) {
		//printf("1Task: %d, LED:%d\n",num,value_LED);
		write_LED((1<<value_LED)&1);
		printf("2Task: %d, LED:%d\n",num,value_LED);
		rt_task_wait_period(NULL);
	}
	printf("End Task  : %d\n",num);
	return;
}

void verify_interrup_side(void *arg) {
	int num=*(int *)arg;
	printf("Task  : %d\n",num);
	//rt_sem_p(&mysync,0);
	
	//rt_task_set_periodic(NULL,TM_NOW, rt_timer_ns2ticks(500*1000)); // define a tarefa com período de 500ms
	int count = 0;
	while(1) {
		//printf("1Task: %d, LED:%d\n",num,value_LED);
		if (read_SIDE() == 1){
			count++;
		}
		printf("SIDE_INTERRUPT_COUNT: %d\n",count);
		//rt_task_wait_period(NULL);
	}
	printf("End Task  : %d\n",num);
	return;
}

//startup code
void startup() {
	int i=0;
	char  str[10] ;
	
	// semaphore to sync task startup on
	rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);

	
	// Inicializando pinos da GPIO
	fd_LED = open(PINO_22, O_WRONLY);	//	PINO
	//fd_SIDE = open(PINO_24, O_RDONLY|O_NONBLOCK);
	fd_SIDE = open(PINO_24, O_RDONLY);
	fd_SWITCH = open(PINO_23, O_RDONLY);

	ret_LED = ioctl(fd_LED, GPIO_RTIOC_DIR_OUT, &value_SWITCH);
	//ret_SIDE = ioctl(fd_SIDE, GPIO_RTIOC_DIR_IN);
	ret_SIDE = ioctl(fd_SIDE, GPIO_RTIOC_IRQEN, &side_trigger);
	ret_SWITCH = ioctl(fd_SWITCH, GPIO_RTIOC_IRQEN, &xeno_trigger);
	

	printf("start task  : %d\n",i);
	sprintf(str,"task%d",i);
	rt_task_create(&demo_task[i], str, 0, 50, 0);
	i++;
	printf("start task  : %d\n",i);
	sprintf(str,"task%d",i);
	rt_task_create(&demo_task[i], str, 0, 55, 0);
	
	rt_task_start(&demo_task[i], &verify_interrup_side, &i);
	i--;
	rt_task_start(&demo_task[i], &blink_led, &i);
	
	rt_sem_broadcast(&mysync);
}

int main(int argc, char* argv[]) {
	startup();
	printf("\nType CTRL-C to end this program\n\n" );
	pause();
}
