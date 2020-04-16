/**
 * @file SenseData.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief 
 * @version 0.1
 * @date 2020-04-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "ArduinoJson.h"

class SenseData
{
private:
    
public:
    SenseData(){}
    SenseData(String mac, int power, bool status);
    String mac;
    int power;
    bool status;

    String toJson();
};

SenseData::SenseData(String mac, int power, bool status){
    this->mac = mac;
    this->power = power;
    this->status = status;
}

String SenseData::toJson(){
    StaticJsonDocument<128> doc;
    doc["mac"] = mac;    
    doc["power"] = power;
    doc["status"] = status;    
    String ret;
    serializeJson(doc, ret);
    return(ret);
}