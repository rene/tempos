#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node /* process node information */
{
    char name[20];     /* The name of the process */
    int priority;          /* priority of the process */
    int quantum;         /* allocate CPU time slice */
    int exectime;       /*CPU execution time*/
    int needtime;      /* time required for process execution*/
    char state;        /* state of the process, W - ready state, R - execution state, F - completion state */
    int executions;         /* record the number of executions */
    struct node *nextNode; /* linked list pointer */
} ProcessControlBlock;

typedef struct Queue /* multi-level ready queue node information */
{
    ProcessControlBlock *ProcessPointer;       /* Process queue pointer in the ready queue */
    int priority;           /* priority of this ready queue*/
    int quantum;          /* time slice allocated by this ready queue*/
    struct Queue *nextNode; /* Pointer pointer to the next ready queue */
} ReadyQueue;

ProcessControlBlock *run = NULL, *finish = NULL;             /* defines three queues, ready queue, execution queue and completion queue */
ReadyQueue *Head = NULL;                     /* defines the first ready queue */
int num;                                     /* number of processes */
int ReadyNum;                                /* Ready queue number */
void Output();                               /*Process information output function*/
void InsertFinish(ProcessControlBlock *in);                  /* Insert the process into the end of the completion queue*/
void InsertReady(ReadyQueue *in);             /*Create a ready queue, specifying the smaller the priority number, the lower the priority */
void CreateReadyQueue();                           /*Create ready queue input function*/
void GetFirst(ReadyQueue *queue);            /*Get the head process in a ready queue*/
void InsertLast(ProcessControlBlock *in, ReadyQueue *queue); /*Inserts the process to the end of the ready queue*/
void ProcessCreate();                        /*Process creation function */
void RoundRobin(ReadyQueue *timechip);         /* Time slice rotation scheduling algorithm */
void Scheduling();                             /*Multi-level scheduling algorithm, one time slice per execution*/

int main(void)
{
    CreateReadyQueue();    /*Create ready queue*/
    ProcessCreate(); /*Create a ready process queue*/
    Scheduling(); /* algorithm starts*/
    Output();        /* Output the final dispatch sequence */
    return 0;
}

void Output() /*Process information output function*/
{
    ReadyQueue *tempReady = Head;
    ProcessControlBlock *PCB;
    printf(" -------------------------------------------------------------------------------------------------------------\n");
    printf("| %7s\t | %6s\t | %8s\t | %8s\t | %9s\t | %7s\t | %10s |\n", "pname", "status", "priority","cpu time","need time", "quantum","executions");
    printf(" -------------------------------------------------------------------------------------------------------------\n");
    while (tempReady)
    {
        if (tempReady->ProcessPointer != NULL)
        {
            PCB = tempReady->ProcessPointer;
            while (PCB)
            {
                printf("| %7s\t | %6c\t | %8d\t | %8d\t | %9d\t | %6d\t | %6d     |\n", PCB->name, PCB->state, PCB->priority, PCB->exectime, PCB->needtime, PCB->quantum, PCB->executions);
                PCB = PCB->nextNode;
            }
        }
        tempReady = tempReady->nextNode;
    }
    PCB = finish;
    while (PCB != NULL)
    {
        printf("| %7s\t | %6c\t | %8d\t | %8d\t | %9d\t | %6d\t | %6d     |\n", PCB->name, PCB->state, PCB->priority, PCB->exectime, PCB->needtime, PCB->quantum, PCB->executions);
        PCB = PCB->nextNode;
    }
    PCB = run;
    while (PCB != NULL)
    {
        printf("| %7s\t | %6c\t | %8d\t | %8d\t | %9d\t | %6d\t | %6d     |\n", PCB->name, PCB->state, PCB->priority, PCB->exectime, PCB->needtime, PCB->quantum, PCB->executions);
        PCB = PCB->nextNode;
    }
    printf(" ---------------------------------------------------------------------------------------------------------------\n");

}
void InsertFinish(ProcessControlBlock *in) /* Inserts the process into the end of the completion queue */
{
    ProcessControlBlock *fst;
    fst = finish;

    if (finish == NULL)
    {
        in->nextNode = finish;
        finish = in;
    }
    else
    {
        while (fst->nextNode != NULL)
        {
            fst = fst->nextNode;
        }
        in->nextNode = fst->nextNode;
        fst->nextNode = in;
    }
}

void InsertReady(ReadyQueue *in) /*Creates a ready queue, specifying that the smaller the priority number, the lower the priority*/
{
    ReadyQueue *fst, *nxt;
    fst = nxt = Head;

    if (Head == NULL) /* If there is no queue, it is the first element */
    {
        in->nextNode = Head;
        Head = in;
    }
    else /*find the right place to insert */
    {
        if (in->priority >= fst->priority) /* is bigger than the first one, it is inserted into the team head */
        {
            in->nextNode = Head;
            Head = in;
        }
        else
        {
            while (fst->nextNode != NULL) /* Move the pointer to find the position of the first individual small element to insert */
            {
                nxt = fst;
                fst = fst->nextNode;
            }

            if (fst->nextNode == NULL) /* has searched for the end of the team, its priority is the smallest, insert it into the end of the team */
            {
                in->nextNode = fst->nextNode;
                fst->nextNode = in;
            }
            else /* is inserted into the queue */
            {
                nxt = in;
                in->nextNode = fst;
            }
        }
    }
}

