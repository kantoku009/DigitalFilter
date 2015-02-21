/**
 * @file	SoundInformation.h
 * @brief	�����������N���X.
 */

#ifndef __SOUND_INFORMATION_H__
#define __SOUND_INFORMATION_H__

class SoundInformation
{
public:
	/**
	 * @brief	�R���X�g���N�^.
	 * @param	long  sampleRate		�T���v�����O���g��. [sample/sec].
	 * @param	short bitsPerSample		�ʎq���r�b�g. [bit/sample].
	 * @param	short numChannels		�`�����l����.
	 * @param	long  numSamples		�T���v����. [sample/channel].
	 * @return	�Ȃ�.
	 */
	SoundInformation(long  sampleRate=44100,
					short bitsPerSample=8,
					short numChannels=1,
					long  numSamples=44100);
    
	/**
	 * @brief	�R�s�[�R���X�g���N�^.
	 * @param	const SoundInformation &ob
	 * @return	�Ȃ�.
	 */
	SoundInformation(const SoundInformation &ob);

	/**
	 * @brief	�f�X�g���N�^.
	 * @param	�Ȃ�.
	 * @return	�Ȃ�.
	 * @note	�R���X�g���N�^�Ŋm�ۂ����������̊m��.
	 */
	virtual ~SoundInformation()
	{
		delete [] this->m_pdSample;
		this->m_pdSample=0;
	}

	/**
	 * @brief	�T���v�����O���g����ݒ�.
	 * @param	long sampleRate	�T���v�����O���g��.
	 * @return	�Ȃ�.
	 */
	void setSampleRate(long sampleRate);

	/**
	 * @brief	�ʎq��bit��ݒ�.
	 * @param	short bitsPerSample	�ʎq���r�b�g.
	 * @return	�Ȃ�.
	 */
	void setBitsPerSample(short bitsPerSample);

	/**
	 * @brief	�`�����l������ݒ�.
	 * @param	short numChannels	�`�����l����.
	 * @return	�Ȃ�.
	 */
	void setNumChannels(short numChannels);

	/**
	 * @brief	1 channel�ɂ��A�����T���v���������邩��ݒ�
	 * @param	long samplesPerChannel 
	 * @return	�Ȃ�.
	 */
	void setSamplesPerChannel(long samplesPerChannel);

	/**
	 * @brief	�T���v�����O���g�����擾.
	 * @param	�Ȃ�.
	 * @return	�T���v�����O���g��.
	 */
	long	getSampleRate() const { return this->m_lSampleRate; }

	/**
	 * @brief	�ʎq��Bit���擾����
	 * @param	�Ȃ�.
	 * @return	�ʎq��Bit.
	 */
	short	getBitsPerSample() const { return this->m_shBitsPerSample; }

	/**
	 * @brief	�`�����l�������擾����.
	 * @param	�Ȃ�.
	 * @return	�`�����l����.
	 */
	short	getNumChannels() const { return this->m_shNumChannels; }

	/**
	 * @brief	1 channel�ɂ��A�����T���v���������邩���擾
	 * @param	�Ȃ�.
	 * @return	1�`�����l���̃T���v����.
	 */
	long	getSamplesPerChannel() const { return this->m_lSamplesPerChannel; }

	/**
	 * @brief	1�̃u���b�N�̋�؂�(byte)���擾.
	 * @param	�Ȃ�.
	 * @return	1�̃u���b�N�̋�؂�(byte).
	 */
	short	getBlockAlign() const { return this->m_shNumChannels*this->getBytesPerSample(); }

	/**
	 * @brief	1�T���v���ɂ��A���o�C�g�g�������擾
	 * @param	�Ȃ�.
	 * @return	1�T���v���ɂ��A���o�C�g�g����.
	 */
	short	getBytesPerSample() const;

	/**
	 * @brief	�S�T���v�������擾.
	 * @param	�Ȃ�.
	 * @return	�S�T���v����.
	 */
	long	getNumSamples() const { return this->m_lSamplesPerChannel*this->m_shNumChannels; }

	/**
	 * @brief	�������ɃT���v���l����������.
	 * @param	double sample
	 * @param	long num
	 * @param	short channel
	 * @return	�Ȃ�.
	 */
	void	writeSampleIntoMemory(double sample, long num, short channel=0);

	/**
	 * @brief	����������T���v���l��ǂ݂���.
	 * @param	long num
	 * @param	short channel
	 * @return	�T���v���l.
	 */
	double	readSampleFromMemory(long num, short channel=0) const;
    
	/**
	 * @brief	=���Z�q�̃I�[�o�[���[�h.
	 */
	const SoundInformation &operator=(const SoundInformation &right);

	/**
	 * @brief	+=���Z�q�̃I�[�o�[���[�h.
	 */
	const SoundInformation &operator+=(SoundInformation &ob);

	/**
	 * @brief	+���Z�q�̃I�[�o�[���[�h.
	 */
	SoundInformation operator+(const SoundInformation &ob);
    
	/**
	 * @brief	�T���v���ƃT���v���̊Ԃ̕��.
	 */
	double	interpolation(double i_dThreshold, short i_shChannel);

protected:
	/**
	 * @brief	sinc�֐�.
	 */
	double sinc(double i_dSample);

	/**
	 * @biref	�T���v�����O���g��. [sample/sec].
	 */
	long    m_lSampleRate;

	/**
	 * @brief	�ʎq���r�b�g. [bit/sample].
	 */
	short   m_shBitsPerSample;

	/**
	 * @brief	�`�����l���̐�.
	 */
	short   m_shNumChannels;

	/**
	 * @brief	�P�`�����l���̃T���v���̐�. [sample/channel].
	 */
	long    m_lSamplesPerChannel;

	/**
	 * @brief	�T���v���l.
	 */
	double  *m_pdSample;
};

#endif

