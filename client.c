#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "circular_queue.c"

int main(int argc,char* argv[]){
    if(argc!=11){
        printf("\nIncorrect args\nCorrect usage is %s -s <store's_shmid> -q <queue's_shmid> -n <no_of_copies> -p <no_of_pages> -f <file_name>",argv[0]);
        return 1;
    }

    int	ShmID,ShmID2;
	struct Circular_Queue *ShmPTR;
	int copies,pages;
    char filename[20];

	ShmID = atoi(argv[2]);
	ShmID2 = atoi(argv[4]);
    pages = atoi(argv[6]);
	copies = atoi(argv[8]);
    strcpy(filename,argv[10]);


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

	insert(pages,copies,filename,ShmPTR);

    shmdt((void*)ShmPTR);
}