#ifndef Display_h
#define Display_h

#include "Particle.h"
#include <LiquidCrystal_I2C_Spark.h>

class Display {
private:
  LiquidCrystal_I2C *_lcd;
public:
  Display(int);
  void showBalance(char*, char*, char*);
  void showReadCard(String);
  void showError(char*);
  void showMessage(String, String);
  void showInit();
  void turnOff();
};

#endif
