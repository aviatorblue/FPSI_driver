/*  sFPI Driver version 1.5
    Copyright (c) 2015 David Miles Houston
    
    This sketch uses the following associated
    pins in conjunction with the AD7837ANZ MDAC,
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
  
*/

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
        DDRB = B00111111; // Sets Port B pins to output
        DDRD = B11111110; // Sets Port D pins to output
        DDRC = B00000000; // Sets Port C pins for input      
        PORTB = B00111000; // ~CS = 1, ~WR = 1, ~LDAC = 1
        PORTD = B00000000; // Initialized the Data Pins to 0
        pinMode(A0,INPUT);
}

// Infinite Loop
void loop()
{
/* Generates a sawtooth wave with 4096 data points allowing for two variations of the signal
   It will pull values from a register or global variable which will determine the first set
   point (i.e. Peak-to-Peak voltage of the sawtooth wave). There will be 10 specific values  
   It will combine that value with a value which will determine by how much to multiply each 
   indiviual voltage step (1/4096 - 1) giving a bit resolution of 4096/div. So no matter which
   value you are at there will 4096 different steps. If this is too dense and undisguishabled
   I will simple change the bit density.
   */
	for (int i = 0; i < wave_bits-1; i = i + skip) {
		writetoPins(i,val);
  }
  val = analogRead(A0);
}

void writetoPins(int i, int scale) {// This is for 1 value
        int LSB = i % 256;
	int MSB = i/256;
	int scale_LSB = scale % 256;
	int scale_MSB = scale/256;

        PORTB = B00011000; // TURNs on the MDAC for DATA recival
	PORTB = B00001000; // Turns on the ~WR bit for writing
        //DAC B
        
        PORTB = PORTB ^ B00000100; // Set A1 to 1
        PORTB = PORTB ^ B00000010; // Set A0 to 1
        
        // DAC B - MSB
        PORTD = PORTD | (scale_MSB) << 1;
        // DAC B - LSB
        PORTB = PORTB ^ B00000010; // Set A0 to 0
        PORTB = PORTB | scale_LSB/128;
        PORTD = PORTD | scale_LSB % 128 << 1; 
        
        // DAC A
        PORTB = PORTB ^ B00000100; // Set A1 to 0
        PORTB = PORTB ^ B00000010; // Set A0 to 1
        // DAC A - MSB
        PORTD = PORTD | MSB << 1;
        // DAC A - LSB
        PORTB = PORTB ^ B00000010; // Set A0 to 0
        PORTB = PORTB | LSB/128;
        PORTD = PORTD | LSB % 128 << 1;

	PORTB = B00011000; // Write high to ~WR
        PORTB = B00111000; // Write high to ~CS
        PORTB = B00110000; // Load Values in Latches
	//delayMicroseconds(5); // wait 5 us
	PORTB = B00111000;
        PORTD = B00000000;

}

