void Warning() {
  int state = 0;
  (analogRead(A0) >= 500) ? (state = 1) : (state = 0);
  if (state == 1) {
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
    Serial.println("ATD+ +919406867350;");
    delay(15000);         // wait for 15 seconds...
    Serial.println("ATH");  //hang up
    delay(2000);
    /*
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
      Serial.println((char)26);  // ASCII code for Ctrl+Z
    */
    while (true) {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_R, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_R, LOW);
      delay(200);
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_R, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_R, LOW);
    }
  }
}
