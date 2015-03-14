/**
 * @file	BIIRFilter.h
 * @brief	IIRフィルタ共通処理を集めたクラス
 */

#ifndef __CIIR_FILTER_H__
#define __CIIR_FILTER_H__

#include <cstdio>		//デバッグ用. printf()を使用したい.
#include <fstream>
#include <cmath>
#include <complex>		//振幅特性と位相特性を計算するのに複素数を使用する.
#include <cstdlib>
#include <string>
#include <map>
using namespace std;

#include "../CBlockDiagram.h"

/**
 * @brief	IIRフィルタ共通処理を集めたクラス.
 */
class BIIRFilter
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	BIIRFilter()
	{
		this->m_pcBlockDiagram = 0;
		this->m_dSampleRate = 0.0;
		this->m_dCutoffFreq = 0.0;
		this->m_dLowCutoffFreq = 0.0;
		this->m_dHighCutoffFreq = 0.0;
		this->m_dPassFreq = 0.0;
		this->m_dRippleGain = 0.0;
		this->m_dStopFreq = 0.0;
		this->m_dAttenuateGain = 0.0;
		this->m_dPrototypeCutFreq = 0.0;
		this->m_dPrototypePassFreq = 0.0;
		this->m_lOrderNumber = 0;
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~BIIRFilter()
	{
		if(this->m_pcBlockDiagram !=0)
		{
			delete [] this->m_pcBlockDiagram;
		}
	}

	/**
	 * @brief	サンプリング周波数を設定.
	 * @param	double i_dSampleRate	サンプリング周波数.
	 * @return	なし.
	 */
	virtual void setSampleRate(double i_dSampleRate)
	{
		m_dSampleRate = i_dSampleRate;
	}

	/**
	 * @brief	サンプリング周波数を取得.
	 * @param	なし.
	 * @return	サンプリング周波数.
	 */
	virtual double getSampleRate() const
	{
		return this->m_dSampleRate;
	}

	/**
	 * @brief フィルタの次数を取得.
	 * @param なし.
	 * @return フィルタの次数.
	 */
	long getOrderNumber() const
	{
		return this->m_lOrderNumber;
	}

protected:

	/**
	 * @brief	コンフィグを設定.
	 */
	virtual void setConfig(map<string,string> i_mapPairsTable, map<string,double*> i_mapConfigTable)
	{
		map<string,double*>::iterator a_piIterator;
		for(a_piIterator=i_mapConfigTable.begin(); a_piIterator!=i_mapConfigTable.end(); ++a_piIterator)
		{
			string a_strKey = a_piIterator->first;
			string a_strValue = i_mapPairsTable[a_strKey];
			*(a_piIterator->second) = atof(a_strValue.c_str());
		}
	}


	/**
	 * @brief	伝達関数にサンプルを通す.
	 * @param	long i_lNumSection	ブロックダイアグラムのセクション数.
	 * @param	double i_dSample	入力サンプル値.
	 * @return	出力サンプル値.
	 */
	virtual double transferFunction(long i_lNumSection, double i_dSample) const
	{
		double a_dData = i_dSample;
		for(long a_lIndex=0;a_lIndex<i_lNumSection;a_lIndex++)
		{
			a_dData = this->m_pcBlockDiagram[a_lIndex].inject(a_dData);
		}

		return a_dData;
	}

	/**
	 * @brief フィルタの次数を設定.
	 * @param long inOrder フィルタの次数.
	 * @return なし.
	 */
	void setOrderNumber(long i_lOrder)
	{
		this->m_lOrderNumber = i_lOrder;
	}

	/**
	 * @brief プロトタイプローパスフィルタを決定.
	 * @param double inPassFreq パスバンドの周波数[Hz]
	 * @param double inRippleGain 偏差の量[dB]
	 * @param double inStopFreq ストップバンドの周波数[Hz]
	 * @param double inAttenuateGain 減衰量[dB]
	 * @return なし.
	 */
	virtual void decisionPrototype(
						double i_dPassFreq,
						double i_dRippleGain,
						double i_dStopFreq,
						double i_dAttenuateGain){ }
    
	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数を取得.
	 * @param	なし.
	 * @return	プロトタイプローパスフィルタのカットオフ周波数.
	 */
	virtual double getPrototypeCutFreq() const
	{
		return this->m_dPrototypeCutFreq;
	}

	/**
	 * @brief	プロトタイプローパスフィルタのパス周波数を取得.
	 */
	virtual double getPrototypePassFreq() const
	{
		return this->m_dPrototypePassFreq;
	}

	/**
	 * @brief デジタルの周波数からアナログの周波数へ変換.
	 * @param double i_dOmega デジタルの周波数.
	 * @return アナログの周波数.
	 */
	virtual double digital2analog(double i_dOmega) const
	{
		return 2*tan(i_dOmega/2);
	}

	/**
	 * @brief	フィルタの設定値を出力.
	 * @note	デバッグ用.
	 */
	void printConfig() const
	{
		printf("  SampleRate = %.1f\n", this->m_dSampleRate);
		printf("  PassFreq = %.1f\n", this->m_dPassFreq);
		printf("  RippleGain = %.1f\n", this->m_dRippleGain);
		printf("  StopFreq = %.1f\n", this->m_dStopFreq);
		printf("  AttenuateGain = %.1f\n", this->m_dAttenuateGain);
		printf("  CutoffFreq = %.1f\n", this->m_dCutoffFreq);
		printf("  LowCutoffFreq = %.1f\n", this->m_dLowCutoffFreq);
		printf("  HighCutoffFreq = %.1f\n", this->m_dHighCutoffFreq);
	}

	/**
	 * @brief	振幅特性を取得.
	 * @param	complex<double> i_cComplexValue
	 * @return	振幅特性.
	 */
	double getAmplitude(complex<double> i_cComplexValue) const
	{
		return 20.0*log( abs(i_cComplexValue) );
	}

	/**
	 * @brief	位相特性を取得.
	 * @param	complex<double> i_cComplexValue
	 * @return	位相特性.
	 */
	double getPhase(complex<double> i_cComplexValue) const
	{
		return arg(i_cComplexValue);
	}

	/**
	 * @brief	フィルタ特性を出力する.
	 * @param	const char* i_pbyAmplitudeFilename	振幅特性を出力するファイル名.
	 * @param	const char* i_pbyPhaseFilename		位相特性を出力するファイル名.
	 * @param	const long i_lNumSection			伝達関数を１次または２次に分けたときのセクションの個数
	 * @return	なし.
	 * @note	ローカル変数.
	 *			a : 伝達関数の1次または２次のセクションの係数
	 *			b : 伝達関数の1次または２次のセクションの係数
	 *			omega : 振幅特性、位相特性の角周波数[rad/sec]
	 *			e1 : 絶対値が1で，位相角度-omegaの複素数
	 *			e2 : 絶対値が1で，位相角度-2*omegaの複素数
	 *			h : インパルス応答
	 */
	virtual void printCharacteristic(
							const char* i_pbyAmplitudeFilename, 
							const char* i_pbyPhaseFilename,
							const long i_lNumSection) const
	{
		//振幅特性を書き込むファイルを開く.
		ofstream a_streamAmplitudeFile(i_pbyAmplitudeFilename, ios::out);
		//位相特性を書き込むファイルを開く.
		ofstream a_streamPhaseFile(i_pbyPhaseFilename, ios::out);


	    for(double omega=0.001; omega<M_PI; omega+=0.001)
		{
	        complex<double> e1 = polar(1.0, -omega);
	        complex<double> e2 = polar(1.0, -2.0*omega);
	        complex<double> h = polar(1.0, 0.0);
	        for(long a_lIndex=0; a_lIndex<i_lNumSection; a_lIndex++)
			{
	            const double* a = this->m_pcBlockDiagram[a_lIndex].getCoefficientA();
	            const double* b = this->m_pcBlockDiagram[a_lIndex].getCoefficientB();
	            
	            h *= b[0] * (a[0] + a[1]*e1 + a[2]*e2) / (1.0 - b[1]*e1 - b[2]*e2);
	        }
	        
			//周波数.
	        double a_dFreq = omega * this->getSampleRate() / (2*M_PI);
			//振幅.
			double a_dAmplitude = this->getAmplitude(h);
			//位相.
			double a_dPhase = this->getPhase(h);
			//振幅特性を出力.
			a_streamAmplitudeFile << a_dFreq << ',' << a_dAmplitude << endl;
			//位相特性を出力.
			a_streamPhaseFile << a_dFreq << ',' << a_dPhase << endl;
	    }
		//ファイルを閉じる.
		a_streamAmplitudeFile.close();
		a_streamPhaseFile.close();
	}

	/**
	 * @brief	ブロックダイアグラム.
	 */
	CBlockDiagram* m_pcBlockDiagram;

	/**
	 * @brief	サンプリング周波数[Hz].
	 */
	double m_dSampleRate;

	/**
	 * @brief	カットオフ周波数[Hz].
	 */
	double m_dCutoffFreq;

	/**
	 * @brief	低い方のカットオフ周波数[Hz].
	 * @note	バンドパスフィルタのみで使用する.
	 */
	double m_dLowCutoffFreq;

	/**
	 * @brief	高い方のカットオフ周波数[Hz].
	 * @note	バンドパスフィルタのみで使用する.
	 */
	double m_dHighCutoffFreq;

	/**
	 * @brief	パスバンドの周波数[Hz].
	 * @note	decisionPrototype()での引数.
	 */
	double m_dPassFreq;

	/**
	 * @brief	偏差の量[dB].
	 * @note	decisionPrototype()での引数.
	 */
	double m_dRippleGain;

	/**
	 * @brief	ストップバンドの周波数[Hz].
	 * @note	decisionPrototype()での引数.
	 */
	double m_dStopFreq;

	/**
	 * @brief	減衰量[dB].
	 * @note	decisionPrototype()での引数.
	 */
	double m_dAttenuateGain;

	/**
	 * @brief	プロトタイプローパスフィルタのカットオフ周波数.
	 */
	double m_dPrototypeCutFreq;

	/**
	 * @brief	プロトタイプローパスフィルタのパス周波数.
	 */
	double m_dPrototypePassFreq;

	/**
	 * @brief フィルタの次数.
	 */
	long m_lOrderNumber;
};

#endif //__CIIR_FILTER_H__

