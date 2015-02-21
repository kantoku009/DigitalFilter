#include "Chebycheff.h"

#include <iostream>
#include <fstream>
#include <new>
#include <complex>
using namespace std;

Chebycheff::Chebycheff()
{
    mSampleRate = 0.0;
    mPrototypeCutFreq = 0.0;
}


Chebycheff::~Chebycheff()
{
    delete [] mSection;
}


/***********************************************************
* decisionPrototype : �v���g�^�C�v���[�p�X�t�B���^������
*
*����
*   inPassFrquency : �p�X�o���h�̎��g��[Hz]
*   inRippleGain : �΍��̗�[dB]
*   inStopFrequency : �X�g�b�v�o���h�̎��g��[Hz]
*   inAttenuateGain : ������[dB]
*
*���[�J���ϐ�
*   omega_dp : �f�W�^�����g���̃p�X�o���h[rad/sec]
*   omega_ds : �f�W�^�����g���̃X�g�b�v�o���h[rad/sec]
*   omega_ap : �A�i���O���g���̃p�X�o���h[rad/sec]
*   omega_as : �A�i���O���g���̃X�g�b�v�o���h[rad/sec]
*   n : �v���g�^�C�v���[�p�X�t�B���^�̎���
*   fs : �T���v�����O���g��
*
*�Ԃ�l
*   �Ȃ�
*
*�����̗���
*
*���l
*   ���������肷�鎮
\end{verbatim}

       \[ n = \frac{ \cosh^{-1}(\pm\frac{1}{\varepsilon}\sqrt{10^{\frac{g}{10}} -1})}{\cosh^{-1}(\frac{\omega _{as}}{\omega _{ap}})} \]

\begin{verbatim}
*   �v���g�^�C�v�̃J�b�g�I�t���g�������肷�鎮
\end{verbatim}

       \[ \omega_{ac} = \omega_{ap} \cosh[\frac{1}{n} \cosh^{-1}(\pm \frac{1}{\varepsilon} \sqrt{10^{\frac{3}{10}} - 1})] \]

\begin{verbatim}
************************************************************/
void Chebycheff::decisionPrototype( double inPassFreq,
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

    //�T���v�����O���g���ŋK�i��
    passFreq = inPassFreq / fs;
    stopFreq = inStopFreq / fs;
    
    //���g������p���g���֕ϊ�
    omega_dp = 2*PI * passFreq;
    omega_ds = 2*PI * stopFreq;
    
    //�f�W�^���̎��g������A�i���O�̎��g���֕ϊ�
    omega_ap = digital2analog(omega_dp);
    omega_as = digital2analog(omega_ds);

    //�Q�C���̐�Βl��
    if(inRippleGain < 0.0) inRippleGain = -inRippleGain;
    if(inAttenuateGain < 0.0) inAttenuateGain = -inAttenuateGain;

    //epsilon�̌���
    mEpsilon = sqrt(pow(10,inRippleGain/10) - 1);
    
    //�����̌���
    n = acosh((1/mEpsilon)*sqrt(pow(10,inAttenuateGain/10)-1)) 
            / acosh(omega_as/omega_ap);
    setOrderNumber(static_cast<long>(n) + 1);

    //alpha0�̌���
    mAlpha0 = sinh( 1/(static_cast<double>(getOrderNumber())) * asinh(1/mEpsilon) );

    //�v���g�^�C�v�̃J�b�g�I�t���g���̌���
    mPrototypeCutFreq = omega_ap
            *cosh( (1/(static_cast<double>(getOrderNumber()))) 
            * acosh((1/mEpsilon)*sqrt(pow(10,0.3)-1)) );

    mPrototypePassFreq = omega_ap;

}


void Chebycheff::initTransferFunction()
{
    try{
        switch (getFilterMode()){
            case kLowpass:
            mSection = initLowTransferFunction(getCutoffFreq());
            break;
            
            case kHighpass:
            mSection = initHighTransferFunction(getCutoffFreq());
            break;
            
            case kBandpass:
            mSection = initBandTransferFunction(getLowCutoffFreq()
                                                ,getHighCutoffFreq());
            break;
            
            default:
            throw FilterError((char*)"Chebycheff error: cause unknown");
        }
    }catch (FilterError err){
        cerr << err.what() << endl;
    }
}


