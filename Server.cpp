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


    //for setting up server
    struct addrinfo serverHints;
    struct addrinfo *serverInfo;
    struct addrinfo *p;


    memset(&serverHints, 0, sizeof(serverHints)); // get hints about the server

    serverHints.ai_family = AF_UNSPEC;
    serverHints.ai_socktype = SOCK_STREAM;
    serverHints.ai_flags = AI_PASSIVE;



    if (getaddrinfo(NULL, port, &serverHints, &serverInfo) != 0) {
        cout << "could not get addres" << endl;
    }

    for (p = serverInfo; p != NULL; p = p->ai_next) {
        if (serverSock = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1) {
            cout << "one addr failed ";
            continue;
        }
        cout << "One addr worked : ";
        if (bind(serverSock, p->ai_addr, p->ai_addrlen) == -1) {
            close(serverSock);
            cout << "but did not bind " << endl;
            continue;
        }
        cout << "and it did bind " << endl;

        if(p->ai_family = AF_INET){
            cout << "using ip v 4 " << endl;
        }else if( p->ai_family == AF_INET6){
            cout << "using ip v6 " << endl;
        }else{
            cout << " dont know what it is using " << endl;
        }

        if(p->ai_family == AF_INET) {
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &p->ai_addr, str, INET_ADDRSTRLEN);
            cout <<"IP v4 : " <<str << endl;
        }else {
            char str2[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &p->ai_addr, str2, INET6_ADDRSTRLEN);
            cout <<"IP v6 :" <<str2 << endl;
        }

        break;
    }

    freeaddrinfo(serverInfo); // all done with this structure


    if (listen(serverSock, 10) == -1) {
        cout << "listen() fucked up somehow " << endl;
    }
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



