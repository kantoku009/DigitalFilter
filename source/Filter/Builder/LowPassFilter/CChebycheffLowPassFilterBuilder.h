/**
 * @file	CChebycheffLowPassFilterBuilder.h
 * @brief	チェビシェフ特性 ローパスフィルタを構築するクラス.
 */
#ifndef __CCHEBYCHEFF_LOW_PASS_FILTER_BUILDER_H__
#define __CCHEBYCHEFF_LOW_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Chebycheff/CChebycheffLowPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	チェビシェフ特性 ローパスフィルタを構築するクラス.
 */
class CChebycheffLowPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffLowPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffLowPassFilterBuilder(){ }
    
protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CChebycheffLowPassFilterDesign* a_pcFilterDesign = new CChebycheffLowPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CCHEBYCHEFF_LOW_PASS_FILTER_BUILDER_H__

