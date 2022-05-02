#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <string>
#include <windows.h>
#include <vector>
#include <iomanip>


#define MAP_SIZE 8 //맵의 크기(8x8)를 설정하기 위한 상수

using namespace std;

/*
* SettingMap class
* Note: 일반맵, 변형맵에서 공통으로 사용하는 맵설정 가상함수를 가지고있는 클래스
*/
class SettingMap {
public:
	virtual void settingMap() = 0;

};

/*
* ShowingMap class
* Note: 일반맵, 변형맵에서 공통으로 사용하는 맵출력 가상함수를 가지고있는 클래스
*/
class ShowingMap {

	virtual void showingMap() = 0;
};

/*
* Map class
* Note: 맵의 기본적인 기능을 사용하기 위한 일반맵, 변형맵의 부모클래스
*/
class Map : public SettingMap, public ShowingMap {
public:
	int map[MAP_SIZE][MAP_SIZE] = { 0 }; //맵 선언
	int isVisit[MAP_SIZE][MAP_SIZE] = { 0 }; //돌을 바꾸기 위해 맵을 방문하는 변수 (방문시 1로 변경)
	int dx[MAP_SIZE] = { -1, 1, 0, 0, -1, -1, 1, 1 }; // 좌 ,우, 위, 아래, 좌하, 좌상, 우하, 우상으로 이동시 x의 변화
	int dy[MAP_SIZE] = { 0, 0, -1, 1, -1, 1, -1, 1 }; // 좌, 우 , 위, 아래, 좌하, 좌상, 우하, 우상으로 이동시 y의 변화

	virtual void settingMap() {};
	virtual void showingMap() {};

