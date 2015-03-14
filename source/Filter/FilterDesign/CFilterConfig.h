/**
 * @file	CFilterConfig.h
 * @brief	フィルタの設定を読み込む.
 */

#ifndef __CFILTER_CONFIG_H__
#define __CFILTER_CONFIG_H__

#include <fstream>
#include <map>
#include <string>	//文字列の分割で使用.
#include <regex>	//C11で使用可能.コンパイル時に オプション -std=c++11 が必要. 
using namespace std;

/**
 * @brief	フィルタの設定値を読み込むクラス.
 */
class CFilterConfig
{
public:
	/**
	 * @brief	コンストラクタ.
	 */
	CFilterConfig()
	{
	}

	/**
	 * @brief	デストラクタ.
	 */
	virtual ~CFilterConfig()
	{
	}

	/**
	 * @brief	フィルタの設定値を取得.
	 * @param	string i_strFilename 設定値が記述されているファイル名.
	 * @return	フィルタの設定値.
	 */
	map<string,string> getConfig(string i_strFilename)
	{
		string a_strLine;
		map<string,string> a_mapPairs;

		ifstream a_cInputStream(i_strFilename);
		if(a_cInputStream.fail())
		{
			//ファイルが開けなかった場合どうする？
			//とりあえず、リターンする.
			return a_mapPairs;
		}

		while( getline(a_cInputStream, a_strLine) )
		{
			E_CONFIG_KIND a_eConfigKind = this->getConfigKind(a_strLine);
			if(eComment == a_eConfigKind) this->nop();
			else if(eConfig == a_eConfigKind) this->setValue(a_strLine, a_mapPairs);
			else this->nop();
		}
		return a_mapPairs;
	}

protected:
	typedef enum{ eUnknown, eComment, eConfig } E_CONFIG_KIND; 

	void nop()
	{
		//cout << "nop: " << endl;
	}

	void setValue(string& i_strLine, map<string,string>& i_mapPairs)
	{
		vector<string> a_vecSplitStr;
		string a_strDelim="=";

		a_vecSplitStr = this->split(i_strLine, a_strDelim );
		string a_strKey = a_vecSplitStr[0];
		string a_strValue = a_vecSplitStr[1];
		i_mapPairs[a_strKey] = a_strValue;
	}

	bool checkCommentString(string& i_strLine)
	{
		regex a_cCommentRegex("^#.+");
		bool a_bIsResult = regex_match(i_strLine, a_cCommentRegex);
		return a_bIsResult;
	}
	bool checkConfigString(string& i_strLine)
	{
		regex a_cConfigRegex(".+=.+");
		bool a_bIsResult = regex_match(i_strLine, a_cConfigRegex);
		return a_bIsResult;
	}

	E_CONFIG_KIND getConfigKind(string& i_strLine)
	{
		bool a_bRet = false;

		//文字列がコメントか否かをチェック.
		a_bRet = this->checkCommentString(i_strLine);
		if(a_bRet) return eComment;

		//文字列がコンフィグか否かをチェック.
		a_bRet = this->checkConfigString(i_strLine);
		if(a_bRet) return eConfig;

		//不明な文字列.
		return eUnknown;
	}

	/**
	 * @brief	文字列の分割.
	 * @param	const string& i_strTarget	分割したい文字列.
	 * @param	const string& i_strDelim	デリミタ.
	 * @return	分割された文字列.vector.
	 */
	vector<string> split(string& i_strTarget, string& i_strDelim)
	{
		vector<string> a_vecPairs;
		size_t a_lCurrent = 0;
		size_t a_lFound;
		size_t a_lDelimLen = i_strDelim.size();
		while((a_lFound = i_strTarget.find(i_strDelim, a_lCurrent)) != string::npos)
		{
			a_vecPairs.push_back( string(i_strTarget, a_lCurrent, a_lFound - a_lCurrent) );
			a_lCurrent = a_lFound + a_lDelimLen;
		}
		a_vecPairs.push_back( string(i_strTarget, a_lCurrent, i_strTarget.size() - a_lCurrent) );
		return a_vecPairs;
	}
};

#endif	//__CFILTER_CONFIG_H__

