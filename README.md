# Producer_Consumer
Visualizing Producer-Consumer problem as a photocopy centre where clients are producers and printers are consumers

## About the Files
Here a circular queue is stored inside the shared memory
These programs will be working with shared memory

The repo has three main files
..*main.c     - creates shared memory
..*client.c   - inserts data into shared memory
..*devices.c  - views and deletes data from shared memory

and three additional files which provides support to above main files
..*circular_queue.c   - has functions to work with Circular Queue
..*circular_queue.h   - has Circular queue's structure
..*photocopy.h        - has Photocopy's structure

## Working
 The Circular queue structure has an array of Photocopy structure.
 Main program creates a queue for a store and the program will run for the time mentioned as a cmd arg.
 Client program inserts data into queue.
 Devices program views the queue and does nothing when the queue is empty, else it simulates printing the data for a specified time and deletes data if time elapsed is 0.
 All programs will terminate if main program for a store terminates.
 (i.e) Only a store has a queue, if there is no store then there will be no queue for a closed store.
 
## Run the files
 Execute in this order
 To create a queue
 ##### `./main -t <total_run_time> -n <no_of_printers> -l <length_of_queue>`
 it will return key for store and queue.
 use it in next 2 programs.
 
 To view and work with the queue.
 ##### `./devices -s <store's_key> -q <queue's_key> -b <max_break_time> -w <max_work_time> `
 
 To insert data into the queue.
 ##### `./client -s <store's_key> -q <queue's_key> -n <no_of_copies> -p <no_of_pages> -f <file_name> `
 
 Happy coding!
