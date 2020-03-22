#if defined(__AVR_ATmega328P__)

// 328P feather
#define VBATPIN A6

float getBatteryVoltage() {

  float measuredvbat = analogRead(VBATPIN);

  measuredvbat *= 2.0f;    // we divided by 2, so multiply back
  measuredvbat *= 3.3f;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024.0f; // convert to voltage

  return measuredvbat;

}
#else

// unknown platform
float getBatteryVoltage() {
  Serial.println("warning: unknown feather. getting battery voltage failed.");
  return 0.0f;
}
#endif
