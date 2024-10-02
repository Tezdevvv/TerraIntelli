#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX = 10, TX = 11

int servoDoor = 2;
int relayListrik = 3;
int gasServo = 4;
int tvLCD = 5;

int stateServoDoor;
int stateRelayListrik;
int stateGasServo;
int stateTvLCD;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(servoDoor, OUTPUT);
  pinMode(relayListrik, OUTPUT);
  pinMode(gasServo, OUTPUT);
  pinMode(tvLCD, OUTPUT);
}

void loop() {
  liveDataCommunication();
}

void liveDataCommunication() {
  if (mySerial.available()) {
    String receivedData = mySerial.readStringUntil('\n');
    Serial.print("Data diterima dari ESP32: ");
    Serial.println(receivedData);

    if (receivedData.startsWith("stateServoDoor|")) {
      int state = receivedData.substring(receivedData.indexOf('|') + 1).toInt();
      // Proses stateServoDoor
      Serial.print("Proses stateServoDoor: ");
      Serial.println(state);
      // Contoh aksi: kontrol LED di pin 13
      if (state == 1) {
        digitalWrite(2, HIGH);  // LED nyala jika state = 1, mati jika state = 0
      } else {
        digitalWrite(2, LOW);  // LED nyala jika state = 1, mati jika state = 0
      }
    } else if (receivedData.startsWith("stateTvLCD|")) {
      int state = receivedData.substring(receivedData.indexOf('|') + 1).toInt();
      // Proses stateTvLCD
      Serial.print("Proses stateTvLCD: ");
      Serial.println(state);
    } else if (receivedData.startsWith("stateGasServo|")) {
      int state = receivedData.substring(receivedData.indexOf('|') + 1).toInt();
      // Proses stateGasServo
      Serial.print("Proses stateGasServo: ");
      Serial.println(state);
    } else if (receivedData.startsWith("stateRelayListrik|")) {
      int state = receivedData.substring(receivedData.indexOf('|') + 1).toInt();
      // Proses stateRelayListrik
      Serial.print("Proses stateRelayListrik: ");
      Serial.println(state);
    }
  }

  delay(100);
}


