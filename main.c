#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "minHeap.h"

#define RAND_MAX_TIME_RANGE 100

pthread_mutex_t lock;
pthread_cond_t updatedHeap;

bool generatorEnd = false;
bool noficiationEnd = false;

int cleanup_pop_arg = 0;

void cleanUpGenerator(void *arg){
}

void *generator(void *passedHeap){
    Heap *heap = (Heap*)passedHeap;
    Node *roomAlarm, *checkNode;
    int i=0;
    pthread_cleanup_push(cleanUpGenerator,NULL);
    while(!generatorEnd){
        pthread_mutex_lock(&lock);
        checkNode = peek(heap);
        roomAlarm = (Node*)malloc(sizeof(Node));
        roomAlarm->time = time(NULL) + (rand() % RAND_MAX_TIME_RANGE);
        roomAlarm->roomNumber = rand()%100;
        addElement(heap,roomAlarm);
        printf("Wake-up time registered for room: %i at %s\n\n", roomAlarm->roomNumber, ctime(&roomAlarm->time));
        printf("Expired wake-up alarms: %i.\nPending wake-up calls: %i.\n\n",heap->expiredTimes, heap->scheduledTimes);
        if(roomAlarm->time < checkNode->time){
            pthread_cond_signal(&updatedHeap);
        }       
        pthread_mutex_unlock(&lock);
        sleep(rand()%5);
    }
    pthread_cleanup_pop(cleanup_pop_arg);
    printf("End of generator!!\n\n");
    return (0);
}

void *notifier(void *passedHeap){
    Heap *heap = (Heap*)passedHeap;
    Node roomAlarm;
    Node *checkNode, *topAlarm;
    time_t currentTime;    
    pthread_cleanup_push(cleanUpGenerator,NULL);
    while(!noficiationEnd){
        pthread_mutex_lock(&lock);
        while(isEmpty(heap)){
            pthread_cond_wait(&updatedHeap,&lock);
        }
        currentTime = time(NULL);
        topAlarm = peek(heap); 
        while(topAlarm->time > currentTime){
            struct timespec ts = {0,0};
            const int gettime_rv = clock_gettime(CLOCK_REALTIME,&ts);
            ts.tv_sec = topAlarm->time;                 
            pthread_cond_timedwait(&updatedHeap,&lock,&ts);
            currentTime = time(NULL);
            topAlarm = peek(heap);
        }
        while(topAlarm->time < currentTime){
            topAlarm = removeTopElement(heap);
            printf("Alarm: room: %i at %s\n",topAlarm->roomNumber ,ctime(&topAlarm->time));
            currentTime = time(NULL);
            topAlarm = peek(heap);
        }        
        pthread_mutex_unlock(&lock);
    }
    pthread_cleanup_pop(cleanup_pop_arg);
    printf("End of notifier!!\n\n");
    return (0);
}

void shutDownSignal(int signalCode){
    if(signalCode == SIGINT){
        noficiationEnd = true;
        generatorEnd = true;
        printf("Flags changed for threads\n\n");
    }
}

int main(int argc, char **argv){
    
    Heap heap;
    init(&heap);
    
    pthread_t generateTimes, alarmNotifier;
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&updatedHeap,NULL);
    
    signal(SIGINT,shutDownSignal);
    
    pthread_create(&generateTimes,NULL,generator,(void*)&heap);
    pthread_create(&alarmNotifier,NULL,notifier,(void*)&heap);
    
    pthread_join(generateTimes, NULL);
	pthread_join(alarmNotifier, NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&updatedHeap);
    //TODO clean up heap
	return 0;
}