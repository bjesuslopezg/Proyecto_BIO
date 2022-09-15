#include "BluetoothSerial.h"


BluetoothSerial ESP_BT; // Object for Bluetooth
//https://how2electronics.com/iot-ecg-monitoring-ad8232-sensor-esp32/

// global vars
boolean BT_cnx = false;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
    BT_cnx = true;
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected");
    BT_cnx = false;
    ESP.restart();
  }
}

void setup() {
  // initialize digital pin 2 as an output.
  // initialize the serial communication:
  Serial.begin(115200);
  Serial.println(); // blank line in serial ...
  pinMode(35, INPUT); // Setup for leads off detection LO +
  pinMode(34, INPUT); // Setup for leads off detection LO -
  pinMode(32, INPUT); // Senal ECG
  // initialize the serial BT communication:
  ESP_BT.register_callback(callback);
  if(!ESP_BT.begin("ESP32_ECG")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized... Bluetooth Device is Ready to Pair...");
  }
}

#define Th 3000


int bpm(){
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
  int beat;
  if((digitalRead(34) == 1)||(digitalRead(35) == 1)){
    Serial.println('!');
    ESP_BT.println('!');
  }
  else{
    beat = bpm();
    Serial.println(beat);
    //Do the same for blutooth
    if(BT_cnx){
      ESP_BT.print('E'); //make the app Blutooth Graphics (https://play.google.com/store/apps/details?id=com.emrctn.BluetoothGraphics&hl=en_US) work (as specified by the app)
      ESP_BT.println(beat);
    } 
  }
  //Wait a little to keep serial data from saturating
  delay(1);
}