#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <string>
#include <windows.h>
#include <vector>
#include <iomanip>


#define MAP_SIZE 8 //���� ũ��(8x8)�� �����ϱ� ���� ���

using namespace std;

/*
* SettingMap class
* Note: �Ϲݸ�, �����ʿ��� �������� ����ϴ� �ʼ��� �����Լ��� �������ִ� Ŭ����
*/
class SettingMap {
public:
	virtual void settingMap() = 0;

};

/*
* ShowingMap class
* Note: �Ϲݸ�, �����ʿ��� �������� ����ϴ� ����� �����Լ��� �������ִ� Ŭ����
*/
class ShowingMap {

	virtual void showingMap() = 0;
};

/*
* Map class
* Note: ���� �⺻���� ����� ����ϱ� ���� �Ϲݸ�, �������� �θ�Ŭ����
*/
class Map : public SettingMap, public ShowingMap {
public:
	int map[MAP_SIZE][MAP_SIZE] = { 0 }; //�� ����
	int isVisit[MAP_SIZE][MAP_SIZE] = { 0 }; //���� �ٲٱ� ���� ���� �湮�ϴ� ���� (�湮�� 1�� ����)
	int dx[MAP_SIZE] = { -1, 1, 0, 0, -1, -1, 1, 1 }; // �� ,��, ��, �Ʒ�, ����, �»�, ����, ������� �̵��� x�� ��ȭ
	int dy[MAP_SIZE] = { 0, 0, -1, 1, -1, 1, -1, 1 }; // ��, �� , ��, �Ʒ�, ����, �»�, ����, ������� �̵��� y�� ��ȭ

	virtual void settingMap() {};
	virtual void showingMap() {};

