#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
String text = "Tidak";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  liveDataCommunication();
}

void liveDataCommunication() {
    if (mySerial.available()) {
    int data = mySerial.readStringUntil('\n').toInt();
    Serial.print("Data dari ESP32: ");
    Serial.println(data);

    if (digitalRead(2) == 0) {
      mySerial.println(1);
      Serial.print("Mengirim ke ESP32: ");
      Serial.println(1);
    } else {
      mySerial.println(0);
      Serial.print("Men0irim ke ESP32: ");
      Serial.println(0);
    }

  }

  delay(1000);
}
