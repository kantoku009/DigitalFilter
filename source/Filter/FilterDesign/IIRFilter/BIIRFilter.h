/**
 * @file	BIIRFilter.h
 * @brief	IIRフィルタ共通処理を集めたクラス
 */

#ifndef __CIIR_FILTER_H__
#define __CIIR_FILTER_H__

#include <cmath>
using namespace std;

#include "../CBlockDiagram.h"

/**
 * @brief	IIRフィルタ共通処理を集めたクラス.
 */
class BIIRFilter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	BIIRFilter()
	{
		this->m_pcBlockDiagram = 0;
		this->m_dSampleRate = 0.0;
		this->m_dPrototypeCutFreq = 0.0;
		this->m_dPrototypePassFreq = 0.0;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~BIIRFilter()
	{
		if(this->m_pcBlockDiagram !=0)
		{
			delete [] this->m_pcBlockDiagram;
		}
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
	 * @brief プロトタイプローパスフィルタを決定.
	 * @param double inPassFreq パスバンドの周波数[Hz]
	 * @param double inRippleGain 偏差の量[dB]
	 * @param double inStopFreq ストップバンドの周波数[Hz]
	 * @param double inAttenuateGain 減衰量[dB]
	 * @return なし.
	 */
	virtual void decisionPrototype(
						double i_dPassFreq,
						double i_dRippleGain,
						double i_dStopFreq,
						double i_dAttenuateGain){ }
    
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
	 * @brief	プロトタイプローパスフィルタのパス周波数を取得.
	 */
	virtual double getPrototypePassFreq()
	{
		return this->m_dPrototypePassFreq;
	}

	/**
	 * @brief デジタルの周波数からアナログの周波数へ変換.
	 * @param double i_dOmega デジタルの周波数.
	 * @return アナログの周波数.
	 */
	virtual double digital2analog(double i_dOmega)
	{
		return 2*tan(i_dOmega/2);
	}

	/**
	 * @brief	ブロックダイアグラム.
	 */
	CBlockDiagram* m_pcBlockDiagram;

	/**
	 * @brief	サンプリング周波数[Hz].
	 */
	double m_dSampleRate;

	/**
	 * @brief	カットオフ周波数[Hz].
	 */
	double m_dCutoffFreq;

	/**
	 * @brief	低い方のカットオフ周波数[Hz].
	 * @note	バンドパスフィルタのみで使用する.
	 */
	double m_dLowCutoffFreq;

	/**
	 * @brief	高い方のカットオフ周波数[Hz].
	 * @note	バンドパスフィルタのみで使用する.
	 */
	double m_dHighCutoffFreq;

	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数.
	 */
	double m_dPrototypeCutFreq;

	/**
	 * @brief	プロトタイプローパスフィルタのパス周波数.
	 */
	double m_dPrototypePassFreq;

	/**
	 * @brief フィルタの次数.
	 */
	long m_lOrderNumber;
};

#endif //__CIIR_FILTER_H__

