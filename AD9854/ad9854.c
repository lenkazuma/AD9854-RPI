#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "ad9854.h"

//**********************System Clock Variables**************************

/* 
 * According to the system clock configuration, enable one block per compile.
 * CLK_Set         : Clock Multiplexer, Range 4~20. Maximum clock freq: 300MHz
 * Freq_mult_ulong : 2^48 / Clock (Long)
 * Freq_mult_double: 2^48 / Clock (Double)
 */

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_double = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_double = 2814749.76710656;
*/

 /*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_double = 2345624.80592213;
  */

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_double = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_double = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_double = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_double = 1407374.88355328;
*/

/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_double = 1279431.712321164;
*/


#define      CLK_Set            12
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_double = 1172812.402961067;


/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_double = 1082596.064271754;
*/

/*
#define      CLK_Set            14
const ulong  Freq_mult_ulong  = 1005268;
const double Freq_mult_double = 1005267.773966629;
*/

/*
#define      CLK_Set            15
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_double = 938249.9223688533;

 */

// Global Variables
uchar FreqWord[6];      //6 bytes freq control

// Macros
#define AD9854_UDCLK() \
    digitalWrite(AD9854_UCLK, HIGH); \
    digitalWrite(AD9854_UCLK, LOW)
 
static int AD9854_IO_Init(void)
{
    if (wiringPiSetup() == -1) {
        return -1;
    }
    // Setup Port DDR.
    pinMode(AD9854_WR,   OUTPUT);
    pinMode(AD9854_RD,   OUTPUT);
    pinMode(AD9854_RST,  OUTPUT);
    pinMode(AD9854_UCLK, OUTPUT);
    
    pinMode(AD9854_ADDR5, OUTPUT);
    pinMode(AD9854_ADDR4, OUTPUT);
    pinMode(AD9854_ADDR3, OUTPUT);
    pinMode(AD9854_ADDR2, OUTPUT);
    pinMode(AD9854_ADDR1, OUTPUT);
    pinMode(AD9854_ADDR0, OUTPUT);
    
    pinMode(AD9854_DATA7, OUTPUT);
    pinMode(AD9854_DATA6, OUTPUT);
    pinMode(AD9854_DATA5, OUTPUT);
    pinMode(AD9854_DATA4, OUTPUT);
    pinMode(AD9854_DATA3, OUTPUT);
    pinMode(AD9854_DATA2, OUTPUT);
    pinMode(AD9854_DATA1, OUTPUT);
    pinMode(AD9854_DATA0, OUTPUT);
    return 0;
}

static void AD9854_WriteFreqWord(uchar address)
{
    uchar count;
    for (count = 6; count > 0; ) {
        AD9854_WR_Byte(address++, FreqWord[--count]);
    }
}

static void AD9854_WriteShape(uint shape)
{
    AD9854_WR_Byte(0x21, shape >> 8);
    AD9854_WR_Byte(0x22, (uchar)(shape & 0xff));
    AD9854_WR_Byte(0x23, shape >> 8);
    AD9854_WR_Byte(0x24, (uchar)(shape & 0xff));
}

static void AD9854_WR_Byte(_uint32 address, _uint32 data)
{
    // Write the address
    digitalWrite(AD9854_ADDR5, (address & 0x20)==0 ? LOW : HIGH);
    digitalWrite(AD9854_ADDR4, (address & 0x10)==0 ? LOW : HIGH);
    digitalWrite(AD9854_ADDR3, (address & 0x08)==0 ? LOW : HIGH);
    digitalWrite(AD9854_ADDR2, (address & 0x04)==0 ? LOW : HIGH);
    digitalWrite(AD9854_ADDR1, (address & 0x02)==0 ? LOW : HIGH);
    digitalWrite(AD9854_ADDR0, (address & 0x01)==0 ? LOW : HIGH);
    // Write the data
    digitalWrite(AD9854_DATA7, (data & 0x80) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA6, (data & 0x40) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA5, (data & 0x20) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA4, (data & 0x10) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA3, (data & 0x08) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA2, (data & 0x04) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA1, (data & 0x02) == 0 ? LOW : HIGH);
    digitalWrite(AD9854_DATA0, (data & 0x01) == 0 ? LOW : HIGH);
    // Write update.
    digitalWrite(AD9854_WR, LOW);
    digitalWrite(AD9854_WR, HIGH);
}

