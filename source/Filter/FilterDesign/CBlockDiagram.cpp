/**
 * @file CBlockDiagram.cpp
 */

#include <new>
#include <iostream>
using namespace std;

#include "CBlockDiagram.h"

/*************************************
* init : ダイアグラムの初期化
*
*引数
*   i_iOrder : ダイアグラムの次数
*   i_pdFeedForwardCoefficient : ダイアグラムの係数.(フィードフォーワード係数).
*   i_pdFeedbackCoefficient : ダイアグラムの係数.(フィードバック係数).
*
*ローカル変数
*   なし
*
*返り値
*   なし
*
**************************************/
void CBlockDiagram::init(int i_iOrder,const double* i_pdFeedForwardCoefficient, const double* i_pdFeedbackCoefficient)
{
	//次数の設定
	this->setOrder(i_iOrder);

	try
	{
		this->m_pdFeedforwardCoefficient = new double [i_iOrder+1];
		this->m_pdFeedbackCoefficient = new double [i_iOrder+1];
	}
	catch(bad_alloc err)
	{
		cerr << err.what() << endl;
	}
    
	for(int i=0;i<=this->m_iOrder;i++)
	{
		this->m_pdFeedforwardCoefficient[i] = i_pdFeedForwardCoefficient[i];
		this->m_pdFeedbackCoefficient[i] = i_pdFeedbackCoefficient[i];
	}

	this->initPreviousSample(this->m_iOrder);
}

/*************************************
* inject : データをダイアグラムに注入
*
* 引数
*   i_dSample : サンプル値
*
* ローカル変数
*   a_dFeedbackValue : データの中間出力(FIR部)
*   a_dFeedforwardValue : データの最終出力(IIR部)
*   a_iIndex : ループカウンタ
*
* 返り値
*   元のデータがダイアグラムを通り抜けた後のデータが出力される
*
**************************************/
double CBlockDiagram::inject(double i_dSample)
{
	double a_dFeedbackValue=0.0;
	double a_dFeedforwardValue=0.0;
	int a_iIndex=0;
    
	// フィードバック(FIR部)
	a_dFeedbackValue = this->m_pdFeedbackCoefficient[0] * i_dSample;
	for(a_iIndex=1; a_iIndex<=this->m_iOrder; a_iIndex++)
	{
		a_dFeedbackValue += this->m_pdFeedbackCoefficient[a_iIndex] * this->getPreviousSample(a_iIndex);
	}

	// フィードフォーワード(IIR部)
	a_dFeedforwardValue = this->m_pdFeedforwardCoefficient[0] * a_dFeedbackValue;
	for(a_iIndex=1; a_iIndex<=this->m_iOrder; a_iIndex++)
	{
		a_dFeedforwardValue += this->m_pdFeedforwardCoefficient[a_iIndex] * this->getPreviousSample(a_iIndex);
	}
    
	//サンプル値の保存
	this->popPreviousSample();
	this->pushPreviousSample(a_dFeedbackValue);

	return a_dFeedforwardValue;
}

