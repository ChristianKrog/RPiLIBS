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
    void setMode(int _gpio, int direction);    //Set mode of GPIO, direction = 0 = input, direction = 1 = output
    void write(int _gpio, int value);          //Write to GPIO, value = 0/1
    void writeString(int _gpio, string value); //Write to GPIO, value = "0"/"1"
    int read(int _gpio);                       //Read from GPIO, return decimal 0/1
    string readString(int _gpio);              //Read from GPIO, return string "0"/"1"
};