void CreateReadyQueue() /*Create ready queue input function */
{
    ReadyQueue *tmp;
    int i;

    printf("[?] Input : Enter Number of ready queues: ");
    scanf("%d", &ReadyNum);

    for (i = 0; i < ReadyNum; i++)
    {
        printf("[?] Input : Enter the CPU time slice for ready queue %d: ", (i+1));
        if ((tmp = (ReadyQueue *)malloc(sizeof(ReadyQueue))) == NULL)
        {
            perror("[-] Malloc : Failed to Allocate Memory !!");
            exit(1);
        }
        scanf("%d", &(tmp->quantum));  /*Enter the CPU time slice allocated to each process in this ready queue*/
        tmp->priority = 50 - tmp->quantum; /* set its priority, the higher the time slice, the lower its priority */
        tmp->ProcessPointer = NULL;         /* Initialize the queue of its connected process is empty */
        tmp->nextNode = NULL;
        InsertReady(tmp); /*Create multiple ready queues by priority from high to low*/
    }
}

void GetFirst(ReadyQueue *queue) /*Get the queue process in a ready queue*/
{
    run = queue->ProcessPointer;

    if (queue->ProcessPointer != NULL)
    {
        run->state = 'R';
        queue->ProcessPointer = queue->ProcessPointer->nextNode;
        run->nextNode = NULL;
    }
}

void InsertLast(ProcessControlBlock *in, ReadyQueue *queue) /*Inserts the process to the end of the ready queue*/
{
    ProcessControlBlock *fst;
    fst = queue->ProcessPointer;

    if (queue->ProcessPointer == NULL)
    {
        in->nextNode = queue->ProcessPointer;
        queue->ProcessPointer = in;
    }
    else
    {
        while (fst->nextNode != NULL)
        {
            fst = fst->nextNode;
        }
        in->nextNode = fst->nextNode;
        fst->nextNode = in;
    }
}

void ProcessCreate() /*Process creation function */
{
    ProcessControlBlock *tmp;
    int i;

    printf("[?] Input : Enter number of input processes : ");
    scanf("%d", &num);
    for (i = 0; i < num; i++)
    {
        printf("[?] Input : Enter process name and process time %d: ", (i+1));
        if ((tmp = (ProcessControlBlock *)malloc(sizeof(ProcessControlBlock))) == NULL)
        {
            perror("[-] Malloc : Failed to Allocate Memory !!");
            exit(1);
        }
        scanf("%s", tmp->name);
        getchar(); /* absorb the carriage return symbol*/
        scanf("%d", &(tmp->needtime));
        tmp->exectime = 0;
        tmp->state = 'W';
        tmp->priority = 50 - tmp->needtime; /* set its priority, the more time it takes, the lower the priority */
        tmp->quantum = Head->quantum;
        tmp->executions = 0;
        InsertLast(tmp, Head); /* is inserted into the ready queue according to the priority from high to low*/
    }
}

void RoundRobin(ReadyQueue *timechip) /* Time slice rotation scheduling algorithm */
{
    int flag = 1;

    GetFirst(timechip);
    while (run != NULL)
    {
        while (flag)
        {
            run->executions++;
            run->exectime++;
            run->needtime--;
            if (run->needtime == 0) /*The process is completed*/
            {
                run->state = 'F';
                InsertFinish(run);
                flag = 0;
            }
            else if (run->executions == timechip->quantum) /*time slice is exhausted*/
            {
                run->state = 'W';
                run->executions = 0; /*The counter is cleared to prepare for the next time*/
                InsertLast(run, timechip);
                flag = 0;
            }
        }
        flag = 1;
        GetFirst(timechip);
        Output();
    }
}

void Scheduling() /* Multi-level scheduling algorithm, one time slice per execution*/
{
    int flag = 1;
    int k = 0;

    ReadyQueue *point;
    point = Head;

    GetFirst(point);
    while (run != NULL)
    {
        Output();
        if (Head->ProcessPointer != NULL)
            point = Head;
        while (flag)
        {
            run->executions++;
            run->exectime++;
            run->needtime--;
            if (run->needtime == 0) /*The process is completed*/
            {
                run->state = 'F';
                InsertFinish(run);
                flag = 0;
            }
            else if (run->executions == run->quantum) /*time slice runs out*/
            {
                run->state = 'W';
                run->executions = 0; /*The counter is cleared to prepare for the nextNode time*/
                if (point->nextNode != NULL)
                {
                    run->quantum = point->nextNode->quantum; /*Set the time slice to be the time slice of the next ready queue*/
                    InsertLast(run, point->nextNode);    /*Inserts the process into the next ready queue*/
                    flag = 0;
                }
                else
                {
                    RoundRobin(point); /* If the last ready queue is called, the time slice rotation algorithm */
                    break;
                }
            }
        }
        flag = 1;
        if (point->ProcessPointer == NULL) /*Ready queue pointer down */
            point = point->nextNode;
        if (point->nextNode == NULL)
        {
            RoundRobin(point);
            break;
        }
        GetFirst(point);
    }
}
