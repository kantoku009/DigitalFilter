#include "./SoundInformation/SoundInformation.h"
#include "./SoundInformation/WaveFormatOperator.h"
#include "Filter.h"
#include "Chebycheff.h"

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

int main(int argc, char* argv[])
{
    WaveFormatOperator waveLow,waveHigh,waveMix;
    SoundInformation *lowSound,*highSound,*mixSound;
    Chebycheff low,high;
	string outLowFileName,outHighFileName,outMixFileName;
    char tempStr[32];
    
    string inFileName(argv[1]);
    double dBPerOctDown=atof(argv[2]);
    //cout << "input filename?" << endl;
    //cin >> inFileName;
    //cout << "dB/Oct down?" << endl;
    //cin >> dBPerOctDown;
    
    sprintf(tempStr,"%2.2f",dBPerOctDown);
    outLowFileName = "low-";
    outLowFileName += tempStr;
    outLowFileName += "dB.wav";
    
    outHighFileName = "high-";
    outHighFileName += tempStr;
    outHighFileName += "dB.wav";
    
    cout << "begin: low pass filter" << endl;
    cout << "load file now" << endl;
    if(waveLow.readWaveFile(inFileName) == true){
        
        cout << "up sampling" << endl;
        waveLow.setSampleRate(waveLow.getSampleRate()*2);
        
        low.setSoundInformation(&waveLow);
        low.setSampleRate(waveLow.getSampleRate());
        
        low.selectFilterMode(kLowpass);
        
        cout << "make prototype" << endl;
        low.decisionPrototype(400,-1,800,dBPerOctDown);
        
        cout << "setting cutoff frequency" << endl;
        low.setCutoffFreq(400);
        
        cout << "init transfer function" << endl;
        low.initTransferFunction();
        
        //cout << "get outLowAmp and outLowPhase" << endl;
        //low.printCharacteristic("outLowAmp.txt","outLowPhase.txt");

        for(short chan=0;chan<waveLow.getNumChannels();chan++){
            cout << "disposal channel" << chan << endl;
            low.runFilter(0,waveLow.getSamplesPerChannel(),chan);
        }
        
        cout << "down sampling" << endl;
        waveLow.setSampleRate(waveLow.getSampleRate()/2);
        
        cout << "write file now" << endl;
        waveLow.writeWaveFile(outLowFileName);

    }else{
        cout << "error: load file" << endl;
    }
    cout << "finish: low pass filter" << endl;
    
    
    
    cout << endl;
    
    
    
    cout << "begin: high pass filter" << endl;
    cout << "load file now" << endl;
    if(waveHigh.readWaveFile(inFileName) == true){
        
        cout << "up sampling" << endl;
        waveHigh.setSampleRate(waveHigh.getSampleRate()*2);
        
        high.setSoundInformation(&waveHigh);
        high.setSampleRate(waveHigh.getSampleRate());
        
        high.selectFilterMode(kHighpass);
        
        cout << "make prototype" << endl;
        high.decisionPrototype(400,-1,800,dBPerOctDown);
        
        cout << "setting cutoff frequency" << endl;
        high.setCutoffFreq(400);
        
        cout << "init transfer function" << endl;
        high.initTransferFunction();
        
        //cout << "get outHighAmp and outHighPahse" << endl;
        //high.printCharacteristic("outHighAmp.txt","outHighPhase.txt");

        for(short chan=0;chan<waveHigh.getNumChannels();chan++){
            cout << "disposal channel" << chan << endl;
            high.runFilter(0,waveHigh.getSamplesPerChannel(),chan);
        }
        
        cout << "down sampling" << endl;
        waveHigh.setSampleRate(waveHigh.getSampleRate()/2);
        
        cout << "write file now" << endl;
        waveHigh.writeWaveFile(outHighFileName);

    }else{
        cout << "error: load file" << endl;
    }
    cout << "finish: high pass filter" << endl;
    
    cout << "Mixed low pass filter and high pass filter" << endl;
    lowSound = (SoundInformation*)&waveLow;
    highSound = (SoundInformation*)&waveHigh;
    mixSound = (SoundInformation*)&waveMix;

    *mixSound = *lowSound + *highSound;

    outMixFileName = "mix-";
    outMixFileName += tempStr;
    outMixFileName += "dB.wav";
    waveMix.writeWaveFile(outMixFileName);
    cout << "finish" << endl;
    return 0;
}

