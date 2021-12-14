#pragma once

const int FPS = 60;


class SystemTimer {

	static SystemTimer* st_Instance;
	float last_time;
	float delta_time;

	SystemTimer() : last_time(0), delta_time(0)
	{}
public:

	~SystemTimer()
	{
		delete st_Instance;
	}

	inline float GetDt() { return delta_time; }
	inline static SystemTimer* GetInstance() { if (!st_Instance) st_Instance = new SystemTimer; return st_Instance; } //Using Singleton
	void Ticks();
};