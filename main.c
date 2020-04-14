#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "circular_queue.c"

int main(int argc,char *argv[]) {

	if(argc!=7){
		printf("\nInsufficient arguments give ./main -t <total_run_time> -d <no_of_printers> -l <length_of_queue>");
		exit(1);
	}

	double elapsed;
	int runtime;
	time_t start, end;
	key_t	ShmKEY,ShmKEY2;
	int	ShmID,ShmID2;
	struct Circular_Queue *ShmPTR;
			
	ShmKEY = rand()%1000 + 100;
	ShmKEY2 = ShmKEY+ 2000;
    
	ShmID = shmget(ShmKEY,sizeof(struct Circular_Queue),IPC_CREAT| 0777);
	if(ShmID<0){
		printf("\n shmget(%d) Failed\n",ShmKEY);
		exit(1);
	}

	ShmPTR = (struct Circular_Queue *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed\n");
		exit(1);
	}

	ShmPTR->no_of_devices = atoi(argv[4]);
	ShmPTR->MAX = atoi(argv[6]);
	ShmPTR->front = -1;
	ShmPTR->rear = -1;
	ShmPTR->status = 1;
	ShmPTR->lock = 0;
	ShmID2=shmget(ShmKEY2,(ShmPTR->MAX*sizeof(struct Photocopy)),IPC_CREAT|0777);
	if(ShmID2<0){
		printf("\n shmget() for ShmKEY2 Failed\n");
		exit(1);
	}
	ShmPTR->cqueue_arr=(struct Photocopy *)shmat(ShmID2,NULL,0);
	if(ShmPTR->cqueue_arr == (struct Photocopy *)(-1)){
		printf("\n shmat() for ShmID2 Failed\n");
		exit(1);
	}
	printf("Photocopy Centre's SHMID is %d \n",ShmID);
	printf("Photocopy Centre's Queue's SHMID is %d \n",ShmID2);

	runtime = atoi(argv[2]);
	start = time(NULL);
	int terminate = 1;
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
