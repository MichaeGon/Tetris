#ifndef ___SCORE_FILE_ERROR___
#define ___SCORE_FILE_ERROR___
#include <cstdlib>
#include <fstream>
#include "Exception.h"
#include "common.h"
using namespace std;

class ScoreFileError :
	public Exception
{
public:
	ScoreFileError()
	{
		message = "\aこのセーブデータは不正です。\n終了します。何か入力してください：";
	}

	~ScoreFileError() {};

	void handling()
	{
		cout << message;

		while (!getchar());

		ofstream ofs(file, ios::binary);
		ofs;

		exit(1);
	}
};

#endif