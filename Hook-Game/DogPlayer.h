#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Level.h"
#include "Rope.h"
#include "Hook.h"
#include "Player.h"

class DogPlayer :
    public Player
{

public:
    DogPlayer();
    ~DogPlayer();


    void loadAnimations() override;
};

