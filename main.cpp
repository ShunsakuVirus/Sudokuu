#include "lib/framework.hpp"
#include <list>
#include <ctime>

enum Size {
	WIDTH = 900,
	HEIGHT = 900
};

int BinP(float x, float y, float w, float h, float px, float py) {
	if (px > x && px < x + w &&
		py > y && py < y + h)
		return 1;
	return 0;
}

Vec2f whCalc(int n) {
	float w = ((n - 1) % 5)*(96.5);
	int h = 0;
	if (n > 5) h = 128;
	return Vec2f(w, h);
}

void eraseDNum(std::list<int>& numList, int delNum) {
	for (auto it = numList.begin(); it != numList.end(); ++it) {
		if (*it == delNum) {
			numList.erase(it);
			return;
		}
	}
}

void generator(std::list<int> possibility[9][9], int num[9][9]) {
	Random dice;
	dice.setSeed(std::time(nullptr));
INIT:
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				possibility[i][j].clear();
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				possibility[i][j].push_back(k + 1);
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			if (possibility[i][j].size() == 0) {
				printf("やっべ!");
				goto INIT;
			}

			int rand = dice(possibility[i][j].size());
			auto it = possibility[i][j].begin();

			for (int k = 0; k < rand; k++) {
				++it;
			}

			int Decision = *it;
			num[i][j] = Decision;

			for (int k = 0; k < 9; k++) {
				eraseDNum(possibility[i][k], Decision);
				eraseDNum(possibility[k][j], Decision);
			}

			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					eraseDNum(possibility[(i - (i % 3)) + k][j - (j % 3) + l], Decision);
				}
			}
		}
	}
}

int main() {
	AppEnv env(Size::WIDTH, Size::HEIGHT);

	Texture toten("res/onetoten.png");

	const int edgeLength = 100;
	int num[9][9];
	std::list<int> possibility[9][9];

	generator(possibility, num);

	Random dice;
	dice.setSeed(std::time(nullptr));

	

  while (env.isOpen()) {
    env.begin();
	Vec2f mouse = env.mousePosition();


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			Vec2f wh = whCalc(num[i][j]);
			if (BinP(j*edgeLength - WIDTH / 2, 
				HEIGHT / 2 - i*edgeLength - edgeLength, 
				100, 100, mouse.x(), mouse.y())) {

				drawTextureBox(j*edgeLength - WIDTH / 2, 
					HEIGHT / 2 - i*edgeLength - edgeLength, 
					100, 100, wh.x(), wh.y(), 102.4, 128, 
					toten,Color(0,1,1,1));
			}
			else {
				drawTextureBox(j*edgeLength - WIDTH/2, 
					HEIGHT/2 - i*edgeLength - edgeLength, 
					100, 100, wh.x(), wh.y(), 102.4, 128, 
					toten);
			}
		}
	}

    env.end();
  }
}
