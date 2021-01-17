#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process //구조체 선언
{
    int burstTime;
    int arrivalTime;
    int exitTime;
    int pid;
    float timeRatio;    //우선순위를 정하는 비율을 저장하는 변수

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
    printf("----------HRN 스케줄링 알고리즘----------\n\n");
    int count = 0; //input에 주어진 프로세스의 개수

    FILE *f;

    f = fopen("input.txt", "r"); // 파일 읽기

    int arrivalTime;
    int burstTime;

    while (fscanf(f, "%d %d", &burstTime, &arrivalTime) != -1)  //input의 정보를 읽어서 pc에 저장
    {
        readyQueue[count] = malloc(sizeof(Process));     //세개의 배열 동적 할당
        finishQueue[count] = malloc(sizeof(Process));
        pc[count] = malloc(sizeof(Process));
        pc[count]->burstTime = burstTime;
        pc[count]->pid = count;
        pc[count]->exitTime = 0;
        pc[count]->timeRatio = 0;
        pc[count++]->arrivalTime = arrivalTime;
    }

    fclose(f); //파일입출력 종료

  

    
    int time = 0;
    int finishPC = 0;
    int readyPC = 0;
    /* running state에 있는 process */
    Process *executePC = NULL;//현재 실행중인 프로세스
    Process *temp = NULL;



    for (time = 0; time < 100; time++)//cpu 할당 시간이 1000ms 라고 가정
    {

        if (isReadyQueueEmpty() && finishPC == count) //cpu 종료 조건 : 레디큐에 아무것도 없거나, 존재하는 프로세스 개수 == 종료 프로세스 개수 일때
        {
            printf("cpu 동작 종료\n\n");
            break;
        }

    /*pc 배열에서 레디큐에 push 하기*/
        if (pc[0] != NULL)
        {
            if (time == pc[0]->arrivalTime)//만약 실행할 프로세스가 아직 남아있을 때
            {

                //input.txt의 프로세스들의 정보가 arrivalTime이 현재 시간일 경우 레디큐에 push
                if (readyPC == 0)
                {
                    readyQueue[0] = pc[0];
                }
                else
                {
                    readyQueue[readyPC] = pc[0];
                }
                readyPC++;

                for (int i = 0; i < count; i++)//왼쪽으로 쉬프팅
                {
                    if (pc[i + 1] == NULL || i == count - 1)
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
       
        //만약 실행중인 프로세스가 있을 경우

        if (executePC != NULL)
        {
            //만료 시간과 현재시간이 일치할 경우 cpu에서 프로세스 해제

            if (executePC->exitTime == time)
            {
                printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
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

                if (readyPC > 1)
                { // 레디큐에 두개이상 있을경우 우선순위를 비교해서 우선순위가 높은 순서대로 해서 제일 처음의 것을 뽑아낸다.

                    for (int i = 0; i < readyPC; i++) // 비율 구하기  // hrn 우선 순위 = (대기시간 + 서비스 시간)/서비스 시간
                    {

                        readyQueue[i]->timeRatio = (float)(time - readyQueue[i]->arrivalTime + readyQueue[i]->burstTime) / (float)(readyQueue[i]->burstTime);
                    }

                    for (int i = 0; i < readyPC; i++) //비율 높은 순으로 정렬
                    {
                        for (int j = 0; j < readyPC - 1; j++)
                        {

                            if (readyQueue[j]->timeRatio < readyQueue[j + 1]->timeRatio)
                            {
                                temp = readyQueue[j];
                                readyQueue[j] = readyQueue[j + 1];
                                readyQueue[j + 1] = temp;
                            }
                        }
                    }

                    executePC = readyQueue[0]; //제일 높은 값을 가진, 제일앞에 위치하는 프로세스 실행

                    for (int i = 0; i < readyPC; i++)   //왼쪽으로 쉬프팅
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
                    readyPC--;  //레디큐에 존재하는 프로세스-1
                    executePC->exitTime = executePC->burstTime + time;  //종료시간 지정
                    printf("time : %d\n프로세스%d 할당\n\n", time, executePC->pid);
                }

                else if (readyPC == 1)  
                { //레디큐에 하나 있을경우 그냥 cpu 에 할당
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

    for (int i = 0; i < count; i++) //종료시간-실행시간-도착시간 = 대기시간
    {
        sum = sum + (finishQueue[i]->exitTime - (finishQueue[i]->burstTime) - (finishQueue[i]->arrivalTime));

        // printf("%d\n", finishQueue[i]->exitTime);
    }

    printf("-----HRN의 평균 대기 시간 : %f-----\n\n", (float)(sum) / (float)(count));

    free(*pc);
    free(*finishQueue);
    free(*readyQueue);
}
