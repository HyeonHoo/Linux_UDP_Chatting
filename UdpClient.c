
#include < unistd.h >
#include < stdio.h >
#include < stdlib.h >
#include < string.h >
#include < strings.h >
#include < arpa / inet.h >
#include < sys / types.h >
#include < sys / socket.h >
#include < sys / poll.h >

int main(int argc, char *argv[])
{
    int nSockFd;
    struct pollfd rfds[2]; // Standard Input, Socket
    int nRetval;

    char p_Buffer[BUFSIZ];
    int nBufferLen;

    struct sockaddr_in stSAddr;
    int nSAddr_size;

    if (argc == 3)
    {
        printf("Usage: %s <IP Address> <port>\n", argv[0]);
        return -1;
    }

    // UDP Socket Creation
    nSockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Keyboard Input
    rfds[0].fd = 0;
    rfds[0].events = POLLIN;
    rfds[0].revents = 0;

    // Socket Input
    rfds[1].fd = nSockFd;
    rfds[1].events = POLLIN;
    rfds[1].revents = 0;

    bzero(&stSAddr, sizeof(stSAddr));
    stSAddr.sin_family = AF_INET;
    stSAddr.sin_addr.s_addr = inet_addr(argv[1]);
    stSAddr.sin_port = htons(atoi(argv[2]));
    nSAddr_size = sizeof(stSAddr);

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
            bzero(p_Buffer, BUFSIZ);
            nBufferLen = read(0, p_Buffer, BUFSIZ);
            nBufferLen = sendto(nSockFd, p_Buffer, nBufferLen, 0,
                                (struct sockaddr *)&stSAddr, nSAddr_size);

            if (nBufferLen > 0)
            {
                printf("TX: %s", p_Buffer);
                if (strncasecmp(p_Buffer, "exit", 4) == 0)
                    break;
            }
        }
        // Socket Receiver part
        if (rfds[1].revents & POLLIN)
        {
            bzero(p_Buffer, BUFSIZ);
            nBufferLen = recvfrom(nSockFd, p_Buffer, BUFSIZ, 0,
                                  (struct sockaddr *)&stSAddr, &nSAddr_size);

            if (nBufferLen > 0)
            {
                printf("RX: %s", p_Buffer);
                if (strncasecmp(p_Buffer, "exit", 4) == 0)
                    break;
            }
        }
    } while (1);
    close(nSockFd);
    return 0;
}
