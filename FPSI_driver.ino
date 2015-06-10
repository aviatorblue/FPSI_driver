/*  sFPI Driver version 1.5
    Copyright (c) 2015 David Miles Houston
    
    This sketch uses the following associated
    pins in conjunction with the ,
    for the use of a sawtooth signal generator.
    
    Pin Name      Pin Location
    ---------    --------------
      DB0              1
      DB1              2
      DB2              3
      DB3              4
      DB4              5
      DB5              6
      DB6              7
      DB7              8
      A0               9
      A1               10
      LDAC             11
      WR               12         
      CS               13

  April 30, 2015 by David Houston
  - Replaced digitalWrite(a,b) and pinMode(a,b) functions with
  simple i/o pin manipulations via registers. Increased speed
  of the program by 10 times. Reduced number of values sent to
  the MDAC for increased speed to meet specs of 100 Hz. Possibly 
  use MDAC as a controllable attenuation rather than a DAC. This
  would be much faster. 
  
  May 07, 2015 by David Houston
  - Frequency requirement was reduced to 20 Hz increasing the bit 
  resolution. The next addition takes the voltage output from the
  transimpedance amplifier of the photodiode stores it in a 4096 
  int array and after 1 cycle transfers the data to an offsite
  processor which processes the data, stores it, and sends it to
  a computer for viewing via USB protocol. The offsite process will
  store atleast 20 cycles. 
  
  Developers Note: In order for this code to operate correctly on the 
  AD7837 MDAC you must both write to DAC A and DAC B, however, do not
  write anything to DAC B's latches or it will screw up the system.
  
     Pin Name      Pin Location
    ---------    --------------
      DB0              1
      DB1              2
      DB2              3
      DB3              4
      DB4              5
      DB5              6
      DB6              7
      DB7              8
      A0               9
      A1               10
      LDAC             11
      WR               12         
      CS               13
  
  June 10, 2015 by David Houston
  - Due to power issues in the first design the current design will use
  the MCP4725 DAC which requires only a 5V supply from the Ardunio 
  eliminating possible failures with power consumption and current draw.
  In the future we may use the AD7837 MDAC for atennuation purposes,
  however it will be put on the shelf for now.
  
  Developers Note: Current Pin out is as follows
  
*/

#include <Wire.h>
#include "Adafruit_MCP4725.h"

Adafruit_MCP4725 dac; // use layout from the Adafruit Header File

#define bitres 1024;
const int numbits = 1024; // Number of tunable bits at one time
const int wave_bits = 4096; // Number of bits in the period of the signal
int skip = 10; // Number of bits to skip each time to increase speed
			
// Variables 

int dc_value,v_peak,multiplier,daca,dacb;
int val=0; // Int Variables
float scalar,dc_read,magnitude,vpp,ramp_wave_max; // Float Variables
int photodiode_response[wave_bits];

// Setup Loop
void setup()
{
  // Setup for Serial Communication
  // Pin Assignments
  pinMode(A0,INPUT);
}

// Infinite Loop
void loop()
{
/* Generates a sawtooth wave with 1024/n data points allowing for two variations of the signal
   It will pull values from a register or global variable which will determine the first set
   point (i.e. Peak-to-Peak voltage of the sawtooth wave). There will be 10 specific values  
   It will combine that value with a value which will determine by how much to multiply each 
   indiviual voltage step (1/4096 - 1) giving a bit resolution of 4096/div. So no matter which
   value you are at there will 4096 different steps. If this is too dense and undisguishabled
   I will simple change the bit density.
   */
	for (int i = 0; i < wave_bits-1; i = i + skip) {
		dac.setVoltage(i,false);
  }
}

