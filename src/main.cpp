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
        "CMake SFML Project",
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
    // std::uniform_int_distribution<> distr(-step, step); // define the range
    std::uniform_int_distribution<> distr(-1, 1); // define the range
    
    std::vector<sf::VertexArray> lines(1, sf::VertexArray(sf::PrimitiveType::Lines, 2));
    for(auto &line : lines)
    {
        line[0].position = line[1].position = {0, 0};
        line[0].color = line[1].color = sf::Color::Red;
    }

    long long steps_count = 0;
    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }
        steps_count++;
        if(steps_count % 400 == 0)
        {
            auto old_count = lines.size();
            lines.resize(2 * old_count);
            std::copy_n(lines.begin(), old_count, lines.begin() + old_count);
        }

        for(auto &line : lines)
        {
            line[0].position = line[1].position;
            line[1].position += {
                // static_cast<float>(distr(gen)),
                // static_cast<float>(distr(gen)),
                static_cast<float>(distr(gen)) * step,
                static_cast<float>(distr(gen)) * step
            };
            // std::cout << static_cast<float>((rand() % 2) - 1) * step << ' ';
            window.draw(line);
        }
        window.draw(back);

        window.display();
        // window.clear();
    }
}