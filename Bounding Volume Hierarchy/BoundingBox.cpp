#include "BoundingBox.h"

#include <array>

#include "Ray.h"
#include "Utilities.h"

BoundingBox::BoundingBox()
{
	max_vertex = glm::vec3(std::numeric_limits<float>::lowest(),
							std::numeric_limits<float>::lowest(),
							std::numeric_limits<float>::lowest());
	min_vertex = glm::vec3(std::numeric_limits<float>::max(),
							std::numeric_limits<float>::max(),
							std::numeric_limits<float>::max());
}

BoundingBox::BoundingBox(glm::vec3 min_vertex, glm::vec3 max_vertex)
{
	this->min_vertex = glm::vec3(std::min(min_vertex.x, max_vertex.x),
					std::min(min_vertex.y, max_vertex.y),
					std::min(min_vertex.z, max_vertex.z));

	this->max_vertex = glm::vec3(std::max(min_vertex.x, max_vertex.x),
					std::max(min_vertex.y, max_vertex.y),
					std::max(min_vertex.z, max_vertex.z));
}

Axis BoundingBox::MaxExtent() const
{
	auto dia = Diagonal();
	if (dia.x > dia.y && dia.x > dia.z)
		return Axis::X;
	else if (dia.y > dia.z)
		return Axis::Y;
	else
		return Axis::Z;
}

float BoundingBox::SurfaceArea() const
{
	auto dia = Diagonal();
	return (dia.x * dia.y + dia.y * dia.z + dia.x * dia.z) * 2.0f;
}

glm::vec3 BoundingBox::Offset(const glm::vec3& p) const
{
	glm::vec3 offset = p - min_vertex;

	//Normalization
	if (max_vertex.x - min_vertex.x > 0.0f)
		offset.x /= max_vertex.x - min_vertex.x;

	if (max_vertex.y - min_vertex.y > 0.0f)
		offset.y /= max_vertex.y - min_vertex.y;

	if (max_vertex.z - min_vertex.z > 0.0f)
		offset.z /= max_vertex.z - min_vertex.z;

	return offset;
}

bool BoundingBox::Inside(const BoundingBox& box) const
{
	if (box.max_vertex.x < max_vertex.x && box.max_vertex.y < max_vertex.y && box.max_vertex.z < max_vertex.z);
		else return false;

	if (box.min_vertex.x > min_vertex.x && box.min_vertex.y > min_vertex.y && box.min_vertex.z > min_vertex.z);
		else return false;

	return true;
}

bool BoundingBox::Intersect(const Ray& ray, const std::array<uint32_t, 3>& dir_is_neg) const
{
	float temp = 0.0f;

	float min_time = (operator[](dir_is_neg[0]).x - ray.origin.x) * ray.dir_inv.x;

	temp = (operator[](dir_is_neg[1]).y - ray.origin.y) * ray.dir_inv.y;
	if (temp > min_time)
		std::swap(min_time, temp);

	temp = (operator[](dir_is_neg[2]).z - ray.origin.z) * ray.dir_inv.z;
	if (temp > min_time)
		std::swap(min_time, temp);

	float max_time = (operator[](!dir_is_neg[0]).x - ray.origin.x) * ray.dir_inv.x;

	temp = (operator[](!dir_is_neg[1]).y - ray.origin.y) * ray.dir_inv.y;
	if (temp < max_time)
		std::swap(max_time, temp);

	temp = (operator[](!dir_is_neg[2]).z - ray.origin.z) * ray.dir_inv.z;
	if (temp < max_time)
		std::swap(max_time, temp);

	if (max_time >= 0.0f && max_time > min_time)
			return true;
	return false;
}

BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2)
{
	BoundingBox box;
	box.min_vertex = Utilities::Min(b1.min_vertex, b2.min_vertex);
	box.max_vertex = Utilities::Max(b1.max_vertex, b2.max_vertex);
	return box;
}

BoundingBox Union(const BoundingBox& box, const glm::vec3& point)
{
	BoundingBox new_box;
	new_box.min_vertex = Utilities::Min(box.min_vertex, point);
	new_box.max_vertex = Utilities::Max(box.max_vertex, point);
	return new_box;
}
