/**
 * @file SenseOperation.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief 
 * @version 0.1
 * @date 2020-04-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "SenseData.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

class SenseOperation{
private:
    HTTPClient client;
public:
    SenseData senseData;
    SenseOperation();
    void begin();
    void task();
};

SenseOperation::SenseOperation(){
}

void SenseOperation::begin(){

}

void SenseOperation::task(){

}