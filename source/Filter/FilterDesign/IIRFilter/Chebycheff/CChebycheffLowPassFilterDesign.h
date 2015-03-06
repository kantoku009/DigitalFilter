/**
 * @file	CChebycheffLowPassFilterDesign.h
 * @brief	チェビシェフ特性 ローパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CCHEBYCHEFF_LOWPASSFILTER_DESIGN_H__
#define __CCHEBYCHEFF_LOWPASSFILTER_DESIGN_H__

#include "../../ILowPassFilterDesign.h"
#include "./CChebycheffCommon.h"

#define	CHEBYCHEFF_LOWPASS_FILTER_DESIGN_NAME	"ChebycheffLowPass"


//readConfig()でファイルから読み出したい変数.
//static double m_dSampleRate;
static double m_dPassFreq_ChebycheffLow;
static double m_dRippleGain_ChebycheffLow;
static double m_dStopFreq_ChebycheffLow;
static double m_dAttenuateGain_ChebycheffLow;
//static double m_dCutoffFreq;



/**
 * @brief	チェビシェフ特性 ローパスデジタルフィルタ設計部のクラス.
 */
class CChebycheffLowPassFilterDesign : public ILowPassFilterDesign, public CChebycheffCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffLowPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffLowPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const
	{
		return CHEBYCHEFF_LOWPASS_FILTER_DESIGN_NAME;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(m_dSampleRate);
		this->decisionPrototype(m_dPassFreq_ChebycheffLow, m_dRippleGain_ChebycheffLow, m_dStopFreq_ChebycheffLow, m_dAttenuateGain_ChebycheffLow);
		this->setCutoffFreq(m_dCutoffFreq);

		m_pcBlockDiagram = this->initLowTransferFunction(m_dCutoffFreq);
	}
	

	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
	 * @note	TBA.未実装.
	 */
	virtual void readConfig()
	{
		this->m_dSampleRate = 44.1*1000;

		m_dPassFreq_ChebycheffLow = 400.0;
		m_dRippleGain_ChebycheffLow = -1.0;
		m_dStopFreq_ChebycheffLow = 800.0;
		m_dAttenuateGain_ChebycheffLow = -48.0;

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

#endif //__CCHEBYCHEFF_LOWPASSFILTER_DESIGN_H__

