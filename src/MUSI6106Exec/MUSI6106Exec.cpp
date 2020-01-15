
#include <iostream>
#include <fstream>
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
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments

    sInputFilePath = argv[1];
    sOutputFilePath = argv[2];

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    
    stFileSpec.eFormat = CAudioFileIf::FileFormat_t::kFileFormatWav;
    stFileSpec.eBitStreamType = CAudioFileIf::BitStream_t::kFileBitStreamInt16;
    stFileSpec.iNumChannels = 2;
    stFileSpec.fSampleRateInHz = 44100;
    
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::FileIoType_t::kFileRead, &stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath, std::fstream::out);

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    int sizeOfBuffer = sizeof(float) * kBlockSize;
    ppfAudioData = (float**) malloc(sizeof(float) * 2);
    *ppfAudioData = (float*) malloc(sizeOfBuffer);
    *(ppfAudioData+1) = (float*) malloc(sizeOfBuffer);
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    long long fileData = sizeOfBuffer;
    
    while(fileData!=0){
        phAudioFile->readData(ppfAudioData, fileData);
        
        for(int buf =0; buf<fileData; buf++){
            hOutputFile << ppfAudioData[0][buf] << "\t" << ppfAudioData[1][buf] <<endl;
        }

    }
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    delete *ppfAudioData;
    delete *(ppfAudioData+1);
    delete ppfAudioData;
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    // all done
    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

