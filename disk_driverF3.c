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
    request *old_req = find_request_similar(queue_proc, block_id, operation);
    if (old_req != NULL && old_req->processing == 0) {
        old_req->req_waiting[old_req->num_waiting] = p_id;
        old_req->num_waiting++;
        //overwrite the old block with the content of the new block
        if (operation == 1) {
            memcpy(old_req->block, block, BLOCK_SIZE);
        }
        printf("\tDRIVER: request re-enqueued, pid: %d, block_id: %d\n", p_id, block_id);
    } else {
            // create new request
        struct request req;
        req.operation = operation;
        req.pid = p_id;
        req.block_id = block_id;
        // copy the content of the block in the request. Use memcpy as in pkernel
        memcpy(req.block, block, BLOCK_SIZE);
        req.error = error;
        req.num_waiting = 0;
        req.processing = 0;
        //add request to queue
        enqueue_ordered(queue_proc, &req);
        printf("\tDRIVER: request o-enqueued, pid: %d, block_id: %d\n", p_id, block_id);
    }
    /*
        comprobar si existe una peticion anterior de mismo bloque y misma opercion
        - Si lectura, junto y comunico a todos
        - Si escritura, junto y ejecuto la ultima, y aviso a todos
        - Si hay una escritura y llega una lectura, enqueue al final, ordenado o sin ordenar (al final)??
        - Si hay mas de 10, enqueue, ordenado o sin ordenar (al final)?


        find_request con lo que me llega

        si request.operation == operation
            si operation == LECTURA
                si  NO hay una escritura en la cola despues de la lectura en el mismo bloque
                    si estan esperando menos de 10
                        añadimos una nueva a esperar
                    si no
                        añadimos una nueva peticion
                si no
                    seguimos buscando
            si operacion == ESCRITURA
                si NO hay una lectura en la cola despues de la escritura en el mismo bloque
                    si estan esperando menos de 10
                        añadimos una nueva a esperar
                    si no
                        añadimos una nueva peticion
                si no
                    seguimos buscando
        */
    if (!is_disk_processing()) {
        // request data to device
        request *req = find_request(queue_proc, block_id, operation);
        if (operation == 1) {
            req->processing = 1;
        }

        printf("\tDRIVER: request sent, pid: %d, block_id: %d\n", p_id, block_id);
        int ret = request_data_to_device(operation, block_id, block, error);
        if (ret == -1) { //error
            return -1;
        }
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



    // mandamos a la primera peticion
    int ret = send_data_to_process(req->pid, req->operation, req->block_id, req->block, req->error);
    if (ret == -1) {
        return -1 ;
    }
    //iteramos sobre todas las peticiones esperando para notificar
    int num_waiting = req->num_waiting;
    int i;
    for (i = 0; i < num_waiting; ++i) {
        printf("\tDRIVER: sending data, pid: %d, block_id: %d\n",req->req_waiting[i], req->block_id);
        int ret = send_data_to_process(req->req_waiting[i], req->operation, req->block_id, req->block, req->error);
        if (ret == -1) {
            return -1 ;
        }
    }

    request *next = next_request(queue_proc, req);
    printf("\tDRIVER: request dequeued, block_id: %d\n", req->block_id);
    remove_request(queue_proc, req);
    if (next != NULL) {
        if (operation == 1) {
            req->processing = 1;
        }
        printf("\tDRIVER: pending sent, block_id: %d\n", next->block_id);
        int ret = request_data_to_device(next->operation, next->block_id, next->block, next->error);
        if (ret == -1) {
            return -1;
        }
    }

    pthread_mutex_unlock(&lock);
    return 1;
}
