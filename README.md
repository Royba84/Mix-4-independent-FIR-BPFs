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

