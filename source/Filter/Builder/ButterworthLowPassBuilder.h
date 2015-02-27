/**
 * @file	ButterworthLowPassFilterBuilder.h
 * @brief	バターワース特性のローパスフィルタを構築するクラス.
 */

#ifndef __BUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__
#define __BUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__

#include "../FilterType/LowPassFilterType.h"
#include "../FilterDesign/LowPassFilterDesign.h"
#include "../FilterDesign/IIRFilter/Butterworth.h"

#include "../Filter.h"
#include "./FilterBuilder.h"

/**
 * @brief	デジタルフィルタを構築するクラス.
 */
class ButterworthLowPassFilterBuilder : public FilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	ButterworthLowPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~ButterworthLowPassFilterBuilder(){ }
    
	/**
	 * @brief	デジタルフィルタを構築する.
	 * @param	なし.
	 * @return	構築したFilterクラス.
	 */
	virtual Filter* build()
	{
		//フィルタ設計としてバターワースフィルタを生成.
		LowPassFilterDesign* a_pcLowPassFilterDesign = new Butterworth();

		//フィルタタイプとしてローパスフィルタを生成.
		LowPassFilterType* a_pcLowPassFilterType = new LowPassFilterType(a_pcLowPassFilterDesign);

		//フィルタを生成.
		Filter* a_pcFilter = new Filter(a_pcLowPassFilterType);

		return a_pcFilter;
	}


};

#endif	//__BUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__

