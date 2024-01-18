#include <C:\Users\daraem\source\repos\physicsplus\SFML\include\SFML\Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "fisicas.hpp"



int main() {

    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/daraem/source/repos/GDMEP/smoke.png")) {
        std::cout << "Error con la PUTA TEXTURA";
    }

    sf::RenderWindow window(sf::VideoMode(1250, 720), "Barrida de semen");
    sf::Mouse mouse;
    window.setFramerateLimit(60);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::vector<Rectangled> rectangles;

    sf::Clock clock;

    const sf::Vector2f puntoCanon = { 100.f, 335.f };


    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float velocidadCanon = 700.f + (rand() % 350);

        sf::CircleShape canon;
        canon.setRadius(10);
        canon.setOrigin(canon.getRadius(), canon.getRadius());
        canon.setPosition(puntoCanon);
        canon.setFillColor(sf::Color(255, 255, 255));

        sf::Vector2f vectorCanon = static_cast<sf::Vector2f>(mouse.getPosition(window)) - puntoCanon;
        sf::Vector2f uniVCanon = vectorCanon / sqrt((vectorCanon.x * vectorCanon.x) + (vectorCanon.y * vectorCanon.y));

        if (mouse.isButtonPressed(mouse.Left)) {
            Rectangled rectangle;
            rectangle.shape.setSize(sf::Vector2f(30, 30));
            rectangle.shape.setOrigin(15.f, 15.f);
            rectangle.posicionParticula = puntoCanon;
            rectangle.velocidad = velocidadCanon * uniVCanon;
            rectangle.shape.setRotation(rand() * 100.f);
            rectangle.shape.setPosition(rectangle.posicionParticula);
            rectangle.shape.setFillColor(sf::Color(0, 0, 0, 255));
            rectangle.shape.setTexture(&texture);
            rectangles.push_back(rectangle);
        }

        window.clear();
        for (const Rectangled& rectangle: rectangles) {
            window.draw(rectangle.shape);
        }
        window.draw(canon);
        update(rectangles, vectorCanon);
        window.display();
    }

    return 0;
}
