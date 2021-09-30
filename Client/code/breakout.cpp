#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <math.h>

#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

using namespace sf;
using namespace std;

RenderWindow window;

Font font;
Text gameOverText;
Text lifeText;
Text scoreText;

//Variables de socket
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

// Variables no fijas
#include <vector>
#include <cstdlib>

const float pi = 3.14159f;

string data = "";

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
int xMousePos = 55;
int ballSpeed = 30;
float xBallPos = 55;
float yBallPos = 737;
float ballAngle = 0;

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
vector<Ball*> balls;

void Initiate();
void Reset();
void Update();
void Render();
void HandleInput();
void loadLevel(int level);
void setGame();

bool BallLeft(RectangleShape rect);
bool BallRight(RectangleShape rect);
bool BallUp(RectangleShape rect);
bool BallBottom(RectangleShape rect);

// Fin de variables

int main() {

    //Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        return 1;
    }

    // Create a hint structure for the server that's connecting with
    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));

    if (connectRes == -1) {
        return 1;
    }

    // While loop
    char buf [4096];
    string userInput;

    window.create(VideoMode(frameWidth,frameHeight), "Breakout");
    window.setPosition(Vector2i(950,-200));
    Initiate();
    loadLevel(0);

    while (window.isOpen()) {
        deltaTime = gameClock.restart().asSeconds();
        HandleInput();
        userInput = to_string(xMousePos);

        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            cout << "Could not send to server\r\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            cout << "There was an error getting response from server\r\n";

        } else {
            // Display message
            data = string(buf,  bytesReceived);
            setGame();

        }

        if (playing&&!gameOver&&!win) {
            Update();
        }

        Render();

    }

    // Close the socket
    close(sock);

    return EXIT_SUCCESS;
}

void setGame() {
    int index = 0;
    string bytes = "";
    for (auto x : data) {
        if (x == ',') {
            if (index == 0) {
                xBallPos = std::stof(bytes);
            } else if (index == 1) {
                yBallPos = std::stof(bytes);
            } else if (index == 2) {
                ballAngle = std::stof(bytes);
            } else if (index == 3) {
                ballSpeed = std::stoi(bytes);
            }
            bytes = "";
            index ++;
        } else {
            bytes = bytes + x;
        }
    }

}

void Initiate() {
    font.loadFromFile("consola.ttf");

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
    ball.setPosition(xBallPos, yBallPos);
    ball.angle = ballAngle;
}

