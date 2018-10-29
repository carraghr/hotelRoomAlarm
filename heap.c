#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "minHeap.h"

void swapHeapElements(Heap *heap, int currentPosition, int parent){
    Node temp = heap->timeHolderList[currentPosition];
    heap->timeHolderList[currentPosition] = heap->timeHolderList[parent];
    heap->timeHolderList[parent] = temp;
}

void init(Heap *heap){
    heap->size = INITINAL_HEAP_SIZE;
    heap->numberOfElements = 0;
    heap->scheduledTimes=0;
    heap->expiredTimes=0;
    heap->timeHolderList = (Node*)malloc(sizeof(Node)*heap->size);
}

void extendHeap(Heap *heap){
    heap->size = heap->size * 2;
    heap->timeHolderList = realloc(heap->timeHolderList,heap->size*(sizeof(Node)));
}

int getParentIndex(int nodeIndex){
    return (nodeIndex - 1) / 2;
}

int getLeftChildIndex(int nodeIndex){
    return 2 * nodeIndex + 1;
}

int getRightChildIndex(int nodeIndex){
    return 2 * nodeIndex + 2;
}

int findMinChild(Heap *heap, int currentPosition){
    int leftChildIndex = getLeftChildIndex(currentPosition);
    int rightChildIndex = getRightChildIndex(currentPosition);
    if(rightChildIndex >= heap->numberOfElements){
        return leftChildIndex;
    }else{
        if(heap->timeHolderList[leftChildIndex].time <= heap->timeHolderList[rightChildIndex].time){
            return leftChildIndex;
        }
        return rightChildIndex;
    }
}

void downHeap(Heap *heap,int currentPosition){
    int minChild = findMinChild(heap,currentPosition);
    if((heap->timeHolderList[currentPosition]).time > (heap->timeHolderList[minChild]).time && minChild <= heap->numberOfElements){
        swapHeapElements(heap,currentPosition,minChild);
        downHeap(heap, minChild);
    }
}

void upHeap(Heap *heap,int nodeIndex){
    
    int parent;
    
    if(nodeIndex != 0){
        int parentIndex = getParentIndex(nodeIndex);
        if(heap->timeHolderList[parentIndex].time > heap->timeHolderList[nodeIndex].time){
          swapHeapElements(heap,nodeIndex,parentIndex);
          upHeap(heap,parentIndex);
        }
    }
}

void addElement(Heap *heap, Node* heapNode){
    if(heap->numberOfElements == heap->size){
        extendHeap(heap);
    }
    heap->scheduledTimes++;
    heap->numberOfElements++;
    heap->timeHolderList[heap->numberOfElements-1] = *heapNode;
    upHeap(heap,heap->numberOfElements-1);
}

Node* removeTopElement(Heap *heap){
    Node temp;
    temp = heap->timeHolderList[0];
    heap->timeHolderList[0] = heap->timeHolderList[heap->numberOfElements-1];
    heap->numberOfElements--;
        heap->expiredTimes++;
    downHeap(heap,0);
    Node *ret;
    ret=(Node*)malloc(sizeof(Node));
    ret->time = temp.time;
    ret->roomNumber = temp.roomNumber;
    return ret;
}

Node* peek(Heap *heap){
    Node temp;
    temp = heap->timeHolderList[0];
    Node *ret;
    ret=(Node*)malloc(sizeof(Node));
    ret->time = temp.time;
    ret->roomNumber = temp.roomNumber;
    return ret;
}

bool isEmpty(Heap *heap){
   return heap->numberOfElements < 1;
}