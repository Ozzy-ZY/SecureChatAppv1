#include <iostream>
#include <winsock.h>
#include <thread>
#include <chrono>
#include <mutex>
#include "Decryption.h"
#include "Encryption.h"
#include "utils.h"

#define PORT 9909
#define BUFFER_SIZE 256
#define SERVER_IP "127.0.0.1"
#define EXIT_KEYWORD "\\exit"
using namespace std;
int nClientSocket;
struct sockaddr_in srv;
char buff[BUFFER_SIZE] = { 0, };
bool recvcontinuation = true; // Set the flag to continue loop1

mutex mtx;

void recive() {
    while (recvcontinuation) {
        this_thread::sleep_for(chrono::seconds(1));
        int bytesReceived = recv(nClientSocket, Decrypt(buff, 20,15), BUFFER_SIZE - 1, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
        {
            cout << "Error receiving response from the server" << endl;
            WSACleanup();
            exit(EXIT_SUCCESS);
        }
        std::lock_guard<std::mutex> lock(mtx);
        cout << buff << endl;
    }
}

void sending() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        fgets(buff, 256, stdin);
        if (strncmp(buff, EXIT_KEYWORD, strlen(EXIT_KEYWORD)) == 0)
        {
            recvcontinuation = false;
            WSACleanup();
            exit(EXIT_SUCCESS);
        }
        size_t len = strlen(buff);
        if (len > 0 && buff[len - 1] == '\n')
            buff[len - 1] = '\0';
        send(nClientSocket, Encrypt(buff, 15, 20), len, 0);
    }
}

int main()
{
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
        char buff[255] = { 0, };
        char userMessage[255] = { 0, };
        // Concatenate the predefined message and user input
        recv(nClientSocket, buff, 256, 0);
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
    }
}
