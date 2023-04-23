

// Standard includes
#include <stdlib.h>
#include <string.h>


static void displayBanner();



int main(int argc, char** argv)
{


    /* Stop WDT and initialize the system-clock of the MCU */
    stopWDT();
    initClk();

    /* Configure command line interface */
    CLI_Configure();

    displayBanner();
}


static void displayBanner()
{
    CLI_Write("\n\r\n\r");
    CLI_Write(" MQTT Twitter Controlled RGB LED - Version ");
    CLI_Write("\n\r*******************************************************************************\n\r");
}
