#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
 
// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

 
// Variables for the Sleep/power down modes:
volatile boolean f_wdt = 1;
volatile int watchdog_counter = 0;

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
  watchdog_counter++;
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
 
  sleep_mode();                        // System actually sleeps here
 
  sleep_disable();                     // System continues execution here when watchdog timed out 
  
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
  
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
 
  byte bb;
  if (ii > 9 ) 
    ii=9;
  bb=ii & 7;
  if (ii > 7) 
    bb|= (1<<5);
  bb|= (1<<WDCE);
 
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// the setup routine runs once when you press reset:
void setup() {                
  ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
  setup_watchdog(3); // approximately 128ms seconds sleep
}

// the loop routine runs over and over again forever:
void loop() {
  if(watchdog_counter > 0) {
    watchdog_counter = 0;
    digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(1, HIGH);
    delay(1000);               // wait for a second
    digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(1, LOW); 
  }
  system_sleep(); //Go to sleep! Wake up 1sec later and check water
}


