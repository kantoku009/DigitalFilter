CC = g++
LIBS = 
LDFLAGS = -Wall -O2

SRCS_MAIN = ./main.cpp 
SRCS_WAVEFILE = ./SoundInformation/SoundInformation.cpp ./SoundInformation/WaveFormatOperator.cpp 
SRCS_FILTERS = ./Filter/FilterDesign/BlockDiagram.cpp 
SRCS_BUTTERWORTH = ./Filter/FilterDesign/IIRFilter/Butterworth.cpp 
SRCS = $(SRCS_MAIN) $(SRCS_WAVEFILE) $(SRCS_FILTERS) $(SRCS_BUTTERWORTH)

OUTFILE = ./myfilter


make:
	$(CC) $(LDFLAGS) -o $(OUTFILE) $(LIBS) $(SRCS)

clean:
	rm $(OUTFILE)
	rm *.wav

