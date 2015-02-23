/**
 * @file	BlockDiagram.h
 * @brief	ブロックダイアグラム.
 */

#ifndef __BLOCK_DIAGRAM_H__
#define __BLOCK_DIAGRAM_H__

#include <deque>
using namespace std;

/**
 * @brief	ブロックダイアグラムのクラス.
 */
class BlockDiagram
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	BlockDiagram()
	{
		this->setOrder(0);
		this->m_pdCoefficientA = 0;
		this->m_pdCoefficientB = 0;
	}

	/**
	 * @brief	コピーコンストラクタ.
	 */
	BlockDiagram(const BlockDiagram& i_cBlockDiagram)
	{
		this->init(i_cBlockDiagram.getOrder(), i_cBlockDiagram.getCoefficientA(), i_cBlockDiagram.getCoefficientB());
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~BlockDiagram()
	{
		delete [] this->m_pdCoefficientA;
		delete [] this->m_pdCoefficientB;
	}
    
	/**
	 * @brief	ダイアグラムの初期化.
	 * @param	int order		ダイアグラムの次数.
	 * @param	const double *a	ダイアグラムの係数.
	 * @param	const double *b	ダイアグラムの係数.
	 * @return	なし.
	 */
	void init(int order, const double *a, const double *b);
    
	/**
	 * @brief	データをダイアグラムに注入.
	 * @param	double sample	入力サンプル値.
	 * @return	入力がダイアグラムを通り抜けた後のデータ.
	 */
	double inject(double sample);
    
	/**
	 * @brief	次数の取得.
	 * @param	なし.
	 * @return	次数.
	 */
	int getOrder() const { return this->m_iOrder; }

	/**
	 * @brief	係数を取得.
	 */
	const double *getCoefficientA() const{ return this->m_pdCoefficientA; }

	/**
	 * @brief	係数を取得.
	 */
	const double *getCoefficientB() const{ return this->m_pdCoefficientB; }

	/**
	 * @brief	=演算子のオーバーロード.
	 */
	const BlockDiagram &operator=(const BlockDiagram &right);
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
	void pushPreviousSample(double val)
	{
		this->m_quePreviousSample.push_front(val);
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
	double getPreviousSample(int n)
	{
		return this->m_quePreviousSample[n-1];
	}

	/**
	 * @brief	以前のサンプル値.
	 * @note	キューで実現する.
	 */
	deque< double > m_quePreviousSample;

	/**
	 * @brief	係数.
	 */
	double *m_pdCoefficientA;

	/**
	 * @brief	係数.
	 */
	double *m_pdCoefficientB;
    
	/**
	 * @brief	次数を設定.
	 */
	void setOrder(int i_iOrder){ this->m_iOrder = i_iOrder; }

	/**
	 * @brief	次数.
	 */
	int m_iOrder;
};

#endif

