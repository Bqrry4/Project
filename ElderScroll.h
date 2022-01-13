#pragma once
#include "GObject.h"

class ElderScroll : public GObject
{
	float vy;
	float middlePosition;

	int PickDt;
public:
	static int InstanceCount;

	ElderScroll();
	~ElderScroll();

	ElderScroll(const ElderScroll&) = default;
	bool CanBePicked();

	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);
	void Update();
};

