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
    key_t	ShmKEY;
	int	ShmID;
	struct Memory *ShmPTR;
	int runtime;
		
	ShmKEY = 2011;

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

    printf("no_of_devices:%d\nqueue_size:%d\n",ShmPTR->no_of_devices,ShmPTR->queue_size);
    shmdt((void*)ShmPTR);

}