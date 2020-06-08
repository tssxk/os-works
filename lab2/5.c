#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>
 
 
#define producer_N 15//生产者数量
#define consumer_N 5//消费者数量
#define Buffer_N 100//缓冲区大小
int producer_id=0;//生产者ID
int consumer_id=0;//消费者ID
int in=0;//消费者放取产品的位置
int out=0;//消费者放取产品的位置
int Buffer[Buffer_N];//缓冲区
sem_t sem_empty;//同步信号量
sem_t sem_full;//同步信号量
pthread_mutex_t mutex;//互斥信号量
void Signal_print(int signo);//处理信号
void print(); //打印缓冲队列 
void *producer();//生产者
void *consumer();//消费者 
void main()
{
	pthread_t m_producer[producer_N];
	pthread_t m_consumer[consumer_N];
	int i,ret1[producer_N],ret2[consumer_N];
	printf("生产者数目都为4,消费者数目为5，产品缓冲为9,生产者每3秒生产一个产品，消费者每6秒消费一个产品,Ctrl+c退出程序\n"); 
	if(signal(SIGINT,Signal_print)==SIG_ERR)
		printf("error signal!\n");
 
 
	int ru1=sem_init(&sem_empty,0,Buffer_N);//初始化有界缓冲区大小
	int ru2=sem_init(&sem_full,0,0);//初始化满缓冲区大小
	if(ru1||ru2)
	{
		printf("error init signal！");
		exit(0);
	}
	int ru3=pthread_mutex_init(&mutex,NULL);
	if(ru3)
	{
		printf("error init thread!\n");
		exit(1);
	}
	// 创建生产者线程  
	for(i = 0; i < producer_N; i++)
	{ 
		ret1[i]= pthread_create(&m_producer[i], NULL, producer, (void *) (&i)); 
		if(ret1[i] != 0) { 
			printf("生产者%d线程创建失败！\n", i); 
			exit(1); 
		} 
	} 
	//创建消费者线程  
	for(i = 0; i < consumer_N; i++)
	{ 
		ret2[i]= pthread_create(&m_consumer[i], NULL, consumer, NULL); 
		if(ret2[i] != 0) { 
			printf("消费者%d线程创建失败！\n", i); 
			exit(1); 
		} 
	} 
	//等待线程销毁  
	for(i = 0; i < producer_N; i++)
		pthread_join(m_producer[i], NULL); 
	for(i = 0; i < consumer_N; i++)
		pthread_join(m_consumer[i], NULL);
	exit(0); 
 
 
}
 
 
void Signal_print(int signo)//处理信号
{
	printf("%d exit applications!\n",signo);
	exit(0);
}
 
 
void *producer()//生产者
{
	int id = producer_id;
	while(1)//生产未完成
	{
		sleep(3);//调节生产者和消费者生产消费的速度
		sem_wait(&sem_empty);//p（empty）操作
		pthread_mutex_lock(&mutex);//p(mutex)操作
		//完成生产者的操作 
		in%= Buffer_N;
		printf("生产者%d将产品放入缓冲区队列第%d号    ",id,in+1);
		Buffer[in]=1;
		print();
		++in;
		pthread_mutex_unlock(&mutex);//v(mutex)操作
		sem_post(&sem_full);//v(full)操作
 
 
	}
}
 
 
void *consumer()//消费者
{
	int id = ++consumer_id;
	while(1)//消费未完成
	{
		sleep(6);//调节生产者和消费者生产消费的速度
		sem_wait(&sem_full);//p(full)操作
		pthread_mutex_lock(&mutex);//p(mutex)操作
		//完成消费者的操作
		out%= Buffer_N;
		printf("消费者%d从缓冲区队列第%d号取走产品    ",id,out+1);
		Buffer[out]=0;
		print();
		++out;
		sem_post(&sem_empty);//v(empty)操作
		pthread_mutex_unlock(&mutex);//v(mutex)操作
	}
}
 
 
// 打印缓冲情况  
void print() { 
	int i; 
	printf("产品队列为"); 
	for(i = 0; i <Buffer_N; i++) 
		printf("%d", Buffer[i]); 
	printf("\n"); 
}