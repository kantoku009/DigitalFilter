/**
 * @file	Butterworth.h
 * @brief	バターワースフィルタ.
 */
#ifndef __BUTTERWORTH_H__
#define __BUTTERWORTH_H__

#include "./IIRFilter.h"
#include "../BlockDiagram.h"

#include <cmath>
#include <complex>
using namespace std;

//#define PI 3.1415926535897932384626433832795
#define	FILTER_DESIGN_NAME	"Butterworth"

/**
 * @brief	バターワースフィルタ.
 */
class Butterworth : public IIRFilter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	Butterworth()
	{
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~Butterworth()
	{
	}
    
	/**
	 * @brief	デジタルフィルタ設計名を取得.
	 */
	virtual const char* description() const { return FILTER_DESIGN_NAME; }

	/**
	 * @brief	ローパスフィルタの初期化.
	 */
	virtual void initLowPassFilter();

	/**
	 * @brief	デジタルフィルタの設定を読み込む.
	 */
	virtual void readConfig();

	/**
	 * @brief	プロトタイプローパスフィルタを決定.
	 * @param	double inPassFreq		パスバンドの周波数[Hz]
	 * @param	double inRippleGain		偏差の量[dB]
	 * @param	double inStopFreq		ストップバンドの周波数[Hz]
	 * @param	double inAttenuateGain	減衰量[dB]
	 * @return	なし.
	 */
	virtual void decisionPrototype(
						double i_dPassFreq,
						double i_dRippleGain,
						double i_dStopFreq,
						double i_dAttenuateGain);
    
	/**
	 * @brief	伝達関数の振幅特性と位相特性をファイルに出力.
	 * @note	デバッグ用.
	 */
	virtual void printCharacteristic(char* i_pbyNameAmp, char* i_pbyNamePhase) const;

private:
	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	伝達関数のΠ
	 * @return	伝達関数の係数.
	 */
	double getAlpha(long i);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getLowBeta(double inCutFreq);

	/**
	 * @brief	伝達関数の係数を取得.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	伝達関数の係数.
	 */
	double getHighBeta(double inCutFreq);
    
	/**
	 * @brief	ローパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	ローパスフィルタの伝達関数.
	 */
	BlockDiagram* initLowTransferFunction(double inCutFreq);

	/**
	 * @brief	ハイパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	ハイパスフィルタの伝達関数.
	 */
	BlockDiagram* initHighTransferFunction(double inCutFreq);

	/**
	 * @brief	バンドパスフィルタの伝達関数の初期化と係数の決定.
	 * @param	double inLowCutFreq		低い方のカットオフ周波数.
	 * @param	double inHighCutFreq	高い方のカットオフ周波数.
	 * @return	バンドパスフィルタの伝達関数.
	 */
	BlockDiagram* initBandTransferFunction(double inLowCutFreq, double inHighCutFreq);
};

#endif	//__BUTTERWORTH_H__

