#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>

using namespace sf;

const int M = 31;
const int N = 18;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

class movement {
	bool rotate = 0;
public:
	bool check()
	{
		for (int i = 0; i<4; i++)
			if (a[i].x<0 || a[i].x >= N || a[i].y >= M) return 0;
			else if (field[a[i].y][a[i].x]) return 0;

			return 1;
	};
	void set_rotate(bool number) {
		rotate = number;
	}
	bool get_rotate() {
		return rotate;
	}
};


int main()
{
	movement n;
	srand(time(0));

	RenderWindow window(VideoMode(550, 620), "TETRIS");

	Texture t1, t2, t3;
	t1.loadFromFile("tiles.png");
	t2.loadFromFile("fondo3.png");
    t3.loadFromFile("alert.png");
	Sprite sprite(t1), fondo(t2),alert(t3);

	int dx = 0; int colorNum = 1;
	float timer = 0, delay = 0.3;


	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;


		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::KeyPressed)

				if (e.key.code == Keyboard::Escape)
					std::cout << "Game Over";;


			if (e.type == Event::KeyPressed)

				if (e.key.code == Keyboard::Escape)
					window.close();


				else if (e.key.code == Keyboard::Up) n.set_rotate(true);


				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;

		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//// <- Move -> ///
		for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].x += dx; }
		if (!n.check()) for (int i = 0; i<4; i++) a[i] = b[i];

		//////Rotate//////
		if (n.get_rotate())
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i<4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!n.check()) for (int i = 0; i<4; i++) a[i] = b[i];
		}

		///////Tick//////
		if (timer>delay)
		{
			for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].y += 1; }

			if (!n.check())
			{
				for (int i = 0; i<4; i++) field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i<4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		///////check lines//////////
		int k = M-1;

		for (int i = M - 1; i>0; i--)
		{
			int count = 0;
			for (int j = 0; j<N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count<N) k--;

		}

		dx = 0; delay = 0.15;
		n.set_rotate(0);

		/////////draw//////////
		window.clear(Color::White);
		window.draw(fondo);

		for (int i = 0; i<M; i++)
			for (int j = 0; j<N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31); //offset
				window.draw(sprite);


			}

		for (int i = 0; i<4; i++)
		{
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			sprite.setPosition(a[i].x * 18, a[i].y * 18);
			sprite.move(28, 31); //offset
			window.draw(sprite);
		}


		window.display();
	}

	return 0;
}
