#include "ShortRangeNPC.h"
#include "SystemTimer.h"

void ShortRangeNPC::Atack()
{
	if (AtackDT < 250) // Atack delay 0.25 sec
	{
		AtackDT += SystemTimer::GetInstance()->GetDt() * 1000;
		return;
	}
	if (AtackIntention)
	{
		if (ObjState != (Uint16)NPCState::Atack)
		{
			ObjState = (Uint16)NPCState::Atack;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				AMode = false;
				ObjState = (Uint16)NPCState::Repaos;
				AtackingMode = false;
				AtackDT = 0;
				AtackFrameTrigger = false;
				AtackIntention = false;

			}
			if (frame.aFrame == frame.States[ObjState] / 2 && !AtackFrameTrigger)
			{
				AtackingMode = true;
				AtackFrameTrigger = true;
			}
			else
			{
				AtackFrameTrigger = false;
			}
		}
	}
}

void ShortRangeNPC::Update()
{
	NPC::Update();
	if (ObjState != (Uint16)NPCState::Dying)
	{
		Atack();
	}

}