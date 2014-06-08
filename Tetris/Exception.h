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

	// メッセージなど変数の初期化を行う場所
	Exception()
	{
		message = "\aエラー発生時に表示するメッセージを入力すること。\n";
	}

	virtual ~Exception() {};

	// エラーをキャッチしたときに行う処理
	virtual void handling() = 0;
};

#endif