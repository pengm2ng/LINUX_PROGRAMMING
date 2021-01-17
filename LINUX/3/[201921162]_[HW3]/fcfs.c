#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process // 구조체 정의
{
    int burstTime;
    int arrivalTime;
    int exitTime; //끝나는 시간
    int pid;      //프로세스 아이디

} Process;

Process *readyQueue[100] = {0};  //레디큐 선언 초기화
Process *finishQueue[100] = {0}; //끝난프로세스들을 담는 배열 선언
Process *pc[100] = {0};          //input 의 정보들을 담는 배열 선언 -> 다음 세개의 배열이 참조됨

//큐 선언
int queueFront = 0;
int queueRear = 0;

void pushToReadyQueue(Process *process)
{
    readyQueue[queueRear++] = process;
}

Process *popFromReadyQueue()
{

    return readyQueue[queueFront++];
}

int isReadyQueueEmpty()
{
    return queueFront == queueRear ? 1 : 0;
}

void readyQueueInit()
{
    int queueFront = 0;
    int queueRear = 0;
}

int main()
{
    printf("----------FCFS 스케줄링 알고리즘----------\n\n");
    int count = 0; //input에 주어진 프로세스의 개수

    FILE *f;

    f = fopen("input.txt", "r"); // 파일 읽기

    int arrivalTime;
    int burstTime;

    while (fscanf(f, "%d %d", &burstTime, &arrivalTime) != -1) //input scanf해서 다음에 넣어주기
    {
        readyQueue[count] = malloc(sizeof(Process)); //배열들 동적 할당
        finishQueue[count] = malloc(sizeof(Process));
        pc[count] = malloc(sizeof(Process));
        pc[count]->burstTime = burstTime; //구조체를 이용하여 bursttime,arrivaltiem,pid 저장
        pc[count]->pid = count;
        pc[count++]->arrivalTime = arrivalTime;
    }

    fclose(f); //파일입출력 종료

    int time = 0;
    int finishPC = 0;          //종료된 프로세스의 개수
    int readyPC = 0;           //레디큐에 존재하는 프로세스의 개수
    Process *executePC = NULL; //실행중인 프로세스를 담는 구조체

    for (time = 0; time < 1000; time++) //최대의 cpu 동작시간이 1000ms 라고 가정
    {

        if (isReadyQueueEmpty() && finishPC == count) //cpu 종료 조건 : 레디큐에 아무것도 없거나, 존재하는 프로세스 개수 == 종료 프로세스 개수 일때
        {
            printf("cpu 동작 종료\n\n");
            break;
        }

        /*pc 배열에서 레디큐에 push 하기*/

        if (pc[0] != NULL) //만약 실행할 프로세스가 아직 남아있을 때
        {
            if (time == pc[0]->arrivalTime) //그 프로세스의 도착시간이 현재시간과 일치할 때
            {

                //input.txt의 프로세스들의 정보가 arrivalTime이 현재 시간일 경우 레디큐에 push
                if (readyPC == 0)           //만약 레디큐에 존재하는 프로세스개수가 0일 때 그냥 레디큐[0]에 pc[0] 프로세스를 담는다.
                {
                    readyQueue[0] = pc[0];
                }
                else                        //레디큐에 존재하는 프로세스가 있을 때, 레디큐의 가능한 마지막에 프레세스를 넣는다.
                {
                    readyQueue[readyPC] = pc[0];
                }
                readyPC++;      //레디큐에 프로세스가 증가하므로 +1

                for (int i = 0; i < count; i++)
                {
                    if (pc[i + 1] == NULL || i == count - 1)    //만약 pc[i+1]이 비어있거나 i 가 마지막 프로세스라면 pc[i]는 비운다.
                    {

                        pc[i] = NULL;
                    }
                    else //그렇지 않다면 pc의 값을 왼쪽으로 쉬프팅 해준다.
                    {
                        pc[i] = pc[i + 1];
                    }
                }
            }
        }

        if (executePC != NULL) //만약 실행중인 프로세스가 있을 경우
        {
            //만료 시간과 현재시간이 일치할 경우 cpu에서 프로세스 해제

            if (executePC->exitTime == time)
            {
                printf("time : %d\n프로세스%d 종료\n\n", time, executePC->pid);
                finishQueue[finishPC] = executePC;
                executePC = NULL;
                finishPC++;
            }
        }

        //만약 실행중인 프로세스가 없을 때,
        if (executePC == NULL)
        {
            //레디큐에 뭐가 있을 경우
            if (readyPC != 0)
            {

                if (readyPC > 1) //레디큐에 존재하는 프로세스가 한개 이상일  경우
                {

                    executePC = readyQueue[0]; //프로세스를 실행시킨다.

                    for (int i = 0; i < readyPC; i++) //레디큐의 항목들을 쉬프팅해준다.
                    {
                        if (i == readyPC - 1)
                        {
                            continue;
                        }
                        else
                        {
                            readyQueue[i] = readyQueue[i + 1];
                        }
                    }
                    readyPC--;        //레디큐 개수 -1
                    executePC->exitTime = executePC->burstTime + time; //프로세스의 종료시간 = bursttime + 현재시간
                    printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                }

                else if (readyPC == 1) //만약 레디큐에 존재하는 프로세스가 한 개일 경우
                {                      //레디큐에 하나 있을경우 그냥 cpu 에 할당
                    executePC = readyQueue[0];
                    readyQueue[0] == NULL;
                    readyPC--;
                    executePC->exitTime = executePC->burstTime + time;
                    printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                }
            }

            else
            { //레디큐에 뭐가 없을 경우 그냥 냅둠
            }
        }
    }

    int sum = 0;

    for (int i = 0; i < count; i++) //각 프로세스의 종료시간 - 실행시간- 도착시간으로 대기시간을 구함.
    {
        sum = sum + (finishQueue[i]->exitTime - (finishQueue[i]->burstTime) - (finishQueue[i]->arrivalTime));

        // printf("%d\n", finishQueue[i]->exitTime);
    }

    printf("-----FCFS의 평균 대기 시간 : %f-----\n\n", (float)(sum) / (float)(count));

    free(*pc);
    free(*finishQueue);
    free(*readyQueue);
}