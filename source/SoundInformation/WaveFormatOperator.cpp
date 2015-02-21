
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;

#include "WaveFormatOperator.h"


// デバッグ用フラグ.
static bool DEBUG=false;

class WaveFormatError
{
public:
    WaveFormatError(char *message)
    {
        int len;
        
        len = strlen(message);
        mMessage = new char [len+1];
        strcpy(mMessage,message);
    }
    const char* what() const { return mMessage; }
private:
    char* mMessage;
};



/**************************
 * WAVEファイルから読みこみ.
 **************************/
bool WaveFormatOperator::readWaveFile(const string i_strFileName)
{
    ifstream   fp(i_strFileName.c_str(),ios::binary);
    TChunk     chunk;
    
    if(!fp){
        cerr << "Can't Open " << i_strFileName << ".\n";
        return false;
    }
    
    try{
		//'RIFF'
		this->readRIFFChunk(fp);

		//'WAVE'
		this->readWAVEChunk(fp);

		while( fp.read((char*)&chunk,sizeof(chunk)) ){
	        if(strncmp(chunk.id,"fmt ",4) == 0)
			{
				// 'fmt ' read.
				this->readFmtChunk(fp, chunk);
	        }
			else if(strncmp(chunk.id,"data",4) == 0)
			{
				// 'data' read.
				this->readSample(fp, chunk);
	        }
			else
			{
				long lSize = this->convert4ByteDataToLong(chunk.size);
	            fp.seekg(lSize,ios::cur);
				if(DEBUG) this->printChunk((char*)"Read Chunk", chunk);
	        }
	    }
	}catch(WaveFormatError err){
        cerr << err.what() << endl;
        return false;
    }
   
    return true;
}

/**************************************
 * WAVEファイルからRIFF chunkを読みこみ.
 **************************************/
bool WaveFormatOperator::readRIFFChunk(ifstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));

	i_cFileStream.read((char*)&chunk,sizeof(chunk));
	if(strncmp(chunk.id,"RIFF",4) != 0)
	{
		throw WaveFormatError((char*)"WaveFormatOperator error: not RIFF file");
	}

	if(DEBUG) printChunk((char*)"Read Chunk", chunk);
	return true;
}

/**************************************
 * WAVEファイルからWAVE chunkを読みこみ.
 **************************************/
bool WaveFormatOperator::readWAVEChunk(ifstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));

	i_cFileStream.read((char*)&chunk.id,sizeof(chunk.id));
	if(strncmp(chunk.id,"WAVE",4) != 0)
	{
		throw WaveFormatError((char*)"WaveFormatOperator error: not WAVE file");
	}

	if(DEBUG) printChunk((char*)"Read Chunk", chunk);
	return true;
}

/**************************************
 * WAVEファイルからfmt chunkを読みこみ.
 **************************************/
bool WaveFormatOperator::readFmtChunk(ifstream& i_cFileStream, TChunk& i_stChunk)
{
	long a_lChunkSize = this->convert4ByteDataToLong(i_stChunk.size);

	TFmtChunk fmt;
	memset((char*)&fmt, 0x00, sizeof(fmt));
	i_cFileStream.read((char*)&fmt, a_lChunkSize);

	long nSamplesPerSec = this->convert4ByteDataToLong(fmt.nSamplesPerSec);
	short wBitsPerSample = this->convert2ByteDataToShort(fmt.wBitsPerSample);
	short nChannels = this->convert2ByteDataToShort(fmt.nChannels);

	this->setSampleRate(nSamplesPerSec);
	this->setBitsPerSample(wBitsPerSample);
	this->setNumChannels(nChannels);

	if(DEBUG) printChunk((char*)"Read Chunk", i_stChunk);
	if(DEBUG) printFmtChunk((char*)"Read fmt", fmt);
 	return true;
}

/******************************************
 * WAVEファイルからSampleデータを読みこみ.
 ******************************************/
bool WaveFormatOperator::readSample(ifstream& i_cFileStream, TChunk& i_stChunk)
{
	long lDataSize = this->convert4ByteDataToLong(i_stChunk.size);
	this->setSamplesPerChannel(lDataSize/this->getBlockAlign());

	//sample read
	bool isReadSuccess = false;
	if(this->getBitsPerSample() <= 8)
	{
		isReadSuccess = this->readSample8FromFile(i_cFileStream);
	}
	else if(this->getBitsPerSample() > 8 && this->getBitsPerSample() <= 16)
	{
		isReadSuccess = this->readSample16FromFile(i_cFileStream);
	}
	else
	{
		isReadSuccess = this->readSampleOtherFromFile(i_cFileStream);
	}

	if(!isReadSuccess)
	{
		throw WaveFormatError((char*)"WaveFormatOperator error: read file");
		return false;
	}

	if(DEBUG) printChunk((char*)"Read Chunk", i_stChunk);
	return true;
}

