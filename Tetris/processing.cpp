#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include "common.h"
using namespace std;

int msec = 500;

string name;

namespace {
	int hairetsu[2][12][22];
	int BlockMove[4][4];
	int dyuma_flag[10];
	int z_R[zrnum + 1] = { SENTINEL, SENTINEL, SENTINEL }; //Right.�@�ҋ@�u���b�N���L���B
	int z_L = SENTINEL;    //Left.   HOLD���L���B
	int z_C = SENTINEL;    //center. BlockMove���L���B
	int x, y;//BlockMove�̍��W���L���Bmake
	int tensuu;//�_��������ϐ��B
	int counter;//�������Ă�͉̂��ڂ̃~�m���B
	int counter2;//�z�[���h����Ă�͉̂��ڂ̃~�m���B
}

void record()
{
	string highs;
	string highn;
	int h = 0;
	ifstream ifs("score.dat", ios::binary);
	if (ifs) {
		getline(ifs, highn);
		getline(ifs, highs);
		h = atoi(highs.c_str());
	}

	ofstream ofs("score.dat", ios::binary);
	if ((h!=0 && tensuu>h) || h==0) {
		ofs << name << endl << tensuu << endl;
	}
	else {
		ofs << highn << endl << h << endl;
	}
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
		int p;//�ڍs�ɗp����ϐ�
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
		cout << "GameOver!" << "\n" << tensuu << "�_�ł����I";
		record();
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
	case GLUT_KEY_UP: // ��L�[
		dyuma_flag[6] = 0;
		if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
			// �V�t�g�L�[��������Ă����Ԃ̎��̋���������
			cout << "Shift + ";
			kaiten(3);
		}
		else{
			kaiten(1);
		}
		cout << "up\n";

		break;
	case GLUT_KEY_DOWN: // ���L�[
		cout << "down\n";
		if (dyuma_flag[0] == 0){
			makeBlock();
		}
		else{
			y++;
			if (Judge() == 1){
				y--;
				/*�Œ艻����������*/
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
	case GLUT_KEY_LEFT: // ���L�[
		cout << "left\n";
		x--;
		if (Judge() == 1){
			x++;
		}
		break;
	case GLUT_KEY_RIGHT: // �E�L�[
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

// �`�揈��������
// ��̓I�ɂ́A�X�R�A�\��(displayScore���g�p)�A�z�[���h�\��(displayHold�g�p)�Anext�\��(displayNext�g�p)�ƁA�t�B�[���h��̃u���b�N�̕`�������B
void inner_display()
{
	displayScore(tensuu); // �X�R�A�`��
	displayHold(z_L); // �z�[���h�`��
	displayNext(z_R[0], z_R[1], z_R[2]); // next�`��



	// �S�[�X�g�u���b�N�̏���
	int bottom = y;
	if (z_C >= 0){
		while (true){
			bool decide = 0;
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					if (BlockMove[i][j] != Black && (hairetsu[1][i + x][j + bottom] >Black || j + bottom>19)){
						if (j + bottom > 19 && hairetsu[1][i + x][j + bottom] <= Black){
							bottom++;
						}
						decide = 1;
						break;
					}
				}
				if (decide == 1){
					break;
				}
			}
			if (decide == 1){
				bottom--;
				break;
			}
			else{
				bottom++;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (BlockMove[i][j] == Black) continue;

			displayGhostBlock(i + x - 1, j + bottom - 1, BlockMove[i][j]);
		}
	}

	// �t�B�[���h��̃u���b�N�`��
	for (int i = 1; i <= 10; i++){
		for (int j = 1; j <= 20; j++){
			if (hairetsu[0][i][j] == Black) continue;
			displayBlock(i-1, j - 1, hairetsu[0][i][j]);
		}
	}
}