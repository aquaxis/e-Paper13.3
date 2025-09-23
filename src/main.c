#include <stdio.h>
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "DEV_Config.h"
#include "EPD_13in3e.h"
#include "ImageData.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    DEV_ModuleExit();
    exit(0);
}

int main(int argc, char **argv)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    // Check command line arguments
    if (argc < 2) {
        printf("Usage: %s <bitmap_file>\n", argv[0]);
        return -1;
    }

    // Load bitmap image
    if (loadbmp(argv[1]) != 0) {
        printf("Failed to load bitmap file: %s\n", argv[1]);
        return -1;
    }

    // Initialize e-Paper display
    printf("13.3inch e-Paper E demo\r\n");
    DEV_ModuleInit();
    DEV_Delay_ms(500);

    EPD_13IN3E_Init();

    // Display the image
    EPD_13IN3E_Display(Image6color);

    // Put display to sleep
    EPD_13IN3E_Sleep();

    // Clean up
    DEV_ModuleExit();

    return 0;
}