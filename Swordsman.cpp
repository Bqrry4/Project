#include "Swordsman.h"
#include "Input.h"
#include "SystemTimer.h"

void Swordsman::Atack()
{
    if(AtackDT < 500) //  Swordsman atack delay 0.5 sec
    {
        AtackDT += SystemTimer::GetInstance()->GetDt() * 1000;
        return;
    }
    if (ObjState != (Uint16)PlayerState::Atack) {

        if (Input::GetInstance().KeyState(Key_Atack_id))
        {
            ObjState = (Uint16)PlayerState::Atack;
            AMode = true;
            frame.aFrame = 0;
        }
    }
    else
    {
        if (frame.aFrame == frame.States[ObjState] - 1)
        {
            AMode = false;
            ObjState = (Uint16)PlayerState::Repaos;
            AtackDT = 0;
            AtackFrameTrigger = false;
        }
        if(frame.aFrame == frame.States[ObjState]/2 && !AtackFrameTrigger)
        {
            AtackingMode = true;
            AtackFrameTrigger = true;
        }
        else
        {
            AtackingMode = false;
        }
    }
}

void Swordsman::Update()
{
    Player::Update();
    Atack();
}