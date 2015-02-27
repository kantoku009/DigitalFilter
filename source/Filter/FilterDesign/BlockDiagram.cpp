#include "BlockDiagram.h"

#include <new>
#include <iostream>
using namespace std;



/*************************************
* init : ダイアグラムの初期化
*
*引数
*   order : ダイアグラムの次数
*   a : ダイアグラムの係数
*   b : ダイアグラムの係数
*
*ローカル変数
*   なし
*
*返り値
*   なし
*
**************************************/
void BlockDiagram::init(int order,const double *a,const double *b)
{
    //次数の設定
    this->setOrder(order);
    
    try
	{
        this->m_pdCoefficientA = new double [order+1];
        this->m_pdCoefficientB = new double [order+1];
    }
	catch(bad_alloc err)
	{
        cerr << err.what() << endl;
    }
    
    for(int i=0;i<=this->m_iOrder;i++){
        this->m_pdCoefficientA[i] = a[i];
        this->m_pdCoefficientB[i] = b[i];
    }
    
    this->initPreviousSample(this->m_iOrder);
}

/*************************************
* inject : データをダイアグラムに注入
*
* 引数
*   sample : サンプル値
*
* ローカル変数
*   data1 : データの中間出力(FIR部)
*   data2 : データの最終出力(IIR部)
*   i : ループカウンタ
*
* 返り値
*   元のデータがダイアグラムを通り抜けた後のデータが出力される
*
**************************************/
double BlockDiagram::inject(double sample)
{
    double data1,data2;
    
    data1 = this->m_pdCoefficientB[0] * sample;
    for(int i_iIndex=1; i_iIndex<=this->m_iOrder; i_iIndex++)
        data1 += this->m_pdCoefficientB[i_iIndex] * getPreviousSample(i_iIndex);

    data2 = this->m_pdCoefficientA[0] * data1;
    for(int i_iIndex=1; i_iIndex<=this->m_iOrder; i_iIndex++)
        data2 += this->m_pdCoefficientA[i_iIndex] * getPreviousSample(i_iIndex);
    
    //サンプル値の保存
    popPreviousSample();
    pushPreviousSample(data1);

    return data2;
}


/******************************
 * =演算子のオーバーロード.
 ******************************/
const BlockDiagram &BlockDiagram::operator=(const BlockDiagram& i_cBlockDiagram)
{
    if(this == &i_cBlockDiagram) return *this;
    
    delete [] this->m_pdCoefficientA;
    delete [] this->m_pdCoefficientB;
    this->init(i_cBlockDiagram.getOrder(), i_cBlockDiagram.getCoefficientA(), i_cBlockDiagram.getCoefficientB());
    
    return *this;
}


