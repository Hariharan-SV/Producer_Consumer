#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>


struct Memory{
	int no_of_devices;
	int queue_size;
};


int main(int argc,char *argv[]) {

	if(argc!=7){
		printf("\nInsufficient arguments give ./main -t <total_run_time> -d <no_of_printers> -l <length_of_queue>");
		exit(1);
	}

	time_t start, end;
	double elapsed;
	key_t	ShmKEY;
	int	ShmID;
	struct Memory *ShmPTR;
	int runtime;
		
	ShmKEY = rand()%1000+1000;

	ShmID = shmget(ShmKEY,sizeof(struct Memory),IPC_CREAT| 0666);
	if(ShmID<0){
		printf("\n shmget() Failed");
		exit(1);
	}

	ShmPTR = (struct Memory *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed");
		exit(1);
	}

	printf("Photocopy Centre's SHMID is %d \n",ShmID);
	ShmPTR->no_of_devices = atoi(argv[4]);
	ShmPTR->queue_size = atoi(argv[6]);
	printf("no_of_devices:%d\nqueue_size:%d\n",ShmPTR->no_of_devices,ShmPTR->queue_size);

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
	shmdt((void*)ShmPTR);
	shmctl(ShmID,IPC_RMID,NULL);	
	printf("Shutting the centre without abnormal termination..\n");

	return 0;
 }
