# PiLOT_G2/GRACE Flight Software
PiLOT_G2/GRACE is second payload after PiLOT-1, indigeneuosly built at SSAPCE Lab in IIST.
## Main objectives of the mission includes:
1) First Ever Implementation of In Orbit Reprogramming of both FPGA and ENVM in India at University Level.
2) Qualify the CubeSat UHF Board designed at IIST.
3) Qualify the Gieger Muller Counter Board used to measure the count of the Radiations in orbit.
4) Qualify the Command Reception architecture from POEM packages.
## Improvements in Flight Software as compare to PiLOT-1 payload:
1) FPGA level implementation of RS485 data transfer to POEM packages. This allowed the SOC to run at minimum of only 30MHz of clock speed for successful transfer of data.
2) Interrupt based data collection mechanism which does not allows the software to stuck in between in case of any unexpected hardware failure.
