#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "circular_queue.c"

int main(int argc,char *argv[]) {

	if(argc!=5){
        printf("\nIncorrect args\nCorrect usage is %s -s <store's_shmid> -q <queue's_shmid>",argv[0]);
        return 1;
    }

	int	ShmID,ShmID2;
	struct Circular_Queue *ShmPTR;

	ShmID = atoi(argv[2]);
	ShmID2 = atoi(argv[4]);

	ShmPTR = (struct Circular_Queue *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed\n");
		exit(1);
	}

	ShmPTR->cqueue_arr=(struct Photocopy *)shmat(ShmID2,NULL,0);
	if(ShmPTR->cqueue_arr == (struct Photocopy *)(-1)){
		printf("\n shmat() for ShmID2 Failed\n");
		exit(1);
	}

	display(ShmPTR);

    shmdt((void*)ShmPTR);

}