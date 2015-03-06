#include "../SoundInformation/CWaveFormatOperator.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    CWaveFormatOperator waveFile;
	char* inFileName = argv[1];
	bool a_bIsSuccess = true;

	// WAVEファイルを読み込み.
	if(a_bIsSuccess)
	{
		cout << "--- Read file now ---" << endl;
		a_bIsSuccess = waveFile.readWaveFile(inFileName);
	}
	if(false == a_bIsSuccess)
	{
		cout << "--- error: Read file ---" << endl;
	}

	cout << endl;

	// WAVEファイルを書き込み.
    if(a_bIsSuccess)
	{
        cout << "--- Write file now ---" << endl;
        a_bIsSuccess = waveFile.writeWaveFile("out.wav");
    }
	if(false == a_bIsSuccess)
	{
		cout << "--- error: Write file ---" << endl;
	}
	
    return 0;
}

