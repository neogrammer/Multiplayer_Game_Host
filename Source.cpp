#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <core/CidWindow.h>
#include <core/VideoGame.h>

#include <entities/Player.h>
#include <core/resources/Cfg.h>
#include <core/Globals.h>


/////////////////////////
// 
// H O S T
// 
// ////////////////////
// Host program


int main(int argc, char* argv[])
{

    Cfg::Initialize();


    Player host{ "Player1",1,100.f,100.f };
    Player guest{ "Guest",2,400.f,100.f };


    // as host you are connecting to the guest, listener on this one
    bool running = true;

    std::cout << "Waiting for guest to join..." << std::endl;
    sf::TcpListener listener;
    auto status = listener.listen(53000);
    if (status != sf::TcpSocket::Status::Done)
    {
        std::cout << "Error" << std::endl;
        running = false;
    }
    sf::TcpSocket socket;
    status = listener.accept(socket);
    if (status != sf::TcpSocket::Status::Done)
    {
        std::cout << "Error" << std::endl;
        running = false;
    }

    CidWindow window;
    std::string str = "SFML3 GAME";
    window.create((int)glb::WINW, (int)glb::WINH, "SFML3 Game", sf::State::Windowed);


    bool soWhat = ImGui::SFML::Init(window, false);
    soWhat = false;

    auto& IO = ImGui::GetIO();
    IO.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)
    // IO.Fonts->AddFontDefault(); // this will load default font as well
    IO.Fonts->AddFontFromFileTTF("Assets\\Fonts\\Crusty.ttf", 18.f);



    // change the position of the window (relatively to the desktop)
   // window.setPosition({ 400, 200 });

    // change the title of the window
    window.setTitle("SFML window");


    // create the game
    VideoGame game{&window, &guest, &host};



    // run the program as long as the window is open
    sf::Clock deltaClock;


    while (running && window.isOpen())
    {

        std::string nameP2 = "Host";
        uint32_t idP2 = 1;
        float xPosP2 = host.xpos;
        float yPosP2 = host.ypos;
        sf::Packet packet;
        status = socket.receive(packet); // blocking    
        if (status != sf::TcpSocket::Status::Done)
        {
            std::cout << "Error" << std::endl;
            running = false;
        }
        bool error = true;
        if (packet >> nameP2)
        {
            if (packet >> idP2)
            {
                if (packet >> xPosP2)
                {
                    if (packet >> yPosP2)
                    {
                        // only got here if all variables are now filled correctly
                        error = false;
                        guest.name = nameP2;
                        guest.id = idP2;
                        guest.xpos = xPosP2;
                        guest.ypos = yPosP2;
                    }
                }
            }
        }
        if (error)
        {
            std::cout << "Error reading in the data" << std::endl;
            //throw an error
        }
        // now pack my data from games data and send it
        std::string nameP1 = host.name;
        uint32_t idP1 = host.id;
        float xPosP1 = host.xpos;
        float yPosP1 = host.ypos;
        {
            sf::Packet packet2;
            packet2 << nameP1 << idP1 << xPosP1 << yPosP1;
            status = socket.send(packet2);  //blocking, maybe turn off blocking and use a selector?
            if (status != sf::TcpSocket::Status::Done)
            {
                std::cout << "Error" << std::endl;
                running = false;
            }
        }

        // now that both are sent to each other and updated , run a game frame and then start back over reading and writing every frame.
        std::cout << guest.name << guest.id << guest.xpos << guest.ypos << std::endl;

        if (running)
        {

            // check all the window's events that were triggered since the last iteration of the loop
            while (const std::optional event = window.pollEvent())
            {
                ImGui::SFML::ProcessEvent(window, *event);
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        window.close();
                }
                // game.input(event);
            }
            sf::Time dt = deltaClock.restart();
            
            game.update(dt, &host, &guest);
           


            soWhat = ImGui::SFML::UpdateFontTexture(); // important call: updates font texture
            ImGui::SFML::Update(window, dt);

            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();


            window.clear(sf::Color(47, 147, 247, 255));

            //window.setView(game.mainView);
            game.render();
            //window.setView(window.getDefaultView()

            ImGui::SFML::Render(window);
            window.display();
        }
    }
    if (window.isOpen())
    {
        window.close();
    }
    ImGui::SFML::Shutdown();
    // now just sent so its time to wait for a reply
    return 69;
}