/****************************************
 * 8bitで量子化されたファイルを読みこむ.
 ****************************************/
bool WaveFormatOperator::readSample8FromFile(ifstream& i_cFileStream)
{
    long max = this->bitShift(this->getBitsPerSample() - 1) - 1;

    //sample read
    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
			char data = 0;
            i_cFileStream.read((char*)&data,sizeof(char));
            data ^= 0x80;
            this->writeSampleIntoMemory((double)data/max, index, channel);
        }
    }

    return true;
}

/****************************************
 * 16bitで量子化されたファイルを読みこむ.
 ****************************************/
bool WaveFormatOperator::readSample16FromFile(ifstream& i_cFileStream)
{
    long max = this->bitShift(this->getBitsPerSample() - 1) - 1;
    bool bigEndian = this->isBigEndian();

    //sample read
    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
			short data = 0;
            i_cFileStream.read((char*)&data, sizeof(short));
            if(bigEndian) this->swapShort((char*)&data);
            this->writeSampleIntoMemory((double)data/max, index, channel);
        }
    }

    return true;
}

/**************************************************
 * 8bit,16bit以外で量子化されたファイルを読みこむ.
 **************************************************/
bool WaveFormatOperator::readSampleOtherFromFile(ifstream& i_cFileStream)
{   
    long max = this->bitShift(getBitsPerSample() - 1) - 1;
    short bytesPerSample = this->getBytesPerSample();
    bool bigEndian = this->isBigEndian();
    
    //sample read
    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
            long data = 0;
            i_cFileStream.read((char*)&data,(long)bytesPerSample);
            if(bigEndian) this->swapLong((char*)&data);
            this->writeSampleIntoMemory((double)data/max, index, channel);
        }
    }

    return true;
}

/****************************************
 * WAVEファイルに書き込み.
 ****************************************/
bool WaveFormatOperator::writeWaveFile(const string i_strFileName)
{
    ofstream      fp(i_strFileName.c_str(),ios::binary);
    if(!fp){
		cerr << "WaveFormatOperator error: do not open" << i_strFileName << endl;
        return false;
    }
    
	bool a_bIsWriteSuccess = false;

	try
	{
	    //'RIFF' write
		a_bIsWriteSuccess = this->writeRIFFChunk(fp);
	   
	    //'WAVE' write
		a_bIsWriteSuccess = this->writeWAVEChunk(fp);
	    
	    //'fmt ' write
		a_bIsWriteSuccess = this->writeFmtChunk(fp);
		
	    //'data' write
		a_bIsWriteSuccess = this->writeSample(fp);
	}
	catch(WaveFormatError err)
	{
        cerr << err.what() << endl;
        return false;
    }
 
    return true;
}

/***************************************************
 * RIFF chunkをWAVEファイルへ書き込み.
 ***************************************************/
bool WaveFormatOperator::writeRIFFChunk(ofstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));

	long dataSize = this->getSamplesPerChannel() * this->getBlockAlign();

    strncpy(chunk.id,"RIFF",4);
	long lRIFFSize = 0;
    lRIFFSize = 4						//'WAVE'
                + sizeof(TChunk)		//'fmt '
                + sizeof(TFmtChunk)		//FMT Chunk
                + sizeof(TChunk)		//'data'
                + dataSize;				//全サンプルのサイズ.
	this->convertLongTo4ByteData(lRIFFSize, chunk.size);

	// chunkをファイルへ書き込み.
    i_cFileStream.write((char*)&chunk,sizeof(chunk));

	if(DEBUG) printChunk((char*)"Write Chunk", chunk);
 	return true;
}

/************************************
 * WAVE chunkをWAVEファイルへ書き込み.
 ************************************/
bool WaveFormatOperator::writeWAVEChunk(ofstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));

	strncpy(chunk.id, "WAVE", 4);
    i_cFileStream.write(chunk.id, 4);

	if(DEBUG) printChunk((char*)"Write Chunk", chunk);
	return true;
}

