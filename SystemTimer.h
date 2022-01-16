#pragma once

//const int FPS = 60;

class SystemTimer {

	float last_time;
	float delta_time;

	SystemTimer() : last_time(0), delta_time(0)
	{}
	SystemTimer(const SystemTimer&) = delete;
	SystemTimer& operator=(const SystemTimer&) = delete;
	SystemTimer(SystemTimer&&) = delete;
	SystemTimer& operator=(SystemTimer&&) = delete;

public:
	~SystemTimer() = default;

	inline static SystemTimer& GetInstance() { static SystemTimer instance; return instance; }

	inline float GetDt() { return delta_time; }
	void Ticks();
};