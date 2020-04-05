#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "converter.hpp"

//GLOBAL VARIABLES
const int PORT = 8080;

using namespace std;

class TCP
{
private:
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    Converter converter;

public:
    TCP();
    ~TCP();
    string readWriteTCP(char *writeMsg, int readSize);
    string readTCP(int readSize);
    string readWithLengthTCP(void);
    void writeTCP(string message);
    void writeWithLengthTCP(string message);
};