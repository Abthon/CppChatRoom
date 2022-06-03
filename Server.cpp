#include<thread> //  Header file for Threading 
#include<cstring> // Header file for accessing c_string functions
#include<iostream> // Header file for input/out operation
#include<unistd.h> // Header file to write a message to the client socket ( read, write )
#include<sys/socket.h> // Header file for creating a socket object
#include<netinet/in.h> // Header file to acess INADDR_IN
#include<chrono> 
#include<vector>
using namespace std;

int activeConnections = 0;
void error(const char* msg); 
void activeConnectionCounter();
void handle_connection(int conn, char name[1024]);

int main(int argc, char *argv[]){


    int server,client,portno,backlog;
    struct sockaddr_in serv_addr, cli_addr;
    char userName[1024] {0};


    // Creating Socket object
    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0)
    {
        error("Error occured while creating the socket object! ");
    }

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    // Binding socket
    if (bind(server, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR occured while binding the socket! ");
        exit(1);
    }

    cout << endl;
    cout << "[ STARTING ] Server is starting......" << endl << endl;

    // Listining for incoming connections
    listen(server,backlog);
    cout << "Listning for incoming connections..." << endl;

    while(1){

        //Accepting socket
        client = accept(server,(struct sockaddr *)NULL,NULL);
        if(client < 0){
            error("Error occured while accepting a new socket! ");
        }


        read(client,userName,sizeof(userName)-1);
        cout << "New connection is Established! " << endl;
        cout << userName << " Joined the chat Room! " << endl;
        thread workerThread(handle_connection,client,userName);
        workerThread.detach();

    }

    //Closing conneciton
    close(client);
    cout << "Connection Closed!";



    return 0;
}


void handle_connection(int conn,char name[1024]){
    cout << "User Id " << conn << endl;
    activeConnectionCounter();
    char userName[1024];
    string messageToSent;
    strcpy(userName,name);
    bool connected = true; 
    cout << "Active Users increased from " << activeConnections -1 << " to " << activeConnections <<endl;
    while (connected){
        cout << "GEba"<< endl;
        char buffer[1024] {0};
        read(conn,buffer,sizeof(buffer)-1);

        if(strcmp(buffer,"Disconnect") == 0){
            connected = false;
        }

        messageToSent.append(userName);
        messageToSent.append(": ");
        messageToSent.append(buffer);
        // write(conn,messageToSent.c_str(),messageToSent.length());
        // write(conn,)
        cout << userName << ": " << buffer << endl;
    }

    close(conn);
}  

void activeConnectionCounter(){
    activeConnections +=1; 
}

void error(const char* msg)
{
    cout << msg <<endl;
    exit(1);
}
