# Mix-4-independent-FIR-BPFs
A design of 4 FIR Band pass filters

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/d88081e2-2184-46a6-bd3d-993020f3644e)

Goal: Design a mechanism that mixes 4 independent FIR BPFs according to the following demands:
- Output signal is a sum of 4 filters.
- Use DIP-Switches to operate each one of them.
- OUT_R Signal comes from filters output.
- OUT_L Signal comes from IN_L (original).
- Band Limit Frequencies:
  F1	500-1000HZ
  F2	2000-4000HZ
  F3	5000-7000HZ
  F4	8000-10000HZ

  - Use Hamming window; Fs=48KHz.
  - Display graph of left and right channels - use (at least) 800 words buffer each.
  - Use headphones (wide bandwidth response)
  - Use BIOS Task and SWI (upon sample acquired) to operate filters.
 

# Solution:

Our project data-flow will be as shown in the scheme below:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/19aed689-2e81-47d7-8500-19c9a37c1f32)

Explanation:
- To meet the requirements mentioned above I implemented this project using DSP/BIOS according the above diagram.
- The 'main' function performs initalizations and calls for SEM_post (step 1) to TASK and finishes on the 'return'.
- The DSP/BIOS accessing the TASK and in there, there is an infinite loop which reads the A/D samples using the function 'DSK6416_AIC23_read'.
- Afterwards the variables IN_R,IN_L getting their values and a SWI_post call is being made (step 2) and right after that SEM_pend (step 3) is being triggered which stops the TASK and SWI performence.
- Inside the SWI I monitor switches 0 to 3 and save the status of each switch in the DIP_FLAG array. In the end of SWI a SEM_post call is being triggered to return to the TASK from the point we made the SEM_pend.
- On TASK (in the point we have returned to after SEM_pend) I filter the entrance IN_L according to the switch mode (DIP_FLAG array) using the function ffir_filter, saving the filter results in OUT_L variable. OUT_R variable gets the entrance signal IN_R without filtration at all.
- Afterwards, I send my output variables OUT_R,OUT_L to the D/A using the function DSK6416_AIC23_write.
- With that process done, the TASK in our loop is infinite and therefore we go back to the beginning to sample new data and it goes like that in theory forever.

  ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/d9968112-adaf-4e4e-b72b-86fc79548540)

What can we see in the above image:
- Input signal is connected through LINE_IN
- Filtration (according to the following):
  - If switch 0 is on, I will perform BPF filtration in frequency range:       500-1000[Hz].
  - If switch 1 is on, I will perform BPF filtration in frequency range:
    2000-4000[Hz].
  - If switch 2 is on, I will perform BPF filtration in frequency range:
    5000-7000[Hz].
  - If switch 3 is on, I will perform BPF filtration in frequency range:
    8000-10000[Hz].
- The output signal is the sum of all 4 filters.
- Output signal will be taken from LINE_OUT.


Introduction - theory:

Using MATLAB Iv'e calculated the coefficients of the impulse response of the filters using parameters as: ripple, sampling frequency, filter type and cut-off frequency.

the output (y) is given by the following equation:
![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/171134a3-6128-4e78-b99e-364b232a2491)

while:
![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/f79a34e5-861e-4e70-8625-c317e654c036)

x[n] - input signal of the filter.
h[n]=bk[n] - represents the filter coefficients.
y[n] - output signal of the filter.
N - represents order (number of coefficiens) of the filter.

Implementation of the output:
![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/3464a1d4-c6ed-433f-9423-107e460b6ce3)

In order to calculate the coefficients i used the fda_tool that can be found on MATLAB, parameters can be seen in the following image:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/1cf6adef-a4d2-4a94-b5de-3709e4eee457)

# Implementations of the 4 filters requested:

1. Filter #1 500-1000[Hz]:

   Frequency range: 500-1000[Hz]
   127 Coefficients
   Sampling frequency: 48[kHz]
   Window type: Hamming window

   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/bcf3568d-02b1-49c1-b90e-ecf9b9f5e82e)

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/ba01263b-8017-4fa7-a436-707161e42b49)

This graph does not cut actually in f=1.5[kHz], there is a small deviation as can be seen above.

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/151ff747-5026-402e-aa5a-81dbc0c29cf8)

In the lower fc (cut-off frequency - under -3dB) is equal to ~485[Hz], while 500[Hz] is being achieved at -2.6dB.

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/8469633b-678d-4533-9bd1-fbd0490d097c)

2. Filter #2 2000-4000[Hz]:

   Frequency range: 2000-4000[Hz]
   127 Coefficients
   Sampling frequency: 48[kHz]
   Window type: Hamming window

   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/3af9e5c8-f855-4703-b329-84c92af96010)

   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/24c706ba-365b-4843-b14c-16bd636c24f2)


3. Filter #3 5000-7000[Hz]:

   Frequency range: 5000-7000[Hz]
   127 Coefficients
   Sampling frequency: 48[kHz]
   Window type: Hamming window

   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/f6f15e96-95a0-4e78-9aa6-a1ef0571572a)


![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/5491dbb9-eec8-42c1-a4c1-a47eb29f84b8)


4. Filter #4 8000-10000[Hz]

   Frequency range: 8000-10000[Hz]
   127 Coefficients
   Sampling frequency: 48[kHz]
   Window type: Hamming window

   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/38764e66-a5d1-4766-952c-ee186a71370f)


   ![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/dc172ef9-c9bd-44e1-9ff0-21c902860b3f)


Coefficients:

# 500-1000[Hz]:

