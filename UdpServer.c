
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

int main(int argc, char *argv[])
{
    int nSocketFd;
    struct pollfd rfds[2]; // Standard Input, Socket
    int nRetval;

    char pBuffer[BUFSIZ];
    int nBufferLen = 0;

    struct sockaddr_in stSAddr;
    struct sockaddr_in stCAddr;
    int nCAddr_size;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }

    // UDP Socket Creation
    nSocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Keyboard Input
    rfds[0].fd = 0;
    rfds[0].events = POLLIN;
    rfds[0].revents = 0;

    // Socket Input
    rfds[1].fd = nSocketFd;
    rfds[1].events = POLLIN;
    rfds[1].revents = 0;

    memset(&stSAddr, 0, sizeof(stSAddr));
    stSAddr.sin_family = PF_INET;
    stSAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stSAddr.sin_port = htons(atoi(argv[1]));

    if (bind(nSocketFd, (struct sockaddr *)&stSAddr,
             sizeof(stSAddr)) < 0)
    {
        printf("Binding Failed. \n");
        return -1;
    }

    do
    {
        nRetval = poll(rfds, 2, 1000);
        if (nRetval < 0)
            break;
        if (nRetval == 0)
            continue;

        // Keyboard Input Part
        if (rfds[0].revents & POLLIN)
        {
            memset(pBuffer, 0, BUFSIZ);
            nBufferLen = read(0, pBuffer, BUFSIZ);
            nBufferLen = sendto(nSocketFd, pBuffer, BUFSIZ, 0,
                                (struct sockaddr *)&stCAddr, nCAddr_size);

            if (nBufferLen > 0)
            {
                printf("TX: %s", pBuffer);
                if (strncasecmp(pBuffer, "exit", 4) == 0)
                    break;
            }
        }

        // Socket Receiver part
        if (rfds[1].revents & POLLIN)
        {
            memset(pBuffer, 0, BUFSIZ);
            nBufferLen = recvfrom(nSocketFd, pBuffer, BUFSIZ, 0,
                                  (struct sockaddr *)&stCAddr, &nCAddr_size);

            if (nBufferLen > 0)
            {
                printf("RX: %s\n", pBuffer);
                if (strncasecmp(pBuffer, "exit", 4) == 0)
                    break;
            }
        }
        while (1)
            ;
        close(nSocketFd);
        return 0;
    }