// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include "CombFilter.h"

static const char*  kCMyProjectBuildDate             = __DATE__;


CCombFilterIf::CCombFilterIf () :
        m_bIsInitialized(false),
        m_pCCombFilter(0),
        m_fSampleRate(0)
{
    // this never hurts
    reset ();
}


CCombFilterIf::~CCombFilterIf ()
{
    reset ();
}

const int  CCombFilterIf::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
        case kMajor:
            iVersion    = MUSI6106_VERSION_MAJOR;
            break;
        case kMinor:
            iVersion    = MUSI6106_VERSION_MINOR;
            break;
        case kPatch:
            iVersion    = MUSI6106_VERSION_PATCH;
            break;
        case kNumVersionInts:
            iVersion    = -1;
            break;
    }

    return iVersion;
}
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CCombFilterIf::create(CCombFilterIf*& pCCombFilter){

    pCCombFilter   = new CCombFilterBase();

    if (!pCCombFilter)
        return kMemError;

    return kNoError;
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter){

    delete pCCombFilter;
    pCCombFilter = 0;

    return kNoError;
}

Error_t CCombFilterIf::init( CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels )
{
    Error_t error = kNoError;
    if(!m_bIsInitialized){
        error = initInternal(eFilterType, fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
    }
    if(error != kNoError){
        return kNotInitializedError;
    }else{
        m_bIsInitialized = true;
    }
    return error;
}


Error_t CCombFilterIf::reset ()
{
    Error_t error = kNoError;
    if(m_bIsInitialized){
        error = resetInternal();
    }
    if(error != kNoError){
        return kNotInitializedError;
    } else{
        m_bIsInitialized = false;
    }

    return error;
}

Error_t CCombFilterIf::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    if (!ppfInputBuffer || !ppfInputBuffer[0] || !ppfOutputBuffer || !ppfOutputBuffer[0] || iNumberOfFrames < 0)
        return kFunctionInvalidArgsError;

    if(!m_bIsInitialized){
        return kNotInitializedError;
    }

    Error_t error = kNoError;
    error = processInternal(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);

    return error;
}

Error_t CCombFilterIf::setParam( FilterParam_t eParam, float fParamValue )
{
    Error_t error = kNoError;
    error = setParamInternal(eParam, fParamValue);
    if(error != kNoError){
        return kNotInitializedError;
    }
    return kNoError;
}

float CCombFilterIf::getParam( FilterParam_t eParam ) const
{
    getParamInternal(eParam);
    return 0;
}