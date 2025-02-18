#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <random>
#include <glm/vec3.hpp>

static const float PI = 3.1415926535F;
static const float EPSILON = 0.01F;
//if the value goes too low a ray will be obstructed by the object itself which shoots the ray at a high chance
//and will cause the pic way too dim and horizontal stripes

enum class Axis { X, Y, Z };

class Utilities
{
public:
	inline static float GetRandomFloat()
	{
		static std::random_device rd;
		static std::mt19937 generator(rd());
		static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

		return distribution(generator);
	}

	inline static int GetRandomIntBetween(int i1, int i2)
	{
		static std::random_device seed;
		static std::mt19937 generator(seed());
		static std::uniform_int_distribution<int> distribution(i1, i2);

		return distribution(generator);
	}

	inline static float DegreeToRadians(float degree)
	{
		return degree * PI / 180.0f;
	}

	inline static glm::vec3 Min(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}

	inline static glm::vec3 Max(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	inline static void UpdateProgress(float progress)
	{
		int bar_width = 70;

		std::cout << "[";
		int pos = bar_width * progress;
		for (int i = 0; i < bar_width; ++i) {
			if (i < pos) std::cout << "=";
			else if (i == pos) std::cout << ">";
			else std::cout << " ";
		}
		std::cout << "] " << int(progress * 100.0) << " %\r";
		std::cout.flush();
	}

	inline static float clamp(const float& lo, const float& hi, const float& v)
	{
		return std::max(lo, std::min(hi, v));
	}

};


#endif
