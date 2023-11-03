#include <SFML/graphics.hpp>
#include "Arrays.hpp"
#include <Windows.h>
#include <thread>

#define NUM 100

using namespace sf;

void updateState(rct* r, RectangleShape* buffrect, RenderWindow* window)
{
    for (int i = 0; i < NUM; i++)
    {
        shuffleArray(r);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    SortArray(r, buffrect, window);
}

int main()
{
    RenderWindow window(VideoMode(), "ArraySorting", Style::Fullscreen);

    RectangleShape buffrect;

    rct* r = initArray(NUM);

    window.setFramerateLimit(60);

    window.clear();

    for (int i = 0; i < NUM; i++)
    {
        buffrect.setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
        buffrect.setSize(r[i].getSize());
        window.draw(buffrect);
    }

    std::thread update(updateState, r, &buffrect, &window);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        drawArray(r, &buffrect, &window);
        window.display();
    }

    update.join();

    return 0;
}