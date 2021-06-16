#include "DHT.h"                //DHT Bibliothek laden

#define DHTPIN1 21               //Sensoren werden an PIN 21-23 angeschlossen    
#define DHTPIN2 22
#define DHTPIN3 23

#define PWMluft 0                //PWM Pins definieren
#define PWMheiz 1
  
#define DHTTYPE DHT22           //DHT22 Sensor

DHT dht1(DHTPIN1, DHTTYPE);     //Sensorsetup

//DHT dht2(DHTPIN2, DHTTYPE);

//DHT dht3(DHTPIN3, DHTTYPE);

float Kp = 3.0;                     //Proportionalbeiwert
float Ki = 1.0;                     //Integrierbeiwert
float Ta = 2.0;                     //Abtastrate der Sensoren in Sekunden
float w = 22.0;                     //Solltemperatur Heizung in °C
float w2 = 30.0;                    //Solltemperatur Lüfter
float e;
float esum;
float e2;
float esum2;
float y;
int yy;
float y2;
int yy2;
float hum;
float temp;
float hum_krit = 50;              //kritische Luftfeuchtigkeit in % (Heizung & Lüfter an)
float temp_krit = 50;             //kritische Temperatur in °C

void setup() {
  Serial.begin(9600);           //Serielle Verbindung starten

  pinMode(PWMluft, OUTPUT);     //PWMpins als output definieren
  pinMode(PWMheiz, OUTPUT);

  dht1.begin();                 //DHT22 Sensor starten
//  dht2.begin();
//  dht3.begin();

}

void loop() {
  delay(2000);                            //Zwei Sekunden warten bis zur Messung damit der Sensor etwas messen (Abtastrate=2s)

  
  float f1 = dht1.readHumidity();         //Luftfeuchtigkeit auslesen und speichern
  
  float t1 = dht1.readTemperature();      //Temperatur auslesen und speichern

//  float f2 = dht2.readHumidity();
//  
//  float t2 = dht2.readTemperature();
//
//  float f3 = dht3.readHumidity();
//  
//  float t3 = dht3.readTemperature();

  float hum = f1;            
  
  float temp = t1;

  if ((hum > hum_krit) && (temp < temp_krit)) {
    analogWrite(PWMheiz, 255);
    analogWrite(PWMluft, 255);
  }
  else if (t1 > temp_krit) {
    analogWrite(PWMheiz, 0);
    analogWrite(PWMluft, 255);
  }
  else{
    e = w - temp;                            //Regelabweichung bestimmen
    esum = esum + e;                        
    if (esum < 0) esum = 0;          //kumulierten Fehler begrenzen um Überlauf von "int yy" zu verhindern
    if (esum > 2000) esum = 2000;
    y = Kp*e + Ki*Ta*esum;                   //Reglergleichung
    yy = y; 
    if (y < 0) yy = 0;                       //Heizleistung begrenzen durch Begrenzung des maximalen Duty Cycle
    if (y > 255) yy = 255;
  
    analogWrite(PWMheiz, yy);

    e2 = temp - w2;                           //Regelabweichung bestimmen
    esum2 = esum2 + e2;                        
    if (esum2 < 0) esum2 = 0;         //kumulierten Fehler begrenzen um Überlauf von "int yy" zu verhindern
    if (esum2 > 2000) esum2 = 2000;
    y2 = Kp*e2 + Ki*Ta*esum2;                 //Reglergleichung
    yy2 = y2; 
    if (y2 < 0) yy2 = 0;                       
    if (y2 > 255) yy2 = 255;

    analogWrite(PWMluft, yy2);                 //PWM Signal erzeugen
  }

  float pro_heiz = y/255;
  float pro_luft = y2/255;

  Serial.print("t");     
  Serial.print(temp);
  Serial.print(hum);
  Serial.print(pro_heiz,2);
  Serial.print(pro_luft,2);
  
}
