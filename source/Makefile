CC = g++
LIBS = 
LDFLAGS = -Wall -O2

SRCS_MAIN = ./main.cpp 
SRCS_WAVEFILE = ./SoundInformation/BSoundInformation.cpp ./SoundInformation/CWaveFormatOperator.cpp 
SRCS_FILTERS = ./Filter/FilterDesign/CBlockDiagram.cpp 
SRCS_BUTTERWORTH = ./Filter/FilterDesign/IIRFilter/Butterworth/CButterworthCommon.cpp
SRCS_CHEBYCHEFF = ./Filter/FilterDesign/IIRFilter/Chebycheff/CChebycheffCommon.cpp
SRCS = $(SRCS_MAIN) $(SRCS_WAVEFILE) $(SRCS_FILTERS) $(SRCS_BUTTERWORTH) $(SRCS_CHEBYCHEFF)

OUTFILE = ./myfilter


make:
	$(CC) $(LDFLAGS) -o $(OUTFILE) $(LIBS) $(SRCS)

clean:
	rm $(OUTFILE)
	rm *.wav

