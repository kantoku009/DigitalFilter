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
* setCutoffFreq : �J�b�g�I�t���g����ݒ�
*
*����
*   inCutFreq : Lowpass �܂���Highpass �̃J�b�g�I�t���g��
*
*���[�J���ϐ�
*   err : 
*
*�Ԃ�l
*   �Ȃ�
*
*���l
*   ���̃����o�֐��̓��[�h��Lowpass�܂���Highpass�t�B���^�̂Ƃ��Ɏg�p
*   Bandpass�̎��ɂ͎g���Ȃ�
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
* getCutoffFreq : �J�b�g�I�t���g�����擾
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   err : 
*
*�Ԃ�l
*   Lowpass�܂���Highpass�̃J�b�g�I�t���g��
*
*���l
*   ���̃����o�֐��̓��[�h��Lowpass�܂���Highpass�t�B���^�̂Ƃ��Ɏg�p
*   Bandpass�̎��ɂ͎g���Ȃ�
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
* setCutoffFreq : �������ƒႢ���̃J�b�g�I�t���g����ݒ�
*
*����
*   inLowCutFreq : �Ⴂ���̃J�b�g�I�t���g��
*   inHighCutFreq : �������̃J�b�g�I�t���g��
*
*���[�J���ϐ�
*   err : 
*
*�Ԃ�l
*   �Ȃ�
*
*���l
*   ���̃����o�֐��̓��[�h��Bandpass�t�B���^�̂Ƃ��Ɏg�p
*   Lowpass,Highpass�̎��ɂ͎g���Ȃ�
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
* getLowCutoffFreq : �Ⴂ���̃J�b�g�I�t���g�����擾
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   err : 
*
*�Ԃ�l
*   �Ⴂ���̃J�b�g�I�t���g��
*
*���l
*   ���̃����o�֐��̓��[�h��Bandpass�t�B���^�̂Ƃ��Ɏg�p
*   Lowpass,Highpass�̎��ɂ͎g���Ȃ�
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
* getHighCutoffFreq : �������̃J�b�g�I�t���g�����擾
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   err : 
*
*�Ԃ�l
*   �������̃J�b�g�I�t���g��
*
*���l
*   ���̃����o�֐��̓��[�h��Bandpass�t�B���^�̂Ƃ��Ɏg�p
*   Lowpass,Highpass�̎��ɂ͎g���Ȃ�
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
* runFilter : �t�B���^�����s����֐�
*
*����
*   begin : �������n�߂�T���v��
*   end : �������I���T���v��
*   chan : ���������s����`�����l��
*
*���[�J���ϐ�
*   currSample : ���[�v���ŁA���ݏ������Ă���T���v��
*   currSampleValue : ���[�v���ŁA���ݏ������Ă���T���v���l
*   data : currSample������������̃T���v���l
*
*�Ԃ�l
*   ���������������Ȃ��true�C���s�����Ȃ��false
*
***********************************************************************/
bool Filter::runFilter(long begin,long end,short chan)
{
    long currSample;
    double currSampleValue,data;
    
    for(currSample=begin;currSample<=end;currSample++){
        currSampleValue = mInfo->readSampleFromMemory(currSample,chan);
        
        data = transferFunction(currSampleValue);
        
        //�v�Z�����l��ۑ�
        mInfo->writeSampleIntoMemory(data,currSample,chan);
    }

    return true;
}


/************************************************************
*transferFunction : �`�B�֐�
*
*����
*   valSample : �`�B�֐��ɓn���T���v���l
*
*���[�J���ϐ�
*   data : �`�B�֐��̂��ꂼ��̃Z�N�V������ʂ��Ă��l
*
*�Ԃ�l
*   ����(valSample)���`�B�֐���ʂ�ʂ����o��(data)
*
*���l
*   H(z)��`�B�֐��Ƃ���
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

