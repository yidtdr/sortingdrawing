#include <SFML/graphics.hpp>
#include "Arrays.hpp"
#include <Windows.h>

#define NUM 100

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(), "ArraySorting", Style::Fullscreen);

    RectangleShape buffrect;

    rct* r = initArray(NUM);

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < NUM; i++)
        {
            buffrect.setPosition(Vector2f(r[i].getPos().x, 1080-r[i].getSize().y));
            buffrect.setSize(r[i].getSize());
            window.draw(buffrect);
        }


        for (int i = 0; i < NUM; i++)
        {
            window.clear();
            shuffleArray(r);
            for (int i = 0; i < NUM; i++)
            {
                buffrect.setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
                buffrect.setSize(r[i].getSize());
                window.draw(buffrect);
            }
            window.display();
        }
        Sleep(5000);

        SortArray(r, &buffrect, &window);

        system("pause");

        window.display();


    }

    return 0;
}