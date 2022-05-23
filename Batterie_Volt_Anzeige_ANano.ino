#include <SPI.h>
#include <Wire.h>
//Bibliotheken für den betrieb des Displays
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#define V_IN A0
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int v_in = 0;
String volt_s; 

String voltBerechnet(int v_in)
{
  //Alle Konstanten als double definiert damit beim Rechnen int nicht mit float kollidiert
  const double BEREICH    = 1024.0;         //Der Bereich von 0 - 1023 sind 1024 Werte
  const double R1         = 9950.0;         //gemessener Wiederstandswert von R1
  const double R2         = 998.0;          //gemessener Wiederstandswert von R2
  const double V_MAX      = 5.0;            //maximale Spannung die an einem Analog Pin anliegen darf "wenn größer dann bruzzel..." 
  const double AADC       = 0.5;            //korrektur des Arduino ADC Fehlers kuckst du hier https://skillbank.co.uk/arduino/adc.htm für eine ausführliche Erklärung
    
  
  float volt_in = (v_in * V_MAX + AADC) / BEREICH; // Umrechnen von dem Bereichswert in die Spannung die an A0 anliegt
  float volt = volt_in / (  R2/(R1+R2)  );         // Die Spannung von A0 auf die tatsächlich Spannung umrechnen mit der Formel für Spannungsteiler   
  
  
  if (volt < 0.01) volt = 0 ;                      // durch die Korrektur des ADC Fehlers in der Formel wird der Wert immer >0 sein auch bei 0 Spannung
                                                   // deshalb die Korrektur der Korrektur... klingt blöd is aber so... weil (0 x 5.0 + 0.5/ 1024 = 0,00048828125) 
  if (volt < 10)                                   // wenn die berechnete Spannung kleiner 10 Volt ist  
    
    { 
    
    String volt_s = "0" + String(volt) + "V";    // hätte ich gerne als Präfix eine "0" und als Suffix ein "V"
    volt_s.replace(".",",");                      // wir sind in Deutschland Dezimaltrennzeichen ist ein "," kein "." 
    return volt_s;                                // und als Stringvariable volt_s zurückgeben
    
    }
  
  else                                            // wenn die berechnete Spannung größer oder gleich 10 Volt ist (sonst wär sie ja kleiner) 
      
      {
      
      String volt_s = String(volt) + "V";        // hätte ich gerne als Suffix ein "V"
      volt_s.replace(".",",");                    // wir sind in Deutschland Dezimaltrennzeichen ist ein "," kein "." 
      return volt_s;                              // und als Stringvariable volt_s zurückgeben
      
      }
}

void setup() 
{
  display.begin(0x3C);
  delay(1000);
  display.clearDisplay();
  pinMode(V_IN, INPUT);
}
 
void loop() {
  v_in    = analogRead(V_IN);      
  volt_s  = voltBerechnet(v_in);
  delay(1000);
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println(volt_s);
  display.display();
  display.clearDisplay();
}