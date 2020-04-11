#include <stdio.h>
#include "circular_queue.c"

int main(int argc,char* argv[]){
    if(argc!=2){
        printf("\nIncorrect args\nCorrect usage is %s -m <shmid> -n <no_of_copies> -p <no_of_pages> -f <file_name>",argv[0]);
        return 1;
    }
    int choice,item;
    int max=atoi(argv[1]);
    struct photocopy *cqueue_arr=initialize(max);
	do
	{
		printf("1.Insert\n");
		printf("2.Delete\n");
		printf("3.Display\n");
		printf("4.Quit\n");
		printf("Enter your choice : ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 :
				printf("Input the element for insertion in queue : ");
				scanf("%d", &item);
				insert(item);
				break;
			case 2 :
				deletion();
				break;
			case 3:
				display();
				break;
			case 4:
				break;
			default:
				printf("Wrong choice\n");
		}
	}while(choice!=4);
	
    return 0;
}