#ifndef __BLOCK_DIAGRAM_H__
#define __BLOCK_DIAGRAM_H__

#include <deque>
using namespace std;

class BlockDiagram
{
public:
    BlockDiagram();
    BlockDiagram(const BlockDiagram &copy);
    ~BlockDiagram();
    
    void init(int order,const double *a,const double *b);
    
    //N次の伝達関数
    double inject(double sample);
    
    //次数の取得
    int getOrder() const { return mOrder; }

    const double *getCoefficientA() const{ return mCoefficientA; }
    const double *getCoefficientB() const{ return mCoefficientB; }

    const BlockDiagram &operator=(const BlockDiagram &right);
private:
    
    //以前のサンプルの操作
    void initPreviousSample(int n);
    void pushPreviousSample(double val);
    void popPreviousSample();
    double getPreviousSample(int n);
    //以前のサンプル値（キューとして実現）
    deque< double > mPreviousSample;

    
    //係数
    double *mCoefficientA;
    double *mCoefficientB;
    
    void setOrder(int order){ mOrder = order; }
    int mOrder; //次数
};

#endif

