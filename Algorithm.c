
// Made by:Roy Ben Avraham
// 23/11/2023 07:53
// Lenovo T-460

/* Coefficient calculated with sampling frequency of 48kHz and cutoff at 8kHz. See included Matlab explaination*/
/*This program reads data in from the left and right channels and then sends the data out to the respective outputs. */

#include "bios_lab2cfg.h"
#define CHIP_6416 1
#include <std.h>
#include <log.h>
#include <csl.h>
#include <csl_timer.h>
#include <csl_irq.h>  
#include <tsk.h>
#include <sem.h>
#include "C6416_DSK_FIRcfg.h"
#include "dsk6416.h"
#include "dsk6416_aic23.h"
#include "dsk6416_dip.h"////////
/* Frequency Definitions */

//void InoutIsr (void);

//short fir_filter (short);
/* Codec configuration settings */
DSK6416_AIC23_Config config = { \
    0x0017,  /* 0 DSK6416_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK6416_AIC23_RIGHTINVOL Right line input channel volume */\
    0x01f9,  /* 2 DSK6416_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x01f9,  /* 3 DSK6416_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0011,  /* 4 DSK6416_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK6416_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK6416_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK6416_AIC23_DIGIF      Digital audio interface format */ \
    0x0081,  /* 8 DSK6416_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK6416_AIC23_DIGACT     Digital interface activation */   \
};
Int16 buffer_l[800] = { 0 };
Int16 buffer_r[800] = { 0 };
int m = 0;

void BPF_TASK(void);//TASK
void filter_pro(void);//SWI
DSK6416_AIC23_CodecHandle hCodec;
Int16 OUT_L, OUT_R;
Int16 OUT_1, OUT_2, OUT_3, OUT_4;
Uint32 IN_L, IN_R;
Int16 DIP_FLAG[4] = { 0 };
short R_in[128], R_in1[128], R_in2[128], R_in3[128], R_in4[128], R_in[128]; 		/* Input samples R_in[0] most recent, R_in[127] oldest */

/* coefficient calculated with sampling frequency of 48kHz and cutoff at 8kHz*/
short h1[] =       		/* Impulse response of FIR filter */
{ 36,37,38,40,41,42,43,44,44,44,42,39,34,27,18,7,
-7,-24,-44,-66	,-91,-119,-150,-182,-217,-252,-289,
-325,-361,-396,-429,-458,-484,-506,-522,-531,
-534,-530,-518,-498,-469,-431,-385,-331,-269,
-200,-124,-43,44,	134	,226,320,414,506,595,680,759,831,896,951	,
997,1031,1055,	1067,1067,1055,1031,997,951	,896,
831,759,680,595,506,414,320,226,134,44,-43,-124,-200,-269,
-331,-385,-431,-469,-498,-518,	-530,-534,-531,
-522,-506,-484,-458,-429,-396,-361,-325,-289,
-252,-217,-182,-150,-119,-91,-66,-44,-24,-7,7,18,27,
34,39,42,44,44,44,43,42,41,40,	38,	37,	36 };

short h2[] =
{ 23,21,15,6,-6,-18,-29,-35,-35,-30,-19,-6,6,13,13,6,-6,
-17,-20,-11,15,55,102,146,171,166,124,48,-52,-157,-245,-295,
-294,-243,-153,-49,42,92,90,38,-41,-112,-132,-69,93,
338,623,880,1030,1003,757,297,-327,-1015,-1640,-2069,
-2193,-1956,-1364,-496,510,1482,2249,2671,2671,2249,
1482,510,-496,-1364,-1956,-2193,-2069,-1640,-1015,-327,
297,757,1003,1030,880,623,338,93,-69,-132,-112,-41,38,
90,92,42,-49,-153,-243,-294,-295,-245,-157,-52,48,
124,166,171,146,102,55,15,-11,-20,-17,-6,6,13,13,6,-6,-19,
-30,-35,-35,-29,-18,-6,6,15,21,23 };

