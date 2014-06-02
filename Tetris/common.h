#ifndef ___COMMONHEADER___
#define ___COMMONHEADER___

// 番兵
const int SENTINEL = -1;

// z_R[]の要素数
const unsigned zrnum = 3;

// ブロックのデータ
const int Block[][4][4] = {
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 2, 2, 2, 2 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 3, 3, 0 },
		{ 0, 3, 3, 0 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 4, 4, 0 },
		{ 4, 4, 0, 0 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 0, 0, 0, 0 },
		{ 5, 5, 0, 0 },
		{ 0, 5, 5, 0 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 6, 0, 0, 0 },
		{ 6, 6, 6, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 0, 0, 7, 0 },
		{ 7, 7, 7, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
	},
	{
		{ 0, 8, 0, 0 },
		{ 8, 8, 8, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
	}
};

// 色指定用列挙体
// 最後はInvalidとする。Invalidを引数に与えた時の挙動は保証しない
enum color { Black, Gray, Aqua, Yellow, Green, Red, Blue, Orange, Purple, Invalid };

// タイトル
extern char* title;

// 何ミリ秒ごとに落ちてくるか
extern int msec;


// 内部処理側で描画すべきものを入れる関数
// スコアの表示、HOLDの表示、NEXTの表示とフィールド上のブロック描画を担当する。
void inner_display();



// 以下openGL関連

// スコア表示用
// 引数はスコア(マイナスは考慮しない)
void displayScore(int score);

// HOLD表示用
// 引数は上記のBlock[i][][]のiでブロックの形を指定
void displayHold(int num);

// NEXT表示用
// 引数は次に落ちてくるブロック、その次に落ちてくるブロック・・・といくつも指定できる。
// 引数に何も指定しなかった場合はnextが非表示になる
void displayNext(int next = SENTINEL, ...);

// キーボード(通常)
void keyboard(unsigned char key, int x, int y);
// キーボード(方向キーなど特殊キー)
void hantei(int hantei, int x0, int y0);

// ブロック描画(左上原点)
template <class Type> void displayBlock(int x, int y, Type col);
// ゴーストブロック描画(左上原点)
template <class Type> void displayGhostBlock(int x, int y, Type col);

// 描画の際に呼び出される関数
void display();

// 時間経過で呼び出される関数
// int型の引数を自由に受け取れる
// glutTimerFunc()のほうで時間などは指定する
void timer(int value);

// リサイズ
void resize(int w, int h);

#endif