/******************************************************************************
* initLowTransferFunction : �`�B�֐��̏������ƌW���̌���
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   alpha : �W��
*   beta : �W��
*   a[3] : �P���܂��͂Q���̓`�B�֐��̕��q�̌W��
*   b[3] : �P���܂��͂Q���̓`�B�֐��̕���̌W��
*   orderNumber : �`�B�֐��̎���
*   m : �`�B�֐����P���܂��͂Q���ɕ������Ƃ��̃Z�N�V�����̌�
*   omega_ap : �v���g�^�C�v���[�p�X�t�B���^�̃J�b�g�I�t���g��
*
*�Ԃ�l
*   �Ȃ�
*
*�����̗���
*   �`�B�֐����P���܂��͂Q���̓`�B�֐��ɕ����������̃u���b�N�_�C�A�O�������m��
*   �u���b�N�_�C�A�O�����̌W��������
*
*���l
*   �`�B�֐��̎����������Ȃ�΂P���̃u���b�N�_�C�A�O�����͎g��Ȃ�
*   \omega_{ap}�̓v���g�^�C�v���[�p�X�t�B���^�̃p�X�o���h
*   �����ł́A���g���͑S�ăT���v�����O���g���ŋK�i������Ă���̂ŁA����T��1
*   ������TEX�ŏ����Ă���
*
*****************************************************************************/
BlockDiagram *Chebycheff::initLowTransferFunction(double inCutFreq)
{
    double alpha,beta,gamma;
    double a[3],b[3];
    long   orderNumber,numSection;
    bool isOrderNumberEven;
    double omega_ap=getPrototypePassFreq();
    BlockDiagram *lowpassSection;
    
    //�������擾
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;

    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else{
        numSection = (orderNumber-1) / 2;
    }
    numSection++;
    
    try{
        lowpassSection = new BlockDiagram [numSection];
    }catch(bad_alloc err){
        cerr << err.what() << endl;
        return 0;
    }

    //�W��������
    gamma = getLowGamma(inCutFreq);
    //1���̓`�B�֐��̏�����
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
        b[0] = mAlpha0*omega_ap*(1-gamma) 
                / ( (mAlpha0*omega_ap+2) - (mAlpha0*omega_ap-2)*gamma );
        b[1] = -( (mAlpha0*omega_ap-2) - (mAlpha0*omega_ap+2)*gamma )
                 / ( (mAlpha0*omega_ap+2) - (mAlpha0*omega_ap-2)*gamma );
        b[2] = 0;
    }
    lowpassSection[0].init(2,a,b);
    
    //2���̓`�B�֐��̏�����
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
* initHighTransferFunction : �`�B�֐��̏������ƌW���̌���
*
*����
*   �Ȃ�
*
*���[�J���ϐ�
*   alpha : �W��
*   beta : �W��
*   a[3] : �P���܂��͂Q���̓`�B�֐��̕��q�̌W��
*   b[3] : �P���܂��͂Q���̓`�B�֐��̕���̌W��
*   orderNumber : �`�B�֐��̎���
*   m : �`�B�֐����P���܂��͂Q���ɕ������Ƃ��̃Z�N�V�����̌�
*   isOrderNumber : �`�B�֐��̎�����������������L������B�����Ȃ��true
*   omega_ac : �v���g�^�C�v���[�p�X�t�B���^�̃J�b�g�I�t���g��
*
*�Ԃ�l
*   �Ȃ�
*
*�����̗���
*   �`�B�֐����P���܂��͂Q���̓`�B�֐��ɕ����������̃u���b�N�_�C�A�O�������m��
*   �u���b�N�_�C�A�O�����̌W��������
*
*���l
*   �`�B�֐��̎����������Ȃ�΂P���̃u���b�N�_�C�A�O�����͎g��Ȃ�
*   \omega_{ac}�̓v���g�^�C�v���[�p�X�t�B���^�̃J�b�g�I�t���g��
*   �����ł́A���g���͑S�ăT���v�����O���g���ŋK�i������Ă���̂ŁA����T��1
*   ������TEX�ŏ����Ă���
*
*   �`�B�֐�
*   \[ HHP(z) = HHP_{0}(z) \prod^{m}_{i=1} HHP_{i} \]
*
*****************************************************************************/
BlockDiagram *Chebycheff::initHighTransferFunction(double inCutFreq)
{
    double alpha,beta,gamma;
    double a[3],b[3];
    long   orderNumber,numSection;
    bool   isOrderNumberEven;
    double omega_ap=getPrototypePassFreq();
    BlockDiagram *highpassSection;
    
    //�������擾
    orderNumber = getOrderNumber();
    isOrderNumberEven = (orderNumber%2 == 0)? true:false;
    if(isOrderNumberEven)
        numSection = orderNumber / 2;
    else{
        numSection = (orderNumber-1) / 2;
    }
    numSection++;
    
    try{
        highpassSection = new BlockDiagram [numSection];
    }catch(bad_alloc err){
        cerr << err.what() << endl;
        return 0;
    }

    //�W��������
    gamma = getHighGamma(inCutFreq);
    
    //1���̓`�B�֐��̏�����
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
        b[0] = mAlpha0*omega_ap*(1-gamma) 
                / ( (mAlpha0*omega_ap+2) - (mAlpha0*omega_ap-2)*gamma );
        b[1] = ( (mAlpha0*omega_ap-2) - (mAlpha0*omega_ap+2)*gamma ) 
                / ( (mAlpha0*omega_ap+2) - (mAlpha0*omega_ap-2)*gamma );
        b[2] = 0;
    }
    highpassSection[0].init(2,a,b);
    
    for(long i=1;i<numSection;i++){
        //�Q���̓`�B�֐��̏�����
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


BlockDiagram *Chebycheff::initBandTransferFunction(double inLowCutFreq,
                                                    double inHighCutFreq)
{
    BlockDiagram *lowpassSection,*highpassSection,*bandpassSection;
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
    
    bandpassSection = new BlockDiagram [numSection*2];
    for(long i=0;i<numSection;i++){
        bandpassSection[i] = lowpassSection[i];
        bandpassSection[i+numSection] = highpassSection[i];
    }
    
    delete [] lowpassSection;
    delete [] highpassSection;
    
    return bandpassSection;
}


/**********************************************************************
*getAlpha : �W�����̎擾
*
*����
*   i : �`�B�֐���$\prod_i$
*
*���[�J���ϐ�
*   n : �`�B�֐��̎���
*   v : �W��
*
*�Ԃ�l
*   �W������Ԃ�
*
*���l
\end{verbatim}

   \[ \alpha_{i} = 2\cos(\frac{\pi}{n}v) \]

\begin{verbatim}
*   v = i - (1/2) (n�������̂Ƃ�)
*   v = i         (n����̂Ƃ�)
*
***********************************************************************/
double Chebycheff::getAlpha(long i)
{
    long n;
    double v;
    
    n = getOrderNumber();
    if(n%2 == 0)
        v = i - 0.5;
    else
        v = i;
    
    return 2*mAlpha0*cos(PI*v/n);
}


/**********************************************************************
* getBeta : �W�����̎擾
*
*����
*   i : �`�B�֐���$\prod_i$
*
*���[�J���ϐ�
*   n : �`�B�֐��̎���
*   v : �W��
*
*�Ԃ�l
*   �W������Ԃ�
*
*���l
\end{verbatim}

   \[ \beta_{i} = \alpha_{0}^{2} + sin(\frac{\pi}{n}v)^2 \]

\begin{verbatim}
*   v = i - (1/2) (n�������̂Ƃ�)
*   v = i         (n����̂Ƃ�)
*
***********************************************************************/
double Chebycheff::getBeta(long i)
{
    long n;
    double v;
    
    n = getOrderNumber();
    if(n%2 == 0)
        v = i - 0.5;
    else
        v = i;
    
    return mAlpha0*mAlpha0 + sin(PI*v/n)*sin(PI*v/n);
}



/*********************************************************************
* getLowGamma  :  �`�B�֐��̌W�����擾
*
*����
*   inCutFreq : �J�b�g�I�t���g��
*
*���[�J���ϐ�
*   fs :  �T���v�����O���g��
*   omega_dc : �f�W�^���̃J�b�g�I�t���g��[rad/sec]
*   omega_ac :  �A�i���O�̃J�b�g�I�t���g��[rad/sec]
*   theta : �v���g�^�C�v���[�p�X�t�B���^�̃J�b�ƃI�t���g��[rad/sec]
*
*�Ԃ�l
*   �`�B�֐��̌W����Ԃ�
*
*
*�����̗���
*   �J�b�g�I�t���g�����T���v�����O���g���ŋK�i�����A�f�W�^���̊p���g���ɂ���
*   �f�W�^���̊p���g�����A�i���O�̊p���g���ɂ���i�v���E�H�[�s���O�j
*   �v���g�^�C�v���[�p�X�t�B���^�̃J�b�g�I�t���g�����擾
*   �W���̌v�Z
*
*���l
*   �W����
\end{verbatim}

   \[ \gamma = \frac{\sin(\frac{\omega_{ac}-\omega_{c}}{2})T}{\sin(\frac{\omega_{ac}+\omega_{c}}{2})T} \]

\begin{verbatim}
*
**********************************************************************/
double Chebycheff::getLowGamma(double inCutFreq)
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
* getHighGamma  :  �`�B�֐��̌W�����擾
*
*����
*   inCutFreq : �J�b�g�I�t���g��
*
*���[�J���ϐ�
*   fs :  �T���v�����O���g��
*   omega_dc : �f�W�^���̃J�b�g�I�t���g��[rad/sec]
*   omega_ac :  �A�i���O�̃J�b�g�I�t���g��[rad/sec]
*   theta : �v���g�^�C�v���[�p�X�t�B���^�̃J�b�ƃI�t���g��[rad/sec]
*
*�Ԃ�l
*   �`�B�֐��̌W����Ԃ�
*
*
*�����̗���
*   �J�b�g�I�t���g�����T���v�����O���g���ŋK�i�����A�f�W�^���̊p���g���ɂ���
*   �f�W�^���̊p���g�����A�i���O�̊p���g���ɂ���i�v���E�H�[�s���O�j
*   �v���g�^�C�v���[�p�X�t�B���^�̃J�b�g�I�t���g�����擾
*   �W���̌v�Z
*
*���l
*   �W����
\end{verbatim}

   \[ \gamma = -\frac{\cos(\frac{\omega_{ac}+\omega_{c}}{2})T}{\cos(\frac{\omega_{ac}-\omega_{c}}{2})T} \]

\begin{verbatim}
*
**********************************************************************/
double Chebycheff::getHighGamma(double inCutFreq)
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
*printCharacteristic : �`�B�֐��̐U�������ƈʑ��������t�@�C���ɏo��
*
*����
*   fNameAmp : �U���������o�͂���t�@�C����
*   fNamePhase : �ʑ��������o�͂���t�@�C����
*
*���[�J���ϐ�
*   fpAmp : �U���������o�͂���t�@�C���̃t�@�C���X�g���[��
*   fpPhase : �ʑ��������o�͂���t�@�C���̃t�@�C���X�g���[��
*   sectionOrder :  �`�B�֐��̃Z�N�V�����̎���
*   n : �`�B�֐��̎���
*   m :  �`�B�֐����P���܂��͂Q���ɕ������Ƃ��̃Z�N�V�����̌�
*   isOrderEven :  �`�B�֐����������ǂ����B�����Ȃ��true
*   a : �`�B�֐���1���܂��͂Q���̃Z�N�V�����̌W��
*   b : �`�B�֐���1���܂��͂Q���̃Z�N�V�����̌W��
*   omega : �U�������A�ʑ������̊p���g��[rad/sec]
*   e1 : ��Βl��1�ŁC�ʑ��p�x-omega�̕��f��
*   e2 : ��Βl��1�ŁC�ʑ��p�x-2*omega�̕��f��
*   h : �C���p���X����
*   amp : �U���̒l
*   phase : �ʑ��̒l
*   freq : ���g��[Hz]
*
*�Ԃ�l
*   �Ȃ�
*
***********************************************************************/
void Chebycheff::printCharacteristic(char *fNameAmp,char *fNamePhase)
{
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
            a = mSection[i].getCoefficientA();
            b = mSection[i].getCoefficientB();
            h *= b[0] * (a[0] + a[1]*e1 + a[2]*e2) / (1.0 - b[1]*e1 - b[2]*e2);
        }

        amp = abs(h);
        phase = arg(h);
        
        fpAmp << freq << ',' << 20*log10(amp) << endl;
        fpPhase << freq << ',' << phase << endl;

    }
}

