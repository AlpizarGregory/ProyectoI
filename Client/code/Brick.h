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
    /**
     * @brief Carga todos los elementos del Brick
     */
    void initiate();
    /**
     * @brief Setea el tamano del Brick
     * @param width
     * @param height
     */
    void setSize(float width, float height);
    /**
     * @brief Setea posicion del Brick en la pantalla
     * @param x
     * @param y
     */
    void setPosition(float x, float y);
    /**
     * @brief Metodo hit que le reduce la vida o elimina al brick
     * @return
     */
    virtual bool hit();
    /**
     * @brief Metodo de Puntajes
     */
    void scoreChange();
    /**
     * @brief Metodo que maneja las sorpresas
     * @param surpTemp
     */
    void surprise(int surpTemp);

};

#endif