short h3[] =
{ 22,10,-10,-27,-29,-13,13,33,33,14,-13,-30,-26,-9,6,5,-6,-8,
   14,51,71,38,-47,-137,-161,-76,86,228,246,108,-113,-278,
   -277,-112,105,230,197,64,-41,-36,39,51,-91,-325,-440,-233,
   287,829,970,461,-522,-1405,-1548,-699,755,1949,2066,900,
   -939,-2348,-2414,-1021,1035,2516,2516,1035,-1021,
   -2414,-2348,-939,900,2066,1949,755,-699,-1548,-1405,
   -522,461,970,829,287,-233,-440,-325,-91,51,39,-36,-41,64,
   197	,30,105,-112,-277,-278,-113,108,246,228,86,-76,-161,
   -137,-47,38,71,51,14,-8,-6,5,6,-9,-26,-30,-13,14,33,33,
   13,-13,-29,-27,-10,10,22 };
short h4[] =
{ 20,-5,-27,-16,17,32,7,-29,-30,7,34,18,-16,-22,-3,5,-5,4,
36,31,-42,-97,-24,123,145,-39,-219,-137,148,277,57,-250,-249,
57,270,138,-118,-163,-21,32,-35,26,233,195,-264,-597,-146,
746,873,-235,-1337,-845,931,1792,385,-1754,-1859,459,2407,
1412,-1452,-2617,-528,2264,2264,-528,-2617,-1452,1412,
2407,459,-1859,-1754,385,1792,931,-845,-1337,-235,873,746,
-146,-597,-264,195,233,26,-35,32,-21,-163,-118,138,270,57,
-249,-250,57,277,148,-137,-219,-39,145,123,-24,-97,-42,31,36,
4,-5,5,-3,-22,-16,18,34,7,-30,-29,7,32,17,-16,-27,-5,20 };

short ffir_filter(short input)
{
    int i;
    short output;
    int acc = 0;
    int prod;
    R_in[0] = input;
    for (i = 0; i < 128; i++)         	/* 128 taps */
    {
        /* Perform Q.15 multiplication */
        prod = (((DIP_FLAG[0] * h1[i]) + (DIP_FLAG[1] * h2[i]) + (DIP_FLAG[2] * h3[i]) + (DIP_FLAG[3] * h4[i])) * R_in[i]);

        acc = acc + prod;           /* Update 32-bit accumulator, catering */
    }
    output = (short)(acc >> 15);    	/* Cast output to 16-bits. */
    for (i = 127; i > 0; i--)         	/* Shift delay samples */
        R_in[i] = R_in[i - 1];
    return output;
}


void main()
{
    //int acc=0;
    /* Initialize the board support library, must be called first */
    DSK6416_init();
    DSK6416_DIP_init();

    /* Start the codec */
    hCodec = DSK6416_AIC23_openCodec(0, &config);

    // Set codec frequency to 48KHz (44kHz does not seem to work!)
    DSK6416_AIC23_setFreq(hCodec, DSK6416_AIC23_FREQ_48KHZ);



    SEM_post(&Flag_Bps);
    /* Close the codec */
    return;
    //   DSK6416_AIC23_closeCodec(hCodec);

}


//SWI
void filter_pro(void)
{
    DIP_FLAG[0] = !DSK6416_DIP_get(0);
    DIP_FLAG[1] = !DSK6416_DIP_get(1);
    DIP_FLAG[2] = !DSK6416_DIP_get(2);
    DIP_FLAG[3] = !DSK6416_DIP_get(3);

    SEM_post(&Flag_Bps);
}

//THE TASK
void BPF_TASK(void)
{
    /* This is your task*/
    while (1)
    {
        // Read sample from the left channel 
        while (!DSK6416_AIC23_read(hCodec, &IN_L));

        // Read sample from the right channel 
        while (!DSK6416_AIC23_read(hCodec, &IN_R));


        SWI_post(&SWI_FILTER_PROJECT);//
        SEM_pend(&Flag_Bps, SYS_FOREVER);
        OUT_L = ffir_filter(IN_L);
        OUT_R = IN_R;
        /* Send the FILTERED sample to the left channel */
        while (!DSK6416_AIC23_write(hCodec, OUT_L));

        /* Send a NON FILTERED sample to the right channel */
        while (!DSK6416_AIC23_write(hCodec, OUT_R));

        if (m < 800)
        {
            buffer_l[m] = OUT_L;
            buffer_r[m] = OUT_R;
            m++;
        }
        else
        {
            m = 0;
            buffer_l[m] = OUT_L;
            buffer_r[m] = OUT_R;
            m++;
        }
    }
    /* Close the codec */
    DSK6416_AIC23_closeCodec(hCodec);//IS IN THE MAIN
    //LOG_printf(&fastprint2,"Task is running \n");
}






