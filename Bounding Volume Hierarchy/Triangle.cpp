#include "Triangle.h"

#include <iostream>
#include <tuple>
#include <glm/gtx/scalar_multiplication.hpp>

#include "BoundingBox.h"
#include "Intersection.h"
#include "Material.h"
#include "Ray.h"
#include "Utilities.h"
Triangle::Triangle()
{

}

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Material* material)
	: v0(v0), v1(v1), v2(v2), e1(v1 - v0), e2(v2 - v0), material(material)
{
	name = "Triangle";
	normal = normalize(glm::cross(e1, e2));
	area = glm::length(glm::cross(e1, e2)) * 0.5f;
}

std::tuple<Intersection, float> Triangle::Sample() const
{
	Intersection pos;
	float pdf;
	float x = std::sqrt(Utilities::GetRandomFloat()); //to bias the two random nums
	float y = Utilities::GetRandomFloat();

	pos.coords = v0 * (1.0f - x) + v1 * x * (1.0f - y) + v2 * (x * y);
	pos.material = this->material;
	pos.normal = this->normal;

	pdf = 1.0f / area; //uniformly sample
	return { pos, pdf };
}


float Triangle::GetArea() const
{
	return area;
}

BoundingBox Triangle::GetBoundingBox() const
{
	return Union(BoundingBox(v0, v1), v2);
}



Intersection Triangle::GetIntersection(const Ray& ray)
{
	//Moller Trumbore Algorithm

	Intersection inter;

	if (glm::dot(ray.direction, normal) > 0.0f)
		return inter;

	glm::vec3 S = ray.origin - v0;
	glm::vec3 S1 = glm::cross(ray.direction, e2);
	glm::vec3 S2 = glm::cross(S, e1);

	float s1_dot_e1_inv = 1.0f / glm::dot(e1, S1);

	if (s1_dot_e1_inv <= 0.0f)
		return inter;

	float b1 = glm::dot(S, S1) * s1_dot_e1_inv;

	if (b1 < 0.0f || b1 > 1.0f)
		return inter;

	float b2 = glm::dot(ray.direction, S2) * s1_dot_e1_inv;

	if (b2 < 0.0f || b2 + b1 > 1.0f)
		return inter;

	float t = glm::dot(e2, S2) * s1_dot_e1_inv;

	if (t < 0.0f)
		return inter;

	inter.happened = true;
	inter.normal = normal;
	inter.material = material;

	//barycentric coordinates
	//interpolation
	inter.coords = (1 - b1 - b2) * v0 + b1 * v1 + b2 * v2;
	inter.distance = t;
	inter.emit = material->GetEmission();
	inter.object = this;

	return inter;
}

std::string Triangle::GetName() const
{
	return name;
}

glm::vec3 Triangle::GetEmission() const
{
	return material->GetEmission();
}

bool Triangle::HasEmission() const
{
	return material->HasEmission();
}

void Triangle::DebugPrint() const
{
	std::cout << name << std::endl;
	std::cout << "v0" << v0 << " ";
	std::cout << "v0" << v0 << " " ;
	std::cout << "v1" << v1 << " ";
	std::cout << "normal" << normal << std::endl;
	std::cout << std::endl;
}
