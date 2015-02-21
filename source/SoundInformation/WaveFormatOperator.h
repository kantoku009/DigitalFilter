/**
 * @file	WaveformtOperator.h
 * @brief	WAVEファイルを扱うクラス.
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
 * @brief	WAVEファイルを扱うクラス.
 */
class WaveFormatOperator : public SoundInformation
{
public:
	/**
	 * @brief	コンストラクタ.
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
	 * @brief	デストラクタ.
	 */
    virtual ~WaveFormatOperator(){ }

	/**
	 * @brief	WAVEファイルから読みこみ.
	 * @param	const string i_strFilename
	 * @return	成功/失敗.
	 */
    bool readWaveFile(const string i_strFileName);

	/**
	 * @brief	WAVEファイルに書き込み.
	 * @param	const string i_strFileName	ファイル名.
	 * @return	成功/失敗.
	 */
    bool writeWaveFile(const string i_strFileName);

private:
	/**
	 * @brief	WAVEファイルからRIFF chunkを読みこみ.
	 * @param	ifstream i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool readRIFFChunk(ifstream& i_cFileStream);

	/**
	 * @brief	WAVEファイルからWAVE chunkを読みこみ.
	 * @param	ifstream i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool readWAVEChunk(ifstream& i_cFileStream);

	/**
	 * @brief	WAVEファイルからfmt chunkを読みこみ.
	 * @param	ifstream i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool readFmtChunk(ifstream& i_cFileStream, TChunk& i_stChunk);

	/**
	 * @brief	WAVEファイルからSampleデータを読みこみ.
	 * @param	ifstream i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool readSample(ifstream& i_cFileStream, TChunk& i_stChunk);

	/**
	 * @brief	8bitで量子化されたファイルを読みこむ.
	 * @param	ifstream& i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool readSample8FromFile(ifstream& i_cFileStream);

	/**
	 * @brief	16bitで量子化されたファイルを読みこむ.
	 * @param	ifstream& i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool readSample16FromFile(ifstream& i_cFileStream);

	/**
	 * @brief	8bit,16bit以外で量子化されたファイルを読みこむ.
	 * @param	ifstream& i_cFileStream	読み込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool readSampleOtherFromFile(ifstream& i_cFileStream);

	/**
	 * @brief	RIFF chunkをWAVEファイルへ書き込み.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool writeRIFFChunk(ofstream& i_cFileStream);

	/**
	 * @brief	WAVE chunkをWAVEファイルへ書き込み.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool writeWAVEChunk(ofstream& i_cFileStream);

	/**
	 * @brief	fmt chunkをWAVEファイルへ書き込み.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool writeFmtChunk(ofstream& i_cFileStream);

	/**
	 * @brief	SampleデータをWAVEファイルへ書き込み.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
	bool writeSample(ofstream& i_cFileStream);

	/**
	 * @brief	8bitで量子化されたファイルを書き込む.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool writeSample8IntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	16bitで量子化されたファイルを書き込む.
	 * @param	ofstream& i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool writeSample16IntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	8bit,16bit以外で量子化されたファイルを書き込む.
	 * @param	ofstream&	i_cFileStream	書き込みファイルストリーム.
	 * @return	成功/失敗.
	 */
    bool writeSampleOtherIntoFile(ofstream& i_cFileStream);

	/**
	 * @brief	bit shiftをする.
	 * @param	short i_shBitShift
	 * @return	bit shiftした結果.
	 */
    long bitShift(short i_shNumShift);
    
	/**
	 * @brief	Big-endianか否かを調べる.
	 * @param	なし.
	 * @return	true:Big-endian / false:Little-endian
	 */
    bool isBigEndian();
    
	/**
	 * @brief	long型 Little-endian -> Big-endian を変換する.
	 * @param	変換するデータ.
	 * @return	変換した結果.
	 */
    long swapLong(char* i_pbyData);

	/**
	 * @brief	short型 Little-endian -> Big-endian を変換する.
	 * @param	変換するデータ.
	 * @return	変換した結果.
	 */
    short swapShort(char* i_pbyData);

	/**
	 * @brief	int型 Little-endian -> Big-endian を変換する.
	 * @param	変換するデータ.
	 * @return	変換した結果.
	 */
    int swapInt(char* i_pbyData);

	/**
	 * @brief	4ByteDataをlong型に変換する.
	 * @param	char* data
	 * @return	変換した数値.
	 */
	long convert4ByteDataToLong(char* data);

	/**
	 * @brief	long型を4ByteDataに変換する.
	 * @param	long i_lInteger
	 * @param	char* i_pbyData out引数.
	 * @return	なし.
	 */
	void convertLongTo4ByteData(long i_lInteger, char* i_pbyData);

	/**
	 * @brief	2ByteDataをshort型に変換する.
	 * @param	char* data
	 * @return	変換した数値.
	 */
	short convert2ByteDataToShort(char* data);

	/**
	 * @brief	short型を2ByteDataに変換する.
	 * @param	short i_shInteger
	 * @param	char* i_pbyData out引数.
	 * @return	なし.
	 */
	void convertShortTo2ByteData(short i_shInteger, char* i_pbyData);
	
	/**
	 * @brief	TChunkを表示.
	 * @param	char* i_pbyMessage	表示したいメッセージ.
	 * @param	TChunk i_stChunk	表示したいTChunk.
	 * @return	なし.
	 * @note	デバッグ用.
	 */
	void printChunk(char* i_pbyMessage, TChunk& i_stChunk);

	/**
	 * @brief	TFmtChunkを表示.
	 * @param	char* i_pbyMessage		表示したいメッセージ.
	 * @param	TFmtChunk& i_stFmtChunk	表示したいTFmtChunk
	 * @return	なし.
	 * @note	デバッグ用.
	 */
	void printFmtChunk(char* i_pbyMessage, TFmtChunk& i_stFmtChunk);

};

#endif

