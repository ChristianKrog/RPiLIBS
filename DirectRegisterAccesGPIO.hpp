#pragma once

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

/////GLOBAL CONSTANTS/////
const unsigned int c_BCM2835_PERI_BASE = 0x20000000;               //Use with RPi Zero & 1 - Not used
const unsigned int c_BCM2836_PERI_BASE = 0x3F000000;               //Use with RPi 2 - Not used
const unsigned int c_BCM2837_PERI_BASE = 0x3F000000;               //Use with RPi 3 - Not used
const unsigned int c_BCM2711_PERI_BASE = 0xFE000000;               //Use with RPi 4
const unsigned int c_GPIO_BASE = (c_BCM2711_PERI_BASE + 0x200000); //GPIO controller
const unsigned int c_PAGE_SIZE = (4 * 1024);
const unsigned int c_BLOCK_SIZE = (4 * 1024);

class GPIODRA
{
private:
    int fdMemory_; //Memory file descriptor

public:
    GPIODRA();                                               //Initiate direct memory access
    ~GPIODRA();                                              //Clear all GPIO ports
    void clearGPIO(const int GPIO);                          //Clear specific GPIO port
    void setModeDRA(const int GPIO, const int direction);    //Set mode of GPIO, direction = 0 = input, direction = 1 = output
    void writeDRA(const int GPIO, const int value);          //Write to GPIO, value = 0/1
    void writeDRAString(const int GPIO, const string value); //Write to GPIO, value = "0"/"1"
    int readDRA(const int GPIO);                             //Read from GPIO, return decimal 0/1
    string readDRAString(const int GPIO);                    //Read from GPIO, return string "0"/"1"
};