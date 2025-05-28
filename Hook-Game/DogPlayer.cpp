#include "DogPlayer.h"

DogPlayer::DogPlayer()
{
    loadAnimations();
}

DogPlayer::~DogPlayer()
{

}

void DogPlayer::loadAnimations()  
{  
    Animation idle;  

    if (idle.texture.loadFromFile("Assets/Player/Textures/dog_idle_sheet.png"))  
    {  
        idle.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));  
        idle.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));  
        idle.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));  
        idle.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));  

        idle.animationSpeed = 0.2f;  
        this->Animations[Player::AnimationStates::IDLE] = idle;
        this->Animations[Player::AnimationStates::CLIMBING] = idle;

    }  

    Animation run;

    if (run.texture.loadFromFile("Assets/Player/Textures/dog_run_sheet.png"))
    {
        run.frames.push_back(sf::IntRect({ 0, 0 }, { 48, 48 }));
        run.frames.push_back(sf::IntRect({ 1 * 48, 0 }, { 48, 48 }));
        run.frames.push_back(sf::IntRect({ 2 * 48, 0 }, { 48, 48 }));
        run.frames.push_back(sf::IntRect({ 3 * 48, 0 }, { 48, 48 }));
        run.frames.push_back(sf::IntRect({ 4 * 48, 0 }, { 48, 48 }));
        run.frames.push_back(sf::IntRect({ 5 * 48, 0 }, { 48, 48 }));

        run.animationSpeed = 0.1f;

        this->Animations[Player::AnimationStates::RUNNING] = run;
        
    }

    Animation jump;

    if (jump.texture.loadFromFile("Assets/Player/Textures/dog_run_sheet.png"))
    {
        jump.frames.push_back(sf::IntRect({ 4 * 48, 0 }, { 48, 48 }));
        jump.frames.push_back(sf::IntRect({ 5 * 48, 0 }, { 48, 48 }));

        jump.animationSpeed = 0.25f;

        this->Animations[Player::AnimationStates::JUMPING] = jump;
        this->Animations[Player::AnimationStates::BOUNCING] = jump;
    }

}