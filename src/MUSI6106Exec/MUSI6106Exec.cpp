#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void testCases();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath ,                 //!< file paths
                            sOutputFilePath;

    CCombFilterIf::CombFilterType_t                        s_filterType;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioInput = 0,
                            **ppfAudioOutput = 0;

    float                   s_maxDelayTime = 0,
                            s_delayTime = 0,
                            s_delayGain = 0;

    CAudioFileIf            *phAudioInputFile = 0,
                            *phAudioOutputFile = 0;

    std::fstream             hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CCombFilterIf            *pCombFilter;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    std::string pname = argv[0];

    if (argc < 5)
    {
        cout << "Insufficient arguments passed. Running test cases. " << endl;
        testCases();
        return 0;
    }else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + "new.wav";

        s_maxDelayTime = atof(argv[3]);

        if(pname.substr(0,5) == "TEST1") {
            s_delayTime = 0.01;
            s_delayGain = -1.0;
        }else if(pname.substr(0,5) == "TEST2"){
            s_delayTime = 0.5;
            s_delayGain = 1.0;
        }else if(pname.substr(0,5) == "TEST3"){
            s_delayTime = 0.5;
            s_delayGain = 1.0;
        }
        else{
            s_delayTime = atof(argv[4]);
            s_delayGain = atof(argv[5]);
        }

        if(strcmp(argv[2], "FIR") == 0) {
            s_filterType = CCombFilterIf::kCombFIR;
        }else if(strcmp(argv[2], "IIR") == 0) {
            s_filterType = CCombFilterIf::kCombIIR;
        }else {
            cout << "Invalid value passed for filter type";
            return -1;
        }

        if(s_delayGain > 1.0 || s_delayGain < -1.0){
            cout << "Illegal argument passed for Delay. Pass values for delay time. Value should be between -1 and 1";
            return -1;
        }

    }

    //////////////////////////////////////////////////////////////////////////////
    // Creating all instances and opening input file
    CAudioFileIf::create(phAudioInputFile);
    CAudioFileIf::create(phAudioOutputFile);
    CCombFilterIf::create(pCombFilter);

    phAudioInputFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioInputFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioInputFile->getFileSpec(stFileSpec);

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

        pCombFilter->init(s_filterType, s_maxDelayTime, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamDelay, s_delayTime);
        pCombFilter->setParam(CCombFilterIf::FilterParam_t::kParamGain, s_delayGain);

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file

    cout << "\r" << "reading and writing";
    while (!phAudioInputFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioInputFile->readData(ppfAudioInput,iNumFrames);

        Error_t error = pCombFilter->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
        if(error != kNoError){
            return -1;
        }

        phAudioOutputFile->writeData(ppfAudioOutput, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioInputFile);
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

    //////////////////////////////////////////////////////////////////////////////
    // Test cases


int testCase1(std::string input){

    std::string pname = "TEST1";
    std::string filterType = "FIR";
    std::string maxDelayLength = "5";
    std::string outputFile = input + "new.wav";
    std::string gain = "-1.0";
    std::string delay = "0.01";
    const int kBlockSize = 1024;
    const double offset = 0.01;
    float error = 0.f;


    char* argv[] = {(char*) pname.c_str(), (char*) input.c_str(), (char*) filterType.c_str(), (char*) maxDelayLength.c_str(), (char*) gain.c_str(), (char*) delay.c_str()};

    main(5, argv);

    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf::FileSpec_t fileSpec;
    CAudioFileIf::create(phAudioFile);

    float **ppfOutputAudioFile = 0;


    phAudioFile->openFile(outputFile, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(fileSpec);

    ppfOutputAudioFile = new float *[fileSpec.iNumChannels];
    long long iNumFrames = kBlockSize;
    for(int i=0; i<fileSpec.iNumChannels; i++){

        ppfOutputAudioFile[i] = new float [kBlockSize];
    }

    phAudioFile->setPosition(offset);
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfOutputAudioFile,iNumFrames);

        for(int i=0; i<fileSpec.iNumChannels; i++){
            for(int j=0; j<iNumFrames; j++){
                error += ppfOutputAudioFile[i][j];
            }
        }
    }

    if(error > 0){
        cout << "Test case failed!" << error <<endl;
    }else{
        cout << "Test case 3 successful!" <<endl;
    }

    CAudioFileIf::destroy(phAudioFile);
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete[] ppfOutputAudioFile[i];
    }
    delete[] ppfOutputAudioFile;
    ppfOutputAudioFile = 0;

    return 0;
}

