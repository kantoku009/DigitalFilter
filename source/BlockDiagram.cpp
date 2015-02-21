#include "BlockDiagram.h"

#include <new>
#include <iostream>
using namespace std;


/*************************************
* BlockDockDiagram : コンストラクタ
*
*引数
*   なし
*
*ローカル変数
*   なし
*
*返り値
*   なし
*
*処理の流れ
*   メンバ変数を初期化
*
**************************************/
BlockDiagram::BlockDiagram()
{
    setOrder(0);
    mCoefficientA = NULL;
    mCoefficientB = NULL;
}


BlockDiagram::BlockDiagram(const BlockDiagram &copy)
{
    init(copy.getOrder(),copy.getCoefficientA(),copy.getCoefficientB());
}

/*************************************
* ~BlockDockDiagram : デストラクタ
*
*引数
*   なし
*
*ローカル変数
*   なし
*
*返り値
*   なし
*
*処理の流れ
*   何もしない
*
**************************************/
BlockDiagram::~BlockDiagram()
{
    delete [] mCoefficientA;
    delete [] mCoefficientB;
}


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
    setOrder(order);
    
    try{
        mCoefficientA = new double [order+1];
        mCoefficientB = new double [order+1];
    }catch(bad_alloc err){
        cerr << err.what() << endl;
    }
    
    for(int i=0;i<=mOrder;i++){
        mCoefficientA[i] = a[i];
        mCoefficientB[i] = b[i];
    }
    
    initPreviousSample(mOrder);
}

/*************************************
* inject : データをダイアグラムに注入
*
*引数
*   sample : サンプル値
*
*ローカル変数
*   data1 : データの中間出力(FIR部)
*   data2 : データの最終出力(IIR部)
*   i : ループカウンタ
*
*返り値
*   元のデータがダイアグラムを通り抜けた後のデータが出力される
*
**************************************/
double BlockDiagram::inject(double sample)
{
    double data1,data2;
    int i;
    
    data1 = mCoefficientB[0] * sample;
    for(i=1;i<=mOrder;i++)
        data1 += mCoefficientB[i] * getPreviousSample(i);

    data2 = mCoefficientA[0] * data1;
    for(i=1;i<=mOrder;i++)
        data2 += mCoefficientA[i] * getPreviousSample(i);
    
    //サンプル値の保存
    popPreviousSample();
    pushPreviousSample(data1);

    return data2;
}


const BlockDiagram &BlockDiagram::operator=(const BlockDiagram &right)
{
    if(this == &right)
        return *this;
    
    delete [] mCoefficientA;
    delete [] mCoefficientB;
    init(right.getOrder(),right.getCoefficientA(),right.getCoefficientB());
    
    return *this;
}


//キューを初期化
void BlockDiagram::initPreviousSample(int n)
{
    int i;
    
    for(i=0;i<=n;i++)
        pushPreviousSample(0.0);
}


//キューに値を追加
void BlockDiagram::pushPreviousSample(double val)
{
    mPreviousSample.push_front(val);
}


//キューから値を押し出す
void BlockDiagram::popPreviousSample()
{
    mPreviousSample.pop_back();
}


//キューから値を取り出す
double BlockDiagram::getPreviousSample(int n)
{
    return mPreviousSample[n-1];
}

