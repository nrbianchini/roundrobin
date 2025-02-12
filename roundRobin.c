#include <stdio.h>

#define timeSlice 4
#define MAXprocesses 100

typedef struct {
    int id;
    int burstTime;
    int remainingTime;
} Process;

typedef struct {
    Process* processes[MAXprocesses];
    int front, rear;
} Queue;

void initializeQueue(Queue* queue);
int queueEmpty(Queue* queue);
void enqueue(Queue* queue, Process* process);
Process* dequeue(Queue* queue);
void displayState(int time, Process* process, int burstTime, Queue* queue);
void roundRobin(Queue* queue, int n);

// initialize queue as empty
void initializeQueue(Queue* queue) {
    queue->front = 0;
    queue->rear = 0;
}

// check if the queue is empty by comparing the start and end indices
int queueEmpty(Queue* queue) {
    // if it returns '1', the queue is empty; if '0', it is not
    return queue->front == queue->rear;
}

/* insert an element at the end of the queue and update the 'rear' 
index to the next position in a circular manner */
void enqueue(Queue* queue, Process* process) {
    int next_rear = (queue->rear + 1) % MAXprocesses;
    if (next_rear != queue->front) {
        queue->processes[queue->rear] = process;
        queue->rear = next_rear;
    } else {
        printf("The queue is full. It is not possible to enqueue.\n");
    }
}

/* remove an element from the front of the queue and update the 'front' 
index to the next position in a circular way */
Process* dequeue(Queue* queue) {
    if (!queueEmpty(queue)) {
        Process* process = queue->processes[queue->front];
        queue->front = (queue->front + 1) % MAXprocesses;
        return process;
    } else {
        printf("The queue is empty. It is not possible to dequeue.\n");
        return NULL;
    }
}

void displayState(int time, Process* process, int burstTime, Queue* queue) {
    printf("Time %d - %d \t Process in execution: P%d \t Time left: %d -> %d \t Action executed: executing %d %s \t\t Next queue: ",
            time, time + burstTime - 1,
            process->id, process->remainingTime + burstTime, process->remainingTime,
            burstTime, (process->remainingTime == 0) ? "(Done)" : "(Pending)");

    int i = queue->front;
    while (i != queue->rear) {
        printf("P%d ", queue->processes[i]->id);
        i = (i + 1) % MAXprocesses;
    }

    if (process->remainingTime > 0) {
        printf("P%d", process->id);
    }

    printf("\n");
}

void roundRobin(Queue* queue, int n) {
    int time = 0;
    int completed = 0;

    while (completed < n) {
        if (!queueEmpty(queue)) {
            Process* process = dequeue(queue);

            if (process->remainingTime > 0) {
                int burstTime = (process->remainingTime > timeSlice) ? timeSlice : process->remainingTime;

                process->remainingTime -= burstTime;
                displayState(time, process, burstTime, queue);
                time += burstTime;

                // check if the process has remaining execution time; if so, re-enqueue its
                if (process->remainingTime == 0) {
                    completed++;
                } else {
                    enqueue(queue, process);
                }
            }
        } else {
            break;
        }
    }
}

int main() {
    int n;
    printf("Insert the number of processes: ");
    scanf("%d", &n);

    Process processes[n];
    Queue queue;
    initializeQueue(&queue);

    for (int i = 0; i < n; i++) {
        printf("Insert the execution time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        processes[i].id = i + 1;
        processes[i].remainingTime = processes[i].burstTime;
        enqueue(&queue, &processes[i]);
    }

    printf("\n");
    
    roundRobin(&queue, n);

    return 0;
}
