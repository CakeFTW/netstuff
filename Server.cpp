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
    //serverHints.ai_flags = AI_PASSIVE;
    serverHints.ai_addr = (sockaddr *) "172.20.10.6";

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

        void *addr;
        char *ipver;
        char ipstr[INET6_ADDRSTRLEN];
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        struct ifaddrs * ifAddrStruct=NULL;
        struct ifaddrs * ifa=NULL;
        void * tmpAddrPtr=NULL;

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr) {
                continue;
            }
            if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
                // is a valid IP6 Address
                tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            }
        }
        if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);


        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);


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



