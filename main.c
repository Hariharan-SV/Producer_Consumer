#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include "circular_queue.c"

int main(int argc,char *argv[]) {

	if(argc != 7 || (strcmp("-t",argv[1]) || strcmp("-d",argv[3]) || strcmp("-l",argv[5])) ){
		printf("\nInsufficient arguments give ./main -t <total_run_time> -d <no_of_printers> -l <length_of_queue>\n");
		exit(1);
	}						// checks whether format specifier and their count are in correct format

	double elapsed;			// holds difference between start time and current time
	int runtime;			// holds input runtime
	time_t start, end;
	key_t	ShmKEY,ShmKEY2;	// holds generated key for shared memory
	int	ShmID,ShmID2;		// holds shared memory for generated key
	struct Circular_Queue *ShmPTR; // An array of photocopy which acts as a circular queue

	ShmKEY = rand()%1000;	// randomly generated key for outer structure
	ShmKEY2 = ShmKEY+ 2;	// randomly generated key for inner structure

	ShmID = shmget(ShmKEY,sizeof(struct Circular_Queue),IPC_CREAT| 0777);
	if(ShmID<0){
		printf("\n shmget(%d) Failed\n",ShmKEY);
		exit(1);
	}						// checking memory id for outer structure is received or not

	ShmPTR = (struct Circular_Queue *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed\n");
		exit(1);
	}						// checking memory id is attached for outer structure is received or not

	// after recieving shared memory initializing it with inputed number of devices and max queue length
	ShmPTR->no_of_devices = atoi(argv[4]);
	ShmPTR->MAX = atoi(argv[6]);
	// basics for initializing a queue
	ShmPTR->front = -1;
	ShmPTR->rear = -1;
	// running status of a program
	ShmPTR->status = 1;
	// lock shared memory so no 2 or more programs access it at a same time
	ShmPTR->lock = 0;
	ShmID2=shmget(ShmKEY2,(ShmPTR->MAX*sizeof(struct Photocopy)),IPC_CREAT|0777);
	if(ShmID2<0){
		printf("\n shmget() for ShmKEY2 Failed\n");
		exit(1);
	}// checking memory id for inner structure
	ShmPTR->cqueue_arr=(struct Photocopy *)shmat(ShmID2,NULL,0);
	if(ShmPTR->cqueue_arr == (struct Photocopy *)(-1)){
		printf("\n shmat() for ShmID2 Failed\n");
		exit(1);
	}// checking memory id is attached or not

	printf("Photocopy Centre's SHMID is %d \n",ShmKEY);
	printf("Photocopy Centre's Queue's SHMID is %d \n",ShmKEY2);

	runtime = atoi(argv[2]);	// max runtime for the program
	start = time(NULL);			// starttime of the program
	int terminate = 1;			// status for termination
	while (terminate) {
		end = time(NULL);
		elapsed = difftime(end, start);

		if (elapsed >= (double)runtime)
			terminate = 0;
		else
			sleep(1);
	}
	ShmPTR->status =0;

	shmdt((void*)ShmPTR);
	shmctl(ShmID,IPC_RMID,NULL);	
	printf("Shutting the centre without abnormal termination..\n");

	return 0;
 }