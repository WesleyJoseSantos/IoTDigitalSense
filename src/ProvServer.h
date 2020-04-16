/**
 * @file ProvServer.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief 
 * @version 0.1
 * @date 2020-04-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <FS.h>
#include "ProvData.h"
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PROV_DATA "/provData.json"

bool dataRecieved = false;

enum class ProvServerState{
    turnedOn,
    waitingData,
    dataRecieved,
    provComplete
};

class ProvServer{
private:
    ProvServerState state;
    ESP8266WebServer server;
    void begin();
    bool handleData();
    void saveData();
public:
    ProvData data;
    ProvServer();
    bool readData();
    bool task();
};

ProvServer::ProvServer():server(80){

}

bool ProvServer::readData(){
    if(SPIFFS.exists(PROV_DATA)){
        File file = SPIFFS.open("/manufData.json", "r");
        data = ProvData(file);
        return true ;
    }else{
        return false ;
    }
}

bool ProvServer::task(){
    switch (state)
    {
    case ProvServerState::turnedOn:
        begin();
        state = ProvServerState::waitingData;
        break;
    case ProvServerState::waitingData:
        if(handleData())
            state = ProvServerState::dataRecieved;
        break;
    case ProvServerState::dataRecieved:
        state = WiFi.isConnected() 
            ? ProvServerState::provComplete
            : ProvServerState::turnedOn;
    case ProvServerState::provComplete:
        saveData();
        return true;
        break;
    default:
        break;
    }
    return false;
}

void ProvServer::begin(){
    SPIFFS.begin();
    WiFi.softAP(WiFi.macAddress());
    MDNS.begin("IoTDigitalSense");
    server.begin();
}

bool ProvServer::handleData(){
    
    server.on("/provData", []{
        dataRecieved = true;
    });

    if(dataRecieved){
        server.send(200, "text/plain", "data recieved");
        dataRecieved = false;
        data.ssid = server.arg("ssid");
        data.pass = server.arg("pass");
        data.host = server.arg("host");
        state = ProvServerState::dataRecieved;
        delay(1000);
        WiFi.softAPdisconnect(true);
        delay(1000);
        WiFi.begin(data.ssid, data.pass);
        delay(5000);
        return true;
    }
    return false;
}

void ProvServer::saveData(){
    File file = SPIFFS.open(PROV_DATA, "w");
    file.print(data.toString());
}
