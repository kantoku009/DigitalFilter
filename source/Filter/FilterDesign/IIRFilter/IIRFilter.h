/**
 * @file	IIRFilter.h
 * @brief	IIRフィルタのクラス
 */

#ifndef __IIR_FILTER_H__
#define __IIR_FILTER_H__

#include <cmath>
using namespace std;

#include "../BlockDiagram.h"
#include "../LowPassFilterDesign.h"

/**
 * @brief	IIRフィルタのクラス.
 * @note	Interface.
 */
class IIRFilter : public LowPassFilterDesign
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	IIRFilter()
	{
		this->m_pcBlockDiagram = 0;
		this->m_dSampleRate = 0.0;
		this->m_dPrototypeCutFreq = 0.0;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~IIRFilter()
	{
		if(this->m_pcBlockDiagram !=0)
		{
			delete [] this->m_pcBlockDiagram;
		}
	}

	/**
	 * @brief	ローパスフィルタの初期化.
	 */
	virtual void initLowPassFilter()
	{
	}

	/**
	 * @brief	ハイパスフィルタの初期化.
	 */
	virtual void initHighPassFilter()
	{
	}

	/**
	 * @brief	バンドパスフィルタの初期化.
	 */
	virtual void initBandPassFilter()
	{
	}

	/**
	 * @brief	ローパスフィルタにサンプル値を通す.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passedThroughLowFilter(double i_dSample) const
	{
		long a_lOrderNumber = this->getOrderNumber();
		bool a_bIsOrderNumberEven = (a_lOrderNumber%2 == 0)? true:false;

		long a_lNumSection = (a_bIsOrderNumberEven)? a_lOrderNumber/2 : (a_lOrderNumber-1)/2;
		a_lNumSection++;

		//バンドパスフィルタの場合
		//if(this->getFilterMode() == kBandpass)
		//{
		//	a_lNumSection *= 2;
		//}

		return this->transferFunction(a_lNumSection, i_dSample);
	}

	/**
	 * @brief	サンプリング周波数を設定.
	 * @param	double i_dSampleRate	サンプリング周波数.
	 * @return	なし.
	 */
	virtual void setSampleRate(double i_dSampleRate)
	{
		m_dSampleRate = i_dSampleRate;
	}

	/**
	 * @brief	サンプリング周波数を取得.
	 * @param	なし.
	 * @return	サンプリング周波数.
	 */
	virtual double getSampleRate() const
	{
		return this->m_dSampleRate;
	}

	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	カットオフ周波数[Hz].
	 * @return	なし.
	 */
	virtual void setCutoffFreq(double i_dCutoffFreq)
	{
		m_dCutoffFreq = i_dCutoffFreq;
	}

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数[Hz].
	 */
	virtual double getCutoffFreq() const
	{
		return m_dCutoffFreq;
	}

	/**
	 * @brief フィルタの次数を取得.
	 * @param なし.
	 * @return フィルタの次数.
	 */
	long getOrderNumber() const { return this->m_lOrderNumber; }

	/**
	 * @brief	伝達関数の振幅特性と位相特性をファイルに出力.
	 * @note	デバッグ用.
	 */
	virtual void printCharacteristic(char* i_pbyNameAmp, char* i_pbyNamePhase) const { }

protected:
	/**
	 * @brief	伝達関数にサンプルを通す.
	 * @param	long i_lNumSection	ブロックダイアグラムのセクション数.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double transferFunction(long i_lNumSection, double i_dSample) const
	{
		double a_dData = i_dSample;
		for(long a_lIndex=0;a_lIndex<i_lNumSection;a_lIndex++)
		{
			a_dData = this->m_pcBlockDiagram[a_lIndex].inject(a_dData);
		}

		return a_dData;
	}

	/**
	 * @brief フィルタの次数を設定.
	 * @param long inOrder フィルタの次数.
	 * @return なし.
	 */
	void setOrderNumber(long i_lOrder){ this->m_lOrderNumber = i_lOrder; }

	/**
	 * @brief	ローパスフィルタの伝達関数初期化.
	 */
	virtual BlockDiagram* initLowPassTransferFunction()
	{
		return 0;
	}

	/**
	 * @brief	ハイパスフィルタの伝達関数初期化.
	 */
	virtual BlockDiagram* initHighPassTransferFunction()
	{
		return 0;
	}

	/**
	 * @brief	バンドパスフィルタの伝達関数初期化.
	 */
	virtual BlockDiagram* initBandPassTransferFunction()
	{
		return 0;
	}

	/**
	 * @brief プロトタイプローパスフィルタを決定.
	 * @param double inPassFreq パスバンドの周波数[Hz]
	 * @param double inRippleGain 偏差の量[dB]
	 * @param double inStopFreq ストップバンドの周波数[Hz]
	 * @param double inAttenuateGain 減衰量[dB]
	 * @return なし.
	 */
	virtual void decisionPrototype(
					double inPassFreq,
					double inRippleGain,
					double inStopFreq,
					double inAttenuateGain)
	{
	}
    
	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数を取得.
	 * @param	なし.
	 * @return	プロトタイプローパスフィルタのカットオフ周波数.
	 */
	virtual double getPrototypeCutFreq() const
	{
		return this->m_dPrototypeCutFreq;
	}
    			
	/**
	 * @brief デジタルの周波数からアナログの周波数へ変換.
	 * @param double inOmega デジタルの周波数.
	 * @return アナログの周波数.
	 */
	virtual double digital2analog(double inOmega)
	{
		return 2*tan(inOmega/2);
	}

	/**
	 * @brief	ブロックダイアグラム.
	 */
	BlockDiagram* m_pcBlockDiagram;

	/**
	 * @brief	サンプリング周波数[Hz].
	 */
	double m_dSampleRate;

	/**
	 * @brief	カットオフ周波数[Hz].
	 */
	double m_dCutoffFreq;

	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数
	 */
	double m_dPrototypeCutFreq;

	/**
	 * @brief フィルタの次数.
	 */
	long m_lOrderNumber;
};

#endif //__IIR_FILTER_H__