	/* ���� 0���� �ʱ�ȭ�ϴ� �Լ� */
	void defaultMap()
	{
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				map[i][j] = 0;
			}
		}
	}

	/* x, y, player�� ������� �� �����ϴ� �Լ� */
	void setMap(int x, int y, int player) {
		// player1�� ���
		if (player == 1) {
			map[x][y] = 1;
		}
		// player2�� ���
		else {
			map[x][y] = 2;
		}
	}

	/* x, y�� ����ǥ�� �������� �Լ� */
	int getMap(int x, int y) {
		return map[x][y];
	}

	/*���� ������ �� �ٲ���� Ȯ���ϴ� �Լ�*/
	bool changeCheck(int x, int y, int player, int direction) {
		bool result = false; //���� �ٲٴ��� Ȯ���ϴ� ����
		int xx = x;
		int yy = y;
		isVisit[xx][yy] = 1;
		if (player == 1) {
			while (1) {
				xx += dx[direction];
				yy += dy[direction];
				if (xx < 0 || yy < 0 || xx >= MAP_SIZE || yy >= MAP_SIZE) { //���� ����� ���
					break;
				}
				if (isVisit[xx][yy] == 1 || map[xx][yy] == 0 || map[xx][yy] == 3) { //�̹� �湮üũ �Ϸ��� ���, ���� ���� ���� ���, ��ֹ��� ���� ���� �� ���� ���
					break;
				}
				if (map[xx][yy] == 1) { //���� �ٲ� �� �ִ� ���
					result = true;
					break;
				}
				isVisit[xx][yy] = 1; //�湮 �� 1�� �ٲ�
			}
		}
		else {
			while (1) {
				xx += dx[direction];
				yy += dy[direction];
				if (xx < 0 || yy < 0 || xx >= MAP_SIZE || yy >= MAP_SIZE) {//���� ����� ���
					break;
				}
				if (isVisit[xx][yy] == 1 || map[xx][yy] == 0 || map[xx][yy] == 3) { //�̹� �湮üũ �Ϸ��� ���, ���� ���� ���� ���, ��ֹ��� ���� ���� �� ���� ���
					break;
				}
				if (map[xx][yy] == 2) { //���� �ٲ� �� �ִ� ���
					result = true;
					break;
				}
				isVisit[xx][yy] = 1; //�湮 �� 1�� �ٲ�
			}
		}
		return result;
	}

	/* ���� ������ �Լ� */
	void change(int player) {
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (player == 1 && isVisit[i][j]) { //player�� 1�̸鼭 �����Ⱑ ������ ���� ���
					if (map[i][j] != 4 && map[i][j] != 3) { //���� �� �ִ� ��ġǥ�õ� ���� �ƴϸ鼭 ��ֹ���ġ�� ǥ�õ� ���� �ƴ� ���
						map[i][j] = 1;
					}
				}
				else if (player == 2 && isVisit[i][j]) { //player�� 2�̸鼭 �����Ⱑ ������ ���� ���
					if (map[i][j] != 4 && map[i][j] != 3) //���� �� �ִ� ��ġǥ�õ� ���� �ƴϸ鼭 ��ֹ���ġ�� ǥ�õ� ���� �ƴ� ���
					{
						map[i][j] = 2;
					}
				}
			}
		}
	}

	/*��ȿ�� �ڸ����� �˻����ִ� �Լ�*/
	bool isVaildSpace(int x, int y) {
		if (getMap(x, y) == 0 || getMap(x, y) == 4) { //x, y�� ��ġ�� ����ְų� ���� �� �ִٰ� ǥ�õ� ���
			return true;
		}
		else if (x > MAP_SIZE || y > MAP_SIZE) { //x�� y�� �ִ������ ��� ���
			return false;
		}
		else if (x < 0 || y < 0) { //x�� y�� �ּҹ����� ��� ���
			return false;
		}
		else //������ ai�� �̹� ���� ���� ��ġ�� ���
		{
			return false;
		}
	}

	/*���� �� �ִ� ��ġ ������ ����*/
	void clearMap() {
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (getMap(i, j) == 4) {
					map[i][j] = 0;
				}
			}
		}
	}

	/*���� ���� �� �ִ��� Ȯ��*/
	void putCheck(int player) {
		int ii, jj;
		if (player == 1) {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (getMap(i, j) == 1) {
						// ��� �˻�
						ii = 1;
						while (i - ii >= 0) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j) == 0) {
								if (getMap(i - ii + 1, j) == 2) {
									map[i - ii][j] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j) == 2) {
								ii++;
							}
							else {
								break;
							}
						}

						// �ϴ� �˻�
						ii = 1;
						while (i + ii < MAP_SIZE) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j) == 0) {
								if (getMap(i + ii - 1, j) == 2) {
									map[i + ii][j] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j) == 2) {
								ii++;
							}
							else {
								break;
							}
						}

						// ���� �˻�
						jj = 1;
						while (j - jj >= 0) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i, j - jj) == 0) {
								if (getMap(i, j - jj + 1) == 2) {
									map[i][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i, j - jj) == 2) {
								jj++;
							}
							else {
								break;
							}
						}

						// ������ �˻�
						jj = 1;
						while (j + jj < MAP_SIZE) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i, j + jj) == 0) {
								if (getMap(i, j + jj - 1) == 2) {
									map[i][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i, j + jj) == 2) {
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (����)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j - jj >= 0)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j - jj) == 0) {
								if (getMap(i - ii + 1, j - jj + 1) == 2) {
									map[i - ii][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j - jj) == 2) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (�Ͽ�)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j + jj < MAP_SIZE)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j + jj) == 0) {
								if (getMap(i + ii - 1, j + jj - 1) == 2) {
									map[i + ii][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j + jj) == 2) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (����)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j - jj >= 0)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j - jj) == 0) {
								if (getMap(i + ii - 1, j - jj + 1) == 2) {
									map[i + ii][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j - jj) == 2) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (���)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j + jj < MAP_SIZE)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j + jj) == 0) {
								if (getMap(i - ii + 1, j + jj - 1) == 2) {
									map[i - ii][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j + jj) == 2) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

					}
				}
			}

		}
		else if (player == 2) {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (getMap(i, j) == 2) {
						// ��� �˻�
						ii = 1;
						while (i - ii >= 0) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j) == 0) {
								if (getMap(i - ii + 1, j) == 1) {
									map[i - ii][j] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j) == 1) {
								ii++;
							}
							else {
								break;
							}
						}

						// �ϴ� �˻�
						ii = 1;
						while (i + ii < MAP_SIZE) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j) == 0) {
								if (getMap(i + ii - 1, j) == 1) {
									map[i + ii][j] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j) == 1) {
								ii++;
							}
							else {
								break;
							}
						}

						// ���� �˻�
						jj = 1;
						while (j - jj >= 0) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i, j - jj) == 0) {
								if (getMap(i, j - jj + 1) == 1) {
									map[i][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i, j - jj) == 1) {
								jj++;
							}
							else {
								break;
							}
						}

						// ������ �˻�
						jj = 1;
						while (j + jj < MAP_SIZE) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i, j + jj) == 0) {
								if (getMap(i, j + jj - 1) == 1) {
									map[i][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i, j + jj) == 1) {
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (����)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j - jj >= 0)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j - jj) == 0) {
								if (getMap(i - ii + 1, j - jj + 1) == 1) {
									map[i - ii][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j - jj) == 1) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (�Ͽ�)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j + jj < MAP_SIZE)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j + jj) == 0) {
								if (getMap(i + ii - 1, j + jj - 1) == 1) {
									map[i + ii][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j + jj) == 1) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (����)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j - jj >= 0)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i + ii, j - jj) == 0) {
								if (getMap(i + ii - 1, j - jj + 1) == 1) {
									map[i + ii][j - jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i + ii, j - jj) == 1) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}

						// �밢�� Ȯ�� (���)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j + jj < MAP_SIZE)) {
							// 0�� �� ���� �� �ִ� ��ġ�� ����
							if (getMap(i - ii, j + jj) == 0) {
								if (getMap(i - ii + 1, j + jj - 1) == 1) {
									map[i - ii][j + jj] = 4;
									break;
								}
								else {
									break;
								}
							}
							else if (getMap(i - ii, j + jj) == 1) {
								ii++;
								jj++;
							}
							else {
								break;
							}
						}
					}
				}
			}
		}
	}

	/* ���� ���� �� �ִ��� �Ǵ��ϴ� �Լ� */
	bool putCanStone(int x, int y, int player) {
		if (getMap(x, y) == 4) {
			return true;
		}
		else if (getMap(x, y) == 1) {
			return false;
		}
		else if (getMap(x, y) == 2) {
			return false;
		}
		else if (getMap(x, y) == 3) {
			return false;
		}
		else
		{
			return false;
		}
	}

	/* ���� ������ ���� �����ϴ� �Լ� */
	void putStone(int x, int y, int player) {
		int recallA, recallB;

		if (putCanStone(x, y, player) && isVaildSpace(x, y)) { //���� �� �ְ�, ��ȿ�� �ڸ��� ���
			swapStone(x, y, player);
			setMap(x, y, player);
		}
		else {
			showingMap();
			if (player == 2)
			{
				cout << "��������������������������������������������������������������������������" << endl;
				cout << "��       ���� ���� ���� �ֽ��ϴ�.    ��" << endl;
				cout << "��������������������������������������������������������������������������" << endl;
				cout << "���� �� ���� ��ġ�Դϴ�. �ٽ� �Է����ּ��� !" << endl;
				cout << "(x y)�� �Է��ϼ��� : ";
				cin >> recallA >> recallB;
				y = recallA - 1;
				x = recallB - 1;
				putStone(x, y, player);
			}

		}
	}

	/* ���� ������ �� �ΰ��� �� ������ ������ ���� �� ���� �ٲ���ϴ� �Լ� */
	void swapStone(int x, int y, int player) {
		for (int i = 0; i < MAP_SIZE; i++) {

			memset(isVisit, 0, sizeof(isVisit)); //isVisit�� �޸𸮸� 0���� �ʱ�ȭ���ִ� �Լ�
			if (changeCheck(x, y, player, i)) {
				change(player);
			}
		}
	}

	/* ���� ���� ���� ��� ���� �Ѿ�� �Լ� */
	bool pass() {
		bool result = true;
		int passNum = 0; //���� �� �ִ� ��ġ�� ������ Ȯ���ϴ� ����

		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (getMap(i, j) == 4) {
					passNum++;
				}
			}
		}

		if (passNum == 0) {
			result = false;
			return result;
		}
		else {
			return result;
		}
	}
};

/*
* standardMap class
* Note: ���� �⺻���� ��� + �Ϲݸʿ����� ����� ����ϱ� ���� Ŭ����
*/

class standardMap : public Map {
public:

	/* ȣ��� ���ÿ� �Ϲݸ��� �����ϴ� ������ */
	standardMap()
	{
		settingMap();
	}

	/* �Ϲݸ��� �ʱⰪ ���� */
	virtual void settingMap() {
		map[3][3] = 1; // ���� ǥ���ϱ� ���ؼ�
		map[4][4] = 1;
		map[3][4] = 2; // ������ ǥ���ϱ����ؼ�
		map[4][3] = 2;
	}

