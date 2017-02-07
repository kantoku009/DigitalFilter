#コンパイラ.
CC = g++
#ライブラリ.
LIBS = 
#コンパイルオプション. c++11オプションをつける(<regex>と<map>の初期化を使用したい)
LDFLAGS = -Wall -O2 -std=c++11

#メイン処理のソースファイル.
SRCS_MAIN = ./main.cpp 
#音声ファイルを扱う処理のソースファイル.
SRCS_WAVEFORMAT_OPERATOR_DIR := WaveFormOperator/source
include $(SRCS_WAVEFORMAT_OPERATOR_DIR)/Makefile
SRCS_WAVEFILE = $(SRCS_WAVEFORMAT_OPERATOR)
#フィルタ処理に関する処理のソースファイル.
SRCS_FILTERS = ./Filter/FilterDesign/CBlockDiagram.cpp 
#バターワース特性フィルタに関する処理のソースファイル.
SRCS_BUTTERWORTH = ./Filter/FilterDesign/IIRFilter/Butterworth/CButterworthCommon.cpp
#チェビシェフ特性フィルタに関する処理のソースファイル.
SRCS_CHEBYCHEFF = ./Filter/FilterDesign/IIRFilter/Chebycheff/CChebycheffCommon.cpp
#ソースファイル全体.
SRCS = $(SRCS_MAIN) $(SRCS_WAVEFILE) $(SRCS_FILTERS) $(SRCS_BUTTERWORTH) $(SRCS_CHEBYCHEFF)

#ビルドされたプログラムファイル名.
OUTFILE = ./myfilter


make:
	$(CC) $(LDFLAGS) -o $(OUTFILE) $(LIBS) $(SRCS)

clean:
	rm $(OUTFILE)
	rm *.wav
	rm *.csv

