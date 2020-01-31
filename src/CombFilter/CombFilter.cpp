//
// Created by Rishikesh Daoo on 1/20/20.
//

#include <stdio.h>
#include <iostream>
#include "CombFilter.h"


using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CCombFilterBase::CCombFilterBase(): CCombFilterIf(){
    reset();
}

CCombFilterBase::~CCombFilterBase()
{
    reset();
}

Error_t CCombFilterBase::firFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {

    int delay = m_filterDelay * m_sampleRate;

    if(delay == 0){ delay = 1;}

    for (int c = 0; c < m_numOfChannels; c++) {
        for (int i = 0; i < iNumberOfFrames; i++) {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + (m_filterGain * ppfDelayLine[c][delay - 1]);
            int output = ppfOutputBuffer[c][i];
            for (int idx = delay - 1; idx > 0; idx--)
                ppfDelayLine[c][idx] = ppfDelayLine[c][idx - 1];
            ppfDelayLine[c][0] = ppfInputBuffer[c][i];
        }
    }
    return kNoError;
}

Error_t CCombFilterBase::iirFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){

    int delay = m_filterDelay * m_sampleRate;

    if(delay == 0){ delay = 1;}

    for (int c = 0; c < m_numOfChannels; c++) {
        for (int i = 0; i < iNumberOfFrames; i++) {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + (m_filterGain * ppfDelayLine[c][delay - 1]);
            for (int j = delay - 1; j > 0; j--)
                ppfDelayLine[c][j] = ppfDelayLine[c][j - 1];
            ppfDelayLine[c][0] = ppfOutputBuffer[c][i];
        }
    }
    return kNoError;
}

Error_t CCombFilterBase::initInternal(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels){
    m_filterType = eFilterType;
    m_maxFilterDelay = fMaxDelayLengthInS;
    m_numOfChannels = iNumChannels;
    m_sampleRate = fSampleRateInHz;

    int delaySamples = (int) m_maxFilterDelay * m_sampleRate;

    ppfDelayLine = new float*[m_numOfChannels];
    for (int i = 0; i < m_numOfChannels; i++){
        ppfDelayLine[i] = new float[delaySamples]();
    }

    return kNoError;
}


Error_t CCombFilterBase::resetInternal(){
    m_maxFilterDelay = 0;
    m_numOfChannels = 0;
    m_sampleRate = 0;

    for(int i=0; i<m_numOfChannels; i++){
        delete[] ppfDelayLine[i];
    }

    delete[] ppfDelayLine;

    ppfDelayLine =0;

    return kNoError;
}


Error_t CCombFilterBase::setParamInternal(FilterParam_t eParam, float fParamValue){

    if(eParam == kParamGain){
        m_filterGain = fParamValue;
    }
    else if(eParam == kParamDelay){
        if(fParamValue > m_maxFilterDelay){
            m_filterDelay = m_maxFilterDelay;
        }else{
            m_filterDelay = fParamValue;
        }
    }

    return kNoError;
}


float CCombFilterBase::getParamInternal( FilterParam_t eParam ) const {

    if(eParam == kParamGain){
        return m_filterGain;
    }
    else if(eParam == kParamDelay){
        return m_filterDelay;
    }
    return 0;
}

Error_t CCombFilterBase::processInternal( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){

    Error_t error = kNoError;

    if(m_filterType == kCombFIR){
        error = firFunction(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
    }
    else if(m_filterType == kCombIIR){
        error = iirFunction(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
    }

    return error;
}
