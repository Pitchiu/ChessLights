#pragma once
#include <glm/glm.hpp>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ConditionsController
{
private:
	enum TimeOfDay
	{
		Morning,
		Afternoon,
		Evening,
		Night
	};

	float mixFactor = 0.0f;
	TimeOfDay timeOfDay = Morning;
	std::chrono::steady_clock::time_point start_time;

	int elapsedMiliSeconds(std::chrono::steady_clock::time_point& start) {
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		std::chrono::milliseconds elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start);
		return elapsed_milliseconds.count();
	}

	//const glm::vec4 backgroundColors[4] = {
	//	{0.94f, 0.97f, 0.96f, 1.0f},
	//	{0.52f, 0.81f, 0.92f, 1.0f},
	//	{0.99f, 0.38f, 0.32f, 1.0f},
	//	{0.05f, 0.08f, 0.27f, 1.0f}
	//};

	const glm::vec3 backgroundColors[4] = {
	{0.6f, 0.8f, 1.0f},
	{0.53f, 0.81f, 0.98f},
	{1.0f, 0.6f, 0.4f},
	{0.0f, 0.0f, 0.0f}
	};

	const glm::vec3 ambients[4] = {
	{0.1f, 0.1f, 0.2f},
	{0.2f, 0.2f, 0.2f},
	{0.1f, 0.1f, 0.2f},
	{0.0f, 0.0f, 0.0f},
	};

	const glm::vec3 diffuses[4] = {
	{0.8f, 0.8f, 0.7f},
	{1.0f, 1.0f, 0.9f},
	{0.9f, 0.7f, 0.6f},
	{0.05f, 0.05f, 0.1f},
	};

	const glm::vec3 speculars [4] = {
	{0.1f, 0.1f, 0.1f},
	{0.5f, 0.5f, 0.5f},
	{0.1f, 0.1f, 0.1f},
	{0.0f, 0.0f, 0.0f},
	};



	const int fullCycleSeconds = 20000;
	const int partialCycleSeconds = 5000;

	const int fullFogChange = 4000;
	const float fogMax = 0.1f;
	std::chrono::steady_clock::time_point fogChangeStart;

	float currentFogValue = 0.0f;
	float startChanging = 0.0f;
	bool fogEnabled = false;

	void updateFog()
	{
		if (fogEnabled && currentFogValue < fogMax)
		{
			currentFogValue = startChanging + 
				fogMax*((float)elapsedMiliSeconds(fogChangeStart) / (float)fullFogChange);
			currentFogValue = std::min(currentFogValue, fogMax);
		}
		else if (!fogEnabled && currentFogValue > 0.0f)
		{
			currentFogValue = startChanging -
				fogMax * ((float)elapsedMiliSeconds(fogChangeStart) / (float)fullFogChange);
			currentFogValue = std::max(currentFogValue, 0.0f);
		}
	}

public:

	ConditionsController()
	{
		start_time = std::chrono::steady_clock::now();
	}

	void updateTime()
	{
		int secondsInCycle = (elapsedMiliSeconds(start_time) % fullCycleSeconds);
		timeOfDay = (TimeOfDay)(secondsInCycle / partialCycleSeconds);
		mixFactor = (float)(secondsInCycle - timeOfDay * partialCycleSeconds)/(float)partialCycleSeconds;
		updateFog();

	}

	glm::vec3 getAmbient() const
	{
		glm::vec3 a1 = ambients[timeOfDay];
		glm::vec3 a2 = ambients[(int)(timeOfDay + 1) % 4];
		return (a1 * (1 - mixFactor) + a2 * mixFactor);
	}

	glm::vec3 getDiffuse() const
	{
		glm::vec3 a1 = diffuses[timeOfDay];
		glm::vec3 a2 = diffuses[(int)(timeOfDay + 1) % 4];
		return (a1 * (1 - mixFactor) + a2 * mixFactor);
	}

	glm::vec3 getSpecular() const
	{
		glm::vec3 a1 = speculars[timeOfDay];
		glm::vec3 a2 = speculars[(int)(timeOfDay + 1) % 4];
		return (a1 * (1 - mixFactor) + a2 * mixFactor);
	}

	glm::vec3 getBackgroundColor() const
	{
		glm::vec3 c1 = backgroundColors[timeOfDay];
		glm::vec3 c2 = backgroundColors[(int)(timeOfDay + 1) % 4];
		return (c1 * (1 - mixFactor) + c2 * mixFactor);
	}

	float getFogDensity() const
	{
		return currentFogValue;
	}

	void changeFog()
	{
		fogEnabled = !fogEnabled;
		startChanging = currentFogValue;
		fogChangeStart = std::chrono::steady_clock::now();
	}

	bool objectShaking = false;
	bool lightsOn = false;
};