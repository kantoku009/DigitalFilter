/**
 * @file	FilterType.h
 * @brief	デジタルフィルタ タイプ のクラス.
 * @note	フィルタのタイプには以下がある.
 *				LowPass
 *				HighPass
 *				BandPass
 */

#ifndef __FILTER_TYPE_H__
#define __FILTER_TYPE_H__

#include "../FilterDesign/FilterDesign.h"

/**
 * @brief	デジタルフィルタタイプのクラス.
 * @note	Inteface
 */
class FilterType
{
public:

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~FilterType(){ }

	/**
	 * @フィルタタイプ名を取得.
	 */
	virtual const char* description() const = 0;

	/**
	 * @brief	初期化.
	 */
	virtual void init(FilterDesign* i_pcFilterDesign) = 0;

	/**
	 * @brief	フィルタにサンプルを通す.
	 * @param	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const = 0;

	/**
	 * @brief	デジタルフィルタ設計方法を取得.
	 * @param	なし.
	 * @return	デジタル設計方法.
	 */
	virtual FilterDesign* getFilterDesign() const = 0;

protected:
	/**
	 * @brief	デジタルフィルタ設計方法を設定.
	 * @param	FilterDesign* i_icFilterDesign	デジタルフィルタ設計方法.
	 * @return	なし.
	 */
	virtual void setFillterDesign(FilterDesign* i_icFilterDesign) = 0;
};

#endif	//__FILTER_TYPE_H__