h1[]=
    {36,37,38,40,41,42,43,44,44,44,42,39,34,27,18,7,
     -7,-24,-44,-66	,-91,-119,-150,-182,-217,-252,-289,
     -325,-361,-396,-429,-458,-484,-506,-522,-531,
     -534,-530,-518,-498,-469,-431,-385,-331,-269,
     -200,-124,-43,44, 134,226,320,414,506,595,680,759,831,896,951,
     997,1031,1055, 1067,1067,1055,1031,997,951	,896,	
     831,759,680,595,506,414,320,226,134,44,-43,-124,-200,-269,
     -331,-385,-431,-469,-498,-518, -530,-534,-531,
     -522,-506,-484,-458,-429,-396,-361,-325,-289,
     -252,-217,-182,-150,-119,-91,-66,-44,-24,-7,7,18,27,
     34,39,42,44,44,44,43,42,41,40,38, 37, 36 };

# 1000-2000[Hz]:

h2[]=       		
     {23,21,15,6,-6,-18,-29,-35,-35,-30,-19,-6,6,13,13,6,-6,
     -17,-20,-11,15,55,102,146,171,166,124,48,-52,-157,-245,-295,
     -294,-243,-153,-49,42,92,90,38,-41,-112,-132,-69,93,
     338,623,880,1030,1003,757,297,-327,-1015,-1640,-2069,
     -2193,-1956,-1364,-496,510,1482,2249,2671,2671,2249,
     1482,510,-496,-1364,-1956,-2193,-2069,-1640,-1015,-327,
     297,757,1003,1030,880,623,338,93,-69,-132,-112,-41,38,
     90,92,42,-49,-153,-243,-294,-295,-245,-157,-52,48,
     124,166,171,146,102,55,15,-11,-20,-17,-6,6,13,13,6,-6,-19,
     -30,-35,-35,-29,-18,-6,6,15,21,23};

# 5000-7000[Hz]:

h3[]=       
     {22,10,-10,-27,-29,-13,13,33,33,14,-13,-30,-26,-9,6,5,-6,-8,
     	14,51,71,38,-47,-137,-161,-76,86,228,246,108,-113,-278,	
     	-277,-112,105,230,197,64,-41,-36,39,51,-91,-325,-440,-233,	
     	287,829,970,461,-522,-1405,-1548,-699,755,1949,2066,900,	
     	-939,-2348,-2414,-1021,1035,2516,2516,1035,-1021,	
     	-2414,-2348,-939,900,2066,1949,755,-699,-1548,-1405,	
     	-522,461,970,829,287,-233,-440,-325,-91,51,39,-36,-41,64,	
     	197	,30,105,-112,-277,-278,-113,108,246,228,86,-76,-161,	
     	-137,-47,38,71,51,14,-8,-6,5,6,-9,-26,-30,-13,14,33,33,	
     	13,-13,-29,-27,-10,10,22};

# 8000-10000[Hz]:

h4[]=       	
     {20,-5,-27,-16,17,32,7,-29,-30,7,34,18,-16,-22,-3,5,-5,4,	
     36,31,-42,-97,-24,123,145,-39,-219,-137,148,277,57,-250,-249,	
     57,270,138,-118,-163,-21,32,-35,26,233,195,-264,-597,-146,	
     746,873,-235,-1337,-845,931,1792,385,-1754,-1859,459,2407,	
     1412,-1452,-2617,-528,2264,2264,-528,-2617,-1452,1412,
     2407,459,-1859,-1754,385,1792,931,-845,-1337,-235,873,746,	
     -146,-597,-264,195,233,26,-35,32,-21,-163,-118,138,270,57,	
     -249,-250,57,277,148,-137,-219,-39,145,123,-24,-97,-42,31,36,	
     4,-5,5,-3,-22,-16,18,34,7,-30,-29,7,32,17,-16,-27,-5,20};


# Graphs from DSP/BIOS

After running our functions, we can see visualy the analysis of our signals:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/6923679e-cc37-4dd7-aa09-ccf05e1bc5d9)

In the above image, we can see a sine wave with 20[kHz] as input signal, which filters the frequencies 2000-4000 [Hz] (meaning switch 1 is on). We can also see that the sine (impulse in the frequency domain) is being dumped drastically. OUT_R signal wthich is the original signal has 6.7*10^5 height, while OUT_L (the filtered signal) has the magnitude of 966, about 100,000 times lower (50dB).

In the 'pass' range of the filter, the magnitudes are the same:
![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/74d78fb9-75c8-48d8-8278-137244a5d9b5)

For input signal with frequency=4[kHz] and switch 0 is on (Filter 500-1000[Hz]) we get:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/18870b1d-6307-4209-9127-1f2767ce53cd)

We can see that this frequecy is being filters, lets examine it in the time domain this time:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/403c36b6-9aed-4e8c-9dbe-8d6dfcbce8d8)


AND:


![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/e7c139be-c1ea-4f55-9520-1eb471a862ae)

Meaning the sine wave in the output does not exist - meaning it was filterd.


# SWI settings and BIOS TASK

SWI & flag:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/3a52183d-1db6-486d-a613-35e3da1bedaf)

TASK:

![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/c8706fb6-d3f0-4e0c-ac69-502cd6316dc9)

flag (SEM):


![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/f0158cca-f067-4646-8677-0ed85bb5f6a9)


# Final thoughts and tips

while working on this project I had some difficulties with output signal distortion as shown in the image below:


![image](https://github.com/Royba84/Mix-4-independent-FIR-BPFs/assets/105777016/e3671a8b-7d8f-4b7f-b72e-2a75ce26ce49)

This was the result of inefficient filtration function. the root cause for that was inaccurate calculation of the coefficients.
to overcome this obstacle i used the comutative property of the convolution operation. On the same input signal we should put N filters instead of performing N convolutions, add the impulse responses and finally perform only 1 convolution.

Yours, Roy B.A.
