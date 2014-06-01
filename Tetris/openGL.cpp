#include <cstdarg>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <GL/glut.h>
#include "common.h"
using namespace std;


double colors[][3] = {
	{ 0.2, 0.2, 0.2 },
	{ 0.4, 0.4, 0.4 },
	{ 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.67, 1.0, 0.02 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.64, 0.0 },
	{ 0.71, 0.0, 0.71 }
};

double colors2[][3] = {
	{ 0.2, 0.2, 0.2 },
	{ 0.3, 0.3, 0.3 },
	{ 0.0, 0.9, 0.9 },
	{ 0.9, 0.9, 0.0 },
	{ 0.57, 0.9, 0.01 },
	{ 0.9, 0.0, 0.0 },
	{ 0.0, 0.0, 0.9 },
	{ 0.9, 0.54, 0.0 },
	{ 0.51, 0.0, 0.51 }
};


char* title = "Tetris";


// ブロックの大きさ
const int size = 50;

// フィールドの横幅(ブロック何個分か)
const int width = 10;
// フィールドの縦幅(ブロック何個分か)
const int height = 20;

// 余白
const int frame = 2;

// z_R[]の要素数
const unsigned zrnum = 1;

namespace {
	char* numbers = "0123456789";

	int crdnt = 0;

	int hairetsu[2][12][22]; 
	int Block[7][4][4] = {
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
	int BlockMove[4][4];
	int dyuma_flag[10];
	int z_R[zrnum+1] = { SENTINEL }; //Right.　待機ブロックを記憶。
	int z_L = SENTINEL;    //Left.   HOLDを記憶。
	int z_C = SENTINEL;    //center. BlockMoveを記憶。
	int x, y;//BlockMoveの座標を記憶。
	int tensuu;//点数を入れる変数。
	int counter;//今落ちてるのは何個目のミノか。
	int counter2;//ホールドされてるのは何個目のミノか。
}

int Judge(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (BlockMove[i][j] != 0){
				if (hairetsu[1][i + x][j + y] != 0){
					return 1;
				}
				if (x + i>10 || x + i<1 || y + j>20 || y + j < 0){
					return 1;
				}
			}
		}
	}
	return 0;
}

void HOLD(){
	static int BlockHold[4][4];

	counter2 = counter;
		if (z_L == SENTINEL){
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					BlockHold[i][j] = BlockMove[i][j];
					BlockMove[i][j] = Block[z_R[0]][i][j];
				}
			}
			z_L = z_C;
			z_C = z_R[0];
			for (int i=0; i < zrnum-1; i++){
				z_R[i] = z_R[i+1];
			}
			z_R[zrnum] = rand() % 7;
		}
		else{
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					int BlockHold2;
					BlockHold2 = BlockHold[i][j];
					BlockHold[i][j] = BlockMove[i][j];
					BlockMove[i][j] = BlockHold2;
				}
			}
			int p;//移行に用いる変数
			p = z_C;
			z_C = z_L;
			z_L = p;
		}
		if (Judge() == 1){
			HOLD();
		}
}

void irekae(int i, int j){
	BlockMove[i][j] = z_C+2 - BlockMove[i][j];
}

void kaiten_2(){
	int dyuma_color;
	dyuma_color = BlockMove[0][0];
	BlockMove[0][0] = BlockMove[0][2];
	BlockMove[0][2] = BlockMove[2][2];
	BlockMove[2][2] = BlockMove[2][0];
	BlockMove[2][0] = dyuma_color;
	dyuma_color = BlockMove[0][1];
	BlockMove[0][1] = BlockMove[1][2];
	BlockMove[1][2] = BlockMove[2][1];
	BlockMove[2][1] = BlockMove[1][0];
	BlockMove[1][0] = dyuma_color;
}


void kaiten(int j){
	for (int i = 0; i < j; i++){
		switch (z_C){
		case 0:irekae(0, 1);
			irekae(1, 1);
			irekae(3, 1);
			irekae(2, 0);
			irekae(2, 2);
			irekae(2, 3);
		case 1:
			break;
		default:
			kaiten_2();
			break;
		}
	}
	if (Judge() == 1&&j==1){
			kaiten(3);
	}
	else if (Judge() == 1 && j == 3){
		kaiten(1);
	}
}

void makeBlock(){
	x = 4;
	y = 0;
	if (z_R[0] == SENTINEL){
		srand((unsigned)time(NULL));
		for (int i = 0; i <= zrnum; i++){
			z_R[i] = rand() % 7;
		}
	}
	z_C = z_R[0];
	for (int i = 0; i < zrnum; i++){
		z_R[i] = z_R[i+1];
	}
	z_R[zrnum] = rand() % 7;
	cout <<counter<<"\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			BlockMove[i][j] = Block[z_C][i][j];
		}
	}
	if (Judge() == 1){
		cout << "GameOver!" << "\n" << tensuu << "点でした！";
		exit(0);
	}
	dyuma_flag[0] = 1;
	counter++;
	if (counter % 10 == 0){
		for (int j = 1, r = rand() % 10+1; j <=20; j++){
			for (int i = 1; i <= 10; i++){
				hairetsu[1][i][j] = hairetsu[1][i][j+1];
				if (j == 20){
					if (r != i){
						hairetsu[1][i][j] = 1;
					}
				}
			}
		}
	}

}

