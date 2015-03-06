/**
 * @file	CButterworthLowPassFilterDesign.h
 * @brief	バターワース特性 ローパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__
#define __CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__

#include "../../ILowPassFilterDesign.h"
#include "./CButterworthCommon.h"

#define	BUTTERWORTH_LOWPASS_FILTER_DESIGN_NAME	"ButterworthLowPass"

//readConfig()でファイルから読み出したい変数.
//static double m_dSampleRate;
static double m_dPassFreq_ButterworthLow;
static double m_dRippleGain_ButterworthLow;
static double m_dStopFreq_ButterworthLow;
static double m_dAttenuateGain_ButterworthLow;
//static double m_dCutoffFreq;


/**
 * @brief	バターワース特性 ローパスデジタルフィルタ設計部のクラス.
 */
class CButterworthLowPassFilterDesign : public ILowPassFilterDesign, public CButterworthCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthLowPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthLowPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const
	{
		return BUTTERWORTH_LOWPASS_FILTER_DESIGN_NAME;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(m_dSampleRate);
		this->decisionPrototype(m_dPassFreq_ButterworthLow, m_dRippleGain_ButterworthLow, m_dStopFreq_ButterworthLow, m_dAttenuateGain_ButterworthLow);
		this->setCutoffFreq(m_dCutoffFreq);

		m_pcBlockDiagram = this->initLowTransferFunction(m_dCutoffFreq);
	}
	
	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
	 * @note	TBA.未実装.ファイルから読み込めるようにすること.
	 */
	virtual void readConfig()
	{
		this->m_dSampleRate = 44.1*1000;

		m_dPassFreq_ButterworthLow = 400.0;
		m_dRippleGain_ButterworthLow = -1.0;
		m_dStopFreq_ButterworthLow = 800.0;
		m_dAttenuateGain_ButterworthLow = -48.0;

		this->m_dCutoffFreq = 400.0;
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
		return m_dCutoffFreq;
	}

protected:
	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	カットオフ周波数.
	 * @return	なし.
	 */
	virtual void setCutoffFreq(double i_dCutoffFreq)
	{
		m_dCutoffFreq = i_dCutoffFreq;
	}


};

#endif //__CBUTTERWORTH_LOWPASSFILTER_DESIGN_H__