/***********************************************
* fmt chunkをWAVEファイルへ書き込み.
************************************************/
bool WaveFormatOperator::writeFmtChunk(ofstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));
	TFmtChunk fmt;
	memset((char*)&fmt, 0x00, sizeof(fmt));

    strncpy(chunk.id,"fmt ",4);
	long lFmtSize = sizeof(fmt);
	this->convertLongTo4ByteData(lFmtSize, chunk.size);
	// chunkをファイルへ書き込み.
    i_cFileStream.write((char*)&chunk,sizeof(chunk));

    short wFormatTag = 1;
	this->convertShortTo2ByteData(wFormatTag, fmt.wFormatTag);

	short nChannels = this->getNumChannels();
	this->convertShortTo2ByteData(nChannels, fmt.nChannels);

	long nSamplesPerSec = this->getSampleRate();
	this->convertLongTo4ByteData(nSamplesPerSec, fmt.nSamplesPerSec);

    long nAvgBytesPerSec = this->getBlockAlign() * this->getSampleRate();
	this->convertLongTo4ByteData(nAvgBytesPerSec, fmt.nAvgBytesPerSec);

    short nBlockAlign = this->getBlockAlign();
	this->convertShortTo2ByteData(nBlockAlign, fmt.nBlockAlign);

    short wBitsPerSample = this->getBitsPerSample();
	this->convertShortTo2ByteData(wBitsPerSample, fmt.wBitsPerSample);

    // fmt chunkをファイルへ書き込み.
    i_cFileStream.write((char*)&fmt,sizeof(fmt));

	if(DEBUG) printChunk((char*)"Write Chunk", chunk);
	if(DEBUG) printFmtChunk((char*)"Write FMT Chunk", fmt);
	return true;
}

/****************************************
* SampleデータをWAVEファイルへ書き込み.
*****************************************/
bool WaveFormatOperator::writeSample(ofstream& i_cFileStream)
{
	TChunk chunk;
	memset((char*)&chunk, 0x00, sizeof(chunk));

    strncpy(chunk.id,"data",4);
    long lDataSize = this->getSamplesPerChannel() * this->getBlockAlign();
	this->convertLongTo4ByteData(lDataSize, chunk.size);
	// chunkをファイルへ書き込み.
    i_cFileStream.write((char*)&chunk,sizeof(chunk));
    
    //sample write
	bool a_bIsWriteSuccess;
	if(this->getBitsPerSample() <= 8)
	{
		// 8bitで量子化されたファイルを書き込む.
		a_bIsWriteSuccess = this->writeSample8IntoFile(i_cFileStream);
	}
	else if(this->getBitsPerSample() > 8 && this->getBitsPerSample() <= 16)
	{
		// 16bitで量子化されたファイルを書き込む.
		a_bIsWriteSuccess = this->writeSample16IntoFile(i_cFileStream);
	}
	else
	{
		// 8bit,16bit以外で量子化されたファイルを書き込む.
		a_bIsWriteSuccess = this->writeSampleOtherIntoFile(i_cFileStream);
	}

	if(!a_bIsWriteSuccess)
	{
		throw WaveFormatError((char*)"WaveFormatOperator error: write file");
		return false;
	}

	if(DEBUG) printChunk((char*)"Write Sample Chunk", chunk);
	return true;
}

/**************************************************
 * 8bitで量子化されたファイルを書き込む.
 **************************************************/
bool WaveFormatOperator::writeSample8IntoFile(ofstream& i_cFileStream)
{
    long max = this->bitShift(this->getBitsPerSample() - 1) - 1;

    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
			char data = 0;
            data = max * readSampleFromMemory(index,channel);
            data ^= 0x80;
            i_cFileStream.write((char*)&data, sizeof(char));
        }
    }

    return true;
}


/**************************************************
 * 16bitで量子化されたファイルを書き込む.
 **************************************************/
bool WaveFormatOperator::writeSample16IntoFile(ofstream& i_cFileStream)
{
    bool bigEndian = this->isBigEndian();
    long max = this->bitShift(this->getBitsPerSample() - 1) - 1;

    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
			short data = 0;
            data = max * this->readSampleFromMemory(index,channel);
            if(bigEndian) data = this->swapShort((char*)&data);
            i_cFileStream.write((char*)&data, sizeof(short));
        }
    }

    return true;
}

/**************************************************
 * 8bit,16bit以外で量子化されたファイルを書き込む.
 **************************************************/
bool WaveFormatOperator::writeSampleOtherIntoFile(ofstream& i_cFileStream)
{
    bool bigEndian = this->isBigEndian();
    long max = this->bitShift(this->getBitsPerSample() - 1) - 1;
    short bytesPerSample = this->getBytesPerSample();

    //sample write
    for(long index=0;index<this->getSamplesPerChannel();index++)
	{
        for(short channel=0;channel<this->getNumChannels();channel++)
		{
            long data = 0;
            data = max * this->readSampleFromMemory(index,channel);
            if(bigEndian) data = this->swapLong((char*)&data);
            i_cFileStream.write((char*)&data, bytesPerSample);
        }
    }

    return true;
}


/**************************************************
 * bit shiftをする.
 **************************************************/
long WaveFormatOperator::bitShift(short i_shBitShift)
{
    long a_lRet=1;
    
    a_lRet <<= i_shBitShift;
    
    return a_lRet;
}


/**************************************************
 * Big-endianか否かを調べる.
 **************************************************/
