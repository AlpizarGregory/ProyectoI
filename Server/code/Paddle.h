/**
 * @file paddle.h
 * @version 1.0
 * @date 30/09/2021
 * @author Alpizar Gregory / Axel Cordero
 * @title objeto Jugador
 * @brief golpea la bola y lo rebota con direccion
 */
#ifndef BREAKOUT_PADDLE_H
#define BREAKOUT_PADDLE_H

#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
public:
    float speed = 1000.f;
    sf::RectangleShape picture;
    /**
     * @brief Carga todos los elementos del juego
     */
    void initiate();
    /**
     * @brief Setea el tamano del paddle
     * @param width
     * @param height
     */
    void setSize(float width, float height);
    /**
     * @brief setea la posicion del paddle en la pantalla
     * @param x
     * @param y
     */
    void setPosition(float x, float y);
};



#endif
