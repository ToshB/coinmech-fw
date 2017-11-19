/*
 * Project coinmech-fw
 * Description:
 * Author: Torstein Bjørnstad
 */

#include "CardReader.h"
#include "Display.h"
#include "notes.h"
#include "util.h"

CardReader *cardReader;
Display *display;

bool hasCard = false;
String currentCard;
unsigned long forgetCardAt = 0;

STARTUP(RGB.mirrorTo(WKP, A5, A4, true, true));

void setup() {
  initBuzzer();
  display = new Display(0x3F);
  cardReader = new CardReader(NULL, NULL);
  System.on(firmware_update, firmwareUpdateHandler);
  Serial.begin(9600);

  Particle.subscribe("hook-response/cardScanned", scanHandler, MY_DEVICES);
  Particle.subscribe("hook-response/scanError", scanErrorHandler, MY_DEVICES);
}

void scanHandler(const char *event, const char *data) {
  Serial.print("Received: ");
  Serial.println(data);

  char *dataStr = strdup(data);
  char *success = strtok(dataStr, ";");
  char *cardId = strtok(NULL, ";");
  char *balance = strtok(NULL, ";");
  char *name = strtok(NULL, ";");
  display->showBalance(cardId, balance, name);
  Serial.print(success);
  if(strcmp(success, "true") == 0){
    Serial.print("coin!");
    triggerCoin();
  }else{
    errorSound();
  }

  RGB.control(false);
  scheduleCardForget();
}

void scanErrorHandler(const char *event, const char *data) {
  char *dataStr = strdup(data);
  Serial.print("Error: ");
  Serial.println(data);
  display->showError(dataStr);
  RGB.control(false);
}

void forgetCard(){
  currentCard = "";
  hasCard = false;
  display->showMessage("-- Scan Card -- ", "");
  display->turnOff();
}

void scheduleCardForget() {
  forgetCardAt = millis() + 3000;
}

void updateCard(String newCard) {
  if (newCard.equals(currentCard)) {
    Serial.println("Duplicate scan");
    return;
  }
  RGB.control(true);
  RGB.color(255, 0, 255); // MAGENTA

  Serial.println("New card scanned: " + newCard);
  display->showReadCard(newCard);
  Particle.publish("cardScanned", newCard, PRIVATE);
  currentCard = newCard;
  hasCard = true;
  readSound();
  scheduleCardForget();

}

void checkCard() {
  String newCard;
  cardReader->read();
  if (cardReader->isValid) {
    updateCard(cardReader->data);
  }
}

void firmwareUpdateHandler(system_event_t event, int param, void *) {
  display->showMessage("Receiving update", "Flashing..");
}


void loop() {
  unsigned long now = millis();
  checkCard();
  if (hasCard && now > forgetCardAt) {
    hasCard = false;
    forgetCard();
	}
}
