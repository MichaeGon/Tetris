#ifndef ___COMMONHEADER___
#define ___COMMONHEADER___

// 番兵
const int SENTINEL = -1;

// 色指定用列挙体
enum color { Black, Gray, Aqua, Yellow, Green, Red, Blue, Orange, Purple };
// 色配列
extern double colors[][3];
// １段階暗い色
extern double colors2[][3];

// タイトル
extern char* title;

// 数字集
extern char* numbers;

// スコア表示用
// 引数はスコア(マイナスは考慮しない)
void displayScore(int score);

// マウス
void mouse(int button, int state, int x, int y);
// キーボード(通常)
void keyboard(unsigned char key, int x, int y);
// キーボード(方向キーなど特殊キー)
void keyboard2(int key, int x, int y);

// ブロック描画(左上原点)
void displayBlock(int x, int y, color color);

// フィールド描画
void displayField();

// 描画の際に呼び出される関数
void display();

// 時間経過で呼び出される関数
// int型の引数を受け取れる
// glutTimerFunc()のほうで時間などは指定する
void timer(int value);

// アニメーション
void idle();

// リサイズ
void resize(int w, int h);

//初期化
void SHOKIKA();

#endif