/**
 * @file	CButterworthHighPassFilterDesign.h
 * @brief	バターワース特性 ハイパスデジタルフィルタ設計部のクラス.
 */
#ifndef __CBUTTERWORTH_HIGHPASSFILTER_DESIGN_H__
#define __CBUTTERWORTH_HIGHPASSFILTER_DESIGN_H__

#include "../../IHighPassFilterDesign.h"
#include "./CButterworthCommon.h"

#define	BUTTERWORTH_HIGHPASS_FILTER_DESIGN_NAME	"ButterworthHighPass"

//readConfig()でファイルから読み出したい変数.
//static double m_dSampleRate;
static double m_dPassFreq_ButterworthHigh;
static double m_dRippleGain_ButterworthHigh;
static double m_dStopFreq_ButterworthHigh;
static double m_dAttenuateGain_ButterworthHigh;
//static double m_dCutoffFreq;


/**
 * @brief	バターワース特性 ハイパスデジタルフィルタ設計部のクラス.
 */
class CButterworthHighPassFilterDesign : public IHighPassFilterDesign, public CButterworthCommon
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthHighPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthHighPassFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const
	{
		return BUTTERWORTH_HIGHPASS_FILTER_DESIGN_NAME;
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->readConfig();

		this->setSampleRate(m_dSampleRate);
		this->decisionPrototype(m_dPassFreq_ButterworthHigh, m_dRippleGain_ButterworthHigh, m_dStopFreq_ButterworthHigh, m_dAttenuateGain_ButterworthHigh);
		this->setCutoffFreq(m_dCutoffFreq);

		m_pcBlockDiagram = this->initHighTransferFunction(m_dCutoffFreq);
	}
	
	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
	 * @note	TBA.未実装.ファイルから読み込めるようにすること.
	 */
	virtual void readConfig()
	{
		this->m_dSampleRate = 44.1*1000;

		m_dPassFreq_ButterworthHigh = 400.0;
		m_dRippleGain_ButterworthHigh = -1.0;
		m_dStopFreq_ButterworthHigh = 800.0;
		m_dAttenuateGain_ButterworthHigh = -48.0;

		this->m_dCutoffFreq = 2000.0;
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

protected:

};

#endif //__CBUTTERWORTH_HIGHPASSFILTER_DESIGN_H__

