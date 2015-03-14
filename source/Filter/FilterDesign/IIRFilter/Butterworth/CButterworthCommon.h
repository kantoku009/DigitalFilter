/**
 * @file	CButterworthCommon.h
 * @brief	バターワース特性のデジタルフィルタ(処理実体).
 */
#ifndef __CBUTTERWORTH_COMMON_H__
#define __CBUTTERWORTH_COMMON_H__

#include "../BIIRFilter.h"
#include "../../CBlockDiagram.h"

/**
 * @brief	バターワース特性のデジタルフィルタ(処理実体).
 */
class CButterworthCommon : public BIIRFilter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthCommon()
	{
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthCommon()
	{
	}

	/**
	 * @brief	ローパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double i_dCutFreq	カットオフ周波数.
	 * @return	ローパスフィルタの伝達関数.
	 */
	CBlockDiagram* initLowTransferFunction(double i_dCutFreq);

	/**
	 * @brief	ハイパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double i_dCutFreq	カットオフ周波数.
	 * @return	ハイパスフィルタの伝達関数.
	 */
	CBlockDiagram* initHighTransferFunction(double i_dCutFreq);

	/**
	 * @brief	バンドパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double i_dLowCutFreq		低い方のカットオフ周波数.
	 * @param	double i_dHighCutFreq	高い方のカットオフ周波数.
	 * @return	バンドパスフィルタの伝達関数.
	 */
	CBlockDiagram* initBandTransferFunction(double i_dLowCutFreq, double i_dHighCutFreq);

	/**
	 * @brief	プロトタイプローパスフィルタを決定.
	 * @param	double i_dPassFreq		パスバンドの周波数[Hz]
	 * @param	double i_dRippleGain	偏差の量[dB]
	 * @param	double i_dStopFreq		ストップバンドの周波数[Hz]
	 * @param	double i_dAttenuateGain	減衰量[dB]
	 * @return	なし.
	 */
	virtual void decisionPrototype(
						double i_dPassFreq,
						double i_dRippleGain,
						double i_dStopFreq,
						double i_dAttenuateGain);
    
protected:
	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	伝達関数のΠ
	 * @return	伝達関数の係数.
	 */
	double getAlpha(long i);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double i_dCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getLowBeta(double i_dCutFreq);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double i_dCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getHighBeta(double i_dCutFreq);
    
};

#endif	//__CBUTTERWORTH_COMMON_H__

