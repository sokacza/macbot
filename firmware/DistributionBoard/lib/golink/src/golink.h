//
// Created by reine on 2020-07-11.
//

#ifndef GO_LINK_GOLINK_H
#define GO_LINK_GOLINK_H

#include <Arduino.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include "binn/src/binn.h"
#include "isotp-c/isotp.h"
#include "arduino-esp32-can/src/CAN.h"

#define ISOTP_BUFSIZE 1024

class goLink
{

    static goLink *instance;

public:
    binn *outgoingDataObject;
    binn *incommingDataObject;

    IsoTpLink g_link;

    /* Alloc send and receive buffer statically in RAM */
    uint8_t g_isotpRecvBuf[ISOTP_BUFSIZE];
    uint8_t g_isotpSendBuf[ISOTP_BUFSIZE];

    /* Create the maps to store and hold information */
    std::map<std::string, int32_t> outgoingInt;
    std::map<std::string, float> outgoingFloat;
    std::map<std::string, std::string> outgoingString;

    std::map<std::string, int32_t> incomingInt;
    std::map<std::string, float> incomingFloat;
    std::map<std::string, std::string> incomingString;

    /* class vars*/
    uint32_t runningTime = 0;
    uint32_t broadCastRate = 1000;
    uint8_t id = 0;
    uint8_t updated = 0;

    /*setup and class def functions*/
    void init(long dataRate, int id);
    void setDataRate(long dataRate);
    void setAddress(int id, int mask);
    void setBroadcastRate(uint32_t miliseconds);
    void setFilterAddress(uint8_t extended, uint32_t address);

    /*runner functions*/
    void spin(void);
    void receiveHandler();
    void buildOutData();

    /*setters and getters*/
    void setInt(const char *key, int32_t val);
    void setFloat(const char *key, float val);
    void setString(const char *key, const char *val);

    int getInt(const char *key);
    float getFloat(const char *key);
    const char *getString(const char *key);

    /*error handling*/
    void myCallBack(std::string error);

    /* general utilities */
    uint32_t getSystemMilis(void);
};

void goLink::spin()
{
    /*
     * 1. check if there is anything to be recieved (unless callback interupt implemented)
     * 2. check if the broadcast timer is clear
     * 3. transmit dataobject
     */

    if (runningTime < (getSystemMilis()))
    {
        runningTime = getSystemMilis() + this->broadCastRate;
        if (this->updated)
        {
            this->buildOutData();
            isotp_send(&this->g_link, (uint8_t *)binn_ptr(outgoingDataObject), binn_size(outgoingDataObject));
        }
    }

    this->receiveHandler();
    isotp_poll(&this->g_link);
    uint16_t out_size;
    uint8_t data[1024];
    uint8_t ret = isotp_receive(&this->g_link, data, 1024, &out_size);
    binn_iter iter;
    binn value;
    char key[128];

    if (ISOTP_RET_OK == ret)
    {
        binn_load((void *)data, this->incommingDataObject);
        binn_object_foreach(this->incommingDataObject, key, value)
        {
            Serial.println(value.vint32);
            switch (value.type)
            {
                
            case BINN_UINT8:
                this->incomingInt[key] = value.vuint8;
                break;
            case BINN_INT8:
                this->incomingInt[key] = value.vint8;
                break;
            case BINN_UINT16:
                this->incomingInt[key] = value.vuint16;
                break;
            case BINN_INT16:
                this->incomingInt[key] = value.vint16;
                break;
            case BINN_UINT32:
                this->incomingInt[key] = value.vuint32;
                break;
            case BINN_INT32:
                this->incomingInt[key] = value.vint32;
                break;
            case BINN_UINT64:
                this->incomingInt[key] = value.vuint64;
                break;
            case BINN_INT64:
                this->incomingInt[key] = value.vint64;
                break;
            case BINN_FLOAT:
                this->incomingFloat[key] = value.vfloat;
                break;
            case BINN_FLOAT64:
                this->incomingFloat[key] = float(value.vdouble);
                break;
            case BINN_STRING:
                this->incomingString[key].assign((const char *)value.ptr, (size_t)value.size);
                break;
            }
        }
    }
}

