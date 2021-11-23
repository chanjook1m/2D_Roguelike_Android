#pragma once
#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED


#include <memory>
#include <array>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Network.hpp>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"
#include "item.h"
#include "wall.h"
#include "animatedGIF.h"
#include "net/client_interface.hpp"
#include <android/log.h>
#include "Chatbox.h"

#define STRESS_TEST true

using namespace std;

#include "Game.hpp"
#include "State.hpp"

class GamePlay : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;

public:
    GamePlay(std::shared_ptr<Context>& context);
    ~GamePlay();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture coinTexture;
    sf::Texture powerUpTexture;
    sf::Texture wallTexture;
    sf::Texture projectileTexture;

     sf::RectangleShape left;
    sf::RectangleShape right;
    sf::RectangleShape up;
     sf::RectangleShape down;
     sf::CircleShape button1;

     sf::Time projectileClockElapsed;
     sf::Time playerCollisionClockElapsed;
     sf::Time aggroClockElapsed;
};
#endif
