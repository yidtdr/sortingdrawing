#define BASE_Y = 500.f
#define SCREENSIZE 1920
#define SCREENHEIGHT 1080
#define PADDING 2
#define NUM 300
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include "globals.h"
using namespace sf;

Color ColorMap[3] = { Color(0, 255, 0), Color(255,0,0), Color(0,0,255)};

class rct {
private:
	float x;
	float y;
	float width;
	float height;
	int active = 0;
public:
	void setAttributes(float newx, float newy, float newwidth, float newheight)
	{
		this->x = newx;
		this->height = newheight;
		this->width = newwidth;
	}
	Vector2f getSize()
	{
		return Vector2f(this->width, this->height);
	}
	Vector2f getPos()
	{
		return Vector2f(this->x, this->y);
	}
	int getActive()
	{
		return this->active;
	}
	void setActive(int newActive)
	{
		this->active = newActive;
	}

	rct() {};
	rct(float nx, float ny, float nwidth, float nheight) : x(nx), y(ny), width(nwidth), height(nheight) { };
};

void elSwap(rct* sw1, rct* sw2)
{
	float buff = sw1->getSize().y;
	sw1->setAttributes(sw1->getPos().x, sw1->getPos().y, sw1->getSize().x, sw2->getSize().y);
	sw2->setAttributes(sw2->getPos().x, sw2->getPos().y, sw2->getSize().x, buff);
}

rct* initArray(const int n)
{
	rct r_Arr[NUM];

	for (int i = 0; i < NUM; i++)
	{
		r_Arr[i].setAttributes((float)(SCREENSIZE * i / n + PADDING / 2), 0, (float)(SCREENSIZE / n - PADDING), (float)((i + 1) * (SCREENHEIGHT / NUM - 1)));
		r_Arr[i].setActive(0);
	}
	

	return r_Arr;
}

rct* shuffleArray(rct* r)
{
	elSwap(&r[rand() % NUM], &r[rand() % NUM]);

	return r;	
}

void drawArray(rct* r, RectangleShape* buffrect, RenderWindow* window)
{
	for (int i = 0; i < NUM; i++)
	{
		buffrect->setFillColor(ColorMap[r[i].getActive()]);

		buffrect->setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
		buffrect->setSize(r[i].getSize());
		window->draw(*buffrect);
	}
}

void SortArray(rct* r, RectangleShape* buffrect, RenderWindow* window)
{

	globals::sortingTime = 0;
	auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < NUM - 1; i++)
	{
		r[i].setActive(1);
		for (int j = i + 1; j < NUM; j++)
		{
			r[j].setActive(2);
			if (r[i].getSize().y < r[j].getSize().y)
			{
				elSwap(&r[i], &r[j]);
			}
			finish = std::chrono::high_resolution_clock::now();
			globals::sortingTime += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
			if (globals::programState != STATE_SORTING)
			{
				return;
			}
			std::this_thread::sleep_for(std::chrono::nanoseconds(globals::sleepTime));
			start = std::chrono::high_resolution_clock::now();
			r[j].setActive(0);
		}
		r[i].setActive(0);
	}
}