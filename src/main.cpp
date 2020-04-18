#include <Arduino.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <avr/wdt.h>

// Interval zapnuteho cerpadla v sekundach
#define RUN_INTERVAL  5
#define STOP_INTERVAL  5
#define RELE  8
#define PIR  A1
#define DIP1  7
#define DIP2  6
#define DIP3  5
#define EEINDEXADR  0
#define EECOUNTERADR  32

inline void pumpOff(void);
inline void pumpIgnDi(void);
inline void blink(void);
inline uint16_t eepAdrGet(void);
inline bool pirRiseEdge(void);
inline void serialPrint(void);

bool ledState, pirState = true, ignore;
uint32_t counter = 0;
uint8_t eeIndex;

Ticker tmrBlink(blink,500);
Ticker tmrRun(pumpOff, RUN_INTERVAL * 1000, 1);
Ticker tmrStop(pumpIgnDi, STOP_INTERVAL * 1000, 1);
Ticker tmrSerial(serialPrint, 5000);

void setup() {
// Init serial
  while (!Serial);
  Serial.begin(9600);
  delay(2000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELE, OUTPUT); // rele
  pinMode(PIR, INPUT_PULLUP); // vstup pre PIR
  pinMode(DIP1, INPUT_PULLUP); // vstup pre DIP SW
  pinMode(DIP2, INPUT_PULLUP); // vstup pre DIP SW
  pinMode(DIP3, INPUT_PULLUP); // vstup pre DIP SW

  // EEPROM INIT
  //EEPROM.put(0,0xff);   // counter reset
  EEPROM.get(EEINDEXADR, eeIndex);
  if (eeIndex == 0xff) {
    eeIndex = 0;
    EEPROM.put(EEINDEXADR, eeIndex);
    EEPROM.put(EECOUNTERADR, counter);
  }
  // nacitanie counter z EEPROMky
  EEPROM.get(eeIndex * sizeof(counter) + EECOUNTERADR, counter);

  tmrBlink.start();
  tmrSerial.start();
  serialPrint();
  wdt_enable(WDTO_1S);
}

void loop() {
  tmrRun.update();
  tmrStop.update();
  tmrBlink.update();
  tmrSerial.update();

  wdt_reset();

  if (!digitalRead(DIP1)) {   // testovaci rezim
    digitalWrite(RELE,digitalRead(PIR));
  }
  else {                      // prevadzkovy rezim
    if (!ignore) {
      if (pirRiseEdge()) {
        digitalWrite(RELE, 1);
        ignore = true;
        tmrRun.start();
        counter++;
        if ((counter / 0xffff) != eeIndex) {
          eeIndex = (counter / 0xffff);
          EEPROM.put(EEINDEXADR, eeIndex);
        }
        EEPROM.put(eeIndex * sizeof(counter) + EECOUNTERADR, counter);
      }
    }
    else pirRiseEdge();
  }
}
// ******************************************************
// ******************************************************
inline void pumpOff(void) {
  digitalWrite(RELE, 0);
  tmrStop.start();
}
// ------------------------------------------------------
inline void pumpIgnDi(void) {
  ignore = false;
}
// ------------------------------------------------------
inline void blink(void) {
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
}
// ------------------------------------------------------
inline bool pirRiseEdge(void) {
  if (digitalRead(PIR)) {
    if (!pirState) {
      pirState = true;
      return(true);
    }
  }
  else pirState = false;
  return(false);
}
// ------------------------------------------------------
inline void serialPrint(void) {
  Serial.println(F("PWP12 v1.0 2020/04"));
  Serial.print(F("Interval for water pump RUN is "));
  Serial.println(RUN_INTERVAL,DEC);
  Serial.print(F("Interval for water pump STOP is "));
  Serial.println(STOP_INTERVAL,DEC);
  Serial.print(F("Counter = "));
  Serial.println(counter,DEC);
  Serial.println();
}
// ------------------------------------------------------
