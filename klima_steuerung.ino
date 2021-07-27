#include "DHT.h"                //DHT Bibliothek laden
#define DHTPIN1 21              //Sensoren werden an PIN 21-23 angeschlossen    
#define DHTPIN2 22
#define DHTPIN3 23
#define PWMluft 0               //PWM Pins definieren
#define PWMheiz 1
#define DHTTYPE DHT22           //Sensortyp (DHT22)

DHT dht1(DHTPIN1, DHTTYPE);     //Sensorsetup
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

float Kp = 3.0;                 //Reglerverstaerkung Proportionalanteil
float Ki = 1.0;                 //Reglerverstaerkung Integralanteil
float Ta = 2.0;                 //Abtastrate der Sensoren in Sekunden
float w = 22.0;                 //Solltemperatur Heizung
float w2 = 30.0;                //Solltemperatur Luefter
float e;                        //aktueller Fehler Heizung
float esum;                     //kumulierter Fehler Heizung
float e2;                       //aktueller Fehler Lueftung
float esum2;                    //kumulierter Fehler Lueftung
float y;                        //Ergebnis Reglergleichung Heizung
int yy;
float y2;                       //Ergebnis Reglergleichung Lueftung
int yy2;
float hum_krit = 50;            //kritische Luftfeuchtigkeit
float temp_krit = 50;           //kritische Temperatur

void setup() {
  Serial.begin(9600);           //Serielle Verbindung starten

  pinMode(PWMluft, OUTPUT);     //PWMpins als output definieren
  pinMode(PWMheiz, OUTPUT);

  dht1.begin();                 //DHT22 Sensor starten
  dht2.begin();
  dht3.begin();
}

void loop() {
  delay(2000);                            //2s warten damit der Sensor etwas messen kann (Abtastrate 2s)
  
  float f1 = dht1.readHumidity();         //Luftfeuchtigkeit auslesen
  float t1 = dht1.readTemperature();      //Temperatur auslesen
  float f2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float f3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();

  float hum = (f1+f2+f3)/3;               //Durchschnitt aller Sensoren
  float temp = (t1+t2+t3)/3;

  if ((hum > hum_krit) && (temp < temp_krit)) {     //Heizung & Lüftung an, wenn kritische Luftfeuchtigkeit überschritten und kritische Temperatur noch nicht erreicht
    analogWrite(PWMheiz, 255);
    analogWrite(PWMluft, 255);
  }
  else if ((t1 > temp_krit) || (t2 > temp_krit) || (t3 > temp_krit)) {    //Lüftung an, wenn kritische Temperatur an einem der drei Sensoren überschritten
    analogWrite(PWMheiz, 0);
    analogWrite(PWMluft, 255);
  }
  else{
    e = w - temp;                         //Regelabweichung bestimmen
    esum = esum + e;                      //kumulierten Fehler berechnen  
    if (esum < 0) esum = 0;               //kumulierten Fehler begrenzen
    if (esum > 2000) esum = 2000;
    y = Kp*e + Ki*Ta*esum;                //Reglergleichung
    yy = y;                               //Ergebnis der Reglergleichung in int umwandeln
    if (y < 0) yy = 0;                    //Ergebnis der Reglergleichung auf Wertebereich der analogWrite Funktion begrenzen
    if (y > 255) yy = 255;
    analogWrite(PWMheiz, yy);             //PWM Signal erzeugen

    e2 = temp - w2;                       //Regelabweichung bestimmen
    esum2 = esum2 + e2;                   //kumulierten Fehler berechnen
    if (esum2 < 0) esum2 = 0;             //kumulierten Fehler begrenzen
    if (esum2 > 2000) esum2 = 2000;
    y2 = Kp*e2 + Ki*Ta*esum2;             //Reglergleichung
    yy2 = y2;                             //Ergebnis der Reglergleichung in int umwandeln
    if (y2 < 0) yy2 = 0;                  //Ergebnis der Reglergleichung auf Wertebereich der analogWrite Funktion begrenzen
    if (y2 > 255) yy2 = 255;
    analogWrite(PWMluft, yy2);            //PWM Signal erzeugen
  }
  float duty_heiz = yy;
  float pro_heiz = duty_heiz/255;         //Heizleistung als Wert zwischen 0 und 1
  float duty_luft = yy2;
  float pro_luft = duty_luft/255;         //Luefterleistung als Wert zwischen 0 und 1
  
  Serial.print("t");                       //Zeichen zur Identifikation des Anfangs eines Datenpakets
  Serial.print(temp);                      //Temperatur und
  Serial.print(hum);                       //Luftfeuchtigkeit auf seriellen Port schreiben
  Serial.print(pro_heiz,2);                //Heiz- und
  Serial.println(pro_luft,2);              //Luefterleistung (0 bis 1) mit 2 Nachkommastellen
}
