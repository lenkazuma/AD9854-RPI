#ifndef _AD9854_H_
#define _AD9854_H_

// Define the pin maps
//  Functions
#define AD9854_RST      23   //AD9854 Reset
#define AD9854_UCLK     24   //AD9854 Update Clock
#define AD9854_WR       25   //AD9854 Write Enable, Low active
#define AD9854_RD       27   //AD9854 Read Enable, Low active
#define AD9854_OSK      28   //AD9854 OSK Controller
#define AD9854_FSK      29   //AD9854 FSK, PSK control
//  Address
#define AD9854_ADDR5    4
#define AD9854_ADDR4    5
#define AD9854_ADDR3    6
#define AD9854_ADDR2    10
#define AD9854_ADDR1    11
#define AD9854_ADDR0    26
//  Data
#define AD9854_DATA7    0
#define AD9854_DATA6    2
#define AD9854_DATA5    3
#define AD9854_DATA4    12
#define AD9854_DATA3    13
#define AD9854_DATA2    14
#define AD9854_DATA1    21
#define AD9854_DATA0    22

// Define the basic types.
#define uint    unsigned int    // 4 bytes
#define uchar   unsigned char   // 1 byte
#define ulong   unsigned long   // 4 bytes

#define _uint32 unsigned int

// Internal static functions

// Extern functions
extern void AD9854_Init(void);
extern void AD9854_SetSine(ulong freq, uint shape);
extern void AD9854_SetSine_double(double freq, uint Shape);
extern void AD9854_InitFSK(void);
extern void AD9854_SetFSK(ulong freq1, ulong freq2);
extern void AD9854_InitBPSK(void);	
extern void AD9854_SetBPSK(uint phase1, uint phase2);

#endif // _AD9854_H_
