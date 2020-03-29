#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>

#define MIN_PID 100
#define MAX_PID 1000
#define NO_OF_PID 12   // Number of threads you want to create

// Here i'm using a character array which is preallocated 
int PID[MAX_PID-MIN_PID]={0};

long ALL_PID(void)   // ALL_PID is for allocation of PID
{
    int l=0;
    int fl=1;       // where fl is used as variable flag
    while(l<MAX_PID-MIN_PID)
    {
        if(PID[l]==0)
        {
            // if pid is available
            //then the next line should be in critical section of program
            // A binary semaphore can be used as a Mutex and therfore whicever thread gets the lock gets the id because of mutex lock
            PID[l]=1;
            fl=0;
            break;
        }
        l++;
    }
    return fl?-1:l;
}
void RELEASED_PID(int ID)
{
    PID[ID]=0;
}
void *threadRoutine(void *args)
{
    int TID =  *(( int* )args);
    
    //get a pid
    int ID = ALL_PID();


    if(ID==-1)
    {
        printf("PID is not available now.");
    }
    else{
        printf("Thread [%3d] PID [%3d] Allocated\n",TID,ID+MIN_PID);
        //random time sleep between 1-10 seconds
        
        int r=1+rand()%30;
        //for making a thread sleep for a random time we have to uncomment the next line
        //sleep(r);

        printf("Thread [%3d] PID [%3d] Released after %d sec\n",TID,ID+MIN_PID,r);
        RELEASED_PID(ID);
    }
    pthread_exit(NULL);
}

long P_Thread(){
int k=0;
    
    pthread_t process[NO_OF_PID];
    srand(time(NULL)); 
    //printf(" %d ", rand());
    
    while(k<NO_OF_PID)
    {
        if(pthread_create(&process[k],NULL,threadRoutine,(void*)&k))
            return -1*printf("There is an Error while creating thread %d!!!!\n",k);    /// here it will return a negative integer value
            k++;
    }
    
    for(k=0; k<NO_OF_PID; k++)
        pthread_join(process[k],NULL);
}    

int main(){
	printf("\n ***Welcome***");
    printf("\n***This is a PID Manager***\n");
    P_Thread();    /// function call

    // we can write here wait(NULL);
    printf("\n******Program Terminates******\n");
}
