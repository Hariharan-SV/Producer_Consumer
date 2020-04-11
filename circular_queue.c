#include <stdio.h>
#include <stdlib.h>
#include "photocopy.h"

int MAX;
int front = -1;
int rear = -1;

struct Photocopy *cqueue_arr;

struct Photocopy* initialize(int x){
	MAX=x;
	cqueue_arr = (struct Photocopy *)malloc(MAX*sizeof(struct Photocopy ));
	return cqueue_arr;
}

int insert(int item)
{
	if((front == 0 && rear == MAX-1) || (front == rear+1)){
		return 1;
	}
	if(front == -1){
		front = 0;
		rear = 0;
	}
	else
	{
		if(rear == MAX-1)
			rear = 0;
		else
			rear = rear+1;
	}
	cqueue_arr[rear].copies = item ;
	printf("%d %d\n",cqueue_arr[rear].copies,rear);
	return 0;
}

int deletion()
{
	if(front == -1)
	{
		printf("Queue Underflown");
		return 1;
	}
	printf("Element deleted from queue is : %d\n",cqueue_arr[front].copies);
	if(front == rear)
	{
		front = -1;
		rear=-1;
	}
	else
	{
		if(front == MAX-1)
			front = 0;
		else
			front = front+1;
	}
	return 0;
}

void display()
{
	int front_pos = front,rear_pos = rear;

	if(front == -1)
	{
		printf("Queue is empty\n");
		return;
	}
	printf("Queue elements :\n");
	if( front_pos <= rear_pos )
		while(front_pos <= rear_pos)
		{
			printf("%d %d\n",front_pos,cqueue_arr[front_pos].copies);
			front_pos++;
		}
	else
	{
		while(front_pos <= MAX-1){
			printf("%d %d\n",front_pos,cqueue_arr[front_pos].copies);
			front_pos++;
		}
		front_pos = 0;
		while(front_pos <= rear_pos){
			printf("%d %d\n",front_pos,cqueue_arr[front_pos].copies);
			front_pos++;
		}
	}
	printf("\n");
}

