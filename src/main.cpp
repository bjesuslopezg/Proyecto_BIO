#include <ThingerESP32.h>
#include <Arduino.h>

//Conexion a Thinger
#define USERNAME "bjesuslopezg"
#define DEVICE_ID "esp32_wifi"
#define DEVICE_CREDENTIAL "sw%5mqvnqsz%k4K1"
#define SSID "bry"
#define SSID_PASSWORD "olivander"
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#define Th 3000 //Umbral para la medición de bpm
int beat; 

void setup() {
  thing.add_wifi(SSID, SSID_PASSWORD);
  // initialize digital pin 2 as an output.
  // initialize the serial communication:
  Serial.begin(115200);
  Serial.println(); // blank line in serial ...
  pinMode(35, INPUT); // Setup for leads off detection LO +
  pinMode(34, INPUT); // Setup for leads off detection LO -
  pinMode(32, INPUT); // Senal ECG
  //Thinger:
  // resource output example (i.e. reading a sensor value)
  thing["bpm"] >> outputValue(beat);
}

int bpm(){
  thing.handle();
  int beatcounter = 0;
  int detector2 = 0;
  int detector1 = 0;
  int currentTime = millis();
  int senal;
  while(millis() < (currentTime + 10000)){
    //Transformación de la señal en un tren de pulsos
    if(analogRead(32) < Th){
      senal = 0;
    }
    else{
      senal = 1;
    }
    //Detector de pulsos
    detector1 = detector2;
    detector2 = senal;
    if (detector2 == 1 && detector1 == 0)
      beatcounter++;
    delay(1);
  }  
  return beatcounter*6;
}


void loop() {
  if((digitalRead(34) == 1)||(digitalRead(35) == 1)){
    Serial.println('!');
  }
  else{
    beat = bpm();
    Serial.println(beat);
  }
  //Wait a little to keep serial data from saturating
  delay(1);
}