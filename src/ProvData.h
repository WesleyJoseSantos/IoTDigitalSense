/**
 * @file ProvData.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief 
 * @version 0.1
 * @date 2020-04-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "ArduinoJson.h"

class ProvData
{
private:
public:
    ProvData(){}
    ProvData(Stream &source);
    ProvData(String ssid, String pass, String host);
    String ssid;
    String pass;
    String host;
    String toString();
};

ProvData::ProvData(Stream &source)
{
    StaticJsonDocument<128> json;
    deserializeJson(json, source);
    ssid = json["ssid"].as<String>();
    pass = json["pass"].as<String>();
    host = json["host"].as<String>();
}

String ProvData::toString(){
    StaticJsonDocument<128> json;
    json["ssid"] = ssid;
    json["pass"] = pass;
    json["host"] = host;
    String str;
    serializeJson(json, str);
    return(str);
}