#include "Paddle.h"
void Paddle::initiate()
{
    picture.setFillColor(sf::Color::White);
}
void Paddle::setSize(float width, float height)
{
    picture.setSize(sf::Vector2f(width, height));
    picture.setOrigin(picture.getSize() / 2.f);
}
void Paddle::setPosition(float x, float y)
{
    picture.setPosition(sf::Vector2f(x, y));
}