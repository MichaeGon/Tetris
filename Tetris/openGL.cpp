#include <fstream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <GL/glut.h>
#include "common.h"
using namespace std;

char* title = "Tetris";



// ブロックの大きさ
const int size = 50;

// 余白
const int frame = 2;

namespace {
	char* numbers = "0123456789";

	// 色配列
	// colors2の方が暗い色
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

// ここに描画の処理を書く
void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
	displayField();
	displayHighScore();
	
	inner_display();

	glutSwapBuffers();
}

// ここにアニメーション処理や時間経過による再描画処理を書く
void timer(int value)
{
	glutTimerFunc(msec, timer, 0); // 次のタイマー

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


void displayBlock(int x, int y, int col)
{
	drawBlock(x, y, static_cast<color>(col), true);
}

void displayBlock(int x, int y, color col)
{
	drawBlock(x, y, col, true);
}

void displayGhostBlock(int x, int y, int col)
{
	drawBlock(x, y, static_cast<color>(col), false);
}

void displayGhostBlock(int x, int y, color col)
{
	drawBlock(x, y, col, false);
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
	va_list ap;
	va_start(ap, next);
	for (int i = 0; i < zrnum; i++) {
		int arg = i ? va_arg(ap, int) : next;
		if (arg >= Black && arg < Invalid) {
			if (i == 0) {
				displayFrame("NEXT", 5, false, true);
			}
			omitNext(arg, i + 1);
		}
	}
	va_end(ap);
}

void displayHighScore()
{
	ifstream ifs("score.dat", ios::binary);
	string highn, highs;
	int h = 0;
	if (ifs) {
		getline(ifs, highn);
		getline(ifs, highs);
		h = atoi(highs.c_str());
	}

	int pos = 17;
	displayFrame("HIGHEST", pos, true, false);

	glColor3dv(colors[Black]);
	glRasterPos2d(size * 3 / 5, size);

	int num[size] = { 0 };
	for (int i = 1; i < size; i++) {
		num[i] = SENTINEL;
	}
	int i;
	for (i = 0; h; i++) {
		num[i] = h % 10;
		h /= 10;
	}
	if (i) {
		i--;
	}

	glRasterPos2d(size * 2, (pos + 8.0 / 5.0)*size);
	for (const char* str = highn.c_str(); *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
	}

	pos++;
	glRasterPos2d(size * 2, (pos + 9.0 / 5.0)*size);
	while (i >= 0) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numbers[num[i--]]);
	}
}