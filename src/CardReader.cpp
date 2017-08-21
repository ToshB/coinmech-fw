
#if defined(ARDUINO) && ARDUINO >= 100
#include "CardReader.h"
#include "Arduino.h"
#include <SoftwareSerial.h>
#else
#include "CardReader.h"
#endif

// SoftwareSerial RFID(2, 3);
const byte RFID_SIZE = 10;
const byte CSUM_SIZE = 2;
const byte DATA_SIZE = RFID_SIZE + CSUM_SIZE;
const char EMPTY_TAG[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

CardReader::CardReader(int rx, int tx) {
#if defined(ARDUINO) && ARDUINO >= 100
  _rfid = new SoftwareSerial(rx, tx);
  _rfid->begin(9600);
#else
  _rfid = &Serial1; // Serial1 for Particle
  _rfid->begin(9600);
#endif
}

void CardReader::initialize() {}

byte asciiToByte(char c) {
  if ((c >= '0') && (c <= '9'))
    return c - '0';
  else if ((c >= 'A') && (c <= 'F'))
    return 10 + c - 'A';

  return c;
}

bool CardReader::validateData(char *tag) {
  byte checksum = 0;
  byte bsum = 0;
  byte code[6];

  if (tag[0] == 0x0) {
    return false;
  }

  for (int pos = 0; pos < DATA_SIZE; pos += 2) {
    bsum = asciiToByte(tag[pos]) | (asciiToByte(tag[pos + 1]) << 4);
    code[pos >> 1] = bsum;
    if (pos != RFID_SIZE) {
      checksum ^= bsum;
    }
  }
  Serial.print("Validated tag (with checksum): ");
  for (int i = 0; i < DATA_SIZE; i++) {
    Serial.print(tag[i]);
  }
  if (code[5] == checksum) {
    Serial.println(" (OK)");
    return true;
  } else {
    Serial.println(" (invalid)");
    return false;
  }
}

void CardReader::read() {
  byte value = 0;
  byte bytesRead = 0;
  char RFIDdata[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  isValid = false;
  if (_rfid->available() > 0) {
    delay(10);
    if (0x02 == (value = _rfid->read())) {
      while (bytesRead < DATA_SIZE) {
        if (_rfid->available() > 0) {
          value = _rfid->read();

          if ((0x02 == value) || (0x03 == value)) { // STX, ETX
            break;
          }

          if (bytesRead < DATA_SIZE) {
            RFIDdata[bytesRead++] = value;
          }
        }
      }
    }

    isValid = validateData(RFIDdata);
    if (isValid) {
      memcpy(&data, &RFIDdata, 10);
      data[10] = 0;
    }
  }
}
