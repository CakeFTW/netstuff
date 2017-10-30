//
// Created by rasmus on 10/21/17.
//

#ifndef GAMESERVER_CLIENT_H
#define GAMESERVER_CLIENT_H


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

class Client {
    bool started = false;
    int sock;
    thread sendThread;
    thread recvThread;
    char msgToBeSent[1024];
    bool hasMsgBeenSent = true;


public:
    Client();
    void start();
    void listen();
    void sendMessage(char* msg, int size);
    void talk();

    void informOfConnection();
};


#endif //GAMESERVER_CLIENT_H
