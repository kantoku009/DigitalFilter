#include "./Filter/Filter.h"
#include "./Filter/Builder/ButterworthLowPassBuilder.h"
#include "./SoundInformation/WaveFormatOperator.h"

#include <iostream>
#include <string>
using namespace std;

bool runFilter(Filter& i_cFilter, SoundInformation& i_cSoundInformation, long i_lBegin, long i_lEnd, short i_shChannel )
{
    double a_dCurrentSample;
    
    for(long a_lIndex=i_lBegin; a_lIndex<=i_lEnd; a_lIndex++)
	{
		// サンプルを取得.
        a_dCurrentSample = i_cSoundInformation.readSampleFromMemory(a_lIndex,i_shChannel);
		// フィルタにサンプルを通す.
        a_dCurrentSample = i_cFilter.passFilter(a_dCurrentSample);
        // フィルタに通したサンプルを保存.
        i_cSoundInformation.writeSampleIntoMemory(a_dCurrentSample, a_lIndex, i_shChannel);
    }

	return true;
}

int main(int argc, char* argv[])
{
    string inFileName(argv[1]);
    double dBPerOctDown=atof(argv[2]);

	WaveFormatOperator a_cWaveFile;
	ButterworthLowPassFilterBuilder a_cBuilder;
	Filter* a_pcFilter = a_cBuilder.build();

    char tempStr[32];
	string outLowFileName;
    sprintf(tempStr,"%2.2f",dBPerOctDown);
    outLowFileName = "low-";
    outLowFileName += tempStr;
    outLowFileName += "dB.wav";

	//フィルタ名を取得.
	const char* a_strFilterName = a_pcFilter->description();

    cout << "begin: " << a_strFilterName << endl; 
    cout << "load file now" << endl;
    if(a_cWaveFile.readWaveFile(inFileName) == true){
        ////up Sampling
        //cout << "up sampling" << endl;
        //a_cWaveFile.setSampleRate(a_cWaveFile.getSampleRate()*2);

        //cout << "get outLowAmp and outLowPhase" << endl;
        //a_pcFilter.printCharacteristic("outLowAmp.txt","outLowPhase.txt");

        for(short chan=0;chan<a_cWaveFile.getNumChannels();chan++)
		{
            cout << "disposal channnel " << chan << endl;
			runFilter(*a_pcFilter, a_cWaveFile, 0, a_cWaveFile.getSamplesPerChannel(), chan);
        }
        
        ////down Sampling
        //cout << "down sampling" << endl;
        //a_cWaveFile.setSampleRate(a_cWaveFile.getSampleRate()/2);

        cout << "write file now" << endl;
        a_cWaveFile.writeWaveFile(outLowFileName);
    }else{
        cout << "error: load file" << endl;
    }
    cout << "finish: " << a_strFilterName << endl;
	delete a_pcFilter;
    
    
    return 0;
}

