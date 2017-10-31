//
// Created by rasmus on 10/21/17.
//

#include "Client.h"

Client::Client() {
    cout << "setting up client" << endl;

    char ip[] = "192.168.43.219"; // Server IP
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "\n-Error establishing socket..." << endl;
        exit(-1);
    }


    cout << "\n- Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);


    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        cout << "- Connection to the server port number: " << port << endl;
    }




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
            cout << " received data " << res << endl;
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