	/* 맵을 0으로 초기화하는 함수 */
	void defaultMap()
	{
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				map[i][j] = 0;
			}
		}
	}

	/* x, y, player를 기반으로 맵 설정하는 함수 */
	void setMap(int x, int y, int player) {
		// player1인 경우
		if (player == 1) {
			map[x][y] = 1;
		}
		// player2인 경우
		else {
			map[x][y] = 2;
		}
	}

	/* x, y의 맵좌표를 가져오는 함수 */
	int getMap(int x, int y) {
		return map[x][y];
	}

	/*돌을 놓았을 때 바뀌는지 확인하는 함수*/
	bool changeCheck(int x, int y, int player, int direction) {
		bool result = false; //돌을 바꾸는지 확인하는 변수
		int xx = x;
		int yy = y;
		isVisit[xx][yy] = 1;
		if (player == 1) {
			while (1) {
				xx += dx[direction];
				yy += dy[direction];
				if (xx < 0 || yy < 0 || xx >= MAP_SIZE || yy >= MAP_SIZE) { //맵을 벗어나는 경우
					break;
				}
				if (isVisit[xx][yy] == 1 || map[xx][yy] == 0 || map[xx][yy] == 3) { //이미 방문체크 완료한 경우, 돌이 없는 곳일 경우, 장애물로 인해 놓을 수 없는 경우
					break;
				}
				if (map[xx][yy] == 1) { //돌을 바꿀 수 있는 경우
					result = true;
					break;
				}
				isVisit[xx][yy] = 1; //방문 시 1로 바뀜
			}
		}
		else {
			while (1) {
				xx += dx[direction];
				yy += dy[direction];
				if (xx < 0 || yy < 0 || xx >= MAP_SIZE || yy >= MAP_SIZE) {//맵을 벗어나는 경우
					break;
				}
				if (isVisit[xx][yy] == 1 || map[xx][yy] == 0 || map[xx][yy] == 3) { //이미 방문체크 완료한 경우, 돌이 없는 곳일 경우, 장애물로 인해 놓을 수 없는 경우
					break;
				}
				if (map[xx][yy] == 2) { //돌을 바꿀 수 있는 경우
					result = true;
					break;
				}
				isVisit[xx][yy] = 1; //방문 시 1로 바뀜
			}
		}
		return result;
	}

	/* 돌을 뒤집는 함수 */
	void change(int player) {
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (player == 1 && isVisit[i][j]) { //player가 1이면서 뒤집기가 가능한 곳일 경우
					if (map[i][j] != 4 && map[i][j] != 3) { //놓을 수 있는 위치표시된 곳이 아니면서 장애물위치에 표시된 곳이 아닐 경우
						map[i][j] = 1;
					}
				}
				else if (player == 2 && isVisit[i][j]) { //player가 2이면서 뒤집기가 가능한 곳일 경우
					if (map[i][j] != 4 && map[i][j] != 3) //놓을 수 있는 위치표시된 곳이 아니면서 장애물위치에 표시된 곳이 아닐 경우
					{
						map[i][j] = 2;
					}
				}
			}
		}
	}

	/*유효한 자리인지 검사해주는 함수*/
	bool isVaildSpace(int x, int y) {
		if (getMap(x, y) == 0 || getMap(x, y) == 4) { //x, y의 위치가 비어있거나 놓을 수 있다고 표시된 경우
			return true;
		}
		else if (x > MAP_SIZE || y > MAP_SIZE) { //x와 y가 최대범위를 벗어난 경우
			return false;
		}
		else if (x < 0 || y < 0) { //x와 y가 최소범위를 벗어난 경우
			return false;
		}
		else //유저나 ai가 이미 돌을 놓은 위치인 경우
		{
			return false;
		}
	}

	/*놓을 수 있는 위치 정보를 갱신*/
	void clearMap() {
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (getMap(i, j) == 4) {
					map[i][j] = 0;
				}
			}
		}
	}

	/*돌을 놓을 수 있는지 확인*/
	void putCheck(int player) {
		int ii, jj;
		if (player == 1) {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (getMap(i, j) == 1) {
						// 상단 검사
						ii = 1;
						while (i - ii >= 0) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 하단 검사
						ii = 1;
						while (i + ii < MAP_SIZE) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 왼쪽 검사
						jj = 1;
						while (j - jj >= 0) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 오른쪽 검사
						jj = 1;
						while (j + jj < MAP_SIZE) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (상좌)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j - jj >= 0)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (하우)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j + jj < MAP_SIZE)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (하좌)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j - jj >= 0)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (상우)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j + jj < MAP_SIZE)) {
							// 0일 때 놓을 수 있는 위치로 변경
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
						// 상단 검사
						ii = 1;
						while (i - ii >= 0) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 하단 검사
						ii = 1;
						while (i + ii < MAP_SIZE) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 왼쪽 검사
						jj = 1;
						while (j - jj >= 0) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 오른쪽 검사
						jj = 1;
						while (j + jj < MAP_SIZE) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (상좌)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j - jj >= 0)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (하우)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j + jj < MAP_SIZE)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (하좌)
						ii = 1;
						jj = 1;
						while ((i + ii < MAP_SIZE) && (j - jj >= 0)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

						// 대각선 확인 (상우)
						ii = 1;
						jj = 1;
						while ((i - ii >= 0) && (j + jj < MAP_SIZE)) {
							// 0일 때 놓을 수 있는 위치로 변경
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

	/* 돌을 놓을 수 있는지 판단하는 함수 */
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

	/* 돌을 놓았을 때의 실행하는 함수 */
	void putStone(int x, int y, int player) {
		int recallA, recallB;

		if (putCanStone(x, y, player) && isVaildSpace(x, y)) { //놓을 수 있고, 유효한 자리일 경우
			swapStone(x, y, player);
			setMap(x, y, player);
		}
		else {
			showingMap();
			if (player == 2)
			{
				cout << "┌───────────────────────────────────┐" << endl;
				cout << "│       검은 돌이 놓고 있습니다.    │" << endl;
				cout << "└───────────────────────────────────┘" << endl;
				cout << "놓을 수 없는 위치입니다. 다시 입력해주세요 !" << endl;
				cout << "(x y)를 입력하세요 : ";
				cin >> recallA >> recallB;
				y = recallA - 1;
				x = recallB - 1;
				putStone(x, y, player);
			}

		}
	}

	/* 돌을 놓았을 때 두개의 돌 사이의 상대방의 돌을 내 돌로 바뀌게하는 함수 */
	void swapStone(int x, int y, int player) {
		for (int i = 0; i < MAP_SIZE; i++) {

			memset(isVisit, 0, sizeof(isVisit)); //isVisit의 메모리를 0으로 초기화해주는 함수
			if (changeCheck(x, y, player, i)) {
				change(player);
			}
		}
	}

	/* 놓을 곳이 없는 경우 턴이 넘어가는 함수 */
	bool pass() {
		bool result = true;
		int passNum = 0; //놓을 수 있는 위치의 개수를 확인하는 변수

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
* Note: 맵의 기본적인 기능 + 일반맵에서의 기능을 사용하기 위한 클래스
*/

class standardMap : public Map {
public:

	/* 호출과 동시에 일반맵을 설정하는 생성자 */
	standardMap()
	{
		settingMap();
	}

	/* 일반맵의 초기값 설정 */
	virtual void settingMap() {
		map[3][3] = 1; // 흰돌을 표시하기 위해서
		map[4][4] = 1;
		map[3][4] = 2; // 검은돌 표시하기위해서
		map[4][3] = 2;
	}

	/* 일반맵을 출력해주는 함수 */
	virtual void showingMap() {
		system("cls");
		cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
		cout << "│   │ ①│ ②│ ③│ ④│ ⑤│ ⑥│ ⑦│ ⑧│" << endl;
		cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		for (int i = 0; i < MAP_SIZE; i++) {
			if (i == 0)cout << "│ ①";
			else if (i == 1)cout << "│ ②";
			else if (i == 2)cout << "│ ③";
			else if (i == 3)cout << "│ ④";
			else if (i == 4)cout << "│ ⑤";
			else if (i == 5)cout << "│ ⑥";
			else if (i == 6)cout << "│ ⑦";
			else if (i == 7)cout << "│ ⑧";
			for (int j = 0; j < MAP_SIZE; j++) {
				if (map[i][j] == 0)
					cout << "│   ";
				else if (map[i][j] == 1)
					cout << "│ ●";
				else if (map[i][j] == 2)
					cout << "│ ○";
				else if (map[i][j] == 4)
					cout << "│ \x1b[32m◎\x1b[0m";
			}
			cout << "│\n";
			if (i < 7)
				cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
		cout << "└───┴───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
	}
};

/*
* transformMap class
* Note: 맵의 기본적인 기능 + 변형맵에서의 기능을 사용하기 위한 클래스
*/
class transformMap : public Map {

public:

	/* 호출과 동시에 변형맵을 설정하는 생성자 */
	transformMap()
	{
		settingMap();
	}

	/* 변형맵의 초기값 설정 */
	virtual void settingMap() {
		map[3][3] = 1;
		map[4][4] = 1;
		map[3][4] = 2;
		map[4][3] = 2;

		int row[2] = { 8,8 };
		int columns[2] = { 8,8 }; // 최대한 서로 다른 행과 열에 막혀있도록 만들기 위해서이며 3개칸만 막을 예정
		srand(static_cast<unsigned int>(time(NULL))); // C언어에서 srand(time(NULL))하듯 한 과정이라고 생각
		int count = 0;
		while (count < 2) {
			int num1 = rand() % 7;
			int num2 = rand() % 7; // 0부터 7까지 칸 지정
			int flag = 0;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					if (row[i] == num1 || columns[j] == num2) // 기존에 있는 행과 열에서 비교하기 위해서 만듦
						flag = 1; // 기존행또는 열이 있다면 1, 없다면 0
			}
			if (flag == 0) {
				row[count] = num1; // 행이 겹쳐지는 것을 방지하고자 그 행 저장
				columns[count] = num2; // 열 또한 그러하게 작용
				if (map[num1][num2] == 0) { // 그러나 처음 시작할 때 1,2가 있을 수 있기 때문에 그런것을 막고자 작성
					map[num1][num2] = 3;
					count += 1;
				}
			}
		}
	}

	/* 변형맵을 출력해주는 함수 */
	virtual void showingMap() {
		system("cls");
		cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
		cout << "│   │ ①│ ②│ ③│ ④│ ⑤│ ⑥│ ⑦│ ⑧│" << endl;
		cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		for (int i = 0; i < MAP_SIZE; i++) {
			if (i == 0)cout << "│ ①";
			else if (i == 1)cout << "│ ②";
			else if (i == 2)cout << "│ ③";
			else if (i == 3)cout << "│ ④";
			else if (i == 4)cout << "│ ⑤";
			else if (i == 5)cout << "│ ⑥";
			else if (i == 6)cout << "│ ⑦";
			else if (i == 7)cout << "│ ⑧";
			for (int j = 0; j < MAP_SIZE; j++) {
				if (map[i][j] == 0)
					cout << "│   ";
				else if (map[i][j] == 1)
					cout << "│ ●";
				else if (map[i][j] == 2)
					cout << "│ ○";
				else if (map[i][j] == 3)
					cout << "│ \x1b[31m▩\x1b[0m";
				else if (map[i][j] == 4)
					cout << "│ \x1b[32m◎\x1b[0m";
			}
			cout << "│\n";
			if (i < 7)
				cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
		cout << "└───┴───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
	}
};

/*
* AI class
* Note : map을 기반으로 돌을 놓는 ai 클래스
*/
class AI {
	int x[60] = { 0 }, y[60] = { 0 };
	int check = 0;
	int bestChoice_X = 0, bestChoice_Y = 0;
public:

	/* ai의 x를 설정하는 함수 */
	void setX(int check, int i) {
		x[check] = i;
	}
	/* ai의 y를 설정하는 함수 */
	void setY(int check, int i) {
		y[check] = i;
	}
	/* ai의 bestChoice_X를 설정하는 함수 */
	void setBestX(int i)
	{
		bestChoice_X = i;
	}
	/* ai의 bestChoice_Y를 설정하는 함수 */
	void setBestY(int i)
	{
		bestChoice_Y = i;
	}
	/* ai의 x를 받아오는 함수 */
	int getX(int i) {
		return x[i];
	}
	/* ai의 y를 받아오는 함수 */
	int getY(int i) {
		return y[i];
	}
	/* ai의 bestChoice_X를 받아오는 함수 */
	int getBestX()
	{
		return bestChoice_X;
	}
	/* ai의 bestChoice_Y를 받아오는 함수 */
	int getBestY()
	{
		return bestChoice_Y;
	}

	/* ai의 x, y좌표를 초기화 */
	void defaultAll()
	{
		for (int i = 0; i < 60; i++) {
			x[i] = 0;
			y[i] = 0;
		}
		check = 0;
	}

	/* 일반맵에서 가능한 지점을 설정 */
	void getPossibleMoves(standardMap map) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (map.getMap(i, j) == 4) {
					setX(check, i);
					setY(check, j);
					check++;
				}
	}

	/* 변환맵에서 가능한 지점을 설정 */
	void getPossibleMoves(transformMap map) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (map.getMap(i, j) == 4) {
					setX(check, i);
					setY(check, j);
					check++;
				}
	}

	/* 일반맵에서 AI가 돌을 놓을 수 있는지 확인하고 최적의 bestChoice_X와 bestChoice_Y를 설정 */
	void smartcheck(standardMap map) {
		int minUser = 99999; //사람이 놓았을 때 가장 적게 뒤집을 수 있는 돌
		for (int i = 0; i < check; i++) {
			standardMap newmap1; //AI가 경우의 수를 보기 위해 가상의 맵을 선언
			newmap1 = map; //가상의 맵을 지금 현재의 맵으로 복사
			newmap1.map[getX(i)][getY(i)] = 1; //AI가 놓을 수 있는 위치에 돌 놓기
			newmap1.swapStone(getX(i), getY(i), 1); //AI가 놓은 돌로 인한 변화
			newmap1.clearMap(); //ai가 놓을 수 있었던 자리 초기화
			newmap1.putCheck(2); //사람이 놓을 수 있는 경우 예측
			int newX[60] = { 0 }, newY[60] = { 0 };
			int newCheck = 0;
			for (int a = 0; a < 8; a++)
				for (int b = 0; b < 8; b++)
					if (newmap1.getMap(a, b) == 4) {
						newX[newCheck] = a;
						newY[newCheck] = b;
						newCheck++;
					}
			int maxUser = -99999; //사람이 놓았을 때 가장 많이 뒤집을 수 있는 돌
			for (int j = 0; j < newCheck; j++) {
				standardMap newmap2; //AI가 경우의 수를 보기 위해 가상의 맵을 선언
				newmap2 = newmap1; //가상의 맵을 지금 현재의 맵으로 복사
				newmap2.map[getX(j)][getY(j)] = 1; //AI가 놓을 수 있는 위치에 돌 놓기
				newmap2.swapStone(getX(j), getY(j), 1); //AI가 놓은 돌로 인한 변화
				int aiStone = 0; //ai가 놓을 수 있었던 자리 초기화
				int userStone = 0; //사람이 놓을 수 있는 경우 예측
				for (int k = 0; k < 8; k++) {
					for (int m = 0; m < 8; m++) {
						if (newmap2.map[k][m] == 1)
							aiStone++;
						else if (newmap2.map[k][m] == 2)
							userStone++;
					}
				}
				if (maxUser < (userStone - aiStone)) //가장 많이 뒤집을 수 있는 경우보다 더 많이 뒤집을 수 있는 경우
					maxUser = (userStone - aiStone);
			}
			if (minUser > maxUser) { //가장 적게 뒤집을 수 있는 경우보다 더 적게 뒤집을 수 있는 경우
				minUser = maxUser;
				setBestX(getX(i)); //최적의 위치 bestChoice_X를 설정
				setBestY(getY(i)); //최적의 위치 bestChoice_Y를 설정
			}
		}
		defaultAll();
	}

	/* 변환맵에서 AI가 돌을 놓을 수 있는지 확인하고 최적의 bestChoice_X와 bestChoice_Y를 설정 (설명은 위의 방식와 동일) */
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
* Note : map을 기반으로 돌을 놓는 User 클래스
*/
class User {
	string name; //User의 이름
	int coin = 0; //User의 재산
	int winCnt = 0; //User의 승리 횟수
	int loseCnt = 0; //User의 패배 횟수
	int drawCnt = 0; //User의 무승부 횟수

public:

	/* User의 초기화 생성자 */
	User(string name, int coin, int winCnt, int loseCnt, int drawCnt)
	{
		this->name = name; this->coin = coin; this->winCnt = winCnt; this->loseCnt = loseCnt; this->drawCnt = drawCnt;
	};
	User() {};

	/* User의 이름을 설정 */
	void setName(string name) {
		this->name = name;
	}
	/* User의 재산을 설정 */
	void setCoin(int coin) {
		this->coin = coin;
	}
	/* User의 승리 횟수를 설정 */
	void setWinCnt(int winCnt) {
		this->winCnt = winCnt;
	}
	/* User의 패배 횟수를 설정 */
	void setLoseCnt(int loseCnt) {
		this->loseCnt = loseCnt;
	}
	/* User의 무승부 횟수를 설정 */
	void setDrawCnt(int drawCnt) {
		this->drawCnt = drawCnt;
	}
	/* User의 이름을 가져오는 함수 */
	string getName() {
		return name;
	}
	/* User의 재산을 가져오는 함수 */
	int getCoin() {
		return coin;
	}
	/* User의 승리 횟수를 가져오는 함수 */
	int getWinCnt() {
		return winCnt;
	}
	/* User의 패배 횟수를 가져오는 함수 */
	int getLoseCnt() {
		return loseCnt;
	}
	/* User의 무승부 횟수를 가져오는 함수 */
	int getDrawCnt() {
		return drawCnt;
	}
};

/*유저의 이름, 재산, 전적을 출력할 때 쓰이는 연산자 오버로딩 함수*/
ostream& operator <<(ostream& os, User& user) {
	os << user.getName() << "   " << user.getCoin() << "     " << user.getWinCnt() << "    " << user.getLoseCnt() << "    " << user.getDrawCnt() << endl;
	return os;
}
/*
* FileManager class
* Note : txt파일 내용을 가지고 list1에 저장하는 FileManager 클래스
*/
class FileManager {
public:
	vector<User> list1; //유저의 목록을 저장하는 변수
	ifstream memberDataOut; //유저 목록을 가져오는 변수
	ofstream memberDataIn; //유저 목록을 내보내는 변수

	/* list1에 저장된 내용을 파일에 저장하는 함수 */
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

	/* 파일을 연결한 후 목록을 list1에 저장하는 함수 */
	vector<User> fileRead() {
		User user;
		memberDataOut.open("Data.txt");
		char line[200];
		int index = 1; //나머지 1이면 이름 2이면 코인 3이면 승리 4이면 패배 나머지 0이면 무승부

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
* Note : FileManager의 내용을 바탕으로 작업하는 UserManager 클래스
*/
class UserManager : public FileManager {
	bool isNew = false; //새로운 유저인지 확인하는 변수
public:

	/* isNew변수를 설정하는 함수 */
	void setIsNew(bool isNew)
	{
		this->isNew = isNew;
	}

	/* isNew변수를 가져오는 함수 */
	bool getIsNew() {
		return isNew;
	}

	/* list1에서 매개변수로 받는 name과 같은지 확인하는 함수 */
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

	/* 매개변수로 받는 name으로 새로운 유저를 생성하고 초기화하는 함수 */
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
			cout << name << "님의 유저 등록이 완료되었습니다." << endl;
		}
		else
			cout << "같은 이름이 존재하여 유저 등록에 실패하였습니다.";
	}

	/* 파일에서 유저데이터를 읽고 list1에 저장하는 함수 */
	void memberInit() {
		list1 = fileRead();
	}

	/* list1의 유저데이터를 파일에 저장하는 함수 */
	void memberRec() {
		fileWrite(list1);
	}

	/* list1의 모든 유저를 출력하는 함수 */
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
* Note : 로그인 관련 클래스
*/

class LoginManager {
public:
	User user;
	UserManager usermanager;

	/* LoginManager호출 시 list1의 데이터를 접근할 수 있는 생성자 */
	LoginManager() {
		usermanager.memberInit();
	}

	/* 로그인 메뉴의 탭을 출력하는 함수 */
	void viewLoginMenu()
	{
		system("cls");
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│           (1) 로그인              │" << endl;
		cout << "└───────────────────────────────────┘" << endl;
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│           (2) 회원 가입           │" << endl;
		cout << "└───────────────────────────────────┘" << endl;
	}

	/* 로그인 메뉴 함수 */
	void loginMenu() {
		string name;
		char input;
		bool isContinue = true;

		while (isContinue) {
			viewLoginMenu();
			input = _getch();
			if (input == '1') { //로그인 진행
				isContinue = login();
			}
			else if (input == '2') { //회원가입 진행
				system("cls");
				cout << "생성할 닉네임을 입력해주세요 : ";
				cin >> name;
				usermanager.insert(usermanager.search(name));
				cout << "잠시 후 로그인 화면으로 돌아갑니다" << endl;
				Sleep(1000);
				cout << "\n3초 후 돌아갑니다\n";
				Sleep(1000);
				cout << "\n2초 후 돌아갑니다\n";
				Sleep(1000);
				cout << "\n1초 후 돌아갑니다\n";
				Sleep(1000);
			}
		}
	}
	/* 로그인 접속이 누구인지 확인 함수 */
	User whoIsloginNow(vector<User> list, string name) {
		User loginUser;
		int index = 0;
		for (unsigned int i = 0; i < list.size(); i++) {

			if (name == list.at(i).getName()) { //입력한 이름과 리스트내의 이름과 동일여부 판단
				index = i;
			}
			loginUser = list.at(index);
		}
		return loginUser; //user정보 반환
	}

	/* 로그인을 선택했을 때의 내용 함수 */
	bool login() {
		string loginName;
		string addName;
		system("cls");
		cout << "로그인 할 닉네임을 작성해주세요 : ";
		cin >> loginName;
		user = whoIsloginNow(usermanager.list1, usermanager.search(loginName));
		if (usermanager.getIsNew()) {//새로운 이름인지 판단
			cout << "찾으시는 닉네임이 존재하지 않습니다." << endl;
			cout << "새로운 닉네임으로 생성하시겠습니까? (Y/N)";
			bool choice = true;
			while (choice) {
				char choose;
				cin >> choose;
				if (choose == 'Y' || choose == 'y') {
					usermanager.insert(loginName);//이름 토대로 유저삽입
					user = whoIsloginNow(usermanager.list1, usermanager.search(loginName));
					cout << "잠시 후 로그인 화면으로 돌아갑니다" << endl;
					Sleep(1000);
					cout << "\n3초 후 돌아갑니다\n";
					Sleep(1000);
					cout << "\n2초 후 돌아갑니다\n";
					Sleep(1000);
					cout << "\n1초 후 돌아갑니다\n";
					Sleep(1000);
					return true;
				}
				else if (choose == 'N' || choose == 'n') {
					return true;
				}
				else
					cout << "잘못 입력하셨습니다." << endl;
			}
		}
		return false;
	}

};

/*
* GameSpace class
* Note : 로그인 메니저를 통해 AI와 유저간의 게임을 진행할수있는 공간을 제공하는 GameSpace class
*/

class GameSpace : public LoginManager {
	AI ai;
public:

	standardMap stdMap;
	transformMap trfMap;

	/* X, Y좌표 입력받고 검사하는 함수 */
	void inputAB(int player, char choose) {
		int inputA, inputB;
		int x, y;

		if (choose == '1') {
			cout << "(x y)를 입력하세요 : ";
			cin >> inputA >> inputB;
			y = inputA - 1; // 입력하는 입장에서는 X,Y이지만 프로그래머 입장에서 for문에 의해 Y,X로 바꾸어줘야 받아오기 편하다는 이점 활용으로 변환
			x = inputB - 1;
			stdMap.putStone(x, y, player);
			stdMap.clearMap();
		}

		if (choose == '2') {
			cout << "(x y)를 입력하세요 : ";
			cin >> inputA >> inputB;
			y = inputA - 1;
			x = inputB - 1;
			trfMap.putStone(x, y, player);
			trfMap.clearMap();
		}
	}

	/* 게임시작함수*/
	void isStart(char choose) {

		int ExitNum = 1; //게임을 종료하는 변수 (1이면 반복)
		char returnNum;
		int turnNum = 1;
		bool flag1 = true; //AI가 놓을 수있는지 판단
		bool flag2 = true; //user가 놓을 수있는지 판단


		// 일반맵
		if (choose == '1') {
			stdMap.defaultMap();
			stdMap.settingMap();
			while (ExitNum == 1) {
				// AI가 흰 돌을 놓을 때
				if (turnNum == 1) {
					stdMap.showingMap();//ai가 놓기 전 맵을 보여주는 위치
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│        흰 돌이 놓고 있습니다.     │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					stdMap.putCheck(turnNum);
					int bestX, bestY;
					ai.getPossibleMoves(stdMap);
					ai.smartcheck(stdMap);
					bestX = ai.getBestX();
					bestY = ai.getBestY();
					if (stdMap.pass() == false) {
						flag1 = false;//AI가 놓을 수 없는 상황

						if ((flag1 == false) && (flag2 == false))//AI가 놓을 수 없고 사람도 놓을 수 없는 상황 판단
						{
							cout << "두 플레이어 모두 놓을 곳이 없으므로 게임이 종료됩니다." << endl;
							break;
						}
						cout << "AI가 놓을 곳이 없으므로 턴이 넘어갑니다." << endl;
						Sleep(1000);
						turnNum++;//user턴으로 넘김
						continue;
					}
					flag1 = true;
					flag2 = true;
					stdMap.putStone(bestX, bestY, 1);
					stdMap.clearMap();
					Sleep(1000);
					stdMap.showingMap();//ai가 좌표에 돌을 놓았을 때 출력해주는 맵
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│        흰 돌이 놓고 있습니다.     │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					cout << "ai가 x = " << bestY + 1 << ",y = " << bestX + 1 << "를 놓았습니다.\n";
					Sleep(2000);                                                                 //2초정도 기다린 후 다음턴으로 넘기기
					turnNum++;//user턴으로 넘김
				}

				// user가 검은 돌을 놓을 때
				else {
					stdMap.putCheck(turnNum);
					stdMap.showingMap();
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│       검은 돌이 놓고 있습니다.    │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					if (stdMap.pass() == false) {
						flag2 = false; //유저가 놓을 수 없는 상황
						if ((flag1 == false) && (flag2 == false))//AI가 놓을 수 없고 사람도 놓을 수 없는 상황 판단
						{
							cout << "두 플레이어 모두 놓을 곳이 없으므로 게임이 종료됩니다." << endl;
							break;
						}
						cout << "User가 놓을 곳이 없으므로 턴이 넘어갑니다." << endl;
						Sleep(1000);
						turnNum--;//AI턴으로 넘김
						continue;
					}
					flag1 = true;
					flag2 = true;
					inputAB(turnNum, choose);
					turnNum--;//AI턴으로 넘김
				}
			}
		}

		// 변형 맵
		else if (choose == '2') {
			trfMap.defaultMap();
			trfMap.settingMap();
			while (ExitNum == 1) {
				// AI가 흰 돌을 놓을 때
				if (turnNum == 1) {
					trfMap.showingMap();
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│        흰 돌이 놓고 있습니다.     │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					trfMap.putCheck(turnNum);
					int bestX, bestY;
					ai.getPossibleMoves(trfMap);
					ai.smartcheck(trfMap);
					bestX = ai.getBestX();
					bestY = ai.getBestY();
					if (trfMap.pass() == false) {
						flag1 = false;//AI가 놓을 수 없는 상황

						if ((flag1 == false) && (flag2 == false))//AI가 놓을 수 없고 사람도 놓을 수 없는 상황 판단
						{
							cout << "두 플레이어 모두 놓을 곳이 없으므로 게임이 종료됩니다." << endl;
							break;
						}
						cout << "AI가 놓을 곳이 없으므로 턴이 넘어갑니다." << endl;
						Sleep(1000);
						turnNum++;//user턴으로 넘김
						continue;
					}
					flag1 = true;
					flag2 = true;
					trfMap.putStone(bestX, bestY, 1);
					trfMap.clearMap();
					Sleep(1000);
					trfMap.showingMap();
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│        흰 돌이 놓고 있습니다.     │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					cout << "ai가 x = " << bestY + 1 << ",y = " << bestX + 1 << "를 놓았습니다.\n";
					Sleep(2000);
					turnNum++;//user턴으로 넘김
				}

				// user가 검은 돌을 놓을 때
				else {
					trfMap.putCheck(turnNum);
					trfMap.showingMap();
					cout << "┌───────────────────────────────────┐" << endl;
					cout << "│       검은 돌이 놓고 있습니다.    │" << endl;
					cout << "└───────────────────────────────────┘" << endl;
					if (trfMap.pass() == false) {
						flag2 = false; //유저가 놓을 수 없는 상황
						if ((flag1 == false) && (flag2 == false))//AI가 놓을 수 없고 사람도 놓을 수 없는 상황 판단
						{
							cout << "두 플레이어 모두 놓을 곳이 없으므로 게임이 종료됩니다." << endl;
							break;
						}
						cout << "User가 놓을 곳이 없으므로 턴이 넘어갑니다." << endl;
						Sleep(1000);
						turnNum--;//AI턴으로 넘김
						continue;
					}
					flag1 = true;
					flag2 = true;
					inputAB(turnNum, choose);
					turnNum--;//AI턴으로 넘김
				}
			}
		}
		result(choose);
		cout << "홈화면으로 돌아가시려면 숫자 1을 눌러주세요";
		returnNum = _getch();//1을 받으면 넘길수 있도록 하는 변수
		if (returnNum == '1')
		{
			cout << "\n5초 후 돌아갑니다\n";
			Sleep(1000);
			cout << "\n4초 후 돌아갑니다\n";
			Sleep(1000);
			cout << "\n3초 후 돌아갑니다\n";
			Sleep(1000);
			cout << "\n2초 후 돌아갑니다\n";
			Sleep(1000);
			cout << "\n1초 후 돌아갑니다\n";
			Sleep(1000);
			stdMap.defaultMap();//일반맵 초기화
			trfMap.defaultMap();//변형맵 초기화
			stdMap.settingMap();//일반맵 세팅
			trfMap.settingMap();//변형맵 세팅
		}
		else {
			;
		}

	}
	/*승부 결과 판단 함수*/
	void result(char choose) {
		int resultNum; // 승리 결과 판단 변수
		resultNum = checkExit(choose);
		updateCoin(resultNum);
		updateRecord(resultNum);

		if (resultNum == 2) {//검은돌 승리
			cout << "┌───────────────────────────────────┐" << endl;
			cout << "│        ★ 검은 돌 승리 ★         │" << endl;
			cout << "│        ★ 검은 돌 승리 ★         │" << endl;
			cout << "│        ★ 검은 돌 승리 ★         │" << endl;
			cout << "│        ★ 검은 돌 승리 ★         │" << endl;
			cout << "│        ★ 검은 돌 승리 ★         │" << endl;
			cout << "└───────────────────────────────────┘" << endl;
		}

		else if (resultNum == 3) {//흰돌 승리
			cout << "┌───────────────────────────────────┐" << endl;
			cout << "│          ★ 흰 돌 승리 ★         │" << endl;
			cout << "│          ★ 흰 돌 승리 ★         │" << endl;
			cout << "│          ★ 흰 돌 승리 ★         │" << endl;
			cout << "│          ★ 흰 돌 승리 ★         │" << endl;
			cout << "│          ★ 흰 돌 승리 ★         │" << endl;
			cout << "└───────────────────────────────────┘" << endl;
		}
		else if (resultNum == 4) {//무승부
			cout << "┌───────────────────────────────────┐" << endl;
			cout << "│           ★ 무 승 부 ★          │" << endl;
			cout << "│           ★ 무 승 부 ★          │" << endl;
			cout << "│           ★ 무 승 부 ★          │" << endl;
			cout << "│           ★ 무 승 부 ★          │" << endl;
			cout << "│           ★ 무 승 부 ★          │" << endl;
			cout << "└───────────────────────────────────┘" << endl;
		}
	}

	/*유저의 재산 업데이트 함수*/
	void updateCoin(int resultNum)
	{
		int index = 0;
		for (unsigned int i = 0; i < usermanager.list1.size(); i++) {
			if (user.getName() == usermanager.list1.at(i).getName()) { //리스트 내에서 로그인 유저인지 판단
				index = i;
				break;
			}
		}

		if (resultNum == 2) //검은 돌이 유저이므로 이겼을 때 +50
		{
			usermanager.list1.at(index).setCoin(user.getCoin() + 50);
		}
		else if (resultNum == 3) //흰 돌이 유저이므로 졌을 때 -20
		{
			usermanager.list1.at(index).setCoin(user.getCoin() - 20);
		}
		else //나머지 상황(무승부)의 경우 변화 없음
		{
			;
		}
	}

	/*유저의 전적 업데이트 함수*/
	void updateRecord(int resultNum)
	{
		int index = 0;
		for (unsigned int i = 0; i < usermanager.list1.size(); i++) {
			if (user.getName() == usermanager.list1.at(i).getName()) { //리스트 내에서 로그인 유저인지 판단
				index = i;
				break;
			}
		}
		if (resultNum == 2)//검은 돌이 유저이므로 이겼을 때 승리횟수 증가
		{
			usermanager.list1.at(index).setWinCnt(user.getWinCnt() + 1);

		}
		else if (resultNum == 3)//흰 돌이 유저이므로 졌을 때 패배횟수 증가
		{
			usermanager.list1.at(index).setLoseCnt(user.getLoseCnt() + 1);

		}
		else if (resultNum == 4)// 무승부일 때 무승부 횟수 증가
		{
			usermanager.list1.at(index).setDrawCnt(user.getDrawCnt() + 1);
		}
	}

	/*게임이 끝났을 때의 결과 판단하는 함수*/
	int checkExit(char choose) {
		int whiteStoneCnt = 0; // 1번은 흰 돌
		int blackStoneCnt = 0; // 2번은 검은 돌
		int notPutBlock = 0;   // 3번일 때의 놓을 수 없는 위치
		int zeroCnt = 0;

		if (choose == '1') {
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if (stdMap.getMap(i, j) == 0) {// 해당칸이 비어있는 칸인지 판단
						zeroCnt++;
					}
					else if (stdMap.getMap(i, j) == 1) {// 해당칸이 흰 돌이 놓여져있는 칸인지 판단
						whiteStoneCnt++;
					}
					else if (stdMap.getMap(i, j) == 2) {// 해당칸이 검은 돌이 놓여져있는 칸인지 판단
						blackStoneCnt++;
					}
				}
			}

			if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt > whiteStoneCnt)) {//흰 돌 개수와 검은 돌 개수와 비어있는 칸의 수가 64개이며 검은 돌이 흰 돌보다 많은 경우
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
				return 2;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt < whiteStoneCnt)) {//흰 돌 개수와 검은 돌 개수와 비어있는 칸의 수가 64개이며 흰 돌이 검은 돌보다 많은 경우
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
				return 3;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt == whiteStoneCnt)) {//흰 돌 개수와 검은 돌 개수와 비어있는 칸의 수가 64개이며 흰 돌과 검은 돌이 같은 경우
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
				return 4;
			}
			else {
				return 1; //나머지 경우 계속 진행
			}
		}
		//위의 방식과 동일
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
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
				return 2;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt + notPutBlock) == (sizeof(trfMap.map) / sizeof(int))) && (blackStoneCnt < whiteStoneCnt)) {
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
				return 3;
			}
			else if (((zeroCnt + blackStoneCnt + whiteStoneCnt + notPutBlock) == (sizeof(stdMap.map) / sizeof(int))) && (blackStoneCnt == whiteStoneCnt)) {
				cout << "검은 돌의 개수 : " << blackStoneCnt << endl;
				cout << "흰 돌의 개수 : " << whiteStoneCnt << endl;
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
	/*로그인 진행 함수*/
	void isNowLogin()
	{
		loginMenu();
	}
	/*게임시작여부를 보여주는 함수*/
	void isChooseGame() {
		viewlogo();
		cout << "			(1)게임 시작" << endl;
		cout << "\n";
		cout << "			(2)게임 종료\n" << endl;
	}
	/*오델로 로고를 보여주는 함수*/
	void viewlogo() {
		system("cls");
		cout << "\n";
		cout << " ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○" << endl;
		cout << " ○○●●●○○○○●○○○●○○○○○○○○○○○●○●○○○○○○○" << endl;
		cout << " ○●○○○●○○○●○○○●○○○○○○○○○○○●○●○○○○○○○" << endl;
		cout << " ○●○○○●○●●●●●○●○○○○○○○○○○○●○●○○○○○○○" << endl;
		cout << " ○●○○○●○○○●○○○●○○○○○○●●●○○●○●○○●●●○○" << endl;
		cout << " ○●○○○●○○○●○○○●●●●○○●○○○●○●○●○●○○○●○" << endl;
		cout << " ○●○○○●○○○●○○○●○○○●○●○○○●○●○●○●○○○●○" << endl;
		cout << " ○●○○○●○○○●○○○●○○○●○●●●●○○●○●○●○○○●○" << endl;
		cout << " ○●○○○●○○○●○○○●○○○●○●○○○○○●○●○●○○○●○" << endl;
		cout << " ○○●●●○○○○●○○○●○○○●○○●●●●○●○●○○●●●○○" << endl;
		cout << " ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○○" << endl;
		cout << "\n\n";
	}

	/*유저의 이름과 전적, 메인 메뉴를 보여주는 함수*/
	void viewMain() {
		user = whoIsloginNow(usermanager.list1, usermanager.search(user.getName()));
		system("cls");
		cout << "\n";
		cout << "	닉네임 : " << user.getName() << "              " << endl;
		cout << "	[전적]             " << endl;
		cout << "	Coin : " << user.getCoin() << endl;
		cout << "	Win : " << user.getWinCnt() << "   Lose : " << user.getLoseCnt() << "   Draw : " << user.getDrawCnt() << endl;
		cout << "\n\n";
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│           (1) 방 만들기           │" << endl;
		cout << "└───────────────────────────────────┘" << endl;
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│           (2) 멤버 목록           │" << endl;
		cout << "└───────────────────────────────────┘" << endl;
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│           (3) 게임 종료           │" << endl;
		cout << "└───────────────────────────────────┘" << endl;

	}

	/*맵 선택 메뉴를 보여주는 함수*/
	void isChooseMap() {

		system("cls");
		cout << "┌───────────────────────────────────┐" << endl;
		cout << "│         맵을 선택해주세요!        │" << endl;
		cout << "│        \x1b[31m(숫자를 눌러주세요)\x1b[0m        │" << endl;
		cout << "│                                   │" << endl;
		cout << "│            (1) 일반 맵            │" << endl;
		cout << "│            (2) 변형 맵            │" << endl;
		cout << "└───────────────────────────────────┘" << endl;

	}
};

