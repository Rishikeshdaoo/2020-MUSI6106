//t
// Created by Rishikesh Daoo on 2/12/20.
//

#include <ErrorDef.h>
#include "RingBuffer.h"

#ifndef MUSI6106_LFO_H
#define MUSI6106_LFO_H

#endif //MUSI6106_LFO_H


class CLfo{

    const double pi = 4 * atan(1.0);

public:
    CLfo(float fModulationFreq){
        m_modulationFrequency = fModulationFreq;
    }

    ~CLfo(){
        m_modulationFrequency = 0;
    }

    /*
     *  Returns the LFO at a given time in seconds
     */
    double getWavetableLFO(double time)
    {
        return sin(2 * pi * m_modulationFrequency * time);
    }

    /*
     *  Sets the LFO frequency
     */
    Error_t setLFOFrequency(float mod_frequency){
        m_modulationFrequency = mod_frequency;

        return kNoError;
    }


private:
    float m_modulationFrequency;
};
