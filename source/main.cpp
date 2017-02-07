#include <iostream>
#include <string>
#include <cstring>		//atof(), atoi()を使用したい.
using namespace std;

#include "./SoundInformation/source/CWaveFile.h"
#include "./Filter/CFilter.h"

//フィルタービルダー
#include "./Filter/Builder/LowPassFilter/CButterworthLowPassFilterBuilder.h"
#include "./Filter/Builder/LowPassFilter/CChebycheffLowPassFilterBuilder.h"
#include "./Filter/Builder/BandPassFilter/CButterworthBandPassFilterBuilder.h"
#include "./Filter/Builder/BandPassFilter/CChebycheffBandPassFilterBuilder.h"
#include "./Filter/Builder/HighPassFilter/CButterworthHighPassFilterBuilder.h"
#include "./Filter/Builder/HighPassFilter/CChebycheffHighPassFilterBuilder.h"

/**
 * @brief	サンプル値をフィルタへ通す.
 * @param	CFilter& i_cFilter
 * @param	BSoundInformation& i_bSoundInformation
 * @param	long i_lBegin
 * @param	long i_lEnd
 * @param	short i_shChannel 
 * @return	成功/失敗.
 */
bool runFilter(CFilter& i_cFilter, BSoundInformation& i_bSoundInformation, long i_lBegin, long i_lEnd, short i_shChannel )
{
	double a_dCurrentSample;

	for(long a_lIndex=i_lBegin; a_lIndex<=i_lEnd; a_lIndex++)
	{
		// サンプルを取得.
		a_dCurrentSample = i_bSoundInformation.getSample(a_lIndex,i_shChannel);
		// フィルタにサンプルを通す.
		a_dCurrentSample = i_cFilter.passFilter(a_dCurrentSample);
		// フィルタに通したサンプルを保存.
		i_bSoundInformation.setSample(a_dCurrentSample, a_lIndex, i_shChannel);
	}

	return true;
}

/**
 * @brief	フィルタを生成.
 * @param	short i_shIndex	フィルタの種類.
 * @return	生成したフィルタ.
 */
CFilter* createFilter(short i_shIndex)
{
	//フィルタビルダーの生成.
	IFilterBuilder* a_piFilterBuilderTable[] = 
	{
		new CButterworthLowPassFilterBuilder(),		//バターワース特性 ローパスフィルタ ビルダー.
		new CButterworthBandPassFilterBuilder(),	//バターワース特性 バンドパスフィルタ ビルダー.
		new CButterworthHighPassFilterBuilder(),	//バターワース特性 ハイパスフィルタ ビルダー.
		new CChebycheffLowPassFilterBuilder(),		//チェビシェフ特性 ローパスフィルタ ビルダー.
		new CChebycheffBandPassFilterBuilder(),		//チェビシェフ特性 バンドバスフィルタ ビルダー.
		new CChebycheffHighPassFilterBuilder(),		//チェビシェフ特性 ハイパスフィルタ ビルダー.
	};
	
	//添え字の範囲チェック.
	short a_shMax = sizeof(a_piFilterBuilderTable) / sizeof(a_piFilterBuilderTable[0]);
	if(i_shIndex >= a_shMax) return 0;

	//フィルタビルダーを取得.
	IFilterBuilder* a_piFilterBuilder = a_piFilterBuilderTable[i_shIndex];

	//フィルタを生成.
	CFilter* a_pcFilter=0;
	if(0 != a_piFilterBuilder)
	{
		a_pcFilter = a_piFilterBuilder->build();	
	}

	//フィルタビルダーのメモリを解放.
	for(short a_shIndex=0; a_shIndex<a_shMax; a_shIndex++)
	{
		delete a_piFilterBuilderTable[a_shIndex];
	}

	//生成したフィルタを返す.
	return a_pcFilter;
}

int main(int argc, char* argv[])
{
	string inFileName(argv[1]);
	short a_shFilterIndex=atoi(argv[2]);

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
	outFileName += ".wav";

	//フィルタの振幅特性と位相特性を出力.
	//cout << "Print AmplitudeProperty and PhaseProperty" << endl;
	//const string a_strAmplitudeFilename = string(a_strFilterName) + string("_AmplitudeProperty.csv");
	//const string a_strPhaseFilename = string(a_strFilterName) + string("_PhaseProperty.csv");
	//a_pcFilter->printProperty(a_strAmplitudeFilename.c_str(),a_strPhaseFilename.c_str());

	cout << "load file now: " << inFileName << endl;
	CWaveFile a_cWaveFile;
	if(true == a_cWaveFile.read(inFileName))
	{
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
		a_cWaveFile.write(outFileName);
	}
	else
	{
		cout << "error: load file" << endl;
	}
	cout << "finish" << endl;
	delete a_pcFilter;
    
	return 0;
}

