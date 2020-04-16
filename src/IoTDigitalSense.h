/**
 * @file IoTDigitalSense.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief 
 * @version 0.1
 * @date 2020-04-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "ProvServer.h"
#include "SenseOperation.h"

enum class IoTDigitalSenseState{
    turnedOn,
    provNeeded,
    working,
    error
};

class IoTDigitalSense{
private:
    IoTDigitalSenseState state;
    ProvServer prov;
    SenseOperation sense;
public:
    IoTDigitalSense();
    void task();
};

IoTDigitalSense::IoTDigitalSense(){

}

void IoTDigitalSense::task(){
    switch (state)
    {
    case IoTDigitalSenseState::turnedOn:
        state = prov.readData() 
            ? IoTDigitalSenseState::working 
            : IoTDigitalSenseState::provNeeded;
        break;
    case IoTDigitalSenseState::provNeeded:
        state = prov.task()
            ? IoTDigitalSenseState::working 
            : IoTDigitalSenseState::provNeeded;
    case IoTDigitalSenseState::working:
        sense.task();
        break;
    case IoTDigitalSenseState::error:
        state = IoTDigitalSenseState::provNeeded;
    default:
        break;
    }
}