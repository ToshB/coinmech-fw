#include "Display.h"
#include <LiquidCrystal_I2C_Spark.h>

Display::Display(int addr) {
  _lcd = new LiquidCrystal_I2C(addr, 16, 2);
  _lcd->init();
  _lcd->backlight();
  showInit();
}

void Display::turnOff() {
  _lcd->noBacklight();
}

void Display::showInit() {
  time_t time = Time.now();
  _lcd->backlight();
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Coinmech v1.0.0");
  _lcd->setCursor(0, 1);
  _lcd->print("Build");
  _lcd->setCursor(6, 1);
  _lcd->print(__TIME__);
}

void Display::showBalance(char *cardId, char *balance, char *name) {
  _lcd->backlight();
  _lcd->clear();
  _lcd->setCursor(0, 0);

  if (strlen(name) > 0) {
    _lcd->print(name);
  }else{
    _lcd->print("Card:");
    _lcd->setCursor(6, 0);
    _lcd->print(cardId);
  }

  _lcd->setCursor(0, 1);
  _lcd->print("Balance:");

  _lcd->setCursor(9, 1);
  _lcd->print(balance);
}

void Display::showReadCard(String cardId) {
  _lcd->backlight();
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Card: ");

  _lcd->setCursor(6, 0);
  _lcd->print(cardId);

  _lcd->setCursor(0, 1);
  _lcd->print("Please wait..");
}

void Display::showError(char *error) {
  _lcd->backlight();
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Error: ");
  _lcd->setCursor(0, 1);
  _lcd->print(error);
}

void Display::showMessage(String l1, String l2) {
  _lcd->backlight();
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print(l1);
  _lcd->setCursor(0, 1);
  _lcd->print(l2);
}
