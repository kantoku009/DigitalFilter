CC = g++
LIBS = 
LDFLAGS = -Wall -O2

SRCS_BUTTERWORTH = ./main_Butterworth.cpp ./Filter/Butterworth.cpp ./Filter/Filter.cpp ./Filter/BlockDiagram.cpp ./SoundInformation/SoundInformation.cpp ./SoundInformation/WaveFormatOperator.cpp
SRCS_CHEBYCHEFF = ./main_Chebycheff.cpp ./Filter/Chebycheff.cpp ./Filter/Filter.cpp ./Filter/BlockDiagram.cpp ./SoundInformation/SoundInformation.cpp ./SoundInformation/WaveFormatOperator.cpp

OUTFILE_BUTTERWORTH = ./butterworth_filter
OUTFILE_CHEBYCHEFF = ./chebycheff_filter


make:
	$(CC) $(LDFLAGS) -o $(OUTFILE_BUTTERWORTH) $(LIBS) $(SRCS_BUTTERWORTH)
	$(CC) $(LDFLAGS) -o $(OUTFILE_CHEBYCHEFF) $(LIBS) $(SRCS_CHEBYCHEFF) 

clean:
	rm $(OUTFILE_BUTTERWORTH)
	rm $(OUTFILE_CHEBYCHEFF)


