//
// Created by rasmus on 10/21/17.
//


#include <ifaddrs.h>
#include "Server.h"

Server::Server(int maxNrOfPlayers) {
    maxPlayers = maxNrOfPlayers;
    clients =  new ClientSock[maxPlayers];
    recvThreads = new thread[maxPlayers];

    for(int i = 0 ; i < maxPlayers ; i++){
        clients[i].client = -1;
    }

    //setting up server

    struct sockaddr_in server_addr;
    socklen_t size;

    serverSock = socket(AF_INET, SOCK_STREAM,0);

    if (serverSock < 0) {
        cout << "Error establishing socket ..." << endl;
        exit(-1);
    }

    cout << "- Socket server has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);


    if ((bind(serverSock, (struct sockaddr*) &server_addr, sizeof(server_addr)))
        < 0) {
        cout
                << "- Error binding connection, the socket has already been established..."
                << endl;
        exit(-1);
    }


    size = sizeof(server_addr);
    cout << "- Looking for clients..." << endl;

    listen(serverSock, 1);


}

void Server::Listner(ClientSock aClient) {
    int ReceivedBytes;
    cout << "listning for client " << aClient.client<< endl;
    while(started){
        if(aClient.client != -1){
            char buffer[1024];
            ReceivedBytes = recv(aClient.client , buffer , 1024 ,0);
            if(ReceivedBytes != 0){

                memcpy(aClient.recvMessage,buffer,1024);
                //cout << " i received data from client : " << aClient.client << " : " << aClient.recvMessage << endl ;
                *aClient.msgSent = false;
            }
        }
    }

    cout << " i lost connection to" << aClient.client << endl;

}

void Server::AcceptClients() {
    while(started){
        int emptySpot = -1;
        for(int i =  0 ; i < maxPlayers ; i++){
            if(clients[i].client == -1){
                emptySpot = i;
                break;
            }
        }

        if(emptySpot == -1){
            continue;
        }
        cout << "waiting for clients to connect " << endl;
        clients[emptySpot].client = accept(serverSock,(struct sockaddr *)& clients[emptySpot],& clients[emptySpot].theriSize);
        ClientSock * test = &clients[emptySpot];
        recvThreads[emptySpot] = thread([this, test] {this->Listner(*test); } );
        cout << "client connected i out them in" << emptySpot<<  endl;
    }
}

void Server::Talker() {
    bool test = false;
    while(started) {
        for (int i = 0; i < maxPlayers; i++) {
            if (clients[i].client == -1) { continue; }


            if(*clients[i].msgSent == false) {

                //send to all connected clients
                for(int j = 0 ; j < maxPlayers ; j++) {

                    if(clients[j].client == -1){ continue; }
                    cout << "i am sending the message " << clients[i].recvMessage << "to client " << i << " to client nr " << j<< endl;
                    clients[i].recvMessage[0] = 'i';
                    send(clients[j].client, clients[i].recvMessage, 1024, 0);
                }

                *clients[i].msgSent = true;
            }
        }
    }
}

void Server::stop() {
    started == false;
}

bool Server::isStarted() {
    return started;
}

int Server::start() {



    started = true;
    accThread = thread([this] { this->AcceptClients(); });
    sendThread = thread([this] {this->Talker();});

    accThread.join();
    sendThread.join();
}



