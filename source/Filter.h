#ifndef __FILTER_H__
#define __FILTER_H__

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
    
    double passFilter(double sample);
    
protected:
    void setOrderNumber(long inOrder){ mOrderNumber = inOrder; }

    BlockDiagram *mSection;
    
private:
    double transferFunction(double valSample);
    
    FilterMode mKindFilter;
    long mOrderNumber;
    double mLowCutoffFreq,mHighCutoffFreq;
};

#endif