void goLink::buildOutData()
{
    binn_free(this->outgoingDataObject);
    this->outgoingDataObject = binn_object();
    this->outgoingDataObject->disable_int_compression = TRUE;
    for (std::pair<std::string, int32_t> itInt : this->outgoingInt)
    {
        binn_object_set_int32(this->outgoingDataObject, itInt.first.c_str(), itInt.second);
    }
    for (std::pair<std::string, float> itFlt : this->outgoingFloat)
    {
        binn_object_set_double(this->outgoingDataObject, itFlt.first.c_str(), itFlt.second);
    }
    for (std::pair<std::string, std::string> itStr : this->outgoingString)
    {
        binn_object_set_str(this->outgoingDataObject, itStr.first.c_str(), (char *)itStr.second.c_str());
    }
}

void goLink::setString(const char *key, const char *val)
{
    this->updated = 1;
    outgoingString[key] = val;
}

const char *goLink::getString(const char *key)
{
    std::map<std::string, std::string>::iterator it = this->outgoingString.find(key);
    if (it != this->outgoingString.end())
    {
        return it->second.c_str();
    }
    it = this->incomingString.find(key);
    if (it != this->incomingString.end())
    {
        return it->second.c_str();
    }
    this->myCallBack("key does not exist in data object");
    return "";
}

void goLink::setInt(const char *key, int32_t val)
{
    this->updated = 1;
    this->outgoingInt[key] = val;
}

int goLink::getInt(const char *key)
{
    std::map<std::string, int32_t>::iterator it = this->incomingInt.find(key);
    if (it != this->incomingInt.end())
    {
        return it->second;
    }
    it = this->outgoingInt.find(key);
    if (it != this->outgoingInt.end())
    {
        return it->second;
    }
    this->myCallBack("key does not exist in data object");
    return 0;
}

void goLink::setFloat(const char *key, float val)
{
    this->updated = 1;
    this->outgoingFloat[key] = val;
}

float goLink::getFloat(const char *key)
{
    std::map<std::string, float>::iterator it = this->incomingFloat.find(key);
    if (it != this->incomingFloat.end())
    {
        return it->second;
    }
    it = this->outgoingFloat.find(key);
    if (it != this->outgoingFloat.end())
    {
        return it->second;
    }
    this->myCallBack("key does not exist in data object");
    return 0;
}

void goLink::init(long dataRate, int id)
{
    this->id = id;
    this->incommingDataObject = binn_object();
    isotp_init_link(&g_link, id + 100,
                    g_isotpSendBuf, sizeof(g_isotpSendBuf),
                    g_isotpRecvBuf, sizeof(g_isotpRecvBuf));

    // setup the CAN bus
    CAN.begin(dataRate);
    CAN.filter(id, 0xffff);
    //CAN.loopback();

    //timerAttachInterrupt(timer, onTimer, true);
}

void goLink::setDataRate(long dataRate)
{
    CAN.begin(dataRate);
}

void goLink::setBroadcastRate(uint32_t miliseconds)
{
    broadCastRate = miliseconds;
}

uint32_t goLink::getSystemMilis(void)
{
    return millis();
}

void goLink::receiveHandler(void)
{
    uint8_t packetSize = CAN.parsePacket();
    uint8_t data[8];
    uint8_t i = 0;

    /*TODO add extended frame flexibility and address check
    */
    if (packetSize)
    {
        while (CAN.available())
        {
            data[i] = CAN.read();
            i++;
        }
    }

    isotp_on_can_message(&g_link, data, packetSize);
}

int isotp_user_send_can(const uint32_t arbitration_id,
                        const uint8_t *data, const uint8_t size)
{
    CAN.beginPacket(int(arbitration_id));
    for (uint8_t i = 0; i < size; i++)
        CAN.write(data[i]);
    CAN.endPacket();
    return 0;
}

/* required, return system tick, unit is millisecond */
uint32_t isotp_user_get_ms(void)
{
    return millis();
}

/* optional, provide to receive debugging log messages */
void isotp_user_debug(const char *message, ...)
{
    Serial.print(message);
}

#endif //GO_LINK_GOLINK_H
