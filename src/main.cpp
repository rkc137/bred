#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <random>

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getFullscreenModes().front(),
        "bred",
        sf::State::Fullscreen
    );
    window.setFramerateLimit(144);
    sf::View view({0, 0}, static_cast<sf::Vector2f>(window.getSize()));
    window.setView(view);

    constexpr int step = 4;
    sf::RectangleShape back(view.getSize());
    back.setFillColor({255, 255, 255, 8});
    back.setOrigin(back.getGeometricCenter());

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> color_r(0, UINT32_MAX); // define the range
    std::uniform_int_distribution<> distr(-step, step); // define the range
    // std::uniform_int_distribution<> distr(-1, 1); // define the range
    
    std::vector<sf::Vertex> vertexes(2, sf::Vertex{});
    for(auto &v : vertexes)
        v.color = sf::Color::Black;

    std::array prims = {
        sf::PrimitiveType::Points,        //!< List of individual points
        sf::PrimitiveType::Lines,         //!< List of individual lines
        sf::PrimitiveType::LineStrip,     //!< List of connected lines, a point uses the previous point to form a line
        sf::PrimitiveType::Triangles,     //!< List of individual triangles
        sf::PrimitiveType::TriangleStrip, //!< List of connected triangles, a point uses the two previous points to form a triangle
        sf::PrimitiveType::TriangleFan  
    };
    auto premitiva = prims.begin();

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                    case sf::Keyboard::Scancode::D:
                    {
                        auto old_count = vertexes.size();
                        vertexes.resize(2 * old_count);
                        std::copy_n(vertexes.begin(), old_count, vertexes.begin() + old_count);
                        for(auto &v : vertexes)
                            v.color = sf::Color(color_r(gen));
                        break;
                    }
                    case sf::Keyboard::Scancode::Space:
                        premitiva++;
                        if(premitiva == prims.end())
                            premitiva = prims.begin();
                        break;
                    default:
                        break;
                }
            }
        }

        // for(auto &v : vertexes)
        //     v.position += {
        //         static_cast<float>(distr(gen)),
        //         static_cast<float>(distr(gen))
        //         // static_cast<float>(distr(gen)) * step,
        //         // static_cast<float>(distr(gen)) * step
        //     };
        
        for(auto vertex = vertexes.begin(); vertex != vertexes.end(); vertex += 2)
        {
            vertex->position = (vertex + 1)->position;
            (vertex + 1)->position += {
                static_cast<float>(distr(gen)),
                static_cast<float>(distr(gen))
                // static_cast<float>(distr(gen)) * step,
                // static_cast<float>(distr(gen)) * step
            };
        }

        window.draw(vertexes.data(), vertexes.size(), *premitiva);
        window.draw(back);
        window.display();
        // window.clear();
    }
}