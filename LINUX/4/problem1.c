#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX 999999999   //sentence 배열의 크기
#define ALPHABET 26

#define filename "./input.txt"  //불러올 파일 이름

pthread_mutex_t mutex[ALPHABET];    //뮤텍스 배열 선언

typedef struct thread   //각 스레드가 읽어들일 텍스트의 범위를 저장하는 구조체
{
    int start, end;

} thread;

int result[30] = {}; //알파벳의 개수를 저장 시키는 배열
char sentence[MAX]; //텍스트를 담는 배열
int length = 0; //문장의 길이

void *count(void *args)
{ //알파벳의 개수를 세는 함수 스레드가 차례대로 들어와서 임무를 수행

    thread arg = *(thread *)args;

    for (int c = arg.start; c < arg.end; c++)   //삼등분 된 범위만큼 각 스레드가 작업 수행
    {

        if (c == 0) //만약 c가 0 즉 맨 첫 글자라면
        {
            if (sentence[c] != ' ') //맨 첫 글자가 공백이 아니라면
            {
                //임계구역 뮤텍스락 지정 다른 스레드 접근 불가
                pthread_mutex_lock(&mutex[sentence[c] - 'a']);
                result[sentence[c] - 'a']++;
                pthread_mutex_unlock(&mutex[sentence[c] - 'a']);
            }
            else if (sentence[c] == ' ')   
            {
                continue;
            }
        }

        else    //나머지 경우에는 c-1 번째의 값이 공백이고 현재 값이 소문자라면 작업 수행
        {
            if (sentence[c - 1] == ' ' && sentence[c] != ' ')
            {

                pthread_mutex_lock(&mutex[sentence[c] - 'a']);
                result[sentence[c] - 'a']++;
                pthread_mutex_unlock(&mutex[sentence[c] - 'a']);
            }
        }
    }
}

int main()
{

    pthread_t thread1, thread2, thread3;
    //파일 입출력을 통해 파일을 읽기
    FILE *fp;
    fp = fopen(filename, "r");
    while (!feof(fp))   //파일의 끝이 될때까지 읽기
    {
        fgets(sentence, MAX, fp);
    }
    length = strlen(sentence);  //텍스트의 길이
   
    int thread1_size = length / 3;  //세개의 스레드 병렬 처리 위해 삼등분
    int thread2_size = thread1_size * 2;
    int thread3_size = length;

    fseek(fp, thread1_size, SEEK_SET);  // 처음부터 스레드 1의 범위 끝까지 지정
    while (fgetc(fp) != ' ') //읽어들인다 공백이나올 경우 끝
    {
    }
    thread1_size = ftell(fp);   //스레드 1 사이즈 재지정

    fseek(fp, thread2_size, SEEK_SET);
    while (fgetc(fp) != ' ')
    {
    }
    thread2_size = ftell(fp);

    fclose(fp);

    for (int i = 0; i < 26; i++)    //뮤텍스락 초기화
    {
        pthread_mutex_init(&mutex[i], NULL);
    }

    thread thread1_com = {0, thread1_size};     //구조체 선언, 스레드 생성
    pthread_create(&thread1, NULL, count, &thread1_com);

    thread thread2_com = {thread1_size, thread2_size};
    pthread_create(&thread2, NULL, count, &thread2_com);

    thread thread3_com = {thread2_size, thread3_size};
    pthread_create(&thread3, NULL, count, &thread3_com);

    pthread_join(thread1, NULL);    //join 스레드 작업 완료 후 반환
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    for (int i = 0; i < 26; i++)    //뮤텍스 파괴
    {
        pthread_mutex_destroy(&mutex[i]);
    }

    printf("\n***결과***\n");
    for (int i = 0; i < ALPHABET; i++)
    {
        printf("%c의 개수 : %d\n", ('a' + i), result[i]);
    }
}