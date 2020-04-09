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
}

int main(void) {
	time_t start, end;
	double elapsed;
	struct Memory *ShmPTR;
	printf("Shared Memory Centre SHMID is \n");
	start = time(NULL);
	int terminate = 1;
	while (terminate) {
		end = time(NULL);
		elapsed = difftime(end, start);
		if (elapsed >= 10.0)
			terminate = 0;
		else
			sleep(1);
	}
	printf("Shutting the centre without abnormal termination..\n");
	return 0;
 }
