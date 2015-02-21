CC = g++
LIBS = 
LDFLAGS = -Wall -O2

SRCS_TEST_WAVEFORMAT_OPERATOR = ./test_WaveFormatOperator.cpp ../SoundInformation/SoundInformation.cpp ../SoundInformation/WaveFormatOperator.cpp

OUTFILE_TEST_WAVEFORMAT_OPERATOR = ./test_WaveFormatOperator

testWaveFormatOperator:
	$(CC) $(LDFLAGS) -o $(OUTFILE_TEST_WAVEFORMAT_OPERATOR) $(LIBS) $(SRCS_TEST_WAVEFORMAT_OPERATOR)

clean:
	rm $(OUTFILE_TEST_WAVEFORMAT_OPERATOR)


