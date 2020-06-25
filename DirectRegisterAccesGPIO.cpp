<<<<<<< HEAD
/***********************************************************************/
/*************    Christian L. Krog & Jesper R. Nielsen    *************/
/***********************************************************************/
/************* gpioDRA.cpp controls the GPIO's on the RPi  *************/
/*************        using direct register access         *************/
/***********************************************************************/

#include "gpioDRA.hpp"

volatile unsigned *gpio; //I/O access
void *gpio_map;

//GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
=======
#include "DirectRegisterAccesGPIO.hpp"

#define BCM2835_PERI_BASE 0x20000000             //Use with RPi Zero & 1 
#define BCM2836_PERI_BASE 0x3F000000             //Use with RPi 2
#define BCM2837_PERI_BASE 0x3F000000             //Use with RPi 3
#define BCM2711_PERI_BASE 0xFE000000             //Use with RPi 4
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000) //GPIO controller
#define PAGE_SIZE (4 * 1024)
#define BLOCK_SIZE (4 * 1024)

int mem_fd;
void *gpio_map;
volatile unsigned *gpio; //I/O access

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
>>>>>>> 8a5db9f70ed0717fd5b0eed3c82314a2e3911a9b
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3)) //Input
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3))  //Output
#define SET_GPIO_ALT(g, a) *(gpio + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))
#define GPIO_SET *(gpio + 7)                  //Sets bits which are 1. Ignores bits which are 0
#define GPIO_CLR *(gpio + 10)                 //Clears bits which are 1. Ignores bits which are 0
#define GET_GPIO(g) (*(gpio + 13) & (1 << g)) //0 if LOW, (1<<g) if HIGH
#define GPIO_PULL *(gpio + 37)                //Pull up/pull down
#define GPIO_PULLCLK0 *(gpio + 38)            //Pull up/pull down clock

GPIODRA::GPIODRA()
{
    if ((fdMemory_ = open("/dev/mem", O_RDWR | O_SYNC)) < 0) //open /dev/mem
    {
        perror("Error - open - gpioDRA.cpp");
        exit(EXIT_FAILURE);
    }

    gpio_map = mmap(
        NULL,                   //Any adddress in our space will do
        c_BLOCK_SIZE,           //Map length
        PROT_READ | PROT_WRITE, //Enable reading & writting to mapped memory
        MAP_SHARED,             //Shared with other processes
        fdMemory_,              //File to map
        c_GPIO_BASE             //Offset to GPIO peripheral
    );
    close(fdMemory_); //No need to keep fdMemory_ open after mmap

    if (gpio_map == MAP_FAILED)
    {
        perror("Error - map - gpioDRA.cpp");
        exit(EXIT_FAILURE);
    }
    gpio = (volatile unsigned *)gpio_map; //Volatile pointer. Important
}

GPIODRA::~GPIODRA()
{
    for (int i = 0; i < 28; i++)
    {
        GPIO_CLR = 1 << i; //Clear all GPIO's. Total of 28 GPIO's on RPi 4b
    }
}

void GPIODRA::clearGPIO(const int GPIO)
{
    GPIO_CLR = 1 << GPIO; //Clear GPIO
}

void GPIODRA::setModeDRA(const int GPIO, const int direction) //0 = input, 1 = output
{
    if (direction == 0)
    {
        INP_GPIO(GPIO); //Set selected GPIO to input mode
    }
    else if (direction == 1)
    {
        INP_GPIO(GPIO); //Must set GPIO to input, before setting to output
        OUT_GPIO(GPIO); //Set selected GPIO to output mode
    }
}

void GPIODRA::writeDRA(const int GPIO, const int value) //Value parameter must be an int datatype (1 or 0)
{
    if (value == 0)
    {
        GPIO_CLR = 1 << GPIO; //Write 0 to selected GPIO
    }
    else if (value == 1)
    {
        GPIO_SET = 1 << GPIO; //Write 1 to selected GPIO
    }
}

void GPIODRA::writeDRAString(const int GPIO, const string value) //Value parameter must be a string datatype ("1" or "0")
{
    if (value == "0")
    {
        GPIO_CLR = 1 << GPIO; //Write 0 to selected GPIO
    }
    else if (value == "1")
    {
        GPIO_SET = 1 << GPIO; //Write 1 to selected GPIO
    }
}

int GPIODRA::readDRA(const int GPIO) //Read selected GPIO
{
    if (GET_GPIO(GPIO))
    {
        return 1; //Return integer 1
    }
    else
    {
        return 0; //Return integer 0
    }
}

string GPIODRA::readDRAString(const int GPIO) //Read selected GPIO
{
    if (GET_GPIO(GPIO))
    {
        return "1"; //Return string "1"
    }
    else
    {
        return "0"; //Return string "0"
    }
}