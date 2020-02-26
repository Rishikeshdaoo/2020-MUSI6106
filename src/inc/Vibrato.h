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

    /*! creates a new instance of the Vibrato class
    \param pCVib: pointer of type CVibrato
    \return Error_t: Error enum type
    */
    static Error_t create(CVibrato*& pCVib);


    /*! destroys an instance of the Vibrato class
    \param pCVib: pointer of type CVibrato
    \return Error_t: Error enum type
    */
    static Error_t destroy(CVibrato*& pCVib);

    /*! initializes a Vibrato instance
    \param modFrequency: Modulation frequency of Vibrato in Hz
    \param mod_amplitude: Modulation amplitude of Vibrato in secs
    \param delayinSec: The delay of Vibrato in secs
    \param iNumChannels: number of audio channels
    \param fSampleRate: sampling rate of the audio
    \return Error_t: Error enum type
    */
    Error_t init(float modFrequency, float mod_amplitude, float delayinSec, int iNumOfChannels, float fSampleRate);

    /*! resets an instance to initial state
    \return Error_t: Error enum type
    */
    Error_t reset();

    /*! processes one block of audio
    \param ppfInputBuffer: input buffer [numChannels][iNumberOfFrames]
    \param ppfOutputBuffer: output buffer [numChannels][iNumberOfFrames]
    \param iNumberOfFrames: buffer length (per channel)
    \return Error_t: Error enum type
    */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumOfFrames);

private:
    CVibrato();
    ~CVibrato();

    /*! converts secs to number of samples using sampling rate
    \param fTimeInSec: time in secs
    \param iNumOfSamples: variable to store the number of samples value
    \return Error_t: Error enum type
    */
    Error_t timeToSamples(float fTimeInSec, int &iNumOfSamples);

    CRingBuffer <float_t> **pCRingDelayLine;
    CLfo *pSineLfo;

    //TODO: Implement the checks for the m_bIsInitialized variable

    bool m_bIsInitialized;  //!< internal bool to check whether the init function has been called
    float m_modFrequency;   //!< modulation frequency of Vibrato
    int m_modAmplitude;     //!< modulation Amplitude of Vibrato
    float m_maxDelayLength; //!< length of the Vibrato ring buffer
    int m_delayLength;      //!< delay of the Vibrato
    int m_numOfChannels;    //!< number of audio channels
    float m_SampleRate;     //!< sampling rate of input audio
};
