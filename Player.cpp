#include "Player.h"
#include "SystemTimer.h"
#include "Input.h"
#include "Game.h"
#include <sstream>

#define Height 70.f
#define TimetoApex 0.4f

Direction operator~(Direction d) { return static_cast<Direction>(~(int)d + 1); }

const float GObject::Gravity = 2.0f * Height / TimetoApex / TimetoApex;

const float ySpeed = 2.0f * Height / TimetoApex;
const float xSpeed = 238;
//float Player::a = 0.1; //Coeficientul de alunecare

bool Player::PlayerDead = false;

Player::Player(SDL_Scancode Key_Up_id, SDL_Scancode Key_Down_id, SDL_Scancode Key_Left_id, SDL_Scancode Key_Right_id, SDL_Scancode Key_Atack_id, SDL_Scancode Key_Ability_Save_id, SDL_Scancode Key_Ability_Load_id, SDL_Scancode Key_Hanging_id) : AnimatedObj(), vx(0), vy(0), CheckPointPosition({ NULL,NULL }), AbilityDT(0), a(0.1f),HangMode(false), HangRange(50), MaxHealth(0), HPrecDT(0.f), SFX(nullptr)
{
    Interact = true;
    collide.Is = true;
    collide.WithOthers = true;

    this->Key_Up_id = Key_Up_id;
    this->Key_Down_id = Key_Down_id;
    this->Key_Left_id = Key_Left_id;
    this->Key_Right_id = Key_Right_id;
    this->Key_Atack_id = Key_Atack_id;
    this->Key_Ability_Save_id = Key_Ability_Save_id;
    this->Key_Ability_Load_id = Key_Ability_Load_id;
    this->Key_Hanging_id = Key_Hanging_id;

}


bool Player::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{
    if (xmlElem == nullptr)
    {
        throw std::string("Invalid Parameters for parsing that object");
    }

    try {
        AnimatedObj::Parse(root, iObject, xmlElem);
    }
    catch (std::string s) { throw s; }

    xmlElem->QueryIntAttribute("HP", &HP);
    xmlElem->QueryIntAttribute("AP", &AP);

    xmlElem->QueryIntAttribute("AtackRange", &AtRange);

    if (HP <= 0 || AP < 0 || AtRange < 0)
        throw std::string("Invalid Object parameters \n");

    HP /= Level::DifficultyAmpl;
    AP /= Level::DifficultyAmpl;


    if (xmlElem->Attribute("sounds"))
    {

        SFX = new __int8[frame.aStates];


        std::string vector = xmlElem->Attribute("sounds");
        std::istringstream stream(vector);
        std::string value;

        for (int i = 0; i < frame.aStates; ++i)
        {
            std::getline(stream, value, ';');
            try
            {
                SFX[i] = std::stoi(value);
            }
            catch (...)
            {
                throw std::string("Invalid state array parameters \n");
            }
        }
    }

    return true;
}

