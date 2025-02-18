#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include <glm/vec3.hpp>

class Ray;
class BoundingBox;
class Intersection;

class Object
{
public:
	Object(){ }
	virtual ~Object(){ }
	virtual float GetArea() const = 0;
					///return intersection and the pdf of sample
	virtual std::tuple<Intersection, float> Sample() const = 0;
	virtual BoundingBox GetBoundingBox() const = 0;
	virtual Intersection GetIntersection(const Ray& ray) = 0;
	virtual std::string GetName() const = 0;
	virtual glm::vec3 GetEmission() const = 0;
	virtual bool HasEmission() const = 0;
};

#endif
