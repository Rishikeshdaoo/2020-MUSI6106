//
// Created by Rishikesh Daoo on 2/10/20.
//

#ifndef MUSI6106_VIBRATO_H
#define MUSI6106_VIBRATO_H

#endif //MUSI6106_VIBRATO_H


#include <math.h>
#include <ErrorDef.h>
#include "Lfo.h"

class CVibrato{

public:

    static Error_t create(CVibrato*& pCVib);
    static Error_t destroy(CVibrato*& pCVib);

    Error_t init(float modFrequency, float mod_amplitude, float delayinSec, int iNumOfChannels, float fSampleRate);
    Error_t reset();

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumOfFrames);

private:
    CVibrato();
    ~CVibrato();

    Error_t timeToSamples(float fTimeInSec, int &iNumOfSamples);

    CRingBuffer <float_t> **pCRingDelayLine;
    CLfo *pSineLfo;

    float m_modFrequency;
    int m_modAmplitude;
    const float m_maxDelayLengthInSec;
    int m_delayLength;
    float m_numOfChannels;
    int m_SampleRate;
    long long int m_numOfSamplesProcessed;
};
