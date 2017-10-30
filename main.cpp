#include <iostream>
#include "Server.h"
#include "Client.h"


using namespace std;
int main() {

    int gameInfo = 0;


    std::cout << "1 for server 2 for client " << std::endl;
    cin >> gameInfo;

    if(gameInfo == 1) {
        cout << " how many player will be on this server" << endl;
        cin >> gameInfo;
        Server gameServer(gameInfo);
        gameServer.start();

    }else if(gameInfo == 2) {
        Client client;
        client.start();

    }


    cout << "the program has executed" << endl;



    return 0;
}