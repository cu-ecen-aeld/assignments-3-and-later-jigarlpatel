#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>


#define MAX_BUFFER_SIZE 2048

int sFD;
int fd;
char rxBuff[MAX_BUFFER_SIZE];
struct sockaddr incomingAddr;
socklen_t incomingAddrLength = 0;
static void mysignalhandler(int sigNum);
static void FailOnExit(void);
void connectSocket(void);

void mysignalhandler(int sigNum)
{
    syslog(LOG_INFO, "Received Signal %d\n",sigNum);
    printf("Received Signal %d\n",sigNum);
    shutdown(sFD,SHUT_RDWR);
    closelog();
    exit(EXIT_SUCCESS);
}

static void FailOnExit(void)
{
    close(fd);
    closelog();

    exit(EXIT_FAILURE);
}




int main(int argc, char* argv[])
{


    signal(SIGINT, mysignalhandler);
    signal(SIGTERM, mysignalhandler);
    
    connectSocket();
    if(argc > 1)
    {
        if(strcmp(argv[1],"-d"))
        {
            pid_t pid = fork();
            if(pid == -1 || pid > 0)
            {
                syslog(LOG_INFO, "Error creating demon \n");
                printf("Error creating demon \n");
                FailOnExit();
            }else{
                setsid();
                chdir("/");
                open("/dev/null", O_RDWR);
            }
        }else{
            syslog(LOG_INFO, "We only support -d as argument \n");
            printf("We only support -d as argument \n");
        }
    }

    if(listen(sFD,5) == -1)
    {
        syslog(LOG_INFO, "listen failed \n");
        printf("listen failed \n");
    }
    else{
        syslog(LOG_INFO, "listen call done \n");
        printf("listen call done \n");
    }
    
    
    while(1)
    {
        int acceptedSocket = accept(sFD,&incomingAddr,&incomingAddrLength);
        if(acceptedSocket == -1)
        {
            syslog(LOG_INFO, "accept failed \n");
            printf("accept failed \n");
        }else{

            syslog(LOG_INFO, "accept pass \n");
            printf("accept pass \n");
            fd = open("/tmp/var/aesdsocketdata",O_CREAT | O_APPEND | O_RDWR, S_IRWXU | S_IRGRP | S_IROTH);
            if(fd == -1)
            {
                syslog(LOG_INFO, "Error Creating/opening file -> %s\n", strerror(errno));
                printf("Error Creating/opening file\n");
            }

            int receivedFinish = 0;
            while(receivedFinish == 0)
            {
                int noOfBytesReceived = recv(acceptedSocket,rxBuff,MAX_BUFFER_SIZE,0);
                if(noOfBytesReceived == -1)
                {
                    syslog(LOG_INFO, "Error in recv() -> %s\n", strerror(errno));
                    printf("Error in recv() -> %s\n", strerror(errno));
                    FailOnExit();
                }

                if(write(fd,rxBuff,noOfBytesReceived) == -1)
                {
                    syslog(LOG_INFO, "Error while write in file -> %s\n", strerror(errno));
                    printf("Error while write in file -> %s\n", strerror(errno));
                    FailOnExit();
                }

                if(strchr(rxBuff,'\n') != NULL)
                {
                    receivedFinish = 1;
                    syslog(LOG_INFO, "received Finish\n");
                    printf("received Finish \n");
                }
            }

            //Now send data to Client
            int sendFinish = 0;
            lseek(fd,0,SEEK_SET);
            printf("******** Now Sending data to Client ******* \n");
            while(sendFinish == 0)
            {
                int numberOfByteRead = read(fd,rxBuff,MAX_BUFFER_SIZE);
                if(numberOfByteRead == -1)
                {
                    syslog(LOG_INFO, "error while read file -> %s\n", strerror(errno));
                    printf("error while read file -> %s\n", strerror(errno));
                    FailOnExit();
                }

                if(numberOfByteRead == 0)
                {
                    syslog(LOG_INFO, "Complete Reading file  send finish\n");
                    printf("Complete Reading file send finish\n");
                    sendFinish = 1;
                }else{
                    send(acceptedSocket,rxBuff,numberOfByteRead,0);
                }
            }

            close(fd);
        }

    }
    
    

}

void connectSocket(void)
{
    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    //get Address info
    struct addrinfo* results;
    if(getaddrinfo(NULL,"9000",&hints,&results) != 0)
    {
        syslog(LOG_INFO, "Error from getaddrinfo\n");
        printf("Error from getaddrinfo\n");
        FailOnExit();
    }

    //Now create socket
    sFD = socket(results->ai_family,results->ai_socktype,results->ai_protocol);
    if(sFD == -1)
    {
        syslog(LOG_ERR, "Error from socket call\n");
        printf("Error from socket call\n");
        FailOnExit();
    }else
    {
        syslog(LOG_INFO, "socket call scussfull\n");
        printf("socket call scussfull\n");
    }

    int val = 1;
    setsockopt(sFD,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

    printf("Now Bind Socket \n");
    syslog(LOG_INFO, "Now Bind Socket \n");

    if(bind(sFD,results->ai_addr,results->ai_addrlen) == -1)
    {
        syslog(LOG_ERR, "Error from bind call\n");
        printf("Error from bind call\n");
        FailOnExit();
    }else{
        syslog(LOG_INFO, "scuss from bind call\n");
        printf("scuss from bind call\n");
    }

    freeaddrinfo(results);

}