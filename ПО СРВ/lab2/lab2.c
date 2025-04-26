#include <stdio.h>
#include <pthread.h>
#include <sys/neutrino.h>

void *thread1(){
	while(1){
		printf("Thread 1\n");
		sleep(1);
	}
}
void *thread2(){
	while(1){
		printf("Thread 2\n");
		sleep(1);
	}
}
int main(){
	pthread_t tr1;
	pthread_t tr2;
	pthread_attr_t tr1_attr;
	pthread_attr_t tr2_attr;
	struct sched_param tr1_shed_param;
	struct sched_param tr2_shed_param;
	
	pthread_attr_init(&tr1_attr);
	pthread_attr_init(&tr2_attr);
	pthread_attr_setschedpolicy(&tr1_attr,SCHED_FIFO);
	pthread_attr_setschedpolicy(&tr2_attr,SCHED_RR);
	tr1_shed_param.sched_priority=4;
	tr2_shed_param.sched_priority=6;
	pthread_attr_setschedparam(&tr1_attr,&tr1_shed_param);
	pthread_attr_setschedparam(&tr2_attr,&tr2_shed_param);

	pthread_create(&tr1,&tr1_attr,thread1,NULL);
	pthread_create(&tr2,&tr2_attr,thread2,NULL);
	sleep(40);
	return 0;
}