#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define MAX_SIZE 4

struct Memory{
	int status;
	int data[4];
};

int main(int argc,char *argv[]) {

	if(argc!=2){
		printf("\nInsufficient arguments give ./a.out <total_run_time>");
		exit(1);
	}

	time_t start, end;
	double elapsed;
	key_t	ShmKEY;
	int	ShmID;
	struct Memory *ShmPTR;
	int runtime;

	runtime=atoi(argv[1]);

	ShmKEY=2011;

	ShmID=shmget(ShmKEY,sizeof(struct Memory),IPC_CREAT| 0666);
	if(ShmID<0){
		printf("\n shmget() Failed");
		exit(1);
	}

	ShmPTR=(struct Memory *)shmat(ShmID,NULL,0);
	if((int)ShmPTR == -1){
		printf("\n shmat() Failed");
		exit(1);
	}

	printf("Shared Memory Centre SHMID is %d \n",ShmID,ShmID);


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
