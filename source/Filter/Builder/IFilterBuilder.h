/**
 * @file	IFilterBuilder.h
 * @brief	デジタルフィルタを構築するインタフェース.
 */
#ifndef __IFILTER_BUILDER_H__
#define __IFILTER_BUILDER_H__

#include "../FilterDesign/IFilterDesign.h"
#include "../CFilter.h"

/**
 * @brief	デジタルフィルタを構築するインタフェース.
 * @note	Interface
 */
class IFilterBuilder
{
public:

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~IFilterBuilder(){ }
    
	/**
	 * @brief	デジタルフィルタを構築.
	 * @param	なし.
	 * @return	構築したFilterクラス.
	 */
	virtual CFilter* build()
	{
		//デジタルフィルタ設計部を生成.
		this->m_piFilterDesign = this->buildFilterDesign();
		this->m_piFilterDesign->init();

		//デジタルフィルタを生成.
		this->m_pcFilter = new CFilter();
		this->m_pcFilter->init(this->m_piFilterDesign);

		return this->m_pcFilter;
	}

protected:
	/**
	 * @brief	フィルタ設計を構築.
	 */
	virtual IFilterDesign* buildFilterDesign() = 0;

private:
	/**
	 * @brief	デジタルフィルタ.
	 */
	CFilter*		m_pcFilter;

	/**
	 * @brief	デジタルフィルタ設計.
	 */
	IFilterDesign*	m_piFilterDesign;
};

#endif	//__IFILTER_BUILDER_H__

