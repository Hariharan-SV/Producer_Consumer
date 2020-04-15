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

	if(argc!=9){
        printf("\nIncorrect args\nCorrect usage is %s -s <store's_shmid> -q <queue's_shmid> -t <max_sleep_time> -w <max_work_time>",argv[0]);
        return 1;
    }
	key_t ShmKEY,ShmKEY2;
	int	ShmID,ShmID2,sleeptime,worktime,printer_no;
	struct Circular_Queue *ShmPTR;

	ShmKEY = atoi(argv[2]);
	ShmKEY2 = atoi(argv[4]);
	sleeptime = atoi(argv[6]);
	worktime = atoi(argv[8]);

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

	printer_no = 1;

	printf("\nSuccess !!\nShared Memories are attached !!");
	int check=0;

	while (ShmPTR->status == 1)
	{
		while (ShmPTR->lock == 1){
			printf("\nSleeping...");
			sleep(3);
		}

		ShmPTR->lock = 1;
		int x = isEmpty(ShmPTR);
		ShmPTR->lock = 0;
		if(x == -1){
			printf("\nNo Requests in queue");
			printf("\nPrinter %d goes to sleep for %d secs.\n",printer_no,sleeptime);
			sleep(sleeptime);
		}else if (check == 1){
			displayall(ShmPTR);
			struct Photocopy *Front = getFront(ShmPTR);
			if(Front->filesize*Front->copies > worktime){
				Front->filesize=Front->filesize*Front->copies - worktime;
				Front->copies=1;
				printf("\nPrinter %d goes works for %d secs.\n",printer_no,worktime);
				sleep(worktime);
			}else{
				printf("\nPrinter %d works for %d secs.\n",printer_no, Front->filesize*Front->copies);
				sleep( Front->filesize*Front->copies);
				deletion(ShmPTR);
			}
			check = 0;
		}else{
			check = 1;
		}
		printer_no = (printer_no + 1) % ShmPTR->no_of_devices;
	}
	
    shmdt((void*)ShmPTR);

}
