/* Copyright (c) 2018 pcbreflux. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>. *
 */

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED


// End of constructor list

char aVocal[] = { 'A', 'E', 'I', 'O', 'U' };
char aConsonant[] = { 'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};
char aEnd[] = { 'L', 'N', 'M', 'R', 'S', 'T' };

#define MIN_LEN 3
#define MAX_LEN 6

String proGen() {
  int len = random(MIN_LEN,MAX_LEN);
  String pro=String("");
  
  randomSeed(0);

    pro += String(aConsonant[random(0,sizeof(aConsonant))]);
  for (int iPos=1;iPos<(len-1);iPos++) {
    if (len>3) {
      if (len%2==0) {  // even 
       if (iPos%2==0) { // even
          pro += String(aVocal[random(0,sizeof(aVocal))]);
        } else { // odd
          if (random(0,1)==0) {
            pro += String(aConsonant[random(0,sizeof(aConsonant))]);
          } else {
            pro += String(aVocal[random(0,sizeof(aVocal))]);
          }
       }
      } else { // odd
        if (iPos%2==0) { // even
          if (random(0,1)==0) {
            pro += String(aConsonant[random(0,sizeof(aConsonant))]);
          } else {
            pro += String(aVocal[random(0,sizeof(aVocal))]);
          }
        } else { // odd
          pro += String(aVocal[random(0,sizeof(aVocal))]);
        }
      }
    } else {
      pro += String(aVocal[random(0,sizeof(aVocal))]);
    }
  }
  pro += String(aEnd[random(0,sizeof(aEnd))]);

  Serial.println(pro);
  return pro;
}

#define GPIO_IN_PIN 0
#define MIN_TIME 100
#define MAX_TIME 3000
uint32_t delay_time=0;

#if defined(ESP32)
void IRAM_ATTR onGPIO(){
#else
void onGPIO(){
#endif
  delay_time = random(MIN_TIME,MAX_TIME);
}

void setup() {
  #if defined(__AVR__)
    #if defined(ARDUINO_AVR_MEGA2560)
      Serial.begin(115200);
    #else
      Serial.begin(9600);
    #endif
  #else
    Serial.begin(115200);
  #endif

  Serial.println("start");
  pinMode(GPIO_IN_PIN, INPUT);
  onGPIO();

  #if defined(ESP32)
    attachInterrupt(GPIO_IN_PIN, &onGPIO, GPIO_PIN_INTR_POSEDGE); // or GPIO_INTR_ANYEDGE or GPIO_PIN_INTR_NEGEDGE
  #endif
  
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_helvR24_tr);	// choose a suitable font
  u8g2.drawStr(0,32,proGen().c_str());	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(delay_time);
}

