#include "photocopy.h"

struct Circular_Queue{
    int MAX;                            // A Macro assigned for Maximum number of elements in a Circular Queue
    int front;                          // Pointer pointing the front of the Queue
    int rear;                           // Pointer pointing the rear of the Queue
    int no_of_devices;                  // Number of Devices in Circular Queue
    int status;                         // Status of each device in Circular Queue
    int lock;                           // Lock value
    struct Photocopy *cqueue_arr;       // Photocopy Structure to be assigned as an element in the Circular Queue
};
