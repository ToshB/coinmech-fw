#ifndef CardReader_h
#define CardReader_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include <SoftwareSerial.h>
#else
#include "Particle.h"
#endif

class CardReader {
private:
#if defined(ARDUINO) && ARDUINO >= 100
  SoftwareSerial *_rfid;
#else
  USARTSerial *_rfid;
#endif
public:
  CardReader(int, int);
  void initialize();
  void read();
  bool validateData(char *);
  bool isValid;
  char data[11];
};

#endif
