#include "ElderScroll.h"
#include "SystemTimer.h"

int ElderScroll::InstanceCount = 0;


ElderScroll::ElderScroll() : vy(10.0f), middlePosition(0.0f), PickDt(0)
{
	Interact = true;
	++InstanceCount;
}

ElderScroll::~ElderScroll()
{
	--InstanceCount;
}

bool ElderScroll::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{

    if (xmlElem == nullptr)
    {
        xmlElem = root->FirstChildElement("Drop");
        for (int i = 0; i < iObject; ++i)
        {
            xmlElem = xmlElem->NextSiblingElement();
        }
    }

    if (!GObject::Parse(root, iObject, xmlElem)) { return false; }

    return true;
}

bool ElderScroll::CanBePicked()
{
	return PickDt > 500;
}

void ElderScroll::Update()
{
	float dt = SystemTimer::GetInstance()->GetDt();

	if (PickDt <= 500)
	{
		PickDt += dt * 1000;
	}

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