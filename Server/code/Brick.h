#ifndef BREAKOUT_BRICK_H
#define BREAKOUT_BRICK_H
#pragma once
#include <SFML/Graphics.hpp>

class Brick
{
public:

    bool enable = true;
    bool moveLeft = true;
    int brickScore = 1;
    int hp = 1;
    float speed = 0;
    sf::RectangleShape picture;
    void initiate();
    void setSize(float width, float height);
    void setPosition(float x, float y);
    virtual bool hit();
    void scoreChange();
};

#endif
