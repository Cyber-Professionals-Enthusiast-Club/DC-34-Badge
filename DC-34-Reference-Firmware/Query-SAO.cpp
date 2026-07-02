#include "Query-SAO.h" 
#include <Wire.h>
#include <Adafruit_MAX1704X.h>
#include "Adafruit_seesaw.h"

query_sao_collection_t query_sao(void)
{
    const int sda_pin = 4;
    const int scl_pin = 5;
  
    query_sao_collection_t returnVal = {QUERY_SAO_NONE, QUERY_SAO_NONE, QUERY_SAO_NONE, 0};
    Adafruit_seesaw sao0;
    Adafruit_seesaw sao1; 
    Adafruit_seesaw sao2;
    Adafruit_MAX17048 maxlipo;
    uint32_t pinRead = 0x3F;
    byte error;
     
    int address = 0x49;

    WIRE.setPins(sda_pin, scl_pin);
    WIRE.begin();

    // Get cell voltage
    maxlipo.begin(&WIRE);
    returnVal.chipID = maxlipo.getChipID();
    maxlipo.cellPercent();
    
    //WIRE.setClock(10000);
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();
    if (error == 0)
    {
        if(sao0.begin(address))
        {
            sao0.pinModeBulk(pinRead, INPUT);
            sao0.pinMode(10, OUTPUT);
            uint32_t pinStatus = sao0.digitalReadBulk(pinRead);
            returnVal.SAO0 = (query_sao_return_t)(pinStatus + 1);
            sao0.digitalWrite(10, LOW);
        }
    }
    address += 1;
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();
    if (error == 0)
    {
        if(sao1.begin(address))
        {
            sao1.pinModeBulk(pinRead, INPUT);
            sao1.pinMode(10, OUTPUT);
            uint32_t pinStatus = sao1.digitalReadBulk(pinRead);
            returnVal.SAO1 = (query_sao_return_t)(pinStatus + 1);
            sao1.digitalWrite(10, LOW);
        }
    }
    address += 1;
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();
    if (error == 0)
    {
        if(sao2.begin(address))
        {
            sao2.pinModeBulk(pinRead, INPUT);
            sao2.pinMode(10, OUTPUT);
            uint32_t pinStatus = sao2.digitalReadBulk(pinRead);
            returnVal.SAO2 = (query_sao_return_t)(pinStatus + 1);
            sao2.digitalWrite(10, LOW);
        }
    }

    
    returnVal.battery_percentage = maxlipo.cellPercent();
    return returnVal;
}

char * sao_return_to_string(query_sao_return_t returnType)
{
    switch(returnType)
    {
        case QUERY_SAO_NONE: return "no sao plugged in";
        case QUERY_SAO_MISSILE_LAUNCHER: return "missile launcher";
        case QUERY_SAO_GATLING_GUN: return "gatling gun";
        case QUERY_SAO_LASER_CANNON: return "laser cannon";
        default: return "unidentified device";
    }
}
