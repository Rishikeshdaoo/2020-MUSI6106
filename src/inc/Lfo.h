//t
// Created by Rishikesh Daoo on 2/12/20.
//

#include <ErrorDef.h>
#include "RingBuffer.h"
#include <Math.h>

#ifndef MUSI6106_LFO_H
#define MUSI6106_LFO_H

#endif //MUSI6106_LFO_H


class CLfo{

public:
    CLfo(float fModulationFreq, float fSampleRate){
        m_fModulationFrequency = fModulationFreq;
        m_fSampleRate = fSampleRate;
        m_iLfoLengthSamples = 44100;
        m_pLfoBuffer = new float[m_iLfoLengthSamples];
        m_currentIdx = 0;

        for(int sample = 0; sample < m_iLfoLengthSamples; ++sample){
            m_pLfoBuffer[sample] = sin( (2 * M_PI  * (float) sample) / m_iLfoLengthSamples);
        }

        m_offset = (m_fModulationFrequency/ m_fSampleRate) * m_iLfoLengthSamples;
    }

    ~CLfo(){
        m_fModulationFrequency = 0;
        m_iLfoLengthSamples = 0;
        delete[] m_pLfoBuffer;
        m_pLfoBuffer = nullptr;
        m_offset = 0;
        m_currentIdx = 0;
    }

    /*
     *  Returns the wavetable LFO
     */
    float getWavetableLFO(){
        int readIdx = floor(m_currentIdx);

        // Taking the mod to wrap the index around if it goes over the length of buffer
        if(round(m_currentIdx + m_offset) > m_iLfoLengthSamples -1) {
            m_currentIdx = (m_currentIdx + m_offset) - (float) m_iLfoLengthSamples;
        }
        else{
           m_currentIdx = (m_currentIdx + m_offset);
        }

        return m_pLfoBuffer[readIdx];
    }

    /*
     *  Sets the LFO frequency
     */
    Error_t setLFOFrequency(float mod_frequency){
        m_fModulationFrequency = mod_frequency;

        return kNoError;
    }


private:
    float m_fModulationFrequency;
    float m_fSampleRate;
    int m_iLfoLengthSamples;
    float *m_pLfoBuffer;
    float m_offset;
    float m_currentIdx;
};
