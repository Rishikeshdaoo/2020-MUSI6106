
#include <iostream>
#include <ctime>
#include <Vibrato.h>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
//#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[]) {
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath,
                sOutputTxtFilePath;


    static const int kBlockSize = 10;

    clock_t         time =0;

    float           **ppfAudioInputData = 0,
                    **ppfAudioOutputData = 0;


    CAudioFileIf    *phAudioInputFile = 0,
                    *phAudioOutputFile = 0;

    std::fstream    hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    CVibrato        *pCVibrato = 0;

    float           mod_freq = 0.F,
                    mod_amp_secs = 0.F,
                    delay_width_secs = 0.1F;

    Error_t         error;

    //CCombFilterIf   *pInstance = 0;
    //CCombFilterIf::create(pInstance);
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 4) {
        cout << "Missing audio input path!";
        return -1;
    } else {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + "new.wav";
        sOutputTxtFilePath = sInputFilePath + "new.txt";
        mod_freq = atof(argv[2]);
        mod_amp_secs = atof(argv[3]);
        delay_width_secs = atof(argv[4]);
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    error = CAudioFileIf::create(phAudioInputFile);
    error = CAudioFileIf::create(phAudioOutputFile);
    error = CVibrato::create(pCVibrato);

    phAudioInputFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioInputFile->isOpen()) {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioInputFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open (sOutputTxtFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open()){
        cout << "Text file open error!";
        return -1;
    }

    // open the output wav file
    phAudioOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phAudioOutputFile->isOpen()) {
        cout << "Wave file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioInputData = new float *[stFileSpec.iNumChannels];
    ppfAudioOutputData = new float *[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfAudioInputData[i] = new float[kBlockSize];
        ppfAudioOutputData[i] = new float[kBlockSize];
    }
    time = clock();

    //////////////////////////////////////////////////////////////////////////////
    // Initializing the Vibrato

    error = pCVibrato->init(mod_freq, mod_amp_secs, delay_width_secs, stFileSpec.iNumChannels, stFileSpec.fSampleRateInHz);
    if(error == kFunctionInvalidArgsError){
        cout << "Parameters passed to Vibrato are not valid. Please check."<< endl;

    if(error == kUnknownError){
        cout << "Memory issues." << endl;
        }
    }

    long temp_num = 0;
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    while (!phAudioInputFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioInputFile->readData(ppfAudioInputData, iNumFrames);
        temp_num = temp_num + iNumFrames;

        cout << "\r" << "reading and writing";
        error = pCVibrato->process(ppfAudioInputData, ppfAudioOutputData, iNumFrames);
        if (error != kNoError)
            return -1;

        phAudioOutputFile->writeData(ppfAudioOutputData, iNumFrames);

        for (int i = 0; i < iNumFrames; i++)
        {
            for (int j = 0; j < stFileSpec.iNumChannels; j++)
            {
                hOutputFile << ppfAudioOutputData[j][i] << " ";
            }
            hOutputFile << endl;
        }

    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up

    CAudioFileIf::destroy(phAudioInputFile);
    CAudioFileIf::destroy(phAudioOutputFile);

    CVibrato::destroy(pCVibrato);

    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        delete[] ppfAudioInputData[i];
        delete[] ppfAudioOutputData[i];
    }

    delete[] ppfAudioInputData;
    delete[] ppfAudioOutputData;

    ppfAudioInputData = 0;
    ppfAudioOutputData = 0;

    return 0;
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

