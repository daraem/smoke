#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <numeric>
#include <vector>
#include <windows.h>

struct Rectangled {
    sf::RectangleShape shape;
    sf::Vector2f velocidad;
    sf::Vector2f posicionParticula;
    float opacity = 50 + (rand() % 205);
};

bool collision = false;

void checkCollision(Rectangled& rectangles) {
    float window_width = 1250;
    float window_height = 720;
    float coef = 1.f;

    sf::Vector2f suelo = { static_cast<float>(window_width), static_cast<float>(window_height)};


    sf::Vector2f distancia = suelo - rectangles.posicionParticula;
    float ModDist = sqrt(distancia.x * distancia.x + distancia.y * distancia.y);

    if (distancia.y < rectangles.shape.getSize().x/2) {
        rectangles.posicionParticula.y = suelo.y - (rectangles.shape.getSize().x / 2);
        rectangles.velocidad *= coef;
    }

    if (distancia.x < rectangles.shape.getSize().x/2) {
        rectangles.posicionParticula.x = suelo.x - (rectangles.shape.getSize().x / 2);
    }

    if (distancia.x > window_width - (rectangles.shape.getSize().x /2)) {
        rectangles.posicionParticula.x = (rectangles.shape.getSize().x / 2);
    }

    if (distancia.y > window_height - (rectangles.shape.getSize().x / 2)) {
        rectangles.posicionParticula.y = (rectangles.shape.getSize().x / 2);
    }
}

void bolasSeparadas(std::vector<Rectangled>& rectangles) {
    const float responseCoef = 4.f;

    for (uint64_t i = 0; i < rectangles.size(); ++i) {
        Rectangled object1 = rectangles[i];

        for (uint64_t k{ i + 1 }; k < rectangles.size(); ++k) {
            Rectangled object2 = rectangles[k];

            sf::Vector2f distancia = (object1.posicionParticula - object2.posicionParticula);
            float SemiModuloDist = distancia.x * distancia.x + distancia.y * distancia.y;
            float minDist = 120 + 120;

            if (SemiModuloDist < minDist * minDist) {
                float ModuloDist = sqrt(SemiModuloDist);
                sf::Vector2f uniDist = distancia / ModuloDist;



                const float delta = 0.5f * responseCoef * (ModuloDist - minDist);


                collision = true;
            }
            else {
                collision = false;
            }
            rectangles[i] = object1;
            rectangles[k] = object2;
        }
    }
}


void vanishingEffect(std::vector<Rectangled>& rectangles) {
    for (uint64_t i = 0; i < rectangles.size(); ++i) {
        sf::Vector2f incrementto = rectangles[i].shape.getSize();

        rectangles[i].shape.setFillColor(sf::Color(128, 128, 128, rectangles[i].opacity));
        if (rectangles[i].opacity > 0) {
            rectangles[i].opacity -= 0.05f;
            if (incrementto.x < (125.f)) {
                incrementto += {0.15f, 0.15f};
                rectangles[i].shape.setSize(incrementto);;
            }
        }else if (rectangles[i].opacity <= 0.f) {
            rectangles.erase(rectangles.begin() + i);
        }

    }
}

void updatePosition(float dt, std::vector<Rectangled>& rectangles, sf::Vector2f vCanon) {
    sf::Vector2f uniVCanon = vCanon / sqrt((vCanon.x * vCanon.x) + (vCanon.y * vCanon.y));

        for (uint64_t i = 0; i < rectangles.size(); ++i) {
            if (rectangles[i].velocidad.x > 1.f) {
                rectangles[i].velocidad.x -= 1.f;
            }

            if (rectangles[i].velocidad.y > 1.f) {
                rectangles[i].velocidad.y -= 0.7f;
            }
            else if (rectangles[i].velocidad.y < 1.f) {
                rectangles[i].velocidad.y += 0.7f;
            }
            rectangles[i].posicionParticula = rectangles[i].posicionParticula + rectangles[i].velocidad * dt;

            checkCollision(rectangles[i]);
            rectangles[i].shape.setPosition(rectangles[i].posicionParticula);
        }
}

void update(std::vector<Rectangled>& rectangles, sf::Vector2f vCanon) {
    float dt = 1.0f / 60.0f;
    float sub_steps = 16;
    float sub_dt = dt / sub_steps;

    for (uint32_t i(sub_steps); i--;) {
        updatePosition(sub_dt, rectangles, vCanon);
        vanishingEffect(rectangles); 
    }
}