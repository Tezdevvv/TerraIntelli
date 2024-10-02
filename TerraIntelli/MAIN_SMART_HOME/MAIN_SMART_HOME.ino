#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <BluetoothSerial.h>
#include <HardwareSerial.h>

BluetoothSerial SerialBT;

HardwareSerial SerialESP(2);

String receivedData = "";
String ssid = "";
String password = "";
String receivedChar;

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyAiPxETnQLlfOf5NpudFCtk8g_OZzTqDkU"
#define DATABASE_URL "https://terra-intelli-default-rtdb.firebaseio.com/" 

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String sValue, sValue2;
bool signupOK = false;
bool stateConnect = false;

int led1 = 13;
int led2 = 12;
int led3 = 14;
int buzzer1 = 27;
int buzzer2 = 26;
int buzzer3 = 25;

int stateAfterEQ = 0;

int stateServoDoor;
int stateRelayListrik;
int stateGasServo;
int stateTvLCD;

int currentState = 0; // Indikator state yang akan dikirim
bool waitForAck = false; // Menunggu konfirmasi dari Arduino

void setup() {
  Serial.begin(115200);
  SerialESP.begin(9600, SERIAL_8N1, 16, 17);
  SerialBT.begin("TerraIntelli CONNECT");
  Serial.println("Bluetooth started! Waiting for connection...");
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(buzzer3, OUTPUT);
}

void loop() {
  connectWithBluetooth();

  if (stateConnect) {
     Firebase.RTDB.setString(&fbdo, "TerraIntelli/ConnectionState", "1");
  }

  if (Firebase.ready() && signupOK) {

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/DoorState")) {
      int doorState = fbdo.stringData().toInt();
      Serial.print("Door State: ");
      Serial.println(doorState);
      stateServoDoor = doorState;
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/Lamp1State")) {
      int lamp1State = fbdo.stringData().toInt();
      Serial.print("Lamp 1 State: ");
      Serial.println(lamp1State);
      if (lamp1State == 1) {
        digitalWrite(led1, HIGH);
      } else {
        digitalWrite(led1, LOW);
      }
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/Lamp2State")) {
      int lamp2State = fbdo.stringData().toInt();
      Serial.print("Lamp 2 State: ");
      Serial.println(lamp2State);
      if (lamp2State == 1) {
        digitalWrite(led2, HIGH);
      } else {
        digitalWrite(led2, LOW);
      }
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/Lamp3State")) {
      int lamp3State = fbdo.stringData().toInt();
      Serial.print("Lamp 3 State: ");
      Serial.println(lamp3State);
      if (lamp3State == 1) {
        digitalWrite(led3, HIGH);
      } else {
        digitalWrite(led3, LOW);
      }
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/TvState")) {
      int tvState = fbdo.stringData().toInt();
      Serial.print("TV State: ");
      Serial.println(tvState);
      stateTvLCD = tvState;
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/EarthQuake")) {
      int earthQuakeState = fbdo.stringData().toInt();
      Serial.print("Earthquake State: ");
      Serial.println(earthQuakeState);
      if (earthQuakeState == 1) {
          digitalWrite(buzzer1, HIGH);
          digitalWrite(buzzer2, HIGH);
          digitalWrite(buzzer3, HIGH);
          stateAfterEQ = 1;
      } else if (earthQuakeState == 2) {
          digitalWrite(buzzer1, HIGH);
          digitalWrite(buzzer2, HIGH);
          digitalWrite(buzzer3, HIGH);
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "1");
          stateAfterEQ = 2;
      } else if (earthQuakeState == 3) {
          digitalWrite(buzzer1, HIGH);
          digitalWrite(buzzer2, HIGH);
          digitalWrite(buzzer3, HIGH);
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/gasServoState", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/relayListrik", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/DoorState", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/relayListrik", "1");
          stateAfterEQ = 3;
      } else if (earthQuakeState == 4) {
          digitalWrite(buzzer1, HIGH);
          digitalWrite(buzzer2, HIGH);
          digitalWrite(buzzer3, HIGH);
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/gasServoState", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/relayListrik", "1");
          Firebase.RTDB.setString(&fbdo, "/TerraIntelli/DoorState", "1");
          stateAfterEQ = 4;
      } else if (earthQuakeState == 0) {
          if (stateAfterEQ == 1) {
            digitalWrite(buzzer1, LOW);
            digitalWrite(buzzer2, LOW);
            digitalWrite(buzzer3, LOW);
            stateAfterEQ = 0;
          } else if (stateAfterEQ == 2) {
            digitalWrite(buzzer1, LOW);
            digitalWrite(buzzer2, LOW);
            digitalWrite(buzzer3, LOW);
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "0");
            stateAfterEQ = 0;
          } else if (stateAfterEQ == 3) {
            digitalWrite(buzzer1, LOW);
            digitalWrite(buzzer2, LOW);
            digitalWrite(buzzer3, LOW);
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/gasServoState", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/relayListrik", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/DoorState", "0");
            stateAfterEQ = 0;
          } else if (stateAfterEQ == 4) {
            digitalWrite(buzzer1, LOW);
            digitalWrite(buzzer2, LOW);
            digitalWrite(buzzer3, LOW);
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp1State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp2State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/Lamp3State", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/gasServoState", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/relayListrik", "0");
            Firebase.RTDB.setString(&fbdo, "/TerraIntelli/DoorState", "0");
            stateAfterEQ = 0;
          }
      }
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/FireState")) {
      int fireState = fbdo.stringData().toInt();
      Serial.print("Fire State: ");
      Serial.println(fireState);
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/gasServoState")) {
      int gasServoState = fbdo.stringData().toInt();
      Serial.print("Gas State: ");
      Serial.println(gasServoState);
      stateGasServo = gasServoState;
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/relayListrik")) {
      int relayListrik = fbdo.stringData().toInt();
      Serial.print("Listrik State: ");
      Serial.println(relayListrik);
      stateRelayListrik = relayListrik;
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, "TerraIntelli/waterLevelState")) {
      int waterLevelState = fbdo.stringData().toInt();
      Serial.print("Water Level State: ");
      Serial.println(waterLevelState);
    } else {
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
    }

    liveDataCommunication();
  }
}

