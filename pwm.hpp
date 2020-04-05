#pragma once

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>

using namespace std;

class PWM
{
private:
    int fdPWM;
    int fdVal;

public:
    PWM();
    ~PWM();
    void clockPWM0(bool enable);
};