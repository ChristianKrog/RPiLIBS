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
    itoa(const unsigned long int &myDecimal, char *myResult, const int &myBase);

public:
    PWM(string period_ns, int dutycycle);
    ~PWM();
    void clockPWM0(bool enable);
};
