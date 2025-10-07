int pin = 33; // seu pino ADC
void setup(){
  Serial.begin(115200);
  analogReadResolution(12);                 // 0..4095
  analogSetPinAttenuation(pin, ADC_11db);  // leitura estável até ~3.3 V
}
void loop(){
  int raw = analogRead(pin);
  float v_esp = raw * (3.3 / 4095.0); // Volts no pino do ESP (depois do divisor)
  Serial.print("V_ESP: ");
  Serial.println(v_esp, 3);
  delay(300);
}