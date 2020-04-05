#include "DirectRegisterAccesGPIO.hpp"
#define BCM2711_PERI_BASE 0xFE000000             //Only for RPi 4
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000) //GPIO controller
#define PAGE_SIZE (4 * 1024)
#define BLOCK_SIZE (4 * 1024)
int mem_fd;
void *gpio_map;
volatile unsigned *gpio; //I/O access
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3)) //Input
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3))  //Output
#define SET_GPIO_ALT(g, a) *(gpio + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))
#define GPIO_SET *(gpio + 7)                  //sets bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio + 10)                 //clears bits which are 1 ignores bits which are 0
#define GET_GPIO(g) (*(gpio + 13) & (1 << g)) //0 if LOW, (1<<g) if HIGH
#define GPIO_PULL *(gpio + 37)                //Pull up/pull down
#define GPIO_PULLCLK0 *(gpio + 38)            //Pull up/pull down clock

GPIODRA::GPIODRA()
{
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) //open /dev/mem
    {
        cout << "can't open /dev/mem " << endl;
        exit(1);
    }

    /////mmap GPIO//////
    gpio_map = mmap(
        NULL,                   //Any adddress in our space will do
        BLOCK_SIZE,             //Map length
        PROT_READ | PROT_WRITE, //Enable reading & writting to mapped memory
        MAP_SHARED,             //Shared with other processes
        mem_fd,                 //File to map
        GPIO_BASE               //Offset to GPIO peripheral
    );
    close(mem_fd); //No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED)
    {
        cout << "mmap error " << (int)gpio_map << endl; //errno also set!
        exit(1);
    }
    gpio = (volatile unsigned *)gpio_map; //Volatile pointer. Important
}

GPIODRA::~GPIODRA()
{
    for (int i = 0; i < 28; i++)
    {
        GPIO_CLR = 1 << i;
    }
}

void GPIODRA::setMode(int _gpio, int direction) //0 = "IN", 1 = "OUT"
{
    if (direction == 0)
    {
        INP_GPIO(_gpio);
    }
    else if (direction == 1)
    {
        INP_GPIO(_gpio);
        OUT_GPIO(_gpio);
    }
}

void GPIODRA::write(int _gpio, int value)
{
    if (value == 0)
    {
        GPIO_CLR = 1 << _gpio;
    }
    else if (value == 1)
    {
        GPIO_SET = 1 << _gpio;
    }
}

void GPIODRA::writeString(int _gpio, string value)
{
    if (value == "0")
    {
        GPIO_CLR = 1 << _gpio;
    }
    else if (value == "1")
    {
        GPIO_SET = 1 << _gpio;
    }
}

int GPIODRA::read(int _gpio)
{
    if (GET_GPIO(_gpio))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

string GPIODRA::readString(int _gpio)
{
    if (GET_GPIO(_gpio))
    {
        return "1";
    }
    else
    {
        return "0";
    }
}
