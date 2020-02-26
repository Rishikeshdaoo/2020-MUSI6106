//
// Created by Rishikesh Daoo on 2/10/20.
//


#include "ErrorDef.h"
#include "RingBuffer.h"

#include "Vibrato.h"


CVibrato::CVibrato() {

    m_maxDelayLength = 0;
    m_numOfChannels = 0;
    m_modAmplitude = 0;
    m_delayLength = 0;
    m_SampleRate = 0;
    m_modFrequency = 0;

    pCRingDelayLine = 0;
    pSineLfo = 0;

    m_bIsInitialized = false;
}

CVibrato::~CVibrato(){
    this->reset();
}

Error_t CVibrato::create(CVibrato*& pCVib) {

    pCVib = new CVibrato();

    if(!pCVib)
        return kUnknownError;

    return kNoError;
}

Error_t CVibrato::destroy(CVibrato*& pCVib) {

    if(!pCVib)
        return kUnknownError;

    pCVib->reset();

    delete[] pCVib -> pCRingDelayLine;
    delete pCVib;
    pCVib = 0;

    return kNoError;
}

Error_t CVibrato::init(float modFrequency, float mod_amplitude, float delayinSec, int iNumOfChannels, float fSampleRate) {

    if (delayinSec <= 0 ||
        fSampleRate <= 0 ||
        iNumOfChannels <= 0)
        return kFunctionInvalidArgsError;

    if(modFrequency < 0 || mod_amplitude < 0 || delayinSec < 0)
        return kFunctionInvalidArgsError;

    m_SampleRate = fSampleRate;
    m_modFrequency = modFrequency;
    Error_t error_delay = timeToSamples(delayinSec, m_delayLength);
    Error_t error_amp = timeToSamples(mod_amplitude, m_modAmplitude);
    m_numOfChannels = iNumOfChannels;

    m_maxDelayLength = 2 * m_delayLength;

    pSineLfo = new CLfo(modFrequency, m_SampleRate);

    if(error_delay != kNoError or error_amp != kNoError){
        return kUnknownError;
    }

    if(m_delayLength > m_maxDelayLength)
        return kFunctionInvalidArgsError;

    if(m_modAmplitude > m_delayLength)
        return kFunctionInvalidArgsError;

    if(m_delayLength * 2 > m_maxDelayLength)
        return kFunctionInvalidArgsError;

    pCRingDelayLine = new CRingBuffer<float> *[iNumOfChannels];
    for (int i = 0; i < iNumOfChannels; i++) {
        pCRingDelayLine[i] = new CRingBuffer<float>(static_cast<int > (m_maxDelayLength));
    }

    m_bIsInitialized = true;

    return kNoError;
}

Error_t CVibrato::reset(){
    m_modFrequency = 0.F;
    m_modAmplitude = 0;
    m_delayLength = 0;
    m_numOfChannels = 0;

    pSineLfo->setLFOFrequency(0.f);

    for(int i=0; i<m_numOfChannels; i++){
        pCRingDelayLine[i]-> reset();
    }

    m_bIsInitialized = false;
    return kNoError;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumOfFrames) {

    if (!m_bIsInitialized)
        return kNotInitializedError;

    float zeiger = 0.F;
    float frac = 0.F;
    int readIndex = 0;

    for (int i = 0; i < iNumOfFrames; ++i) {

        zeiger = m_delayLength + (m_modAmplitude * pSineLfo->getWavetableLFO());
        readIndex = floor(zeiger);
        frac = zeiger - readIndex;

        for(int j=0; j<m_numOfChannels; j++){

            pCRingDelayLine[j]->putPostInc(ppfInputBuffer[j][i]);
            float val1 = pCRingDelayLine[j]->get(readIndex);
            float val = (pCRingDelayLine[j]->get(readIndex+1) * frac) + (pCRingDelayLine[j]->get(readIndex) * (1-frac));
            ppfOutputBuffer[j][i] = (pCRingDelayLine[j]->get(readIndex+1) * frac) + (pCRingDelayLine[j]->get(readIndex) * (1-frac));
            pCRingDelayLine[j]->getPostInc();
        }
    }
    return kNoError;
}

Error_t CVibrato::timeToSamples(float fTimeInSec, int &iNumOfSamples) {

    iNumOfSamples = static_cast<int> (fTimeInSec * m_SampleRate);

    return kNoError;
}

