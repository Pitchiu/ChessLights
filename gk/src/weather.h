#pragma once
#include <glm/glm.hpp>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WeatherConditionsController
{
	const glm::vec4 backgroundColors[4] = {
		{0.94f, 0.97f, 0.96f, 1.0f},
		{0.52f, 0.81f, 0.92f, 1.0f},
		{0.99f, 0.38f, 0.32f, 1.0f},
		{0.05f, 0.08f, 0.27f, 1.0f}
	};

	const int fullCycleSeconds = 40000;
	const int partialCycleSeconds = 10000;

public:
	enum TimeOfDay
	{
		Morning,
		Afternoon,
		Evening,
		Night
	};

	WeatherConditionsController()
	{
		start_time = std::chrono::steady_clock::now();
	}

	void updateTime()
	{
		int secondsInCycle = (elapsedMiliSeconds() % fullCycleSeconds);
		timeOfDay = (TimeOfDay)(secondsInCycle / partialCycleSeconds);
		mixFactor = (float)(secondsInCycle - timeOfDay * partialCycleSeconds)/(float)partialCycleSeconds;
	}

	glm::vec4 getBackgroundColor()
	{
		glm::vec4 c1 = backgroundColors[timeOfDay];
		glm::vec4 c2 = backgroundColors[(int)(timeOfDay + 1) % 4];
		return c1 * (1 - mixFactor) + c2 * mixFactor;
	}


private:
	float mixFactor = 0.0f;
	TimeOfDay timeOfDay = Morning;
	std::chrono::steady_clock::time_point start_time;
	int elapsedMiliSeconds() {
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		std::chrono::milliseconds elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
		return elapsed_milliseconds.count();
	}
};