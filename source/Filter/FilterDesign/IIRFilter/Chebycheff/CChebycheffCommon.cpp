#include "CChebycheffCommon.h"

#include <iostream>
#include <fstream>
#include <new>
#include <complex>
using namespace std;

#define PI 3.1415926535897932384626433832795

/***********************************************************
* decisionPrototype : プロトタイプローパスフィルタを決定
*
*引数
*   inPassFrquency : パスバンドの周波数[Hz]
*   inRippleGain : 偏差の量[dB]
*   inStopFrequency : ストップバンドの周波数[Hz]
*   inAttenuateGain : 減衰量[dB]
*
*ローカル変数
*   omega_dp : デジタル周波数のパスバンド[rad/sec]
*   omega_ds : デジタル周波数のストップバンド[rad/sec]
*   omega_ap : アナログ周波数のパスバンド[rad/sec]
*   omega_as : アナログ周波数のストップバンド[rad/sec]
*   n : プロトタイプローパスフィルタの次数
*   fs : サンプリング周波数
*
*返り値
*   なし
*
*処理の流れ
*
*備考
*   次数を決定する式
¥end{verbatim}

       ¥[ n = ¥frac{ ¥cosh^{-1}(¥pm¥frac{1}{¥varepsilon}¥sqrt{10^{¥frac{g}{10}} -1})}{¥cosh^{-1}(¥frac{¥omega _{as}}{¥omega _{ap}})} ¥]

¥begin{verbatim}
*   プロトタイプのカットオフ周波数を決定する式
¥end{verbatim}

       ¥[ ¥omega_{ac} = ¥omega_{ap} ¥cosh[¥frac{1}{n} ¥cosh^{-1}(¥pm ¥frac{1}{¥varepsilon} ¥sqrt{10^{¥frac{3}{10}} - 1})] ¥]

¥begin{verbatim}
************************************************************/
void CChebycheffCommon::decisionPrototype( double inPassFreq,
                                    double inRippleGain,
                                    double inStopFreq,
                                    double inAttenuateGain)
{
    double passFreq,stopFreq;
    double omega_dp,omega_ds;
    double omega_ap,omega_as;
    double n;
    double fs;
    
    fs = getSampleRate();

    //サンプリング周波数で規格化
    passFreq = inPassFreq / fs;
    stopFreq = inStopFreq / fs;
    
    //周波数から角周波数へ変換
    omega_dp = 2*PI * passFreq;
    omega_ds = 2*PI * stopFreq;
    
    //デジタルの周波数からアナログの周波数へ変換
    omega_ap = digital2analog(omega_dp);
    omega_as = digital2analog(omega_ds);

    //ゲインの絶対値化
    if(inRippleGain < 0.0) inRippleGain = -inRippleGain;
    if(inAttenuateGain < 0.0) inAttenuateGain = -inAttenuateGain;

    //epsilonの決定
    this->m_dEpsilon = sqrt(pow(10,inRippleGain/10) - 1);
    
    //次数の決定
    n = acosh((1/this->m_dEpsilon)*sqrt(pow(10,inAttenuateGain/10)-1)) 
            / acosh(omega_as/omega_ap);
    setOrderNumber(static_cast<long>(n) + 1);

    //alpha0の決定
    this->m_dAlpha0 = sinh( 1/(static_cast<double>(getOrderNumber())) * asinh(1/this->m_dEpsilon) );

    //プロトタイプのカットオフ周波数の決定
    this->m_dPrototypeCutFreq = omega_ap
            *cosh( (1/(static_cast<double>(getOrderNumber()))) 
            * acosh((1/this->m_dEpsilon)*sqrt(pow(10,0.3)-1)) );

    this->m_dPrototypePassFreq = omega_ap;

}

