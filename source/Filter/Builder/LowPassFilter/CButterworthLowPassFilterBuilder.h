/**
 * @file	CButterworthLowPassFilterBuilder.h
 * @brief	バターワース特性 ローパスフィルタを構築するクラス.
 */
#ifndef __CBUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__
#define __CBUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Butterworth/CButterworthLowPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	バターワース特性 ローパスフィルタを構築するクラス.
 */
class CButterworthLowPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthLowPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthLowPassFilterBuilder(){ }

protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CButterworthLowPassFilterDesign* a_pcFilterDesign = new CButterworthLowPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CBUTTERWORTH_LOW_PASS_FILTER_BUILDER_H__

