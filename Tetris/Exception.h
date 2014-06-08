// 例外のひな形クラス。
// 例外を新規作成するときはこれを継承してください

#ifndef ___SUPER_EXCEPTION___
#define ___SUPER_EXCEPTION___
#include <string>
#include <iostream>
using namespace std;

class Exception
{
protected:
	string message;
public:
	Exception()
	{
		message = "\aエラー発生時に表示するメッセージを入力すること。\n";
		// 同時にエラー出力もできます
		cout << message;
	}

	virtual ~Exception() {};

	// エラーをキャッチしたときに行う処理
	virtual void handling() = 0;
};

#endif