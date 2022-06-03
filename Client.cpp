#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<thread>
using namespace std;



// A function for displaying anerror message
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// A function for sending a message to the server
void sendMsg(int server)
{
    while (1){
        string msg;
        getline(cin,msg);
        write(server,msg.c_str(),strlen(msg.c_str()));
    }
}

// A function for reciving a message from the server and printing it to the console
void reciveMsg(int server, string client)
{
    while(1){
        char msg[1024] {0};
        read(server,msg,sizeof(msg)-1);
        cout << msg << endl;
    }
    
}


// The main function
int main(int argc, char *argv[])
{
    int sockfd, portno, n,name;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    

    char buffer[1024]{0};

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);

    if (server == NULL){
        fprintf(stderr, "Errror, no such host\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    string userName;
    cout << "Please Enter your userName: ";
    getline(cin,userName);
    write(sockfd,userName.c_str(),strlen(userName.c_str()));


    thread recvMsg(reciveMsg,sockfd,userName);
    recvMsg.detach();
    sendMsg(sockfd);

    close(sockfd);
    cout << "Connection is Closed!" << endl;

    return 0;
}