void Player::Movement()
{
    float dt = SystemTimer::GetInstance().GetDt();

    __int8 movement = (Input::GetInstance().KeyState(Key_Right_id) - Input::GetInstance().KeyState(Key_Left_id));
    
    vx = a * movement * xSpeed + (1 - a) * vx;

    if (movement == 1) //Right
    {
        if (ObjState != (Uint16)PlayerState::Jump) {
            ObjState = (Uint16)PlayerState::Run;
        }
        flip = SDL_FLIP_NONE;
        direction = Direction::Right;
    }
    if (movement == 0) //Repaos
    {
        if (ObjState != (Uint16)PlayerState::Jump) {
            ObjState = (Uint16)PlayerState::Repaos;
        }
    }
    if (movement == -1) //Left
    {
        if (ObjState != (Uint16)PlayerState::Jump) {
            ObjState = (Uint16)PlayerState::Run;
        }
        flip = SDL_FLIP_HORIZONTAL;
        direction = Direction::Left;
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
}

void Player::Jump()
{
    float dt = SystemTimer::GetInstance().GetDt();

    if (collide.Below)
    {
        vy = 0;
        AMode = false;
        ObjState = (Uint16)PlayerState::Repaos;
    }
    if(ObjState != (Uint16)PlayerState::Jump){
        if (Input::GetInstance().KeyState(Key_Up_id) && collide.Below)
        {
            vy = -1 * ySpeed;
            collide.Below = false;
            ObjState = (Uint16)PlayerState::Jump;
            AMode = true;
            frame.aFrame = 0;
        }
    }
    if (ObjState == (Uint16)PlayerState::Jump) 
    {
        if (frame.aFrame == frame.States[ObjState] - 1)
        {
            AMode = false;
            ObjState = (Uint16)PlayerState::Repaos;
        }
    }

    if (collide.Above)
    {
        vy = 0;
        collide.Above = false;
    }

    if (!collide.Below) //Fast grounding when key realeased
    {
        vy += (!Input::GetInstance().KeyState(Key_Up_id) + Input::GetInstance().KeyState(Key_Down_id) + 1) * Gravity * dt;
    }

    hitbox.y += vy * dt;

}

void Player::Update()
{
    IsDiyng();
    if (ObjState != (Uint16)PlayerState::Atack && ObjState != (Uint16)PlayerState::Ability && ObjState != (Uint16)PlayerState::Dying) {
        Jump();
        Movement();
        HealthRecovery();
    }
    if(!IsDead())
        Ability();
}

void Player::Draw(const SDL_Point* CameraTranslate)
{
    AnimatedObj::Draw(CameraTranslate);
    if (!IsDead())
        DrawHealthBar();
}

//void Player::Atack()
//{
//
//    //if (ObjState != (Uint16)PlayerState::Atack) {
//
//    //    if (Input::GetInstance()->KeyState(Key_Atack_id))
//    //    {
//    //        ObjState = (Uint16)PlayerState::Atack;
//    //        AtackingMode = true;
//    //        AMode = true;
//    //        frame.aFrame = 0;
//    //    }
//    //}
//    //else
//    //{   
//    //    if(frame.aFrame == frame.States[ObjState] - 1) 
//    //    {
//    //        AMode = false;
//    //        ObjState = (Uint16)PlayerState::Repaos;
//    //    }
//
//    //    AtackingMode = false;
//    // 
//    //}
//}

void Player::Ability()
{
    if (Input::GetInstance().KeyState(Key_Hanging_id) && ObjState != (Uint16)PlayerState::Run)
        HangMode = true;
    else
        HangMode = false;

    if (ObjState != (Uint16)PlayerState::Ability)
    {
        if (Input::GetInstance().KeyState(Key_Ability_Save_id) && collide.Below) //Record Position
        {
            ObjState = (Uint16)PlayerState::Ability;
            AMode = true;
            frame.aFrame = 0;

            if (SFX)
            {
                Sound* sound;
                if (sound = SoundManager::GetInstance().getSound(SFX[(Uint16)PlayerState::Ability]))
                    sound->Play();
            }

            CheckPointPosition = { (int)hitbox.x, (int)hitbox.y };
            //std::swap(hitbox.x, hitbox.y);
            //std::swap(vx, vy);
        }
        if (AbilityDT < 5000) //  Ability delay 5 sec
        {
            AbilityDT += SystemTimer::GetInstance().GetDt() * 1000;
        }
        else {
            if (Input::GetInstance().KeyState(Key_Ability_Load_id) && collide.Below) //Apply Position
            {
                //ObjState = (Uint16)PlayerState::Ability;
                //AMode = true;
                //frame.aFrame = 0;
                AbilityDT = 0;
                if (CheckPointPosition.x != NULL && CheckPointPosition.y != NULL)
                {
                    hitbox.x = CheckPointPosition.x;
                    hitbox.y = CheckPointPosition.y;
                }

            }
        }
    }
    else
    {
        if (frame.aFrame == frame.States[ObjState] - 1)
        {
            AMode = false;
            ObjState = (Uint16)PlayerState::Repaos;
        }
    }
}

void Player::IsDiyng()
{
    if (HP <= 0)
    {
        if (ObjState != (Uint16)PlayerState::Dying)
        {
            ObjState = (Uint16)PlayerState::Dying;
            collide.WithOthers = false;
            AMode = true;
            frame.aFrame = 0;
        }
        else
        {
            if (frame.aFrame == frame.States[ObjState] - 1)
            {
                PlayerDead = true;
            }
        }
    }
}

void Player::DrawHealthBar()
{
    if (!MaxHealth) MaxHealth = HP;

    float ratio = (float)HP / MaxHealth;

    SDL_Texture* texture = TextureManager::FillTransparent(nullptr, { 0xFE, 0, 0, 0 }, { 0, 0, (int)(80 * ratio), 20 });
    TextureManager::Draw(texture, { 0, 0, (int)(80 * ratio), 20 }, { 10, Game::ScreenHeigth() - 30, (int)(80 * ratio), 20 });
    SDL_DestroyTexture(texture);
}

void Player::HealthRecovery()
{
    if (HP < MaxHealth) // 1HP per second
    {
        HPrecDT += SystemTimer::GetInstance().GetDt();

        HP += (int)HPrecDT;
        HPrecDT -= (int)HPrecDT;
    }
}