/*
    GecProject - For GEC students to use as a start point for their projects.
    Already has SFML linked and ImGui set up.
*/

#include "ExternalHeaders.h"
#include "RedirectCout.h"

#include <iostream>
#include <SFML/Graphics.hpp>

void DefineGUI();



float x = 100.f;
float y = 100.f;

float red = 5.f;
float green = 5.f;
float blue = 5.f;
float opacity = 255.f;

int window_x = 800;
int window_y = 600;


static float timer = 0.0f;
static int seconds = 0;

int main()
{
    // Redirect cout to the Visual Studio output pane
    outbuf ob;
    std::streambuf* sb{ std::cout.rdbuf(&ob) };

    // Redirect cerr
    outbuferr oberr;
    std::streambuf* sberr{ std::cerr.rdbuf(&oberr) };

    // Turn on memory leak checking
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(window_x, window_y)), "GEC Start Project");

    // Set up ImGui (the UI library)
    if (!ImGui::SFML::Init(window))
        return -1;

    // Create a simple shape to draw
    sf::RectangleShape shape({100.f, 100.f});
    
    
    // Clock required by ImGui
    sf::Clock uiDeltaClock;
  
    while (window.isOpen())
    {

        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Feed ImGui
            ImGui::SFML::ProcessEvent(window, event.value());

            // User clicked on window close X
            if (event->is<sf::Event::Closed>())
                window.close();                          
        }

        float x_delta = x;
        float y_delta = y;

        float deltaTime = ImGui::GetIO().DeltaTime;

        timer += deltaTime;

        if (timer >= 1.0f) {
            seconds += (int)timer;
            timer = fmodf(timer, 1.0f);
        }

        if (seconds > 10) {
            seconds = 1;
        }


        // ImGui must be updated each frame
        ImGui::SFML::Update(window, uiDeltaClock.restart());

        // The UI gets defined each time
        DefineGUI();

        if (x != x_delta) {
            shape.setPosition({ x, y });
        }

        if (y != y_delta) {
            shape.setPosition({ x, y });
        }
        

        // Clear the window
        window.clear();
        
        // Draw the shape
        window.draw(shape);
        shape.setFillColor(sf::Color(red, green, blue, opacity));

        // UI needs drawing last
        ImGui::SFML::Render(window);

        window.display();
    }

    std::cout << "Finished!" << std::endl;

	ImGui::SFML::Shutdown();

    return 0;
}

/* 
    Use IMGUI for a simple on screen GUI
    See: https://github.com/ocornut/imgui/wiki/
*/



void DefineGUI()
{
    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("GEC");				// Create a window called "3GP" and append into it.

    ImGui::Text("Elapsed Time: %d seconds", seconds);

    ImGui::SliderFloat("X Coord", &x, 0, window_x);
    ImGui::SliderFloat("Y Coord", &y, 0, window_y);

    if (ImGui::Button("B_Red")) {
        red = 255.f;
        green = 0.f;
        blue = 0.f;
    }

    if (ImGui::Button("B_Green")) {
        red = 0.f;
        green = 255.f;
        blue = 0.f;
    }

    if (ImGui::Button("B_Blue")) {
        red = 0.f;
        green = 0.f;
        blue = 255.f;
    }
    
    ImGui::SliderFloat("Red", &red, 0, 255);
    ImGui::SliderFloat("Green", &green, 0, 255);
    ImGui::SliderFloat("Blue", &blue, 0, 255);
    ImGui::SliderFloat("Opacity", &opacity, 0, 255);

 //   ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

  //  ImGui::Checkbox("Cull Face", &m_cullFace);

   // ImGui::SliderFloat("Speed", &gAnimationSpeed, 0.01f, 0.3f);	// Slider from 0.0 to 1.0

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}
