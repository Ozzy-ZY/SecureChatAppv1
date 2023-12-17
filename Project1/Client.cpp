#include <iostream>
#include <winsock.h>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include "Decryption.h"
#include "Encryption.h"
#include "utils.h"
#include <condition_variable>

#define PORT 9909
#define BUFFER_SIZE 1000
#define SERVER_IP "127.0.0.1"
#define EXIT_KEYWORD "\\exit"

using namespace std;
vector<string> users = { "zyad", "seif", "haidar", "seifWaleed", "zyadQasem" };
condition_variable cv;
atomic<bool> terminateFlag(false);
bool atom = false;
mutex mtx;
int nClientSocket;
struct sockaddr_in srv;
char buff[BUFFER_SIZE ] = { 0 };

void recive() {
    while (!atom) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(nClientSocket, &readSet);

        timeval timeout;
        timeout.tv_sec = 1;  // 1-second timeout
        timeout.tv_usec = 0;

        int readySockets = select(0, &readSet, nullptr, nullptr, &timeout);

        if (readySockets == SOCKET_ERROR) {
            cout << "Error in select()" << endl;
            break;
        }

        if (readySockets > 0 && FD_ISSET(nClientSocket, &readSet)) {
            int bytesReceived = recv(nClientSocket, buff, BUFFER_SIZE - 1, 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                cout << "Error receiving response from the server" << endl;
                break;
            }
            Decrypt(buff, 10, 5);
            cout << buff << endl;
            memset(buff, 0, sizeof(buff));
        }
    }
}

void sending() {
    while (!terminateFlag) {
        this_thread::sleep_for(chrono::seconds(1));
        fgets(buff, BUFFER_SIZE, stdin);
        if (strncmp(buff, EXIT_KEYWORD, strlen(EXIT_KEYWORD)) == 0)
        {
            atom = true;
            break;
        }
        else
        {
            size_t len = strlen(buff);
            if (len > 0 && buff[len - 1] == '\n')
                buff[len - 1] = '\0';
                send(nClientSocket, Encrypt(buff, 10, 5).c_str(), len, 0);
        }
    }
}
int main()
{
    string user;
    cout<<"Enter your username: ";
    cin >> user;
    if (!Authenticate(user, users)) {
		cout << "You are not allowed to use this service" << endl;
		return 0;
	}
    int nRet = 0;
    WSAData ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << "WSAStartup failed" << endl;
        WSACleanup();
        return (EXIT_FAILURE);
    }

    nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (nClientSocket < 0)
    {
        cout <<"socket call failed" << endl;
        WSACleanup();
        return (EXIT_FAILURE);
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&srv.sin_zero, 0, 8);
    nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet < 0)
    {
        cout <<"connect failed" << endl;
        WSACleanup();
        return (EXIT_FAILURE);
    }
    else
    {
       //connected to the sever
        char userMessage[BUFFER_SIZE-1] = { 0, };
        // Concatenate the predefined message and user input
        recv(nClientSocket, buff, BUFFER_SIZE, 0);
        cout <<"Just press enter to see the message received from other clients" << endl;
        cin.ignore(); // Ignore any newline character left in the buffer
        cout<< "type " << EXIT_KEYWORD << " to quit";
        cout << buff << endl;
        //now send what you want
        // Create two threads
        thread thread2(sending);
        thread thread1(recive);
        // Wait for both threads to finish (which will never happen in this case)
        thread2.join();
        thread1.join();
        WSACleanup();
        return 0;
    }
}
