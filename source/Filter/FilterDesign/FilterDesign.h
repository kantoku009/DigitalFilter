/**
 * @file	FilterDesign.h
 * @brief	デジタルフィルタ設計のクラス.
 * @note	デジタルフィルタの設計には以下がある.
 *				Butterworth
 *				Chebycheff
 */

#ifndef __FILTER_DESIGN_H__
#define __FILTER_DESIGN_H__

/**
 * @brief	デジタルフィルタ設計のクラス.
 * @note	Inteface
 */
class FilterDesign
{
public:
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~FilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計名を取得.
	 */
	virtual const char* description() const = 0;

	/**
	 * @brief	初期化.
	 */
	virtual void init() = 0;

	/**
	 * @brief	デジタルフィルタの設定を読み込む.
	 */
	virtual void readConfig() = 0;

	/**
	 * @brief	デジタルフィルタにサンプル値を通す.
	 */
	virtual double passFilter(double i_dSample) const = 0;
};

#endif	//__FILTER_DESIGN_H__

