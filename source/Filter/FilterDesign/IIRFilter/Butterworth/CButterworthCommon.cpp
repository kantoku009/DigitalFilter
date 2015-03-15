#include "CButterworthCommon.h"

#include <iostream>
#include <cmath>
using namespace std;

/***********************************************************
* decisionPrototype : プロトタイプローパスフィルタを決定
*
* 引数
*   i_dPassFrquency : パスバンドの周波数[Hz]
*   i_dRippleGain : 偏差の量[dB]
*   i_dStopFrequency : ストップバンドの周波数[Hz]
*   i_dAttenuateGain : 減衰量[dB]
*
* ローカル変数
*   omega_dp : デジタル周波数のパスバンド[rad/sec]
*   omega_ds : デジタル周波数のストップバンド[rad/sec]
*   omega_ap : アナログ周波数のパスバンド[rad/sec]
*   omega_as : アナログ周波数のストップバンド[rad/sec]
*   n : プロトタイプローパスフィルタの次数
*   fs : サンプリング周波数
*
* 返り値
*   なし
*
* 処理の流れ
*
* 備考
*   次数を決定する式
¥end{verbatim}
¥[ n = ¥frac{1}{2} ¥frac{¥log[¥frac{10^{¥frac{g}{10}} - 1}{10^{¥frac{r}{10}} - 1}]}{¥log[¥frac{¥omega_{as}}{¥omega_{ap}}]} ¥]
¥begin{verbatim}
*   プロトタイプのカットオフ周波数を決定する式
¥end{verbatim}
¥[ ¥omega_{ac} = ¥frac{¥omega_{ap}}{(10^{¥frac{r}{10}} - 1)^{¥frac{1}{2n}}} ¥]
¥begin{verbatim}
************************************************************/
void CButterworthCommon::decisionPrototype(
									double i_dPassFreq,
									double i_dRippleGain,
									double i_dStopFreq,
									double i_dAttenuateGain)
{
	double omega_dp,omega_ds;
	double omega_ap,omega_as;
	double n;
	double fs;
    
	fs = this->getSampleRate();

	//サンプリング周波数で規格化
	i_dPassFreq /= fs;
	i_dStopFreq /= fs;
    
	//周波数から角周波数へ変換
	omega_dp = 2*M_PI * i_dPassFreq;
	omega_ds = 2*M_PI * i_dStopFreq;

	//デジタルの周波数からアナログの周波数へ変換
	omega_ap = digital2analog(omega_dp);
	omega_as = digital2analog(omega_ds);
    
	//ゲインの絶対値化
	if(i_dRippleGain < 0.0)		i_dRippleGain = -i_dRippleGain;
	if(i_dAttenuateGain < 0.0)	i_dAttenuateGain = -i_dAttenuateGain;

	//次数の決定
	n = 0.5;
	n *= ( log10(pow(10,i_dAttenuateGain/10)-1) - log10(pow(10,i_dRippleGain/10)-1) );
	n /= ( log10(omega_as) - log10(omega_ap) );
	this->setOrderNumber(static_cast<long>(n) + 1);
    
	//プロトタイプのカットオフ周波数の決定
	this->m_dPrototypeCutFreq = omega_ap / pow((pow(10,i_dRippleGain/10) - 1),
		1/(2*(static_cast<double>(this->getOrderNumber()))));
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
¥end{verbatim}

   ¥[ HLP(z) = HLP_{0}(z) ¥prod^{m}_{i=1} HLP_{i} ¥]
   ¥[ HLP_{i}(z) = b_{i0} ¥frac{a_{i0} + a_{i1}z^{-1} + a_{i2}z^{-2}}{1 - b_{i1}z^{-1} - b{i2}z^{-2}} ¥]

¥begin{verbatim}
*   HLP_{i}(z)の分子の係数
¥end{verbatim}

   ¥[ a_{i0} = 1 , a_{i1} = 2 , a_{i2} = 1 ¥]

¥begin{verbatim}
*   HLP_{i}(z)の分母の係数
¥end{verbatim}

   ¥[ b_{i0} = ¥frac{¥omega^{2}_{ac}T^2 (1-¥beta)^2}{(4 + 2¥alpha_i ¥omega_{ac} T + ¥omega^{2}_{ac} T^2) - 2(¥omega^{2}_{ac} T^2 - 4)¥beta + (4 - 2¥alpha_i ¥omega_{ac} T + ¥omega^{2}_{ac} T^2)¥beta^2} ¥]

   ¥[ b_{i1} = -2¥frac{(¥omega_{ac}T + 2)(¥omega_{ac}T - 2) - 2(¥omega_{ac}^{2}T^{2} + 4)¥beta + (¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta^{2}}{(4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}} ¥]

   ¥[ b_{i2} = -¥frac{(4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}}{(4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}} ¥]

¥begin{verbatim}
*
*   HLP_{0}は伝達関数の次数が偶数ならば1
*   奇数ならば
¥end{verbatim}

   ¥[ HLP_{0}(z) = b_{0}¥frac{a_{0} + a_{1}z^{-1}}{1 - b_{1}z^{-1}} ¥]

¥begin{verbatim}
*   HLP_{0}の分子の係数
¥end{verbatim}

   ¥[ a_{0} = 1 , a_{1} = 1 ¥]

¥begin{verbatim}
*   HLP_{0}の分母の係数
¥end{verbatim}

   ¥[ b_{0} = ¥frac{¥omega_{ac}T(1 - ¥beta)}{(¥omega_{ac}T + 2) - (¥omega_{ac}T - 2)¥beta} ¥]
   ¥[ b_{1} = -¥frac{(¥omega_{ac}T - 2) - (¥omega_{ac}T + 2)¥beta}{(¥omega_{ac}T + 2) - (¥omega_{ac}T - 2)¥beta} ¥]

¥begin{verbatim}
*
*   係数β
¥end{verbatim}

   ¥[ ¥beta = ¥frac{¥sin(¥frac{¥omega_{ac}-¥omega_c}{2})T}{¥sin(¥frac{¥omega_{ac}+¥omega_c}{2})T} ¥]

¥begin{verbatim}
*
*****************************************************************************/
CBlockDiagram* CButterworthCommon::initLowTransferFunction(double i_dCutFreq)
{
	double alpha,beta;
	double a[3],b[3];
	double omega_ac=getPrototypeCutFreq();
	CBlockDiagram *lowpassSection;
    
	//次数を取得
	long a_lOrderNumber = this->getOrderNumber();
	bool a_IsOrderNumberEven = (0==a_lOrderNumber%2)? true:false;
	long a_lNumSection = (a_IsOrderNumberEven)? (a_lOrderNumber/2) : ((a_lOrderNumber-1)/2);
	a_lNumSection++;

	try
	{
		lowpassSection = new CBlockDiagram [a_lNumSection];
	}
	catch(bad_alloc err)
	{
		cerr << err.what() << endl;
		return 0;
	}

	//係数を決定
	beta = this->getLowBeta(i_dCutFreq);
	//1次の伝達関数の初期化
	if(a_IsOrderNumberEven)
	{
		a[0] = 1;
		a[1] = 0;
		a[2] = 0;
		b[0] = 1;
		b[1] = 0;
		b[2] = 0;
	}
	else
	{
		a[0] = 1;
		a[1] = 1;
		a[2] = 0;
		b[0] = omega_ac*(1-beta) / ((omega_ac+2) - (omega_ac-2)*beta);
		b[1] = -((omega_ac-2) - (omega_ac+2)*beta) / 
					((omega_ac+2) - (omega_ac-2)*beta);
		b[2] = 0;
	}
	lowpassSection[0].init(2,a,b);
	
	//2次の伝達関数の初期化
	for(long i=1;i<a_lNumSection;i++)
	{
		alpha = getAlpha(i);
		a[0] = 1;
		a[1] = 2;
		a[2] = 1;
		b[0] = omega_ac*omega_ac*(1-beta)*(1-beta) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
		b[1] = -2*( (omega_ac+2)*(omega_ac-2) - 
					2*(omega_ac*omega_ac+4)*beta 
					+ (omega_ac+2)*(omega_ac-2)*beta*beta ) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac)
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
		b[2] = -( (4-2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4+2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta ) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
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
¥end{verbatim}

   ¥[ HHP(z) = HHP_{0}(z) ¥prod^{m}_{i=1} HHP_{i} ¥]

   ¥[ HHP_{i}(z) = b_{i0} ¥frac{a_{i0} + a_{i1}z^{-1} + a_{i2}z^{-2}}{1 - b_{i1}z^{-1} - b{i2}z^{-2}} ¥]

¥begin{verbatim}
*
*   HHP_{i}(z)の分子の係数
¥end{verbatim}

   ¥[ a_{i0} = 1 , a_{i1} = -2 , a_{i2} = 1 ¥]

¥begin{verbatim}
*   HHP_{i}(z)の分母の係数
¥end{verbatim}

   ¥[ b_{i0} = ¥frac{(1 - ¥beta)^{2} ¥omega_{ac}^{2}T^{2}}{(4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}} ¥]
   
   ¥[ b_{i1} = 2¥frac{(¥omega_{ac}T + 2)(¥omega_{ac}T - 2) - 2(¥omega_{ac}^{2}T^{2} + 4)¥beta + (¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta^{2}}{(4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}} ¥]

   ¥[ b_{i2} = -¥frac{(4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}}{(4 + 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2}) -2(¥omega_{ac}T + 2)(¥omega_{ac}T - 2)¥beta + (4 - 2¥alpha_{i}¥omega_{ac}T + ¥omega_{ac}^{2}T^{2})¥beta^{2}} ¥]

¥begin{verbatim}
*   HHP_{0}は伝達関数の次数が偶数ならば1
*   奇数ならば
¥end{verbatim}

   ¥[ HHP_{0}(z) = b_{0}¥frac{a_{0} + a_{1}z^{-1}}{1 - b_{1}z^{-1}} ¥]

¥begin{verbatim}
*   HHP_{0}の分子の係数
¥end{verbatim}

   ¥[ a_{0} = 1 , a_{1} = -1 ¥]

¥begin{verbatim}
*   HHP_{0}の分母の係数
¥end{verbatim}

   ¥[ b_{0} = ¥frac{¥omega_{ac}T(1 - ¥beta)}{(¥omega_{ac}T + 2) - (¥omega_{ac}T - 2)¥beta} ¥]
   ¥[ b_{1} = ¥frac{(¥omega_{ac}T - 2) - (¥omega_{ac}T + 2)¥beta}{(¥omega_{ac}T + 2) - (¥omega_{ac}T - 2)¥beta} ¥]

¥begin{verbatim}
*
*   係数β
¥end{verbatim}

   ¥[ ¥beta = -¥frac{¥cos(¥frac{¥omega_{ac}+¥omega_c}{2})T}{¥cos(¥frac{¥omega_{ac}-¥omega_c}{2})T} ¥]

¥begin{verbatim}
*
*****************************************************************************/
CBlockDiagram* CButterworthCommon::initHighTransferFunction(double i_dCutFreq)
{
	double alpha,beta;
	double a[3],b[3];
	double omega_ac=getPrototypeCutFreq();
	CBlockDiagram *highpassSection;

	//次数を取得
	long a_lOrderNumber = this->getOrderNumber();
	bool a_IsOrderNumberEven = (0==a_lOrderNumber%2)? true:false;
	long a_lNumSection = (a_IsOrderNumberEven)? (a_lOrderNumber/2) : ((a_lOrderNumber-1)/2);
	a_lNumSection++;
    
	try
	{
		highpassSection = new CBlockDiagram [a_lNumSection];
	}
	catch(bad_alloc err)
	{
		cerr << err.what() << endl;
		return 0;
	}

	//係数を決定
	beta = getHighBeta(i_dCutFreq);

	//1次の伝達関数の初期化
	if(a_IsOrderNumberEven)
	{
		a[0] = 1;
		a[1] = 0;
		a[2] = 0;
		b[0] = 1;
		b[1] = 0;
		b[2] = 0;
	}
	else
	{
		a[0] = 1;
		a[1] = -1;
		a[2] = 0;
		b[0] = omega_ac*(1 - beta) / ( (omega_ac+2) - (omega_ac-2)*beta);
		b[1] = ( (omega_ac-2) - (omega_ac+2)*beta ) /
						( (omega_ac+2) - (omega_ac-2)*beta);
		b[2] = 0;
	}
	highpassSection[0].init(2,a,b);

	for(long i=1;i<a_lNumSection;i++)
	{
		//２次の伝達関数の初期化
		alpha = getAlpha(i);
		a[0] = 1;
		a[1] = -2;
		a[2] = 1;
		b[0] = omega_ac*omega_ac*(1-beta)*(1-beta) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
		b[1] = 2*( (omega_ac+2)*(omega_ac-2) 
					- 2*(omega_ac*omega_ac+4)*beta
					+ (omega_ac+2)*(omega_ac-2)*beta*beta ) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
		b[2] = -( (4-2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4+2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta ) / 
					( (4+2*alpha*omega_ac+omega_ac*omega_ac) 
					- 2*(omega_ac+2)*(omega_ac-2)*beta 
					+ (4-2*alpha*omega_ac+omega_ac*omega_ac)*beta*beta );
		highpassSection[i].init(2,a,b);
	}

	return highpassSection;
}

/*********************************************************
 * バンドパスフィルタの伝達関数の初期化と係数の決定.
 *********************************************************/
CBlockDiagram* CButterworthCommon::initBandTransferFunction (
									double i_dLowCutFreq,
									double i_dHighCutFreq)
{
	//フィルタの次数を取得.
	long a_lOrderNumber = this->getOrderNumber();
	bool a_IsOrderNumberEven = (0==a_lOrderNumber%2)? true:false;
	long a_lNumSection = (a_IsOrderNumberEven)? (a_lOrderNumber/2) : ((a_lOrderNumber-1)/2);
	a_lNumSection++;

	//ローパスフィルタの伝達関数.
	CBlockDiagram* lowpassSection = initLowTransferFunction(i_dLowCutFreq);
	//ハイパスフィルタの伝達関数.
	CBlockDiagram* highpassSection = initHighTransferFunction(i_dHighCutFreq);
	//バンドパスフィルタの伝達関数.（ローパスとハイパスを組み合わせる）.
	CBlockDiagram* bandpassSection = new CBlockDiagram [a_lNumSection*2];
	for(long a_lIndex=0;a_lIndex<a_lNumSection;a_lIndex++)
	{
		bandpassSection[a_lIndex] = lowpassSection[a_lIndex];
		bandpassSection[a_lIndex+a_lNumSection] = highpassSection[a_lIndex];
	}

	delete [] lowpassSection;
	delete [] highpassSection;

	return bandpassSection;
}


/**********************************************************************
* getAlpha : 係数αの取得
*
* 引数
*	i : 伝達関数の$¥prod_i$
*
* ローカル変数
*	n : 伝達関数の次数
*	v : 係数
*
* 返り値
* 係数αを返す
*
* 備考
¥end{verbatim}
¥[ ¥alpha_{i} = 2¥cos(¥frac{¥pi}{n}v) ¥]
¥begin{verbatim}
*	v = i - (1/2)	(nが偶数のとき)
*	v = i			(nが奇数のとき)
*
***********************************************************************/
double CButterworthCommon::getAlpha(long i_lProd)
{
	double a_dCoeff;

	long a_lOrderNumber = this->getOrderNumber();
	if(0 == a_lOrderNumber%2)
	{
		a_dCoeff = i_lProd - 0.5;
	}
	else
	{
		a_dCoeff = i_lProd;
	}

	return 2*cos(M_PI*a_dCoeff/a_lOrderNumber);
}


/*********************************************************************
* getBeta  :  伝達関数の係数を取得
*
* 引数
*   i_dCutFreq : カットオフ周波数
*
* ローカル変数
*   fs :  サンプリング周波数
*   omega_dc : デジタルのカットオフ周波数[rad/sec]
*   omega_ac :  アナログのカットオフ周波数[rad/sec]
*   theta : プロトタイプローパスフィルタのカッとオフ周波数[rad/sec]
*
* 返り値
*   伝達関数の係数を返す
*
*
* 処理の流れ
*   カットオフ周波数をサンプリング周波数で規格化し、デジタルの角周波数にする
*   デジタルの角周波数をアナログの角周波数にする（プリウォーピング）
*   プロトタイプローパスフィルタのカットオフ周波数を取得
*   係数の計算
*
* 備考
*   係数β
¥end{verbatim}

   ¥[ ¥beta = ¥frac{¥sin(¥frac{¥omega_{ac}-¥omega_c}{2})T}{¥sin(¥frac{¥omega_{ac}+¥omega_c}{2})T} ¥]

¥begin{verbatim}
*
**********************************************************************/
double CButterworthCommon::getLowBeta(double i_dCutFreq)
{
	double fs;
	double omega_dc,omega_ac;
	double theta;
    
	fs = this->getSampleRate();
	omega_dc = 2*M_PI * i_dCutFreq / fs;
	omega_ac = digital2analog(omega_dc);
    
	theta = getPrototypeCutFreq();

	return sin(theta/2-omega_ac/2) / sin(theta/2+omega_ac/2);
}


/*********************************************************************
* getHighBeta  :  伝達関数の係数を取得
*
* 引数
*   i_dCutFreq : カットオフ周波数
*
* ローカル変数
*   fs :  サンプリング周波数
*   omega_dc : デジタルのカットオフ周波数[rad/sec]
*   omega_ac :  アナログのカットオフ周波数[rad/sec]
*   theta : プロトタイプローパスフィルタのカッとオフ周波数[rad/sec]
*
* 返り値
*   伝達関数の係数を返す
*
*
* 処理の流れ
*   カットオフ周波数をサンプリング周波数で規格化し、デジタルの角周波数にする
*   デジタルの角周波数をアナログの角周波数にする（プリウォーピング）
*   プロトタイプローパスフィルタのカットオフ周波数を取得
*   係数の計算
*
* 備考
*   係数β
¥end{verbatim}


   ¥[ ¥beta = -¥frac{¥cos(¥frac{¥omega_{ac}+¥omega_c}{2})T}{¥cos(¥frac{¥omega_{ac}-¥omega_c}{2})T} ¥]

¥begin{verbatim}
*
**********************************************************************/
double CButterworthCommon::getHighBeta(double i_dCutFreq)
{
	double fs;
	double omega_dc,omega_ac;
	double theta;
    
	fs = getSampleRate();
	omega_dc = 2*M_PI * i_dCutFreq / fs;
	omega_ac = digital2analog(omega_dc);
    
	theta = getPrototypeCutFreq();

	return -cos(theta/2+omega_ac/2) / cos(theta/2-omega_ac/2);
}


