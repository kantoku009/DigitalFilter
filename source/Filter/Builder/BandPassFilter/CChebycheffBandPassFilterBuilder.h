/**
 * @file	CChebycheffBandPassFilterBuilder.h
 * @brief	チェビシェフ特性 バンドパスフィルタを構築するクラス.
 */
#ifndef __CCHEBYCHEFF_BAND_PASS_FILTER_BUILDER_H__
#define __CCHEBYCHEFF_BAND_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Chebycheff/CChebycheffBandPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	チェビシェフ特性 バンドパスフィルタを構築するクラス.
 */
class CChebycheffBandPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffBandPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffBandPassFilterBuilder(){ }

protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CChebycheffBandPassFilterDesign* a_pcFilterDesign = new CChebycheffBandPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CCHEBYCHEFF_BAND_PASS_FILTER_BUILDER_H__