	/* �Ϲݸ��� ������ִ� �Լ� */
	virtual void showingMap() {
		system("cls");
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��   �� �禢 �覢 �馢 �ꦢ �릢 �즢 �� �" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		for (int i = 0; i < MAP_SIZE; i++) {
			if (i == 0)cout << "�� ��";
			else if (i == 1)cout << "�� ��";
			else if (i == 2)cout << "�� ��";
			else if (i == 3)cout << "�� ��";
			else if (i == 4)cout << "�� ��";
			else if (i == 5)cout << "�� ��";
			else if (i == 6)cout << "�� ��";
			else if (i == 7)cout << "�� ��";
			for (int j = 0; j < MAP_SIZE; j++) {
				if (map[i][j] == 0)
					cout << "��   ";
				else if (map[i][j] == 1)
					cout << "�� ��";
				else if (map[i][j] == 2)
					cout << "�� ��";
				else if (map[i][j] == 4)
					cout << "�� \x1b[32m��\x1b[0m";
			}
			cout << "��\n";
			if (i < 7)
				cout << "��������������������������������������������������������������������������" << endl;
		}
		cout << "��������������������������������������������������������������������������" << endl;
	}
};

/*
* transformMap class
* Note: ���� �⺻���� ��� + �����ʿ����� ����� ����ϱ� ���� Ŭ����
*/
class transformMap : public Map {

public:

	/* ȣ��� ���ÿ� �������� �����ϴ� ������ */
	transformMap()
	{
		settingMap();
	}

	/* �������� �ʱⰪ ���� */
	virtual void settingMap() {
		map[3][3] = 1;
		map[4][4] = 1;
		map[3][4] = 2;
		map[4][3] = 2;

		int row[2] = { 8,8 };
		int columns[2] = { 8,8 }; // �ִ��� ���� �ٸ� ��� ���� �����ֵ��� ����� ���ؼ��̸� 3��ĭ�� ���� ����
		srand(static_cast<unsigned int>(time(NULL))); // C���� srand(time(NULL))�ϵ� �� �����̶�� ����
		int count = 0;
		while (count < 2) {
			int num1 = rand() % 7;
			int num2 = rand() % 7; // 0���� 7���� ĭ ����
			int flag = 0;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					if (row[i] == num1 || columns[j] == num2) // ������ �ִ� ��� ������ ���ϱ� ���ؼ� ����
						flag = 1; // ������Ǵ� ���� �ִٸ� 1, ���ٸ� 0
			}
			if (flag == 0) {
				row[count] = num1; // ���� �������� ���� �����ϰ��� �� �� ����
				columns[count] = num2; // �� ���� �׷��ϰ� �ۿ�
				if (map[num1][num2] == 0) { // �׷��� ó�� ������ �� 1,2�� ���� �� �ֱ� ������ �׷����� ������ �ۼ�
					map[num1][num2] = 3;
					count += 1;
				}
			}
		}
	}

	/* �������� ������ִ� �Լ� */
	virtual void showingMap() {
		system("cls");
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��   �� �禢 �覢 �馢 �ꦢ �릢 �즢 �� �" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		for (int i = 0; i < MAP_SIZE; i++) {
			if (i == 0)cout << "�� ��";
			else if (i == 1)cout << "�� ��";
			else if (i == 2)cout << "�� ��";
			else if (i == 3)cout << "�� ��";
			else if (i == 4)cout << "�� ��";
			else if (i == 5)cout << "�� ��";
			else if (i == 6)cout << "�� ��";
			else if (i == 7)cout << "�� ��";
			for (int j = 0; j < MAP_SIZE; j++) {
				if (map[i][j] == 0)
					cout << "��   ";
				else if (map[i][j] == 1)
					cout << "�� ��";
				else if (map[i][j] == 2)
					cout << "�� ��";
				else if (map[i][j] == 3)
					cout << "�� \x1b[31m��\x1b[0m";
				else if (map[i][j] == 4)
					cout << "�� \x1b[32m��\x1b[0m";
			}
			cout << "��\n";
			if (i < 7)
				cout << "��������������������������������������������������������������������������" << endl;
		}
		cout << "��������������������������������������������������������������������������" << endl;
	}
};

/*
* AI class
* Note : map�� ������� ���� ���� ai Ŭ����
*/
class AI {
	int x[60] = { 0 }, y[60] = { 0 };
	int check = 0;
	int bestChoice_X = 0, bestChoice_Y = 0;
public:

	/* ai�� x�� �����ϴ� �Լ� */
	void setX(int check, int i) {
		x[check] = i;
	}
	/* ai�� y�� �����ϴ� �Լ� */
	void setY(int check, int i) {
		y[check] = i;
	}
	/* ai�� bestChoice_X�� �����ϴ� �Լ� */
	void setBestX(int i)
	{
		bestChoice_X = i;
	}
	/* ai�� bestChoice_Y�� �����ϴ� �Լ� */
	void setBestY(int i)
	{
		bestChoice_Y = i;
	}
	/* ai�� x�� �޾ƿ��� �Լ� */
	int getX(int i) {
		return x[i];
	}
	/* ai�� y�� �޾ƿ��� �Լ� */
	int getY(int i) {
		return y[i];
	}
	/* ai�� bestChoice_X�� �޾ƿ��� �Լ� */
	int getBestX()
	{
		return bestChoice_X;
	}
	/* ai�� bestChoice_Y�� �޾ƿ��� �Լ� */
	int getBestY()
	{
		return bestChoice_Y;
	}

	/* ai�� x, y��ǥ�� �ʱ�ȭ */
	void defaultAll()
	{
		for (int i = 0; i < 60; i++) {
			x[i] = 0;
			y[i] = 0;
		}
		check = 0;
	}

