/**
 * @file	LowPassFilterType.h
 * @brief	ローパスフィルタのクラス.
 */

#ifndef __LOWPASS_FILTER_TYPE_H__
#define __LOWPASS_FILTER_TYPE_H__

#include "./FilterType.h"
#include "../FilterDesign/FilterDesign.h"
#include "../FilterDesign/LowPassFilterDesign.h"

/**
 * @brief	フィルタタイプ名.
 */
#define	FILTER_TYPE_NAME	"LowPassFilter"

/**
 * @brief	ローパスフィルタのクラス.
 */
class LowPassFilterType : public FilterType
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	LowPassFilterType(LowPassFilterDesign* i_pcLowPassFilterDesign)
	{
		this->init(i_pcLowPassFilterDesign);
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~LowPassFilterType()
	{
		if(0!=m_pcLowPassFilterDesign)
		{
			delete m_pcLowPassFilterDesign;
		}
	}

	/**
	 * @brief	フィルタタイプ名を取得.
	 */
	virtual const char* description() const { return FILTER_TYPE_NAME; }

	/**
	 * @brief	初期化.
	 */
	virtual void init(FilterDesign* i_pcFilterDesign)
	{
		this->setFillterDesign(i_pcFilterDesign);
		i_pcFilterDesign->init();
	}

	/**
	 * @brief	デジタルフィルタ設計方法を取得.
	 * @param	なし.
	 * @return	デジタル設計方法.
	 */
	virtual FilterDesign* getFilterDesign() const
	{
		return ( static_cast<FilterDesign*>(m_pcLowPassFilterDesign) );
	}
    
	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double i_dCuttoffFreq	カットオフ周波数.
	 * @return	なし.
	 */
	virtual	void setCutoffFreq(double i_dCuttoffFreq)
	{
		if(0!=m_pcLowPassFilterDesign)
		{
			m_pcLowPassFilterDesign->setCutoffFreq(i_dCuttoffFreq);
		}
	}

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数.
	 */
	virtual double getCutOffFreq() const
	{
		if(0==m_pcLowPassFilterDesign) return 0;
		return m_pcLowPassFilterDesign->getCutoffFreq();
	}

	/**
	 * @brief	フィルタにサンプルを通す.
	 * @param	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const
	{
		return m_pcLowPassFilterDesign->passFilter(i_dSample);
	}


protected:
	/**
	 * @brief	デジタルフィルタ設計方法を設定.
	 * @param	FilterDesign i_icFilterDesign	デジタルフィルタ設計方法.
	 * @return	なし.
	 */
	virtual void setFillterDesign(FilterDesign* i_icFilterDesign)
	{
		m_pcLowPassFilterDesign = static_cast<LowPassFilterDesign*>(i_icFilterDesign);
	}

private:

	/**
	 * @brief	デジタルフィルタ設計
	 */
	LowPassFilterDesign* m_pcLowPassFilterDesign;
};

#endif	//__LOWPASS_FILTER_TYPE_H__

