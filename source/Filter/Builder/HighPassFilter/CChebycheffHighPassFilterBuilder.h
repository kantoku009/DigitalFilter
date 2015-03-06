/**
 * @file	CChebycheffHighPassFilterBuilder.h
 * @brief	チェビシェフ特性 ハイパスフィルタを構築するクラス.
 */
#ifndef __CCHEBYCHEFF_HIGH_PASS_FILTER_BUILDER_H__
#define __CCHEBYCHEFF_HIGH_PASS_FILTER_BUILDER_H__

#include "../../FilterDesign/IIRFilter/Chebycheff/CChebycheffHighPassFilterDesign.h"
#include "../../FilterDesign/IFilterDesign.h"
#include "../IFilterBuilder.h"

/**
 * @brief	チェビシェフ特性 ハイパスフィルタを構築するクラス.
 */
class CChebycheffHighPassFilterBuilder : public IFilterBuilder
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CChebycheffHighPassFilterBuilder(){ }
	
	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CChebycheffHighPassFilterBuilder(){ }
    
protected:
	/**
	 * @brief	デジタルフィルタ設計：CButterworth を生成.
	 */
	virtual IFilterDesign* buildFilterDesign()
	{
		CChebycheffHighPassFilterDesign* a_pcFilterDesign = new CChebycheffHighPassFilterDesign();
		return static_cast<IFilterDesign*>(a_pcFilterDesign);
	}
};

#endif	//__CCHEBYCHEFF_HIGH_PASS_FILTER_BUILDER_H__