	/* �Ϲݸʿ��� ������ ������ ���� */
	void getPossibleMoves(standardMap map) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (map.getMap(i, j) == 4) {
					setX(check, i);
					setY(check, j);
					check++;
				}
	}

	/* ��ȯ�ʿ��� ������ ������ ���� */
	void getPossibleMoves(transformMap map) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (map.getMap(i, j) == 4) {
					setX(check, i);
					setY(check, j);
					check++;
				}
	}

	/* �Ϲݸʿ��� AI�� ���� ���� �� �ִ��� Ȯ���ϰ� ������ bestChoice_X�� bestChoice_Y�� ���� */
	void smartcheck(standardMap map) {
		int minUser = 99999; //����� ������ �� ���� ���� ������ �� �ִ� ��
		for (int i = 0; i < check; i++) {
			standardMap newmap1; //AI�� ����� ���� ���� ���� ������ ���� ����
			newmap1 = map; //������ ���� ���� ������ ������ ����
			newmap1.map[getX(i)][getY(i)] = 1; //AI�� ���� �� �ִ� ��ġ�� �� ����
			newmap1.swapStone(getX(i), getY(i), 1); //AI�� ���� ���� ���� ��ȭ
			newmap1.clearMap(); //ai�� ���� �� �־��� �ڸ� �ʱ�ȭ
			newmap1.putCheck(2); //����� ���� �� �ִ� ��� ����
			int newX[60] = { 0 }, newY[60] = { 0 };
			int newCheck = 0;
			for (int a = 0; a < 8; a++)
				for (int b = 0; b < 8; b++)
					if (newmap1.getMap(a, b) == 4) {
						newX[newCheck] = a;
						newY[newCheck] = b;
						newCheck++;
					}
			int maxUser = -99999; //����� ������ �� ���� ���� ������ �� �ִ� ��
			for (int j = 0; j < newCheck; j++) {
				standardMap newmap2; //AI�� ����� ���� ���� ���� ������ ���� ����
				newmap2 = newmap1; //������ ���� ���� ������ ������ ����
				newmap2.map[getX(j)][getY(j)] = 1; //AI�� ���� �� �ִ� ��ġ�� �� ����
				newmap2.swapStone(getX(j), getY(j), 1); //AI�� ���� ���� ���� ��ȭ
				int aiStone = 0; //ai�� ���� �� �־��� �ڸ� �ʱ�ȭ
				int userStone = 0; //����� ���� �� �ִ� ��� ����
				for (int k = 0; k < 8; k++) {
					for (int m = 0; m < 8; m++) {
						if (newmap2.map[k][m] == 1)
							aiStone++;
						else if (newmap2.map[k][m] == 2)
							userStone++;
					}
				}
				if (maxUser < (userStone - aiStone)) //���� ���� ������ �� �ִ� ��캸�� �� ���� ������ �� �ִ� ���
					maxUser = (userStone - aiStone);
			}
			if (minUser > maxUser) { //���� ���� ������ �� �ִ� ��캸�� �� ���� ������ �� �ִ� ���
				minUser = maxUser;
				setBestX(getX(i)); //������ ��ġ bestChoice_X�� ����
				setBestY(getY(i)); //������ ��ġ bestChoice_Y�� ����
			}
		}
		defaultAll();
	}

	/* ��ȯ�ʿ��� AI�� ���� ���� �� �ִ��� Ȯ���ϰ� ������ bestChoice_X�� bestChoice_Y�� ���� (������ ���� ��Ŀ� ����) */
	void smartcheck(transformMap map) {
		int minUser = 99999;
		for (int i = 0; i < check; i++) {
			transformMap newmap1;
			newmap1 = map;
			newmap1.map[getX(i)][getY(i)] = 1;
			newmap1.swapStone(getX(i), getY(i), 1);
			newmap1.clearMap();
			newmap1.putCheck(2);
			int newX[60] = { 0 }, newY[60] = { 0 };
			int newCheck = 0;
			for (int a = 0; a < 8; a++)
				for (int b = 0; b < 8; b++)
					if (newmap1.getMap(a, b) == 4) {
						newX[newCheck] = a;
						newY[newCheck] = b;
						newCheck++;
					}
			int maxUser = -99999;
			for (int j = 0; j < newCheck; j++) {
				transformMap newmap2;
				newmap2 = newmap1;
				newmap2.map[getX(j)][getY(j)] = 1;
				newmap2.swapStone(getX(j), getY(j), 1);
				int aiStone = 0;
				int userStone = 0;
				for (int k = 0; k < 8; k++) {
					for (int m = 0; m < 8; m++) {
						if (newmap2.map[k][m] == 1)
							aiStone++;
						else if (newmap2.map[k][m] == 2)
							userStone++;
					}
				}
				if (maxUser < (userStone - aiStone))
					maxUser = (userStone - aiStone);
			}
			if (minUser > maxUser) {
				minUser = maxUser;
				setBestX(getX(i));
				setBestY(getY(i));
			}
		}
		defaultAll();
	}
};

/*
* User class
* Note : map�� ������� ���� ���� User Ŭ����
*/
class User {
	string name; //User�� �̸�
	int coin = 0; //User�� ���
	int winCnt = 0; //User�� �¸� Ƚ��
	int loseCnt = 0; //User�� �й� Ƚ��
	int drawCnt = 0; //User�� ���º� Ƚ��

public:

	/* User�� �ʱ�ȭ ������ */
	User(string name, int coin, int winCnt, int loseCnt, int drawCnt)
	{
		this->name = name; this->coin = coin; this->winCnt = winCnt; this->loseCnt = loseCnt; this->drawCnt = drawCnt;
	};
	User() {};

	/* User�� �̸��� ���� */
	void setName(string name) {
		this->name = name;
	}
	/* User�� ����� ���� */
	void setCoin(int coin) {
		this->coin = coin;
	}
	/* User�� �¸� Ƚ���� ���� */
	void setWinCnt(int winCnt) {
		this->winCnt = winCnt;
	}
	/* User�� �й� Ƚ���� ���� */
	void setLoseCnt(int loseCnt) {
		this->loseCnt = loseCnt;
	}
	/* User�� ���º� Ƚ���� ���� */
	void setDrawCnt(int drawCnt) {
		this->drawCnt = drawCnt;
	}
	/* User�� �̸��� �������� �Լ� */
	string getName() {
		return name;
	}
	/* User�� ����� �������� �Լ� */
	int getCoin() {
		return coin;
	}
	/* User�� �¸� Ƚ���� �������� �Լ� */
	int getWinCnt() {
		return winCnt;
	}
	/* User�� �й� Ƚ���� �������� �Լ� */
	int getLoseCnt() {
		return loseCnt;
	}
	/* User�� ���º� Ƚ���� �������� �Լ� */
	int getDrawCnt() {
		return drawCnt;
	}
};

/*������ �̸�, ���, ������ ����� �� ���̴� ������ �����ε� �Լ�*/
ostream& operator <<(ostream& os, User& user) {
	os << user.getName() << "   " << user.getCoin() << "     " << user.getWinCnt() << "    " << user.getLoseCnt() << "    " << user.getDrawCnt() << endl;
	return os;
}
/*
* FileManager class
* Note : txt���� ������ ������ list1�� �����ϴ� FileManager Ŭ����
*/
class FileManager {
public:
	vector<User> list1; //������ ����� �����ϴ� ����
	ifstream memberDataOut; //���� ����� �������� ����
	ofstream memberDataIn; //���� ����� �������� ����

