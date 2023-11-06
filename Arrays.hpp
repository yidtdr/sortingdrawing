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
	void setSizeY(float newy)
	{
		this->height = newy;
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

void sleepAndMeasure()
{
	auto tstart = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::nanoseconds(globals::sleepTime));
	auto finish = std::chrono::high_resolution_clock::now();
	globals::sleeped += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - tstart).count();
}

void initArray(rct* r, int n)
{
	for (int i = 0; i < n; i++)
	{
		r[i].setAttributes((float)(SCREENSIZE * i / n + PADDING / 2), 0, (float)(SCREENSIZE / n - PADDING), (float)((i + 1) * (SCREENHEIGHT / NUM - 1)));
		r[i].setActive(0);
	}
}

rct* shuffleArray(rct* r)
{
	elSwap(&r[rand() % globals::curArraySize], &r[rand() % globals::curArraySize]);

	return r;	
}

void drawArray(rct* r, RectangleShape* buffrect, RenderWindow* window)
{
	for (int i = 0; i < globals::curArraySize; i++)
	{
		buffrect->setFillColor(ColorMap[r[i].getActive()]);

		buffrect->setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
		buffrect->setSize(r[i].getSize());
		window->draw(*buffrect);
	}
}

void Merge(rct* r, int start_1, int end_1, int start_2, int end_2)
{
	std::vector<float> h;
	float h1, h2;
	int start = start_1;

	while ((start_1 <= end_1) && (start_2 <= end_2))
	{
		h1 = r[start_1].getSize().y;
		h2 = r[start_2].getSize().y;
		if (h1 > h2)
		{
			h.push_back(h1);
		    start_1++;
		}
		else
		{
			h.push_back(h2);
			start_2++;
		}
	}

	for (int i = start_1; i <= end_1; i++)
	{
		h.push_back(r[i].getSize().y);
	}
	for (int i = start_2; i <= end_2; i++)
	{
		h.push_back(r[i].getSize().y);
	}

	for (int i = 0; i < h.size(); i++)
	{
		r[start + i].setSizeY(h[i]);
		sleepAndMeasure();
	}
}

void MergeSort(rct* r, int start, int end)
{
	r[start].setActive(1);
	r[end].setActive(2);
	if ((start + 1) < end)
	{
		MergeSort(r, start, (start + end) / 2);
		MergeSort(r, ((start + end) / 2) + 1, end);
		Merge(r, start, (start + end) / 2, (start + end) / 2 + 1, end);
		return;
	}

	if (r[start].getSize().y < r[end].getSize().y)
	{
		sleepAndMeasure();
		elSwap(&r[start], &r[end]);
	}
	r[start].setActive(0);
	r[end].setActive(0);
	return;
}

void QuickSort(rct* r, int start, int end)
{
	sleepAndMeasure();
	int left = start, right = end;
	int piv = r[(right + left) / 2].getSize().y;
	while (left <= right)
	{
		r[left].setActive(1);
		r[right].setActive(2);
		while (r[left].getSize().y > piv)
		{
			r[left].setActive(0);
			left++;
		}
		while (r[right].getSize().y < piv)
		{
			r[right].setActive(0);
			right--;
		}
		if (left <= right)
		{
			r[left].setActive(0);
			r[right].setActive(0);
			elSwap(&r[left++], &r[right--]);
		}
		sleepAndMeasure();
		r[left].setActive(0);
		r[right].setActive(0);
	}
	if (start < right)
		QuickSort(r, start, right);
	if (end > left)
		QuickSort(r, left, end);
}

void SortArray(rct* r)
{
	if (globals::sortAlg == 0)
	{
		globals::sortingTime = 0;
		auto start = std::chrono::high_resolution_clock::now();
		auto finish = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < globals::curArraySize - 1; i++)
		{
			r[i].setActive(1);
			for (int j = i + 1; j < globals::curArraySize; j++)
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
		return;
	}
	if (globals::sortAlg == 1)
	{
		globals::sortingTime = 0;
		globals::sleeped = 0;
		auto start = std::chrono::high_resolution_clock::now();
		MergeSort(r, 0, globals::curArraySize - 1);
		auto finish = std::chrono::high_resolution_clock::now();
		globals::sortingTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() - globals::sleeped;
		return;
	}
	if (globals::sortAlg == 2)
	{
		globals::sortingTime = 0;
		globals::sleeped = 0;
		auto start = std::chrono::high_resolution_clock::now();
		QuickSort(r, 0, globals::curArraySize - 1);
		auto finish = std::chrono::high_resolution_clock::now();
		globals::sortingTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() - globals::sleeped;
		return;
	}
}