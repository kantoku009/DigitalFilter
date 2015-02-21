/**
 * @file	Chebycheff.h
 * @brief	チェビシェフフィルタ.
 */

#ifndef __CHEBYCHEFF_H__
#define __CHEBYCHEFF_H__

#include "Filter.h"

#include <cmath>
#include <complex>
using namespace std;

#define PI 3.1415926535897932384626433832795

/**
 * @brief	チェビシェフフィルタ.
 * @note	IIRフィルタの一種.
 */
class Chebycheff : public Filter{
public:
	/**
	 * @brief	コンストラクタ.
	 */
    Chebycheff()
	{
		this->mSampleRate = 0.0;
		this->mPrototypeCutFreq = 0.0;
	}

	/**
	 * @brief	デストラクタ.
	 */
    virtual ~Chebycheff()
	{
		if(0!=this->mSection)
		{
			delete [] this->mSection;
		}
	}

	/**
	 * @brief	サンプリング周波数を設定.
	 * @param	double inSampleRate	サンプリング周波数.
	 * @return	なし.
	 */
    void setSampleRate(double inSampleRate){ mSampleRate = inSampleRate; }

	/**
	 * @brief	サンプリング周波数を取得.
	 * @param	なし.
	 * @return	サンプリング周波数.
	 */
	double getSampleRate(){ return mSampleRate; }

 	/**
	 * @brief	プロトタイプローパスフィルタを決定.
	 * @param	double inPassFreq		パスバンドの周波数[Hz]
	 * @param	double inRippleGain		偏差の量[dB]
	 * @param	double inStopFreq		ストップバンドの周波数[Hz]
	 * @param	double inAttenuateGain	減衰量[dB]
	 * @return	なし.
	 */   
    void decisionPrototype(double inPassFreq,
                            double inRippleGain,
                            double inStopFreq,
                            double inAttenuateGain);
    
	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数を取得.
	 * @param	なし.
	 * @return	プロトタイプローパスフィルタのカットオフ周波数.
	 */
    double getPrototypeCutFreq(){ return mPrototypeCutFreq; }

	/**
	 * @brief	プロトタイプのカットオフ周波数を取得.
	 */
    double getPrototypePassFreq(){ return mPrototypePassFreq; }
    
	/**
	 * @brief	デジタルの周波数からアナログの周波数へ変換.
	 * @param	double inOmega	デジタルの周波数.
	 * @return	アナログの周波数.
	 */
    double digital2analog(double inOmega){ return 2*tan(inOmega/2); }

	/**
	 * @brief	伝達関数を初期化.
	 */
    void initTransferFunction();

	/**
	 * @brief	振幅特性と位相特性をファイルへ出力.
	 */
    void printCharacteristic(char *inAmpFileName,char *inPhaseFileName);
private:
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
    
    //double asinh(double theta){ return log(theta+sqrt(theta*theta+1)); }
    double acosh(double theta){ return log(theta+sqrt((theta+1)*(theta-1))); }

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
    BlockDiagram* initBandTransferFunction(double inLowCutFreq,double inHighCutFreq);
    
	/**
	 * @brief	epsilon
	 */
    double mEpsilon;

	/**
	 * @brief	係数.
	 */
    double mAlpha0;

	/**
	 * @brief	サンプリング周波数.
	 */
    double mSampleRate;

	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数.
	 */
    double mPrototypeCutFreq;

	/**
	 * @brief	プロトタイプのカットオフ周波数.
	 */
    double mPrototypePassFreq;
};

#endif

