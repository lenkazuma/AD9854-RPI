
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
 
static void AD9854_IO_Init(void)
{
    // Setup Wiring Pi.
    wiringPiSetup();
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
    ulong FreqBuf;
    ulong Temp=Freq_mult_ulong;              

    uchar Array_Freq[4];      
    Array_Freq[0]=(uchar) freq;
    Array_Freq[1]=(uchar)(freq>>8);
    Array_Freq[2]=(uchar)(freq>>16);
    Array_Freq[3]=(uchar)(freq>>24);

    FreqBuf=Temp*Array_Freq[0];                  
    FreqWord[0]=FreqBuf;    
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[1]);
    FreqWord[1]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[2]);
    FreqWord[2]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[3]);
    FreqWord[3]=FreqBuf;
    FreqBuf>>=8;

    FreqWord[4]=FreqBuf;
    FreqWord[5]=FreqBuf>>8;            
}

//====================================================================================
//void Freq_doublt_convert(double Freq)
//====================================================================================
static void FreqDoubleConvert(double freq)   
{
    ulong Low32;
    uint  High16;
    double Temp=Freq_mult_double;
    freq*=(double)(Temp);
    //1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
    //2^32 = 4294967295
    High16 =(int)(freq/4294967295);
    freq -= (double)High16*4294967295;
    Low32 = (ulong)freq;
    //Set the 6 FreqWord to write in Addr
    FreqWord[0]=Low32;
    FreqWord[1]=Low32>>8;
    FreqWord[2]=Low32>>16;
    FreqWord[3]=Low32>>24;
    FreqWord[4]=High16;
    FreqWord[5]=High16>>8;
}

static void AD9854_Initialized(_uint32 sysMode)
{
    // Initialized I/O.
    AD9854_IO_Init();
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
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_Init(void)
{
    // Set system mode to 0, update by external.
    AD9854_Initialized(0x00);
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_InitFSK(void)
{
    // Set system mode to 1, update by external.
    AD9854_Initialized(0x02);
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_InitBPSK(void)
{
    // Set system mode to 4, update by external.
    AD9854_Initialized(0x08);
}

/*
 * Name       :
 * Desciption :
 * Input      :
 * Output     :
 */
void AD9854_SetSine(ulong freq, uint shape)
{
    uchar count;
    //Select the initial address
    uchar address = 0x04;
    
    // Convert the frequency.
    FreqConvert(freq);
    // Write the frequency to AD9854.
    for(count=6; count>0; )
    {
        AD9854_WR_Byte(address++, FreqWord[--count]);
    }
    // Set I channel amplitude
    AD9854_WR_Byte(0x21, shape>>8);
    AD9854_WR_Byte(0x22, (uchar)(shape & 0xff));
    // Set Q channel amplitude
    AD9854_WR_Byte(0x23, shape>>8);
    AD9854_WR_Byte(0x24, (uchar)(shape & 0xff));
    // Update output.
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
    uchar count;
    //Select the initial address
    uchar address = 0x04;
    
    // Convert the frequency.
    FreqDoubleConvert(freq);
    // Write the frequency to AD9854.
    for(count=6; count>0; )
    {
        AD9854_WR_Byte(address++, FreqWord[--count]);
    }
    // Set I channel amplitude
    AD9854_WR_Byte(0x21, shape>>8);
    AD9854_WR_Byte(0x22, (uchar)(shape & 0xff));
    // Set Q channel amplitude
    AD9854_WR_Byte(0x23, shape>>8);
    AD9854_WR_Byte(0x24, (uchar)(shape & 0xff));
    // Update output.
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
    uchar count=6;
    // Set the freq initial address
    uchar address1=0x04, address2=0x0a;
    // Set the shape, 12-bit, range 0~4095
    const uint shape=4000;
    // Convert the frequency 1
    FreqConvert(Freq1);
    // Write the frequency 1.
    for(count=6; count>0; )
    {
        AD9854_WR_Byte(address1++, FreqWord[--count]);
    }
    // Convert the frequency 2
    FreqConvert(Freq2);
    // Write the frequency 2.
    for(count=6; count>0; )
    {
        AD9854_WR_Byte(address2++, FreqWord[--count]);
    }
    // Set I channel amplitude
    AD9854_WR_Byte(0x21, shape>>8);
    AD9854_WR_Byte(0x22, (uchar)(shape & 0xff));
    // Set Q channel amplitude
    AD9854_WR_Byte(0x23, shape>>8);
    AD9854_WR_Byte(0x24, (uchar)(shape & 0xff));
    // Update output.
    AD9854_UDCLK();
}