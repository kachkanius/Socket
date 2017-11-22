#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <unistd.h>
#include <iostream>

int tcp()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("binding...\n");
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    printf("binded. Start listen...\n");
    listen(listener, 1);
    printf("listen end. Open socket...\n");
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        printf("Socket opened!\n");


        switch(fork())
        {
        case -1:
            perror("fork");
            break;

        case 0:
            std::cout << "new proccess!\n";
            close(listener);
            while(1)
            {
                bytes_read = recv(sock, buf, 1024, 0);
                printf("got %d bytes data, msg: %s\n",bytes_read, buf);
                if(bytes_read <= 0){
                    printf("No data anymore !\n");
                    break;
                }
                printf("Sending back..\n");
                sleep(1);
                send(sock, buf, bytes_read, 0);
            }

            close(sock);
            _exit(0);

        default:
            close(sock);
        }
    }

    return 0;
}

int udp (){
    int sock;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    std::cout <<"opening socket\n";
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    std::cout <<"Bind...\n";

    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    std::cout <<"reading messages...\n";
    int count  = 1;
    while(1)
    {
        bytes_read = recvfrom(sock, buf, 1024, 0, NULL, NULL);
        buf[bytes_read] = '\0';
        std::cout << "Got message '" << std::string(buf) <<"'"<<std::endl;
        if (count)
        {
            count = 0;
            std::cout << "sleep...\n";
            sleep(1);
        }
    }

    return 0;
}
#include <netdb.h>

int main (int argc, char* argv[]) {

    if(argc !=2)
    {
        printf("Please select type 'tcp' or 'udp'\n");
        // return 0;
    }

    if (!strcmp((const char*)argv[1], "tcp")) {
        printf("tcp!\n");
        tcp();
    } else {
        printf("udp!\n");
        udp();
    }


    return 0;

}
