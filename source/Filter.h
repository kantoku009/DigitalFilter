#ifndef __FILTER_H__
#define __FILTER_H__

#include "./SoundInformation/SoundInformation.h"
#include "BlockDiagram.h"

typedef enum{kLowpass,kHighpass,kBandpass} FilterMode;


class FilterError{
public:
    FilterError(char *message)
    {
        int len;
        
        len = strlen(message);
        mMessage = new char [len+1];
        strcpy(mMessage,message);
    }
    const char *what() const { return mMessage; }
private:
    char *mMessage;
};


class Filter
{
public:
    Filter();
    ~Filter();
    
    void selectFilterMode(FilterMode inKindFilter){ mKindFilter = inKindFilter; }
    FilterMode getFilterMode(){ return mKindFilter; }
    
    void setCutoffFreq(double inCutFreq);
    double getCutoffFreq();
    
    void setCutoffFreq(double inLowCutFreq,double inHighCutFreq);
    double getLowCutoffFreq();
    double getHighCutoffFreq();
    
    long getOrderNumber(){ return mOrderNumber; }
    
    void setSoundInformation(SoundInformation *inInfo){ mInfo = inInfo; }
    SoundInformation *getSoundInformation(){ return mInfo; }
    
    bool runFilter(long inBegin,long inEnd,short inChan);
    
protected:
    void setOrderNumber(long inOrder){ mOrderNumber = inOrder; }

    BlockDiagram *mSection;
    
    SoundInformation *mInfo;
    
private:
    double transferFunction(double valSample);
    
    FilterMode mKindFilter;
    long mOrderNumber;
    double mLowCutoffFreq,mHighCutoffFreq;
};

#endif

