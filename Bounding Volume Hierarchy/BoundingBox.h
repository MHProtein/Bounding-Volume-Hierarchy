#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include <array>
#include <glm/glm.hpp>

enum class Axis;
class Ray;

class BoundingBox
{
	friend BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);
	friend BoundingBox Union(const BoundingBox& box, const glm::vec3& point);

public:
	BoundingBox();
	BoundingBox(glm::vec3 min_vertex, glm::vec3 max_vertex);

	glm::vec3 Diagonal() const { return max_vertex - min_vertex; }
	glm::vec3 Centroid() const { return (min_vertex + max_vertex) / 2.0f; }
	glm::vec3 operator[](uint32_t index) const { return index == 0 ? min_vertex : max_vertex; }

	//axis where the box stretches the most on 
	Axis MaxExtent() const;
	float SurfaceArea() const;
	glm::vec3 Offset(const glm::vec3& p) const;
	bool Inside(const BoundingBox& box) const;

	bool Intersect(const Ray& ray, const std::array<uint32_t, 3>& dir_is_neg) const;

private:
	glm::vec3 min_vertex;
	glm::vec3 max_vertex;
};

BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);

BoundingBox Union(const BoundingBox& box, const glm::vec3& point);

#endif
