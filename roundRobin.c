#include <stdio.h>
#include <locale.h>

// definição de constantes para as fatias de tempo e nro. máximo de processos
#define timeSlice 4
#define MAXprocesses 100

// definição da estrutura de processos
typedef struct {
    int id;
    int burstTime;
    int remainingTime;
} Process;

// definição de uma estrutura fila circular
typedef struct {
    Process* processes[MAXprocesses];
    int front, rear;
} Queue;

// prototipação das funções
void initializeQueue(Queue* queue);
int queueEmpty(Queue* queue);
void enqueue(Queue* queue, Process* process);
Process* dequeue(Queue* queue);
void displayState(int time, Process* process, int burstTime, Queue* queue);
void roundRobin(Queue* queue, int n);

// inicializa a fila como vazia
void initializeQueue(Queue* queue) {
    queue->front = 0;
    queue->rear = 0;
}

// verifica se a fila está vazia comparando os índices de ínicio e fim
int queueEmpty(Queue* queue) {
    // se retornar '1' está vazia, se '0' não está vazia
    return queue->front == queue->rear;
}

/* insere um elemento no final da fila e atualiza 
o índice 'rear' para a próxima posição de forma circular */
void enqueue(Queue* queue, Process* process) {
    int next_rear = (queue->rear + 1) % MAXprocesses;
    // verifica se a fila está cheia antes de adicionar um novo processo.
    if (next_rear != queue->front) {
        queue->processes[queue->rear] = process;
        queue->rear = next_rear;
    } else {
        wprintf(L"A fila está cheia. Não é possível enfileirar.\n");
    }
}

/* remove um elemento do início da fila e atualiza o índice 
'front' para a próxima posição de forma circular */
Process* dequeue(Queue* queue) {
    if (!queueEmpty(queue)) {
        Process* process = queue->processes[queue->front];
        queue->front = (queue->front + 1) % MAXprocesses;
        return process;
    } else {
        wprintf(L"A fila está vazia. Não é possível desenfileirar.\n");
        return NULL;
    }
}

// imprime o estado atual da execução do processo
void displayState(int time, Process* process, int burstTime, Queue* queue) {
    wprintf(L"Tempo %d - %d \t Processo em execução: P%d \t Tempo restante: %d -> %d \t Ação executada: executa %d %s \t\t Próxima fila: ",
            time, time + burstTime - 1,
            process->id, process->remainingTime + burstTime, process->remainingTime,
            burstTime, (process->remainingTime == 0) ? L"(Concluído)" : L"");

    int i = queue->front;
    while (i != queue->rear) {
        wprintf(L"P%d ", queue->processes[i]->id);
        i = (i + 1) % MAXprocesses;
    }

    if (process->remainingTime > 0) {
        wprintf(L"P%d", process->id);
    }

    wprintf(L"\n");
}

// executa o algoritmo de round robin
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

                // verifica se o processo ainda tem tempo restante para ser executado, se sim, re-enfileira o processo.
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
    setlocale(LC_ALL, "Portuguese");
    int n;
    wprintf(L"Insira o número de processos: ");
    scanf("%d", &n);

    Process processes[n];
    Queue queue;
    initializeQueue(&queue);

    /* inicializa o vetor de processos com os tempos de execução informados
    pelo usuário e enfileira cada processo */
    for (int i = 0; i < n; i++) {
        wprintf(L"Insira o tempo de execução do processo %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        processes[i].id = i + 1;
        processes[i].remainingTime = processes[i].burstTime;
        enqueue(&queue, &processes[i]);
    }

    printf("\n");
    
    // executa o algoritmo de escalonamento round robin
    roundRobin(&queue, n);

    return 0;
}
