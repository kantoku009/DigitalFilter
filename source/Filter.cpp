#include "Filter.h"

#include <iostream>
using namespace std;

Filter::Filter()
{
    mSection = 0;
    mInfo = 0;
    
    mKindFilter = kLowpass;
    mOrderNumber = 0;
    mLowCutoffFreq = 0;
    mHighCutoffFreq = 0;
}


Filter::~Filter()
{
    
}


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
double Filter::getCutoffFreq()
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
double Filter::getLowCutoffFreq()
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
double Filter::getHighCutoffFreq()
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
* runFilter : フィルタを実行する関数
*
*引数
*   begin : 処理を始めるサンプル
*   end : 処理を終わるサンプル
*   chan : 処理を実行するチャンネル
*
*ローカル変数
*   currSample : ループ内で、現在処理しているサンプル
*   currSampleValue : ループ内で、現在処理しているサンプル値
*   data : currSampleを処理した後のサンプル値
*
*返り値
*   処理が成功したならばtrue，失敗したならばfalse
*
***********************************************************************/
bool Filter::runFilter(long begin,long end,short chan)
{
    long currSample;
    double currSampleValue,data;
    
    for(currSample=begin;currSample<=end;currSample++){
        currSampleValue = mInfo->readSampleFromMemory(currSample,chan);
        
        data = transferFunction(currSampleValue);
        
        //計算した値を保存
        mInfo->writeSampleIntoMemory(data,currSample,chan);
    }

    return true;
}


/************************************************************
*transferFunction : 伝達関数
*
*引数
*   valSample : 伝達関数に渡すサンプル値
*
*ローカル変数
*   data : 伝達関数のそれぞれのセクションを通ってく値
*
*返り値
*   入力(valSample)が伝達関数を通りぬけた出力(data)
*
*備考
*   H(z)を伝達関数とする
*   \[ H(z) = H_{0}(z) \prod^{m}_{i=1} H_{i}(z) \]
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

