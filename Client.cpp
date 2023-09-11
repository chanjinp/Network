#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define  BUFSIZE 1024

void ErrorHandling(char *message);

int main(int argc, char **argv) {

    SOCKET clientSocket;
    WSADATA wsadata;
    SOCKADDR_IN  servAddr;

    char sAddr[15];
    int sPort;
    int nRcv;
    unsigned int Addr;
    char message[BUFSIZE];

    struct  hostent *host;

    printf("Server Address : "); gets(sAddr);
    printf("Port Number : "); gets(message);

    sPort = atoi(message);

    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0) {
        ErrorHandling("Load WinSock 2.2 DLL Error");
    }
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == INVALID_SOCKET) {
        ErrorHandling("Socket Error");
    }
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(sAddr);
    servAddr.sin_port = htons(sPort);

    if(connect(clientSocket, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("Connection Error");
    } else {
        printf("Connect OK! \n Start...\n");
    }

    while(1) {
        printf("\nSend Message : ");
        gets(message);

        if(strcmp(message, "exit") == 0) {
            send(clientSocket, message, (int)strlen(message), 0);
            break;
        }
        send(clientSocket, message, (int)strlen(message), 0);
        printf("Message Receives ...\n");

        nRcv = recv(clientSocket, message, sizeof(message) -1, 0);
        if(nRcv == SOCKET_ERROR) {
            printf("Reiceve Error .. \n");
            break;
        }

        message[nRcv] = '\0';

        if(strcmp(message, "exit") == 0) {
            printf("Close Server Connection..\n");
            break;
        }

        printf("Receive Message : %s", message);
    }

    closesocket(clientSocket);
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