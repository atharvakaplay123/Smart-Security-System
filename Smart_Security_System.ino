#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

#define RST_PIN D3
#define SS_PIN D4

#define BUZZER D2
#define IRSENSOR A0
#define LED_R D1
#define LED_G D0
#define Relay_pin D8

String RFID;
bool door_stat = 0;  // door_stat = 1(door is open with valid RFID card), door_stat = 0(door is closed)


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
ESP8266WiFiMulti WiFiMulti;
MFRC522::StatusCode status;

/* Be aware of Sector Trailer Blocks */
int blockNum = 2;

/* Create another array to read data from Block */
/* Legthn of buffer should be 2 Bytes more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];

String data2;
const String data1 = "https://script.google.com/macros/s/AKfycbwysfjP_i8Fu2FnF6qgupFJjn03AptaBXQonkTRzoPLkO3b0Ny1a2UJH3W4HMqlsM-i/exec?name=";

void setup() {
  Serial.begin(9600);
  // Serial.setDebugOutput(true);
  // Serial.println();
  // Serial.println();
  // Serial.println();
  Serial.println("AT");
  delay(100);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(Relay_pin, OUTPUT);
  pinMode(IRSENSOR, INPUT);

  digitalWrite(BUZZER, LOW);
  digitalWrite(Relay_pin, LOW);

  // for (uint8_t t = 4; t > 0; t--) {
  //   Serial.printf("[SETUP] WAIT %d...\n", t);
  //   Serial.flush();
  //   delay(1000);
  // }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Vijay_Jio", "Kaplay@108");
  WiFiMulti.addAP("Atharva_Kaplay", "Kaplay@108");
  while (WiFiMulti.run() != WL_CONNECTED || sim_stat() == false) {
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
    delay(100);
    digitalWrite(LED_R, LOW);
    delay(100);
  }
  if (WiFiMulti.run() == WL_CONNECTED && sim_stat() == true) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
  }
  SPI.begin();
}

void loop() {
  if (WiFiMulti.run() != WL_CONNECTED || sim_stat() == false) {
    while (WiFiMulti.run() != WL_CONNECTED || sim_stat() == false) {
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, HIGH);
      delay(100);
      digitalWrite(LED_R, LOW);
      delay(100);
    }
    if (WiFiMulti.run() == WL_CONNECTED && sim_stat() == true) {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
    }
  }
  
  if (digitalRead(IRSENSOR) == 1) {
    Warning();
  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  /* Select one of the cards */
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  /* Read data from the same block */
  // Serial.println();
  // Serial.println(F("Reading last data from RFID..."));
  ReadDataFromBlock(blockNum, readBlockData);
  /* If you want to print the full memory dump, uncomment the next line */
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  /* Print the data read from block */
  // Serial.println();
  // Serial.print(F("Last data in RFID:"));
  // Serial.print(blockNum);
  // Serial.print(F(" --> "));
  RFID.remove(0, RFID.length());
  int j = 0;
  while (int(readBlockData[j]) > 0) {
    RFID.concat(char(readBlockData[j]));
    j++;
  }
  // Serial.println(RFID);
  door_stat == 1;
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  delay(200);
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_G, LOW);


  if ((WiFiMulti.run() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    //client->setFingerprint(fingerprint);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
    client->setInsecure();

    data2 = data1 + RFID;
    data2.trim();
    // Serial.println(data2);

    HTTPClient https;
    // Serial.print(F("[HTTPS] begin...\n"));
    if (https.begin(*client, (String)data2)) {
      //HTTP
      // Serial.print(F("[HTTPS] GET...\n"));
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        // file found at server
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_R, HIGH);
        delay(200);
        digitalWrite(LED_R, LOW);
        delay(200);
        digitalWrite(LED_R, HIGH);
        delay(200);
        digitalWrite(LED_R, LOW);
      } else {
        // Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_R, HIGH);
      }
      https.end();
      delay(1000);
    } else {
      // Serial.printf("[HTTPS} Unable to connect\n");
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, HIGH);
    }
  }
  int a = digitalRead(IRSENSOR);
  if (a == 1) {
    while (a == 1) {
      digitalWrite(LED_G, LOW);
      digitalWrite(Relay_pin, HIGH);
      a = digitalRead(IRSENSOR);
    }
  }
  digitalWrite(LED_G, HIGH);
  digitalWrite(Relay_pin, LOW);
}
