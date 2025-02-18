#pragma once
#ifndef RAY_H
#define RAY_H
#include <ostream>
#include <glm/gtx/io.hpp>

class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 dir_inv;

public:
	Ray(const glm::vec3& origin, const glm::vec3& direction, const double t = 0.0f)
		: origin(origin), direction(direction)
	{
		dir_inv = glm::vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
	}

};


#endif