//====================================================================================
//void Freq_convert(long Freq)
//====================================================================================
static void FreqConvert(long freq)
{
    uint64_t word = (uint64_t)(unsigned long)freq * (uint64_t)Freq_mult_ulong;

    FreqWord[0] = (uchar)word;
    FreqWord[1] = (uchar)(word >> 8);
    FreqWord[2] = (uchar)(word >> 16);
    FreqWord[3] = (uchar)(word >> 24);
    FreqWord[4] = (uchar)(word >> 32);
    FreqWord[5] = (uchar)(word >> 40);
}

//====================================================================================
//void Freq_doublt_convert(double Freq)
//====================================================================================
static void FreqDoubleConvert(double freq)
{
    uint64_t word = (uint64_t)(freq * Freq_mult_double);

    FreqWord[0] = (uchar)word;
    FreqWord[1] = (uchar)(word >> 8);
    FreqWord[2] = (uchar)(word >> 16);
    FreqWord[3] = (uchar)(word >> 24);
    FreqWord[4] = (uchar)(word >> 32);
    FreqWord[5] = (uchar)(word >> 40);
}

static int AD9854_Initialized(_uint32 sysMode)
{
    if (AD9854_IO_Init() != 0) {
        fprintf(stderr, "AD9854: wiringPiSetup() failed (run as root on Raspberry Pi?)\n");
        return -1;
    }
    // Disable R/W control port
    digitalWrite(AD9854_WR, HIGH);
    digitalWrite(AD9854_RD, HIGH);
    // Clear the update clock.
    digitalWrite(AD9854_UCLK, LOW);
    // Reset AD9854
    digitalWrite(AD9854_RST, HIGH);
    digitalWrite(AD9854_RST, LOW);
    // Configure the AD9854.
    AD9854_WR_Byte(0x1d, 0x00);         //Close the comparator.
    AD9854_WR_Byte(0x1e, CLK_Set);      //Set the system clock multiplexer 
    AD9854_WR_Byte(0x1f, sysMode);      //Set system mode.
    AD9854_WR_Byte(0x20, 0x60);         //Cancel the interpolation compensation
    // Update output.
    AD9854_UDCLK();
    return 0;
}

void AD9854_Init(void)
{
    AD9854_Initialized(0x00);
}

void AD9854_InitFSK(void)
{
    if (AD9854_Initialized(0x02) == 0) {
        pinMode(AD9854_FSK, INPUT);
    }
}

void AD9854_InitBPSK(void)
{
    if (AD9854_Initialized(0x08) == 0) {
        pinMode(AD9854_FSK, INPUT);
    }
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_SetSine(ulong freq, uint shape)
{
    FreqConvert(freq);
    AD9854_WriteFreqWord(0x04);
    AD9854_WriteShape(shape);
    AD9854_UDCLK();
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_SetSine_double(double freq, uint shape)
{
    FreqDoubleConvert(freq);
    AD9854_WriteFreqWord(0x04);
    AD9854_WriteShape(shape);
    AD9854_UDCLK();
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_SetFSK(ulong Freq1,ulong Freq2)
{
    const uint shape = 4000;

    FreqConvert(Freq1);
    AD9854_WriteFreqWord(0x04);
    FreqConvert(Freq2);
    AD9854_WriteFreqWord(0x0a);
    AD9854_WriteShape(shape);
    AD9854_UDCLK();
}

void AD9854_SetBPSK(uint phase1, uint phase2)
{
    phase1 &= 0x3fff;
    phase2 &= 0x3fff;

    AD9854_WR_Byte(0x00, (uchar)(phase1 >> 8));
    AD9854_WR_Byte(0x01, (uchar)(phase1 & 0xff));
    AD9854_WR_Byte(0x02, (uchar)(phase2 >> 8));
    AD9854_WR_Byte(0x03, (uchar)(phase2 & 0xff));
    AD9854_UDCLK();
}