	/* list1�� ����� ������ ���Ͽ� �����ϴ� �Լ� */
	void fileWrite(vector<User> list) {
		memberDataIn.open("Data.txt");
		for (unsigned int i = 0; i < list1.size(); i++) {
			if (i == list1.size() - 1)
				memberDataIn << list1.at(i).getName() << " " << list1.at(i).getCoin() << " " << list1.at(i).getWinCnt() << " " << list1.at(i).getLoseCnt() << " " << list1.at(i).getDrawCnt();
			else
				memberDataIn << list1.at(i).getName() << " " << list1.at(i).getCoin() << " " << list1.at(i).getWinCnt() << " " << list1.at(i).getLoseCnt() << " " << list1.at(i).getDrawCnt() << endl;
		}
		memberDataIn.close();
	}

	/* ������ ������ �� ����� list1�� �����ϴ� �Լ� */
	vector<User> fileRead() {
		User user;
		memberDataOut.open("Data.txt");
		char line[200];
		int index = 1; //������ 1�̸� �̸� 2�̸� ���� 3�̸� �¸� 4�̸� �й� ������ 0�̸� ���º�

		while (!memberDataOut.eof()) {
			if (index % 5 == 1) {
				memberDataOut >> line;
				index++;
				user.setName(line);
			}
			if (index % 5 == 2) {
				memberDataOut >> line;
				index++;
				user.setCoin(atoi(line));
			}
			if (index % 5 == 3) {
				memberDataOut >> line;
				index++;
				user.setWinCnt(atoi(line));
			}
			if (index % 5 == 4) {
				memberDataOut >> line;
				index++;
				user.setLoseCnt(atoi(line));
			}
			if (index % 5 == 0) {
				memberDataOut >> line;
				index++;
				user.setDrawCnt(atoi(line));
			}
			list1.push_back(user);
			continue;
		}
		memberDataOut.close();

		return list1;

	}

};

/*
* UserManager class
* Note : FileManager�� ������ �������� �۾��ϴ� UserManager Ŭ����
*/
class UserManager : public FileManager {
	bool isNew = false; //���ο� �������� Ȯ���ϴ� ����
public:

	/* isNew������ �����ϴ� �Լ� */
	void setIsNew(bool isNew)
	{
		this->isNew = isNew;
	}

	/* isNew������ �������� �Լ� */
	bool getIsNew() {
		return isNew;
	}

	/* list1���� �Ű������� �޴� name�� ������ Ȯ���ϴ� �Լ� */
	string search(string name) {
		string existName;
		for (unsigned int i = 0; i < list1.size(); i++) {
			existName = list1.at(i).getName();

			if (name == existName) {
				setIsNew(false);
				return name;

			}
		}
		setIsNew(true);
		return name;
	}

	/* �Ű������� �޴� name���� ���ο� ������ �����ϰ� �ʱ�ȭ�ϴ� �Լ� */
	void insert(string name) {
		User addUser;
		const int forBeginsCoin = 100;

		if (isNew) {
			addUser.setName(name);
			addUser.setCoin(forBeginsCoin);
			addUser.setWinCnt(0);
			addUser.setLoseCnt(0);
			addUser.setDrawCnt(0);
			list1.push_back(addUser);
			cout << name << "���� ���� ����� �Ϸ�Ǿ����ϴ�." << endl;
		}
		else
			cout << "���� �̸��� �����Ͽ� ���� ��Ͽ� �����Ͽ����ϴ�.";
	}

	/* ���Ͽ��� ���������͸� �а� list1�� �����ϴ� �Լ� */
	void memberInit() {
		list1 = fileRead();
	}

	/* list1�� ���������͸� ���Ͽ� �����ϴ� �Լ� */
	void memberRec() {
		fileWrite(list1);
	}

	/* list1�� ��� ������ ����ϴ� �Լ� */
	void memberPrint() {
		User user;
		cout << "     Name   Coin    win lose draw\n";
		for (unsigned int i = 0; i < list1.size(); i++) {
			user = list1.at(i);
			cout << i + 1 << " : ";
			cout << setw(6) << user;
		}
		cout << "\n";
	}

};

/*
* LoginManager class
* Note : �α��� ���� Ŭ����
*/

class LoginManager {
public:
	User user;
	UserManager usermanager;

	/* LoginManagerȣ�� �� list1�� �����͸� ������ �� �ִ� ������ */
	LoginManager() {
		usermanager.memberInit();
	}

	/* �α��� �޴��� ���� ����ϴ� �Լ� */
	void viewLoginMenu()
	{
		system("cls");
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��           (1) �α���              ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��           (2) ȸ�� ����           ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
	}

	/* �α��� �޴� �Լ� */
	void loginMenu() {
		string name;
		char input;
		bool isContinue = true;

		while (isContinue) {
			viewLoginMenu();
			input = _getch();
			if (input == '1') { //�α��� ����
				isContinue = login();
			}
			else if (input == '2') { //ȸ������ ����
				system("cls");
				cout << "������ �г����� �Է����ּ��� : ";
				cin >> name;
				usermanager.insert(usermanager.search(name));
				cout << "��� �� �α��� ȭ������ ���ư��ϴ�" << endl;
				Sleep(1000);
				cout << "\n3�� �� ���ư��ϴ�\n";
				Sleep(1000);
				cout << "\n2�� �� ���ư��ϴ�\n";
				Sleep(1000);
				cout << "\n1�� �� ���ư��ϴ�\n";
				Sleep(1000);
			}
		}
	}
	/* �α��� ������ �������� Ȯ�� �Լ� */
	User whoIsloginNow(vector<User> list, string name) {
		User loginUser;
		int index = 0;
		for (unsigned int i = 0; i < list.size(); i++) {

			if (name == list.at(i).getName()) { //�Է��� �̸��� ����Ʈ���� �̸��� ���Ͽ��� �Ǵ�
				index = i;
			}
			loginUser = list.at(index);
		}
		return loginUser; //user���� ��ȯ
	}

