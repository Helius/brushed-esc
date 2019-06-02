Speed controller for brushed motor. 
Input signal is standard servo signal from rc receiver.
I use it for 8520 coreless motor on my plane.

Arduino (Atmega 328P)

Input: EXT0 (connected to rc receiver servo signal)
Output: OC0A (use non-invert PWM to drive a motor) 

Build:

This is avr gcc CMake based project.
```
mkdir build
cd build
cmake ../
make
```
Have fun! )
