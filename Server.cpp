#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>

#define BUFSIZE 1024

void ErrorHandling(char *message);

int main(int argc, char **argv) {
    WSADATA wsaData;
    SOCKET servSock, clientSock;
    SOCKADDR_IN servAddr, clientAddr;

    char message[BUFSIZE];
    int strLen;
    int fromLen, nRcv;

    if (argc != 2) {
        printf("Please, Insert Port Number\n");
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("Load winSock 2.2 DLL Error");
    } //Winsock 라이브러리 초기화하는 부분 유지

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET) {
        ErrorHandling("Socket Error");
    }

    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(servSock, (SOCKADDR *)(&servAddr), sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("Bind Error");
    }
    if (listen(servSock, 2) == SOCKET_ERROR) {
        ErrorHandling("Listen Error");
    }

    fromLen = sizeof(clientAddr);

    clientSock = accept(servSock, (SOCKADDR *)&clientAddr, &fromLen);
    if (clientSock == INVALID_SOCKET) {
        ErrorHandling("Accept Error");
    } else {
        printf("%s Connection Coplete!\n", inet_ntoa(clientAddr.sin_addr));
        printf("Start ...\n");
    }

    closesocket(servSock);

    while (1) {
        printf("Message Receives ...\n");
        nRcv = recv(clientSock, message, BUFSIZE - 1, 0);

        if (nRcv == 0) {
            printf("Receive Error ... \n");
            break; // 클라이언트가 연결을 종료함
        } else if (nRcv == SOCKET_ERROR) {
            ErrorHandling("Recv Error");
        }

        message[nRcv] = '\0';

        if(strcmp(message, "exit") == 0) {
            printf("Close Client Connection.. \n");
            break;
        }

        printf("Received message: %s\n", message);
        printf("\nSend Message : ");
        gets(message);

        if(strcmp(message, "exit") == 0) {
           send(clientSock, message, (int)strlen(message),0);
            break;
        }
        // 클라이언트에게 메시지를 다시 보내는 예제
        send(clientSock, message, strlen(message), 0);
    }

    closesocket(clientSock);
    WSACleanup();

    printf("Close Connection..\n");
    _getch();

    return 0;
}

void ErrorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    _getch();
    exit(1);
}
