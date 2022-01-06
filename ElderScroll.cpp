#include "ElderScroll.h"
#include "SystemTimer.h"

void ElderScroll::Update()
{
	float dt = SystemTimer::GetInstance()->GetDt();
	if (!middlePosition)
	{
		middlePosition = hitbox.y;
	}
	if (abs(hitbox.y - middlePosition) > abs(vy)/2)
	{
		vy *= -1;
	}

	hitbox.y += vy * dt;
}