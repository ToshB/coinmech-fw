/*
 * Project coinmech-fw
 * Description:
 * Author: Torstein Bjørnstad
 */

#include "CardReader.h"
#include "Display.h"
#include "notes.h"
#include "util.h"

const int BUTTON = A6;
bool buttonPressed = true;
CardReader *cardReader;
Display *display;

bool hasCard = false;
String currentCard;
unsigned long forgetCardAt = 0;

void setup() {
  buttonPressed = true;
  pinMode(BUTTON, INPUT_PULLUP);
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
  char *cardId = strtok(dataStr, ";");
  char *balance = strtok(NULL, ";");
  char *name = strtok(NULL, ";");
  display->showBalance(cardId, balance, name);
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

void checkButton() {
  int buttonState = digitalRead(BUTTON);
  if (buttonState == LOW) {
    buttonPressed = false;
  } else if (buttonState == HIGH && !buttonPressed) {
    buttonPressed = true;
    updateCard("test-event");
  }
}


void loop() {
  unsigned long now = millis();
  checkCard();
  checkButton();
  if (hasCard && now > forgetCardAt) {
    hasCard = false;
    forgetCard();
	}
}
