#include <vector>
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

#include <iostream>
#include <string>
using namespace std;

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

IFilterBuilder* init_builder(short i_shIndex)
{
	IFilterBuilder* a_pcBuilder;
	vector<IFilterBuilder*> a_vecBuilder;

	//バターワース特性 ローパスフィルタ.
	a_pcBuilder = new CButterworthLowPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//バターワース特性 バンドパスフィルタ.
	a_pcBuilder = new CButterworthBandPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//バターワース特性 ハイパスフィルタ.
	a_pcBuilder = new CButterworthHighPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//チェビシェフ特性 ローパスフィルタ.
	a_pcBuilder = new CChebycheffLowPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//チェビシェフ特性 バンドパスフィルタ.
	a_pcBuilder = new CChebycheffBandPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//チェビシェフ特性 ハイパスフィルタ.
	a_pcBuilder = new CChebycheffHighPassFilterBuilder();
	a_vecBuilder.push_back(a_pcBuilder);

	//return a_vecBuilder[i_shIndex];
	return a_pcBuilder;
}

int main(int argc, char* argv[])
{
    string inFileName(argv[1]);
    double dBPerOctDown=atof(argv[2]);

	//フィルタ ビルダー
	//バターワース特性 ローパスフィルタ.
	//CButterworthLowPassFilterBuilder a_cBuilder;
	//バターワース特性 バンドパスフィルタ.
	//CButterworthBandPassFilterBuilder a_cBuilder;
	//バターワース特性 ハイパスフィルタ.
	//CButterworthHighPassFilterBuilder a_cBuilder;
	//チェビシェフ特性 ローパスフィルタ.
	//CChebycheffLowPassFilterBuilder a_cBuilder;
	//チェビシェフ特性 バンドパスフィルタ.
	//CChebycheffBandPassFilterBuilder a_cBuilder;
	//チェビシェフ特性 ハイパスフィルタ.
	CChebycheffHighPassFilterBuilder a_cBuilder;
	CFilter* a_pcFilter = a_cBuilder.build();

	//フィルタ名を取得.
	char a_strFilterName[256];
	strcpy(a_strFilterName, a_pcFilter->description());

	//出力ファイル名を決定.
	string outFileName(a_strFilterName);
    char tempStr[32];
    sprintf(tempStr,"%2.2f",dBPerOctDown);
    outFileName += tempStr;
    outFileName += "dB.wav";

    cout << "load file now: " << inFileName << endl;
	CWaveFormatOperator a_cWaveFile;
    if(a_cWaveFile.readWaveFile(inFileName) == true){
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
    }else{
        cout << "error: load file" << endl;
    }
    cout << "finish" << endl;
	delete a_pcFilter;
    
    
    return 0;
}

