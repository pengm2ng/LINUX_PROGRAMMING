#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);//에러 핸들링 함수 선언



int main() {
    int sock;    // 소켓, IP,스트링 길이 변수
    struct sockaddr_in serv_addr;   
    char message[100];
    int str_len;

    sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성

    if (sock == 1) {// 값이 1일 경우 에러
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));   // SOCKET 초기화
	serv_addr.sin_family = AF_INET; //IPv4 인터넷 프로토콜 사용
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 서버 주소는 자신의 주소를 사용
    serv_addr.sin_port = htons(1983);  // 포트번호 1983

    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {//CONNECT 값이 -1 일 경우 에러핸들링
        error_handling("connect() error!");
    }

    char buffer[200] = { NULL };// 버퍼 초기화
  
    char topic[]={"본인이 생각하는 운영체제 과제의 난이도를 선택해주세요\n1.매우 어렵다\n2.어렵다\n3.보통이다.\n4.쉽다\n5.매우 쉽다\n번호를 입력해주세요 : "} ;
		size_t read_len = read(sock, buffer, sizeof(buffer)); //READ() 함수실행, 사이즈 값 저장
		buffer[read_len] = 0x00;//사이즈의 마지막값은 NULL
		printf("%s\n", buffer);//버퍼 출력
        printf("%s",topic);
        
        scanf("%s", &buffer);//설문 값 입력
        write(sock, buffer, sizeof(buffer));//버퍼 WRITE()
        
        
        read_len = read(sock, buffer, sizeof(buffer));
        buffer[read_len] = 0x00;
        printf("%s", buffer);

       
    
    close(sock);//소켓 종료
    return 0;
}

void error_handling(char * message) {//에러핸들링 함수
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}