	/* �α����� �������� ���� ���� �Լ� */
	bool login() {
		string loginName;
		string addName;
		system("cls");
		cout << "�α��� �� �г����� �ۼ����ּ��� : ";
		cin >> loginName;
		user = whoIsloginNow(usermanager.list1, usermanager.search(loginName));
		if (usermanager.getIsNew()) {//���ο� �̸����� �Ǵ�
			cout << "ã���ô� �г����� �������� �ʽ��ϴ�." << endl;
			cout << "���ο� �г������� �����Ͻðڽ��ϱ�? (Y/N)";
			bool choice = true;
			while (choice) {
				char choose;
				cin >> choose;
				if (choose == 'Y' || choose == 'y') {
					usermanager.insert(loginName);//�̸� ���� ��������
					user = whoIsloginNow(usermanager.list1, usermanager.search(loginName));
					cout << "��� �� �α��� ȭ������ ���ư��ϴ�" << endl;
					Sleep(1000);
					cout << "\n3�� �� ���ư��ϴ�\n";
					Sleep(1000);
					cout << "\n2�� �� ���ư��ϴ�\n";
					Sleep(1000);
					cout << "\n1�� �� ���ư��ϴ�\n";
					Sleep(1000);
					return true;
				}
				else if (choose == 'N' || choose == 'n') {
					return true;
				}
				else
					cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			}
		}
		return false;
	}

};

/*
* GameSpace class
* Note : �α��� �޴����� ���� AI�� �������� ������ �����Ҽ��ִ� ������ �����ϴ� GameSpace class
*/

class GameSpace : public LoginManager {
	AI ai;
public:

	standardMap stdMap;
	transformMap trfMap;

	/* X, Y��ǥ �Է¹ް� �˻��ϴ� �Լ� */
	void inputAB(int player, char choose) {
		int inputA, inputB;
		int x, y;

		if (choose == '1') {
			cout << "(x y)�� �Է��ϼ��� : ";
			cin >> inputA >> inputB;
			y = inputA - 1; // �Է��ϴ� ���忡���� X,Y������ ���α׷��� ���忡�� for���� ���� Y,X�� �ٲپ���� �޾ƿ��� ���ϴٴ� ���� Ȱ������ ��ȯ
			x = inputB - 1;
			stdMap.putStone(x, y, player);
			stdMap.clearMap();
		}

		if (choose == '2') {
			cout << "(x y)�� �Է��ϼ��� : ";
			cin >> inputA >> inputB;
			y = inputA - 1;
			x = inputB - 1;
			trfMap.putStone(x, y, player);
			trfMap.clearMap();
		}
	}

	/* ���ӽ����Լ�*/
	void isStart(char choose) {

		int ExitNum = 1; //������ �����ϴ� ���� (1�̸� �ݺ�)
		char returnNum;
		int turnNum = 1;
		bool flag1 = true; //AI�� ���� ���ִ��� �Ǵ�
		bool flag2 = true; //user�� ���� ���ִ��� �Ǵ�


		// �Ϲݸ�
		if (choose == '1') {
			stdMap.defaultMap();
			stdMap.settingMap();
			while (ExitNum == 1) {
				// AI�� �� ���� ���� ��
				if (turnNum == 1) {
					stdMap.showingMap();//ai�� ���� �� ���� �����ִ� ��ġ
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��        �� ���� ���� �ֽ��ϴ�.     ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					stdMap.putCheck(turnNum);
					int bestX, bestY;
					ai.getPossibleMoves(stdMap);
					ai.smartcheck(stdMap);
					bestX = ai.getBestX();
					bestY = ai.getBestY();
					if (stdMap.pass() == false) {
						flag1 = false;//AI�� ���� �� ���� ��Ȳ

						if ((flag1 == false) && (flag2 == false))//AI�� ���� �� ���� ����� ���� �� ���� ��Ȳ �Ǵ�
						{
							cout << "�� �÷��̾� ��� ���� ���� �����Ƿ� ������ ����˴ϴ�." << endl;
							break;
						}
						cout << "AI�� ���� ���� �����Ƿ� ���� �Ѿ�ϴ�." << endl;
						Sleep(1000);
						turnNum++;//user������ �ѱ�
						continue;
					}
					flag1 = true;
					flag2 = true;
					stdMap.putStone(bestX, bestY, 1);
					stdMap.clearMap();
					Sleep(1000);
					stdMap.showingMap();//ai�� ��ǥ�� ���� ������ �� ������ִ� ��
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��        �� ���� ���� �ֽ��ϴ�.     ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "ai�� x = " << bestY + 1 << ",y = " << bestX + 1 << "�� ���ҽ��ϴ�.\n";
					Sleep(2000);                                                                 //2������ ��ٸ� �� ���������� �ѱ��
					turnNum++;//user������ �ѱ�
				}

				// user�� ���� ���� ���� ��
				else {
					stdMap.putCheck(turnNum);
					stdMap.showingMap();
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��       ���� ���� ���� �ֽ��ϴ�.    ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					if (stdMap.pass() == false) {
						flag2 = false; //������ ���� �� ���� ��Ȳ
						if ((flag1 == false) && (flag2 == false))//AI�� ���� �� ���� ����� ���� �� ���� ��Ȳ �Ǵ�
						{
							cout << "�� �÷��̾� ��� ���� ���� �����Ƿ� ������ ����˴ϴ�." << endl;
							break;
						}
						cout << "User�� ���� ���� �����Ƿ� ���� �Ѿ�ϴ�." << endl;
						Sleep(1000);
						turnNum--;//AI������ �ѱ�
						continue;
					}
					flag1 = true;
					flag2 = true;
					inputAB(turnNum, choose);
					turnNum--;//AI������ �ѱ�
				}
			}
		}

		// ���� ��
		else if (choose == '2') {
			trfMap.defaultMap();
			trfMap.settingMap();
			while (ExitNum == 1) {
				// AI�� �� ���� ���� ��
				if (turnNum == 1) {
					trfMap.showingMap();
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��        �� ���� ���� �ֽ��ϴ�.     ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					trfMap.putCheck(turnNum);
					int bestX, bestY;
					ai.getPossibleMoves(trfMap);
					ai.smartcheck(trfMap);
					bestX = ai.getBestX();
					bestY = ai.getBestY();
					if (trfMap.pass() == false) {
						flag1 = false;//AI�� ���� �� ���� ��Ȳ

						if ((flag1 == false) && (flag2 == false))//AI�� ���� �� ���� ����� ���� �� ���� ��Ȳ �Ǵ�
						{
							cout << "�� �÷��̾� ��� ���� ���� �����Ƿ� ������ ����˴ϴ�." << endl;
							break;
						}
						cout << "AI�� ���� ���� �����Ƿ� ���� �Ѿ�ϴ�." << endl;
						Sleep(1000);
						turnNum++;//user������ �ѱ�
						continue;
					}
					flag1 = true;
					flag2 = true;
					trfMap.putStone(bestX, bestY, 1);
					trfMap.clearMap();
					Sleep(1000);
					trfMap.showingMap();
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��        �� ���� ���� �ֽ��ϴ�.     ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "ai�� x = " << bestY + 1 << ",y = " << bestX + 1 << "�� ���ҽ��ϴ�.\n";
					Sleep(2000);
					turnNum++;//user������ �ѱ�
				}

				// user�� ���� ���� ���� ��
				else {
					trfMap.putCheck(turnNum);
					trfMap.showingMap();
					cout << "��������������������������������������������������������������������������" << endl;
					cout << "��       ���� ���� ���� �ֽ��ϴ�.    ��" << endl;
					cout << "��������������������������������������������������������������������������" << endl;
					if (trfMap.pass() == false) {
						flag2 = false; //������ ���� �� ���� ��Ȳ
						if ((flag1 == false) && (flag2 == false))//AI�� ���� �� ���� ����� ���� �� ���� ��Ȳ �Ǵ�
						{
							cout << "�� �÷��̾� ��� ���� ���� �����Ƿ� ������ ����˴ϴ�." << endl;
							break;
						}
						cout << "User�� ���� ���� �����Ƿ� ���� �Ѿ�ϴ�." << endl;
						Sleep(1000);
						turnNum--;//AI������ �ѱ�
						continue;
					}
					flag1 = true;
					flag2 = true;
					inputAB(turnNum, choose);
					turnNum--;//AI������ �ѱ�
				}
			}
		}
		result(choose);
		cout << "Ȩȭ������ ���ư��÷��� ���� 1�� �����ּ���";
		returnNum = _getch();//1�� ������ �ѱ�� �ֵ��� �ϴ� ����
		if (returnNum == '1')
		{
			cout << "\n5�� �� ���ư��ϴ�\n";
			Sleep(1000);
			cout << "\n4�� �� ���ư��ϴ�\n";
			Sleep(1000);
			cout << "\n3�� �� ���ư��ϴ�\n";
			Sleep(1000);
			cout << "\n2�� �� ���ư��ϴ�\n";
			Sleep(1000);
			cout << "\n1�� �� ���ư��ϴ�\n";
			Sleep(1000);
			stdMap.defaultMap();//�Ϲݸ� �ʱ�ȭ
			trfMap.defaultMap();//������ �ʱ�ȭ
			stdMap.settingMap();//�Ϲݸ� ����
			trfMap.settingMap();//������ ����
		}
		else {
			;
		}

	}
	/*�º� ��� �Ǵ� �Լ�*/
	void result(char choose) {
		int resultNum; // �¸� ��� �Ǵ� ����
		resultNum = checkExit(choose);
		updateCoin(resultNum);
		updateRecord(resultNum);

		if (resultNum == 2) {//������ �¸�
			cout << "��������������������������������������������������������������������������" << endl;
			cout << "��        �� ���� �� �¸� ��         ��" << endl;
			cout << "��        �� ���� �� �¸� ��         ��" << endl;
			cout << "��        �� ���� �� �¸� ��         ��" << endl;
			cout << "��        �� ���� �� �¸� ��         ��" << endl;
			cout << "��        �� ���� �� �¸� ��         ��" << endl;
			cout << "��������������������������������������������������������������������������" << endl;
		}

		else if (resultNum == 3) {//�� �¸�
			cout << "��������������������������������������������������������������������������" << endl;
			cout << "��          �� �� �� �¸� ��         ��" << endl;
			cout << "��          �� �� �� �¸� ��         ��" << endl;
			cout << "��          �� �� �� �¸� ��         ��" << endl;
			cout << "��          �� �� �� �¸� ��         ��" << endl;
			cout << "��          �� �� �� �¸� ��         ��" << endl;
			cout << "��������������������������������������������������������������������������" << endl;
		}
		else if (resultNum == 4) {//���º�
			cout << "��������������������������������������������������������������������������" << endl;
			cout << "��           �� �� �� �� ��          ��" << endl;
			cout << "��           �� �� �� �� ��          ��" << endl;
			cout << "��           �� �� �� �� ��          ��" << endl;
			cout << "��           �� �� �� �� ��          ��" << endl;
			cout << "��           �� �� �� �� ��          ��" << endl;
			cout << "��������������������������������������������������������������������������" << endl;
		}
	}

