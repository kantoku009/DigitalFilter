#include "Filter.h"

#include <iostream>
using namespace std;


/**********************************************************************
* setCutoffFreq : カットオフ周波数を設定
*
*引数
*   inCutFreq : Lowpass またはHighpass のカットオフ周波数
*
*ローカル変数
*   err : 
*
*返り値
*   なし
*
*備考
*   このメンバ関数はモードがLowpassまたはHighpassフィルタのときに使用
*   Bandpassの時には使えない
*
**********************************************************************/
void Filter::setCutoffFreq(double inCutFreq)
{
    try{
        switch (mKindFilter){
            case kLowpass:
            mLowCutoffFreq = inCutFreq;
            break;
            
            case kHighpass:
            mHighCutoffFreq = inCutFreq;
            break;
            
            case kBandpass:
            throw FilterError((char*)"Filter error: filter mode Lowpass or Hipass. do not operate" );
            
            default:
            throw FilterError((char*)"Filter error: cause unknown" );
        }
    }catch(FilterError err){
        cerr << err.what() << endl;
    }
}


/**********************************************************************
* getCutoffFreq : カットオフ周波数を取得
*
*引数
*   なし
*
*ローカル変数
*   err : 
*
*返り値
*   LowpassまたはHighpassのカットオフ周波数
*
*備考
*   このメンバ関数はモードがLowpassまたはHighpassフィルタのときに使用
*   Bandpassの時には使えない
*
**********************************************************************/
double Filter::getCutoffFreq() const
{
    try{
        switch (mKindFilter){
            case kLowpass:
            return mLowCutoffFreq;
            
            case kHighpass:
            return mHighCutoffFreq;
            
            case kBandpass:
            throw FilterError((char*)"Filter error: do not filter mode Bandpass. do not operate.");
            
            default:
            throw FilterError((char*)"Filter error: cause unknown");
        }
    }catch(FilterError err){
        cerr << err.what() << endl;
    }
    
    return 0;
}


/**********************************************************************
* setCutoffFreq : 高い方と低い方のカットオフ周波数を設定
*
*引数
*   inLowCutFreq : 低い方のカットオフ周波数
*   inHighCutFreq : 高い方のカットオフ周波数
*
*ローカル変数
*   err : 
*
*返り値
*   なし
*
*備考
*   このメンバ関数はモードがBandpassフィルタのときに使用
*   Lowpass,Highpassの時には使えない
*
**********************************************************************/
void Filter::setCutoffFreq(double inLowCutFreq,double inHighCutFreq)
{
    try{
        switch (mKindFilter){
          case kLowpass:
          case kHighpass:
            throw FilterError((char*)"Filter error: filter mode Bandpass. do not operate.");
            
          case kBandpass:
            mLowCutoffFreq = inLowCutFreq;
            mHighCutoffFreq = inHighCutFreq;
            break;
            
          default:
            throw FilterError((char*)"Filter error: cause unknown");
        }
    }catch(FilterError err){
        cerr << err.what() << endl;
    }
}


/**********************************************************************
* getLowCutoffFreq : 低い方のカットオフ周波数を取得
*
*引数
*   なし
*
*ローカル変数
*   err : 
*
*返り値
*   低い方のカットオフ周波数
*
*備考
*   このメンバ関数はモードがBandpassフィルタのときに使用
*   Lowpass,Highpassの時には使えない
*
**********************************************************************/
double Filter::getLowCutoffFreq() const
{
    try{
        switch (mKindFilter){
          case kLowpass:
          case kHighpass:
            throw FilterError((char*)"Filter error: filter mode Bandpass. do not operate.");
            
          case kBandpass:
            return mLowCutoffFreq;

          default:
            throw FilterError((char*)"Filter error: cause unknown");

        }
    }catch (FilterError err){
        cerr << err.what() << endl;
    }
    
    return 0;
}


/**********************************************************************
* getHighCutoffFreq : 高い方のカットオフ周波数を取得
*
*引数
*   なし
*
*ローカル変数
*   err : 
*
*返り値
*   高い方のカットオフ周波数
*
*備考
*   このメンバ関数はモードがBandpassフィルタのときに使用
*   Lowpass,Highpassの時には使えない
*
**********************************************************************/
double Filter::getHighCutoffFreq() const
{
    try{
        switch (mKindFilter){
          case kLowpass:
          case kHighpass:
            throw FilterError((char*)"Filter error: filter mode Bandpass. do not operate."); 
            
          case kBandpass:
            return mHighCutoffFreq;
            
          default:
            throw FilterError((char*)"Filter error: cause unknown");
        }
    }catch (FilterError err){
        cerr << err.what() << endl;
    }
    
    return 0;
}


/**********************************************************************
* passFilter : フィルタを実行する
*
* 引数
*   sample: 処理するサンプル値
*
* 返り値
*   フィルタを通したサンプル値
*
***********************************************************************/
double Filter::passFilter(double sample)
{
	return this->transferFunction(sample);
}


/************************************************************
* transferFunction : 伝達関数
*
* 引数
*   valSample : 伝達関数に渡すサンプル値
*
* ローカル変数
*   data : 伝達関数のそれぞれのセクションを通ってく値
*
* 返り値
*   入力(valSample)が伝達関数を通りぬけた出力(data)
*
* 備考
*   H(z)を伝達関数とする
*   ¥[ H(z) = H_{0}(z) ¥prod^{m}_{i=1} H_{i}(z) ¥]
*
************************************************************/
double Filter::transferFunction(double valSample)
{
    double data;
    long orderNumber,numSection;
    bool isOrderNumberEven;
    
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;
    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else
        numSection = (orderNumber-1) / 2;
    
    numSection++;
    if(getFilterMode() == kBandpass)
        numSection *= 2;
    
    data = valSample;
    for(long i=0;i<numSection;i++)
        data = mSection[i].inject(data);

    return data;
}

