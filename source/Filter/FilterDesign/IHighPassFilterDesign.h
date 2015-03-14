/**
 * @file	CHighPassFilterDesign.h
 * @brief	ハイパスフィルタ設計のインタフェース.
 */
#ifndef __IHIGHPASS_FILTER_DESIGN_H__
#define __IHIGHPASS_FILTER_DESIGN_H__

#include "./IFilterDesign.h"

/**
 * @brief	ハイパスフィルタ設計のインタフェース.
 * @note	Inteface
 */
class IHighPassFilterDesign : public IFilterDesign
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	IHighPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~IHighPassFilterDesign(){ }

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

#endif //__IHIGHPASS_FILTER_DESIGN_H__

