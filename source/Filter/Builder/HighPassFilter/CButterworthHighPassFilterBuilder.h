/**
 * @file	CButterworthHighPassFilterBuilder.h
 * @brief	バターワース特性 ハイパスフィルタを構築するクラス.
 */
#ifndef __CBUTTERWORTH_HIGH_PASS_FILTER_BUILDER_H__
#define __CBUTTERWORTH_HIGH_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Butterworth/CButterworthHighPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	バターワース特性 ハイパスフィルタを構築するクラス.
 */
class CButterworthHighPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthHighPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthHighPassFilterBuilder(){ }
    
protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CButterworthHighPassFilterDesign* a_pcFilterDesign = new CButterworthHighPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CBUTTERWORTH_HIGH_PASS_FILTER_BUILDER_H__

