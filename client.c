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
	if(argc!=11 || (strcmp("-s",argv[1]) || strcmp("-q",argv[3]) || strcmp("-n",argv[5]) || strcmp("-p",argv[7]) || strcmp("-f",argv[7]) )){
		printf("\nIncorrect args\nCorrect usage is %s -s <store's_shmid> -q <queue's_shmid> -n <no_of_copies> -p <no_of_pages> -f <file_name>",argv[0]);
		return 1;
	}								// checks whether format specifier and their count are in correct format

	key_t ShmKEY,ShmKEY2;			// holds generated key for shared memory
	int	ShmID,ShmID2;				// holds shared memory for generated key
	struct Circular_Queue *ShmPTR;  // Shared memory consists of lock,status,circular queue
	int copies,pages;				// holds number of copies, number of pages
	char filename[20];				// holds input filename

	// assigning values inputed in cmd line args
	ShmKEY = atoi(argv[2]);
	ShmKEY2 = atoi(argv[4]);
    copies = atoi(argv[6]);
	pages = atoi(argv[8]);
	strcat(argv[10],"\0");
	strcpy(filename,argv[10]);

	ShmID = shmget(ShmKEY,sizeof(struct Circular_Queue), 0777);
	if(ShmID<0){
		printf("\n shmget(%d) Failed\n",ShmKEY);
		exit(1);
	} // checking memory id for outer structure is received or not

	ShmPTR = (struct Circular_Queue *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed\n");
		exit(1);
	} // checking memory id is attached for outer structure is received or not

	ShmID2=shmget(ShmKEY2,(ShmPTR->MAX*sizeof(struct Photocopy)),0777);

	if(ShmID2<0){
        printf("\n shmget() for ShmKEY2 Failed\n");
        exit(1);
    } // checking memory id for inner structure is received or not

	ShmPTR->cqueue_arr=(struct Photocopy *)shmat(ShmID2,NULL,0);
	if(ShmPTR->cqueue_arr == (struct Photocopy *)(-1)){
		printf("\n shmat() for ShmID2 Failed\n");
		exit(1);
	} // checking memory id is attached for inner structure is received or not

    printf("\nSuccess !!\nShared Memories are attached !!");

	// data will be inserted into the queue if device is not checking queue at that time
	while (ShmPTR->lock == 1);
	ShmPTR->lock = 1;
	displayall(ShmPTR);
	int task = insert(pages,copies,filename,ShmPTR);
	displayall(ShmPTR);
	ShmPTR->lock = 0;
	if(task == -1)
		printf("\nNo more requests can be accepted\nBuffer is FULL !!\n");
	// after inserting to the queue the pointer attached to the shared memory will be deleted
	shmdt((void*)ShmPTR);
}
