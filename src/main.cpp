#include "main.h"

manufData_t manufData;
HTTPClient httpIoTSense;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  Serial.println("IoT Digital Sense Started.");
  iotSenseInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkSample();
}

void iotSenseInit(){
  SPIFFS.begin();
  File file = SPIFFS.open("/manufData.json", "r");
  StaticJsonDocument<256> doc;
  deserializeJson(doc, file);

  manufData.wifiSsid = doc["wifiSsid"];
  manufData.wifiPass = doc["wifiPass"];
  manufData.appHost  = doc["appHost"];
  manufData.senseName = doc["senseName"];
  manufData.gpioStatusInput = doc["gpioStatusInput"];
  manufData.gpioStatusOutput = doc["gpioStatusOutput"];

  Serial.printf("ssid: %s\n pass: %s\n", manufData.wifiSsid, manufData.wifiPass);
  WiFi.mode(WIFI_STA);
  WiFi.begin(manufData.wifiSsid, manufData.wifiPass);
  while (!WiFi.isConnected())
  {
    Serial.println("Waiting wifi...");
    delay(1000);
  }
  Serial.printf("connected to IP: "); Serial.println(WiFi.localIP());

  httpIoTSense.begin(manufData.appHost);

  pinMode(manufData.gpioStatusInput, INPUT_PULLUP);
  pinMode(manufData.gpioStatusOutput, OUTPUT);
}

void iotSenseWork(){
  static int t = 0;
  t++;
  StaticJsonDocument<256> doc;

  doc["senseMac"] = WiFi.macAddress();
  doc["senseLifeTime"] = t;
  doc["senseRssi"] = WiFi.RSSI();
  doc["senseStatus"] = !digitalRead(manufData.gpioStatusInput);

  String payload;
  serializeJson(doc, payload);

  httpIoTSense.addHeader("Content-Type", "application/json");
  int errCode = httpIoTSense.POST(payload);

  Serial.printf("\nSense data sended to %s\n", manufData.appHost);
  Serial.printf("Data: %s\n", payload.c_str());
  Serial.printf("errCode: %i\n", errCode);

  static int cont = 0;
  if(errCode != HTTP_CODE_OK){
    delay(500);
    cont ++;
    if(cont > 5){
    ESP.restart();
    }
  }else{
    cont = 0;
    iotSenseProcessRequest();
    delay(1000);
  } 
}

void iotSenseProcessRequest(){
  String req = httpIoTSense.getString();
  Serial.printf("req: %s\n", req.c_str());
}

void blinkSample(){
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(1000);                      // Wait for a second
  Serial.println("led off");
  delay(250);                      // Wait for one-quarter of second

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  Serial.println("led on");
  delay(250);   
}