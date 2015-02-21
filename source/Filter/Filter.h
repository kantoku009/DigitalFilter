/**
 * @file	Filter.h
 * @brief	デジタルフィルタのクラス.
 */

#ifndef __FILTER_H__
#define __FILTER_H__

#include "BlockDiagram.h"

/**
 * @brief	フィルタモード.
 */
typedef enum
{
	kLowpass,	// Low  pass filter.
	kHighpass,	// High pass filter.
	kBandpass,	// Band pass filter.
}FilterMode;


class FilterError{
public:
    FilterError(char *message)
    {
        int len;
        
        len = strlen(message);
        mMessage = new char [len+1];
        strcpy(mMessage,message);
    }
    const char *what() const { return mMessage; }
private:
    char *mMessage;
};

/**
 * @brief	デジタルフィルタのクラス.
 */
class Filter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
    Filter()
	{
		mSection = 0;
    
		mKindFilter = kLowpass;
		mOrderNumber = 0;
		mLowCutoffFreq = 0;
		mHighCutoffFreq = 0;
	}

	/**
	 * @brief	デストラクタ.
	 */
    virtual ~Filter(){ }
    
	/**
	 * @brief	フィルタモードを設定.
	 * @note	モードに関しては, FilterModeを参照.
	 */
    void selectFilterMode(FilterMode inKindFilter){ mKindFilter = inKindFilter; }
	/**
	 * @brief	フィルタモードを取得.
	 */
    FilterMode getFilterMode(){ return mKindFilter; }
    
	/**
	 * @brief	カットオフ周波数を設定.
	 * @param	double inCutFreq	カットオフ周波数.
	 * @return	なし.
	 * @note	このメンバ関数はモードがLowpassまたはHighpassフィルタのときに使用する.
	 *			Bandpassの時には使えない.
	 */
    void setCutoffFreq(double inCutFreq);

	/**
	 * @brief	カットオフ周波数を取得.
	 * @param	なし.
	 * @return	カットオフ周波数.
	 * @note	このメンバ関数はモードがLowpassまたはHighpassフィルタのときに使用する.
	 *			Bandpassの時には使えない.
	 */
    double getCutoffFreq() const;
    
	/**
	 * @brief	高い方と低い方のカットオフ周波数を設定.
	 * @param	double inLowCutFreq
	 * @param	double inHighCutFreq
	 * @return	なし.
	 * @note	このメンバ関数はモードがBandpassフィルタのときに使用する.
	 *			Lowpass,Highpassの時には使えない
	 */
    void setCutoffFreq(double inLowCutFreq,double inHighCutFreq);

	/**
	 * @brief	低い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	低い方のカットオフ周波数.
	 * @note	このメンバ関数はモードがBandpassフィルタのときに使用する.
	 *			Lowpass,Highpassの時には使えない.
	 */
    double getLowCutoffFreq() const;

	/**
	 * @brief	高い方のカットオフ周波数を取得.
	 * @param	なし.
	 * @return	高い方のカットオフ周波数.
	 * @note	このメンバ関数はモードがBandpassフィルタのときに使用する.
	 *			Lowpass,Highpassの時には使えない.
	 */
    double getHighCutoffFreq() const;
    
	/**
	 * @brief	フィルタの次数を取得.
	 * @param	なし.
	 * @return	フィルタの次数.
	 */
    long getOrderNumber() const { return mOrderNumber; }
    
	/**
	 * @brief	フィルタを実行する.
	 * @param	double sample	処理するサンプル値.
	 * @return	フィルタを通したサンプル値.
	 */
    double passFilter(double sample);
    
protected:
	/**
	 * @brief	フィルタの次数を設定.
	 * @param	long inOrder	フィルタの次数.
	 * @return	なし.
	 */
    void setOrderNumber(long inOrder){ mOrderNumber = inOrder; }

	/**
	 * @brief	ブロックダイアグラム.
	 */
    BlockDiagram *mSection;
    
private:
	/**
	 * @brief	伝達関数.
	 * @param	double valSample	伝達関数に渡すサンプル値.
	 * @return	伝達関数を通したサンプル値.
	 */
    double transferFunction(double valSample);
    
	/**
	 * @brief	フィルタのモード.
	 */
    FilterMode mKindFilter;

	/**
	 * @brief	フィルタの次数.
	 */
    long mOrderNumber;

	/**
	 * @brief	低い方のカットオフ周波数.
	 * @note	以下のフィルタモードで使用する.
	 *			  ・kLowpass
	 *			  ・kBandpass
	 */

    double mLowCutoffFreq;
	/**
	 * @brief	高い方のカットオフ周波数.
	 * @note	以下のフィルタモードで使用する.
	 *			  ・kHighpass
	 *			  ・kBandpass
	 */
	double mHighCutoffFreq;
};

#endif

