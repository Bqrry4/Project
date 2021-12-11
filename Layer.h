#pragma once

class Layer {

public:
	virtual void Draw(SDL_Point* CameraTranslate) = 0;
	//virtual void Update() = 0;
protected:
	virtual ~Layer() {}
};