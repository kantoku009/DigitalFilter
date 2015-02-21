#include "SoundInformation.h"
#include <new>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

/*******************************************************************
*SoundInformationのコンストラクタ
*
*引数
*	sampleRate:サンプリング周波数
*	bitsPerSample:1sampleにつきのbit数
*	numChannels:チャンネル数
*	samplesPerChannel:1channelにつきのサンプル数
*
*ローカル変数
*	なし
*
*返り値
*	なし
*
*処理の流れ
*	メンバ変数の初期化
*
********************************************************************/
SoundInformation::SoundInformation(long  sampleRate,
                                   short bitsPerSample,
                                   short numChannels,
                                   long  samplesPerChannel)
{
	try{
		this->m_pdSample = new double [samplesPerChannel*numChannels];
	}
	catch(bad_alloc err){
		cerr << "SoundInformation error: SoundInformation::SoundInformation. " << err.what() << endl;
	}
	memset(this->m_pdSample, 0, samplesPerChannel*numChannels*sizeof(double));
	
	this->m_lSampleRate = sampleRate;
	this->m_shBitsPerSample = bitsPerSample;
	this->m_shNumChannels = numChannels;
	this->m_lSamplesPerChannel = samplesPerChannel;
}


/**************************************************************
*コピーコンストラクター
*
**************************************************************/
SoundInformation::SoundInformation(const SoundInformation &ob)
{	

	this->m_lSampleRate = ob.getSampleRate();
	this->m_shBitsPerSample = ob.m_shBitsPerSample;

	if(ob.getNumSamples() != getNumSamples()){
		this->m_lSamplesPerChannel = ob.m_lSamplesPerChannel;
		this->m_shNumChannels = ob.m_shNumChannels;
		try
		{
			this->m_pdSample = new double [this->getNumSamples()];
		}
		catch(bad_alloc err)
		{
			cerr << "SoundInformation::operator=. " << err.what() << endl;
		}
	}
	memcpy(this->m_pdSample, ob.m_pdSample, this->getNumSamples()*sizeof(double));
	
}

/*************************************************************************************************
*setSampleRate : サンプリング周波数の設定
*
*引数
*	sampleRate : 設定後のサンプリング周波数
*
*ローカル変数
*	data : サンプル値の値
*	delta : 現在のサンプリング周波数と設定したいサンプリング周波数の比(再サンプリング時に必要)
*	samplePerChannel : サンプリング周波数を設定した後の1channelにつきのサンプル数
*	temp : 現在のSoundInformationを保存しておく変数
*
*返り値
*	なし
*
*処理の流れ
*	現在のサンプリング周波数と設定後のサンプリング周波数が同じ値ならば何もせずにreturn
*	tempに現在のSoundInformationを保存
*	現在のサンプリング周波数と設定後のサンプリング周波数の比を求め、設定後のsamplePerCahnnelを設定
*	再サンプリングの処理
*	サンプリング周波数の設定
*
**************************************************************************************************/
void SoundInformation::setSampleRate(long sampleRate)
{
	double				data,delta;
	long				samplesPerChannel;
	SoundInformation	temp;
	
	if(sampleRate == getSampleRate()) return;
	
	temp = *this;

	delta = (double)getSampleRate()/sampleRate;
	
	samplesPerChannel = getSamplesPerChannel() / delta;
	setSamplesPerChannel(samplesPerChannel);

	for(short channel=0;channel<getNumChannels();channel++){
		for(long index=0;index<getSamplesPerChannel();index++){
			data = temp.interpolation(index*delta,channel);
			writeSampleIntoMemory(data,index,channel);
		}
	}

	this->m_lSampleRate = sampleRate;
}

/***************************************************************************
*setBitsPerSample : 量子化ビットの設定
*
*引数
*	bitsPerSample : 設定後の量子化ビット
*
*
*ローカル変数
*	なし
*
*返り値
*	なし
*
*処理の流れ
*	設定前の量子化ビットと設定後の量子化ビットが同じならばすぐにreturn
*	量子化ビットを設定
*
**************************************************************************/
void SoundInformation::setBitsPerSample(short bitsPerSample)
{
	
	if(bitsPerSample == this->getBitsPerSample()) return;
	this->m_shBitsPerSample = bitsPerSample;
}


