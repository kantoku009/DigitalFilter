/**
 * @file	CChebycheffHighPassFilterDesign.h
 * @brief	チェビシェフ特性 ハイパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CCHEBYCHEFF_HIGHPASSFILTER_DESIGN_H__
#define __CCHEBYCHEFF_HIGHPASSFILTER_DESIGN_H__

#include <string>
#include <map>
using namespace std;

#include "../../IHighPassFilterDesign.h"
#include "./CChebycheffCommon.h"

#define	CHEBYCHEFF_HIGHPASS_FILTER_DESIGN_NAME		"ChebycheffHighPass"
#define CHEBYCHEFF_HIGHPASS_FILTER_CONFIG_FILENAME	"./config/ChebycheffHighPassFilter.conf"


/**
 * @brief	チェビシェフ特性 ハイパスデジタルフィルタ設計部のクラス.
 */
class CChebycheffHighPassFilterDesign : public IHighPassFilterDesign, public CChebycheffCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffHighPassFilterDesign()
	{
		this->m_pbyFilterDesignName = CHEBYCHEFF_HIGHPASS_FILTER_DESIGN_NAME;
		this->m_pbyConfigFileName = CHEBYCHEFF_HIGHPASS_FILTER_CONFIG_FILENAME;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffHighPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const
	{
		return this->m_pbyFilterDesignName;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(this->m_dSampleRate);
		this->decisionPrototype(this->m_dPassFreq, this->m_dRippleGain, this->m_dStopFreq, this->m_dAttenuateGain);
		this->setCutoffFreq(this->m_dHighCutoffFreq);

		this->m_pcBlockDiagram = this->initHighTransferFunction(this->m_dHighCutoffFreq);
	}
	

	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
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
			{"HighCutoffFreq",	&this->m_dHighCutoffFreq},
		};

		//コンフィグの設定.
		this->setConfig(a_mapPairsTable, a_mapConfigTable);

		//デバッグ用にコンフィグを出力.
		if(this->m_bIsDebug) this->printConfig();

		//this->m_dSampleRate = 44.1*1000;
		//this->m_dPassFreq = 400.0;
		//this->m_dRippleGain = -1.0;
		//this->m_dStopFreq = 800.0;
		//this->m_dAttenuateGain = -48.0;
		//this->m_dHighCutoffFreq = 2000.0;
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
		return m_dHighCutoffFreq;
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
		m_dHighCutoffFreq = i_dCutoffFreq;
	}
};

#endif //__CCHEBYCHEFF_HIGHPASSFILTER_DESIGN_H__

