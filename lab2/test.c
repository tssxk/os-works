
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void waiting();
void stop();
int wait_mark;
void waiting()
{
	//sleep(5);
    while(wait_mark!=0);
   //kill(getpid(),SIGALRM);
}
/*void alarming()
{
	wait_mark=0;
}*/
void stop()
{
	wait_mark=0;
}
//void alarming();
 
int main()
{
	pid_t pid1,pid2;//进程标识符
	pid1 = fork();  //创建一个新的进程
	if(pid1<0)
	{
		printf("创建进程失败!");
		exit(1);
	}
	else if(pid1==0)  //如果pid为0则表示当前执行的是子进程
	{
		wait_mark=1;
            signal(16,stop);
            //signal(SIGINT,SIG_IGN);
            //signal(SIGQUIT,SIG_IGN)
            while(wait_mark!=0);
            lockf(1,1,0);
            printf("Child Process 1(pid=%d,ppid=%d) is killed by Parent!\n",getpid(),getppid());
            lockf(1,0,0);
            exit(0);
	}
	else //否则为父进程
	{
		pid2 = fork();//創建一個新的進
		if(pid2<0)
		{
			printf("创建进程失败!");
			exit(1);
		}
		else if(pid2==0) //如果pid为0则表示当前执行的是子进程
		{
			 wait_mark=1;
            signal(17,stop);
            //signal(SIGINT,SIG_IGN);
            //signal(SIGQUIT,SIG_IGN)
            while(wait_mark!=0);
            lockf(1,1,0);
            printf("Child Process 2(pid=%d,ppid=%d) is killed by Parent!\n",getpid(),getppid());
            lockf(1,0,0);
            exit(0);
		}
		else //否则为父进程
		{
			 wait_mark=1;
            signal(SIGINT,stop);
            //signal(SIGALRM,alarming);
            waiting();
            sleep(1);
            kill(pid1,16);
            sleep(1);
            kill(pid2,17);

            wait(0);
            wait(0);
            printf("Parent(pid=%d) Process is killed!\n",getpid());
            exit(0);
		}
	}
	return 0;
}
