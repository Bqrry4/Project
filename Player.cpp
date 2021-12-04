#include "Player.h"
#include "SystemTimer.h"
#include "Input.h"



const float Height = 70;
const float TimetoApex = 0.8;

float ySpeed = 2.0f * Height / TimetoApex;
float Gravity = 2.0f * Height / TimetoApex / TimetoApex;

float xSpeed = 238;
float a = 0.05; //Coeficientul de alunecare


void Player::Movement()
{
    float dt = SystemTimer::GetInstance()->GetDt();
    
#pragma region Run/Walk

    __int8 movement = (Input::GetInstance()->KeyState(SDL_SCANCODE_RIGHT) - Input::GetInstance()->KeyState(SDL_SCANCODE_LEFT));
    
    vx = a * movement * xSpeed + (1 - a) * vx;
    if (movement == 1) //Right
    {
        ObjState = (Uint16)PlayerState::Run;
        flip = SDL_FLIP_NONE;
        if (colideRight) { vx = 0; }
    }
    if (movement == 0) //Repaos
    {
        ObjState = (Uint16)PlayerState::Repaos;
    }
    if (movement == -1) //Left
    {
        ObjState = (Uint16)PlayerState::Run;
        flip = SDL_FLIP_HORIZONTAL;
        if (colideLeft) { vx = 0; }
    }

    hitbox.x += vx * dt;

#pragma endregion


#pragma region Jump

    //SDL_Log("%d", colideBelow);
    if (colideBelow)
    {
        vy = 0;
    }

    if (Input::GetInstance()->KeyState(SDL_SCANCODE_UP) && colideBelow)
    {
        vy = -1 * ySpeed;
        colideBelow = false;
        ObjState = (Uint16)PlayerState::Jump;
    }



    if (!colideBelow) //Fast grounding when key realeased
    {
        vy += (!Input::GetInstance()->KeyState(SDL_SCANCODE_UP) + Input::GetInstance()->KeyState(SDL_SCANCODE_DOWN) + 1) * Gravity/2 * dt;
    }

    hitbox.y += vy * dt;


#pragma endregion


   /* SDL_Log("%f %f", hitbox.x / 32, hitbox.y / 32);*/

}

void Player::Update()
{
    Movement();
}

void Player::Atack()
{

}
//void Player::Draw()
//{
//    frame.count = (SDL_GetTicks() / frame.AnimSpeed) % frame.aFrames;
//    TextureManager::GetInstance()->Draw(type, hitbox.x, hitbox.y, hitbox.w, hitbox.h, (int)State, frame.count, flip);
//}