#include "Player.h"
#include "SystemTimer.h"
#include "Input.h"


const float Height = 70;
const float TimetoApex = 0.4;

float ySpeed = 2.0f * Height / TimetoApex;
float Gravity = 2.0f * Height / TimetoApex / TimetoApex;

float xSpeed = 238;
float a = 0.1; //Coeficientul de alunecare


void Player::Movement()
{
    float dt = SystemTimer::GetInstance()->GetDt();
    
#pragma region Run/Walk

    __int8 movement = (Input::GetInstance()->KeyState(Key_Right_id) - Input::GetInstance()->KeyState(Key_Left_id));
    
    vx = a * movement * xSpeed + (1 - a) * vx;
    if (movement == 1) //Right
    {
        ObjState = (Uint16)PlayerState::Run;
        flip = SDL_FLIP_NONE;
    }
    if (movement == 0) //Repaos
    {
        ObjState = (Uint16)PlayerState::Repaos;
    }
    if (movement == -1) //Left
    {
        ObjState = (Uint16)PlayerState::Run;
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (vx > 0)
    {
        if (collide.Right) { vx = 0; }
    }
    else
    {
        if (collide.Left) { vx = 0; }
    }

    hitbox.x += vx * dt;

#pragma endregion


#pragma region Jump

    if (collide.Below)
    {
        vy = 0;
    }

    if (Input::GetInstance()->KeyState(Key_Up_id) && collide.Below)
    {
        vy = -1 * ySpeed;
        collide.Below = false;
        ObjState = (Uint16)PlayerState::Jump;
    }


    if (collide.Above)
    {
        vy = 0;
        collide.Above = false;
    }

    if (!collide.Below) //Fast grounding when key realeased
    {
        vy += (!Input::GetInstance()->KeyState(Key_Up_id) + Input::GetInstance()->KeyState(Key_Down_id) + 1) * Gravity * dt;
    }

    hitbox.y += vy * dt;


#pragma endregion


   /* SDL_Log("%f %f", hitbox.x / 32, hitbox.y / 32);*/

}

void Player::Update()
{
    Movement();
    Atack();
    Ability();
}

void Player::Atack()
{
    if(Input::GetInstance()->KeyState(Key_Atack_id))
    { 
        ObjState = (Uint16)PlayerState::Atack;
    }
}

void Player::Ability()
{
    if (Input::GetInstance()->KeyState(Key_Ability_id))
    {
        ObjState = (Uint16)PlayerState::Ability;
        std::swap(hitbox.x, hitbox.y);
        //std::swap(vx, vy);
    }
}

