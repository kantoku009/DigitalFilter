/**
 * @file	CChebycheffCommon.h
 * @brief	チェビシェフ特性のデジタルフィルタ(処理実体).
 */

#ifndef __CCHEBYCHEFF_COMMON_H__
#define __CCHEBYCHEFF_COMMON_H__

#include "../BIIRFilter.h"
#include "../../CBlockDiagram.h"

/**
 * @brief	チェビシェフ特性のデジタルフィルタ(処理実体).
 */
class CChebycheffCommon : public BIIRFilter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffCommon() { }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffCommon() { }

	/**
	 * @brief	ローパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	ローパスフィルタの伝達関数.
	 */
	CBlockDiagram* initLowTransferFunction(double inCutFreq);

	/**
	 * @brief	ハイパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	ハイパスフィルタの伝達関数.
	 */
	CBlockDiagram* initHighTransferFunction(double inCutFreq);

	/**
	 * @brief	バンドパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inLowCutFreq		低い方のカットオフ周波数.
	 * @param	double inHighCutFreq	高い方のカットオフ周波数.
	 * @return	バンドパスフィルタの伝達関数.
	 */
	CBlockDiagram* initBandTransferFunction(double inLowCutFreq,double inHighCutFreq);
    
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
	double getAlpha(long i);
	double getBeta(long i);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getLowGamma(double inCutFreq);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getHighGamma(double inCutFreq);
    
	double acosh(double theta){ return log(theta+sqrt((theta+1)*(theta-1))); }

	/**
	 * @brief	epsilon
	 */
	double m_dEpsilon;

	/**
	 * @brief	係数.
	 */
	double m_dAlpha0;
};

#endif	//__CCHEBYCHEFF_COMMON_H__