void liveDataCommunication() {
  SerialESP.println("stateServoDoor|" + String(stateServoDoor));
  delay(20);
  SerialESP.println("stateTvLCD|" + String(stateTvLCD));
  delay(20);
  SerialESP.println("stateGasServo|" + String(stateGasServo));
  delay(20);
  SerialESP.println("stateRelayListrik|" + String(stateRelayListrik));
  delay(20);

  if (SerialESP.available()) {
    int data = SerialESP.readStringUntil('\n').toInt();
    Serial.print("Data dari Arduino: ");
    Serial.println(data);
  }

  delay(20);
}

void connectWithFirebase() {
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Connect");
    signupOK = true;
    stateConnect = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    stateConnect = false;
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void connectWithBluetooth() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');

    receivedChar = command;
    Serial.println(receivedChar);

    receivedData += receivedChar;

    if (receivedChar != "") {
      Serial.print("Data yang diterima: ");
      Serial.println(receivedData);

      int separatorIndex = receivedData.indexOf('|');
      if (separatorIndex != -1) {
        ssid = receivedData.substring(0, separatorIndex);
        password = receivedData.substring(separatorIndex + 1);

        ssid.trim();
        password.trim();

        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Password: ");
        Serial.println(password);

        connectToWifi(ssid, password);
      } else {
        Serial.println("Format data salah. Gunakan format: SSID|password");
      }
      receivedData = "";
    }
  }

  delay(500);
}

void connectToWifi(String ssid, String password) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
    delay(1000);
  }

  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to Wi-Fi");
  int numberOfTries = 20;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    numberOfTries--;
    if (numberOfTries == 0) {
      Serial.println();
      Serial.println("Failed to connect to Wi-Fi");
      return;
    }
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  connectWithFirebase();
}
