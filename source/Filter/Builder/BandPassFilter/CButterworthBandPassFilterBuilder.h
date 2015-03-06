/**
 * @file	CButterworthBandPassFilterBuilder.h
 * @brief	バターワース特性 バンドパスフィルタを構築するクラス.
 */
#ifndef __CBUTTERWORTH_BAND_PASS_FILTER_BUILDER_H__
#define __CBUTTERWORTH_BAND_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Butterworth/CButterworthBandPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	バターワース特性 バンドパスフィルタを構築するクラス.
 */
class CButterworthBandPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CButterworthBandPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CButterworthBandPassFilterBuilder(){ }
    
protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CButterworthBandPassFilterDesign* a_pcFilterDesign = new CButterworthBandPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CBUTTERWORTH_BAND_PASS_FILTER_BUILDER_H__

