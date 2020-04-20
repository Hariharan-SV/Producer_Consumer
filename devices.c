#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include "circular_queue.c"

int main(int argc,char *argv[]) {

	if(argc != 9 || (strcmp("-s",argv[1]) || strcmp("-q",argv[3]) || strcmp("-t",argv[5]) || strcmp("-w",argv[7]) )){
        printf("\nIncorrect args\nCorrect usage is %s -s <store's_shmid> -q <queue's_shmid> -t <max_sleep_time> -w <max_work_time>",argv[0]);
        return 1;
    }												// checks whether format specifier and their count are in correct format

	key_t ShmKEY,ShmKEY2;							// holds generated key for shared memory
	int	ShmID,ShmID2,sleeptime,worktime,printer_no; // holds shared memory for generated key
	struct Circular_Queue *ShmPTR;					// Shared memory consists of lock,status,circular queue

	// assigning values inputed in cmd line args
	ShmKEY = atoi(argv[2]);
	ShmKEY2 = atoi(argv[4]);
	sleeptime = atoi(argv[6]);
	worktime = atoi(argv[8]);

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

	printer_no = 1;	// Unique number to identify printer

	printf("\nSuccess !!\nShared Memories are attached !!");
	int file_descriptor = 0;
	file_descriptor = open("printer_log.txt", O_CREAT | O_WRONLY |O_APPEND,0700);
	// Photocopy operations are written on this file for future references

	while (ShmPTR->status == 1)
	{
		while (ShmPTR->lock == 1){
			printf("\nSleeping...");
			sleep(3);
		}
		// the queue will not be checked while inserting the data
		ShmPTR->lock = 1;
		int x = isEmpty(ShmPTR);
		ShmPTR->lock = 0;
		if(x == -1){
			printf("\nNo Requests in queue");
			printf("\nPrinter %d goes to sleep for %d secs.\n",printer_no,sleeptime);
			sleep(sleeptime);
			// if there are no requests then the device which checks the queue goes to sleep
		}else{
			while (ShmPTR->lock == 1){
				printf("\nSleeping...");
				sleep(3);
			}
			// the front of the queue will not be received while inserting the data
			ShmPTR->lock = 1;
			displayall(ShmPTR);
			struct Photocopy *Front = getFront(ShmPTR);
			ShmPTR->lock = 0;
			if(Front->filesize*Front->copies > worktime){
				Front->filesize=Front->filesize*Front->copies - worktime;
				Front->copies=1;
				printf("\nPrinter %d goes works for %d secs.\n",printer_no,worktime);
				sleep(worktime);
				write(file_descriptor,Front->filename,strlen(Front->filename));
				write(file_descriptor," printed by printer : ",strlen(" printed by printer : "));
				char x[2];
  				sprintf(x, "%d", printer_no);
				write(file_descriptor,x,sizeof(x));
				write(file_descriptor,"\n",strlen("\n"));
				// the device which checked the queue will work for maxworktime mentioned already
			}else{
				// if the time need to print the file is less than or equal to max work time
				// the task will be done and then next device will check the queue
				printf("\nPrinter %d works for %d secs.\n",printer_no, Front->filesize*Front->copies);
				sleep( Front->filesize*Front->copies);
				write(file_descriptor,Front->filename,strlen(Front->filename));
				write(file_descriptor," printed by printer : ",strlen(" printed by printer : "));
				char x[2];
  				sprintf(x, "%d", printer_no);
				write(file_descriptor,x,sizeof(x));
				write(file_descriptor,"\nCompleted",strlen("\nCompleted"));
				write(file_descriptor,"\n",strlen("\n"));
				while (ShmPTR->lock == 1){
					printf("\nSleeping...");
					sleep(3);
				}	// the front of the queue will not be deleted if it is already used by other program
				ShmPTR->lock = 1;
				deletion(ShmPTR);
				ShmPTR->lock = 0;
			}
		}
		printer_no = (printer_no + 1) % ShmPTR->no_of_devices; // swirching onto next printer
	}
	
    close(file_descriptor);
    shmdt((void*)ShmPTR);

}
