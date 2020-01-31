//
//  Created by Rishikesh Daoo on 1/18/20.
//

#ifndef MUSI6106_COMBFILTER_H
#define MUSI6106_COMBFILTER_H

#endif //MUSI6106_COMBFILTER_H

#include "CombFilterIf.h"

class CCombFilterBase : public CCombFilterIf
{

public:
    CCombFilterBase ();
    virtual ~CCombFilterBase ();

    Error_t firFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    Error_t iirFunction(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:
    Error_t initInternal(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) override;
    Error_t resetInternal() override ;
    Error_t setParamInternal(FilterParam_t eParam, float fParamValue) override;
    float getParamInternal( FilterParam_t eParam ) const override;
    Error_t processInternal( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;

    CombFilterType_t m_filterType;
    float **ppfDelayLine;
    float m_filterDelay;
    float m_filterGain;
    float m_maxFilterDelay;
    int m_numOfChannels;
    int m_sampleRate;
};