/**
 * @file	CBandPassFilterDesign.h
 * @brief	バンドパスフィルタ設計部のインタフェース.
 */

#ifndef __IBANDPASS_FILTER_DESIGN_H__
#define __IBANDPASS_FILTER_DESIGN_H__

#include "./IFilterDesign.h"
/**
 * @brief	バンドパスフィルタ設計部のインタフェース.
 * @note	Inteface
 */
class IBandPassFilterDesign : public IFilterDesign
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	IBandPassFilterDesign(){ }

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~IBandPassFilterDesign(){ }

	/**
	 * @brief	低い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	低い方のカットオフ周波数.
	 */
	virtual double getLowCutoffFreq() const = 0;

	/**
	 * @brief	高い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	高い方のカットオフ周波数.
	 */
	virtual double getHighCutoffFreq() const = 0;

protected:
	/**
	 * @brief	低い方のカットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	低い方のカットオフ周波数.
	 * @return	なし.
	 */
	virtual void setLowCutoffFreq(double i_dCutoffFreq) = 0;

	/**
	 * @brief	高い方のカットオフ周波数を設定.
	 * @param	double i_dCutoffFreq	高い方のカットオフ周波数.
	 * @return	なし.
	 */
	virtual void setHighCutoffFreq(double i_dCutoffFreq) = 0;
};

#endif //__IBANDPASS_FILTER_DESIGN_H__

