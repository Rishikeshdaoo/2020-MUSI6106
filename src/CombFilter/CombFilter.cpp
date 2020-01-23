//
//  CombFilter.cpp
//  CombFilter
//
//  Created by MarketingGRAMusic on 1/21/20.
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

Error_t CCombFilterBase::firFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){
    
    int head = 0;
//    int tail = 0;
    int delay = m_filterDelay * m_sampleRate;
    
    for (int c = 0; c < m_numOfChannels; c++)
    {
        for (int i = 0; i < delay; i++)
        {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i];
            ppfDelayLine[c][i] = ppfInputBuffer[c][i];
        }
        
        for (int j = delay; j < iNumberOfFrames; j++){
            ppfOutputBuffer[c][j] += m_filterGain * ppfDelayLine[c][head];
            head++;
            ppfDelayLine[c][head] = ppfInputBuffer[c][head];
        }
    }
    return kNoError;
};


Error_t CCombFilterBase::iirFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){
  
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
    
    return kNoError;
}


Error_t CCombFilterBase::setParamInternal(FilterParam_t eParam, float fParamValue){
   
    if(eParam == kParamGain){
        m_filterGain = fParamValue;
    }
    else if(eParam == kParamDelay){
        m_filterDelay = fParamValue;
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

Error_t CCombFilterBase::filterCallInternal(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){
    
    if(m_filterType == kCombFIR){
        firFunction(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
    }
    else if(m_filterType == kCombIIR){
        iirFunction(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
    }
}




