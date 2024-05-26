#pragma once

#include <PololuTWISlave.h>

class RomiTWIPeripheral : public PololuTWISlave
{
public:
    enum registerAddr
    {
        TARGET_LEFT_L = 0x00,
        TARGET_LEFT_H = 0x01,
        TARGET_RIGHT_L = 0x02,
        TARGET_RIGHT_H = 0x03,
        SPEED_LEFT_L = 0x04,
        SPEED_LEFT_H = 0x05,
        SPEED_RIGHT_L = 0x06,
        SPEED_RIGHT_H = 0x07,
        LAST_REGISTER = 0x08,
    };

private:
    volatile uint8_t registerMap[8];
    volatile uint8_t currAddr = 0x00;

    enum TWIState
    {
        TWI_IDLE,
        TWI_WAITING_FOR_ADDR,
        TWI_RECEIVING_DATA,
        TWI_SENDING_DATA,
    };

    volatile TWIState twiState = TWI_IDLE;

public:

    virtual void start(void) { twiState = TWI_WAITING_FOR_ADDR; }
    virtual void stop(void) { twiState = TWI_IDLE; }
    virtual void receive(uint8_t b)
    {
        if(twiState == TWI_WAITING_FOR_ADDR) 
        { 
            if(b >= 0x00 && b < LAST_REGISTER) currAddr = b; 
            twiState = TWI_RECEIVING_DATA;
        }

        else if(twiState == TWI_RECEIVING_DATA)
        {
            if(currAddr >= 0x00 && currAddr < LAST_REGISTER)
                registerMap[currAddr++] = b;
        }
    }

    virtual uint8_t transmit(void)
    {
        return registerMap[currAddr++];
    }

    /* Initialize the slave on a given address. */
    void init(uint8_t address)
    {
        PololuTWISlave::init(address, *this);
    }
};