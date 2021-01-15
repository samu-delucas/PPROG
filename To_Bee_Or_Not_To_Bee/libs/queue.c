/**
 * @file    queue.c
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module implements a queue in c. 
 *          In the game it is used to store the continuation of certain quests, 
 *          to give the story a sense of continuity
 * */

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

#define MAXQUEUE 1024

struct _Queue{
    void* items [MAXQUEUE];
    int front;
    int rear;
    destroy_element_function_type free_element_function;
    copy_element_function_type copy_element_function;
    print_element_function_type print_element_function;
};

Queue* queue_init(destroy_element_function_type f1,
copy_element_function_type f2, print_element_function_type f3){
    Queue *q = NULL;
    int i;
    
    if(!f1 || !f2 || !f3){
        return NULL;
    }
    
    if(!(q = (Queue *)calloc(1, sizeof(Queue)))){
        return NULL;
    }
    
    for(i = 0; i < MAXQUEUE; i++){
        q->items[i] = NULL;
    }
    
    q->front = 0;
    q->rear = 0;
    
    q->free_element_function = f1;
    q->copy_element_function = f2;
    q->print_element_function = f3;
    
    return q;
}

void queue_free(Queue *q){
    int i;
    if(q){
        for(i = 0; i < MAXQUEUE; i++){
            if(q->items[i]){
                q->free_element_function(q->items[i]);
            }
        }
    free(q);
    }
}

bool queue_isEmpty(const Queue *q){
    if(!q){
        return TRUE;
    }
    
    if (q->front == q->rear) {
        return TRUE;
        
    }
    
    return FALSE;
}

bool queue_isFull(const Queue *q){
    if (q == NULL) {
        return TRUE;
        
    }
    
    if (q->front == (q->rear+1) % MAXQUEUE) {
        return TRUE;
    }
    
    return FALSE;
}

status queue_insert(Queue *q, const void* pElem){
    void * aux;
    
    if(!q || !pElem || queue_isFull(q) == TRUE){
        return ERROR;
    }
    
    aux = (q->copy_element_function)(pElem);
    if(!aux){
        return ERROR;
    }
    
    q->items[q->rear] = aux;
    q->rear = (q->rear + 1) % MAXQUEUE;
    return OK;
}

void * queue_extract(Queue *q){
    void * element = NULL;
    
    if(!q || queue_isEmpty(q) == TRUE){
        return NULL;
    }
    
    element = q->items[q->front];
    q->items[q->front] = NULL;
    q->front = (q->front+1) % MAXQUEUE;
    return element;
}

int queue_size (const Queue *q){
    if(!q){
        return -1;
    }
    
    return ((q->rear - q->front + MAXQUEUE) % MAXQUEUE);
}

int queue_print(FILE *pf, const Queue *q){
    int i, size, cont = 0;
    if(!q || !pf){
        return -1;
    }
    
    size = queue_size(q);
    for(i = 0; i < size; i++){
        cont += (q->print_element_function)(pf, q->items[(q->front + i) % MAXQUEUE]);
        cont += fprintf(pf, "\n");
    }  
    
    return cont;
}
