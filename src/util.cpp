#include "util.h"
#include "notes.h"

const int COIN = A0;
const int BUZZER = A7;

void initBuzzer(){
  pinMode(BUZZER, OUTPUT);
}

void triggerCoin() {
  digitalWrite(COIN, HIGH);
  delay(50);
  digitalWrite(COIN, LOW);
}

void readSound() {
  tone(BUZZER, NOTE_C8);
  delay(100);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, NOTE_C8);
  delay(200);
  noTone(BUZZER);
}
