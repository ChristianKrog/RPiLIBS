#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

class GPIODRA
{
private:
public:
    GPIODRA();
    ~GPIODRA();
    void setModeDRA(int _gpio, int direction);    //Set mode of GPIO, direction = 0 = input, direction = 1 = output
    void writeDRA(int _gpio, int value);          //Write to GPIO, value = 0/1
    void writeDRAString(int _gpio, string value); //Write to GPIO, value = "0"/"1"
    int readDRA(int _gpio);                       //Read from GPIO, return decimal 0/1
    string readDRAString(int _gpio);              //Read from GPIO, return string "0"/"1"
};