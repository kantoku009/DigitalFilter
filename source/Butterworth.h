#ifndef __BUTTERWORTH_H__
#define __BUTTERWORTH_H__

#include "Filter.h"

#include <cmath>
#include <complex>
using namespace std;

#define PI 3.1415926535897932384626433832795

class Butterworth : public Filter{
public:
    Butterworth();
    ~Butterworth();
    
    void setSampleRate(double inSampleRate){ mSampleRate = inSampleRate; }
    double getSampleRate(){ return mSampleRate; }
    
    void decisionPrototype(double inPassFreq,double inRippleGain,
                           double inStopFreq,double inAttenuateGain);
    
    double getPrototypeCutFreq(){ return mPrototypeCutFreq; }
    
    double digital2analog(double inOmega){ return 2*tan(inOmega/2); }

    void initTransferFunction();

    void printCharacteristic(char *inAmpFileName,char *inPhaseFileName);
private:
    double getAlpha(long i);
    double getLowBeta(double inCutFreq);
    double getHighBeta(double inCutFreq);
    
    BlockDiagram *initLowTransferFunction(double inCutFreq);
    BlockDiagram *initHighTransferFunction(double inCutFreq);
    BlockDiagram *initBandTransferFunction(double inLowCutFreq,double inHighCutFreq);
    
    double mSampleRate;
    double mPrototypeCutFreq;
};

#endif

