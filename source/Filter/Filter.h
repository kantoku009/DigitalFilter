/**
 * @file	Filter.h
 * @brief	デジタルフィルタのクラス.
 */

#ifndef __FILTER_H__
#define __FILTER_H__

#include <string>
using namespace std;

#include "./FilterType/FilterType.h"

/**
 * @brief	デジタルフィルタのクラス.
 */
class Filter
{
public:

	/**
	 * @brief	コンストラクタ.
	 */
	Filter(FilterType* i_icFilterType)
	{
		this->init(i_icFilterType);
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~Filter()
	{
		delete m_icFilterType;
	}
    
	/**
	 * @brief	フィルタ名を取得.
	 */
	virtual const char* description() const
	{
		string a_strFilterTypeName = string(m_icFilterType->description());
		string a_strFilterDesignName = string(m_icFilterType->getFilterDesign()->description());
		string a_strFilterName = a_strFilterDesignName + a_strFilterTypeName;
		return a_strFilterName.c_str();
	}

	/**
	 * @brief	初期化.
	 */
	virtual void init(FilterType* i_icFilterType)
	{
		this->setFilterType(i_icFilterType);
	}

	/**
	 * @brief	フィルタにサンプル値を通す.
	 * @param	double sample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double passFilter(double i_dSample) const
	{
		return m_icFilterType->passFilter(i_dSample);
	}

	/**
	 * @brief	フィルタタイプを取得.
	 * @param	なし.
	 * @return	フィルタタイプ.
	 */
	virtual FilterType* getFilterType() const
	{
		return m_icFilterType;
	}

protected:

	/**
	 * @brief	フィルタタイプを設定.
	 * @param	FilterType* i_icFilterType フィルタタイプ.
	 * @return	なし.
	 */
	virtual void setFilterType(FilterType* i_icFilterType)
	{
		m_icFilterType = i_icFilterType;
	}

private:

	/**
	 * @brief	フィルタタイプ.
	 */
	FilterType* m_icFilterType;
};

#endif	//__FILTER_H__

