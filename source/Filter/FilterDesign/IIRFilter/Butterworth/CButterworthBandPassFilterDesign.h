/**
 * @file	CButterworthBandPassFilterDesign.h
 * @brief	バターワース特性 バンドパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CBUTTERWORTH_BANDPASSFILTER_DESIGN_H__
#define __CBUTTERWORTH_BANDPASSFILTER_DESIGN_H__

#include "../../IBandPassFilterDesign.h"
#include "./CButterworthCommon.h"

#define	BUTTERWORTH_BANDPASS_FILTER_DESIGN_NAME	"ButterworthBandPass"

//readConfig()でファイルから読み出したい変数.
//static double m_dSampleRate;
static double m_dPassFreq_ButterworthBand;
static double m_dRippleGain_ButterworthBand;
static double m_dStopFreq_ButterworthBand;
static double m_dAttenuateGain_ButterworthBand;
//static double m_dCutoffFreq;


/**
 * @brief	バターワース特性 バンドパスデジタルフィルタ設計部のクラス.
 */
class CButterworthBandPassFilterDesign : public IBandPassFilterDesign, public CButterworthCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthBandPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthBandPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const
	{
		return BUTTERWORTH_BANDPASS_FILTER_DESIGN_NAME;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(m_dSampleRate);
		this->decisionPrototype(m_dPassFreq_ButterworthBand, m_dRippleGain_ButterworthBand, m_dStopFreq_ButterworthBand, m_dAttenuateGain_ButterworthBand);
		this->setLowCutoffFreq(m_dLowCutoffFreq);
		this->setHighCutoffFreq(m_dHighCutoffFreq);

		m_pcBlockDiagram = this->initBandTransferFunction(m_dLowCutoffFreq, m_dHighCutoffFreq);
	}
	
	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
	 * @note	TBA.未実装.ファイルから読み込めるようにすること.
	 */
	virtual void readConfig()
	{
		this->m_dSampleRate = 44.1*1000;

		m_dPassFreq_ButterworthBand = 400.0;
		m_dRippleGain_ButterworthBand = -1.0;
		m_dStopFreq_ButterworthBand = 800.0;
		m_dAttenuateGain_ButterworthBand = -48.0;

		this->m_dLowCutoffFreq = 400.0;
		this->m_dHighCutoffFreq = 2000.0;
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

