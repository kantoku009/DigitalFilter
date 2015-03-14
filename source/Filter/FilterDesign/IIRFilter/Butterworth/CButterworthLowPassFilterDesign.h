/**
 * @file	CButterworthLowPassFilterDesign.h
 * @brief	バターワース特性 ローパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__
#define __CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__

#include <string>
#include <map>
using namespace std;

#include "../../ILowPassFilterDesign.h"
#include "./CButterworthCommon.h"
#include "../../CFilterConfig.h"

#define	BUTTERWORTH_LOWPASS_FILTER_DESIGN_NAME		"ButterworthLowPass"
#define BUTTERWORTH_LOWPASS_FILTER_CONFIG_FILENAME	"./config/ButterworthLowPassFilter.conf"

/**
 * @brief	バターワース特性 ローパスデジタルフィルタ設計部のクラス.
 */
class CButterworthLowPassFilterDesign : public ILowPassFilterDesign, public CButterworthCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthLowPassFilterDesign()
	{
		this->m_pbyFilterDesignName = BUTTERWORTH_LOWPASS_FILTER_DESIGN_NAME;
		this->m_pbyConfigFileName = BUTTERWORTH_LOWPASS_FILTER_CONFIG_FILENAME;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthLowPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計名を取得.
	 */
	virtual const char* description() const
	{
		return m_pbyFilterDesignName;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(this->m_dSampleRate);
		this->decisionPrototype(this->m_dPassFreq, this->m_dRippleGain, this->m_dStopFreq, this->m_dAttenuateGain);
		this->setCutoffFreq(m_dLowCutoffFreq);

		this->m_pcBlockDiagram = this->initLowTransferFunction(this->m_dLowCutoffFreq);
	}
	
	/**
	 * @brief	デジタルフィルタの設定を読み込む.
	 */
	virtual void readConfig()
	{
		//configファイルを読み込み.
		CFilterConfig a_cConfig;
		map<string,string> a_mapPairsTable = a_cConfig.getConfig(this->m_pbyConfigFileName);

		//mapを初期化するには コンパイル時に オプション -std=c++11 が必要.
		map<string, double*> a_mapConfigTable = 
		{
			{"SampleRate",		&this->m_dSampleRate},
			{"PassFreq",		&this->m_dPassFreq},
			{"RippleGain",		&this->m_dRippleGain},
			{"StopFreq",		&this->m_dStopFreq},
			{"AttenuateGain",	&this->m_dAttenuateGain},
			{"CutoffFreq",		&this->m_dLowCutoffFreq},
		};

		//コンフィグの設定.
		this->setConfig(a_mapPairsTable, a_mapConfigTable);

		//デバッグ用にコンフィグを出力.
		if(this->m_bIsDebug) this->printConfig();
	}

	/**
	 * @brief	デジタルフィルタにサンプル値を通す.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const
	{
		long a_lOrderNumber = this->getOrderNumber();
		bool a_bIsOrderNumberEven = (a_lOrderNumber%2 == 0)? true:false;

		long a_lNumSection = (a_bIsOrderNumberEven)? a_lOrderNumber/2 : (a_lOrderNumber-1)/2;
		a_lNumSection++;

		return this->transferFunction(a_lNumSection, i_dSample);
	}

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数.
	 */
	virtual double getCutoffFreq() const
	{
		return m_dLowCutoffFreq;
	}

	/**
	 * @brief	振幅特性と位相特性を出力.
	 * @param	const char* i_pbyAmplitudeFilename
	 * @param	const char* i_pbyPhaseFilename
	 * @return	なし.
	 * @note	ローカル変数.
	 *			a_lOrderNumber:伝達関数の次数.
	 *			a_lNumSection:伝達関数を１次または２次に分けたときのセクションの個数.
	 */
	virtual void printProperty(const char* i_pbyAmplitudeFilename, const char* i_pbyPhaseFilename) const
	{
		long a_lOrderNumber = this->getOrderNumber();
		long a_lNumSection = (0==a_lOrderNumber%2)? (a_lOrderNumber/2) : ((a_lOrderNumber-1)/2);
		a_lNumSection++;

		this->printCharacteristic(i_pbyAmplitudeFilename, i_pbyPhaseFilename, a_lNumSection);
	}

protected:
	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	カットオフ周波数.
	 * @return	なし.
	 */
	virtual void setCutoffFreq(double i_dCutoffFreq)
	{
		m_dLowCutoffFreq = i_dCutoffFreq;
	}


};

#endif //__CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__

