/**
 * @file	LowPassFilterDesign.h
 * @brief	ローパスデジタルフィルタ設計のクラス.
 * @note	フィルタの設計には以下がある.
 *				Butterworth
 *				Chebycheff
 *				...
 */

#ifndef __LOW_PASS_FILTER_DESIGN_H__
#define __LOW_PASS_FILTER_DESIGN_H__

#include "./FilterDesign.h"
/**
 * @brief	デジタルフィルタタイプのクラス.
 * @note	Inteface
 */
class LowPassFilterDesign : public FilterDesign
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	LowPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~LowPassFilterDesign(){ }

	/**
	 * @brief	初期化.
	 */
	virtual void init()
	{
		this->initLowPassFilter();
	}

	/**
	 * @biref	ローパスフィルタの初期化.
	 */
	virtual void initLowPassFilter() = 0;

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数.
	 */
	virtual double getCutoffFreq() const = 0;

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	double i_dCutoffFreq	カットオフ周波数.
	 * @return	なし.
	 */
	virtual void setCutoffFreq(double i_dCutoffFreq) = 0;

	/**
	 * @brief	フィルタにサンプル値を通す.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const
	{
		return this->passedThroughLowFilter(i_dSample);
	}

	/**
	 * @brief	ローパスフィルタにサンプル値を通す.
	 */
	virtual double passedThroughLowFilter(double i_dSample) const = 0;
};

#endif //__LOW_PASS_FILTER_DESIGN_H__