/***************************************************************************
*setNumChannels : チャンネル数を設定
*
*引数
*	numChannels : 設定後のチャンネル数
*
*ローカル変数
*	numSamples : 全サンプル数
*	data : サンプル値を保存する
*	lessChannel : 設定前と設定後のどちらかの少ないほうのチャンネル数
*	temp : 設定前のSoundInformation
*
*返り値
*	なし
*
*処理の流れ
*	tempに現在の状態のSoundInformationをコピー
*	設定後のチャンネル数と全サンプル数を設定
*	dataを保持するメモリを確保
*	設定前と設定後でチャンネル数の少ないほうを記録
*	サンプル値のコピー
*
****************************************************************************/
void SoundInformation::setNumChannels(short numChannels)
{
	long				numSamples;
	double				data;
	short				lessChannel;
	SoundInformation	temp;
	
	if(numChannels == getNumChannels()) return;
	
	temp = *this;
	
	this->m_shNumChannels = numChannels;
	numSamples = this->getNumSamples();
	
	try
	{
		delete [] this->m_pdSample;
		this->m_pdSample = new double [numSamples];
	}
	catch(bad_alloc err)
	{
		cerr << "SoundInformation::setNumChannels. " << err.what() << endl;
	}
	memset(this->m_pdSample, 0, numSamples*sizeof(double));
	
	if(temp.getNumChannels() < this->getNumChannels())
		lessChannel = temp.getNumChannels();
	else
		lessChannel = this->getNumChannels();
		
	for(long index=0;index<getSamplesPerChannel();index++){
		for(short channel=0;channel<lessChannel;channel++){
			data = temp.readSampleFromMemory(index,channel);
			writeSampleIntoMemory(data,index,channel);
		}
	}
}


/************************************************************************
*setSamplePerChannel : １チャンネルのサンプル数を設定
*
*引数
*	samplesPerChannel : 設定後の１チャンネルのサンプル数
*
*ローカル変数
*	temp : 設定前のSoundInformation
*	oldNumSamples : 設定前の全サンプル数
*	newNumSamples : 設定後の全サンプル数
*	lessNumSamples : 設定前と設定後の少ない方の全サンプル数
*
*返り値
*	なし
*
*処理の流れ
*	tempに現在の状態のSoundInformationをコピー
*	設定前の全サンプル数と設定後の全サンプル数を記憶
*	設定前と設定後での全サンプル数の少ないほうを記憶
*	全サンプル数、分だけメモリを確保
*	確保したメモリにサンプル値をコピー
*
*************************************************************************/
void SoundInformation::setSamplesPerChannel(long samplesPerChannel)
{
	SoundInformation temp;
	long oldNumSamples,newNumSamples,lessNumSamples;
	
	if(samplesPerChannel == getSamplesPerChannel()) return;
	
	temp = *this;
	
	oldNumSamples = this->getNumSamples();
	this->m_lSamplesPerChannel = samplesPerChannel;
	newNumSamples = this->getNumSamples();
	if(oldNumSamples < newNumSamples)
		lessNumSamples = oldNumSamples;
	else
		lessNumSamples = newNumSamples;

	try
	{
		delete [] this->m_pdSample;
		this->m_pdSample = new double [newNumSamples];
	}
	catch(bad_alloc err)
	{
		cerr << "SoundInformation::setSamplesPerChannel. " << err.what() << endl;
	}

	memset(this->m_pdSample, 0, newNumSamples*sizeof(double));
	memcpy(this->m_pdSample, temp.m_pdSample, lessNumSamples*sizeof(double));
}


/***************************************************************************************
*getBytesPerSample : １サンプルのバイト数を取得
*
*引数
*	なし
*
*ローカル変数
*	bytesPerSample : １サンプルのバイト数
*
*返り値
*	1サンプルのバイト数
*
*処理の流れ
*	1サンプルのビット数を8で割りバイト数を算出し,
*	1サンプルのビット数が8の倍数でなければ，bytesPerSampeに+1する
*	bytesPerSampleが4byteより小さければ、bytesPerSampleはそのままで，
*	大きければbytesPerSampleを4に設定
*
****************************************************************************************/
short SoundInformation::getBytesPerSample() const 
{
	short bytesPerSample;
	
	bytesPerSample = this->m_shBitsPerSample/8 + (this->m_shBitsPerSample%8 != 0);
	bytesPerSample = (bytesPerSample < sizeof(long))? bytesPerSample:sizeof(long);
	
	return bytesPerSample;
}


