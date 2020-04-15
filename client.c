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

	key_t ShmKEY,ShmKEY2;
	int	ShmID,ShmID2;
	struct Circular_Queue *ShmPTR;
	int copies,pages;
	char filename[20];

	ShmKEY = atoi(argv[2]);
	ShmKEY2 = atoi(argv[4]);
	pages = atoi(argv[6]);
	copies = atoi(argv[8]);
	strcat(argv[10],"\0");
	strcpy(filename,argv[10]);

	ShmID = shmget(ShmKEY,sizeof(struct Circular_Queue), 0777);
	if(ShmID<0){
		printf("\n shmget(%d) Failed\n",ShmKEY);
		exit(1);
	}

	ShmPTR = (struct Circular_Queue *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed\n");
		exit(1);
	}

	ShmID2=shmget(ShmKEY2,(ShmPTR->MAX*sizeof(struct Photocopy)),0777);

	if(ShmID2<0){
        printf("\n shmget() for ShmKEY2 Failed\n");
        exit(1);
    }

	ShmPTR->cqueue_arr=(struct Photocopy *)shmat(ShmID2,NULL,0);
	if(ShmPTR->cqueue_arr == (struct Photocopy *)(-1)){
		printf("\n shmat() for ShmID2 Failed\n");
		exit(1);
	}

    printf("\nSuccess !!\nShared Memories are attached !!");

	while (ShmPTR->lock == 1);
	ShmPTR->lock = 1;
	displayall(ShmPTR);
	int task = insert(pages,copies,filename,ShmPTR);
	displayall(ShmPTR);
	ShmPTR->lock = 0;
	if(task == -1)
		printf("\nNo more requests can be accepted\nBuffer is FULL !!\n");
	shmdt((void*)ShmPTR);
}
