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
    
    //N���̓`�B�֐�
    double inject(double sample);
    
    //�����̎擾
    int getOrder() const { return mOrder; }

    const double *getCoefficientA() const{ return mCoefficientA; }
    const double *getCoefficientB() const{ return mCoefficientB; }

    const BlockDiagram &operator=(const BlockDiagram &right);
private:
    
    //�ȑO�̃T���v���̑���
    void initPreviousSample(int n);
    void pushPreviousSample(double val);
    void popPreviousSample();
    double getPreviousSample(int n);
    //�ȑO�̃T���v���l�i�L���[�Ƃ��Ď����j
    deque< double > mPreviousSample;

    
    //�W��
    double *mCoefficientA;
    double *mCoefficientB;
    
    void setOrder(int order){ mOrder = order; }
    int mOrder; //����
};

#endif