void kurikaeshi(int j){
	int flag3=0;
	for (int i = 1; i <= 10; i++){
		if (hairetsu[0][i][j] != 0){
			flag3++;
		}
	}
	if (flag3 == 10){
		dyuma_flag[2]++;
		kurikaeshi(j-1);
	}
}

int tensuukeisann(int i){
	if (i == 1){
		return 1;
	}
	else{
		return tensuukeisann(i - 1)*2;
	}
}

void Del_Judge(){
	dyuma_flag[2] = 0;
	dyuma_flag[3] = 20;
	for (int j = 20; j >= 1 + dyuma_flag[2]; j--){
		
		if (dyuma_flag[2]+j <=  dyuma_flag[3]){
			kurikaeshi(j);
			dyuma_flag[3] = j;
		}
		for (int k = 1; k <= 10; k++){
			hairetsu[0][k][j] = hairetsu[0][k][j - dyuma_flag[2]];
			hairetsu[1][k][j] = hairetsu[1][k][j - dyuma_flag[2]];
		}
	}
	if (dyuma_flag[2] != 0){
		tensuu += tensuukeisann(dyuma_flag[2])*10;
	}
	cout << "\n" << tensuu;
}

void hantei(int houkou, int x0, int y0){
	switch (houkou) {
	case GLUT_KEY_UP: // 上キー
		if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
			// シフトキーが押されている状態の時の挙動を書く
			cout << "Shift + ";
			kaiten(3);
		}
		else{
			kaiten(1);
		}
		cout << "up\n";
		
		break;
	case GLUT_KEY_DOWN: // 下キー
		cout << "down\n";
		if (dyuma_flag[0] == 0){
			makeBlock();
		}
		else{
			y++;
			if (Judge() == 1){
				y--;
				/*固定化処理を入れる*/
				for (int i = 0; i < 4; i++){
					for (int j = 0; j < 4; j++){
						if (BlockMove[i][j] != 0){
							hairetsu[1][x + i][y + j] = BlockMove[i][j];
						}
					}
				}
				dyuma_flag[0] = 0;
			}
		}
		break;
	case GLUT_KEY_LEFT: // 左キー
		cout << "left\n";
		x++;
		if (Judge() == 1){
			x--;
		}
		break;
	case GLUT_KEY_RIGHT: // 右キー
		cout << "right\n";
		x--;
		if (Judge() == 1){
			x++;
		}
		break;
	case GLUT_KEY_F1:
		if (counter2 < counter){
			HOLD();
		}
		break;
	default:
		break;
	}
	
	for (int i = 1; i <= 10; i++){
		for (int j = 1; j <= 20; j++){
			hairetsu[0][i][j] = hairetsu[1][i][j];
		}
	}
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (BlockMove[i][j]){
				hairetsu[0][i + x][j + y] = BlockMove[i][j];
			}
		}
	}
	if (dyuma_flag[0] == 0){
		Del_Judge(); 
	}
	
	glutPostRedisplay();
}

// ここに描画の処理を書く
void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
	displayField();
	displayScore(tensuu); // 引数にスコアを渡してください
	displayHold(z_L);
	displayNext(z_R[0], z_R[1], z_R[2]);

	// ここに描画の処理を書く
	int i, j;
	for (i = 1; i <= 10; i++){
		for (j = 1; j <= 20; j++){
			displayBlock(10-i, j - 1, hairetsu[0][i][j]);
		}
	}


	glutSwapBuffers();
}

// ここにアニメーション処理や時間経過による再描画処理を書く
void timer(int value)
{
	// ここに処理を書く
	
	// 以下サンプル
	if (++crdnt >= height) {
		crdnt = 0;
	}
	glutTimerFunc(500, timer, 0); // 次のタイマー

	hantei(GLUT_KEY_DOWN, SENTINEL, SENTINEL);
}

void drawBlock(int x, int y, color color, bool fill)
{
	glColor3dv((fill ? colors2[color] : colors[color]));
	glBegin(GL_QUADS);
	glVertex2d((width + frame / 2 + x)*size, (frame / 2 + y)*size);
	glVertex2d((width + frame / 2 + x + 1)*size, (frame / 2 + y)*size);
	glVertex2d((width + frame / 2 + x + 1)*size, (frame / 2 + y + 1)*size);
	glVertex2d((width + frame / 2 + x)*size, (frame / 2 + y + 1)*size);
	glEnd();
	glColor3dv(colors[(fill ? color : Black)]);
	glBegin(GL_QUADS);
	glVertex2d((width + frame / 2 + x + 0.1)*size, (frame / 2 + y + 0.1)*size);
	glVertex2d((width + frame / 2 + x + 0.9)*size, (frame / 2 + y + 0.1)*size);
	glVertex2d((width + frame / 2 + x + 0.9)*size, (frame / 2 + y + 0.9)*size);
	glVertex2d((width + frame / 2 + x + 0.1)*size, (frame / 2 + y + 0.9)*size);
	glEnd();
}