	/*������ ��� ������Ʈ �Լ�*/
	void updateCoin(int resultNum)
	{
		int index = 0;
		for (unsigned int i = 0; i < usermanager.list1.size(); i++) {
			if (user.getName() == usermanager.list1.at(i).getName()) { //����Ʈ ������ �α��� �������� �Ǵ�
				index = i;
				break;
			}
		}

		if (resultNum == 2) //���� ���� �����̹Ƿ� �̰��� �� +50
		{
			usermanager.list1.at(index).setCoin(user.getCoin() + 50);
		}
		else if (resultNum == 3) //�� ���� �����̹Ƿ� ���� �� -20
		{
			usermanager.list1.at(index).setCoin(user.getCoin() - 20);
		}
		else //������ ��Ȳ(���º�)�� ��� ��ȭ ����
		{
			;
		}
	}

	/*������ ���� ������Ʈ �Լ�*/
	void updateRecord(int resultNum)
	{
		int index = 0;
		for (unsigned int i = 0; i < usermanager.list1.size(); i++) {
			if (user.getName() == usermanager.list1.at(i).getName()) { //����Ʈ ������ �α��� �������� �Ǵ�
				index = i;
				break;
			}
		}
		if (resultNum == 2)//���� ���� �����̹Ƿ� �̰��� �� �¸�Ƚ�� ����
		{
			usermanager.list1.at(index).setWinCnt(user.getWinCnt() + 1);

		}
		else if (resultNum == 3)//�� ���� �����̹Ƿ� ���� �� �й�Ƚ�� ����
		{
			usermanager.list1.at(index).setLoseCnt(user.getLoseCnt() + 1);

		}
		else if (resultNum == 4)// ���º��� �� ���º� Ƚ�� ����
		{
			usermanager.list1.at(index).setDrawCnt(user.getDrawCnt() + 1);
		}
	}

	/*������ ������ ���� ��� �Ǵ��ϴ� �Լ�*/
	int checkExit(char choose) {
		int whiteStoneCnt = 0; // 1���� �� ��
		int blackStoneCnt = 0; // 2���� ���� ��
		int notPutBlock = 0;   // 3���� ���� ���� �� ���� ��ġ
		int zeroCnt = 0;

		if (choose == '1') {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (stdMap.getMap(i, j) == 0) {// �ش�ĭ�� ����ִ� ĭ���� �Ǵ�
						zeroCnt++;
					}
					else if (stdMap.getMap(i, j) == 1) {// �ش�ĭ�� �� ���� �������ִ� ĭ���� �Ǵ�
						whiteStoneCnt++;
					}
					else if (stdMap.getMap(i, j) == 2) {// �ش�ĭ�� ���� ���� �������ִ� ĭ���� �Ǵ�
						blackStoneCnt++;
					}
				}
			}

