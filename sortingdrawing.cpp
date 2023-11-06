#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "Arrays.hpp"
#include <Windows.h>
#include <thread>
#include "globals.h"
#include "imApi.hpp"

#define NUM 300


using namespace sf;

void updateState(rct* r, RectangleShape* buffrect, RenderWindow* window)
{
    while(true) {
        switch (globals::programState)
        {
            case STATE_SHUFFLING:
            {
                for (int i = 0; i < NUM * 100 * globals::shuffleDepth; i++)
                {
                    shuffleArray(r);
                }
                globals::programState = STATE_WAITING;
                break;
            }
            case STATE_SORTING:
            {
                SortArray(r);
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
    int speed = 50;
    RectangleShape buffrect;
    ImGui::SFML::Init(window);

    rct* r = initArray(NUM);

    window.setFramerateLimit(50);

    window.clear();

    for (int i = 0; i < NUM; i++)
    {
        buffrect.setPosition(Vector2f(r[i].getPos().x, 1080 - r[i].getSize().y));
        buffrect.setSize(r[i].getSize());
        window.draw(buffrect);
    }

    std::thread update(updateState, r, &buffrect, &window);

    Clock Deltaclock;

    imapi::SetupImGuiStyle();
    ImGuiStyle& style = ImGui::GetStyle();

    int buttonwidth = (350 - (style.IndentSpacing + style.ItemSpacing.x));
    int buttonwidth2 = (350 - (style.IndentSpacing + style.ItemSpacing.x + style.ItemSpacing.x)) / 2;

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
        if (globals::programState != STATE_SORTING)
        {
            if (ImGui::Button("Shuffle", ImVec2(buttonwidth2, 45)))
            {
                globals::programState = STATE_SHUFFLING;
            }
            ImGui::SameLine();
            if (ImGui::Button("Sort", ImVec2(buttonwidth2, 45)))
            {
                globals::programState = STATE_SORTING;
            }
        }
        else
        {
            if (globals::sortAlg == 0) {
                if (ImGui::Button("Stop sorting", ImVec2(buttonwidth, 45)))
                {
                    globals::programState = STATE_WAITING;
                }
            }
            if (globals::sortAlg == 1) {
                ImGui::Text("Currently sorting...");
            }
        }
        ImGui::Separator();
        ImGui::ListBox("Sorting\nalgoritm", &globals::sortAlg, globals::sortingAloritmsList, 3, 2);
        ImGui::SliderInt("Sorting speed", &speed, 1, 100);
        ImGui::SliderInt("Shuffle iter.", &globals::shuffleDepth, 1, 1000);
        //if (speed > 100)
        //    speed = 100;
        //if (speed < 1)
        //    speed = 1;

        // 1 = 100 000 in sleeptime
        // 100 = 1 in sleeptime

        globals::sleepTime = (100 / speed) * (100 / speed) * (100 / speed);

        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(210, 40));
        ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs );
        if (globals::sortAlg == 0) {
            int seconds = globals::sortingTime / 1000000000;
            int milliseconds = globals::sortingTime / 1000000 - seconds * 1000;
            int microseconds = globals::sortingTime / 1000 - milliseconds * 1000 - seconds * 1000000;
            int nanoseconds = globals::sortingTime % 1000;
            ImGui::Text("%i s.%i ms. %i mcs. %i ns.", seconds, milliseconds, microseconds, nanoseconds);
        }
        if (globals::sortAlg == 1)
        {
            if (globals::programState == STATE_SORTING) {
                ImGui::Text("Measuring time...");
            }
            else {
                int seconds = globals::sortingTime / 1000000000;
                int milliseconds = globals::sortingTime / 1000000 - seconds * 1000;
                int microseconds = globals::sortingTime / 1000 - milliseconds * 1000 - seconds * 1000000;
                int nanoseconds = globals::sortingTime % 1000;
                ImGui::Text("%i s.%i ms. %i mcs. %i ns.", seconds, milliseconds, microseconds, nanoseconds);
            }
        }
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