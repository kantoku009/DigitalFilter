#include <iostream>
#include <string>
#include <cstring>
using namespace std;

#include "./SoundInformation/CWaveFormatOperator.h"
#include "./Filter/CFilter.h"

//フィルタービルダー
#include "./Filter/Builder/LowPassFilter/CButterworthLowPassFilterBuilder.h"
#include "./Filter/Builder/LowPassFilter/CChebycheffLowPassFilterBuilder.h"
#include "./Filter/Builder/BandPassFilter/CButterworthBandPassFilterBuilder.h"
#include "./Filter/Builder/BandPassFilter/CChebycheffBandPassFilterBuilder.h"
#include "./Filter/Builder/HighPassFilter/CButterworthHighPassFilterBuilder.h"
#include "./Filter/Builder/HighPassFilter/CChebycheffHighPassFilterBuilder.h"

bool runFilter(CFilter& i_cFilter, BSoundInformation& i_bSoundInformation, long i_lBegin, long i_lEnd, short i_shChannel )
{
    double a_dCurrentSample;
    
    for(long a_lIndex=i_lBegin; a_lIndex<=i_lEnd; a_lIndex++)
	{
		// サンプルを取得.
        a_dCurrentSample = i_bSoundInformation.readSampleFromMemory(a_lIndex,i_shChannel);
		// フィルタにサンプルを通す.
        a_dCurrentSample = i_cFilter.passFilter(a_dCurrentSample);
        // フィルタに通したサンプルを保存.
        i_bSoundInformation.writeSampleIntoMemory(a_dCurrentSample, a_lIndex, i_shChannel);
    }

	return true;
}

CFilter* createFilter(short i_shIndex)
{

	IFilterBuilder* a_piFilterBuilder=0;
	CFilter* a_pcFilter=0;

	switch (i_shIndex)
	{
		case 0:		//バターワース特性 ローパスフィルタ.
		a_piFilterBuilder = new CButterworthLowPassFilterBuilder();
		break;

		case 1:		//バターワース特性 バンドパスフィルタ.
		a_piFilterBuilder = new CButterworthBandPassFilterBuilder();
		break;

		case 2:		//バターワース特性 ハイパスフィルタ.
		a_piFilterBuilder = new CButterworthHighPassFilterBuilder();
		break;

		case 3:		//チェビシェフ特性 ローパスフィルタ.
		a_piFilterBuilder = new CChebycheffLowPassFilterBuilder();
		break;

		case 4:		//チェビシェフ特性 バンドパスフィルタ.
		a_piFilterBuilder = new CChebycheffBandPassFilterBuilder();
		break;

		case 5:		//チェビシェフ特性 ハイパスフィルタ.
		a_piFilterBuilder = new CChebycheffHighPassFilterBuilder();
		break;

		default:
		a_piFilterBuilder = 0;
		break;
	}

	if(0 != a_piFilterBuilder)
	{
		a_pcFilter = a_piFilterBuilder->build();	
		delete a_piFilterBuilder;
	}

	return a_pcFilter;
}

int main(int argc, char* argv[])
{
    string inFileName(argv[1]);
    double dBPerOctDown=atof(argv[2]);
	short a_shFilterIndex=atoi(argv[3]);

	////フィルタ ビルダー
	////バターワース特性 ローパスフィルタ.
	//CButterworthLowPassFilterBuilder a_cBuilder;
	////バターワース特性 バンドパスフィルタ.
	//CButterworthBandPassFilterBuilder a_cBuilder;
	////バターワース特性 ハイパスフィルタ.
	//CButterworthHighPassFilterBuilder a_cBuilder;
	////チェビシェフ特性 ローパスフィルタ.
	//CChebycheffLowPassFilterBuilder a_cBuilder;
	////チェビシェフ特性 バンドパスフィルタ.
	//CChebycheffBandPassFilterBuilder a_cBuilder;
	////チェビシェフ特性 ハイパスフィルタ.
	//CChebycheffHighPassFilterBuilder a_cBuilder;
	//CFilter* a_pcFilter = a_cBuilder.build();

	//フィルタを生成.
	CFilter* a_pcFilter = createFilter(a_shFilterIndex);
	if(0==a_pcFilter)
	{
		cout << "error: CFilter create failed." << endl;
		return -1;
	}

	//フィルタ名を取得.
	const char* a_strFilterName = a_pcFilter->description();

	//出力ファイル名を決定.
	string outFileName(a_strFilterName);
    char tempStr[32];
    sprintf(tempStr,"%2.2f",dBPerOctDown);
    outFileName += tempStr;
    outFileName += "dB.wav";

    cout << "load file now: " << inFileName << endl;
	CWaveFormatOperator a_cWaveFile;
    if(true == a_cWaveFile.readWaveFile(inFileName))
	{
        //cout << "get outLowAmp and outLowPhase" << endl;
        //a_pcFilter.printCharacteristic("outLowAmp.txt","outLowPhase.txt");

		// up sampling.
		//a_cWaveFile.setSampleRate(a_cWaveFile.getSampleRate() * 2);

		//フィルタ処理.
		cout << "disposal: " << a_strFilterName << endl; 
        for(short chan=0;chan<a_cWaveFile.getNumChannels();chan++)
		{
            cout << "  channnel " << chan << endl;
			runFilter(*a_pcFilter, a_cWaveFile, 0, a_cWaveFile.getSamplesPerChannel(), chan);
        }
        
		// down sampling.
		//a_cWaveFile.setSampleRate(a_cWaveFile.getSampleRate() / 2);

		//フィルタ処理後の音声を書き出す.
        cout << "write file now: " << outFileName << endl;
        a_cWaveFile.writeWaveFile(outFileName);
    }
	else
	{
        cout << "error: load file" << endl;
    }
    cout << "finish" << endl;
	delete a_pcFilter;
    
    return 0;
}