bool WaveFormatOperator::isBigEndian()
{
    short	a_shCheck = 1;
	bool	a_bIsBigEndian = false;
    
    if(*((char*)&a_shCheck))
	{
		a_bIsBigEndian = false;
	}
    else
	{
		a_bIsBigEndian = true;
	}

	return a_bIsBigEndian;
}

/**************************************************
 * long型 Little-endian -> Big-endian を変換する.
 **************************************************/
long WaveFormatOperator::swapLong(char* i_pbyData)
{
    return (*i_pbyData<<24) | (*(i_pbyData+1)<<16) | (*(i_pbyData+2)<<8) | *(i_pbyData+3);
}

/**************************************************
 * short型 Little-endian -> Big-endian を変換する.
 **************************************************/
short WaveFormatOperator::swapShort(char* i_pbyData)
{
    return (*i_pbyData<<8) | *(i_pbyData+1);
}


/**************************************************
 * int型 Little-endian -> Big-endian を変換する.
 **************************************************/
int WaveFormatOperator::swapInt(char* i_pbyData)
{
    switch (sizeof(int)){
        case 2:
        return (int)this->swapShort(i_pbyData);
        
        case 4:
        return (int)this->swapLong(i_pbyData);
    }
    
    return 0;
}

/*********************************
 * 2ByteDataをshort型に変換する.
 *********************************/
short WaveFormatOperator::convert2ByteDataToShort(char* i_pbyData)
{
	short a_shSize = 0;

	memcpy((char*)&a_shSize, i_pbyData, 2);
	if(this->isBigEndian()) a_shSize = this->swapShort((char*)&a_shSize);
	return a_shSize;
}

/*********************************
 * short型を2ByteDataに変換する.
 *********************************/
void WaveFormatOperator::convertShortTo2ByteData(short i_shInteger, char* i_pbyData)
{
	bool a_bIsBigEndian = this->isBigEndian();
	if(a_bIsBigEndian) i_shInteger = this->swapShort((char*)&i_shInteger);

	memcpy(i_pbyData, (char*)&i_shInteger, 2);
}

/*********************************
 * 4ByteDataをlong型に変換する.
 *********************************/
long WaveFormatOperator::convert4ByteDataToLong(char* i_pbyData)
{
	long a_lSize = 0;

	memcpy((char*)&a_lSize, i_pbyData, 4);
	if(this->isBigEndian()) a_lSize = this->swapLong((char*)&a_lSize);
	return a_lSize;
}

/**********************************
 * long型を4ByteDataに変換する.
 **********************************/
void WaveFormatOperator::convertLongTo4ByteData(long i_lInteger, char* i_pbyData)
{
	bool a_bIsBigEndian = this->isBigEndian();
	if(a_bIsBigEndian) i_lInteger = this->swapShort((char*)&i_lInteger);

	memcpy(i_pbyData, (char*)&i_lInteger, 4);
}

/****************************************
 * TChunkを表示.
 * デバッグ用.
 ****************************************/
void WaveFormatOperator::printChunk(char* i_pbyMessage, TChunk& i_stChunk)
{
	printf("%s\n", i_pbyMessage);
	printf("  %c%c%c%c\n", i_stChunk.id[0], i_stChunk.id[1], i_stChunk.id[2], i_stChunk.id[3]);
	long lSize = this->convert4ByteDataToLong(i_stChunk.size);
	printf("  %lX\n", lSize);
}

/**********************************
 * TFmtChunkを表示.
 * デバッグ用.
 **********************************/
void WaveFormatOperator::printFmtChunk(char* i_pbyMessage, TFmtChunk& i_stFmtChunk)
{
    short wFormatTag = this->convert2ByteDataToShort(i_stFmtChunk.wFormatTag);
    short nChannels = this->convert2ByteDataToShort(i_stFmtChunk.nChannels);
    long  nSamplesPerSec = this->convert4ByteDataToLong(i_stFmtChunk.nSamplesPerSec);
    long  nAvgBytesPerSec = this->convert4ByteDataToLong(i_stFmtChunk.nAvgBytesPerSec);
    short nBlockAlign = this->convert2ByteDataToShort(i_stFmtChunk.nBlockAlign);
    short wBitsPerSample = this->convert2ByteDataToShort(i_stFmtChunk.wBitsPerSample);

	printf("%s\n", i_pbyMessage);
	printf("  wFormatTag = %X\n", wFormatTag);
	printf("  nChannels = %X\n", nChannels);
	printf("  nSamplesPerSec = %lX\n", nSamplesPerSec);
	printf("  nAvgBytesPerSec = %lX\n", nAvgBytesPerSec);
	printf("  nBlockAlign = %X\n", nBlockAlign);
	printf("  wBitsPerSample = %X\n", wBitsPerSample);
}


