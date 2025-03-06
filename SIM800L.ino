
bool sim_stat() {
  delay(200);
  Serial.println("AT+CREG?");

  unsigned long startTime = millis();    // Record the start time
  while (millis() - startTime < 1000) {  // Wait for a maximum of 1 second
    if (Serial.available()) {
      String a = Serial.readStringUntil('\n');
      a.trim();  // Remove any leading/trailing whitespace
      if (a == "+CREG: 0,1" || a == "+CREG: 0,5") {
        return true;
      } else {
        return false;
      }
    }
  }
  // If we didn't receive anything within the timeout period, return false
  return false;
}

void Warning() {
  int val = digitalRead(IRSENSOR);
  int state = 0;
  (val == 1) ? (state = 1) : (state = 0);
  // int state = 0;
  // int old_val = 0;
  // if (val == HIGH && old_val == LOW) {
  //   state = 1 - state;
  //   delay(10);
  // }
  // old_val = val;
  if (state == 1) {
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
    // Set SMS text mode
    Serial.println("AT+CMGF=1");
    delay(500);

    // Set recipient phone number and message
    Serial.println("AT+CMGS=\"+919406867350\"\r");
    delay(500);

    // Send the message text
    Serial.println("Suspecious Activity found at the security system, please take action");
    delay(500);

    // Send Ctrl+Z to indicate end of message
    Serial.write((char)26);  // ASCII code for Ctrl+Z
    delay(1000);
    Serial.println("ATD+ +919406867350;");  //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
    delay(20000);                           // wait for 20 seconds...
    Serial.println("ATH");                  //hang up
    while (true) {
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
    }
  }
}
