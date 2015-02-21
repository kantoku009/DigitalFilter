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
		this->mCoefficientA = 0;
		this->mCoefficientB = 0;
	}

	/**
	 * @brief	コピーコンストラクタ.
	 */
    BlockDiagram(const BlockDiagram &copy)
	{
		this->init(copy.getOrder(), copy.getCoefficientA(), copy.getCoefficientB());
	}

	/**
	 * @brief	デストラクタ.
	 */
    virtual ‾BlockDiagram()
	{
		delete [] this->mCoefficientA;
		delete [] this->mCoefficientB;
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
    int getOrder() const { return mOrder; }

	/**
	 * @brief	係数を取得.
	 */
    const double *getCoefficientA() const{ return mCoefficientA; }

	/**
	 * @brief	係数を取得.
	 */
    const double *getCoefficientB() const{ return mCoefficientB; }

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
		this->mPreviousSample.push_front(val);
	}

	/**
	 * @brief	以前のサンプル値を押し出す.
	 */
    void popPreviousSample()
	{
		this->mPreviousSample.pop_back();
	}

	/**
	 * @brief	以前のサンプル値を取り出す.
	 */
    double getPreviousSample(int n)
	{
		return this->mPreviousSample[n-1];
	}

	/**
	 * @brief	以前のサンプル値.
	 * @note	キューで実現する.
	 */
    deque< double > mPreviousSample;

	/**
	 * @brief	係数.
	 */
    double *mCoefficientA;

	/**
	 * @brief	係数.
	 */
    double *mCoefficientB;
    
	/**
	 * @brief	次数を設定.
	 */
    void setOrder(int order){ mOrder = order; }

	/**
	 * @brief	次数.
	 */
    int mOrder;
};

#endif

