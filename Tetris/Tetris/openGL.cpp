#include <cstdlib>
#include <iostream>
#include <ctime>
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
	{ 0.31, 0.0, 0.31 }
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
	{ 0.21, 0.0, 0.21 }
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

namespace {
	int crdnt = 0;
}

// ここに描画の処理を書く
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	displayField();

	// ここに描画の処理を書く

	displayBlock(0, crdnt, Aqua); // サンプルAqua
	displayBlock(1, crdnt + 1, Yellow); //サンプルYellow
	displayBlock(2, crdnt + 2, Green); // サンプルGreen
	displayBlock(3, crdnt + 3, Red); // サンプルRed
	displayBlock(4, crdnt + 4, Blue); // サンプルBlue
	displayBlock(5, crdnt + 5, Orange); // サンプルOrange
	displayBlock(6, crdnt + 6, Purple); // サンプルPurple

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

	glutPostRedisplay(); // いじらないこと
}

void idle()
{

}


// ブロックをx,yにcolor色で描画("フィールドの"左上が原点)
void displayBlock(int x, int y, color color)
{
	glColor3dv(colors2[color]);
	glBegin(GL_QUADS);
	glVertex2d((width + frame / 2 + x)*size, (frame / 2 + y)*size);
	glVertex2d((width + frame / 2 + x + 1)*size, (frame / 2 + y)*size);
	glVertex2d((width + frame / 2 + x + 1)*size, (frame / 2 + y + 1)*size);
	glVertex2d((width + frame / 2 + x)*size, (frame / 2 + y + 1)*size);
	glEnd();
	glColor3dv(colors[color]);
	glBegin(GL_QUADS);
	glVertex2d((width + frame / 2 + x + 0.1)*size, (frame / 2 + y + 0.1)*size);
	glVertex2d((width + frame / 2 + x + 0.9)*size, (frame / 2 + y + 0.1)*size);
	glVertex2d((width + frame / 2 + x + 0.9)*size, (frame / 2 + y + 0.9)*size);
	glVertex2d((width + frame / 2 + x + 0.1)*size, (frame / 2 + y + 0.9)*size);
	glEnd();
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

// マウスの挙動を書く
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
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

// 特殊キーが押されたときの挙動を書く
void keyboard2(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: // 上キー
		break;
	case GLUT_KEY_DOWN: // 下キー
		cout << "down\n";
		++crdnt;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: // 左キー
		break;
	case GLUT_KEY_RIGHT: // 右キー
		break;
	default:
		break;
	}
}