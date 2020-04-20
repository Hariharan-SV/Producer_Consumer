#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_queue.h"

int insert(int filesize,int copies,char filename[20],struct Circular_Queue *CQ)
{
	// Normal insertion logic of a circular queue
	// if the front index is equal to rear index  + 1 ( or ) front index is 0 and rear index is max 
	if((CQ->front == 0 && CQ->rear == CQ->MAX-1) || (CQ->front == CQ->rear+1)){
		return -1;
	}// At start both indexes will be -1 after inserting first element both will be 0
	if(CQ->front == -1){
		CQ->front = 0;
		CQ->rear = 0;
	}// If the element to be inserted is not the first element
	else{
		// If the rear index has reached max size it is changed to 0
		if(CQ->rear == CQ->MAX-1)
			CQ->rear = 0;
		else
			CQ->rear = CQ->rear+1;
	}
	// Insertion of elements to photocopy is made at the photocopy of rear index
	CQ->cqueue_arr[CQ->rear].copies = copies ;
	CQ->cqueue_arr[CQ->rear].filesize = filesize;
	strcpy(CQ->cqueue_arr[CQ->rear].filename,filename);
	printf("%s %d\n",CQ->cqueue_arr[CQ->rear].filename,CQ->rear);
	return 0;
}

// The front index will be returned for reference
struct Photocopy* getFront(struct Circular_Queue *CQ)
{
	printf("\n%d copies of %d pages of File : %s\n",CQ->cqueue_arr[CQ->front].copies,CQ->cqueue_arr[CQ->front].filesize,CQ->cqueue_arr[CQ->front].filename);
	return &(CQ->cqueue_arr[CQ->front]);
};

// Function to delete front element in the queue
int deletion(struct Circular_Queue *CQ)
{
	// deletion will not be done if the queue is empty
	if(CQ->front == -1){
		return -1;
	}
	printf("Photocopy is taken so file : %s is removed from queue\n",CQ->cqueue_arr[CQ->front].filename);
	if(CQ->front == CQ->rear){
		CQ->front = -1;
		CQ->rear=-1;
	}// if the rear index and front index is max size it will be set to 0 as it is circular else front will be incremented
	else{
		if(CQ->front == CQ->MAX-1)
			CQ->front = 0;
		else
			CQ->front = CQ->front+1;
	}
	return 0;
}

// negative return value means queue is empty
// else it is not empty
int isEmpty(struct Circular_Queue *CQ){
	if(CQ->front == -1){
		return -1;
	}else{
		return 0;
	}
}

// displaying all elements in circular queue
int displayall(struct Circular_Queue *CQ)
{
	int front_pos = CQ->front,rear_pos = CQ->rear;
	// checking whether queue is empty
	if(CQ->front == -1)
	{
		return -1;
	}
	printf("Queue elements :\n");
	// if front index is less than rear index
	if( front_pos <= rear_pos )
		while(front_pos <= rear_pos)
		{
			printf("Queue[%d]:-\nCopies:%d\tPages:%d\tFilename:%s\n",front_pos,CQ->cqueue_arr[front_pos].copies,CQ->cqueue_arr[front_pos].filesize,CQ->cqueue_arr[front_pos].filename);
			front_pos++;
		}
	else
	{	//front index is not less than rear index so it is split as front index to max and 0 to rear index
		while(front_pos <= CQ->MAX-1){
			printf("Queue[%d]:-\nCopies:%d\tPages:%d\tFilename:%s\n",front_pos,CQ->cqueue_arr[front_pos].copies,CQ->cqueue_arr[front_pos].filesize,CQ->cqueue_arr[front_pos].filename);
			front_pos++;
		}
		front_pos = 0;
		while(front_pos <= rear_pos){
			printf("Queue[%d]:-\nCopies:%d\tPages:%d\tFilename:%s\n",front_pos,CQ->cqueue_arr[front_pos].copies,CQ->cqueue_arr[front_pos].filesize,CQ->cqueue_arr[front_pos].filename);
			front_pos++;
		}
	}
	printf("\n");
	return 0;
}

