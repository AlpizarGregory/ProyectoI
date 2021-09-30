/**
 * @file Ball.h
 * @version 1.0
 * @date 30/09/2021
 * @author Alpizar Gregory / Axel Cordero
 * @title objeto ball y sus metodos
 * @brief logica del balon en el juego
 */
#ifndef BREAKOUT_BALL_H
#define BREAKOUT_BALL_H
#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
public:
    float speed = 500.f;
    float angle;
    sf::CircleShape picture;
    /**
     * @brief Carga elementos de Ball en la pantalla
     */
    void initiate();
    /**
     * @brief Setea el tamano de la bola
     * @param radius
     */
    void setSize(float radius);
    /**
     * @brief setea la posicion de la bola
     * @param x
     * @param y
     */
    void setPosition(float x, float y);
};

#endif
