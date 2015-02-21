#include "BlockDiagram.h"

#include <new>
#include <iostream>
using namespace std;


/*************************************
* BlockDockDiagram : �R���X�g���N�^
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   �Ȃ�
*
*�Ԃ�l
*   �Ȃ�
*
*�����̗���
*   �����o�ϐ���������
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
* ~BlockDockDiagram : �f�X�g���N�^
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   �Ȃ�
*
*�Ԃ�l
*   �Ȃ�
*
*�����̗���
*   �������Ȃ�
*
**************************************/
BlockDiagram::~BlockDiagram()
{
    delete [] mCoefficientA;
    delete [] mCoefficientB;
}


/*************************************
* init : �_�C�A�O�����̏�����
*
*����
*   order : �_�C�A�O�����̎���
*   a : �_�C�A�O�����̌W��
*   b : �_�C�A�O�����̌W��
*
*���[�J���ϐ�
*   �Ȃ�
*
*�Ԃ�l
*   �Ȃ�
*
**************************************/
void BlockDiagram::init(int order,const double *a,const double *b)
{
    //�����̐ݒ�
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
* inject : �f�[�^���_�C�A�O�����ɒ���
*
*����
*   sample : �T���v���l
*
*���[�J���ϐ�
*   data1 : �f�[�^�̒��ԏo��(FIR��)
*   data2 : �f�[�^�̍ŏI�o��(IIR��)
*   i : ���[�v�J�E���^
*
*�Ԃ�l
*   ���̃f�[�^���_�C�A�O������ʂ蔲������̃f�[�^���o�͂����
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
    
    //�T���v���l�̕ۑ�
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


//�L���[��������
void BlockDiagram::initPreviousSample(int n)
{
    int i;
    
    for(i=0;i<=n;i++)
        pushPreviousSample(0.0);
}


//�L���[�ɒl��ǉ�
void BlockDiagram::pushPreviousSample(double val)
{
    mPreviousSample.push_front(val);
}


//�L���[����l�������o��
void BlockDiagram::popPreviousSample()
{
    mPreviousSample.pop_back();
}


//�L���[����l�����o��
double BlockDiagram::getPreviousSample(int n)
{
    return mPreviousSample[n-1];
}