/****************************************************************************************
*readSampleFromMemory : メモリからサンプル値を取得
*
*引数
*	num : 取得するサンプルが最初から何番目のものかを指定
*	channel : 取得するサンプルのチャンネルを指定
*
*ローカル変数
*	index : 1ブロックの中の何番目かを指定
*
*返り値
*	サンプル値
*
*処理の流れ
*	indexが全サンプル数より小さければサンプル値を返す
*	indexが全サンプル数より大きければ0を返す
*
****************************************************************************************/
double SoundInformation::readSampleFromMemory(long num,short channel)  const
{
	long index;
	short numChan;
	
	numChan = this->getNumChannels();
	index = numChan*num + channel;
	
	if(index < getNumSamples())
		return this->m_pdSample[index];
	else
		return 0.0;
}


/****************************************************************************************
*writeSampleIntoMemory : メモリにサンプル値を書き込む
*
*引数
*	sample : 書き込むサンプル値
*	num : 書き込むサンプルが最初から何番目のものかを指定
*	channel : 書き込むサンプルのチャンネルを指定
*
*ローカル変数
*	index : 1ブロックの中の何番目かを指定
*
*返り値
*	なし
*
*処理の流れ
*	indexが全サンプル数より小さければサンプル値を書き込む，
*	indexが全サンプル数より大きければ書き込まない
*
****************************************************************************************/
void SoundInformation::writeSampleIntoMemory(double sample,long num,short channel)
{
	long index;
	short numChan;
	
	numChan = this->getNumChannels();
	
	index = numChan*num + channel;

	if(index < getNumSamples())
		this->m_pdSample[index] = sample;
}


/*********************************************************************************************************
*&operator= : =演算子のオーバーロード
*
*引数
*	=演算子の右辺のSoundInformation
*
*ローカル変数
*	なし
*
*返り値
*	thisポインタ
*
*処理の流れ
*	左辺値と右辺値が同じものならば自分自信をを返す
*	左辺値の全サンプル数と右辺値の全サンプル数が異なっていれば,
*		右辺値の全サンプル数を設定，右辺値のチャンネル数を設定，右辺値の全サンプル数分だけメモリ確保
*	サンプル値をコピー
*	サンプリング周波数を設定
*	１サンプルのビット数を設定
*
*******************************************************************************************************/
const SoundInformation &SoundInformation::operator=(const SoundInformation &right)
{
	
	if(this == &right) return *this;
	
	if(right.getNumSamples() != getNumSamples()){
		delete [] this->m_pdSample;
		this->m_lSamplesPerChannel = right.m_lSamplesPerChannel;
		this->m_shNumChannels = right.m_shNumChannels;
		try
		{
			this->m_pdSample = new double [this->getNumSamples()];
		}
		catch(bad_alloc err)
		{
			cerr << "SoundInformation::operator=. " << err.what() << endl;
		}
	}
	memcpy(this->m_pdSample, right.m_pdSample, this->getNumSamples()*sizeof(double));
	
	this->m_lSampleRate = right.m_lSampleRate;
	this->m_shBitsPerSample = right.m_shBitsPerSample;

	return *this;
}


SoundInformation SoundInformation::operator+(const SoundInformation &ob)
{
	SoundInformation temp;
	short numChan;
	long numSamplesPerChan;
	
	if(getSampleRate() < ob.getSampleRate())
		temp.setSampleRate(getSampleRate());
	else
		temp.setSampleRate(ob.getSampleRate());

	if(getSamplesPerChannel() < ob.getSamplesPerChannel())
		temp.setSamplesPerChannel(getSamplesPerChannel());
	else
		temp.setSamplesPerChannel(ob.getSamplesPerChannel());
	
	if(getNumChannels() < ob.getNumChannels())
		temp.setNumChannels(getNumChannels());
	else
		temp.setNumChannels(ob.getNumChannels());

	if(getBitsPerSample() < ob.getBitsPerSample())
		temp.setBitsPerSample(getBitsPerSample());
	else
		temp.setBitsPerSample(ob.getBitsPerSample());
	
	numSamplesPerChan = temp.getSamplesPerChannel();
	numChan = temp.getNumChannels();
	for(long index=0;index<numSamplesPerChan;index++){
		for(short chan=0;chan<numChan;chan++){
			double data;
			
			data = readSampleFromMemory(index,chan) + ob.readSampleFromMemory(index,chan);
			temp.writeSampleIntoMemory(data,index,chan);
		}
	}
	
	return temp;
}