int main() {

	char startNum; //시작여부 설정 변수
	char menuChoice; //메뉴 선택 설정 변수
	char choose; //맵 선택 설정 변수
	GameSpace gamespace; //게임스페이스 객체 생성
	gamespace.isChooseGame();
	cout << "\t\t\t";
	startNum = _getch();
	if (startNum == '1') //게임 시작 확인
	{
		gamespace.isNowLogin();

		while (1)
		{
			gamespace.viewMain();
			cout << "\t\t   ";
			menuChoice = _getch();
			if (menuChoice == '1') { //방 생성 확인
				gamespace.isChooseMap();
				cout << "\t\t    \n";
				choose = _getch();
				if (choose == '1' || choose == '2') {//일반 맵 또는 변형맵 선택
					gamespace.isStart(choose);
				}
				else {
					cout << "다시 입력해주세요";
					gamespace.isChooseMap();
					cout << "\t\t    ";
				}
			}
			else if (menuChoice == '2') //유저 기록들 전체 확인
			{
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n3초 후 돌아갑니다";
				Sleep(1000);
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n2초 후 돌아갑니다";
				Sleep(1000);
				system("cls");
				gamespace.usermanager.memberPrint();
				cout << "\n1초 후 돌아갑니다";
				Sleep(1000);
			}
			else if (menuChoice == '3') //게임 종료 여부 확인
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