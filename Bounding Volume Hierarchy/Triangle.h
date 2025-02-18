#pragma once
#ifndef TRIANGEL_H
#define TRIANGLE_H
#include <glm/vec3.hpp>

#include "Object.h"

class BoundingBox;
class Ray;
class Material;

class Triangle : public Object
{

public:
	Triangle();
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Material* material);
	std::tuple<Intersection, float> Sample() const override;
	float GetArea() const override;
	BoundingBox GetBoundingBox() const override;
	Intersection GetIntersection(const Ray& ray) override;
	std::string GetName() const override;
	glm::vec3 GetEmission() const override;
	bool HasEmission() const override;
	void DebugPrint() const;

private:
	std::string name;
	glm::vec3 v0, v1, v2; //counter-clockwise
	glm::vec3 e1, e2; //e1 = v1 - v0, e2 = v2 - v0
	glm::vec3 normal;
	Material* material;
	float area;
};

#endif
