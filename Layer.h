#pragma once

class Layer {

public:
	virtual void Draw() = 0;
	//virtual void Update() = 0;
protected:
	virtual ~Layer() {}
};