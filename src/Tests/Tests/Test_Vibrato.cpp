#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "UnitTest++.h"

#include "Vibrato.h"
#include "Synthesis.h"
#include "Vector.h"

SUITE(Vibrato)
{
    struct VibratoData
    {
        VibratoData():
                pVibrato(0),
                inputData(0),
                outputData(0),
                dataLength(22523),
                //maxDelayLength(3.F),
                blockLength(10),
                numChannels(2),
                sampleRate(8000),
                delay_width(.002F),
                mod_freq(11.F),
                mod_amp(.002F)

        {
            CVibrato::create(pVibrato);

            inputData = new float*[numChannels];
            outputData = new float*[numChannels];
            inputTmp = new float*[numChannels];
            outputTmp = new float*[numChannels];
            for (int i = 0;i < numChannels; i++)
            {
                inputData[i] = new float[dataLength];
                outputData[i] = new float[dataLength];
                CVectorFloat::setZero(inputData[i], dataLength);
                CVectorFloat::setZero(outputData[i], dataLength);
            }
        }

        ~VibratoData()
        {
            for (int i = 0;i < numChannels; i++)
            {
                delete[] inputData[i];
                delete[] outputData[i];
                //delete[] inputTmp[i];
                //delete[] outputTmp[i];
            }
            delete[] inputData;
            delete[] outputData;
            delete[] inputTmp;
            delete[] outputTmp;

            CVibrato::destroy(pVibrato);
        }

        void TestProcess()
        {
            int numFramesRemaining = dataLength;
            while (numFramesRemaining > 0)
            {
                int numFrames = std::min(numFramesRemaining, blockLength);
                for (int i = 0; i < numChannels; i++)
                {
                    inputTmp[i] = &inputData[i][dataLength - numFramesRemaining];
                    outputTmp[i] = &outputData[i][dataLength - numFramesRemaining];
                }
                pVibrato->process(inputTmp, outputTmp, numFrames);
                numFramesRemaining -= numFrames;
            }
        }

        CVibrato *pVibrato;
        float   **inputData,
                **outputData,
                **inputTmp,
                **outputTmp;
        int     dataLength;
        //float   maxDelayLength;
        int     blockLength;
        int     numChannels;
        long int   sampleRate;
        float   delay_width,
                mod_freq,
                mod_amp;
    };


    TEST_FIXTURE(VibratoData, mod_amp_zero)
    {
        pVibrato->init(mod_freq,0,delay_width,numChannels,sampleRate);

        // Generating an input for the test case.
        for(int i=0; i< numChannels; i++){
            CSynthesis::generateSine(inputData[i], 441, sampleRate, dataLength);

        TestProcess();

        for (int i = 0; i < numChannels; i++){
            for (int j = static_cast<int>(round(delay_width*sampleRate)); j < dataLength; j++){
                CHECK_CLOSE(inputData[i][j- static_cast<int>(round((delay_width*sampleRate)))], outputData[i][j], 1e-3F);
                }
            }
        }
    }

}

#endif //WITH_TESTS