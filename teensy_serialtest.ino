#include "DHT.h"                

#define DHTPIN1 21 
#define DHTTYPE DHT22
DHT dht1(DHTPIN1, DHTTYPE); 

void setup() {
  Serial.begin(9600);  
  dht1.begin();   
}
void loop() {
  delay(2000);                            
  
  float f1 = dht1.readHumidity();         
  
  float t1 = dht1.readTemperature();  
  
  hum = f1;            
  temp = t1;
  byte sendtemp = temp;
  
  if (Serial.available()){
  Serial.write(sendtemp);
  }
}
