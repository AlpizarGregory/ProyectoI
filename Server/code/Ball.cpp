#include "Ball.h"

void Ball::initiate()
{
    picture.setFillColor(sf::Color::White);
}
void Ball::setSize(float radius)
{
    picture.setRadius(radius);
    picture.setOrigin(radius / 2.f, radius / 2.f);
}
void Ball::setPosition(float x, float y) {
    picture.setPosition(sf::Vector2f(x, y));
}