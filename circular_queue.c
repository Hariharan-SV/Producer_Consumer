#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_queue.h"

int insert(int filesize,int copies,char filename[20],struct Circular_Queue *CQ)
{
	if((CQ->front == 0 && CQ->rear == CQ->MAX-1) || (CQ->front == CQ->rear+1)){
		return -1;
	}
	if(CQ->front == -1){
		CQ->front = 0;
		CQ->rear = 0;
	}
	else{
		if(CQ->rear == CQ->MAX-1)
			CQ->rear = 0;
		else
			CQ->rear = CQ->rear+1;
	}
	CQ->cqueue_arr[CQ->rear].copies = copies ;
	CQ->cqueue_arr[CQ->rear].filesize = filesize;
	strcpy(CQ->cqueue_arr[CQ->rear].filename,filename);
	printf("%s %d\n",CQ->cqueue_arr[CQ->rear].filename,CQ->rear);
	return 0;
}

struct Photocopy* getFront(struct Circular_Queue *CQ)
{
	printf("\n%d %d %s\n",CQ->cqueue_arr[CQ->front].copies,CQ->cqueue_arr[CQ->front].filesize,CQ->cqueue_arr[CQ->front].filename);
	return &(CQ->cqueue_arr[CQ->front]);
};


int deletion(struct Circular_Queue *CQ)
{
	if(CQ->front == -1){
		return -1;
	}
	printf("Element deleted from queue is : %s\n",CQ->cqueue_arr[CQ->front].filename);
	if(CQ->front == CQ->rear){
		CQ->front = -1;
		CQ->rear=-1;
	}
	else{
		if(CQ->front == CQ->MAX-1)
			CQ->front = 0;
		else
			CQ->front = CQ->front+1;
	}
	return 0;
}

int isEmpty(struct Circular_Queue *CQ){
	if(CQ->front == -1){
		return -1;
	}else{
		printf("\nSize :- %d\n",(CQ->rear - CQ->front + 1)%(CQ->MAX));
		return 0;
	}
}

int displayall(struct Circular_Queue *CQ)
{
	int front_pos = CQ->front,rear_pos = CQ->rear;

	if(CQ->front == -1)
	{
		return -1;
	}
	printf("Queue elements :\n");
	if( front_pos <= rear_pos )
		while(front_pos <= rear_pos)
		{
			printf("%d %d\n",front_pos,rear_pos);
			printf("%d %d\n",front_pos,CQ->cqueue_arr[front_pos].filesize);
			front_pos++;
		}
	else
	{
		while(front_pos <= CQ->MAX-1){
			printf("%d %d\n",front_pos,CQ->cqueue_arr[front_pos].filesize);
			front_pos++;
		}
		front_pos = 0;
		while(front_pos <= rear_pos){
			printf("%d %s\n",front_pos,CQ->cqueue_arr[front_pos].filename);
			front_pos++;
		}
	}
	printf("\n");
	return 0;
}

