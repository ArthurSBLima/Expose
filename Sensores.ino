#include <math.h>

#define tdsPin 34
#define turbPin 32
#define ph_poPin 33
#define ph_doPin 14
#define ph_toPin 35 

#define adc_m 4095.0
#define v_ref 3.3


#define R_FIXED 10000.0
#define R0 10000.0
#define BETA 3950.0
#define T0K 298.15


float ph_v7 = 2.500;
float ph_v4 = 3.000;


void setup() {
  Serial.begin(9600);
  analogReadResolution(12);
  analogSetPinAttenuation(tdsPin, ADC_11db); //
  analogSetPinAttenuation(turbPin, ADC_11db);
  analogSetPinAttenuation(ph_poPin, ADC_11db);
  analogSetPinAttenuation(ph_toPin, ADC_11db);

  pinMode(ph_doPin, INPUT);
}

float ADCmedio(int pin, int n){
  long s = 0;
  for(int i = 0; i < n; i++){
    s += analogRead(pin);
  }
  return s/(float)n;
}

void loop() {
  const int n = 50;
  //Ph e  Temperatura 
  float aTO = ADCmedio(ph_toPin, n);
  float vTO = (aTO/adc_m)*v_ref;
  if(vTO<=0.01){
     vTO=0.01;
  }             
  if(vTO>=v_ref-0.01){
    vTO=v_ref-0.01; 
  }
  float rNTC = R_FIXED*(vTO/(v_ref-vTO));

  float tk = 1.0/(1.0/T0K+(1.0/BETA)*log(rNTC/R0));
  float temperature = tk - 273.15;

  float aPH = ADCmedio(ph_poPin, n);
  float vPH = (aPH/adc_m)*v_ref; 

  float m =(7.0-4.0)/(ph_v7-ph_v4);
  float mT = m*((tk)/T0K);
  float b = 7.0-mT*ph_v7;
  float ph = mT*vPH+b;
  int alarme_ph = digitalRead(ph_doPin);

  //Total de sólidos dissolvidos 
  
  float aTDS = ADCmedio(tdsPin, n);
  float vTDS = (aTDS/adc_m)*v_ref;
  float ec = 133.42*vTDS*vTDS*vTDS-255.86*vTDS*vTDS+857.39*vTDS;
  ec = ec/(1.0+0.02*(temperature-25.0));
  float tds=ec*0.5;

  //Turbidez
  float aTURB = ADCmedio(turbPin, n);
  float vTURB = (aTURB/adc_m)*v_ref; 
  float ntu = -1120.4*vTURB*vTURB+5742.3*vTURB-4352.9;
  if(ntu < 0){
    ntu = 0;
  }

  Serial.print("T(°C): ");Serial.print(temperature,2);Serial.print(" || ");
  Serial.print("pH_V: ");Serial.print(vPH,3);Serial.print(" | pH: ");Serial.print(ph,2);Serial.print(" | DO: ");Serial.print(alarme_ph);Serial.print(" || ");
  Serial.print("EC(uS/cm): ");Serial.print(ec,1);Serial.print(" | TDS(ppm): ");Serial.print(tds,0);Serial.print(" || ");
  Serial.print("NTU: ");Serial.println(ntu,1);

  delay(500);

}
