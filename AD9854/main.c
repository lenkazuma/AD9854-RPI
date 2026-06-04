#include <unistd.h>
#include "ad9854.h"

int main(void)
{
    AD9854_Init();
    AD9854_SetSine(70000000, 4095);
    /* Keep process alive so the DDS output remains configured. */
    for (;;) {
        pause();
    }
    return 0;
}