			if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt > whiteStoneCnt)) {//�� �� ������ ���� �� ������ ����ִ� ĭ�� ���� 64���̸� ���� ���� �� ������ ���� ���
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 2;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt < whiteStoneCnt)) {//�� �� ������ ���� �� ������ ����ִ� ĭ�� ���� 64���̸� �� ���� ���� ������ ���� ���
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 3;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt == whiteStoneCnt)) {//�� �� ������ ���� �� ������ ����ִ� ĭ�� ���� 64���̸� �� ���� ���� ���� ���� ���
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 4;
			}
			else {
				return 1; //������ ��� ��� ����
			}
		}
		//���� ��İ� ����
		else if (choose == '2') {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (trfMap.getMap(i, j) == 0) {
						zeroCnt++;
					}
					else if (trfMap.getMap(i, j) == 1) {
						whiteStoneCnt++;
					}
					else if (trfMap.getMap(i, j) == 2) {
						blackStoneCnt++;
					}
					else if (trfMap.getMap(i, j) == 3) {
						notPutBlock++;
					}
				}
			}

			if (((zeroCnt + blackStoneCnt + whiteStoneCnt + notPutBlock) == (sizeof(trfMap.map) / sizeof(int))) && (blackStoneCnt > whiteStoneCnt)) {
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 2;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt + notPutBlock) == (sizeof(trfMap.map) / sizeof(int))) && (blackStoneCnt < whiteStoneCnt)) {
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 3;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt + notPutBlock) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt == whiteStoneCnt)) {
				cout << "���� ���� ���� : " << blackStoneCnt << endl;
				cout << "�� ���� ���� : " << whiteStoneCnt << endl;
				return 4;
			}
			else {
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	/*�α��� ���� �Լ�*/
	void isNowLogin()
	{
		loginMenu();
	}
	/*���ӽ��ۿ��θ� �����ִ� �Լ�*/
	void isChooseGame() {
		viewlogo();
		cout << "			(1)���� ����" << endl;
		cout << "\n";
		cout << "			(2)���� ����\n" << endl;
	}
	/*������ �ΰ� �����ִ� �Լ�*/
	void viewlogo() {
		system("cls");
		cout << "\n";
		cout << " �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�" << endl;
		cout << " �ۡۡܡܡܡۡۡۡۡܡۡۡۡܡۡۡۡۡۡۡۡۡۡۡۡܡۡܡۡۡۡۡۡۡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡۡۡۡۡۡۡۡۡۡۡܡۡܡۡۡۡۡۡۡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡܡܡܡܡܡۡܡۡۡۡۡۡۡۡۡۡۡۡܡۡܡۡۡۡۡۡۡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡۡۡۡۡۡܡܡܡۡۡܡۡܡۡۡܡܡܡۡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡܡܡܡۡۡܡۡۡۡܡۡܡۡܡۡܡۡۡۡܡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡܡۡۡۡܡۡܡۡܡۡܡۡۡۡܡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡܡܡܡܡۡۡܡۡܡۡܡۡۡۡܡ�" << endl;
		cout << " �ۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡۡۡܡۡܡۡۡۡۡۡܡۡܡۡܡۡۡۡܡ�" << endl;
		cout << " �ۡۡܡܡܡۡۡۡۡܡۡۡۡܡۡۡۡܡۡۡܡܡܡܡۡܡۡܡۡۡܡܡܡۡ�" << endl;
		cout << " �ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�" << endl;
		cout << "\n\n";
	}

	/*������ �̸��� ����, ���� �޴��� �����ִ� �Լ�*/
	void viewMain() {
		user = whoIsloginNow(usermanager.list1, usermanager.search(user.getName()));
		system("cls");
		cout << "\n";
		cout << "	�г��� : " << user.getName() << "              " << endl;
		cout << "	[����]             " << endl;
		cout << "	Coin : " << user.getCoin() << endl;
		cout << "	Win : " << user.getWinCnt() << "   Lose : " << user.getLoseCnt() << "   Draw : " << user.getDrawCnt() << endl;
		cout << "\n\n";
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��           (1) �� �����           ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��           (2) ��� ���           ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��           (3) ���� ����           ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;

	}

	/*�� ���� �޴��� �����ִ� �Լ�*/
	void isChooseMap() {

		system("cls");
		cout << "��������������������������������������������������������������������������" << endl;
		cout << "��         ���� �������ּ���!        ��" << endl;
		cout << "��        \x1b[31m(���ڸ� �����ּ���)\x1b[0m        ��" << endl;
		cout << "��                                   ��" << endl;
		cout << "��            (1) �Ϲ� ��            ��" << endl;
		cout << "��            (2) ���� ��            ��" << endl;
		cout << "��������������������������������������������������������������������������" << endl;

	}
};

int main() {

	char startNum; //���ۿ��� ���� ����
	char menuChoice; //�޴� ���� ���� ����
	char choose; //�� ���� ���� ����
	GameSpace gamespace; //���ӽ����̽� ��ü ����
	gamespace.isChooseGame();
	cout << "\t\t\t";
	startNum = _getch();
	if (startNum == '1') //���� ���� Ȯ��
	{
		gamespace.isNowLogin();

		while (1)
		{
			gamespace.viewMain();
			cout << "\t\t   ";
			menuChoice = _getch();
			if (menuChoice == '1') { //�� ���� Ȯ��
				gamespace.isChooseMap();
				cout << "\t\t    \n";
				choose = _getch();
				if (choose == '1' || choose == '2') {//�Ϲ� �� �Ǵ� ������ ����
					gamespace.isStart(choose);
				}
				else {
					cout << "�ٽ� �Է����ּ���";
					gamespace.isChooseMap();
					cout << "\t\t    ";
				}
			}
			else if (menuChoice == '2') //���� ��ϵ� ��ü Ȯ��
			{
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n3�� �� ���ư��ϴ�";
				Sleep(1000);
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n2�� �� ���ư��ϴ�";
				Sleep(1000);
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n1�� �� ���ư��ϴ�";
				Sleep(1000);
			}
			else if (menuChoice == '3') //���� ���� ���� Ȯ��
			{
				break;
			}
			else
			{
				continue;
			}
			gamespace.usermanager.memberRec();

		}

	}
	return 0;

}