const SoundInformation &SoundInformation::operator+=(SoundInformation &ob)
{
	short numChan;
	long numSamplesPerChan;
	
	if(getSampleRate() != ob.getSampleRate()){
		if(getSampleRate() < ob.getSampleRate())
			ob.setSampleRate(getSampleRate());
		else
			setSampleRate(ob.getSampleRate());
	}
	
	if(getSamplesPerChannel() != ob.getSamplesPerChannel()){
		if(getSamplesPerChannel() < ob.getSamplesPerChannel())
			ob.setSamplesPerChannel(getSamplesPerChannel());
		else
			setSamplesPerChannel(ob.getSamplesPerChannel());
	}
	
	if(getNumChannels() != ob.getNumChannels()){
		if(getNumChannels() < ob.getNumChannels())
			ob.setNumChannels(getNumChannels());
		else
			setNumChannels(ob.getNumChannels());
	}
	
	if(getBitsPerSample() < ob.getBitsPerSample()){
		if(getBitsPerSample() < ob.getBitsPerSample())
			ob.setBitsPerSample(getBitsPerSample());
		else
			setBitsPerSample(ob.getBitsPerSample());
	}
	
	numSamplesPerChan = getSamplesPerChannel();
	numChan = getNumChannels();
	for(long index=0;index<numSamplesPerChan;index++){
		for(short chan=0;chan<numChan;chan++){
			double data;
			
			data = readSampleFromMemory(index,chan) + ob.readSampleFromMemory(index,chan);
			writeSampleIntoMemory(data,index,chan);
		}
	}

	return *this;
}


/*****************************************
*sinc : sinc関数の近似
*
*引数
*	x : 閾値
*
*ローカル変数
*	なし
*
*処理の流れ
*	xの絶対値をとる
*	sinc関数の近似値を計算
*
*返り値
*	sinc関数の値
*
*備考
*-2～2までの間しか考えていない
*その他のところは0と近似してしまう
*
******************************************/
double SoundInformation::sinc(double i_dSample)
{
    if(i_dSample < 0.0) i_dSample = -i_dSample;
    
    if(i_dSample < 1.0) return (1.0 - 2.0*i_dSample*i_dSample + i_dSample*i_dSample*i_dSample);
    if(i_dSample < 2.0) return (4.0 - 8.0*i_dSample + 5.0*i_dSample*i_dSample - i_dSample*i_dSample*i_dSample);
    
    return 0.0;
}

/********************************************
*interpolation : 補間関数
*
*引数
*	x : 閾値
*	channel : ターゲットのチャンネル
*
*ローカル変数
*	x0 : 閾値を整数化したもの（小数点以下切り捨て）
*	y : 補完した値
*
*返り値
*	補完した値
*
*備考
*	sinc関数を用いて近傍４点で補間
*	閾値の整数点からその間(xの値)を補完する
*
*********************************************/
double SoundInformation::interpolation(double i_dThreshold, short i_shChannel)
{
    long a_lThresholdInteger;
    
    a_lThresholdInteger = (long)i_dThreshold;

    double a_dInterpolation = 0.0;
    for(int a_iIndex=-1;a_iIndex<=2;a_iIndex++)
	{
        if( ((i_dThreshold+a_iIndex) >= 0) && ((i_dThreshold+a_iIndex) < this->getSamplesPerChannel()) )
		{
             a_dInterpolation+= this->readSampleFromMemory(a_lThresholdInteger+a_iIndex, i_shChannel)*sinc(i_dThreshold-(a_lThresholdInteger+a_iIndex));
		}
    }

    if(a_dInterpolation > 1.0) a_dInterpolation = 1.0;
    if(a_dInterpolation < -1.0) a_dInterpolation = -1.0;

    return a_dInterpolation;
}

