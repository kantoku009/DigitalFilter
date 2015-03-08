/**
 * @file	CButterworthBandPassFilterDesign.h
 * @brief	バターワース特性 バンドパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CBUTTERWORTH_BANDPASSFILTER_DESIGN_H__
#define __CBUTTERWORTH_BANDPASSFILTER_DESIGN_H__

#include "../../IBandPassFilterDesign.h"
#include "./CButterworthCommon.h"

#define	BUTTERWORTH_BANDPASS_FILTER_DESIGN_NAME		"ButterworthBandPass"
#define BUTTERWORTH_BANDPASS_FILTER_CONFIG_FILENAME	"./config/ButterworthBandPassFilter.conf"


/**
 * @brief	バターワース特性 バンドパスデジタルフィルタ設計部のクラス.
 */
class CButterworthBandPassFilterDesign : public IBandPassFilterDesign, public CButterworthCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthBandPassFilterDesign()
	{ 
		this->m_pbyFilterDesignName = BUTTERWORTH_BANDPASS_FILTER_DESIGN_NAME;
		this->m_pbyConfigFileName = BUTTERWORTH_BANDPASS_FILTER_CONFIG_FILENAME;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthBandPassFilterDesign(){ }

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
		this->setLowCutoffFreq(this->m_dLowCutoffFreq);
		this->setHighCutoffFreq(this->m_dHighCutoffFreq);

		this->m_pcBlockDiagram = this->initBandTransferFunction(this->m_dLowCutoffFreq, this->m_dHighCutoffFreq);
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
			{"LowCutoffFreq",	&this->m_dLowCutoffFreq},
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
		//this->m_dLowCutoffFreq = 400.0;
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
		a_lNumSection *= 2;

		return this->transferFunction(a_lNumSection, i_dSample);
	}

	/**
	 * @brief	低い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	低い方のカットオフ周波数.
	 */
	virtual double getLowCutoffFreq() const
	{
		return m_dLowCutoffFreq;
	}

	/**
	 * @brief	高い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	高い方のカットオフ周波数.
	 */
	virtual double getHighCutoffFreq() const
	{
		return m_dHighCutoffFreq;
	}

protected:
	/**
	 * @brief	低い方のカットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	低い方のカットオフ周波数.
	 * @return	なし.
	 */
	virtual void setLowCutoffFreq(double i_dCutoffFreq)
	{
		m_dLowCutoffFreq = i_dCutoffFreq;
	}

	/**
	 * @brief	高い方のカットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	高い方のカットオフ周波数.
	 * @return	なし.
	 */
	virtual void setHighCutoffFreq(double i_dCutoffFreq)
	{
		m_dHighCutoffFreq = i_dCutoffFreq;
	}

};

#endif //__CBUTTERWORTH_BANDPASSFILTER_DESIGN_H__