void Update(){
    ball.angle = ballAngle;

    float factor = ball.speed * deltaTime;
    ball.picture.move(cos(ball.angle) * factor, sin(ball.angle) * factor);

    //physics
    //edge
    if (ball.picture.getPosition().y + ball.picture.getRadius() > frameHeight) {
        playing = false;
        life--;
        Reset();

    } else if (ball.picture.getPosition().x - ball.picture.getRadius() < 50.f) {
        ball.angle = ballAngle;
        ball.picture.setPosition(xBallPos, yBallPos);

    } else if (ball.picture.getPosition().x + ball.picture.getRadius() > frameWidth-50) {
        ball.angle = ballAngle;
        ball.setPosition(xBallPos, yBallPos);

    } else if (ball.picture.getPosition().y - ball.picture.getRadius() < 50.f) {
        ball.angle = ballAngle;
        ball.setPosition(xBallPos, yBallPos);

    }

    //paddle
    if (BallBottom(paddle.picture)) {
        int dis = ball.picture.getPosition().x - paddle.picture.getPosition().x;

        if (dis > 30 && ball.angle > 1.f / 2.f*pi) {
            ball.angle = ballAngle;

        } else if (dis < -30 && ball.angle < 1.f / 2.f*pi) {
            ball.angle = ballAngle;

        } else {
            ball.angle = ballAngle;

            if (ball.angle > 1.f / 2.f*pi && ball.angle < 7.f / 8.f*pi) {
                ball.angle = ballAngle;

           } else if (ball.angle < 1.f / 2.f*pi && ball.angle > 1.f / 8.f*pi) {
                ball.angle = ballAngle;

            } else if (ball.angle <= 1.f / 8.f*pi) {
                ball.angle = ballAngle;

            } else if (ball.angle >= 7.f / 8.f*pi) {
                ball.angle = ballAngle;
            }
        }

        ball.setPosition(xBallPos, yBallPos);

    }

    //bricks
    for (int i = 0; i < bricks.size(); ++i) {
        if (bricks[i] -> enable) {
            if (bricks[i] -> speed != 0.f) {
                if (bricks[i] -> picture.getPosition().x - bricks[i] -> picture.getSize().x / 2 < 50.f)
                    bricks[i] -> moveLeft = false;

                else if (bricks[i] -> picture.getPosition().x + bricks[i] -> picture.getSize().x / 2 > frameWidth - 50.f)
                    bricks[i] -> moveLeft = true;

                if (bricks[i] -> moveLeft)
                    bricks[i] -> picture.move(-bricks[i] -> speed* deltaTime, 0.0f);

                else
                    bricks[i] -> picture.move(bricks[i] -> speed* deltaTime, 0.0f);

            }

            if (BallUp(bricks[i] -> picture)) {
                ball.angle = ballAngle;
                ball.setPosition(xBallPos, yBallPos);
                if (bricks[i]->hit()){
                    int surpTemp = rand() % 6;
                    (bricks[i]->scoreChange());
                    (bricks[i]->surprise(surpTemp));
                }
                else{}


            } else if (BallBottom(bricks[i] -> picture)) {
                ball.angle = ballAngle;
                ball.setPosition(xBallPos, yBallPos);
                if (bricks[i]->hit()) {
                    int surpTemp = rand() % 6;
                    (bricks[i]->scoreChange());
                    (bricks[i]->surprise(surpTemp));
                }

                else{

                }


            } else if (BallLeft(bricks[i] -> picture)) {
                ball.angle = ballAngle;
                ball.setPosition(xBallPos, yBallPos);
                int surpTemp = rand() % 8;
                (bricks[i]->scoreChange());
                (bricks[i]->surprise(surpTemp));


            } else if (BallRight(bricks[i] -> picture)) {
                ball.angle = ballAngle;
                ball.setPosition(xBallPos, yBallPos);
                int surpTemp = rand() % 8;
                (bricks[i]->scoreChange());
                (bricks[i]->surprise(surpTemp));
          }
        }
    }

    if (life <= 0) {
        gameOver = true;
        gameOverText.setString("Game Over, press \"Enter\" restart");
    }

    int count = 0;

    for (int i = 0; i < bricks.size(); ++i) {
        if (bricks[i] -> enable && bricks[i] -> hp < 4)
            count++;

    }

    if (count <= 0) {
        win = true;
        ball.speed = ballSpeed;
        gameOverText.setString("Win! press \"Enter\" to next level");

    }

    lifeText.setString("Life: " + to_string(life));
    scoreText.setString("Score: " + to_string(score));

}
void Render() {
    window.clear(sf::Color::Black);
    window.draw(background);
    window.draw(paddle.picture);
    window.draw(ball.picture);

    for (int i = 0; i < bricks.size(); ++i) {
        if (bricks[i]->enable) {
            if (bricks[i]->hp == 1) {
                bricks[i]->picture.setTexture(&textureBrick);
                bricks[i]->picture.setFillColor(Color(49, 107, 131, 255));
            } else if (bricks[i]->hp == 2) {
                bricks[i]->picture.setTexture(&textureBrick);
                bricks[i]->picture.setFillColor(Color(109, 130, 153, 255));
            }
            else if (bricks[i]->hp == 3) {
                    bricks[i]->picture.setTexture(&textureBrick);
                    bricks[i]->picture.setFillColor(Color(140, 161, 165, 255));
            } else {
                bricks[i]->picture.setTexture(&textureBrick);
                bricks[i]->picture.setFillColor(Color(213, 191, 191, 255));
            }
            window.draw(bricks[i]->picture);

        }

    }
    window.draw(lifeText);
    window.draw(gameOverText);
    window.draw(scoreText);
    window.display();

}

void HandleInput() {
    Event event;

    while (window.pollEvent(event)) {
        if(event.type == Event::Closed) {
            window.close();

            for (int i = 0; i < bricks.size(); ++i) {
                delete bricks[i];
                bricks[i] = nullptr;

            }

            bricks.clear();

        } else if (event.type == Event::MouseMoved && !gameOver && !win) {
            xMousePos = Mouse::getPosition(window).x;
            if (Mouse::getPosition(window).x < (frameWidth - 100.f) && Mouse::getPosition(window).x > 100.f) {
                paddle.picture.setPosition(Vector2f(event.mouseMove.x, paddle.picture.getPosition().y));

            }
            if (!playing) {
                ball.picture.setPosition(xBallPos, yBallPos);

            }
        }
    }

    if (!gameOver) {
        if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) &&
                (paddle.picture.getPosition().x - paddle.picture.getSize().x / 2.f > 50.f)) {
            paddle.picture.move(-paddle.speed * deltaTime, 0.f);

        } if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) &&
                (paddle.picture.getPosition().x + paddle.picture.getSize().x / 2.f < frameWidth - 50.f)) {
            paddle.picture.move(paddle.speed * deltaTime, 0.f);

        }
        if (Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left)) {
            playing = true;

        }
        if (!playing) {
            ball.picture.setPosition(xBallPos, yBallPos);

        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Return)) {
        if (gameOver) {
            life = 3;
            gameOver = false;
            score = 0;
            loadLevel(level);

        } else if (win) {
            win = false;
            level = (level + 1) % 3;
            loadLevel(level);

        }
    } else if (Keyboard::isKeyPressed(Keyboard::Num1)) {
        level = 0;
        loadLevel(level);

    } else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
        level = 1;
        loadLevel(level);

    } else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
        level = 2;
        loadLevel(level);

    }
}

