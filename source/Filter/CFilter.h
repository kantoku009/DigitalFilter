/**
 * @file	CFilter.h
 * @brief	デジタルフィルタのクラス.
 */

#ifndef __CFILTER_H__
#define __CFILTER_H__

#include <string>
using namespace std;

#include "./FilterDesign/IFilterDesign.h"

/**
 * @brief	デジタルフィルタのクラス.
 */
class CFilter
{
public:

	/**
	 * @brief	コンストラクタ.
	 */
	CFilter()
	{
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CFilter()
	{
		delete m_piFilterDesign;
	}
    
	/**
	 * @brief	デジタルフィルタ名を取得.
	 */
	virtual const char* description() const
	{
		//デジタルフィルタ設計部名を取得.
		string a_strFilterDesignName = string(m_piFilterDesign->description());
		return a_strFilterDesignName.c_str();
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init(IFilterDesign* i_piFilterDesign)
	{
		this->setFilterDesign(i_piFilterDesign);
	}

	/**
	 * @brief	デジタルフィルタにサンプル値を通す.
	 * @param	double sample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const
	{
		return m_piFilterDesign->passFilter(i_dSample);
	}

	/**
	 * @brief	デジタルフィルタ設計部を取得.
	 * @param	なし.
	 * @return	デジタルフィルタ設計部.
	 */
	virtual IFilterDesign* getFilterDesign() const
	{
		return m_piFilterDesign;
	}

protected:
	/**
	 * @brief	デジタルフィルタ設計部を設定.
	 * @param	IFilterDesign* i_piFilterDesign	デジタルフィルタ設計部.
	 * @return	なし.
	 */
	virtual void setFilterDesign(IFilterDesign* i_piFilterDesign)
	{
		m_piFilterDesign = i_piFilterDesign;
	}

private:
	/**
	 * @brief	デジタルフィルタ設計部.
	 */
	IFilterDesign* m_piFilterDesign;
};

#endif	//__CFILTER_H__

