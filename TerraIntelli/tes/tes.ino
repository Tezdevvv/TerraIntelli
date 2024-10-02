#include <HardwareSerial.h>

HardwareSerial SerialESP(2);
int stateLed = 0;

void setup() {
  Serial.begin(115200);
  SerialESP.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  liveDataCommunication();
}

void liveDataCommunication() {
  SerialESP.println("Tes");
  Serial.print("Mengirim ke Arduino: ");
  Serial.println("Tes");

  if (SerialESP.available()) {
    int data = SerialESP.readStringUntil('\n').toInt();
    Serial.print("Data dari Arduino: ");
    Serial.println(data);

    stateLed = data;

    if (stateLed == 0) {
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
    } else {
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
    }
  }

  delay(1000);
}

