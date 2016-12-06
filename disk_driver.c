/*
 *  disk_driver.c
 *
 *  DrvSim. Version 2.0
 *
 *  INFODSO@ARCOS.INF.UC3M.ES
 *
 */

#include "disk_driver.h"
#include "queue.h"
#include <pthread.h>



struct queue *queue_proc;
pthread_mutex_t lock;


/*
 * Elements available within pKernel
 */

int send_data_to_process (pid_t p_id, int operation, int block_id, char *block, int error) ;
int request_data_to_device (int operation, int block_id, char *block, int error); // returns -1 on error.
int is_disk_processing (void) ;


/*
 * Elements to be implemented
 */
int mount_driver (void)
{
    //create a new queue
    queue_proc = queue_new();
    return 1;
}

int unmount_driver (void)
{
    //destroy the queue of processes
    queue_destroy(queue_proc);
    return 1;
}


/* receive a request from a client.
    - If the client is free, send the request to the device
    - If not, enqueue the request
*/
int disk_driver_block_request (pid_t p_id, int operation, int block_id, char *block, int error)
{
    
    pthread_mutex_lock(&lock);

    // create new request
    struct request req;
    req.operation = operation;
    req.pid = p_id;
    req.block_id = block_id;
    // copy the content of the block in the request. Use memcpy as in pkernel
    memcpy(req.block, block, BLOCK_SIZE);
    req.error = error;
    //add request to queue
    enqueue_ordered(queue_proc, &req);
    queue_print(queue_proc);
    printf("\tDRIVER: request o-enqueued, pid: %d, block_id: %d\n", p_id, block_id);

    if (!is_disk_processing()) {
        // request data to device
        printf("\tDRIVER: request sent, pid: %d, block_id: %d\n", p_id, block_id);
        request_data_to_device(operation, block_id, block, error);
    }

    pthread_mutex_unlock(&lock);

    return 1;
}

/*
Device response.
    - Dequeue the request
    - Send it to the client
    - If the queue is not empty and the device is free
        - Send the first request of the queue.
*/
int disk_driver_hardware_interrupt_handler (pid_t p_id, int operation, int block_id, char *block, int error )
{
    pthread_mutex_lock(&lock);

    request *req = find_request(queue_proc, block_id, operation);
    printf("\tDRIVER: sending data, pid: %d, block_id: %d\n",req->pid, req->block_id);
    send_data_to_process(req->pid, req->operation, req->block_id, req->block, req->error);

    request *next = next_request(queue_proc, req);
    printf("\tDRIVER: request dequeued, block_id: %d\n", req->block_id);
    remove_request(queue_proc, req);

    if (next != NULL  && !is_disk_processing()) {
        printf("\tDRIVER: pending sent, block_id: %d\n", next->block_id);
        request_data_to_device(next->operation, next->block_id, next->block, next->error);
    }

    pthread_mutex_unlock(&lock);

    return 1;
}
