#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "Arrays.hpp"
#include <Windows.h>
#include <thread>
#include "globals.h"

#define NUM 300

/*        for (int i = 0; i < NUM; i++)
        {
            shuffleArray(r);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        SortArray(r, buffrect, window);
*/

using namespace sf;

void updateState(rct* r, RectangleShape* buffrect, RenderWindow* window)
{
    while(true) {
        switch (globals::programState)
        {
            case STATE_SHUFFLING:
            {
                for (int i = 0; i < NUM * globals::shuffleDepth; i++)
                {
                    shuffleArray(r);
                }
                globals::programState = STATE_WAITING;
                break;
            }
            case STATE_SORTING:
            {
                SortArray(r, buffrect, window);
                globals::programState = STATE_WAITING;
                break;
            }
            case STATE_WAITING:
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                break;
            }
        }

    }
}

int main()
{
    RenderWindow window(VideoMode(), "ArraySorting", Style::Fullscreen);

    RectangleShape buffrect;
    ImGui::SFML::Init(window);

    rct* r = initArray(NUM);

    window.setFramerateLimit(120);

    window.clear();

    for (int i = 0; i < NUM; i++)
    {
        buffrect.setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
        buffrect.setSize(r[i].getSize());
        window.draw(buffrect);
    }

    std::thread update(updateState, r, &buffrect, &window);

    Clock Deltaclock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        ImGui::SFML::Update(window, Deltaclock.restart());
        ImGui::SetNextWindowSize(ImVec2(350, 400));
        ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SameLine();
        if (ImGui::Button("Shuffle", ImVec2(155, 45)))
        {
            globals::programState = STATE_SHUFFLING;
        }
        ImGui::SameLine();
        if (globals::programState != STATE_SORTING)
        {
            if (ImGui::Button("Sort", ImVec2(155, 45)))
            {
                globals::programState = STATE_SORTING;
            }
        }
        else
        {
            if (ImGui::Button("Stop sorting...", ImVec2(155, 45)))
            {
                globals::programState = STATE_WAITING;
            }
        }
        ImGui::Separator();
        ImGui::InputInt("Sorting speed", &globals::sleepTime, 1, 7);
        if (globals::sleepTime > 499)
            globals::sleepTime = 500;
        if (globals::sleepTime < 0)
            globals::sleepTime = 0;
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs );
        int seconds = globals::sortingTime / 1000000000;
        int milliseconds = globals::sortingTime / 1000000 - seconds*1000;
        int microseconds = globals::sortingTime / 1000 - milliseconds * 1000 - seconds * 1000000;
        int nanoseconds = globals::sortingTime % 1000;
        ImGui::Text("%i sec.%i millisec. %i microsec. %i nanosec.", seconds, milliseconds, microseconds, nanoseconds);
        ImGui::End();

        window.clear();
        drawArray(r, &buffrect, &window);
        ImGui::SFML::Render(window);
        window.display();
    }

    update.join();

    ImGui::SFML::Shutdown();

    return 0;
}