#ifndef BREAKOUT_PADDLE_H
#define BREAKOUT_PADDLE_H

#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
public:
    float speed = 1000.f;
    sf::RectangleShape picture;
    void initiate();
    void setSize(float width, float height);
    void setPosition(float x, float y);
};



#endif