int testCase2(std::string input){
    std::string pname = "TEST2";
    std::string filterType = "IIR";
    std::string maxDelayLength = "5";
    std::string outputFile = input + "new.wav";
    std::string gain = "1.0";
    std::string delay = "0.5";
    const int kBlockSize = 1024;
    const double offset = 0.01;
    float error = 0.f;


    char* argv[] = {(char*) pname.c_str(), (char*) input.c_str(), (char*) filterType.c_str(), (char*) maxDelayLength.c_str(), (char*) gain.c_str(), (char*) delay.c_str()};

    main(5, argv);

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t fileSpec;
    CAudioFileIf::create(phInputAudioFile);
    CAudioFileIf::create(phOutputAudioFile);

    float **ppfOutputAudioFile = 0;
    float **ppfInputAudioFile = 0;

    phInputAudioFile->openFile(input, CAudioFileIf::kFileRead);
    phOutputAudioFile->openFile(outputFile, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen() || !phOutputAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(fileSpec);
    phOutputAudioFile->getFileSpec(fileSpec);

    ppfInputAudioFile = new float *[fileSpec.iNumChannels];
    ppfOutputAudioFile = new float *[fileSpec.iNumChannels];
    long long iNumFrames = kBlockSize;
    for(int i=0; i<fileSpec.iNumChannels; i++){
        ppfInputAudioFile[i] = new float [kBlockSize];
        ppfOutputAudioFile[i] = new float [kBlockSize];
    }

    phInputAudioFile->setPosition(offset);
    phInputAudioFile->setPosition(offset);
    while (!phInputAudioFile->isEof() || !phOutputAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phInputAudioFile->readData(ppfOutputAudioFile,iNumFrames);
        phInputAudioFile->readData(ppfOutputAudioFile,iNumFrames);

        // Checking if after the delay line has started to add into the input, whether the output is less than input
        for(int i=0; i<fileSpec.iNumChannels; i++){
            for(int j=0; j<iNumFrames; j++){
                if(ppfOutputAudioFile[i][j] < ppfInputAudioFile[i][j]){
                    cout << "Test case 2 failed!" << endl;
                    return -1;
                }
            }
        }
    }

    CAudioFileIf::destroy(phInputAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete[] ppfOutputAudioFile[i];
        delete[] ppfInputAudioFile[i];
    }
    delete[] ppfOutputAudioFile;
    delete[] ppfInputAudioFile;
    ppfOutputAudioFile = 0;
    ppfInputAudioFile =0;

    cout << "Test case 2 successful!" << endl;

    return 0;
}

int testCase3(std::string input){
    std::string pname = "TEST3";
    std::string filterType = "IIR";
    std::string maxDelayLength = "5";
    std::string outputFile = input + "new.wav";
    std::string gain = "0.5";
    std::string delay = "0.5";
    const double offset = 0.01;
    float error = 0.f;

    const int kBlockSize = 1024;

    char* argv[] = {(char*) pname.c_str(), (char*) input.c_str(), (char*) filterType.c_str(), (char*) maxDelayLength.c_str(), (char*) gain.c_str(), (char*) delay.c_str()};
    main(5, argv);


    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf::FileSpec_t fileSpec;
    CAudioFileIf::create(phAudioFile);

    float **ppfOutputAudioFile = 0;


    phAudioFile->openFile(outputFile, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(fileSpec);

    ppfOutputAudioFile = new float *[fileSpec.iNumChannels];
    long long iNumFrames = kBlockSize;
    for(int i=0; i<fileSpec.iNumChannels; i++){

        ppfOutputAudioFile[i] = new float [kBlockSize];
    }

    phAudioFile->setPosition(offset);
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfOutputAudioFile,iNumFrames);

        for(int i=0; i<fileSpec.iNumChannels; i++){
            for(int j=0; j<iNumFrames; j++){
                error += ppfOutputAudioFile[i][j];
            }
        }
    }

    if(error > 0){
        cout << "Test case failed!" << error <<endl;
    }else{
        cout << "Test case 1 successful!" <<endl;
    }

    CAudioFileIf::destroy(phAudioFile);
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete[] ppfOutputAudioFile[i];
    }
    delete[] ppfOutputAudioFile;
    ppfOutputAudioFile = 0;

    return 0;
}