/******************************************************************************
* initLowTransferFunction : 伝達関数の初期化と係数の決定
*
*引数
*   なし
*
*ローカル変数
*   alpha : 係数
*   beta : 係数
*   a[3] : １次または２次の伝達関数の分子の係数
*   b[3] : １次または２次の伝達関数の分母の係数
*   orderNumber : 伝達関数の次数
*   m : 伝達関数を１次または２次に分けたときのセクションの個数
*   omega_ap : プロトタイプローパスフィルタのカットオフ周波数
*
*返り値
*   なし
*
*処理の流れ
*   伝達関数を１次または２次の伝達関数に分けた個数分のブロックダイアグラムを確保
*   ブロックダイアグラムの係数を決定
*
*備考
*   伝達関数の次数が偶数ならば１次のブロックダイアグラムは使わない
*   ¥omega_{ap}はプロトタイプローパスフィルタのパスバンド
*   ここでは、周波数は全てサンプリング周波数で規格化されているので、周期Tは1
*   数式はTEXで書いてある
*
*****************************************************************************/
CBlockDiagram* CChebycheffCommon::initLowTransferFunction(double inCutFreq)
{
    double alpha,beta,gamma;
    double a[3],b[3];
    long   orderNumber,numSection;
    bool isOrderNumberEven;
    double omega_ap=getPrototypePassFreq();
    CBlockDiagram *lowpassSection;
    
    //次数を取得
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;

    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else{
        numSection = (orderNumber-1) / 2;
    }
    numSection++;
    
    try{
        lowpassSection = new CBlockDiagram [numSection];
    }catch(bad_alloc err){
        cerr << err.what() << endl;
        return 0;
    }

    //係数を決定
    gamma = getLowGamma(inCutFreq);
    //1次の伝達関数の初期化
    if(isOrderNumberEven){
        a[0] = 1;
        a[1] = 0;
        a[2] = 0;
        b[0] = 1;
        b[1] = 0;
        b[2] = 0;
    }else{
        a[0] = 1;
        a[1] = 1;
        a[2] = 0;
        b[0] = this->m_dAlpha0*omega_ap*(1-gamma) 
                / ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
        b[1] = -( (this->m_dAlpha0*omega_ap-2) - (this->m_dAlpha0*omega_ap+2)*gamma )
                 / ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
        b[2] = 0;
    }
    lowpassSection[0].init(2,a,b);
    
    //2次の伝達関数の初期化
    for(long i=1;i<numSection;i++){
        alpha = getAlpha(i);
        beta = getBeta(i);
        a[0] = 1;
        a[1] = 2;
        a[2] = 1;
        b[0] = beta*omega_ap*omega_ap*(1-gamma)*(1-gamma) 
                / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                    - 2*(beta*omega_ap*omega_ap-4)*gamma 
                    + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        b[1] = -2*( (beta*omega_ap*omega_ap-4) 
                - 2*(beta*omega_ap*omega_ap+4)*gamma 
                + (beta*omega_ap*omega_ap-4)*gamma*gamma ) 
            / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                - 2*(beta*omega_ap*omega_ap-4)*gamma 
                + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        b[2] = -( (4-2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                - 2*(beta*omega_ap*omega_ap-4)*gamma 
                + (4+2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma ) 
            / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                - 2*(beta*omega_ap*omega_ap-4)*gamma 
                + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        lowpassSection[i].init(2,a,b);
    }
    
    return lowpassSection;
}

/******************************************************************************
* initHighTransferFunction : 伝達関数の初期化と係数の決定
*
*引数
*   なし
*
*ローカル変数
*   alpha : 係数
*   beta : 係数
*   a[3] : １次または２次の伝達関数の分子の係数
*   b[3] : １次または２次の伝達関数の分母の係数
*   orderNumber : 伝達関数の次数
*   m : 伝達関数を１次または２次に分けたときのセクションの個数
*   isOrderNumber : 伝達関数の次数が偶数か奇数かを記憶する。偶数ならばtrue
*   omega_ac : プロトタイプローパスフィルタのカットオフ周波数
*
*返り値
*   なし
*
*処理の流れ
*   伝達関数を１次または２次の伝達関数に分けた個数分のブロックダイアグラムを確保
*   ブロックダイアグラムの係数を決定
*
*備考
*   伝達関数の次数が偶数ならば１次のブロックダイアグラムは使わない
*   ¥omega_{ac}はプロトタイプローパスフィルタのカットオフ周波数
*   ここでは、周波数は全てサンプリング周波数で規格化されているので、周期Tは1
*   数式はTEXで書いてある
*
*   伝達関数
*   ¥[ HHP(z) = HHP_{0}(z) ¥prod^{m}_{i=1} HHP_{i} ¥]
*
*****************************************************************************/
CBlockDiagram* CChebycheffCommon::initHighTransferFunction(double inCutFreq)
{
    double alpha,beta,gamma;
    double a[3],b[3];
    long   orderNumber,numSection;
    bool   isOrderNumberEven;
    double omega_ap=getPrototypePassFreq();
    CBlockDiagram *highpassSection;
    
    //次数を取得
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;
    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else{
        numSection = (orderNumber-1) / 2;
    }
    numSection++;
    
    try{
        highpassSection = new CBlockDiagram [numSection];
    }catch(bad_alloc err){
        cerr << err.what() << endl;
        return 0;
    }

    //係数を決定
    gamma = getHighGamma(inCutFreq);
    
    //1次の伝達関数の初期化
    if(isOrderNumberEven){
        a[0] = 1;
        a[1] = 0;
        a[2] = 0;
        b[0] = 1;
        b[1] = 0;
        b[2] = 0;
    }else{
        a[0] = 1;
        a[1] = -1;
        a[2] = 0;
        b[0] = this->m_dAlpha0*omega_ap*(1-gamma) 
                / ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
        b[1] = ( (this->m_dAlpha0*omega_ap-2) - (this->m_dAlpha0*omega_ap+2)*gamma ) 
                / ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
        b[2] = 0;
    }
    highpassSection[0].init(2,a,b);
    
    for(long i=1;i<numSection;i++){
        //２次の伝達関数の初期化
        alpha = getAlpha(i);
        beta = getBeta(i);
        a[0] = 1;
        a[1] = -2;
        a[2] = 1;
        b[0] = beta*omega_ap*omega_ap*(1-gamma)*(1-gamma) 
                / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap)
                     - 2*(beta*omega_ap*omega_ap-4)*gamma 
                     + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        b[1] = 2*( (beta*omega_ap*omega_ap-4) 
                - 2*(beta*omega_ap*omega_ap+4)*gamma 
                + (beta*omega_ap*omega_ap-4)*gamma*gamma ) 
            / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                - 2*(beta*omega_ap*omega_ap-4)*gamma 
                + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        b[2] = -( (4-2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                    - 2*(beta*omega_ap*omega_ap-4)*gamma 
                    + (4+2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma ) 
                / ( (4+2*alpha*omega_ap+beta*omega_ap*omega_ap) 
                    - 2*(beta*omega_ap*omega_ap-4)*gamma 
                    + (4-2*alpha*omega_ap+beta*omega_ap*omega_ap)*gamma*gamma );
        highpassSection[i].init(2,a,b);
    }
    
    return highpassSection;
}




CBlockDiagram* CChebycheffCommon::initBandTransferFunction(double inLowCutFreq, double inHighCutFreq)
{
    CBlockDiagram *lowpassSection,*highpassSection,*bandpassSection;
    long orderNumber,numSection;
    bool isOrderNumberEven;
    
    //lowpassSection = 0;
    //highpassSection = 0;
    //bandpassSection = 0;
    
    lowpassSection = initLowTransferFunction(inHighCutFreq);
    
    highpassSection = initHighTransferFunction(inLowCutFreq);
    
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;
    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else
        numSection = (orderNumber-1) / 2;
    
    numSection++;
    
    bandpassSection = new CBlockDiagram [numSection*2];
    for(long i=0;i<numSection;i++){
        bandpassSection[i] = lowpassSection[i];
        bandpassSection[i+numSection] = highpassSection[i];
    }
    
    delete [] lowpassSection;
    delete [] highpassSection;
    
    return bandpassSection;
}


/**********************************************************************
*getAlpha : 係数αの取得
*
*引数
*   i : 伝達関数の$¥prod_i$
*
*ローカル変数
*   n : 伝達関数の次数
*   v : 係数
*
*返り値
*   係数αを返す
*
*備考
¥end{verbatim}

   ¥[ ¥alpha_{i} = 2¥cos(¥frac{¥pi}{n}v) ¥]

¥begin{verbatim}
*   v = i - (1/2) (nが偶数のとき)
*   v = i         (nが奇数のとき)
*
***********************************************************************/
double CChebycheffCommon::getAlpha(long i)
{
    long n;
    double v;
    
    n = getOrderNumber();
    if(n%2 == 0)
        v = i - 0.5;
    else
        v = i;
    
    return 2*this->m_dAlpha0*cos(PI*v/n);
}


/**********************************************************************
* getBeta : 係数βの取得
*
*引数
*   i : 伝達関数の$¥prod_i$
*
*ローカル変数
*   n : 伝達関数の次数
*   v : 係数
*
*返り値
*   係数βを返す
*
*備考
¥end{verbatim}

   ¥[ ¥beta_{i} = ¥alpha_{0}^{2} + sin(¥frac{¥pi}{n}v)^2 ¥]

¥begin{verbatim}
*   v = i - (1/2) (nが偶数のとき)
*   v = i         (nが奇数のとき)
*
***********************************************************************/
double CChebycheffCommon::getBeta(long i)
{
    long n;
    double v;
    
    n = getOrderNumber();
    if(n%2 == 0)
        v = i - 0.5;
    else
        v = i;
    
    return this->m_dAlpha0*this->m_dAlpha0 + sin(PI*v/n)*sin(PI*v/n);
}



/*********************************************************************
* getLowGamma  :  伝達関数の係数を取得
*
*引数
*   inCutFreq : カットオフ周波数
*
*ローカル変数
*   fs :  サンプリング周波数
*   omega_dc : デジタルのカットオフ周波数[rad/sec]
*   omega_ac :  アナログのカットオフ周波数[rad/sec]
*   theta : プロトタイプローパスフィルタのカッとオフ周波数[rad/sec]
*
*返り値
*   伝達関数の係数を返す
*
*
*処理の流れ
*   カットオフ周波数をサンプリング周波数で規格化し、デジタルの角周波数にする
*   デジタルの角周波数をアナログの角周波数にする（プリウォーピング）
*   プロトタイプローパスフィルタのカットオフ周波数を取得
*   係数の計算
*
*備考
*   係数γ
¥end{verbatim}

   ¥[ ¥gamma = ¥frac{¥sin(¥frac{¥omega_{ac}-¥omega_{c}}{2})T}{¥sin(¥frac{¥omega_{ac}+¥omega_{c}}{2})T} ¥]

¥begin{verbatim}
*
**********************************************************************/
double CChebycheffCommon::getLowGamma(double inCutFreq)
{
    double fs;
    double omega_dc,omega_ac;
    double theta;
    
    fs = getSampleRate();
    omega_dc = 2*PI * inCutFreq / fs;
    omega_ac = digital2analog(omega_dc);
    
    theta = getPrototypeCutFreq();

    return sin(theta/2-omega_ac/2) / sin(theta/2+omega_ac/2);
}


/*********************************************************************
* getHighGamma  :  伝達関数の係数を取得
*
*引数
*   inCutFreq : カットオフ周波数
*
*ローカル変数
*   fs :  サンプリング周波数
*   omega_dc : デジタルのカットオフ周波数[rad/sec]
*   omega_ac :  アナログのカットオフ周波数[rad/sec]
*   theta : プロトタイプローパスフィルタのカッとオフ周波数[rad/sec]
*
*返り値
*   伝達関数の係数を返す
*
*
*処理の流れ
*   カットオフ周波数をサンプリング周波数で規格化し、デジタルの角周波数にする
*   デジタルの角周波数をアナログの角周波数にする（プリウォーピング）
*   プロトタイプローパスフィルタのカットオフ周波数を取得
*   係数の計算
*
*備考
*   係数γ
¥end{verbatim}

   ¥[ ¥gamma = -¥frac{¥cos(¥frac{¥omega_{ac}+¥omega_{c}}{2})T}{¥cos(¥frac{¥omega_{ac}-¥omega_{c}}{2})T} ¥]

¥begin{verbatim}
*
**********************************************************************/
double CChebycheffCommon::getHighGamma(double inCutFreq)
{
    double fs;
    double omega_dc,omega_ac;
    double theta;
    
    fs = getSampleRate();
    omega_dc = 2*PI * inCutFreq / fs;
    omega_ac = digital2analog(omega_dc);
    
    theta = getPrototypeCutFreq();

    return -cos(theta/2+omega_ac/2) / cos(theta/2-omega_ac/2);
}


/***********************************************************************
*printCharacteristic : 伝達関数の振幅特性と位相特性をファイルに出力
*
*引数
*   fNameAmp : 振幅特性を出力するファイル名
*   fNamePhase : 位相特性を出力するファイル名
*
*ローカル変数
*   fpAmp : 振幅特性を出力するファイルのファイルストリーム
*   fpPhase : 位相特性を出力するファイルのファイルストリーム
*   sectionOrder :  伝達関数のセクションの次数
*   n : 伝達関数の次数
*   m :  伝達関数を１次または２次に分けたときのセクションの個数
*   isOrderEven :  伝達関数が偶数かどうか。偶数ならばtrue
*   a : 伝達関数の1次または２次のセクションの係数
*   b : 伝達関数の1次または２次のセクションの係数
*   omega : 振幅特性、位相特性の角周波数[rad/sec]
*   e1 : 絶対値が1で，位相角度-omegaの複素数
*   e2 : 絶対値が1で，位相角度-2*omegaの複素数
*   h : インパルス応答
*   amp : 振幅の値
*   phase : 位相の値
*   freq : 周波数[Hz]
*
*返り値
*   なし
*
***********************************************************************/
void CChebycheffCommon::printCharacteristic(char *fNameAmp,char *fNamePhase) const
{
	//とりあえずコメントアウト.
	/*
    ofstream fpAmp(fNameAmp,ios::out),fpPhase(fNamePhase,ios::out);
    long orderNumber,numSection;
    bool isOrderEven;
    const double *a,*b;
    double omega;
    complex<double> h;
    
    orderNumber = getOrderNumber();
    isOrderEven = (orderNumber%2 == 0)? true:false;
    if(isOrderEven){
        numSection = orderNumber / 2;
    }else{
        numSection = (orderNumber-1) / 2;
    }
    numSection++;
    
    if(getFilterMode() == kBandpass)
        numSection *= 2;
    
    for(omega=0.001;omega<PI;omega+=0.001){
        complex<double> e1,e2,h;
        double amp,phase,freq;
        freq = omega *getSampleRate() / (2*PI);

        e1 = polar(1.0,-omega);
        e2 = polar(1.0,-2*omega);
        h = polar(1.0,0.0);
        for(long i=0;i<numSection;i++){
            a = this->m_pcBlockDiagram[i].getCoefficientA();
            b = this->m_pcBlockDiagram[i].getCoefficientB();
            h *= b[0] * (a[0] + a[1]*e1 + a[2]*e2) / (1.0 - b[1]*e1 - b[2]*e2);
        }

        amp = abs(h);
        phase = arg(h);
        
        fpAmp << freq << ',' << 20*log10(amp) << endl;
        fpPhase << freq << ',' << phase << endl;

    }
	*/
}