void loadLevel(int level) {
    playing = false;
    gameOver = false;

    gameOverText.setString("");

    paddle.initiate();
    paddle.setSize(150,35);
    paddle.setPosition(frameWidth / 2, frameHeight - paddle.picture.getSize().y);
    paddle.picture.setTexture(&texturePaddle);

    ball.initiate();
    ball.setSize(10);
    ball.setPosition(xBallPos, yBallPos);
    ball.angle = ballAngle;
    ball.picture.setTexture(&textureBall);

    for (int i = 0; i < bricks.size(); ++i) {
        delete bricks[i];
        bricks[i] = nullptr;

    }

    bricks.clear();

    if (level == 0) {
        for (int i = 0; i < 10; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2);
            bptr -> hp = 1;
            bptr -> brickScore = 1;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 10; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x, startPosY + 3 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 2;
            bptr -> brickScore = 2;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 10; ++j) {
                Brick* bptr = new Brick;
                bptr -> initiate();
                bptr -> setSize(70, 30);
                bptr ->setPosition(startPosX + bptr -> picture.getSize().x / 2 + j*bptr -> picture.getSize().x, startPosY + 6 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                bptr -> hp = 3;
                bptr -> brickScore = 3;
                bricks.push_back(bptr);

            }
        }

        for (int i = 0; i < 10; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr->picture.getSize().x, startPosY + 9 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 1;
            bptr -> brickScore = 1;
            bptr -> speed = 400;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 4; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x, startPosY + 11 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 99999;
            bptr -> brickScore = 4;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 4; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x + 6*bptr -> picture.getSize().x, startPosY + 11 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 99999;
            bptr -> brickScore = 4;
            bricks.push_back(bptr);

        }
    }

    else if (level == 1) {
        for (int i = 0; i < 5; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x, startPosY + 10 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2 - i*bptr -> picture.getSize().y);
            bptr -> hp = 1;
            bptr -> brickScore = 1;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 5; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x + 5 * bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y + 6 * bptr -> picture.getSize().y);
            bptr -> hp = 1;
            bptr -> brickScore = 1;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                Brick* bptr = new Brick;
                bptr -> initiate();
                bptr -> setSize(70, 30);
                bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j*bptr -> picture.getSize().x + 4 * bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                bptr -> hp = 2;
                bptr -> brickScore = 2;
                bricks.push_back(bptr);

            }
        }

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                Brick* bptr = new Brick;
                bptr -> initiate();
                bptr -> setSize(70, 30);
                bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j*bptr -> picture.getSize().x + 4 * bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y + 4 * bptr -> picture.getSize().y);
                bptr -> hp = 2;
                bptr -> brickScore = 2;
                bricks.push_back(bptr);

            }
        }

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 10; ++j) {
                Brick* bptr = new Brick;
                bptr -> initiate();
                bptr -> setSize(70, 30);
                bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j*bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y + 2 * bptr -> picture.getSize().y);
                bptr -> hp = 1;
                bptr -> brickScore = 1;
                bptr -> speed = 300;
                bricks.push_back(bptr);

            }
        }

        for (int i = 0; i < 4; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x, startPosY + 11 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 99999;
            bptr -> brickScore = 4;
            bricks.push_back(bptr);

        }

        for (int i = 0; i < 4; ++i) {
            Brick* bptr = new Brick;
            bptr -> initiate();
            bptr -> setSize(70, 30);
            bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + i*bptr -> picture.getSize().x + 6 * bptr -> picture.getSize().x, startPosY + 11 * bptr -> picture.getSize().y + bptr -> picture.getSize().y / 2);
            bptr -> hp = 99999;
            bptr -> brickScore = 4;
            bricks.push_back(bptr);

        }
    }

    else if (level == 2) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                int temp = rand() % 5;
                if (temp == 0) {
                    Brick* bptr = new Brick;
                    bptr -> initiate();
                    bptr -> setSize(70, 30);
                    bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j* bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                    bptr -> hp = 1;
                    bptr -> brickScore = 1;
                    bricks.push_back(bptr);

                } else if (temp == 1) {
                    Brick* bptr = new Brick;
                    bptr -> initiate();
                    bptr -> setSize(70, 30);
                    bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j* bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                    bptr -> hp = 2;
                    bptr -> brickScore = 2;
                    bricks.push_back(bptr);

                } else if (temp == 2) {
                    Brick* bptr = new Brick;
                    bptr -> initiate();
                    bptr -> setSize(70, 30);
                    bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j* bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                    bptr -> hp = 99999;
                    bptr -> brickScore = 4;
                    bricks.push_back(bptr);

                } else if (temp == 3) {
                    Brick* bptr = new Brick;
                    bptr -> initiate();
                    bptr -> setSize(70, 30);
                    bptr -> setPosition(startPosX + bptr -> picture.getSize().x / 2 + j* bptr -> picture.getSize().x, startPosY + bptr -> picture.getSize().y / 2 + i*bptr -> picture.getSize().y);
                    bptr -> hp = 1;

                    bptr -> brickScore = 1;
                    bptr -> speed = 300;
                    bricks.push_back(bptr);

                }
            }
        }
    }
}
bool BallLeft(RectangleShape rect)
{
    if (ball.picture.getPosition().x + ball.picture.getRadius() > rect.getPosition().x - rect.getSize().x / 2 &&
            ball.picture.getPosition().x + ball.picture.getRadius() < rect.getPosition().x + rect.getSize().x / 2 &&
            ball.picture.getPosition().y + ball.picture.getRadius() >= rect.getPosition().y - rect.getSize().y / 2 &&
            ball.picture.getPosition().y - ball.picture.getRadius() <= rect.getPosition().y + rect.getSize().y / 2)
        return true;
    else
        return false;
}
bool BallRight(RectangleShape rect)
{
    if (ball.picture.getPosition().x - ball.picture.getRadius() > rect.getPosition().x - rect.getSize().x / 2 &&
            ball.picture.getPosition().x - ball.picture.getRadius() < rect.getPosition().x + rect.getSize().x / 2 &&
            ball.picture.getPosition().y + ball.picture.getRadius() >= rect.getPosition().y - rect.getSize().y / 2 &&
            ball.picture.getPosition().y - ball.picture.getRadius() <= rect.getPosition().y + rect.getSize().y / 2)
        return true;
    else
        return false;
}
bool BallUp(RectangleShape rect)
{
    if (ball.picture.getPosition().x + ball.picture.getRadius() >= rect.getPosition().x - rect.getSize().x / 2 &&
            ball.picture.getPosition().x - ball.picture.getRadius() <= rect.getPosition().x + rect.getSize().x / 2 &&
            ball.picture.getPosition().y - ball.picture.getRadius() < rect.getPosition().y + rect.getSize().y / 2 &&
            ball.picture.getPosition().y - ball.picture.getRadius() > rect.getPosition().y - rect.getSize().y / 2)
        return true;
    else
        return false;
}
bool BallBottom(RectangleShape rect)
{
    if (ball.picture.getPosition().x + ball.picture.getRadius() >= rect.getPosition().x - rect.getSize().x / 2 &&
            ball.picture.getPosition().x - ball.picture.getRadius() <= rect.getPosition().x + rect.getSize().x / 2 &&
            ball.picture.getPosition().y + ball.picture.getRadius() < rect.getPosition().y + rect.getSize().y / 2 &&
            ball.picture.getPosition().y + ball.picture.getRadius() > rect.getPosition().y - rect.getSize().y / 2)
        return true;
    else
        return false;
}


void Brick::scoreChange() {
    if (brickScore == 1) {
        score = score + 10;
    } else if (brickScore == 2) {
        score = score + 15;
    } else if (brickScore == 3) {
        score = score + 20;
    } else {
        score = score + 30;
    }
}

void Brick::surprise(int surpTemp) {
    if (surpTemp == 0) {
        ball.speed = ballSpeed;
    }
    else if (surpTemp == 1){
        ball.speed = ballSpeed;
    }
    else if (surpTemp == 2){
        paddle.setSize(100,35);
    }
    else if(surpTemp == 3){
        paddle.setSize(200,35);
    }else if(surpTemp == 4){
        Ball* balls = new Ball;
//        balls -> ball.initiate();
    }
    else{

    }}