int testCase4(std::string input){

    std::string pname = "TEST1";
    std::string filterType = "FIR";
    std::string maxDelayLength = "5";
    std::string outputFile = input + "new.wav";
    std::string gain = "-1.0";
    std::string delay = "0.01";
    const int kBlockSize = 1024;
    const double offset = 0.01;
    float error = 0.f;


    char* argv[] = {(char*) pname.c_str(), (char*) input.c_str(), (char*) filterType.c_str(), (char*) maxDelayLength.c_str(), (char*) gain.c_str(), (char*) delay.c_str()};

    main(5, argv);

    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf::FileSpec_t fileSpec;
    CAudioFileIf::create(phAudioFile);

    float **ppfOutputAudioFile = 0;


    phAudioFile->openFile(outputFile, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(fileSpec);

    ppfOutputAudioFile = new float *[fileSpec.iNumChannels];
    long long iNumFrames = kBlockSize;
    for(int i=0; i<fileSpec.iNumChannels; i++){

        ppfOutputAudioFile[i] = new float [kBlockSize];
    }

    phAudioFile->setPosition(offset);
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfOutputAudioFile,iNumFrames);

        for(int i=0; i<fileSpec.iNumChannels; i++){
            for(int j=0; j<iNumFrames; j++){
                if(ppfOutputAudioFile[i][j] > 0){
                    cout << "Test case failed!" << error <<endl;
                }
            }
        }
    }

    CAudioFileIf::destroy(phAudioFile);
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete[] ppfOutputAudioFile[i];
    }
    delete[] ppfOutputAudioFile;
    ppfOutputAudioFile = 0;

    cout << "Test case 4 successful!" <<endl;

    return 0;
}

// TEST CASE 5: If the gain and delay are zero. Then Input == Output

int testCase5(std::string input){
    std::string pname = "TEST5";
    std::string filterType = "FIR";
    std::string maxDelayLength = "5";
    std::string outputFile = input + "new.wav";
    std::string gain = "0.0";
    std::string delay = "0.0";
    const int kBlockSize = 1024;
    const double offset = 0.01;
    float error = 0.f;


    char* argv[] = {(char*) pname.c_str(), (char*) input.c_str(), (char*) filterType.c_str(), (char*) maxDelayLength.c_str(), (char*) gain.c_str(), (char*) delay.c_str()};

    main(5, argv);

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t fileSpec;
    CAudioFileIf::create(phInputAudioFile);
    CAudioFileIf::create(phOutputAudioFile);

    float **ppfOutputAudioFile = 0;
    float **ppfInputAudioFile = 0;

    phInputAudioFile->openFile(input, CAudioFileIf::kFileRead);
    phOutputAudioFile->openFile(outputFile, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen() || !phOutputAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(fileSpec);
    phOutputAudioFile->getFileSpec(fileSpec);

    ppfInputAudioFile = new float *[fileSpec.iNumChannels];
    ppfOutputAudioFile = new float *[fileSpec.iNumChannels];
    long long iNumFrames = kBlockSize;
    for(int i=0; i<fileSpec.iNumChannels; i++){
        ppfInputAudioFile[i] = new float [kBlockSize];
        ppfOutputAudioFile[i] = new float [kBlockSize];
    }

    phInputAudioFile->setPosition(offset);
    phInputAudioFile->setPosition(offset);
    while (!phInputAudioFile->isEof() || !phOutputAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phInputAudioFile->readData(ppfOutputAudioFile,iNumFrames);
        phInputAudioFile->readData(ppfOutputAudioFile,iNumFrames);

        // Checking if after the delay line has started to add into the input, whether the output is less than input
        for(int i=0; i<fileSpec.iNumChannels; i++){
            for(int j=0; j<iNumFrames; j++){
                if(ppfOutputAudioFile[i][j] != ppfInputAudioFile[i][j]){
                    cout << "Test case 5 failed!" << endl;
                    return -1;
                }
            }
        }
    }

    CAudioFileIf::destroy(phInputAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);
    for (int i = 0; i < fileSpec.iNumChannels; i++) {
        delete[] ppfOutputAudioFile[i];
        delete[] ppfInputAudioFile[i];
    }
    delete[] ppfOutputAudioFile;
    delete[] ppfInputAudioFile;
    ppfOutputAudioFile = 0;
    ppfInputAudioFile =0;

    cout << "Test case 5 successful!" << endl;

    return 0;
}

void testCases(){

    testCase1("../../test/sine441.wav");
    testCase2("../../test/sine441.wav");
    testCase3("../../test/noise.wav");
    testCase4("../../test/silence.wav");
    testCase5("../../test/noise.wav");
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}