#include "PWM.hpp"

PWM::PWM(string period_ns, int dutycycle)
{
    if (open("/sys/class/pwm/pwmchip0/pwm0", O_RDONLY) == -1) //Check if pwm0 is exported
    {
        /////Export PWM0/////
        fdPWM = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
        fdVal = write(fdPWM, "0", 1);

        if (fdVal == -1)
        {
            cout << "Error on writing to export. " << strerror(errno) << endl;
        }
        else
        {
            close(fdPWM);
        }
    }

    /////Set period/////
    fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    fdVal = write(fdPWM, period_ns, strlen(period_ns));
    if (fdVal == -1)
    {
        cout << "Error on writing to period. " << strerror(errno) << endl;
    }
    else
    {
        close(fdPWM);
    }

    /////Set Dutycycle/////
    int period = stoi(period_ns, nullptr, 10);
    int dutyTemp = period/(100/dutycycle);
    char dutyBuffer[10];
    itoa(dutyTemp, dutyBuffer, 10);
    fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    fdVal = write(fdPWM, dutyBuffer, strlen(dutyBuffer));

    if (fdVal == -1)
    {
        cout << "Error on writing to dutycycle. " << strerror(errno) << endl;
    }
    else
    {
        close(fdPWM);
    }

    /////Set polarity/////
    char polarity[] = "normal";
    char readBUF[10];
    fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/polarity", O_RDWR);
    fdVal = read(fdPWM, readBUF, 6);

    if (fdVal == -1)
    {
        cout << "Error on reading from polarity. " << strerror(errno) << endl;
    }
    else
    {
        if (readBUF != polarity)
        {
            fdVal = write(fdPWM, polarity, strlen(polarity));

            if (fdVal == -1)
            {
                cout << "Error on writing to polarity. " << strerror(errno) << endl;
            }
            else
            {
                close(fdPWM);
            }
        }
    }
}

PWM::~PWM()
{
    /////Unexport PWM0/////
    if (open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY) == -1) //Check if PWM0 is unexported
    {
        fdPWM = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
        fdVal = write(fdPWM, "0", 1);

        if (fdVal == -1)
        {
            cout << "Error on writing to unexport. " << strerror(errno) << endl;
        }
        else
        {
            close(fdPWM);
        }
    }
}

void PWM::clockPWM0(bool enable)
{
    if (enable == true)
    {
        fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
        fdVal = write(fdPWM, "1", 1);

        if (fdVal == -1)
        {
            cout << "Error on writing to enable. " << strerror(errno) << endl;
        }
        else
        {
            close(fdPWM);
        }
    }
    else
    {
        fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
        fdVal = write(fdPWM, "0", 1);

        if (fdVal == -1)
        {
            cout << "Error on writing to enable. " << strerror(errno) << endl;
        }
        else
        {
            close(fdPWM);
        }
    }
}


char *PWM::itoa(const unsigned long int &myDecimal, char *myResult, const int &myBase)
{
    if (myBase < 2 || myBase > 16)
    {
        *myResult = 0;
        return myResult;
    }
    char *out = myResult;
    unsigned long int quotient = myDecimal;

    do
    {
        *out = "0123456789abcdef"[quotient % myBase];
        ++out;
        quotient /= myBase;
    } while (quotient);
    reverse(myResult, out);
    *out = 0;
    return myResult;
}