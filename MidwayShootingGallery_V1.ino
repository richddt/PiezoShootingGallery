//#########-MIDWAY SHOOTING GALLERY-##############
//Code by Rich DDT 7/25/18 www.richddt.com
//adapted from Arduino Sensor Example code "Knock" and Adafruit VS1053 Example code "player_simple"
//Piezo sensors are embedded behind Targets in a shooting gallery. Each Piezo is wired in parallel with a 1 MegaOhm resistor.
//This code assigns soundFX and Thresholds to each Analog input pin on an Arduino Mega2560, and plays the specified soundFX upon Target impact.

//#########-PIEZO TARGET SETTINGS-##############
//Variables and Arrays
const int pinCount = 10; // the number of Targets with embedded Piezos
const int piezoPins[] = { A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,  A8,  A9 }; // array of analog input pins for Piezos 
const int thresh[] =    { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }; // array of individual thresholds per Target; Default thresh = 100; lower is more sensitive; 50-100 seems good; Full Range: min = 0, max = 1023
const int timeout = 20; // delay time (ms) between reading Piezos; default 100, try 20 or lower
int piezoReading[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // array for holding Piezo sensor readings

//#########-SOUNDFX SETTINGS-##############
const char* soundFX[] = { "TRACK000.mp3", "TRACK001.mp3", "TRACK002.mp3", "TRACK003.mp3", "TRACK004.mp3", "TRACK005.mp3", "TRACK006.mp3", "TRACK007.mp3", "TRACK008.mp3", "TRACK009.mp3" }; // array for holding sound fx filenames
const int volume = 15; // lower numbers = louder volume, default 20

//#########-MP3 SHIELD INIT-##############
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  // Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


 

void setup() {
  Serial.begin(9600);       // use the serial port

//#########-MP3 SHIELD SETUP-##############
  Serial.println("MP3Shield VS1053 Bootup");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("micro SD card failed, or not present"));
    while (1);  // don't do anything more
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(volume,volume); // default 20,20

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  // musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
//  // Play one file, don't return until complete
//  Serial.println(F("Playing track 001"));
//  musicPlayer.playFullFile("track001.mp3");
//  // Play another file in the background, REQUIRES interrupts!
//  Serial.println(F("Playing track 002"));
//  musicPlayer.startPlayingFile("track002.mp3");
}

void loop() {
//#########-PIEZO MAIN LOOP-##############

// get all sensor readings into piezoReading array
for (int thisPin = 0; thisPin < pinCount; thisPin++) {
  piezoReading[thisPin] = analogRead(piezoPins[thisPin]);

// check piezoReading array against trigger thresholds
//for (int thisPin = 0; thisPin < pinCount; thisPin++) {
  // if the sensor reading is greater than the threshold
  if (piezoReading[thisPin] >= thresh[thisPin]) {
    //Print detection to Serial monitor; subtract 54 from Analog pin number (internal Pin ID's: A0=54, A9=63)
  Serial.print("Target ");Serial.print(piezoPins[thisPin]-54);Serial.print(" HIT: Playing soundFX "); Serial.println(soundFX[thisPin]);
    //Play SoundFX
    musicPlayer.playFullFile(soundFX[thisPin]);
       if (musicPlayer.stopped()) {
       Serial.println("Done playing sound"); }
  }
}
  
  delay(timeout);  // delay to avoid overloading the serial port buffer; default 100
}
