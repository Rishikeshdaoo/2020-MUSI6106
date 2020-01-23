
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath,
                            s_filterType;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioInput = 0,
                            **ppfAudioOutput = 0;
    
    float                   s_maxDelayTime = 0,
                            s_delayTime = 0,
                            s_delayGain = 0;

    CAudioFileIf            *phAudioFile = 0,
                            *phAudioOutputFile = 0;
    
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CCombFilterIf           *pCombFilter;
    

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 5)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sInputFilePath = sInputFilePath + "sine.wav";
        sOutputFilePath = sInputFilePath + "sinenew.wav";
        
        s_filterType = argv[2];
        s_maxDelayTime = atof(argv[3]);
        s_delayTime = atof(argv[4]);
        s_delayGain = atof(argv[5]);
    }

    //////////////////////////////////////////////////////////////////////////////
    // Creating all instances and opening input file
    CAudioFileIf::create(phAudioFile);
    CAudioFileIf::create(phAudioOutputFile);
    CCombFilterIf::create(pCombFilter);
    
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output audio file
    phAudioOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phAudioOutputFile->isOpen()) {
        cout << "Wave file open error!";
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioInput = new float*[stFileSpec.iNumChannels];
    ppfAudioOutput = new float*[stFileSpec.iNumChannels];
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++){
        ppfAudioInput[i] = new float[kBlockSize];
        ppfAudioOutput[i] = new float[kBlockSize];
    }
    time = clock();
    
    //////////////////////////////////////////////////////////////////////////////
    // Initializing the comb filter
    
    if(s_filterType == "FIR"){
    pCombFilter->init(CCombFilterIf::CombFilterType_t::kCombFIR, s_maxDelayTime, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamDelay, s_delayTime);
        pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamGain, s_delayGain);
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    
    cout << "\r" << "reading and writing";
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioInput, iNumFrames);
        
        Error_t error = pCombFilter->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
        if(error != kNoError){
            return -1;
        }
        
        phAudioOutputFile->writeData(ppfAudioInput, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioOutputFile);
    
    CCombFilterIf::destroy(pCombFilter);

    for (int i = 0; i < stFileSpec.iNumChannels; i++){
        delete[] ppfAudioInput[i];
        delete[] ppfAudioOutput[i];
    }
    
    delete[] ppfAudioInput;
    delete[] ppfAudioOutput;
    ppfAudioInput = 0;
    ppfAudioOutput = 0;
            
    return 0;
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

