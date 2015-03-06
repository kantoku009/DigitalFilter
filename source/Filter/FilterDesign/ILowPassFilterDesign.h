/**
 * @file	CLowPassFilterDesign.h
 * @brief	ローパスフィルタ設計部のインタフェース.
 */

#ifndef __ILOWPASS_FILTER_DESIGN_H__
#define __ILOWPASS_FILTER_DESIGN_H__

#include "./IFilterDesign.h"
/**
 * @brief	ローパスフィルタ設計部のインタフェース.
 * @note	Inteface
 */
class ILowPassFilterDesign : public IFilterDesign
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	ILowPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~ILowPassFilterDesign(){ }

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数.
	 */
	virtual double getCutoffFreq() const = 0;

protected:
	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	カットオフ周波数.
	 * @return	なし.
	 */
	virtual void setCutoffFreq(double i_dCutoffFreq) = 0;
};

#endif //__ILOWPASS_FILTER_DESIGN_H__

