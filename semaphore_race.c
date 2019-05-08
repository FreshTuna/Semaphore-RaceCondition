#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int a ;
int b;
int c;
int sem_value;
sem_t s1,s2,s3; //Declare three variables so threads functions in a specific order.

void *thread1()
{
    sem_wait(&s1);
    sem_getvalue(&s1,&sem_value); 
    printf("Thread 1 sem_value : %d ",sem_value);
    a++; 
    b = b + c;
    c = c + a;
    sem_post(&s2);
}

void *thread2()
{	
    sem_wait(&s2);
    sem_getvalue(&s2,&sem_value); 
    printf("Thread 2 sem_value : %d ",sem_value);
    a = a + b; 
    b++;
    c = c + a;
    
    sem_post(&s3);
}

void *thread3()
{
    sem_wait(&s3);
    sem_getvalue(&s3,&sem_value); 
    printf("Thread 3 sem_value : %d\n" ,sem_value);
    a = a + b; 
    b = b + c;
    c++;
    sem_post(&s1);
}

int main()
{
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);
    sem_init(&s3, 0, 0);
    while(1) {
        
        a=1;
        b=2;
        c=3;	
        pthread_t th1;
        pthread_t th2;
        pthread_t th3;

        pthread_create(&th1, NULL, thread1, NULL);
        pthread_create(&th2, NULL, thread2, NULL);
        pthread_create(&th3,NULL, thread3,NULL);
     
        pthread_join(th1, NULL);
        
        pthread_join(th2, NULL);
       
        pthread_join(th3, NULL);
        

        
        if(a!=13 || b!=18 || c!=13){
        	printf("@@@@@@Found!!!@@@@@@");
	} //Check if there is race condition
	printf("a= %d, b = %d, c = %d\n",a, b, c);
        usleep(50000);
    }
    return 0;
}
