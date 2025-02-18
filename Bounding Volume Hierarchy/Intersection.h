#pragma once
#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <glm/glm.hpp>

class Material;
class Object;

class Intersection
{
public:
	Intersection()
	{
		happened = false;
		coords = glm::vec3();
		tex_coords = glm::vec2();
		distance = std::numeric_limits<float>::max();
		object = nullptr;
		material = nullptr;
		normal = glm::vec3();
		emit = glm::vec3();

	}

	bool happened;
	glm::vec3 coords;
	glm::vec2 tex_coords;
	float distance;
	Object* object;
	Material* material;
	glm::vec3 normal;
	glm::vec3 emit;

};

#endif
