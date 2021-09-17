#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>



using namespace sf;
using namespace std;

RenderWindow window;

Font font;
Text gameOverText;
Text lifeText;
Text scoreText;

// Variables no fijas
#include <vector>
#include <math.h>

#include <cstdlib>

#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

const float pi = 3.14159f;

Clock gameClock;
float deltaTime;

float frameWidth = 800;
float frameHeight = 800;

bool playing = false;
bool gameOver = false;
bool win = false;

int life = 3;
int level = 0;
int score = 0;
int combo = 0;

const float startPosX = 55;
const float startPosY = 70;

Paddle paddle;
Ball ball;

Texture textureBall;
RectangleShape background;
Texture textureBack;
Texture texturePaddle;
Texture textureBrick;

vector<Brick*> bricks;

void Initiate();
void Reset();
void Update();
void Render();
void HandleInput();
void loadLevel(int level);

bool BallLeft(RectangleShape rect);
bool BallRight(RectangleShape rect);
bool BallUp(RectangleShape rect);
bool BallBottom(RectangleShape rect);

// Fin de variables

int main() {

    window.create(VideoMode(frameWidth,frameHeight), "Breakout");
    Initiate();
    loadLevel(0);

    while (window.isOpen()) {
        deltaTime = gameClock.restart().asSeconds();
//        HandleInput();

//        if (playing&&!gameOver&&!win) {
            Update();
//        }

//        Render();
    }

    return EXIT_SUCCESS;
}

void Initiate() {
    textureBall.loadFromFile("ball.png");
    textureBack.loadFromFile("back.png");
    texturePaddle.loadFromFile("paddle.png");
    textureBrick.loadFromFile("brick.png");

    background.setSize(Vector2f(frameWidth, frameHeight));
    background.setPosition(0, 0);
    background.setTexture(&textureBack);

    lifeText.setFont(font);
    lifeText.setCharacterSize(20);
    lifeText.setPosition(620, frameHeight-30);
    lifeText.setString("life:"+ std::to_string(life));

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(35);
    gameOverText.setPosition(100, 400);
    gameOverText.setString("");

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(80, frameHeight - 30);
    scoreText.setString("score:"+ std::to_string(score));
}
void Reset(){
    ball.setPosition(paddle.picture.getPosition().x, paddle.picture.getPosition().y - paddle.picture.getSize().y / 2 - ball.picture.getRadius());
    ball.angle = (270 + std::rand() % 60 - 30) * 2 * pi / 360;
}

void Update(){
    if (ball.angle < 0) {
        ball.angle += 2 * pi;
    }
    ball.angle = fmod(ball.angle, 2 * pi);

    float factor = ball.speed * deltaTime;
    ball.picture.move(cos(ball.angle) * factor, sin(ball.angle) * factor);
    //physics
    //edge

    if (ball.picture.getPosition().y + ball.picture.getRadius() > frameHeight) {
        playing = false;
        life--;
        Reset();

    } else if (ball.picture.getPosition().x - ball.picture.getRadius() < 50.f) {
        ball.angle = pi - ball.angle;
        ball.picture.setPosition(ball.picture.getRadius() + 50.1f, ball.picture.getPosition().y);

    } else if (ball.picture.getPosition().x + ball.picture.getRadius() > frameWidth-50) {
        ball.angle = pi - ball.angle;
        ball.setPosition(frameWidth - ball.picture.getRadius() - 50.1f, ball.picture.getPosition().y);

    } else if (ball.picture.getPosition().y - ball.picture.getRadius() < 50.f) {
        ball.angle = -ball.angle;
        ball.setPosition(ball.picture.getPosition().x, ball.picture.getRadius()+50.1f);

    }

    //paddle
    if (BallBottom(paddle.picture)) {
        int dis = ball.picture.getPosition().x - paddle.picture.getPosition().x;

        if (dis > 30 && ball.angle > 1.f / 2.f*pi) {
            ball.angle = ball.angle - pi;

        } else if (dis < -30 && ball.angle < 1.f / 2.f*pi) {
            ball.angle = ball.angle - pi;

        } else {
            ball.angle = -ball.angle;

//            if (ball.angle > 1.f / 2.f)
// ############################ Línea 237 del Github ###################################################
        }
    }
}
void Render(){

};
