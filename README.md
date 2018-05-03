MSP430 Microcontroller Final
============================

## John Harvey

### About
Written for use with the MSP430 Launchpad microcontroller.  

Using the msp430 along with peripherals, the goal of the program is to light up the LED light with various percentages of red, blue, and green light. The user will type 'setc' followed by an 'r', 'g', or 'b', followed by a digit between 0-99.The bubble display will show the values given, and the LED light will light up. The color of the LED will be determined by the values entered for 'r', 'g', and 'b'.  

**Example Input**
```
$ setc r 88
$ setc g 25
$ setc b 72
```
### Install Notes
1. Clone repo with git `git clone https://github.com/jharvey7136/msp430_lights.git`
2. Connect msp430 to computer
3. Navigate to app's root directory
4. Compile program with command: `make`
5. Flash compiled .elf file to msp430: `make flash`
6. Run with command: `make screen`












