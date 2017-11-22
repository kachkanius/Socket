#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


#include <stdlib.h>

#include <unistd.h>
#include <iostream>
using namespace std;

char message[] = "Hello there!\n";
char buf[sizeof(message)];

int tcp(std::string msg)
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    while(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        //exit(2);
        cout << "wait for 2 seconds...\n";
        sleep(2);
    }
    cout <<"Connected!\n Sending ..."<<endl;
    for (int i = 0; i < 1000; ++i) {

        send(sock, msg.c_str(), msg.length(), 0);
        cout <<"recv..."<<endl;
        recv(sock, buf, sizeof(message), 0);
        cout <<"got message..." <<buf <<endl;
    }


    close(sock);

    return 0;
}

int udp(){

    int sock;
    struct sockaddr_in addr;
    cout << "creating socket" <<endl;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("Cannot create socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    std::string hello("UDP hello ");
    for (int i = 0; i < 1000; ++i) {
        std::string msgTosend = hello + std::to_string(i);
        cout << "sending '"<< msgTosend << "'"<<endl;

        int res = sendto(sock, (msgTosend).c_str(), msgTosend.length(),
                         0, (struct sockaddr *)&addr, sizeof(addr));
        if (res<0) {
            perror("failed to sendещ! ");
        }

    }
    // connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    // send(sock, msg2, sizeof(msg2), 0);
    cout << "close"<<endl;
    close(sock);

    return 0;

}

int main (int argc, char* argv[]) {

    if(argc != 3)
    {
        printf("Please select type 'tcp' or 'udp'\n");
        return 0;
    }

    if (!strcmp((const char*)argv[1], "tcp")) {
        printf("tcp!\n");
        tcp(std::string(argv[2]));
    } else {
        printf("udp!\n");
        udp();
    }



    return 0;

}
