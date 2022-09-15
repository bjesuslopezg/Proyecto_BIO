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
  pinMode(32, INPUT); //Senal ECG
  // initialize the serial BT communication:
  ESP_BT.register_callback(callback);
  if(!ESP_BT.begin("ESP32_ECG")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized... Bluetooth Device is Ready to Pair...");
  }
}

void loop() {
  if((digitalRead(34) == 1)||(digitalRead(35) == 1)){
    Serial.println('!');
    ESP_BT.println('!');
  }
  else{
    // send the value of analog input 0 to serial:
    Serial.println(analogRead(32));
    //Do the same for blutooth
    if(BT_cnx){
      ESP_BT.print('E'); //make the app Blutooth Graphics (https://play.google.com/store/apps/details?id=com.emrctn.BluetoothGraphics&hl=en_US) work (as specified by the app)
      ESP_BT.println(analogRead(32));
    } 
  }
  //Wait a little to keep serial data from saturating
  delay(1);
}