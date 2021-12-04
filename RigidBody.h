#pragma once


class RigidBody {

	float mass;

	float dx;
	float dy;

	float vx = 0;
	float vy = 0;
public:
	RigidBody(float mass) : mass(mass)
	{
		dx = 0;
		dy = 0;
	}
	inline float Dx() { return dx; };
	inline float Dy() { return dy; };
	inline float Vx() { return vx; };
	inline float Vy() { return vy; };

	void ApplyForceX(float Force);
	void ApplyForceY(float Force);

};