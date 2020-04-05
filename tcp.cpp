#include "TCP.hpp"


TCP::TCP()
{
    int opt = 1;
    addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(1);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(1);
    }
}

TCP::~TCP()
{
    close(server_fd);
}

string TCP::readWriteTCP(char *writeMsg, int readSize)
{
    int newSocket;

    //Create new socket
    if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        return "";
    }

    char buffer[readSize] = {0}; //Read buffer

    //Read message from client
    if (read(newSocket, buffer, readSize) < 0)
    {
        perror("Read TCP");
        return "";
    }

    //Send message to client
    if (send(newSocket, writeMsg, strlen(writeMsg), 0) < 0)
    {
        perror("Write TCP");
        return "";
    }

    return converter.convertToString(buffer, readSize);
}

string TCP::readTCP(int readSize)
{
    int newSocket;

    //Create new socket
    if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        return "";
    }

    char buffer[readSize] = {0}; //Read buffer

    //Read message from client
    if (read(newSocket, buffer, readSize) < 0)
    {
        perror("Read TCP");
        return "";
    }

    return converter.convertToString(buffer, readSize);
}

string TCP::readWithLengthTCP(void)
{
    int length = converter.binToDec(readTCP(8));
    return readTCP(length);
}

void TCP::writeTCP(string message)
{
    int newSocket;

    //Create new socket
    if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        return;
    }

    char *writeMsg = converter.convertToCharArray(message);

    //Send message to client
    if (send(newSocket, writeMsg, strlen(writeMsg), 0) < 0)
    {
        perror("Write TCP");
        return;
    }
}

void TCP::writeWithLengthTCP(string message)
{
    writeTCP(converter.decToBin(message.size(), 8));
    writeTCP(message);
}