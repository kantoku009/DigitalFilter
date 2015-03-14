#include "CChebycheffCommon.h"

#include <iostream>
#include <cmath>
using namespace std;

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
	omega_dp = 2*M_PI * passFreq;
	omega_ds = 2*M_PI * stopFreq;

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
	double omega_ap=getPrototypePassFreq();
	CBlockDiagram *lowpassSection;

	//フィルタの次数を取得.
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
	gamma = getLowGamma(inCutFreq);

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
		b[0] = this->m_dAlpha0*omega_ap*(1-gamma) 
				/ ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
		b[1] = -( (this->m_dAlpha0*omega_ap-2) - (this->m_dAlpha0*omega_ap+2)*gamma )
				/ ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
		b[2] = 0;
	}
	lowpassSection[0].init(2,a,b);

	//2次の伝達関数の初期化
	for(long i=1;i<a_lNumSection;i++)
	{
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
	double omega_ap=getPrototypePassFreq();
	CBlockDiagram *highpassSection;

	//フィルタの次数を取得.
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
	gamma = getHighGamma(inCutFreq);
    
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
		b[0] = this->m_dAlpha0*omega_ap*(1-gamma) 
				/ ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
		b[1] = ( (this->m_dAlpha0*omega_ap-2) - (this->m_dAlpha0*omega_ap+2)*gamma ) 
				/ ( (this->m_dAlpha0*omega_ap+2) - (this->m_dAlpha0*omega_ap-2)*gamma );
		b[2] = 0;
	}
	highpassSection[0].init(2,a,b);

	for(long i=1;i<a_lNumSection;i++){
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

/*********************************************************
 * バンドパスフィルタの伝達関数の初期化と係数の決定.
 *********************************************************/
CBlockDiagram* CChebycheffCommon::initBandTransferFunction(double inLowCutFreq, double inHighCutFreq)
{
	//フィルタの次数を取得.
	long a_lOrderNumber = this->getOrderNumber();
	bool a_IsOrderNumberEven = (0==a_lOrderNumber%2)? true:false;
	long a_lNumSection = (a_IsOrderNumberEven)? (a_lOrderNumber/2) : ((a_lOrderNumber-1)/2);
	a_lNumSection++;

	CBlockDiagram* lowpassSection = initLowTransferFunction(inHighCutFreq);
	CBlockDiagram* highpassSection = initHighTransferFunction(inLowCutFreq);
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
*getAlpha : 係数αの取得
*
*引数
*	i : 伝達関数の$¥prod_i$
*
*ローカル変数
*	n : 伝達関数の次数
*	v : 係数
*
*返り値
* 係数αを返す
*
*備考
¥end{verbatim}

	¥[ ¥alpha_{i} = 2¥cos(¥frac{¥pi}{n}v) ¥]

¥begin{verbatim}
*	v = i - (1/2) (nが偶数のとき)
*	v = i (nが奇数のとき)
*
***********************************************************************/
double CChebycheffCommon::getAlpha(long i_lProd)
{
	long a_lOrderNumber = this->getOrderNumber();
	double a_dCoeff;

	if(0 == a_lOrderNumber%2)
	{
		a_dCoeff = i_lProd - 0.5;
	}
	else
	{
		a_dCoeff = i_lProd;
	}

	return 2*this->m_dAlpha0*cos(M_PI*a_dCoeff/a_lOrderNumber);
}


/**********************************************************************
* getBeta : 係数βの取得
*
*引数
*	i : 伝達関数の$¥prod_i$
*
*ローカル変数
*	n : 伝達関数の次数
*	v : 係数
*
*返り値
* 係数βを返す
*
*備考
¥end{verbatim}

	¥[ ¥beta_{i} = ¥alpha_{0}^{2} + sin(¥frac{¥pi}{n}v)^2 ¥]

¥begin{verbatim}
*	v = i - (1/2) (nが偶数のとき)
*	v = i (nが奇数のとき)
*
***********************************************************************/
double CChebycheffCommon::getBeta(long i_lProd)
{
	long a_lOrderNumber = this->getOrderNumber();
	double a_dCoeff;

	if(0 == a_lOrderNumber%2)
	{
		a_dCoeff = i_lProd - 0.5;
	}
	else
	{
		a_dCoeff = i_lProd;
	}

	return this->m_dAlpha0*this->m_dAlpha0 + sin(M_PI*a_dCoeff/a_lOrderNumber)*sin(M_PI*a_dCoeff/a_lOrderNumber);
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
	omega_dc = 2*M_PI * inCutFreq / fs;
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
	omega_dc = 2*M_PI * inCutFreq / fs;
	omega_ac = digital2analog(omega_dc);

	theta = getPrototypeCutFreq();

	return -cos(theta/2+omega_ac/2) / cos(theta/2-omega_ac/2);
}



