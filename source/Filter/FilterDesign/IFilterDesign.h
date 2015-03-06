/**
 * @file	IFilterDesign.h
 * @brief	デジタルフィルタ設計部のインタフェース.
 */
#ifndef __IFILTER_DESIGN_H__
#define __IFILTER_DESIGN_H__

/**
 * @brief	デジタルフィルタ設計部のインタフェース.
 * @note	Inteface
 */
class IFilterDesign
{
public:
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~IFilterDesign(){ }

	/**
	 * @brief	デジタルフィルタ設計部名を取得.
	 */
	virtual const char* description() const = 0;

	/**
	 * @brief	初期化.
	 */
	virtual void init() = 0;

	/**
	 * @brief	デジタルフィルタの設定部を読み込む.
	 */
	virtual void readConfig() = 0;

	/**
	 * @brief	デジタルフィルタにサンプル値を通す.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const = 0;
};

#endif	//__IFILTER_DESIGN_H__

