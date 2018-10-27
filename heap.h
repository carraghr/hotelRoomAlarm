#ifndef heap
#define heap

#define INITINAL_HEAP_SIZE 3;

typedef struct heapNode{
    time_t time;
    int roomNumber;
}Node;

typedef struct{
    int numberOfElements;
    int size;
    int scheduledTimes;
    int expiredTimes;
    Node *timeHolderList;
}Heap;

void init(Heap *heap);
void addElement(Heap *heap, Node *heapNode);
Node removeTopElement(Heap *heap);
Node peek(Heap *heap);
void upHeap(Heap *heap,int nodeIndex);
void downHeap(Heap *heap,int currentPosition);
void swapHeapElements(Heap *heap, int currentPosition, int parent);
int findMinChild(Heap *heap, int currentPosition);
void extendHeap(Heap *heap);

#endif