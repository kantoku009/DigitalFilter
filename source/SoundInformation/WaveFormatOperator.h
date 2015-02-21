/**
 * @file	WaveformtOperator.h
 * @brief	WAVE�t�@�C���������N���X.
 */

#ifndef __WAVE_FORMAT_OPERATOR_H__
#define __WAVE_FORMAT_OPERATOR_H__

#include <fstream>
using namespace std;

#include "SoundInformation.h"

typedef struct TChunk
{
    char id[4];
	char size[4];
}TChunk;

typedef struct TFmtChunk
{
//    short wFormatTag;
//    short nChannels;
//    long  nSamplesPerSec;
//    long  nAvgBytesPerSec;
//    short nBlockAlign;
//    short wBitsPerSample;
    char wFormatTag[2];
    char nChannels[2];
    char nSamplesPerSec[4];
    char nAvgBytesPerSec[4];
    char nBlockAlign[2];
    char wBitsPerSample[2];
}TFmtChunk;


/**
 * @brief	WAVE�t�@�C���������N���X.
 */
class WaveFormatOperator : public SoundInformation
{
public:
	/**
	 * @brief	�R���X�g���N�^.
	 */
    WaveFormatOperator(long  sampleRate=44100,
                       short bitsPerSample=8,
                       short numChannels=1,
                       long  numSamples=44100)
			:SoundInformation(
							sampleRate, 
							bitsPerSample, 
							numChannels, 
							numSamples)
	{
	}
							
	/**
	 * @brief	�f�X�g���N�^.
	 */
    virtual ~WaveFormatOperator(){ }

	/**
	 * @brief	WAVE�t�@�C������ǂ݂���.
	 * @param	const string i_strFilename
	 * @return	����/���s.
	 */
    bool readWaveFile(const string i_strFileName);

	/**
	 * @brief	WAVE�t�@�C���ɏ�������.
	 * @param	const string i_strFileName	�t�@�C����.
	 * @return	����/���s.
	 */
    bool writeWaveFile(const string i_strFileName);

private:
	/**
	 * @brief	WAVE�t�@�C������RIFF chunk��ǂ݂���.
	 * @param	ifstream i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool readRIFFChunk(ifstream& i_cFileStream);

	/**
	 * @brief	WAVE�t�@�C������WAVE chunk��ǂ݂���.
	 * @param	ifstream i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool readWAVEChunk(ifstream& i_cFileStream);

	/**
	 * @brief	WAVE�t�@�C������fmt chunk��ǂ݂���.
	 * @param	ifstream i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool readFmtChunk(ifstream& i_cFileStream, TChunk& i_stChunk);

	/**
	 * @brief	WAVE�t�@�C������Sample�f�[�^��ǂ݂���.
	 * @param	ifstream i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool readSample(ifstream& i_cFileStream, TChunk& i_stChunk);

	/**
	 * @brief	8bit�ŗʎq�����ꂽ�t�@�C����ǂ݂���.
	 * @param	ifstream& i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool readSample8FromFile(ifstream& i_cFileStream);

	/**
	 * @brief	16bit�ŗʎq�����ꂽ�t�@�C����ǂ݂���.
	 * @param	ifstream& i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool readSample16FromFile(ifstream& i_cFileStream);

	/**
	 * @brief	8bit,16bit�ȊO�ŗʎq�����ꂽ�t�@�C����ǂ݂���.
	 * @param	ifstream& i_cFileStream	�ǂݍ��݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool readSampleOtherFromFile(ifstream& i_cFileStream);

	/**
	 * @brief	RIFF chunk��WAVE�t�@�C���֏�������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool writeRIFFChunk(ofstream& i_cFileStream);

	/**
	 * @brief	WAVE chunk��WAVE�t�@�C���֏�������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool writeWAVEChunk(ofstream& i_cFileStream);

	/**
	 * @brief	fmt chunk��WAVE�t�@�C���֏�������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool writeFmtChunk(ofstream& i_cFileStream);

	/**
	 * @brief	Sample�f�[�^��WAVE�t�@�C���֏�������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
	bool writeSample(ofstream& i_cFileStream);

	/**
	 * @brief	8bit�ŗʎq�����ꂽ�t�@�C������������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool writeSample8IntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	16bit�ŗʎq�����ꂽ�t�@�C������������.
	 * @param	ofstream& i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool writeSample16IntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	8bit,16bit�ȊO�ŗʎq�����ꂽ�t�@�C������������.
	 * @param	ofstream&	i_cFileStream	�������݃t�@�C���X�g���[��.
	 * @return	����/���s.
	 */
    bool writeSampleOtherIntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	bit shift������.
	 * @param	short i_shBitShift
	 * @return	bit shift��������.
	 */
    long bitShift(short i_shNumShift);
    
	/**
	 * @brief	Big-endian���ۂ��𒲂ׂ�.
	 * @param	�Ȃ�.
	 * @return	true:Big-endian / false:Little-endian
	 */
    bool isBigEndian();
    
	/**
	 * @brief	long�^ Little-endian -> Big-endian ��ϊ�����.
	 * @param	�ϊ�����f�[�^.
	 * @return	�ϊ���������.
	 */
    long swapLong(char* i_pbyData);

	/**
	 * @brief	short�^ Little-endian -> Big-endian ��ϊ�����.
	 * @param	�ϊ�����f�[�^.
	 * @return	�ϊ���������.
	 */
    short swapShort(char* i_pbyData);

	/**
	 * @brief	int�^ Little-endian -> Big-endian ��ϊ�����.
	 * @param	�ϊ�����f�[�^.
	 * @return	�ϊ���������.
	 */
    int swapInt(char* i_pbyData);

	/**
	 * @brief	4ByteData��long�^�ɕϊ�����.
	 * @param	char* data
	 * @return	�ϊ��������l.
	 */
	long convert4ByteDataToLong(char* data);

	/**
	 * @brief	long�^��4ByteData�ɕϊ�����.
	 * @param	long i_lInteger
	 * @param	char* i_pbyData out����.
	 * @return	�Ȃ�.
	 */
	void convertLongTo4ByteData(long i_lInteger, char* i_pbyData);

	/**
	 * @brief	2ByteData��short�^�ɕϊ�����.
	 * @param	char* data
	 * @return	�ϊ��������l.
	 */
	short convert2ByteDataToShort(char* data);

	/**
	 * @brief	short�^��2ByteData�ɕϊ�����.
	 * @param	short i_shInteger
	 * @param	char* i_pbyData out����.
	 * @return	�Ȃ�.
	 */
	void convertShortTo2ByteData(short i_shInteger, char* i_pbyData);
	
	/**
	 * @brief	TChunk��\��.
	 * @param	char* i_pbyMessage	�\�����������b�Z�[�W.
	 * @param	TChunk i_stChunk	�\��������TChunk.
	 * @return	�Ȃ�.
	 * @note	�f�o�b�O�p.
	 */
	void printChunk(char* i_pbyMessage, TChunk& i_stChunk);

	/**
	 * @brief	TFmtChunk��\��.
	 * @param	char* i_pbyMessage		�\�����������b�Z�[�W.
	 * @param	TFmtChunk& i_stFmtChunk	�\��������TFmtChunk
	 * @return	�Ȃ�.
	 * @note	�f�o�b�O�p.
	 */
	void printFmtChunk(char* i_pbyMessage, TFmtChunk& i_stFmtChunk);

};

#endif

