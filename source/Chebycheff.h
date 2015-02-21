#ifndef __CHEBYCHEFF_H__
#define __CHEBYCHEFF_H__

#include "Filter.h"

#include <cmath>
#include <complex>
using namespace std;

#define PI 3.1415926535897932384626433832795

class Chebycheff : public Filter{
public:
    Chebycheff();
    ~Chebycheff();
    
    void setSampleRate(double inSampleRate){ mSampleRate = inSampleRate; }
    double getSampleRate(){ return mSampleRate; }
    
    void decisionPrototype(double inPassFreq,
                            double inRippleGain,
                            double inStopFreq,
                            double inAttenuateGain);
    
    double getPrototypeCutFreq(){ return mPrototypeCutFreq; }
    double getPrototypePassFreq(){ return mPrototypePassFreq; }
    
    double digital2analog(double inOmega){ return 2*tan(inOmega/2); }

    void initTransferFunction();

    void printCharacteristic(char *inAmpFileName,char *inPhaseFileName);
private:
    double getAlpha(long i);
    double getBeta(long i);
    double getLowGamma(double inCutFreq);
    double getHighGamma(double inCutFreq);
    
    double asinh(double theta){ return log(theta+sqrt(theta*theta+1)); }
    double acosh(double theta){ return log(theta+sqrt((theta+1)*(theta-1))); }
    
    BlockDiagram *initLowTransferFunction(double inCutFreq);
    BlockDiagram *initHighTransferFunction(double inCutFreq);
    BlockDiagram *initBandTransferFunction(double inLowCutFreq,double inHighCutFreq);
    
    double mEpsilon;
    double mAlpha0;
    double mSampleRate;
    double mPrototypeCutFreq;
    double mPrototypePassFreq;
};

#endif

