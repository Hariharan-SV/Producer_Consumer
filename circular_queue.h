#include "photocopy.h"

struct Circular_Queue{
    int MAX;
    int front;
    int rear;
    int no_of_devices;
    struct Photocopy *cqueue_arr;
};