/**
 * @file	FilterBuilder.h
 * @brief	デジタルフィルタを構築するクラス.
 */

#ifndef __FILTER_BUILDER_H__
#define __FILTER_BUILDER_H__

#include "../Filter.h"

/**
 * @brief	デジタルフィルタを構築するクラス.
 */
class FilterBuilder
{
public:

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~FilterBuilder(){ }
    
	/**
	 * @brief	デジタルフィルタを構築する.
	 * @param	なし.
	 * @return	構築したFilterクラス.
	 */
	virtual Filter* build() = 0;

};

#endif	//__FILTER_BUILDER_H__