// ブロックをx,yにcolor色で描画("フィールドの"左上が原点)
template <class Type> void displayBlock(int x, int y, Type col)
{
	drawBlock(x, y, static_cast<color>(col), true);
}

template <class Type> void displayGhostBlock(int x, int y, Type col)
{
	drawBlock(x, y, static_cast<color>(col), false);
}

void displayField()
{
	for (int i = -1; i <= width; ++i) {
		for (int j = -1; j <= height; ++j) {
			glColor3dv(colors[(i == -1 || j == -1 || i == width || j == height) ? Gray : Black]);
			glBegin(GL_QUADS);
			glVertex2d((width + frame / 2 + i)*size, (frame / 2 + j)*size);
			glVertex2d((width + frame / 2 + i + 1)*size, (frame / 2 + j)*size);
			glVertex2d((width + frame / 2 + i + 1)*size, (frame / 2 + j + 1)*size);
			glVertex2d((width + frame / 2 + i)*size, (frame / 2 + j + 1)*size);
			glEnd();
		}
	}
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(0, (width * 3 + frame)*size, (height + frame)*size, 0, -1, 1);
}

// キーボードの挙動を書く
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
		exit(0);
	default:
		break;
	}

}

void displayFrame(char* str, int pos, bool left, bool big)
{
	int package = 4;

	glColor3dv(colors[Black]);
	for (int i = 0; i < width; i += width - 2) {
		glBegin(GL_LINES);
		int tmp = left ? i : width * 2 + 2 + i;
		glVertex2d((tmp + 1.0 / 2.0)*size, pos*size);
		glVertex2d((tmp + 1.0 / 2.0)*size, (pos + package*(big ? zrnum : 1))*size);
		glEnd();
	}

	glBegin(GL_LINES);
	int tmp = left ? width : width * 3 + 2;
	glVertex2d((tmp - 1.5)*size, pos*size);
	glVertex2d((tmp - 1.5)*size, (pos - 0.5)*size);
	glVertex2d((tmp - 1.5)*size, (pos - 0.5)*size);
	glVertex2d((tmp - 2.5)*size, (pos - 0.5)*size);
	glVertex2d((tmp-width + (left ? 1 : 0.5))*size / (left ? 2.0 : 1), (pos + package*(big ? zrnum : 1))*size);
	glVertex2d((tmp - 1.5)*size, (pos + package*(big ? zrnum : 1))*size);
	glEnd();

	glRasterPos2d((tmp-width+1)*size, (pos - 1.0 / 5.0)*size);
	for (; *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
	}
}

// スコア表示用
// 引数はスコア
void displayScore(int score)
{
	glColor3dv(colors[Black]);
	glRasterPos2d(size * 3 / 5, size);
	for (char* str = title; *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
	}

	int pos = 11;
	displayFrame("SCORE", pos, true, false);

	int num[size] = { 0 };
	for (int i = 1; i < size; i++) {
		num[i] = SENTINEL;
	}
	int i;
	for (i = 0; score; i++) {
		num[i] = score % 10;
		score /= 10;
	}
	if (i) {
		i--;
	}
	
	glRasterPos2d(size * 2, (pos + 9.0 / 5.0)*size);
	while (i >= 0) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numbers[num[i--]]);
	}
}

void displayHold(int num)
{
	int pos = 5;
	displayFrame("HOLD", pos, true, false);
	if (num >= 0 && num <= 8) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (Block[num][i][j] != Black) {
					displayBlock(j - 8, i + pos - 1, Block[num][i][j]);
				}
			}
		}
	}
}

void omitNext(int next, int num)
{
	int interval = 4;
	glColor3dv(colors[Black]);
	glRasterPos2d((width * 2 + 3)*size, (interval*num + 3)*size);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numbers[num]);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (Block[next][i][j] != Black) {
				displayBlock(j + width*1.4, i + interval*num, Block[next][i][j]);
			}
		}
	}
}

void displayNext(int next, ...)
{
	int i = 1;
	if (next >= Black && next < Invalid) {
		omitNext(next, i);
	}

	va_list ap;
	for (va_start(ap, next); i < zrnum; i++) {
		int arg = va_arg(ap, int);
		if (arg >= Black && arg < Invalid) {
			omitNext(arg, i + 1);
		}
	}
	va_end(ap);

	displayFrame("NEXT", 5, false, true);
}