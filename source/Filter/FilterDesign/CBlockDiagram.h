/**
 * @file	CBlockDiagram.h
 * @brief	ブロックダイアグラム.
 */

#ifndef __CBLOCK_DIAGRAM_H__
#define __CBLOCK_DIAGRAM_H__

#include <deque>
using namespace std;

/**
 * @brief	ブロックダイアグラムのクラス.
 */
class CBlockDiagram
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CBlockDiagram()
	{
		this->setOrder(0);
		this->m_pdFeedforwardCoefficient = 0;
		this->m_pdFeedbackCoefficient = 0;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CBlockDiagram()
	{
		delete [] this->m_pdFeedforwardCoefficient;
		delete [] this->m_pdFeedbackCoefficient;
	}
    
	/**
	 * @brief	ダイアグラムの初期化.
	 * @param	int i_iOrder				ダイアグラムの次数.
	 * @param	const double* i_pdFeedforwardCoefficient	ダイアグラムの係数.フィードフォーワード係数.
	 * @param	const double* i_pdFeedbackCoefficient	ダイアグラムの係数.フィードバック係数.
	 * @return	なし.
	 */
	void init(int i_iOrder, const double* i_pdFeedforwardCoefficient, const double* i_pdFeedbackCoefficient);
    
	/**
	 * @brief	データをダイアグラムに注入.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	入力がダイアグラムを通り抜けた後のデータ.
	 */
	double inject(double i_dSample);
    
	/**
	 * @brief	次数の取得.
	 * @param	なし.
	 * @return	次数.
	 */
	int getOrder() const { return this->m_iOrder; }

	/**
	 * @brief	係数を取得.
	 */
	const double *getFeedforwardCoefficient() const{ return this->m_pdFeedforwardCoefficient; }

	/**
	 * @brief	係数を取得.
	 */
	const double *getFeedbackCoefficient() const{ return this->m_pdFeedbackCoefficient; }

private:
    
	/**
	 * @brief	以前のサンプル値を初期化.
	 */
	void initPreviousSample(int i_iNum)
	{
		for(int a_iIndex=0; a_iIndex<=i_iNum; a_iIndex++)
		{
			this->pushPreviousSample(0.0);
		}
	}

	/**
	 * @brief	以前のサンプル値を追加.
	 */
	void pushPreviousSample(double i_dSample)
	{
		this->m_quePreviousSample.push_front(i_dSample);
	}

	/**
	 * @brief	以前のサンプル値を押し出す.
	 */
	void popPreviousSample()
	{
		this->m_quePreviousSample.pop_back();
	}

	/**
	 * @brief	以前のサンプル値を取り出す.
	 */
	double getPreviousSample(int i_iIndex)
	{
		return this->m_quePreviousSample[i_iIndex-1];
	}

	/**
	 * @brief	以前のサンプル値.
	 * @note	キューで実現する.
	 */
	deque<double> m_quePreviousSample;

	/**
	 * @brief	フィードフォーワード係数.(IIR部).
	 */
	double *m_pdFeedforwardCoefficient;

	/**
	 * @brief	フィードバック係数.(FIR部).
	 */
	double *m_pdFeedbackCoefficient;
    
	/**
	 * @brief	次数を設定.
	 */
	void setOrder(int i_iOrder){ this->m_iOrder = i_iOrder; }

	/**
	 * @brief	ブロックダイアグラムの次数.
	 */
	int m_iOrder;
};

#endif	//__CBLOCK_DIAGRAM_H__

