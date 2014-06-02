#include <iostream>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include "common.h"
using namespace std;

int msec = 500;

namespace {
	int hairetsu[2][12][22];
	int BlockMove[4][4];
	int dyuma_flag[10];
	int z_R[zrnum + 1] = { SENTINEL, SENTINEL, SENTINEL }; //Right.　待機ブロックを記憶。
	int z_L = SENTINEL;    //Left.   HOLDを記憶。
	int z_C = SENTINEL;    //center. BlockMoveを記憶。
	int x, y;//BlockMoveの座標を記憶。make
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
		for (int i = 0; i < zrnum - 1; i++){
			z_R[i] = z_R[i + 1];
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
	BlockMove[i][j] = z_C + 2 - BlockMove[i][j];
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
		case 0:irekae(1, 0);
			irekae(1, 1);
			irekae(1, 3);
			irekae(0, 2);
			irekae(2, 2);
			irekae(3, 2);
		case 1:
			break;
		default:
			kaiten_2();
			break;
		}
	}
	dyuma_flag[6]++;
	if (dyuma_flag[6] != 2){
		if (Judge() == 1 && j == 1){
			kaiten(3);
		}
		else if (Judge() == 1 && j == 3){
			kaiten(1);
		}
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
		z_R[i] = z_R[i + 1];
	}
	z_R[zrnum] = rand() % 7;
	cout << counter << "\n";
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			BlockMove[i][j] = Block[z_C][j][i];
		}
	}
	if (Judge() == 1){
		cout << "GameOver!" << "\n" << tensuu << "点でした！";
		exit(0);
	}
	dyuma_flag[0] = 1;
	counter++;
	msec--;
	cout << msec;
	if (counter % 10 == 0){
		for (int j = 1, r = rand() % 10 + 1; j <= 20; j++){
			for (int i = 1; i <= 10; i++){
				hairetsu[1][i][j] = hairetsu[1][i][j + 1];
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
	int flag3 = 0;
	for (int i = 1; i <= 10; i++){
		if (hairetsu[0][i][j] != 0){
			flag3++;
		}
	}
	if (flag3 == 10){
		dyuma_flag[2]++;
		kurikaeshi(j - 1);
	}
}

int tensuukeisann(int i){
	if (i == 1){
		return 1;
	}
	else{
		return tensuukeisann(i - 1) * 2;
	}
}

void Del_Judge(){
	dyuma_flag[2] = 0;
	dyuma_flag[3] = 20;
	for (int j = 20; j >= 1 + dyuma_flag[2]; j--){

		if (dyuma_flag[2] + j <= dyuma_flag[3]){
			kurikaeshi(j);
			dyuma_flag[3] = j;
		}
		for (int k = 1; k <= 10; k++){
			hairetsu[0][k][j] = hairetsu[0][k][j - dyuma_flag[2]];
			hairetsu[1][k][j] = hairetsu[1][k][j - dyuma_flag[2]];
		}
	}
	if (dyuma_flag[2] != 0){
		tensuu += tensuukeisann(dyuma_flag[2]) * 10;
	}
	cout << "\n" << tensuu;
}

void hantei(int houkou, int x0, int y0){
	switch (houkou) {
	case GLUT_KEY_UP: // 上キー
		dyuma_flag[6] = 0;
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
		x--;
		if (Judge() == 1){
			x++;
		}
		break;
	case GLUT_KEY_RIGHT: // 右キー
		cout << "right\n";
		x++;
		if (Judge() == 1){
			x--;
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

// 描画処理を書く
// 具体的には、スコア表示(displayScoreを使用)、ホールド表示(displayHold使用)、next表示(displayNext使用)と、フィールド上のブロックの描画をする。
void inner_display()
{
	displayScore(tensuu); // スコア描画
	displayHold(z_L); // ホールド描画
	displayNext(z_R[0], z_R[1], z_R[2]); // next描画

	// ゴーストブロック表示テスト
	int bottom = 20;
	while (true) {
		bool decide = true;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if ((BlockMove[i][j]>Black && j + bottom>20) || (hairetsu[0][i + x][j + bottom]>Black && BlockMove[i][j]>Black)) {
					decide = false;
					break;
				}
			}
			if (!decide) {
				break;
			}
		}
		if (decide) {
			break;
		}
		else {
			bottom--;
		}
	}
	if (y <= bottom) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (BlockMove[i][j] == Black) continue;

				displayGhostBlock(i + x - 1, j + bottom - 1, BlockMove[i][j]);
			}
		}
	}
	

	// フィールド上のブロック描画
	for (int i = 1; i <= 10; i++){
		for (int j = 1; j <= 20; j++){
			if (hairetsu[0][i][j] == Black) continue;
			displayBlock(i-1, j - 1, hairetsu[0][i][j]);
		}
	}
}