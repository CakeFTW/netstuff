//
// Created by rasmus on 10/21/17.
//

#include "Client.h"

Client::Client() {
    cout << "setting up client" << endl;
    struct addrinfo *p;
    struct addrinfo socketHints;
    struct addrinfo * ClientInfo;

    char * point = "172.0.0.1";;

    memset(&socketHints, 0, sizeof(socketHints)); // get hints about the server
    string test;
    cout <<  "type addr to cennect to " << endl;
    cin >> test;

    cout << "i got the addr " << test.c_str() << endl;

    socketHints.ai_family = AF_UNSPEC;
    socketHints.ai_socktype = SOCK_STREAM;
    //socketHints.ai_flags = AI_PASSIVE;
    socketHints.ai_addr = (sockaddr *) test.c_str();


    if (getaddrinfo(NULL, port, &socketHints, &ClientInfo) != 0) {
        cout << "could not get addres" << endl;
    }

    for (p = ClientInfo; p != NULL; p = p->ai_next) {
        if (sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1) {
            cout << "one addr failed ";
            continue;
        }
        cout << "One addr worked trying to connect: ";
        if (connect(sock , p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            cout << "but could not connect " << endl;
            continue;
        }
        cout << "connection established " << endl;


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
    freeaddrinfo(ClientInfo); // all done with this structure
    started = true;
}

void Client::talk() {
    int haha = 0;


    informOfConnection();
    bool test = false;

    while (started) {

        if(hasMsgBeenSent == true){
            continue ;
        }

        send(sock, msgToBeSent, 1024, 0)!= 0;
        hasMsgBeenSent = true;
        cout << "msg sent " << endl;
    }
}

void Client::listen() {
    cout << "im listning " << endl;
    char res[1024];
    int dataReceived;
    while (started){
        dataReceived = recv(sock, res , 1024 , 0);
        if(dataReceived != 0){
            //cout << " received data " << res << endl;
        }

        //listen logic goes here

    }

}

void Client::start() {
    sendThread = thread([this]{ talk();});
    recvThread = thread([this]{ listen();});
    cout << "threads have been started" << endl;
    int counter = 0;

    double time = rand() % 10;
    char arr[1024];
    while (true){

        if(counter++ > 500000000+(4*time * 50000000)) {
            string name = "rasmus";
            string info = "c " + name + " sais fuckka you";
            strcpy(arr, info.c_str());
            sendMessage(arr, 1024);
            counter = 0;

        }

    }
    sendThread.join();
    recvThread.join();

}

void Client::sendMessage(char * msg, int size) {
    if(size != 1024){
        cout << "msg to large or to small use 1024" << endl;
        return;
    }
    for(int i = 0 ; i < 1024; i++) {
        *(msgToBeSent + i) = *(msg + i);
    }
    hasMsgBeenSent = false;
}

void Client::informOfConnection() {
    char arr[1024];
    string name = "rasmus";
    string info = "c " + name + " would like to join the server";
    strcpy(arr, info.c_str());
    sendMessage(arr, 1024);


}

