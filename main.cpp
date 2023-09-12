#include "imgui.h"
#include "imgui-SFML.h"
#include "AppGui.h"

#include <SFML/Graphics.hpp>
#include <iostream>

struct GuiInitSettings
{
    constexpr static int winFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
     ImVec2 winSize;
     ImVec2 winPos;
     
};
int main()
{
    std::cout << "Program started\n";

    sf::RenderWindow window(sf::VideoMode(800, 800), "reinr:: file explorer");
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    AppGui gui;
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Window title");

       /* ImGui::Text("Window text!");*/

            gui.DisplayMenu();
            ImGui::Separator();
            gui.DisplayContent();
            ImGui::Separator();
            gui.DisplayActions();
            ImGui::Separator();
            gui.DisplayFilter(); //filter by file extensions
        
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}