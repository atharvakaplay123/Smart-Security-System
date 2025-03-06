
bool sim_stat() {
  delay(200);
  Serial.write("AT+CREG?");
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
  int state = 0;
  (digitalRead(IRSENSOR) == 1) ? (state = 1) : (state = 0);
  if (state == 1) {
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
    // Set SMS text mode
    Serial.write("AT+CMGF=1");
    delay(500);

    // Set recipient phone number and message
    Serial.write("AT+CMGS=\"+919406867350\"\r");
    delay(500);

    // Send the message text
    Serial.write("Suspecious Activity found at the security system, please take action");
    delay(500);

    // Send Ctrl+Z to indicate end of message
    Serial.write((char)26);  // ASCII code for Ctrl+Z
    delay(1000);

    Serial.write("ATD+ +919406867350;");
    delay(15000);                           // wait for 15 seconds...
    Serial.write("ATH");                  //hang up

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
