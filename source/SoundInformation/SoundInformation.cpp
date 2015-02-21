#include "SoundInformation.h"
#include <new>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

/*******************************************************************
*SoundInformation�̃R���X�g���N�^
*
*����
*	sampleRate:�T���v�����O���g��
*	bitsPerSample:1sample�ɂ���bit��
*	numChannels:�`�����l����
*	samplesPerChannel:1channel�ɂ��̃T���v����
*
*���[�J���ϐ�
*	�Ȃ�
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	�����o�ϐ��̏�����
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
*�R�s�[�R���X�g���N�^�[
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
*setSampleRate : �T���v�����O���g���̐ݒ�
*
*����
*	sampleRate : �ݒ��̃T���v�����O���g��
*
*���[�J���ϐ�
*	data : �T���v���l�̒l
*	delta : ���݂̃T���v�����O���g���Ɛݒ肵�����T���v�����O���g���̔�(�ăT���v�����O���ɕK�v)
*	samplePerChannel : �T���v�����O���g����ݒ肵�����1channel�ɂ��̃T���v����
*	temp : ���݂�SoundInformation��ۑ����Ă����ϐ�
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	���݂̃T���v�����O���g���Ɛݒ��̃T���v�����O���g���������l�Ȃ�Ή���������return
*	temp�Ɍ��݂�SoundInformation��ۑ�
*	���݂̃T���v�����O���g���Ɛݒ��̃T���v�����O���g���̔�����߁A�ݒ���samplePerCahnnel��ݒ�
*	�ăT���v�����O�̏���
*	�T���v�����O���g���̐ݒ�
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
*setBitsPerSample : �ʎq���r�b�g�̐ݒ�
*
*����
*	bitsPerSample : �ݒ��̗ʎq���r�b�g
*
*
*���[�J���ϐ�
*	�Ȃ�
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	�ݒ�O�̗ʎq���r�b�g�Ɛݒ��̗ʎq���r�b�g�������Ȃ�΂�����return
*	�ʎq���r�b�g��ݒ�
*
**************************************************************************/
void SoundInformation::setBitsPerSample(short bitsPerSample)
{
	
	if(bitsPerSample == this->getBitsPerSample()) return;
	this->m_shBitsPerSample = bitsPerSample;
}


/***************************************************************************
*setNumChannels : �`�����l������ݒ�
*
*����
*	numChannels : �ݒ��̃`�����l����
*
*���[�J���ϐ�
*	numSamples : �S�T���v����
*	data : �T���v���l��ۑ�����
*	lessChannel : �ݒ�O�Ɛݒ��̂ǂ��炩�̏��Ȃ��ق��̃`�����l����
*	temp : �ݒ�O��SoundInformation
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	temp�Ɍ��݂̏�Ԃ�SoundInformation���R�s�[
*	�ݒ��̃`�����l�����ƑS�T���v������ݒ�
*	data��ێ����郁�������m��
*	�ݒ�O�Ɛݒ��Ń`�����l�����̏��Ȃ��ق����L�^
*	�T���v���l�̃R�s�[
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
*setSamplePerChannel : �P�`�����l���̃T���v������ݒ�
*
*����
*	samplesPerChannel : �ݒ��̂P�`�����l���̃T���v����
*
*���[�J���ϐ�
*	temp : �ݒ�O��SoundInformation
*	oldNumSamples : �ݒ�O�̑S�T���v����
*	newNumSamples : �ݒ��̑S�T���v����
*	lessNumSamples : �ݒ�O�Ɛݒ��̏��Ȃ����̑S�T���v����
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	temp�Ɍ��݂̏�Ԃ�SoundInformation���R�s�[
*	�ݒ�O�̑S�T���v�����Ɛݒ��̑S�T���v�������L��
*	�ݒ�O�Ɛݒ��ł̑S�T���v�����̏��Ȃ��ق����L��
*	�S�T���v�����A���������������m��
*	�m�ۂ����������ɃT���v���l���R�s�[
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
*getBytesPerSample : �P�T���v���̃o�C�g�����擾
*
*����
*	�Ȃ�
*
*���[�J���ϐ�
*	bytesPerSample : �P�T���v���̃o�C�g��
*
*�Ԃ�l
*	1�T���v���̃o�C�g��
*
*�����̗���
*	1�T���v���̃r�b�g����8�Ŋ���o�C�g�����Z�o��,
*	1�T���v���̃r�b�g����8�̔{���łȂ���΁CbytesPerSampe��+1����
*	bytesPerSample��4byte��菬������΁AbytesPerSample�͂��̂܂܂ŁC
*	�傫�����bytesPerSample��4�ɐݒ�
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
*readSampleFromMemory : ����������T���v���l���擾
*
*����
*	num : �擾����T���v�����ŏ����牽�Ԗڂ̂��̂����w��
*	channel : �擾����T���v���̃`�����l�����w��
*
*���[�J���ϐ�
*	index : 1�u���b�N�̒��̉��Ԗڂ����w��
*
*�Ԃ�l
*	�T���v���l
*
*�����̗���
*	index���S�T���v������菬������΃T���v���l��Ԃ�
*	index���S�T���v�������傫�����0��Ԃ�
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
*writeSampleIntoMemory : �������ɃT���v���l����������
*
*����
*	sample : �������ރT���v���l
*	num : �������ރT���v�����ŏ����牽�Ԗڂ̂��̂����w��
*	channel : �������ރT���v���̃`�����l�����w��
*
*���[�J���ϐ�
*	index : 1�u���b�N�̒��̉��Ԗڂ����w��
*
*�Ԃ�l
*	�Ȃ�
*
*�����̗���
*	index���S�T���v������菬������΃T���v���l���������ށC
*	index���S�T���v�������傫����Ώ������܂Ȃ�
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
*&operator= : =���Z�q�̃I�[�o�[���[�h
*
*����
*	=���Z�q�̉E�ӂ�SoundInformation
*
*���[�J���ϐ�
*	�Ȃ�
*
*�Ԃ�l
*	this�|�C���^
*
*�����̗���
*	���Ӓl�ƉE�Ӓl���������̂Ȃ�Ύ������M����Ԃ�
*	���Ӓl�̑S�T���v�����ƉE�Ӓl�̑S�T���v�������قȂ��Ă����,
*		�E�Ӓl�̑S�T���v������ݒ�C�E�Ӓl�̃`�����l������ݒ�C�E�Ӓl�̑S�T���v�����������������m��
*	�T���v���l���R�s�[
*	�T���v�����O���g����ݒ�
*	�P�T���v���̃r�b�g����ݒ�
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
*sinc : sinc�֐��̋ߎ�
*
*����
*	x : 臒l
*
*���[�J���ϐ�
*	�Ȃ�
*
*�����̗���
*	x�̐�Βl���Ƃ�
*	sinc�֐��̋ߎ��l���v�Z
*
*�Ԃ�l
*	sinc�֐��̒l
*
*���l
*-2�`2�܂ł̊Ԃ����l���Ă��Ȃ�
*���̑��̂Ƃ����0�Ƌߎ����Ă��܂�
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
*interpolation : ��Ԋ֐�
*
*����
*	x : 臒l
*	channel : �^�[�Q�b�g�̃`�����l��
*
*���[�J���ϐ�
*	x0 : 臒l�𐮐����������́i�����_�ȉ��؂�̂āj
*	y : �⊮�����l
*
*�Ԃ�l
*	�⊮�����l
*
*���l
*	sinc�֐���p���ċߖT�S�_�ŕ��
*	臒l�̐����_���炻�̊�(x�̒l)��⊮����
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

