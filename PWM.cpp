#include "PWM.hpp"

PWM::PWM()
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
    char period[] = "200000"; // 1/5000Hz*1e-9
    fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    fdVal = write(fdPWM, period, strlen(period));

    if (fdVal == -1)
    {
        cout << "Error on writing to period. " << strerror(errno) << endl;
    }
    else
    {
        close(fdPWM);
    }

    /////Set Dutycycle/////
    char dutyCycle[] = "100000"; //Period/2 = 50%;
    fdPWM = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    fdVal = write(fdPWM, dutyCycle, strlen(dutyCycle));

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
