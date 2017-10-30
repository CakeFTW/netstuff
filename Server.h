//
// Created by rasmus on 10/21/17.
//

#ifndef GAMESERVER_SERVER_H
#define GAMESERVER_SERVER_H

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include "CONSTANTS.h"

using namespace std;



struct  ClientSock{
    int client;
    sockaddr thierAddr;
    socklen_t theriSize;
    char * recvMessage = new char[1024];
    bool * msgSent = new bool;
};

class Server {

    //setting the server up
    int maxPlayers;
    int nrOfPlayers = 0;
    bool started = false;


    //internet
    ClientSock * clients  ;       //all the socket the players connect to
    int serverSock;

    //sending and receiving
    char * msg[1024];
    thread accThread;
    thread * recvThreads;
    thread sendThread;


public:
    Server(int maxNrOfPLayers);


    // setup server so and start to listen on server socket;

    int start();

    void Talker();
    void stop();


    bool isStarted();

    void AcceptClients();


    void Listner(ClientSock aClient);
};


#endif //GAMESERVER_SERVER_H
