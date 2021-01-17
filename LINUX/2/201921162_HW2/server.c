#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>



void error_handling(char *message);

int people = 0;//설문에 참여한 클라이언트 수 변수
int main(void)
{
    
    int serv_sock; // 서버 소켓
    int clnt_sock; // 클라이언트 소켓
    pid_t pid;//프로세스 ID
    struct sockaddr_in serv_addr;//소켓 주소
    struct sockaddr_in clnt_addr;//클라이언트 주소

    socklen_t clnt_addr_size;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);//소켓 생성
    if (serv_sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr)); // 소켓 초기화
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(1983);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)//bind() 실행 -1이면 에러핸들링
    {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 10) == -1)//listen() 실행 10개까지 클라이언트 수용가능 -1 이면 에러핸들링
    {
        error_handling("listen() error");
    }

    printf("연결을 기다리는 중...\n\n");

    clnt_addr_size = sizeof(clnt_addr);

    char buffer[200] = {NULL};
   
    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);//클라이언트 소켓을 accept 한값을 저장


        if (clnt_sock == -1)//그값이 -1 일경우 에러핸들링
        {
            error_handling("accept() error");
        }
        else
        {
            printf("소켓 연결 성공!\n");
        }

        pid = fork();//다중프로세스 구현,자식 프로세스 생성 pid 값에 저장
            people = people + 1;// 설문 조사 참여 인원 증가

        if (pid == 0)//만약 자식 프로세스 이면 다음을 실행한다.
        {
            sprintf(buffer, "설문 참여한 인원:%d\n", people);//people의 값을 가진 다음의 문장을 buffer에 저장
            write(clnt_sock, buffer, sizeof(buffer));
            
            read(clnt_sock, buffer, sizeof(buffer));
            printf("클라이언트 메시지: %s\n", buffer);


            write(clnt_sock, "감사합니다.\n", sizeof("감사합니다.\n"));
           // memset(buffer, 0x00, sizeof(buffer));
        
            break;
        }
        else if (pid != 0)//부모 프로세스이면 continue
        {
            continue;
        }
    }

    close(clnt_sock);//소켓 종료
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}