#include "Query-SAO.h" 
#include <Wire.h>
#include "Adafruit_seesaw.h"

query_sao_collection_t query_sao(void)
{
    query_sao_collection_t returnVal = {QUERY_SAO_NONE, QUERY_SAO_NONE, QUERY_SAO_NONE};
    Adafruit_seesaw sao0;
    Adafruit_seesaw sao1; 
    Adafruit_seesaw sao2;
    uint32_t pinRead = 0x3F;
    byte error;
     
    int address = 0x49;
    
    WIRE.begin();
    
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();
    if (error == 0)
    {
        if(sao0.begin(address))
        {
            sao0.pinModeBulk(pinRead, INPUT);
            uint32_t pinStatus = sao0.digitalReadBulk(pinRead);
            returnVal.SAO0 = (query_sao_return_t)(pinStatus + 1);
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
            uint32_t pinStatus = sao1.digitalReadBulk(pinRead);
            returnVal.SAO1 = (query_sao_return_t)(pinStatus + 1);
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
            uint32_t pinStatus = sao2.digitalReadBulk(pinRead);
            returnVal.SAO2 = (query_sao_return_t)(pinStatus + 1);
        }
    }
      
    return returnVal;
}

char * sao_return_to_string(query_sao_return_t returnType)
{
    switch(returnType)
    {
        case QUERY_SAO_NONE: return "no sao plugged in";
        case QUERY_SAO_MISSILE_LAUNCHER: return "missile launcher";
        case QUERY_SAO_GATLING_GUN: return "gatling gun";
        case QUERY_SAO_CANNON: return "cannon";
        case QUERY_SAO_LASER: return "laser";
        default: return "unidentified device";
    }
}
