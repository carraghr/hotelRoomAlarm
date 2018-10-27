#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <heap.h>
#define RAND_MAX_TIME_RANGE 100

pthread_mutex_t lock;
pthread_cond_t updatedHeap;

void *generator(void *passedHeap){
    Heap *heap = (Heap*)passedHeap;
    Node roomAlarm, checkNode;
    
    while(heap!=NULL){
        pthread_mutex_lock(&lock);
        checkNode = peek(heap);
        roomAlarm.time = time(NULL) + (rand() % RAND_MAX_TIME_RANGE);
        roomAlarm.roomAlarm = rand()%100;
        addElement(heap,roomAlarm);
        printf("Wake-up time registered for room: %i at %s\n\n", roomAlarm.roomNumber, ctime(&roomAlarm.time));
        printf("Expired wake-up alarms: %i.\nPending wake-up calls: %i.\n\n",heap->expiredTimes, heap->scheduledTimes);
        if(roomAlarm.time == checkNode.time){
            pthread_cond_signal(&updatedHeap);
        }
        pthread_mutex_unlock(&lock);
		sleep(rand()%5);
    }
    return (0);
}

void *notifier(void *passedHeap){
    Heap *heap = (Heap*)passedHeap;
    Node roomAlarm;
    time_t currentTime;
    
    while(heap!=NULL){
        pthread_mutex_lock(&lock);
        checkNode = peek(heap);
        
        currentTime = time(NULL);
        
        Node topAlarm = peek(heap); 
        while(topAlarm.time > currentTime){
            pthread_cond_timedwait(&updatedHeap,&lock,topAlarm.time-currentTime);
            currentTime = time(NULL);
            checkNode = peek(heap);
        }
        while(topAlarm.time < currentTime){
            checkNode = removeTopElement(heap);
            printf("Alarm: room: %i at %s\n",temp.roomNumber ,ctime(&tempTime));
            currentTime = time(NULL);
            checkNode = peek(heap);
        }
        
        pthread_mutex_unlock(&lock);
    }
    return (0);
}

int main(int argc, char **argv){
    
    Heap heap;
    init(&heap);
    
    pthread generateTimes, alarmNotifier;
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&updatedHeap,NULL)
    
    pthread_create(&generateTimes,NULL,generator,(void*)&heap);
    pthread_create(&alarmNotifier,NULL,notifier,(void*)&heap);
    
    pthread_join(generateTimes, NULL);
	pthread_join(alarmNotifier, NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&updatedHeap);
	return 0;
    
}
