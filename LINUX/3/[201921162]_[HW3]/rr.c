#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process //구조체 선언
{
    int burstTime;
    int arrivalTime;
    int exitTime;
    int pid;
    int timer;          //시간 할당량을 체크할 타이머
    int serviceTime;    // 현재까지의 실행시간을 체크해주는 타이머

} Process;

Process *readyQueue[100] = {0};     //레디큐
Process *finishQueue[100] = {0};    // 끝난 프로세스를 담는 배열
Process *pc[100] = {0};             //input에서 읽은 정보 담는 배열 -> 세개의 배열 은 참조

//큐선언
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

    printf("----------RR 스케줄링 알고리즘----------\n\n");

    int count = 0; //input에 주어진 프로세스의 개수

    FILE *f;

    f = fopen("input.txt", "r"); // 파일 읽기

    int arrivalTime;
    int burstTime;

    while (fscanf(f, "%d %d", &burstTime, &arrivalTime) != -1)  //input의 정보를 읽어서 pc에 저장
    {
        pc[count] = malloc(sizeof(Process));    //세개의 배열 동적 할당
        readyQueue[count] = malloc(sizeof(Process));
        finishQueue[count] = malloc(sizeof(Process));
        pc[count]->burstTime = burstTime;
        pc[count]->pid = count;
        pc[count]->timer = 0;
        pc[count]->exitTime = 0;
        pc[count]->serviceTime = 0;
        pc[count++]->arrivalTime = arrivalTime;
    }

    fclose(f); //파일입출력 종료

    
    int time = 0;   //현재의 시간
    int finishPC = 0;   //끝난 프로세스의 개수


    Process *executePC = NULL;  //현재 실행중인 프로세스


    for (time = 0; time < 1000; time++) //cpu 할당 시간이 1000ms 라고 가정
    {

        if (isReadyQueueEmpty() && finishPC == count) //cpu 종료 조건 : 레디큐에 아무것도 없거나, 존재하는 프로세스 개수 == 종료 프로세스 개수 일때
        {
            printf("cpu 동작 종료\n\n");
            break;
        }

        /*pc 배열에서 레디큐에 push 하기*/

        if (pc[0] != NULL)//만약 실행할 프로세스가 아직 남아있을 때
        {
            if (time == pc[0]->arrivalTime)
            {

                //input.txt의 프로세스들의 정보가 arrivalTime이 현재 시간일 경우 레디큐에 push
                pushToReadyQueue(pc[0]);    //레디큐로 push

                for (int i = 0; i < count; i++) //왼쪽으로 쉬프팅
                {
                    if (i == count - 1)
                    {

                        pc[i] = NULL;
                    }
                    else
                    {
                        pc[i] = pc[i + 1];
                    }
                }
            }
        }

        if (executePC == NULL) //실행중인 cpu가 없는경우
        {
            if (!isReadyQueueEmpty()) //만약 레디큐가 비어있지 않은 경우
            {
                executePC = popFromReadyQueue();    //프로세스 실행 
                printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);

                executePC->timer += 1;  //실행중인 프로세스 타이머 +1
                executePC->serviceTime += 1; //실행 프로세스 현재까지의 실행시간 +1
            }
            else    //만약 레디큐가 비어있으면 continue;
            {
               
                continue;
            }
        }
        else //실행중인 cpu가 있는경우
        {
            if (executePC->timer == 4)  //만약 실행 프로세스가 시간할당량을 채웠을 경우
            {
                if (executePC->burstTime - executePC->serviceTime == 0) //실행시간-현재까지의 실행시간 =0, 실행을 완료할 경우
                {
                    printf("time : %d\n프로세스%d 종료\n\n", time, executePC->pid);
                    executePC->timer = 0;   //타이머 초기화
                    executePC->exitTime = time; //프로세스 종료시간 지정
                    finishQueue[finishPC] = executePC; //끝난 프로세스로 저장

                    executePC = NULL;   //실행 프로세스 없음
                    finishPC++;         //끝난 프로세스 개수 +1
                    if (!isReadyQueueEmpty()) 
                    {                                    //레디큐 있으면 실행
                        executePC = popFromReadyQueue(); //기다리는 프로세스 실행
                        executePC->timer += 1;           //타이머 +1
                        printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                        executePC->serviceTime += 1;     //현재까지의 실행시간 +1
                    }
                }
                else  //시간 할당량을 채웠는데, 실행시간이 남은 경우
                {
                    printf("time : %d\n프로세스%d 중단\n\n", time, executePC->pid);
                    executePC->timer = 0;   //타이머 초기화
                    pushToReadyQueue(executePC);    //다시 레디큐로 PUSH
                    if (!isReadyQueueEmpty())   
                    {                                    //레디큐 있으면 실행
                        executePC = popFromReadyQueue(); //기다리는 프로세스 실행
                        executePC->timer += 1;
                        printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                        executePC->serviceTime += 1;
                    }
                }
            }
            else    //만약 시간 할당량을 채우지 못했을 경우
            {

                if (executePC->burstTime - executePC->serviceTime == 0) //실행시간을 채운 경우
                {
                    printf("time : %d\n프로세스%d 종료\n\n", time, executePC->pid);

                    executePC->timer = 0;   //타이머 초기화
                    executePC->exitTime = time; //종료시간 지정
                    finishQueue[finishPC] = executePC;  //종료 프로세스로 저장

                    executePC = NULL;   //실행중인 프로세스 없음
                    finishPC++; //종료 프로세스 개수 +1

                    if (!isReadyQueueEmpty())
                    {                                    //레디큐 있으면 실행
                        executePC = popFromReadyQueue(); //기다리는 프로세스 실행
                        executePC->timer += 1;
                        printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                        executePC->serviceTime += 1;
                    }
                }
                else //시간 할당량 못채우고 실행시간도 못채운경우 실행중,,,
                {
                    executePC->timer += 1;
                    executePC->serviceTime += 1;
                }
            }
        }
    }

    int sum = 0;

    for (int i = 0; i < count; i++) //종료시간-실행시간-도착시간 = 대기시간
    {
        sum = sum + (finishQueue[i]->exitTime - (finishQueue[i]->burstTime) - (finishQueue[i]->arrivalTime));

        // printf("%d\n", finishQueue[i]->exitTime);
    }

    printf("-----RR의 평균 대기 시간 : %f-----\n\n", ((double)sum / (double)count));

    free(*pc);
    free(*finishQueue);
    free(*readyQueue);
}
